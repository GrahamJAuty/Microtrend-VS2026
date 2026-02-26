//**********************************************************************
#include "..\SQLTable_DeptInfo\SQLRecordSetDeptInfo.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLHelpers.h"
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRepositoryDeptInfo.h"
//**********************************************************************

CSQLRepositoryDeptInfo::CSQLRepositoryDeptInfo()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryDeptInfo::SelectRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetDeptInfo RecordSet(pDatabase,
			RSParams_DeptInfo_NormalByDbNoDeptNo{ SQLRowDept.GetDbNo(), SQLRowDept.GetDeptNo() });

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
				RecordSet.SaveToDeptInfoRow(SQLRowDept);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryDeptInfo::UpdateRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetDeptInfo RecordSet(pDatabase,
			RSParams_DeptInfo_NormalByDbNoDeptNo{ SQLRowDept.GetDbNo(), SQLRowDept.GetDeptNo() });

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
				RecordSet.LoadFromDeptInfoRow(SQLRowDept);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryDeptInfo::InsertRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetDeptInfo RecordSet(pDatabase, RSParams_DeptInfo_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromDeptInfoRow(SQLRowDept);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryDeptInfo::DeleteRow(CSQLRowDeptInfo& SQLRowDept, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetDeptInfo RecordSet(pDatabase,
			RSParams_DeptInfo_NormalByDbNoDeptNo{ SQLRowDept.GetDbNo(), SQLRowDept.GetDeptNo() });

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

void CSQLRepositoryDeptInfo::ImportDeptInfo(int nDbNo, CString strFilename)
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
	Columns.Add("DEPTNO");
	Columns.Add("NAME");
	Columns.Add("BONUSTRIP");
	Columns.Add("BONUSTYPE");
	Columns.Add("BONUSPTS");
	Columns.Add("BONUSVAL");
	Columns.Add("BONUSPCNT");
	Columns.Add("GENSPENDBONUS");

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

	if (nMatched < 7)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	if (Server.GetEcrManagerGlobalDeptFlag() == TRUE)
	{
		nDbNo = 1;
	}

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CString strCommand = "";
			strCommand.Format("DELETE FROM %s WHERE %s = %d",
				SQLTableNames::DeptInfo,
				DeptInfo::DbNo.Label,
				nDbNo);

			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strDeptInfo = "";
		while (fileImport.ReadString(strDeptInfo) == TRUE)
		{
			CCSV csv(strDeptInfo);
			CSQLRowDeptInfo RowDeptInfo;
			RowDeptInfo.SetDbNo(nDbNo);
			RowDeptInfo.SetDeptNo(csv.GetInt(0));
			RowDeptInfo.SetName(csv.GetString(1));
			RowDeptInfo.SetLoyaltyBonusTrip(csv.GetDouble(2));
			RowDeptInfo.SetLoyaltyBonusType(csv.GetInt(3));
			RowDeptInfo.SetLoyaltyBonusPoints(csv.GetInt(4));
			RowDeptInfo.SetLoyaltyBonusValue(csv.GetDouble(5));
			RowDeptInfo.SetLoyaltyBonusPercent(csv.GetInt(6));

			if (nMatched > 7)
			{
				RowDeptInfo.SetGeneralSpendBonusFlag(csv.GetBool(7));
			}
			else
			{
				RowDeptInfo.SetGeneralSpendBonusFlag(TRUE);
			}

			InsertRow(RowDeptInfo, NULL);
		}
	}
}

//**********************************************************************

