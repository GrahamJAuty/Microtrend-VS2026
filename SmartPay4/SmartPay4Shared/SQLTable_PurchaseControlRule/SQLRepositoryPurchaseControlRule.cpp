	//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRecordSetPurchaseControlRule.h"
//**********************************************************************
#include "SQLRepositoryPurchaseControlRule.h"
//**********************************************************************

CSQLRepositoryPurchaseControlRule::CSQLRepositoryPurchaseControlRule()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlRule::SelectRow(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlRule RecordSet(pDatabase, 
			RSParams_PurchaseControlRule_NormalByRuleId{ RowPCR.GetRuleID() });

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
				RecordSet.SaveToPurchaseControlRuleRow(RowPCR);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlRule::UpdateRow(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlRule RecordSet(pDatabase,
			RSParams_PurchaseControlRule_NormalByRuleId{ RowPCR.GetRuleID() });

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
				RecordSet.LoadFromPurchaseControlRuleRow(RowPCR);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlRule::InsertRow(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlRule RecordSet(pDatabase,
			RSParams_PurchaseControlRule_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPurchaseControlRuleRow(RowPCR);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlRule::UpsertRow(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPCR, pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPCR, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPCR, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlRule::DeleteRow(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlRule RecordSet(pDatabase,
			RSParams_PurchaseControlRule_NormalByRuleId{ RowPCR.GetRuleID() });

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

CSQLResultInfo CSQLRepositoryPurchaseControlRule::RowExists(CSQLRowPurchaseControlRule& RowPCR, CDatabase* pDatabase)
{
	CSQLRecordSetPurchaseControlRule RecordSet(pDatabase,
		RSParams_PurchaseControlRule_NormalByRuleId{ RowPCR.GetRuleID() }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryPurchaseControlRule::ImportPurchaseControlInfo(CString strFilename)
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
	Columns.Add("PURCHASECTRLID");
	Columns.Add("ENABLE");
	Columns.Add("THRESHOLD");
	Columns.Add("EXPIRY");
	Columns.Add("DESCRIPTION");

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

	if (nMatched < 5)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CString strCommand = "DELETE FROM ";
			strCommand += SQLTableNames::PurchaseControlRules;
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strPurchaseControlInfo = "";
		while (fileImport.ReadString(strPurchaseControlInfo) == TRUE)
		{
			CCSV csv(strPurchaseControlInfo);

			int nRuleID = csv.GetInt(0);
			if ((nRuleID < PurchaseControlStamp::RuleID.Min) || (nRuleID > PurchaseControlStamp::RuleID.Max))
			{
				continue;
			}

			CSQLRowPurchaseControlRule RowPCR;
			RowPCR.SetRuleID(nRuleID);
			RowPCR.SetEnableFlag(csv.GetBool(1));
			RowPCR.SetThreshold(csv.GetInt(2));
			RowPCR.SetExpiryDays(csv.GetInt(3));
			RowPCR.SetDescription(csv.GetString(4));

			InsertRow(RowPCR, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryPurchaseControlRule::GetEnabledRules(CArray<CEnabledPurchaseControlInfo, CEnabledPurchaseControlInfo>& arrayRules)
{
	arrayRules.RemoveAll();

	CSQLRecordSetConnectionPool rsRules(NULL);

	CString strSQL = "";
	strSQL.Format("SELECT %s FROM %s WHERE %s = 1",
		PurchaseControlRule::RuleID.Label,
		SQLTableNames::PurchaseControlRules,
		PurchaseControlRule::Enabled.Label);

	rsRules.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

	while ( rsRules.IsEOF() == FALSE )
	{
		CString strRuleID = "";
		
		short nIndex = 0;
		rsRules.GetFieldValue(nIndex, strRuleID);
		
		CEnabledPurchaseControlInfo info;
		info.m_nRuleID = atoi(strRuleID);
		arrayRules.Add(info);

		rsRules.MoveNext();
	}
}

//**********************************************************************

void CSQLRepositoryPurchaseControlRule::GetRuleName(int nRuleID, int& nLastRuleID, CString& strRuleName, CDatabase* pDatabase)
{
	if (nRuleID != nLastRuleID)
	{
		strRuleName = "";
		{
			CSQLRowPurchaseControlRule RowPCR;
			RowPCR.SetRuleID(nRuleID);

			CSQLRepositoryPurchaseControlRule RepoRule;
			if (RepoRule.SelectRow(RowPCR, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				strRuleName = RowPCR.GetDisplayDescription();
			}
		}
		nLastRuleID = nRuleID;
	}
}

//**********************************************************************

CString CSQLRepositoryPurchaseControlRule::GetRuleName(int nRuleID, CDatabase* pDatabase)
{
	CString strRuleName = "";
	int nLastRuleID = nRuleID - 1;
	GetRuleName(nRuleID, nLastRuleID, strRuleName, pDatabase);
	return strRuleName;
}

//**********************************************************************