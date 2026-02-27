//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLDb.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "..\SQLSchema\SQLBuilderCreateTable.h"
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************
#include "SQLAccountManager.h"
//**********************************************************************

CSQLAccountManager::CSQLAccountManager() : 
	m_PrepStatMain{},
	m_PrepStatLeaver{}
{
	m_nLeaverYear = 0;
	m_strLastValidLocateOn = "";
}

//**********************************************************************

CSQLAccountManager::~CSQLAccountManager()
{
}

//**********************************************************************

CString CSQLAccountManager::GetCurrentTableName()
{
	CString strResult = "";

	if ( 0 == m_nLeaverYear )
	{
		strResult = SQLTableNames::Accounts;
	}
	else
	{
		strResult = SQLTableNames::Leavers;
	}

	return strResult;
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	if ( 0 == m_nLeaverYear )
	{
		return m_PrepStatMain.SelectRow(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.SelectRow(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase)
{
	if ( 0 == m_nLeaverYear )
	{
		return m_PrepStatMain.SelectRowFind(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.SelectRowFind(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.UpdateRow(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.UpdateRow(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.InsertRow(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.InsertRow(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::InsertLeaversDirect(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	return m_PrepStatLeaver.InsertDirect(SQLRowAccount, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.UpsertRow(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.UpsertRow(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.DeleteRow(SQLRowAccount, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.DeleteRow(SQLRowAccount, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.RowExists(nUserID, pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.RowExists(nUserID, pDatabase);
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::CountRows(CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.CountRows(pDatabase);
	}
	else
	{
		return m_PrepStatLeaver.CountRows(pDatabase);
	}
}

//**********************************************************************

void CSQLAccountManager::ClearUserIDCache()
{
	if (0 == m_nLeaverYear)
	{
		m_PrepStatMain.ClearUserIDCache();
	}
	else
	{
		m_PrepStatLeaver.ClearUserIDCache();
	}
}

//**********************************************************************

void CSQLAccountManager::BuildUserIDCache()
{
	if (0 == m_nLeaverYear)
	{
		m_PrepStatMain.BuildUserIDCache();
	}
	else
	{
		m_PrepStatLeaver.BuildUserIDCache();
	}
}

//**********************************************************************

bool CSQLAccountManager::CheckUserIDCache(__int64 nUserID)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.CheckUserIDCache(nUserID);
	}
	else
	{
		return m_PrepStatLeaver.CheckUserIDCache(nUserID);
	}
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CSQLResultInfo CSQLAccountManager::CountRowsInRange(__int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.CountRowsInRange(nUserIDFrom, nUserIDTo, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::GetUserIDListInRange(CArray<__int64,__int64>& arrayUserIDs, __int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.GetUserIDListInRange( arrayUserIDs, nUserIDFrom, nUserIDTo, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CSQLResultInfo CSQLAccountManager::GetUserIDList(CArray<__int64, __int64>& arrayUserIDs, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.GetUserIDList(arrayUserIDs, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::GetHighestCurrentUserID(CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.GetHighestCurrentUserID(pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::LookupMemberID( CString strMemberID, CDatabase* pDatabase)
{
	if (strMemberID.Right(3) == "Z4C")
	{
		int x = 2;
	}

	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.LookupIndex(Account::MemberID.Label, strMemberID, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::LookupExtRef1(CString strMemberID, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.LookupIndex(Account::ExternalRef1.Label, strMemberID, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::LookupIndex(CString strLocateOn, CString strMemberID, CDatabase* pDatabase)
{
	CSQLResultInfo Result;
	Result.SetSQLError(SQLCRUD_ERR_RESET);

	if (strLocateOn != "")
	{
		bool bAcceptLocateOn = (strLocateOn == m_strLastValidLocateOn);

		if (FALSE == bAcceptLocateOn)
		{
			bAcceptLocateOn |= (strLocateOn == Account::Info1.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info2.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info3.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info4.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info5.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info6.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info7.Label);
			bAcceptLocateOn |= (strLocateOn == Account::Info8.Label);
			bAcceptLocateOn |= (strLocateOn == Account::ExternalRef1.Label);
			bAcceptLocateOn |= (strLocateOn == Account::ExternalRef2.Label);
			bAcceptLocateOn |= (strLocateOn == Account::MemberID.Label);
		}

		if (TRUE == bAcceptLocateOn)
		{
			m_strLastValidLocateOn = strLocateOn;

			if ( 0 == m_nLeaverYear)
			{
				Result  = m_PrepStatMain.LookupIndex(strLocateOn, strMemberID, pDatabase);
			}
			else
			{
				Result = NotImplemented();
			}
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::RemoveExtRef1(CString str, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.RemoveExtRef1(str, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::RemoveExtRef2(CString str, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.RemoveExtRef2(str, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************

CSQLResultInfo CSQLAccountManager::RemoveMemberID(CString str, CDatabase* pDatabase)
{
	if (0 == m_nLeaverYear)
	{
		return m_PrepStatMain.RemoveMemberID(str, pDatabase);
	}
	else
	{
		return NotImplemented();
	}
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

int CSQLAccountManager::GetFilterYesNoValue(int nFilter)
{
	switch (nFilter)
	{
	case nFILTER_YES:	return 1;
	case nFILTER_NO:	return 0;
	default:			return -1;
	}
}

//**********************************************************************
#ifndef SYSTEMTYPE_SERVER
//**********************************************************************

void CSQLAccountManager::AppendExportWhereCondition(CDbExportHandler& dbExportHandler, CSQLWhereBuilder& WhereBuilder, CSQLBuilderBind& BuilderBind, int nCondition)
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

void CSQLAccountManager::PrepareExportWhere(CDbExportHandler& dbExportHandler, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind)
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

	if (m_nLeaverYear != 0)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::LeaverYear.Label, szEQUAL, m_nLeaverYear);
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#endif
//**********************************************************************

void CSQLAccountManager::PreparePurchaseWhere(CPurchaseHistorySearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind)
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

void CSQLAccountManager::PrepareFindWhere(CSQLRowAccountFindInfo& infoSearch, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind)
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

	if (m_nLeaverYear != 0)
	{
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt(BuilderBind, Account::LeaverYear.Label, szEQUAL, m_nLeaverYear);
	}

	strWhere = WhereBuilder.GetWhereString();
}

//**********************************************************************
#endif
#endif
//**********************************************************************

CSQLResultInfo CSQLAccountManager::GetNextFreeUserID(__int64 nStartID, __int64 nMaxID, CDatabase* pDatabase )
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountUserID RecordSet(pDatabase,
			RSParams_AccountUserId_NormalByRange{
				FormatInt64Value(nStartID),
				FormatInt64Value(nMaxID)
			});
			
		RecordSet.m_strFilter = "UserID >= ? AND UserID <= ?";

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

			if (Result.GetSQLResult()==0)
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

CSQLResultInfo CSQLAccountManager::NotImplemented()
{
	AfxMessageBox("Not Implemented");
	CSQLResultInfo info;
	info.SetSQLError(SQLCRUD_ERR_FAIL);
	return info;
}

//**********************************************************************

bool CSQLAccountManager::IsLeaverMode()
{
	return (0 != m_nLeaverYear);
}

//**********************************************************************
