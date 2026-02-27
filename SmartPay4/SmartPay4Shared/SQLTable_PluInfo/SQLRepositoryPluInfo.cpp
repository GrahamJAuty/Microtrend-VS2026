//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRecordSetPluInfo.h"
//**********************************************************************
#include "SQLRepositoryPluInfo.h"
//**********************************************************************

CSQLRepositoryPluInfo::CSQLRepositoryPluInfo()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::SelectRow(CSQLRowPluInfo& RowPI, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase, 
			RSParams_PluInfo_NormalByPluNo{ FormatInt64Value(RowPI.GetPluNo()) });

		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.SaveToPluInfoRow(RowPI);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::UpdateRow(CSQLRowPluInfo& RowPI, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByPluNo{ FormatInt64Value(RowPI.GetPluNo()) });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.Edit();
				RecordSet.LoadFromPluInfoRow(RowPI);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::InsertRow(CSQLRowPluInfo& RowPI, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPluInfoRow(RowPI);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::UpsertRow(CSQLRowPluInfo& RowPI, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPI.GetPluNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPI, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPI, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::DeleteRow(CSQLRowPluInfo& RowPI, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByPluNo{ FormatInt64Value(RowPI.GetPluNo()) });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == FALSE)
			{
				RecordSet.Delete();
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			else
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::RowExists(__int64 nPluNo, CDatabase* pDatabase)
{
	CSQLRecordSetPluInfo RecordSet(pDatabase,
		RSParams_PluInfo_NormalByPluNo{ FormatInt64Value(nPluNo) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryPluInfo::ImportPluInfo(CString strFilename)
{
	CSSFile fileImport;
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	if (fileImport.ReadString(strHeader) == FALSE)
	{
		return;
	}

	CStringArray Columns;
	Columns.Add("PLUNO");
	Columns.Add("TEXT");
	Columns.Add("POINTS");
	Columns.Add("DEPTNO");
	Columns.Add("TAXCODE");
	Columns.Add("MODTYPE");
	Columns.Add("STMPOFFID");
	Columns.Add("PCTRLID");
	Columns.Add("STMPWGT");
	
	strHeader.MakeUpper();
	CCSV csv(strHeader);

	int nMatched = 0;
	for (; nMatched < Columns.GetSize() && nMatched < csv.GetSize(); nMatched++)
	{
		if (Columns.GetAt(nMatched) != csv.GetString(nMatched))
		{
			break;
		}
	}

	if (nMatched < 3)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CString strCommand = "";
			strCommand.Format("DELETE FROM %s", SQLTableNames::PluInfo);
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strPluInfo = "";
		while (fileImport.ReadString(strPluInfo) == TRUE)
		{
			CCSV csv(strPluInfo);
			CSQLRowPluInfo RowPI;
			RowPI.SetPluNo(csv.GetInt64(0));
			RowPI.SetDescription(csv.GetString(1));
			RowPI.SetPoints(csv.GetInt(2));

			if (nMatched >= 4)
			{
				RowPI.SetDeptNo(csv.GetInt(3));
			}

			if (nMatched >= 5)
			{
				RowPI.SetTaxCode(csv.GetString(4));
			}

			if (nMatched >= 6)
			{
				RowPI.SetModType(csv.GetInt(5));
			}

			if (nMatched >= 8)
			{
				RowPI.SetPCtrlRuleID(csv.GetInt(7));
			}

			if (nMatched >= 9)
			{
				RowPI.SetStampWeight(csv.GetInt(8));
			}

			InsertRow(RowPI, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryPluInfo::GetPluCountsByPCtrlRuleID(CReportConsolidationArray<CSortedIntByInt>& arrayPluCounts)
{
	arrayPluCounts.RemoveAll();

	{
		CSQLRecordSetConnectionPool rsCount(NULL);

		CString strSQL = "";
		strSQL.Format("SELECT %s, Count(*) AS PluCount FROM %s GROUP BY %s",
			PluInfo::PCtrlRuleID.Label,
			SQLTableNames::PluInfo,
			PluInfo::PCtrlRuleID.Label);

		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strRuleID = "";
			CString strCount = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strRuleID);
			rsCount.GetFieldValue(nIndex + 1, strCount);

			CSortedIntByInt item;
			item.m_nKey = atoi(strRuleID);
			item.m_nVal = atoi(strCount);
			arrayPluCounts.Consolidate(item);

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************

void CSQLRepositoryPluInfo::GetPluListByPCtrlRuleID(int nRuleID, CReportConsolidationArray<CSortedStringAndIntByInt64>& arrayPluItems)
{
	arrayPluItems.RemoveAll();

	{
		CSQLRecordSetConnectionPool rsCount(NULL);

		CString strSQL = "";
		strSQL.Format("SELECT %s, %s, %s FROM %s WHERE %s = %d",
			PluInfo::PluNo.Label,
			PluInfo::Description.Label,
			PluInfo::StampWeight.Label,
			SQLTableNames::PluInfo,
			PluInfo::PCtrlRuleID.Label,
			nRuleID);

		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strPluNo = "";
			CString strDescription = "";
			CString strStampWeight = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strPluNo);
			rsCount.GetFieldValue(nIndex + 1, strDescription);
			rsCount.GetFieldValue(nIndex + 2, strStampWeight);

			CSortedStringAndIntByInt64 item;
			item.m_nKey = _atoi64(strPluNo);
			item.m_strVal = strDescription;
			item.m_nVal = atoi(strStampWeight);
			arrayPluItems.Consolidate(item);

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************