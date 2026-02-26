//**********************************************************************
#include "..\Defines.h"
#include "..\SQLConnectionPool.h"
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLNamespaces.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************
#include "SQLRepositoryAccount.h"
//**********************************************************************

CSQLRepositoryAccount::CSQLRepositoryAccount()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase )
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalById{ SQLRowAccount.GetUserIDString() });
			
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
				RecordSet.SaveToAccountRow(SQLRowAccount);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalById{ SimpleFormatInt64Value(SQLRowAccount.GetUserID()) });

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
				RecordSet.SaveToAccountRow(SQLRowAccount);

				RecordSet.MoveNext();
				if (RecordSet.IsEOF() == FALSE)
				{
					Result.SetSQLError(SQLCRUD_ERR_DUPLICATE);
				}
				else
				{
					Result.SetSQLError(SQLCRUD_ERR_NONE);
				}
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalById{ SQLRowAccount.GetUserIDString() });

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
				RecordSet.LoadFromAccountRow(SQLRowAccount);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAccountRow(SQLRowAccount);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(SQLRowAccount.GetUserID(), pDatabase);

	if (SQLCRUD_ERR_NONE == Result.GetSQLError())
	{
		if (0 == Result.GetSQLResult())
		{
			Result = InsertRow(SQLRowAccount, pDatabase);
		}
		else
		{
			Result = UpdateRow(SQLRowAccount, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalById{ SQLRowAccount.GetUserIDString() });

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

CSQLResultInfo CSQLRepositoryAccount::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_NormalById{ SimpleFormatInt64Value(nUserID) },
			TRUE);

		return RecordSet.CheckRowExists();
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::CountRows(CDatabase* pDatabase)
{
	return CountRowsInRange(Account::UserID.Min, Account::UserID.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::CountRowsInRange(__int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	CSQLRecordSetAccountFull RecordSet(pDatabase,
		RSParams_AccountFull_RowCountByRange{
			SimpleFormatInt64Value(nUserIDFrom),
			SimpleFormatInt64Value(nUserIDTo) });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetUserIDList(CArray<__int64, __int64>& arrayUserIDs, CDatabase* pDatabase)
{
	return GetUserIDListInRange(arrayUserIDs, Account::UserID.Min, Account::UserID.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetUserIDListInRange(CArray<__int64, __int64>& arrayUserIDs, __int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);

		arrayUserIDs.RemoveAll();

		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_NormalByRange{ 
				SimpleFormatInt64Value(nUserIDFrom), 
				SimpleFormatInt64Value(nUserIDTo) });

		Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == TRUE)
		{
			Result.SetSQLError(SQLCRUD_ERR_NONE);

			while (RecordSet.IsEOF() == FALSE)
			{
				arrayUserIDs.Add(RecordSet.m_MD.m_nUserID);
				RecordSet.MoveNext();
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetHighestCurrentUserID(CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);
		Result.SetSQLResult(0);

		CString strStatement = "";
		strStatement.Format("SELECT MAX(UserID) FROM %s",
			SQLTableNames::Accounts);

		CSQLRecordSetConnectionPool rsCount(pDatabase);

		Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
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

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::LookupMember(CString strLocateOn, CString strMemberID, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);
		Result.SetSQLResult(0);

		if (strLocateOn != "")
		{
			CSQLRecordSetAccountUserID RecordSet(pDatabase, 
				RSParams_AccountUserId_NormalByMemberID{ strMemberID });

			RecordSet.m_strFilter.Format("%s = ? AND NextUserID = 0",
				(const char*)strLocateOn);

			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			
			__int64 nResult = 0;
			if (RecordSet.StepSelect(nResult) == FALSE)
			{
				nResult = 0;
			}
			else
			{
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			Result.SetSQLResult(nResult);
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveQRInfo(CString strInfo, CDatabase* pDatabase)
{
	CSQLRecordSetAccountUserID RecordSet(pDatabase,
		RSParams_AccountUserId_ResetQRInfoByQRInfo{ strInfo });
		
	return RemoveMemberInfo(RecordSet);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveExtRef(CString strInfo, CDatabase* pDatabase)
{
	CSQLRecordSetAccountUserID RecordSet(pDatabase,
		RSParams_AccountUserId_ResetExtRefByExtRef{ strInfo });

	return RemoveMemberInfo(RecordSet);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::RemoveMemberInfo(CSQLRecordSetAccountUserID& RecordSet)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);
		Result.SetSQLResult(0);

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

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::GetNextFreeUserID(__int64 nStartID, __int64 nMaxID, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);
		Result.SetSQLResult(0);

		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_NormalByRange{
				SimpleFormatInt64Value(nStartID),
				SimpleFormatInt64Value(nMaxID) });
			
		Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == TRUE)
		{
			__int64 nIDToTry = nStartID;

			while (RecordSet.IsEOF() == FALSE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NONE);

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
			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAccount::SetAllAccountsInactive(CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_NONE);

		int nConnection = -1;

		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection, pDatabase);

		if (pDatabase != NULL)
		{
			int nSQLResult = CSQLHelpers::ExecuteSQL(pDatabase, "UPDATE Accounts SET Inactive = 1");

			if (nSQLResult != SQLCRUD_ERR_NONE)
			{
				Result.SetSQLError(nSQLResult);
			}
		}
		else
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}

		if (nConnection != -1)
		{
			SQLConnectionPool.FreeConnection(nConnection);
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************
#ifndef LOYSERVER
//**********************************************************************

void CSQLRepositoryAccount::PrepareFindWhere(CSQLRowAccountFindInfo& infoSearch, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	CSQLWhereBuilder WhereBuilder;

	if (infoSearch.m_strMatchString != "")
	{
		CString strPercent = "%";
		CString strMatch = strPercent + infoSearch.m_strMatchString + strPercent;

		WhereBuilder.AppendOpenBracket();

		if (TRUE == infoSearch.m_bMatchFullname)
		{
			WhereBuilder.AppendConditionString(BuilderBind, Account::Fullname.Label, szLIKE, strMatch);
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

		if (TRUE == infoSearch.m_bMatchInfo)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info1.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info2.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info3.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Info4.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchAddress)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Address1.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Address2.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Address3.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Address4.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchTelephone)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Phone1.Label, szLIKE, strMatch);
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Phone2.Label, szLIKE, strMatch);
		}

		if (TRUE == infoSearch.m_bMatchPostcode)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Address5.Label, szLIKE, strMatch);
		}

		WhereBuilder.AppendCloseBracket();
	}

	if (infoSearch.m_nGroupSel != 0)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, infoSearch.m_nGroupNo);
	}

	strWhere = WhereBuilder.GetWhereString();
}

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

	//GROUP
	{
		int nGroupReqd = dbExportHandler.GetGroupNo();
		if ( nGroupReqd > 0) 
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, nGroupReqd);
		}
	}

	//GENDER
	{
		bool bGotBind = FALSE;
		CString strGender = "";

		if (dbExportHandler.GetGenderOption() == 1)
		{
			switch (dbExportHandler.GetGenderCode())
			{
			case 0:	
				{
					WhereBuilder.AppendAnd();
					WhereBuilder.AppendConditionDirect("Gender IS NULL");
				}
				break;

			case 1:
				bGotBind = TRUE;
				strGender = "M";
				break;

			case 2:
				bGotBind = TRUE;
				strGender = "F";
				break;
			}
		}

		if (TRUE == bGotBind)
		{
			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Gender.Label, szEQUAL, strGender);
		}
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

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************

void CSQLRepositoryAccount::PrepareSearchWhere(CReportInfoPurchaseHistory& ReportInfo, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	CSQLWhereBuilder WhereBuilder;

	if (ReportInfo.GetCardRangeType() != nREPORT_LIST)
	{
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szGREATEROREQUAL, ReportInfo.GetRangeFromCardNo());
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szLESSTHANOREQUAL, ReportInfo.GetRangeToCardNo());
	}

	if (ReportInfo.GetSingleGroupFlag() == TRUE)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, ReportInfo.GetSingleGroupNo());
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#else
//**********************************************************************

void CSQLRepositoryAccount::PrepareServerSearchWhereAccountLookup(CSPOSSearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	CString strPercent = "%";

	CSQLWhereBuilder WhereBuilder;

	if (infoSearch.m_strSurname != "")
	{
		CString strMatch = strPercent + infoSearch.m_strSurname + strPercent;
		WhereBuilder.AppendOpenBracket();
		WhereBuilder.AppendConditionString(BuilderBind, Account::Surname.Label, szLIKE, strMatch);
		
		if (Server.GetSearchSurnameMatchFullnameFlag() == TRUE)
		{
			WhereBuilder.AppendOr();
			WhereBuilder.AppendOpenBracket();
			
			WhereBuilder.AppendOpenBracket();
			WhereBuilder.AppendConditionDirect("Surname IS NULL");
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionDirect("Surname = ''");
			WhereBuilder.AppendCloseBracket();
			
			WhereBuilder.AppendAnd();

			WhereBuilder.AppendOpenBracket();
			WhereBuilder.AppendConditionDirect("Forename IS NULL");
			WhereBuilder.AppendOr();
			WhereBuilder.AppendConditionDirect("Forename = ''");
			WhereBuilder.AppendCloseBracket();

			WhereBuilder.AppendAnd();
			WhereBuilder.AppendConditionString(BuilderBind, Account::Fullname.Label, szLIKE, strMatch);
			WhereBuilder.AppendCloseBracket();
		}
		
		WhereBuilder.AppendCloseBracket();
	}

	if (infoSearch.m_strForename != "")
	{
		CString strMatch = strPercent + infoSearch.m_strForename + strPercent;
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionString(BuilderBind, Account::Forename.Label, szLIKE, strMatch);
	}

	if (TRUE == infoSearch.m_bExcludeInactive)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::Inactive.Label, szEQUAL, 0);
	}

	if (TRUE == infoSearch.m_bExcludeHotlist)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::HotlistCode.Label, szEQUAL, 0);
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************

void CSQLRepositoryAccount::PrepareServerSearchWhereSnakesAndLadders(CSPOSSearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind)
{
	CString strPercent = "%";

	CSQLWhereBuilder WhereBuilder;

	if (infoSearch.m_strSurname != "")
	{
		CString strMatch = strPercent + infoSearch.m_strSurname + strPercent;
		WhereBuilder.AppendOpenBracket();
		WhereBuilder.AppendConditionString(BuilderBind, Account::Surname.Label, szLIKE, strMatch);
		WhereBuilder.AppendOr();
		WhereBuilder.AppendConditionString(BuilderBind, Account::Fullname.Label, szLIKE, strMatch);
		WhereBuilder.AppendCloseBracket();
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#endif
//**********************************************************************


