//**********************************************************************
#include "..\GlobalState.h"
#include "..\ServerData.h"
#//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\GroupShiftLogger.h"
#include "..\GroupShiftValidator.h"
//**********************************************************************
#include "SQLRepositoryAccount.h"
//**********************************************************************
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

CSQLServerSearchInfo::CSQLServerSearchInfo()
{
    m_strForename = "";
    m_strSurname = "";
    m_strInfo1 = "";
    m_strInfo2 = "";
    m_strInfo3 = "";
    m_strInfo4 = "";
    m_strInfo5 = "";
    m_strInfo6 = "";
    m_strInfo7 = "";
    m_strInfo8 = "";
    m_nGroupType = - 1;
    m_nStartID = 0;
    m_nMaxToFind = Server.GetSearchMax();
}

//**********************************************************************

CSQLRecordSetMetadataAccountFull::CSQLRecordSetMetadataAccountFull(bool bGroupSetMode, bool bForExistenceCheck)
{
    m_bGroupSetMode = bGroupSetMode;
    m_bForExistenceCheck = bForExistenceCheck;
    bool bLeaverMode = g_GlobalState.IsLeaverMode();

    if ( TRUE == bLeaverMode)
    {
        bGroupSetMode = FALSE;
    }
    
    if (TRUE == m_bForExistenceCheck)
    {
        bGroupSetMode = FALSE;
        return;
    }

    if (TRUE == bLeaverMode)
    {
        AddIntField(&m_nLeaverYear, Account::LeaverYear.Label);
        m_nGroupSet = 0;
    }
    else if (TRUE == m_bGroupSetMode)
    {
        AddIntField(&m_nGroupSet, Group::SetNo.Label);
        m_nLeaverYear = 0;
    }
    else
    {
        m_nLeaverYear = 0;
        m_nGroupSet = 0;
	}

    AddInt64Field(&m_nUserID, ACLabel(Account::UserID.Label));
    AddTextField(&m_strUsername, ACLabel(Account::Username.Label));
    AddTextField(&m_strForename, Account::Forename.Label);
    AddTextField(&m_strSurname, Account::Surname.Label);
    AddTextField(&m_strDOB, Account::DOB.Label);
    AddTextField(&m_strExpiryDate, Account::ExpiryDate.Label);
    AddTextField(&m_strGender, Account::Gender.Label);
    AddIntField(&m_nGroupNo, ACLabel(Account::GroupNo.Label));
    AddTextField(&m_strDayGroups, Account::DayGroups.Label);
    AddIntField(&m_nGroupShiftStatus, Account::GroupShiftStatus.Label);
    AddTextField(&m_strGroupShiftStartDate, Account::GroupShiftStartDate.Label);
    AddTextField(&m_strGroupShiftEndDate, Account::GroupShiftEndDate.Label);
    AddIntField(&m_nGroupShiftFutureGroup, Account::GroupShiftFutureGroup.Label);
    AddTextField(&m_strEmail, Account::Email.Label);
    AddTextField(&m_strInfo1, Account::Info1.Label);
    AddTextField(&m_strInfo2, Account::Info2.Label);
    AddTextField(&m_strInfo3, Account::Info3.Label);
    AddTextField(&m_strInfo4, Account::Info4.Label);
    AddTextField(&m_strInfo5, Account::Info5.Label);
    AddTextField(&m_strInfo6, Account::Info6.Label);
    AddTextField(&m_strInfo7, Account::Info7.Label);
    AddTextField(&m_strInfo8, Account::Info8.Label);
    AddBOOLField(&m_bTick1, Account::Tick1.Label, FALSE);
    AddBOOLField(&m_bTick2, Account::Tick2.Label, FALSE);
    AddBOOLField(&m_bTick3, Account::Tick3.Label, FALSE);
    AddBOOLField(&m_bTick4, Account::Tick4.Label, FALSE);
    AddIntField(&m_nAllergyCodes, Account::AllergyCodes.Label);
    AddTextField(&m_strDietary, Account::Dietary.Label);
    AddBOOLField(&m_bInactive, Account::Inactive.Label, FALSE);
    /****/
    AddTextField(&m_strMemberID, Account::MemberID.Label);
    AddTextField(&m_strExternalRef1, Account::ExternalRef1.Label);
    AddTextField(&m_strExternalRef2, Account::ExternalRef2.Label);
    AddInt64Field(&m_nPreviousUserID, Account::PreviousUserID.Label);
    AddInt64Field(&m_nNextUserID, Account::NextUserID.Label);
    AddTextField(&m_strLegacyPebblePurseID, Account::PebblePurseID.Label);
    /****/
    AddIntField(&m_nPurse1Allow, ACLabel(Account::Purse1Allow.Label));
    AddDecimalField(&m_strPurse1Liability, Account::Purse1Liability.Label);
    AddDecimalField(&m_strPurse1Credit, Account::Purse1Credit.Label);
    AddTextField(&m_strPurse1LastSpendDate, Account::Purse1LastSpendDate.Label);
    AddTextField(&m_strPurse1LastSpendTime, Account::Purse1LastSpendTime.Label);
    AddDecimalField(&m_strPurse1LastSpend, Account::Purse1LastSpend.Label);
    AddDecimalField(&m_strPurse1SpendToDate, Account::Purse1SpendToDate.Label);
    /****/
    AddIntField(&m_nPurse2Allow, ACLabel(Account::Purse2Allow.Label));
    AddDecimalField(&m_strPurse2Balance, Account::Purse2Balance.Label);
    AddTextField(&m_strPurse2LastSpendDate, Account::Purse2LastSpendDate.Label);
    AddTextField(&m_strPurse2LastSpendTime, Account::Purse2LastSpendTime.Label);
    AddDecimalField(&m_strPurse2LastSpend, Account::Purse2LastSpend.Label);
    AddDecimalField(&m_strPurse2SpendToDate, Account::Purse2SpendToDate.Label);
    AddIntField(&m_nPurse2RefreshFlags, Account::Purse2RefreshFlags.Label);
    AddTextField(&m_strPurse2RefreshedDate, Account::Purse2RefreshedDate.Label);
    AddTextField(&m_strPurse2RefreshedTime, Account::Purse2RefreshedTime.Label);
    /****/
    AddIntField(&m_nPurse3Allow, ACLabel(Account::Purse3Allow.Label));
    AddDecimalField(&m_strPurse3Liability, Account::Purse3Liability.Label);
    AddDecimalField(&m_strPurse3Credit, Account::Purse3Credit.Label);
    AddTextField(&m_strPurse3LastSpendDate, Account::Purse3LastSpendDate.Label);
    AddTextField(&m_strPurse3LastSpendTime, Account::Purse3LastSpendTime.Label);
    AddDecimalField(&m_strPurse3LastSpend, Account::Purse3LastSpend.Label);
    AddDecimalField(&m_strPurse3SpendToDate, Account::Purse3SpendToDate.Label);
    /****/
    AddDecimalField(&m_strCashSpendToDate, Account::CashSpendToDate.Label);
    /****/
    AddIntField(&m_nPoints, Account::Points.Label);
    AddIntField(&m_nPointsToDate, Account::PointsToDate.Label);
    /****/
    AddBOOLField(&m_bOwnMaxSpend, Account::OwnMaxSpend.Label, FALSE);
    AddDecimalField(&m_strMaxSpend[0][0], ACLabel(Account::MaxSpendPurse1Period1.Label));
    AddDecimalField(&m_strMaxSpend[0][1], ACLabel(Account::MaxSpendPurse1Period2.Label));
    AddDecimalField(&m_strMaxSpend[0][2], ACLabel(Account::MaxSpendPurse1Period3.Label));
    AddDecimalField(&m_strMaxSpend[0][3], ACLabel(Account::MaxSpendPurse1Period4.Label));
    AddDecimalField(&m_strMaxSpend[0][4], ACLabel(Account::MaxSpendPurse1Period5.Label));
    AddDecimalField(&m_strMaxSpend[0][5], ACLabel(Account::MaxSpendPurse1Period6.Label));
    AddDecimalField(&m_strMaxSpend[1][0], ACLabel(Account::MaxSpendPurse3Period1.Label));
    AddDecimalField(&m_strMaxSpend[1][1], ACLabel(Account::MaxSpendPurse3Period2.Label));
    AddDecimalField(&m_strMaxSpend[1][2], ACLabel(Account::MaxSpendPurse3Period3.Label));
    AddDecimalField(&m_strMaxSpend[1][3], ACLabel(Account::MaxSpendPurse3Period4.Label));
    AddDecimalField(&m_strMaxSpend[1][4], ACLabel(Account::MaxSpendPurse3Period5.Label));
    AddDecimalField(&m_strMaxSpend[1][5], ACLabel(Account::MaxSpendPurse3Period6.Label));
    AddDecimalField(&m_strMaxOverdraft[0][0], ACLabel(Account::MaxOverdraftPurse1Period1.Label));
    AddDecimalField(&m_strMaxOverdraft[0][1], ACLabel(Account::MaxOverdraftPurse1Period2.Label));
    AddDecimalField(&m_strMaxOverdraft[0][2], ACLabel(Account::MaxOverdraftPurse1Period3.Label));
    AddDecimalField(&m_strMaxOverdraft[0][3], ACLabel(Account::MaxOverdraftPurse1Period4.Label));
    AddDecimalField(&m_strMaxOverdraft[0][4], ACLabel(Account::MaxOverdraftPurse1Period5.Label));
    AddDecimalField(&m_strMaxOverdraft[0][5], ACLabel(Account::MaxOverdraftPurse1Period6.Label));
    AddDecimalField(&m_strMaxOverdraft[1][0], ACLabel(Account::MaxOverdraftPurse3Period1.Label));
    AddDecimalField(&m_strMaxOverdraft[1][1], ACLabel(Account::MaxOverdraftPurse3Period2.Label));
    AddDecimalField(&m_strMaxOverdraft[1][2], ACLabel(Account::MaxOverdraftPurse3Period3.Label));
    AddDecimalField(&m_strMaxOverdraft[1][3], ACLabel(Account::MaxOverdraftPurse3Period4.Label));
    AddDecimalField(&m_strMaxOverdraft[1][4], ACLabel(Account::MaxOverdraftPurse3Period5.Label));
    AddDecimalField(&m_strMaxOverdraft[1][5], ACLabel(Account::MaxOverdraftPurse3Period6.Label));
    /****/
    AddIntField(&m_nAlertCode, Account::AlertCode.Label);
    AddTextField(&m_strAlertDate, Account::AlertDate.Label);
    AddTextField(&m_strAlertTime, Account::AlertTime.Label);
}
  
//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddNoParams()
{
    if (g_GlobalState.IsLeaverMode() == TRUE)
    {
        AddIntParam(Account::LeaverYear.Label, g_GlobalState.GetLeaverYear());
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddParamsByUserID(const CString strUserID)
{
    if (g_GlobalState.IsLeaverMode() == TRUE)
    {
        AddIntParam(Account::LeaverYear.Label, g_GlobalState.GetLeaverYear());
    }

    AddTextParam(Account::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo)
{
    if (g_GlobalState.IsLeaverMode())
    {
        AddIntParam(Account::LeaverYear.Label, g_GlobalState.GetLeaverYear());
    }

    AddTextParam(Account::UserID.Label, strUserIDFrom);
    AddTextParam(Account::UserID.Label, strUserIDTo);
}

//**********************************************************************

CString CSQLRecordSetMetadataAccountFull::ACLabel(CString str)
{
    CString strResult = "";

    if (g_GlobalState.IsLeaverMode())
    {
		strResult += SQLTableNames::Leavers;
    }
    else
    {
		strResult += SQLTableNames::Accounts;
    }

    strResult += ".[";
    strResult += str;
    strResult += "]";
    return strResult;
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ById params, bool bForExistenceCheck)
    : CSQLRecordSetConnectionPool(pDatabase),
	m_MD(FALSE, bForExistenceCheck)
{
    m_strFilter = "";
	m_MD.AddParamsByUserID(params.strUserID);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_CountRowsByRange params)
    : CSQLRecordSetConnectionPool(pDatabase),
    m_MD(FALSE, TRUE)
{
    m_strFilter = "";
	m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(params.strUserIDFrom, params.strUserIDTo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NoParams params, bool bGroupSetMode)
    : CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bGroupSetMode, FALSE)
{
    m_strFilter = "";
	m_MD.AddNoParams();
    CommonInit();

    if (TRUE == m_bGroupSetMode)
    {
        m_strFilter = m_strAccountTableName + ".[GroupNo] = AccountGroups.[GroupNo]";
        m_strMyDefaultSQL += ",AccountGroups";
    }
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_Backup params)
    : CSQLRecordSetConnectionPool(pDatabase),
    m_MD(FALSE, FALSE)
{
    m_strFilter = "";
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ByParams params)
    : CSQLRecordSetConnectionPool(pDatabase),
    m_MD(FALSE, FALSE)
{
	m_strFilter = params.strWhere;
    m_MD.BindGeneralParams(params.pBind);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountFull::CommonInit()
{
	m_bGroupSetMode = m_MD.m_bGroupSetMode;
	m_bForExistenceCheck = m_MD.m_bForExistenceCheck;

    if (g_GlobalState.IsLeaverMode())
    {
        m_strAccountTableName = SQLTableNames::Leavers;
    }
    else
    {
        m_strAccountTableName = SQLTableNames::Accounts;
	}

    m_strMyDefaultSQL = m_strAccountTableName;

    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();

    if (FALSE == m_bForExistenceCheck)
    {
        m_nFields++;
    }

    SetMetadata(&m_MD);

    if (m_strFilter == "")
    {
        SetInitialFilterString();
    }
}

//**********************************************************************

void CSQLRecordSetAccountFull::DoFieldExchange(CFieldExchange* pFX)
{
    ExchangeFields(pFX);

    if (FALSE == m_bForExistenceCheck)
    {
        RFX_Binary(pFX, Account::RowVersion.Label, m_arrayRowVersion);
    }

    ExchangeParams(pFX);
}

//**********************************************************************

void CSQLRecordSetAccountFull::LoadFromAccountRow(CSQLRowAccountFull& RowAccount)
{
    m_MD.m_nLeaverYear = RowAccount.GetLeaverYear();
    m_MD.m_nUserID = RowAccount.GetUserID();
    m_MD.m_strUsername = RowAccount.GetUsername();
    m_MD.m_strForename = RowAccount.GetForename();
    m_MD.m_strSurname = RowAccount.GetSurname();
    m_MD.m_strDOB = RowAccount.GetDOB();
    m_MD.m_strExpiryDate = RowAccount.GetExpiryDate();
    m_MD.m_strGender = RowAccount.GetGender();
    m_MD.m_nGroupNo = RowAccount.GetGroupNo();
    m_MD.m_strDayGroups = RowAccount.GetDayGroups();
    m_MD.m_nGroupShiftStatus = RowAccount.GetGroupShiftStatus();
    m_MD.m_strGroupShiftStartDate = RowAccount.GetGroupShiftStartDate();
    m_MD.m_strGroupShiftEndDate = RowAccount.GetGroupShiftEndDate();
    m_MD.m_nGroupShiftFutureGroup = RowAccount.GetGroupShiftFutureGroup();
    m_MD.m_strEmail = RowAccount.GetEmail();
    m_MD.m_strInfo1 = RowAccount.GetInfo1();
    m_MD.m_strInfo2 = RowAccount.GetInfo2();
    m_MD.m_strInfo3 = RowAccount.GetInfo3();
    m_MD.m_strInfo4 = RowAccount.GetInfo4();
    m_MD.m_strInfo5 = RowAccount.GetInfo5();
    m_MD.m_strInfo6 = RowAccount.GetInfo6();
    m_MD.m_strInfo7 = RowAccount.GetInfo7();
    m_MD.m_strInfo8 = RowAccount.GetInfo8();
    m_MD.m_bTick1 = RowAccount.GetTick1();
    m_MD.m_bTick2 = RowAccount.GetTick2();
    m_MD.m_bTick3 = RowAccount.GetTick3();
    m_MD.m_bTick4 = RowAccount.GetTick4();
    m_MD.m_nAllergyCodes = RowAccount.GetAllergyCodes();
    m_MD.m_strDietary = RowAccount.GetDietary();
    m_MD.m_bInactive = RowAccount.GetInactive();
    /*****/
    m_MD.m_strMemberID = RowAccount.GetMemberID();
    m_MD.m_strExternalRef1 = RowAccount.GetExternalRef1();
    m_MD.m_strExternalRef2 = RowAccount.GetExternalRef2();
    m_MD.m_nPreviousUserID = RowAccount.GetPreviousUserID();
    m_MD.m_nNextUserID = RowAccount.GetNextUserID();
    m_MD.m_strLegacyPebblePurseID = RowAccount.GetLegacyPebblePurseID();
    /*****/
    m_MD.m_nPurse1Allow = RowAccount.GetPurse1Allow();
	SetDoubleString(m_MD.m_strPurse1Liability, RowAccount.GetPurse1Liability());
	SetDoubleString(m_MD.m_strPurse1Credit, RowAccount.GetPurse1Credit());
    m_MD.m_strPurse1LastSpendDate = RowAccount.GetPurse1LastSpendDate();
    m_MD.m_strPurse1LastSpendTime = RowAccount.GetPurse1LastSpendTime();
	SetDoubleString(m_MD.m_strPurse1LastSpend, RowAccount.GetPurse1LastSpend());
    SetDoubleString(m_MD.m_strPurse1SpendToDate, RowAccount.GetPurse1SpendToDate());
    /*****/
    m_MD.m_nPurse2Allow = RowAccount.GetPurse2Allow();
	SetDoubleString(m_MD.m_strPurse2Balance, RowAccount.GetPurse2Balance());
    m_MD.m_strPurse2LastSpendDate = RowAccount.GetPurse2LastSpendDate();
    m_MD.m_strPurse2LastSpendTime = RowAccount.GetPurse2LastSpendTime();
	SetDoubleString(m_MD.m_strPurse2LastSpend, RowAccount.GetPurse2LastSpend());
    SetDoubleString(m_MD.m_strPurse2SpendToDate, RowAccount.GetPurse2SpendToDate());
    m_MD.m_nPurse2RefreshFlags = RowAccount.GetPurse2RefreshFlags();
    m_MD.m_strPurse2RefreshedDate = RowAccount.GetPurse2RefreshedDate();
    m_MD.m_strPurse2RefreshedTime = RowAccount.GetPurse2RefreshedTime();
    /*****/
    m_MD.m_nPurse3Allow = RowAccount.GetPurse3Allow();
    SetDoubleString(m_MD.m_strPurse3Liability, RowAccount.GetPurse3Liability());
    SetDoubleString(m_MD.m_strPurse3Credit, RowAccount.GetPurse3Credit());
    m_MD.m_strPurse3LastSpendDate = RowAccount.GetPurse3LastSpendDate();
    m_MD.m_strPurse3LastSpendTime = RowAccount.GetPurse3LastSpendTime();
    SetDoubleString(m_MD.m_strPurse3LastSpend, RowAccount.GetPurse3LastSpend());
    SetDoubleString(m_MD.m_strPurse3SpendToDate, RowAccount.GetPurse3SpendToDate());
    /*****/
	SetDoubleString(m_MD.m_strCashSpendToDate, RowAccount.GetCashSpendToDate());
    /*****/
    m_MD.m_nPoints = RowAccount.GetPoints();
    m_MD.m_nPointsToDate = RowAccount.GetPointsToDate();
    /*****/
    m_MD.m_bOwnMaxSpend = RowAccount.GetOwnMaxSpend();
    /*****/
	SetDoubleString(m_MD.m_strMaxSpend[0][0], RowAccount.GetMaxSpendPurse1Period1());
	SetDoubleString(m_MD.m_strMaxSpend[0][1], RowAccount.GetMaxSpendPurse1Period2());
	SetDoubleString(m_MD.m_strMaxSpend[0][2], RowAccount.GetMaxSpendPurse1Period3());
	SetDoubleString(m_MD.m_strMaxSpend[0][3], RowAccount.GetMaxSpendPurse1Period4());
	SetDoubleString(m_MD.m_strMaxSpend[0][4], RowAccount.GetMaxSpendPurse1Period5());
    SetDoubleString(m_MD.m_strMaxSpend[0][5], RowAccount.GetMaxSpendPurse1Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxSpend[1][0], RowAccount.GetMaxSpendPurse3Period1());
	SetDoubleString(m_MD.m_strMaxSpend[1][1], RowAccount.GetMaxSpendPurse3Period2());
	SetDoubleString(m_MD.m_strMaxSpend[1][2], RowAccount.GetMaxSpendPurse3Period3());
	SetDoubleString(m_MD.m_strMaxSpend[1][3], RowAccount.GetMaxSpendPurse3Period4());
    SetDoubleString(m_MD.m_strMaxSpend[1][4], RowAccount.GetMaxSpendPurse3Period5());
    SetDoubleString(m_MD.m_strMaxSpend[1][5], RowAccount.GetMaxSpendPurse3Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxOverdraft[0][0], RowAccount.GetMaxOverdraftPurse1Period1());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][1], RowAccount.GetMaxOverdraftPurse1Period2());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][2], RowAccount.GetMaxOverdraftPurse1Period3());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][3], RowAccount.GetMaxOverdraftPurse1Period4());
    SetDoubleString(m_MD.m_strMaxOverdraft[0][4], RowAccount.GetMaxOverdraftPurse1Period5());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][5], RowAccount.GetMaxOverdraftPurse1Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxOverdraft[1][0], RowAccount.GetMaxOverdraftPurse3Period1());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][1], RowAccount.GetMaxOverdraftPurse3Period2());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][2], RowAccount.GetMaxOverdraftPurse3Period3());
    SetDoubleString(m_MD.m_strMaxOverdraft[1][3], RowAccount.GetMaxOverdraftPurse3Period4());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][4], RowAccount.GetMaxOverdraftPurse3Period5());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][5], RowAccount.GetMaxOverdraftPurse3Period6());
    /*****/
    m_MD.m_nAlertCode = RowAccount.GetAlertCode();
    m_MD.m_strAlertDate = RowAccount.GetAlertDate();
    m_MD.m_strAlertTime = RowAccount.GetAlertTime();
}

//**********************************************************************

void CSQLRecordSetAccountFull::SaveToAccountRow(CSQLRowAccountFull& RowAccount)
{
    RowAccount.SetLeaverYear(m_MD.m_nLeaverYear);
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetUsername(m_MD.m_strUsername);
    RowAccount.SetForename(m_MD.m_strForename);
    RowAccount.SetSurname(m_MD.m_strSurname);
    RowAccount.SetDOB(m_MD.m_strDOB);
    RowAccount.SetExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetGender(m_MD.m_strGender);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
    RowAccount.SetDayGroups(m_MD.m_strDayGroups);
    RowAccount.SetGroupShiftStatus(m_MD.m_nGroupShiftStatus);
    RowAccount.SetGroupShiftStartDate(m_MD.m_strGroupShiftStartDate);
    RowAccount.SetGroupShiftEndDate(m_MD.m_strGroupShiftEndDate);
    RowAccount.SetGroupShiftFutureGroup(m_MD.m_nGroupShiftFutureGroup);
    RowAccount.SetEmail(m_MD.m_strEmail);
    RowAccount.SetInfo1(m_MD.m_strInfo1);
    RowAccount.SetInfo2(m_MD.m_strInfo2);
    RowAccount.SetInfo3(m_MD.m_strInfo3);
    RowAccount.SetInfo4(m_MD.m_strInfo4);
    RowAccount.SetInfo5(m_MD.m_strInfo5);
    RowAccount.SetInfo6(m_MD.m_strInfo6);
    RowAccount.SetInfo7(m_MD.m_strInfo7);
    RowAccount.SetInfo8(m_MD.m_strInfo8);
    RowAccount.SetTick1(m_MD.m_bTick1);
    RowAccount.SetTick2(m_MD.m_bTick2);
    RowAccount.SetTick3(m_MD.m_bTick3);
    RowAccount.SetTick4(m_MD.m_bTick4);
    RowAccount.SetAllergyCodes(m_MD.m_nAllergyCodes);
    RowAccount.SetDietary(m_MD.m_strDietary);
    RowAccount.SetInactive(m_MD.m_bInactive);
    /*****/
    RowAccount.SetMemberID(m_MD.m_strMemberID);
    RowAccount.SetExternalRef1(m_MD.m_strExternalRef1);
    RowAccount.SetExternalRef2(m_MD.m_strExternalRef2);
    RowAccount.SetPreviousUserID(m_MD.m_nPreviousUserID);
    RowAccount.SetNextUserID(m_MD.m_nNextUserID);
    RowAccount.SetLegacyPebblePurseID(m_MD.m_strLegacyPebblePurseID);
    /*****/
    RowAccount.SetPurse1Allow(m_MD.m_nPurse1Allow);
    RowAccount.SetPurse1Liability(atof(m_MD.m_strPurse1Liability));
	RowAccount.SetPurse1Credit(atof(m_MD.m_strPurse1Credit));
    RowAccount.SetPurse1LastSpendDate(m_MD.m_strPurse1LastSpendDate);
    RowAccount.SetPurse1LastSpendTime(m_MD.m_strPurse1LastSpendTime);
	RowAccount.SetPurse1LastSpend(atof(m_MD.m_strPurse1LastSpend));
	RowAccount.SetPurse1SpendToDate(atof(m_MD.m_strPurse1SpendToDate));
    /*****/
    RowAccount.SetPurse2Allow(m_MD.m_nPurse2Allow);
	RowAccount.SetPurse2Balance(atof(m_MD.m_strPurse2Balance));
    RowAccount.SetPurse2LastSpendDate(m_MD.m_strPurse2LastSpendDate);
    RowAccount.SetPurse2LastSpendTime(m_MD.m_strPurse2LastSpendTime);
	RowAccount.SetPurse2LastSpend(atof(m_MD.m_strPurse2LastSpend));
	RowAccount.SetPurse2SpendToDate(atof(m_MD.m_strPurse2SpendToDate));
    RowAccount.SetPurse2RefreshFlags(m_MD.m_nPurse2RefreshFlags);
    RowAccount.SetPurse2RefreshedDate(m_MD.m_strPurse2RefreshedDate);
    RowAccount.SetPurse2RefreshedTime(m_MD.m_strPurse2RefreshedTime);
    /*****/
    RowAccount.SetPurse3Allow(m_MD.m_nPurse3Allow);
	RowAccount.SetPurse3Liability(atof(m_MD.m_strPurse3Liability));
    RowAccount.SetPurse3Credit(atof(m_MD.m_strPurse3Credit));
    RowAccount.SetPurse3LastSpendDate(m_MD.m_strPurse3LastSpendDate);
    RowAccount.SetPurse3LastSpendTime(m_MD.m_strPurse3LastSpendTime);
	RowAccount.SetPurse3LastSpend(atof(m_MD.m_strPurse3LastSpend));
	RowAccount.SetPurse3SpendToDate(atof(m_MD.m_strPurse3SpendToDate));
    /*****/
    RowAccount.SetCashSpendToDate(atof(m_MD.m_strCashSpendToDate));
    /*****/
    RowAccount.SetPoints(m_MD.m_nPoints);
    RowAccount.SetPointsToDate(m_MD.m_nPointsToDate);
    /*****/
    RowAccount.SetOwnMaxSpend(m_MD.m_bOwnMaxSpend);
    /*****/
    RowAccount.SetMaxSpendPurse1Period1(atof(m_MD.m_strMaxSpend[0][0]));
	RowAccount.SetMaxSpendPurse1Period2(atof(m_MD.m_strMaxSpend[0][1]));
	RowAccount.SetMaxSpendPurse1Period3(atof(m_MD.m_strMaxSpend[0][2]));
	RowAccount.SetMaxSpendPurse1Period4(atof(m_MD.m_strMaxSpend[0][3]));
	RowAccount.SetMaxSpendPurse1Period5(atof(m_MD.m_strMaxSpend[0][4]));
	RowAccount.SetMaxSpendPurse1Period6(atof(m_MD.m_strMaxSpend[0][5]));
    /*****/
	RowAccount.SetMaxSpendPurse3Period1(atof(m_MD.m_strMaxSpend[1][0]));
	RowAccount.SetMaxSpendPurse3Period2(atof(m_MD.m_strMaxSpend[1][1]));
	RowAccount.SetMaxSpendPurse3Period3(atof(m_MD.m_strMaxSpend[1][2]));
	RowAccount.SetMaxSpendPurse3Period4(atof(m_MD.m_strMaxSpend[1][3]));
	RowAccount.SetMaxSpendPurse3Period5(atof(m_MD.m_strMaxSpend[1][4]));
	RowAccount.SetMaxSpendPurse3Period6(atof(m_MD.m_strMaxSpend[1][5]));
    /*****/
	RowAccount.SetMaxOverdraftPurse1Period1(atof(m_MD.m_strMaxOverdraft[0][0]));
	RowAccount.SetMaxOverdraftPurse1Period2(atof(m_MD.m_strMaxOverdraft[0][1]));
	RowAccount.SetMaxOverdraftPurse1Period3(atof(m_MD.m_strMaxOverdraft[0][2]));
	RowAccount.SetMaxOverdraftPurse1Period4(atof(m_MD.m_strMaxOverdraft[0][3]));
	RowAccount.SetMaxOverdraftPurse1Period5(atof(m_MD.m_strMaxOverdraft[0][4]));
	RowAccount.SetMaxOverdraftPurse1Period6(atof(m_MD.m_strMaxOverdraft[0][5]));
    /*****/
	RowAccount.SetMaxOverdraftPurse3Period1(atof(m_MD.m_strMaxOverdraft[1][0]));
	RowAccount.SetMaxOverdraftPurse3Period2(atof(m_MD.m_strMaxOverdraft[1][1]));
	RowAccount.SetMaxOverdraftPurse3Period3(atof(m_MD.m_strMaxOverdraft[1][2]));
	RowAccount.SetMaxOverdraftPurse3Period4(atof(m_MD.m_strMaxOverdraft[1][3]));
	RowAccount.SetMaxOverdraftPurse3Period5(atof(m_MD.m_strMaxOverdraft[1][4]));
	RowAccount.SetMaxOverdraftPurse3Period6(atof(m_MD.m_strMaxOverdraft[1][5]));
    /*****/
    RowAccount.SetAlertCode(m_MD.m_nAlertCode);
    RowAccount.SetAlertDate(m_MD.m_strAlertDate);
    RowAccount.SetAlertTime(m_MD.m_strAlertTime);
    /*****/
    RowAccount.SetRowVersion(m_arrayRowVersion);

    if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
    {
        switch (m_MD.m_nGroupShiftStatus)
        {
        case 1:
        case 2:
        {
            COleDateTime dateNow = COleDateTime::GetCurrentTime();
            CGroupShiftValidator GroupShiftValidator(dateNow, RowAccount);

            if (GroupShiftValidator.UpdateAccount(RowAccount) == TRUE)
            {
                GroupShiftLogger.LogSystemChange(RowAccount, GroupShiftValidator);
            }
        }
        break;
        }
    }
}

//**********************************************************************

void CSQLRecordSetAccountFull::SaveToAccountRow(CSQLRowAccountFind& RowAccount)
{
    RowAccount.SetLeaverYear(m_MD.m_nLeaverYear);
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetUsername(m_MD.m_strUsername);
    RowAccount.SetForename(m_MD.m_strForename);
    RowAccount.SetSurname(m_MD.m_strSurname);
    RowAccount.SetExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
    RowAccount.SetInfo1(m_MD.m_strInfo1);
    RowAccount.SetInfo2(m_MD.m_strInfo2);
    RowAccount.SetInfo3(m_MD.m_strInfo3);
    RowAccount.SetInfo4(m_MD.m_strInfo4);
    RowAccount.SetInfo5(m_MD.m_strInfo5);
    RowAccount.SetInfo6(m_MD.m_strInfo6);
    RowAccount.SetInfo7(m_MD.m_strInfo7);
    RowAccount.SetInfo8(m_MD.m_strInfo8);
    RowAccount.SetInactive(m_MD.m_bInactive);
    /*****/
    RowAccount.SetMemberID(m_MD.m_strMemberID);
    /*****/
    RowAccount.SetAlertCode(m_MD.m_nAlertCode);

    if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
    {
        switch (m_MD.m_nGroupShiftStatus)
        {
        case 1:
        case 2:
        {
            CSQLRowAccountFull RowAccountFull;
            RowAccountFull.SetGroupNo(m_MD.m_nGroupNo);
            RowAccountFull.SetGroupShiftStatus(m_MD.m_nGroupShiftStatus);
            RowAccountFull.SetGroupShiftStartDate(m_MD.m_strGroupShiftStartDate);
            RowAccountFull.SetGroupShiftEndDate(m_MD.m_strGroupShiftEndDate);
            RowAccountFull.SetGroupShiftFutureGroup(m_MD.m_nGroupShiftFutureGroup);

            COleDateTime dateNow = COleDateTime::GetCurrentTime();
            CGroupShiftValidator GroupShiftValidator(dateNow, RowAccountFull);

            if (GroupShiftValidator.UpdateAccount(RowAccountFull) == TRUE)
            {
                GroupShiftLogger.LogSystemChange(RowAccountFull, GroupShiftValidator);
            }

            RowAccount.SetGroupNo(RowAccountFull.GetGroupNo());
        }
        break;
        }
    }
}

//**********************************************************************

bool CSQLRecordSetAccountFull::StepSelectAll(CSQLRowAccountFull& RowAccount)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToAccountRow(RowAccount);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************

bool CSQLRecordSetAccountFull::StepSelectAll(CSQLRowAccountFull& RowAccount, int& nGroupSet)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToAccountRow(RowAccount);
    nGroupSet = m_MD.m_nGroupSet;
    MoveNext();

    return TRUE;
}

//**********************************************************************

bool CSQLRecordSetAccountFull::StepSelectAll(CSQLRowAccountFind& RowFind)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToAccountRow(RowFind);
    MoveNext();

    return TRUE;
}

//**********************************************************************
