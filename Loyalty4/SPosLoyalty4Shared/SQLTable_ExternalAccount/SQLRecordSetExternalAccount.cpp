//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetExternalAccount.h"
//**********************************************************************

CSQLRecordSetMetadataExternalAccount::CSQLRecordSetMetadataExternalAccount(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strAESUsername, ExternalAccount::Username.Label);
        AddTextField(&m_strPasswordHash, ExternalAccount::Password.Label);
        AddTextField(&m_strAESUserID, ExternalAccount::UserID.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataExternalAccount::AddParamsByUsername(const CString strUsername)
{
    AddTextParam(ExternalAccount::Username.Label, strUsername);
}

//**********************************************************************

void CSQLRecordSetMetadataExternalAccount::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(ExternalAccount::UserID.Label, strUserID);
}

//**********************************************************************

CSQLRecordSetExternalAccount::CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetExternalAccount::CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalByUsername params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByUsername(params.strUsername);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetExternalAccount::CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalByUserId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	m_MD.AddParamsByUserID(params.strUserId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetExternalAccount::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetExternalAccount::GetDefaultSQL()
{ 
    return SQLTableNames::ExternalAccounts; 
}

//**********************************************************************

void CSQLRecordSetExternalAccount::LoadFromExternalAccountRow(CSQLRowExternalAccount& RowAccount)
{
    m_MD.m_strAESUsername = RowAccount.GetAESUsername();
    m_MD.m_strPasswordHash = RowAccount.GetPasswordHash();
    m_MD.m_strAESUserID = RowAccount.GetAESUserID();
}

//**********************************************************************

void CSQLRecordSetExternalAccount::SaveToExternalAccountRow(CSQLRowExternalAccount& RowAccount)
{
    RowAccount.SetAESUsername(m_MD.m_strAESUsername);
    RowAccount.SetPasswordHash(m_MD.m_strPasswordHash);
    RowAccount.SetAESUserID(m_MD.m_strAESUserID);
}

//**********************************************************************

bool CSQLRecordSetExternalAccount::StepSelectAll(CSQLRowExternalAccount& RowAccount)
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

    SaveToExternalAccountRow(RowAccount);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
