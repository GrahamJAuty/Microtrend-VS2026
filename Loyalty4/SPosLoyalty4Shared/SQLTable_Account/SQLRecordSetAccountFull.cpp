//**********************************************************************
#include "..\SQLNamespaces.h"
#include "..\SQLRecordSetMetadata.h"
//**********************************************************************
#include "..\..\SPosLoyalty4Manager\SQLOut.h"
//**********************************************************************
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

CSQLRecordSetMetadataAccountFull::CSQLRecordSetMetadataAccountFull(RSDataType_AccountFull type, bool bForExistenceCheck)
{
    if (FALSE == bForExistenceCheck)
    {
        switch (type)
        {
        case RSDataType_AccountFull::Normal:
        case RSDataType_AccountFull::Export:
            break;

        default:
            type = RSDataType_AccountFull::Normal;
            break;
        }

        AddInt64Field(&m_nUserID, Account::UserID.Label);
        AddTextField(&m_strFullname, Account::Fullname.Label);
        AddTextField(&m_strForename, Account::Forename.Label);
        AddTextField(&m_strSurname, Account::Surname.Label);
        AddTextField(&m_strDOB, Account::DOB.Label);
        AddTextField(&m_strExpiryDate, Account::ExpiryDate.Label);
        AddTextField(&m_strGender, Account::Gender.Label);
        AddIntField(&m_nGroupNo, Account::GroupNo.Label);
        AddIntField(&m_nSchemeNo, Account::SchemeNo.Label);
        AddTextField(&m_strInfo1, Account::Info1.Label);
        AddTextField(&m_strInfo2, Account::Info2.Label);
        AddTextField(&m_strInfo3, Account::Info3.Label);
        AddTextField(&m_strInfo4, Account::Info4.Label);
        AddTextField(&m_strAddress1, Account::Address1.Label);
        AddTextField(&m_strAddress2, Account::Address2.Label);
        AddTextField(&m_strAddress3, Account::Address3.Label);
        AddTextField(&m_strAddress4, Account::Address4.Label);
        AddTextField(&m_strAddress5, Account::Address5.Label);
        AddTextField(&m_strPhone1, Account::Phone1.Label);
        AddTextField(&m_strPhone2, Account::Phone2.Label);
        AddBOOLField(&m_bInactive, Account::Inactive.Label);
        /*****/
        AddTextField(&m_strExternalRef, Account::ExternalRef.Label);
        AddTextField(&m_strQRInfo, Account::QRInfo.Label);
        AddInt64Field(&m_nPreviousUserID, Account::PreviousUserID.Label);
        AddInt64Field(&m_nNextUserID, Account::NextUserID.Label);
        /*****/
        AddDecimalField(&m_strPurse1Balance, Account::Purse1Balance.Label);
        AddTextField(&m_strPurse1LastSpendDate, Account::Purse1LastSpendDate.Label);
        AddTextField(&m_strPurse1LastSpendTime, Account::Purse1LastSpendTime.Label);
        AddDecimalField(&m_strPurse1LastSpend, Account::Purse1LastSpend.Label);
        AddDecimalField(&m_strPurse1SpendToDate, Account::Purse1SpendToDate.Label);
        /*****/
        AddDecimalField(&m_strPurse2Balance, Account::Purse2Balance.Label);
        AddTextField(&m_strPurse2LastSpendDate, Account::Purse2LastSpendDate.Label);
        AddTextField(&m_strPurse2LastSpendTime, Account::Purse2LastSpendTime.Label);
        AddDecimalField(&m_strPurse2LastSpend, Account::Purse2LastSpend.Label);
        AddDecimalField(&m_strPurse2SpendToDate, Account::Purse2SpendToDate.Label);
        AddTextField(&m_strPurse2RefreshedDate, Account::Purse2RefreshedDate.Label);
        AddTextField(&m_strPurse2RefreshedTime, Account::Purse2RefreshedTime.Label);
        /*****/
        AddTextField(&m_strCashLastSpendDate, Account::CashLastSpendDate.Label);
        AddTextField(&m_strCashLastSpendTime, Account::CashLastSpendTime.Label);
        AddDecimalField(&m_strCashLastSpend, Account::CashLastSpend.Label);
        AddDecimalField(&m_strCashSpendToDate, Account::CashSpendToDate.Label);
        /*****/
        AddIntField(&m_nPoints, Account::Points.Label);
        AddIntField(&m_nPointsToDate, Account::PointsToDate.Label);
        /*****/
        AddIntField(&m_nHotlistCode, Account::HotlistCode.Label);
        AddTextField(&m_strHotlistDate, Account::HotlistDate.Label);
        AddTextField(&m_strHotlistTime, Account::HotlistTime.Label);
        /*****/
        AddDecimalField(&m_strMaxSpend, Account::MaxSpend.Label);
        AddDecimalField(&m_strMaxOverdraft, Account::MaxOverdraft.Label);
        /*****/
        AddTextField(&m_strAlertCodes, Account::AlertCodes.Label);
        /*****/

        switch (type)
        {
        case RSDataType_AccountFull::Export:
            AddDecimalField(&m_strSQLTotalSpendToDate, Account::TotalSpendToDate.Label);
            break;
        }
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddParamsById(const CString strUserID)
{
    AddTextParam(Account::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo)
{
    AddTextParam(Account::UserID.Label, strUserIDFrom);
    AddTextParam(Account::UserID.Label, strUserIDTo);
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalById Params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_AccountFull::Normal, bForExistenceCheck)
{ 
    m_strFilter = "";
	m_bForExistenceCheck = bForExistenceCheck;
	m_MD.AddParamsById(Params.strUserID);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalByRange Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountFull::Normal)
{
    m_strFilter = "";
    m_bForExistenceCheck = FALSE;
	m_MD.AddParamsByRange(Params.strUserIDFrom, Params.strUserIDTo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_RowCountByRange Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountFull::Normal, TRUE)
{
    m_strFilter = "";
    m_bForExistenceCheck = TRUE;
	m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(Params.strUserIDFrom, Params.strUserIDTo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalByParams Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountFull::Normal)
{
	m_strFilter = Params.strWhere;
	m_bForExistenceCheck = FALSE;
    m_MD.BindGeneralParams(Params.pBind);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ExportByParams Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountFull::Export)
{
    m_strFilter = Params.strWhere;
    m_bForExistenceCheck = FALSE;
    m_MD.BindGeneralParams(Params.pBind);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalNoParams Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountFull::Normal)
{
    m_strFilter = "";
    m_bForExistenceCheck = FALSE;
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountFull::CommonInit()
{
    m_strSort = Account::UserID.Label;
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

CString CSQLRecordSetAccountFull::GetDefaultSQL()
{
    return SQLTableNames::Accounts;
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
    m_MD.m_nUserID = RowAccount.GetUserID();
    m_MD.m_strFullname = RowAccount.GetFullname();
    m_MD.m_strForename = RowAccount.GetForename();
    m_MD.m_strSurname = RowAccount.GetSurname();
    m_MD.m_strDOB = RowAccount.GetDOB();
    m_MD.m_strExpiryDate = RowAccount.GetExpiryDate();
    m_MD.m_strGender = RowAccount.GetGender();
    m_MD.m_nGroupNo = RowAccount.GetGroupNo();
    m_MD.m_nSchemeNo = RowAccount.GetSchemeNo();
    m_MD.m_strInfo1 = RowAccount.GetInfo1();
    m_MD.m_strInfo2 = RowAccount.GetInfo2();
    m_MD.m_strInfo3 = RowAccount.GetInfo3();
    m_MD.m_strInfo4 = RowAccount.GetInfo4();
    m_MD.m_strAddress1 = RowAccount.GetAddress1();
    m_MD.m_strAddress2 = RowAccount.GetAddress2();
    m_MD.m_strAddress3 = RowAccount.GetAddress3();
    m_MD.m_strAddress4 = RowAccount.GetAddress4();
    m_MD.m_strAddress5 = RowAccount.GetAddress5();
    m_MD.m_strPhone1 = RowAccount.GetPhone1();
    m_MD.m_strPhone2 = RowAccount.GetPhone2();
    m_MD.m_bInactive = RowAccount.GetInactive();
    /*****/
    m_MD.m_strExternalRef = RowAccount.GetExternalRef();
    m_MD.m_strQRInfo = RowAccount.GetQRInfo();
    m_MD.m_nPreviousUserID = RowAccount.GetPreviousUserID();
    m_MD.m_nNextUserID = RowAccount.GetNextUserID();
    /*****/
	SetDoubleString(m_MD.m_strPurse1Balance, RowAccount.GetPurse1Balance());
    m_MD.m_strPurse1LastSpendDate = RowAccount.GetPurse1LastSpendDate();
    m_MD.m_strPurse1LastSpendTime = RowAccount.GetPurse1LastSpendTime();
	SetDoubleString(m_MD.m_strPurse1LastSpend, RowAccount.GetPurse1LastSpend());
	SetDoubleString(m_MD.m_strPurse1SpendToDate, RowAccount.GetPurse1SpendToDate());
    /*****/
	SetDoubleString(m_MD.m_strPurse2Balance, RowAccount.GetPurse2Balance());
    m_MD.m_strPurse2LastSpendDate = RowAccount.GetPurse2LastSpendDate();
    m_MD.m_strPurse2LastSpendTime = RowAccount.GetPurse2LastSpendTime();
	SetDoubleString(m_MD.m_strPurse2LastSpend, RowAccount.GetPurse2LastSpend());
	SetDoubleString(m_MD.m_strPurse2SpendToDate, RowAccount.GetPurse2SpendToDate());
    m_MD.m_strPurse2RefreshedDate = RowAccount.GetPurse2RefreshedDate();
    m_MD.m_strPurse2RefreshedTime = RowAccount.GetPurse2RefreshedTime();
    /*****/
    m_MD.m_strCashLastSpendDate = RowAccount.GetCashLastSpendDate();
    m_MD.m_strCashLastSpendTime = RowAccount.GetCashLastSpendTime();
	SetDoubleString(m_MD.m_strCashLastSpend, RowAccount.GetCashLastSpend());
	SetDoubleString(m_MD.m_strCashSpendToDate, RowAccount.GetCashSpendToDate());
    /*****/
    m_MD.m_nPoints = RowAccount.GetPoints();
    m_MD.m_nPointsToDate = RowAccount.GetPointsToDate();
    /*****/
    m_MD.m_nHotlistCode = RowAccount.GetHotlistCode();
    m_MD.m_strHotlistDate = RowAccount.GetHotlistDate();
    m_MD.m_strHotlistTime = RowAccount.GetHotlistTime();
    /*****/
	SetDoubleString(m_MD.m_strMaxSpend, RowAccount.GetMaxSpend());
	SetDoubleString(m_MD.m_strMaxOverdraft, RowAccount.GetMaxOverdraft());
    /*****/
    m_MD.m_strAlertCodes = RowAccount.GetAlertCodes();
}

//**********************************************************************

void CSQLRecordSetAccountFull::SaveToAccountRow(CSQLRowAccountFull& RowAccount)
{
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetFullname(m_MD.m_strFullname);
    RowAccount.SetForename(m_MD.m_strForename);
    RowAccount.SetSurname(m_MD.m_strSurname);
    RowAccount.SetDOB(m_MD.m_strDOB);
    RowAccount.SetExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetGender(m_MD.m_strGender);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
	RowAccount.SetSchemeNo(m_MD.m_nSchemeNo);
    RowAccount.SetInfo1(m_MD.m_strInfo1);
    RowAccount.SetInfo2(m_MD.m_strInfo2);
    RowAccount.SetInfo3(m_MD.m_strInfo3);
    RowAccount.SetInfo4(m_MD.m_strInfo4);
    RowAccount.SetAddress1(m_MD.m_strAddress1);
    RowAccount.SetAddress2(m_MD.m_strAddress2);
    RowAccount.SetAddress3(m_MD.m_strAddress3);
    RowAccount.SetAddress4(m_MD.m_strAddress4);
    RowAccount.SetAddress5(m_MD.m_strAddress5);
    RowAccount.SetPhone1(m_MD.m_strPhone1);
    RowAccount.SetPhone2(m_MD.m_strPhone2);
    RowAccount.SetInactive(m_MD.m_bInactive);
    /*****/
    RowAccount.SetExternalRef(m_MD.m_strExternalRef);
    RowAccount.SetQRInfo(m_MD.m_strQRInfo);
    RowAccount.SetPreviousUserID(m_MD.m_nPreviousUserID);
    RowAccount.SetNextUserID(m_MD.m_nNextUserID);
    /*****/
    RowAccount.SetPurse1Balance(atof(m_MD.m_strPurse1Balance));
    RowAccount.SetPurse1LastSpendDate(m_MD.m_strPurse1LastSpendDate);
    RowAccount.SetPurse1LastSpendTime(m_MD.m_strPurse1LastSpendTime);
    RowAccount.SetPurse1LastSpend(atof(m_MD.m_strPurse1LastSpend));
    RowAccount.SetPurse1SpendToDate(atof(m_MD.m_strPurse1SpendToDate));
    /*****/
    RowAccount.SetPurse2Balance(atof(m_MD.m_strPurse2Balance));
    RowAccount.SetPurse2LastSpendDate(m_MD.m_strPurse2LastSpendDate);
    RowAccount.SetPurse2LastSpendTime(m_MD.m_strPurse2LastSpendTime);
    RowAccount.SetPurse2LastSpend(atof(m_MD.m_strPurse2LastSpend));
    RowAccount.SetPurse2SpendToDate(atof(m_MD.m_strPurse2SpendToDate));
    RowAccount.SetPurse2RefreshedDate(m_MD.m_strPurse2RefreshedDate);
    RowAccount.SetPurse2RefreshedTime(m_MD.m_strPurse2RefreshedTime);
    /*****/
    RowAccount.SetCashLastSpendDate(m_MD.m_strCashLastSpendDate);
    RowAccount.SetCashLastSpendTime(m_MD.m_strCashLastSpendTime);
    RowAccount.SetCashLastSpend(atof(m_MD.m_strCashLastSpend));
    RowAccount.SetCashSpendToDate(atof(m_MD.m_strCashSpendToDate));
    /*****/
    RowAccount.SetPoints(m_MD.m_nPoints);
    RowAccount.SetPointsToDate(m_MD.m_nPointsToDate);
    /*****/
    RowAccount.SetHotlistCode(m_MD.m_nHotlistCode);
    RowAccount.SetHotlistDate(m_MD.m_strHotlistDate);
    RowAccount.SetHotlistTime(m_MD.m_strHotlistTime);
    /*****/
    RowAccount.SetMaxSpend(atof(m_MD.m_strMaxSpend));
    RowAccount.SetMaxOverdraft(atof(m_MD.m_strMaxOverdraft));
    /*****/
    RowAccount.SetAlertCodes(m_MD.m_strAlertCodes);
    /*****/
    RowAccount.SetRowVersion(m_arrayRowVersion);
}

//**********************************************************************

void CSQLRecordSetAccountFull::SaveToAccountRow(CSQLRowAccountFind& RowAccount)
{
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetFullname(m_MD.m_strFullname);
    RowAccount.SetForename(m_MD.m_strForename);
    RowAccount.SetSurname(m_MD.m_strSurname);
    RowAccount.SetExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
	RowAccount.SetInfo1(m_MD.m_strInfo1);
    RowAccount.SetInfo2(m_MD.m_strInfo2);
    RowAccount.SetInfo3(m_MD.m_strInfo3);
    RowAccount.SetInfo4(m_MD.m_strInfo4);
    RowAccount.SetAddress1(m_MD.m_strAddress1);
    RowAccount.SetAddress2(m_MD.m_strAddress2);
    RowAccount.SetAddress3(m_MD.m_strAddress3);
    RowAccount.SetAddress4(m_MD.m_strAddress4);
    RowAccount.SetAddress5(m_MD.m_strAddress5);
    RowAccount.SetPhone1(m_MD.m_strPhone1);
    RowAccount.SetPhone2(m_MD.m_strPhone2);
    RowAccount.SetInactive(m_MD.m_bInactive);
    /*****/
    RowAccount.SetAlertCodes(m_MD.m_strAlertCodes);
    /*****/
    RowAccount.SetHotlistCode(m_MD.m_nHotlistCode);
    /*****/
    RowAccount.SetPurse1SpendToDate(atof(m_MD.m_strPurse1SpendToDate));
    RowAccount.SetPurse2SpendToDate(atof(m_MD.m_strPurse2SpendToDate));
    RowAccount.SetCashSpendToDate(atof(m_MD.m_strCashSpendToDate));
    RowAccount.SetPointsToDate(m_MD.m_nPointsToDate);
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

