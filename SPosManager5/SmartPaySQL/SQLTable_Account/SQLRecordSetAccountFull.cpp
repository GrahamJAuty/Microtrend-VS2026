//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLPrepStatAccount.h"
//**********************************************************************
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

CSQLRecordSetMetadataAccountFull::CSQLRecordSetMetadataAccountFull()
{   
    AddInt64Field(&m_nUserID, Account::UserID.Label);
    AddIntField(&m_nGroupNo, Account::GroupNo.Label);
    AddTextField(&m_strInfo3, Account::Info3.Label);
    AddTextField(&m_strInfo4, Account::Info4.Label);
}
  
//**********************************************************************

void CSQLRecordSetMetadataAccountFull::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(Account::UserID.Label, strUserID);
}

//**********************************************************************

CSQLRecordSetAccountFull::CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ById params)
    : CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
    m_strFilter = "";
	m_MD.AddParamsByUserID(params.strUserID);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountFull::CommonInit()
{
	m_strAccountTableName = SQLTableNames::Accounts;
    m_strMyDefaultSQL = m_strAccountTableName;

    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();

    SetMetadata(&m_MD);
    SetBasicFilterString();
}

//**********************************************************************

void CSQLRecordSetAccountFull::DoFieldExchange(CFieldExchange* pFX)
{
    ExchangeFields(pFX);
    ExchangeParams(pFX);
}

//**********************************************************************

void CSQLRecordSetAccountFull::LoadFromAccountRow(CSQLRowAccountFull& RowAccount)
{
    m_MD.m_nUserID = RowAccount.GetUserID();
    m_MD.m_nGroupNo = RowAccount.GetGroupNo();
    m_MD.m_strInfo3 = RowAccount.GetInfo3();
    m_MD.m_strInfo4 = RowAccount.GetInfo4();
}

//**********************************************************************

void CSQLRecordSetAccountFull::SaveToAccountRow(CSQLRowAccountFull& RowAccount)
{
    RowAccount.SetUserID(m_MD.m_nUserID);
    RowAccount.SetGroupNo(m_MD.m_nGroupNo);
    RowAccount.SetInfo3(m_MD.m_strInfo3);
    RowAccount.SetInfo4(m_MD.m_strInfo4);
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


