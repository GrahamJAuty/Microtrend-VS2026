//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\Consolidation.h"
#include "..\GlobalFunctions.h"
#include "..\GlobalState.h"
#include "..\SQLConnectionPool.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************
#include "memory"
//**********************************************************************
#include "SQLRepositoryAccount.h"
//**********************************************************************

CSQLRepositoryAccount::CSQLRepositoryAccount()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::SelectRow(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		int nConnection = -1;

		//recordset will add year parameter if in leaver mode
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_ById{RowAccount.GetUserIDString() });
		
		nConnection = RecordSet.GetConnection();

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
				RecordSet.SaveToAccountRow(RowAccount);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		// Handle connection cleanup for specific errors
		if (nConnection != -1)
		{
			switch (Result.GetSQLError())
			{
			case SQLCRUD_ERR_EXCEPT_DB:
			case SQLCRUD_ERR_EXCEPT_MISC:
			case SQLCRUD_ERR_RESET:
				SQLConnectionPool.DeleteConnection(nConnection);
				break;
			}
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::SelectRowFind(CSQLRowAccountFind& RowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		//recordset will add year parameter if in leaver mode
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_ById{ SimpleFormatInt64Value(RowAccount.GetUserID()) });

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
				RecordSet.SaveToAccountRow(RowAccount);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::UpdateRow(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		//recordset will add year parameter if in leaver mode
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_ById{ RowAccount.GetUserIDString() });

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
				RecordSet.LoadFromAccountRow(RowAccount);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::InsertRow(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase, RSParams_AccountFull_NoParams{}, FALSE);
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAccountRow(RowAccount);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		if ((System.GetAllowPINNumbersFlag() == TRUE) && (g_GlobalState.IsLeaverMode() == FALSE) )
		{
			//SPECIFY ZERO AS PIN NUMBER TO LOOK FOR NEW PIN
			CSQLRepositoryPINNumber RepoPIN;
			RepoPIN.LinkAccountIDToPINNumber(0, RowAccount.GetUserID(), pDatabase);
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::InsertLeaverDirect(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	int nLeaverYear = g_GlobalState.GetLeaverYear();
	g_GlobalState.SetLeaverYear(RowAccount.GetLeaverYear());

	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase, RSParams_AccountFull_NoParams{}, FALSE);
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAccountRow(RowAccount);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	CSQLResultInfo Result = CSQLHelpers::ExecuteWithExceptionHandling(operation);
	g_GlobalState.SetLeaverYear(nLeaverYear);
	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::UpsertRow(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowAccount.GetUserID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowAccount, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowAccount, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::DeleteRow(CSQLRowAccountFull& RowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		//recordset will add year parameter if in leaver mode
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_ById{ RowAccount.GetUserIDString() });

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

		if ((Result.GetSQLError() == SQLCRUD_ERR_NONE) && (g_GlobalState.IsLeaverMode() == FALSE))
		{
			CSQLRepositoryPINNumber RepoPIN;
			RepoPIN.RemoveAccountIDLink(RowAccount.GetUserID(), pDatabase);
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	//recordset will add year parameter if in leaver mode
	CSQLRecordSetAccountFull RecordSet(pDatabase,
		RSParams_AccountFull_ById{ SimpleFormatInt64Value(nUserID)}, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::CountRows(CDatabase* pDatabase)
{
	return CountRowsByRange(Account::UserID.Min, Account::UserID.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::CountRowsByRange(__int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	CSQLRecordSetAccountFull RecordSet(pDatabase,
		RSParams_AccountFull_CountRowsByRange{
			SimpleFormatInt64Value(nUserIDFrom),
			SimpleFormatInt64Value(nUserIDTo) });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetUserIDListInRange(CArray<__int64, __int64>& arrayUserIDs, __int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	arrayUserIDs.RemoveAll();

	if (g_GlobalState.IsLeaverMode() == TRUE)
	{
		return NotImplemented();
	}

	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_DetailByRange{
				FormatInt64Value(nUserIDFrom),
				FormatInt64Value(nUserIDTo)
			});

		__int64 nUserID = 0;	
		while (RecordSet.StepSelect(nUserID, TRUE) == TRUE)
		{
			arrayUserIDs.Add(nUserID);
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetUserIDList( CArray<__int64, __int64>& arrayUserIDs, CDatabase* pDatabase)
{
	return GetUserIDListInRange( arrayUserIDs, Account::UserID.Min, Account::UserID.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetHighestCurrentUserID(CDatabase* pDatabase)
{
	if (g_GlobalState.IsLeaverMode() == TRUE)
	{
		return NotImplemented();
	}

	CSQLResultInfo Result;
	Result.SetSQLError(SQLCRUD_ERR_RESET);

	try
	{
		CString strStatement = "";
		strStatement.Format("SELECT MAX(UserID) FROM %s",
			SQLTableNames::Accounts);

		CSQLRecordSetConnectionPool rsCount(pDatabase);

		if (rsCount.Open(CRecordset::forwardOnly, strStatement, CRecordset::executeDirect) == TRUE)
		{
			if (rsCount.IsEOF() == FALSE)
			{
				short nIndex = 0;
				CString strResult = "";
				{
					rsCount.GetFieldValue(nIndex, strResult);
					Result.SetSQLResult(_atoi64(strResult));
					Result.SetSQLError(SQLCRUD_ERR_NONE);
				}
			}

			rsCount.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::LookupMemberID(CString strMemberID, CDatabase* pDatabase)
{
	if (g_GlobalState.IsLeaverMode() == FALSE)
	{
		return LookupIndex(Account::MemberID.Label, strMemberID, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::LookupExtRef1(CString strMemberID, CDatabase* pDatabase)
{
	if (g_GlobalState.IsLeaverMode() == FALSE)
	{
		return LookupIndex(Account::ExternalRef1.Label, strMemberID, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::LookupIndex(CString strLocateOn, CString strMemberID, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		if (strLocateOn != "")
		{
			CSQLRecordSetAccountUserID RecordSet(pDatabase, 
				RSParams_AccountUserId_NormalByMemberId{ strMemberID });
				
			RecordSet.m_strFilter.Format("%s = ? AND NextUserID = 0",
				(const char*)strLocateOn);

			__int64 nResult = 0;
			if (RecordSet.StepSelect(nResult) == FALSE)
			{
				nResult = 0;
			}
			Result.SetSQLResult(nResult);
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveExtRef1(CString str, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	if (g_GlobalState.IsLeaverMode() == FALSE)
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_ResetExtRef1ByExtRef1{ str });

		Result = RemoveMemberInfo(RecordSet, pDatabase);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveExtRef2(CString str, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	if (g_GlobalState.IsLeaverMode() == FALSE)
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_ResetExtRef2ByExtRef2{ str });

		Result = RemoveMemberInfo(RecordSet, pDatabase);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveMemberID(CString str, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	if (g_GlobalState.IsLeaverMode() == FALSE)
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_ResetMemberIdByMemberId{ str });

		Result = RemoveMemberInfo(RecordSet, pDatabase);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveMemberInfo(CSQLRecordSetAccountUserID& RecordSet, CDatabase* pDatabase)
{
	if (g_GlobalState.IsLeaverMode() == TRUE)
	{
		return NotImplemented();
	}

	CSQLResultInfo Result;

	try
	{
		__int64 nUserID = 0;
		while (RecordSet.StepSelect(nUserID, TRUE) == TRUE)
		{
			RecordSet.Edit();
			RecordSet.m_MD.m_strMemberID = "";

			if (RecordSet.WrappedUpdate() != SQLCRUD_ERR_NONE)
			{
				Result.SetSQLError(SQLCRUD_ERR_PARTFAIL);
			}
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

void CSQLRepositoryAccount::PrepareServerSearchWhere(CSQLServerSearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	CSQLWhereBuilder WhereBuilder;

	if (infoSearch.m_nStartID > 0)
	{
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szGREATER, infoSearch.m_nStartID);
	}

	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Forename.Label, infoSearch.m_strForename);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Surname.Label, infoSearch.m_strSurname);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info1.Label, infoSearch.m_strInfo1);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info2.Label, infoSearch.m_strInfo2);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info3.Label, infoSearch.m_strInfo3);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info4.Label, infoSearch.m_strInfo4);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info5.Label, infoSearch.m_strInfo5);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info6.Label, infoSearch.m_strInfo6);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info7.Label, infoSearch.m_strInfo7);
	AppendToServerSearchWhere(WhereBuilder, BuilderBind, Account::Info8.Label, infoSearch.m_strInfo8);

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************

void CSQLRepositoryAccount::AppendToServerSearchWhere(CSQLWhereBuilder& WhereBuilder, CSQLBuilderBind& BuilderBind, CString strLabel, CString strSearchText)
{
	if (strSearchText != "")
	{
		CString strPercent = "%";
		CString strMatch = strPercent + strSearchText + strPercent;
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionString(BuilderBind, strLabel, szLIKE, strMatch);
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetNextFreeUserID(__int64 nStartID, __int64 nMaxID, CDatabase* pDatabase)
{
	if (g_GlobalState.IsLeaverMode() == TRUE)
	{
		return NotImplemented();
	}

	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_DetailByRange{
				FormatInt64Value(nStartID),
				FormatInt64Value(nMaxID)
			});

		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == TRUE)
		{
			__int64 nIDToTry = nStartID;

			while (RecordSet.IsEOF() == FALSE)
			{
				if (nIDToTry != RecordSet.m_MD.m_nUserID)
				{
					Result.SetSQLResult(nIDToTry);
					break;
				}
				nIDToTry++;
				RecordSet.MoveNext();
			}

			if (Result.GetSQLResult() == 0)
			{
				Result.SetSQLResult(nIDToTry);
			}
		}

		if (Result.GetSQLResult() > Account::UserID.Max)
		{
			Result.SetSQLResult(0);
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************
#ifndef SYSTEMTYPE_SERVER
//**********************************************************************

void CSQLRepositoryAccount::AppendExportWhereCondition(CDbExportHandler& dbExportHandler, CSQLWhereBuilder& WhereBuilder, CSQLBuilderBind& BuilderBind, int nCondition)
{
	CString strField = dbExportHandler.GetSQLConditionalField(nCondition);
	bool bIsPoints = (strField == Account::Points.Label) || (strField == Account::PointsToDate.Label);

	if (nCondition != 2)
	{
		if (TRUE == bIsPoints)
		{
			WhereBuilder.AppendConditionInt(BuilderBind, strField, dbExportHandler.GetCondition1(), int(floor(dbExportHandler.GetValue1())));
		}
		else
		{
			WhereBuilder.AppendConditionDouble(BuilderBind, strField, dbExportHandler.GetCondition1(), dbExportHandler.GetValue1());
		}
	}
	else
	{
		if (TRUE == bIsPoints)
		{
			WhereBuilder.AppendConditionInt(BuilderBind, strField, dbExportHandler.GetCondition2(), int(floor(dbExportHandler.GetValue2())));
		}
		else
		{
			WhereBuilder.AppendConditionDouble(BuilderBind, strField, dbExportHandler.GetCondition2(), dbExportHandler.GetValue2());
		}
	}
}

//**********************************************************************

void CSQLRepositoryAccount::PrepareExportWhere(CDbExportHandler& dbExportHandler, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	strWhere = "";
	BuilderBind.Reset();

	CSQLWhereBuilder WhereBuilder;

	//BALANCE CONDITIONS
	{
		CString strAndOr = dbExportHandler.GetAndOr();
		bool bOR = (strAndOr == szOR);
		CString strCondition1 = dbExportHandler.GetCondition1();
		CString strCondition2 = dbExportHandler.GetCondition2();
		CString strOP1 = WhereBuilder.GetOperatorForCondition(strCondition1);
		CString strOP2 = WhereBuilder.GetOperatorForCondition(strCondition2);

		if ((strOP1 != "") && (strOP2 != ""))
		{
			WhereBuilder.AppendOpenBracket();
			AppendExportWhereCondition(dbExportHandler, WhereBuilder, BuilderBind, 1);

			if (strAndOr == szOR)
			{
				WhereBuilder.AppendOr();
			}
			else
			{
				WhereBuilder.AppendAnd();
			}

			AppendExportWhereCondition(dbExportHandler, WhereBuilder, BuilderBind, 2);

			WhereBuilder.AppendCloseBracket();
		}
		else if (strOP1 != "")
		{
			AppendExportWhereCondition(dbExportHandler, WhereBuilder, BuilderBind, 1);
		}
		else if (strOP2 != "")
		{
			AppendExportWhereCondition(dbExportHandler, WhereBuilder, BuilderBind, 2);
		}
	}

	{
		CString strCon3 = dbExportHandler.GetCondition3();

		if (strCon3 == szUNKNOWN)
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionDirect("Gender IS NULL");
		}
		else if (strCon3 == szFEMALE)
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Gender.Label, szEQUAL, "F");

		}
		else if (strCon3 == szMALE)
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Gender.Label, szEQUAL, "M");
		}
	}

	//USER TICKS
	int nTick1 = GetFilterYesNoValue(dbExportHandler.GetTick1());
	int nTick2 = GetFilterYesNoValue(dbExportHandler.GetTick2());
	int nTick3 = GetFilterYesNoValue(dbExportHandler.GetTick3());
	int nTick4 = GetFilterYesNoValue(dbExportHandler.GetTick4());

	if (nTick1 != -1)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::Tick1.Label, szEQUAL, nTick1);
	}

	if (nTick2 != -1)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::Tick2.Label, szEQUAL, nTick2);
	}

	if (nTick3 != -1)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::Tick3.Label, szEQUAL, nTick3);

	}

	if (nTick4 != -1)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::Tick4.Label, szEQUAL, nTick4);
	}

	//BLANK SORT FIELD
	if (dbExportHandler.GetSkipBlanks() == TRUE)
	{
		CString strLabel = dbExportHandler.GetSortOnLabel();

		if (strLabel != Account::UserID.Label)
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, strLabel, szNOTEQUAL, "''");
		}
	}

	if (-1 != nUserID)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szEQUAL, nUserID);
	}

	if (g_GlobalState.IsLeaverMode())
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::LeaverYear.Label, szEQUAL, g_GlobalState.GetLeaverYear());
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#endif
//**********************************************************************

void CSQLRepositoryAccount::PreparePurchaseWhere(CPurchaseHistorySearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	strWhere = "";
	BuilderBind.Reset();

	CSQLWhereBuilder WhereBuilder;

	WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szGREATEROREQUAL, infoSearch.m_nUserIDFrom);
	WhereBuilder.AppendAnd();
	WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szLESSTHANOREQUAL, infoSearch.m_nUserIDTo);

	if (infoSearch.m_nUserIDExact >= 0)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szEQUAL, infoSearch.m_nUserIDExact);
	}

	if (TRUE == infoSearch.m_bMatchGroup)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, infoSearch.m_nGroupNo);
	}

	if (infoSearch.m_strFilterText != "")
	{
		CString strLabel = "";
		switch (infoSearch.m_nInfoTextIndex)
		{
		case 1:	strLabel = Account::Info1.Label;	break;
		case 2:	strLabel = Account::Info2.Label;	break;
		case 3:	strLabel = Account::Info3.Label;	break;
		case 4:	strLabel = Account::Info4.Label;	break;
		case 5:	strLabel = Account::Info5.Label;	break;
		case 6:	strLabel = Account::Info6.Label;	break;
		case 7:	strLabel = Account::Info7.Label;	break;
		case 8:	strLabel = Account::Info8.Label;	break;
		}

		if (strLabel != "")
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, strLabel, szEQUAL, infoSearch.m_strFilterText);
		}
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#ifndef SYSTEMTYPE_SERVER
#ifndef SYSTEMTYPE_BACKGROUND
//**********************************************************************

void CSQLRepositoryAccount::PrepareFindWhere(CSQLRowAccountFindInfo& infoSearch, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	strWhere = "";
	CSQLWhereBuilder WhereBuilder;

	if (infoSearch.m_strMatchString != "")
	{
		WhereBuilder.AppendOpenBracket();

		CString strPercent = "%";
		CString strMatch = strPercent + infoSearch.m_strMatchString + strPercent;

		if (TRUE == infoSearch.m_bMatchUsername)
		{
			WhereBuilder.AppendConditionString(BuilderBind, Account::Username.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchForename)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Forename.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchSurname)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Surname.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchMemberID)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::MemberID.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo1)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info1.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo2)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info2.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo3)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info3.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo4)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info4.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo5)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info5.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo6)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info5.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo7)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info7.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchInfo8)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info8.Label, szLIKE, strMatch);
		}

		WhereBuilder.AppendCloseBracket();
	}

	if (infoSearch.m_nGroupSel != 0)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, infoSearch.m_nGroupNo);
	}

	if (g_GlobalState.IsLeaverMode())
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::LeaverYear.Label, szEQUAL, g_GlobalState.GetLeaverYear());
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#endif
#endif
//**********************************************************************

int CSQLRepositoryAccount::GetFilterYesNoValue(int nFilter)
{
	switch (nFilter)
	{
	case nFILTER_YES:	
		return 1;

	case nFILTER_NO:	
		return 0;

	default:
		return -1;
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::NotImplemented() const
{
	AfxMessageBox("Not Implemented");
	CSQLResultInfo info;
	info.SetSQLError(SQLCRUD_ERR_FAIL);
	return info;
}

//**********************************************************************
