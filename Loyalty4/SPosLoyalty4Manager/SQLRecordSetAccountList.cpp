//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAccountList.h"
//**********************************************************************

CSQLRecordSetMetadataAccountList::CSQLRecordSetMetadataAccountList(RSDataType_AccountList type)
{
    switch (type)
    {
    case RSDataType_AccountList::Normal:
    case RSDataType_AccountList::CardPicker:
        break;

    default:
        type = RSDataType_AccountList::Normal;
        break;
    }

    m_type = type;

    AddInt64Field(&m_nUserID, Account::UserID.Label);
    AddTextField(&m_strFullname, Account::Fullname.Label);
    AddIntField(&m_nGroupNo, Account::GroupNo.Label);
    AddTextField(&m_strExpiryDate, Account::ExpiryDate.Label);
    AddTextField(&m_strInfo1, Account::Info1.Label);
    AddTextField(&m_strAddress1, Account::Address1.Label);
    AddBOOLField(&m_bInactive, Account::Inactive.Label);
    AddIntField(&m_nHotlistCode, Account::HotlistCode.Label);

    if (RSDataType_AccountList::Normal == type)
    {
        AddTextField(&m_strInfo2, Account::Info2.Label);
        AddTextField(&m_strInfo3, Account::Info3.Label);
        AddTextField(&m_strInfo4, Account::Info4.Label);
        AddTextField(&m_strAddress5, Account::Address5.Label);
        AddDecimalField(&m_strPurse1Balance, Account::Purse1Balance.Label);
        AddDecimalField(&m_strPurse1SpendToDate, Account::Purse1SpendToDate.Label);
        AddDecimalField(&m_strPurse2Balance, Account::Purse2Balance.Label);
        AddDecimalField(&m_strPurse2SpendToDate, Account::Purse2SpendToDate.Label);
        AddDecimalField(&m_strCashSpendToDate, Account::CashSpendToDate.Label);
        AddIntField(&m_nPoints, Account::Points.Label);
        AddIntField(&m_nPointsToDate, Account::PointsToDate.Label);
        AddTextField(&m_strAlertCodes, Account::AlertCodes.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountList::AddParamsById(const CString strUserId)
{
    AddTextParam(Account::UserID.Label, strUserId);
}

//**********************************************************************

CSQLRecordSetAccountList::CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_AccountList::Normal)
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountList::CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalById params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountList::Normal)
{
    m_MD.AddParamsById(params.strUserID);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountList::CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_CardPickerNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountList::CardPicker)
{
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountList::CommonInit()
{
    m_strSort = Account::UserID.Label;
    m_type = m_MD.m_type;
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetAccountList::GetDefaultSQL()
{
    return SQLTableNames::Accounts; 
}

//**********************************************************************

void CSQLRecordSetAccountList::SaveToAccountRow(CSQLRowAccountList& RowAccount)
{
    RowAccount.SetACListUserID(m_MD.m_nUserID);
    RowAccount.SetACListFullname(m_MD.m_strFullname);
    RowAccount.SetACListGroupNo(m_MD.m_nGroupNo);
    RowAccount.SetACListExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetACListInfo1(m_MD.m_strInfo1);
    RowAccount.SetACListAddress1(m_MD.m_strAddress1);
    RowAccount.SetACListInactive(m_MD.m_bInactive);
    RowAccount.SetACListHotlistCode(m_MD.m_nHotlistCode);

    if (RSDataType_AccountList::Normal == m_type)
    {
        RowAccount.SetACListInfo2(m_MD.m_strInfo2);
        RowAccount.SetACListInfo3(m_MD.m_strInfo3);
        RowAccount.SetACListInfo4(m_MD.m_strInfo4);
        RowAccount.SetACListAddress5(m_MD.m_strAddress5);
        RowAccount.SetACListPurse1Balance(atof(m_MD.m_strPurse1Balance));
        RowAccount.SetACListPurse1SpendToDate(atof(m_MD.m_strPurse1SpendToDate));
        RowAccount.SetACListPurse2Balance(atof(m_MD.m_strPurse2Balance));
        RowAccount.SetACListPurse2SpendToDate(atof(m_MD.m_strPurse2SpendToDate));
        RowAccount.SetACListCashSpendToDate(atof(m_MD.m_strCashSpendToDate));
        RowAccount.SetACListPoints(m_MD.m_nPoints);
        RowAccount.SetACListPointsToDate(m_MD.m_nPointsToDate);
        RowAccount.SetACListAlertCodes(m_MD.m_strAlertCodes);
    }
}

//**********************************************************************

bool CSQLRecordSetAccountList::StepSelectAll(CSQLRowAccountList& RowAccount)
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
