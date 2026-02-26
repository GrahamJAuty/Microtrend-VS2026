//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPluInfo.h"
//**********************************************************************
#include "SQLRepositoryPluInfo.h"
//**********************************************************************

CSQLRepositoryPluInfo::CSQLRepositoryPluInfo()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::SelectRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByDbNoPluNo{
				SQLRowPlu.GetDbNo(),
				SimpleFormatInt64Value(SQLRowPlu.GetPluNo())
			});
			
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
				RecordSet.SaveToPluInfoRow(SQLRowPlu);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::UpdateRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByDbNoPluNo{
				SQLRowPlu.GetDbNo(),
				SimpleFormatInt64Value(SQLRowPlu.GetPluNo())
			});

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
				RecordSet.LoadFromPluInfoRow(SQLRowPlu);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::InsertRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase, RSParams_PluInfo_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPluInfoRow(SQLRowPlu);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::UpsertRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(SQLRowPlu.GetDbNo(), SQLRowPlu.GetPluNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0)
		{
			Result = InsertRow(SQLRowPlu, pDatabase);
		}
		else
		{
			Result = UpdateRow(SQLRowPlu, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPluInfo::DeleteRow(CSQLRowPluInfo& SQLRowPlu, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByDbNoPluNo{
				SQLRowPlu.GetDbNo(),
				SimpleFormatInt64Value(SQLRowPlu.GetPluNo())
			});

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

CSQLResultInfo CSQLRepositoryPluInfo::RowExists(int nDbNo, __int64 nPluNo, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLRecordSetPluInfo RecordSet(pDatabase,
			RSParams_PluInfo_NormalByDbNoPluNo{
				nDbNo,
				SimpleFormatInt64Value(nPluNo)
			}, 
			TRUE);

		return RecordSet.CheckRowExists();
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

void CSQLRepositoryPluInfo::ImportPluInfo(int nDbNo, CString strFilename)
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
			strCommand.Format("DELETE FROM %s WHERE %s = %d", 
				SQLTableNames::PluInfo, 
				PluInfo::DbNo.Label, 
				nDbNo);

			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase,strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strPluInfo = "";
		while (fileImport.ReadString(strPluInfo) == TRUE)
		{
			CCSV csv(strPluInfo);
			CSQLRowPluInfo RowPluInfo;
			RowPluInfo.SetDbNo(nDbNo);
			RowPluInfo.SetPluNo(csv.GetInt64(0));
			RowPluInfo.SetDescription(csv.GetString(1));
			RowPluInfo.SetPoints(csv.GetInt(2));

			if (nMatched >= 4)
			{
				RowPluInfo.SetDeptNo(csv.GetInt(3));
			}

			if (nMatched >= 5)
			{
				RowPluInfo.SetTaxCode(csv.GetString(4));
			}

			if (nMatched >= 6)
			{
				RowPluInfo.SetModType(csv.GetInt(5));
			}

			if (nMatched >= 7)
			{
				RowPluInfo.SetStampOfferID(csv.GetInt(6));
			}

			InsertRow(RowPluInfo, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryPluInfo::GetPluCountsByStampOfferID(CReportConsolidationMap<CMapKeyInt, CMapDataSortedInt>& arrayPluCounts)
{
	arrayPluCounts.RemoveAll();

	{
		CSQLRecordSetConnectionPool rsCount(NULL);

		CString strSQL = "";
		strSQL.Format("SELECT %s, Count(*) AS PluCount FROM %s GROUP BY %s",
			PluInfo::StampOfferID.Label,
			SQLTableNames::PluInfo,
			PluInfo::StampOfferID.Label);

		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strOfferID = "";
			CString strCount = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strOfferID);
			rsCount.GetFieldValue(nIndex + 1, strCount);

			CMapKeyInt Key(atoi(strOfferID));
			CMapDataSortedInt Data(atoi(strCount));
			arrayPluCounts.Consolidate(Key, Data);

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************

void CSQLRepositoryPluInfo::GetPluListByStampOfferID(int nOfferID, CReportConsolidationArray<CSortedStringByIntAndInt64>& arrayPluItems)
{
	arrayPluItems.RemoveAll();

	{
		CSQLRecordSetConnectionPool rsCount(NULL);

		CString strSQL = "";
		strSQL.Format("SELECT %s, %s, %s FROM %s WHERE %s = %d", 
			PluInfo::DbNo.Label,
			PluInfo::PluNo.Label,
			PluInfo::Description.Label,
			SQLTableNames::PluInfo,
			PluInfo::StampOfferID.Label,
			nOfferID);

		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strDbNo = "";	
			CString strPluNo = "";
			CString strDescription = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strDbNo);
			rsCount.GetFieldValue(nIndex + 1, strPluNo);
			rsCount.GetFieldValue(nIndex + 2, strDescription);

			CSortedStringByIntAndInt64 item;
			item.m_nIntKey = atoi(strDbNo);
			item.m_nInt64Key = _atoi64(strPluNo);
			item.m_strItem = strDescription;
			arrayPluItems.Consolidate(item);

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************

