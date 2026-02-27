//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
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
    AddIntField(&m_nGroupNo, Account::GroupNo.Label);
    AddTextField(&m_strUsername, Account::Username.Label);
    AddTextField(&m_strForename, Account::Forename.Label);
    AddTextField(&m_strExpiryDate, Account::ExpiryDate.Label);
    AddBOOLField(&m_bInactive, Account::Inactive.Label);
    AddIntField(&m_nAlertCode, Account::AlertCode.Label);

    switch (m_type)
    {
    case RSDataType_AccountList::CardPicker:
        AddTextField(&m_strMemberID, Account::MemberID.Label);
        AddTextField(&m_strInfo1, Account::Info1.Label);
        break;

    case RSDataType_AccountList::Normal:
        AddTextField(&m_strSurname, Account::Surname.Label);
        AddIntField(&m_nGroupShiftStatus, Account::GroupShiftStatus.Label);
        AddTextField(&m_strGroupShiftStartDate, Account::GroupShiftStartDate.Label);
        AddTextField(&m_strGroupShiftEndDate, Account::GroupShiftEndDate.Label);
        AddIntField(&m_nGroupShiftFutureGroup, Account::GroupShiftFutureGroup.Label);
        AddDecimalField(&m_strPurse1Liability, Account::Purse1Liability.Label);
        AddDecimalField(&m_strPurse1Credit, Account::Purse1Credit.Label);
        AddDecimalField(&m_strPurse2Balance, Account::Purse2Balance.Label);
        AddDecimalField(&m_strPurse3Liability, Account::Purse3Liability.Label);
        AddDecimalField(&m_strPurse3Credit, Account::Purse3Credit.Label);
        AddIntField(&m_nPoints, Account::Points.Label);
        break;
    }
}

//**********************************************************************

CSQLRecordSetAccountList::CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_AccountList::Normal)
{
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
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
    RowAccount.SetUsername(m_MD.m_strUsername);
    RowAccount.SetForename(m_MD.m_strForename);
    RowAccount.SetExpiryDate(m_MD.m_strExpiryDate);
    RowAccount.SetInactive(m_MD.m_bInactive);
    RowAccount.SetAlertCode(m_MD.m_nAlertCode);
   
    /*****/

    switch (m_type)
    {
    case RSDataType_AccountList::Normal:
        RowAccount.SetSurname(m_MD.m_strSurname);
        //NO GROUP SHIFT STATUS IN ROW
        RowAccount.SetPurse1Credit(atof(m_MD.m_strPurse1Credit));
        RowAccount.SetPurse1Liability(atof(m_MD.m_strPurse1Liability));
        RowAccount.SetPurse2Balance(atof(m_MD.m_strPurse2Balance));
        RowAccount.SetPurse3Credit(atof(m_MD.m_strPurse3Credit));
        RowAccount.SetPurse3Liability(atof(m_MD.m_strPurse3Liability));
        RowAccount.SetPoints(m_MD.m_nPoints);
        break;

	case RSDataType_AccountList::CardPicker:
        RowAccount.SetMemberID(m_MD.m_strMemberID);
        RowAccount.SetInfo1(m_MD.m_strInfo1);
        break;
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