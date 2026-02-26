//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CString strTableName, CDatabase* pDatabase, int nType, int nBindFields) : CSQLRecordSetConnectionPool(pDatabase)
{
    m_nRecordSetType = nType;
    m_strTableName = strTableName;
 
    switch (m_nRecordSetType)
    {
    case ACCOUNTSET_SEARCH:
        m_nFields = 1;
        m_nParams = nBindFields;
        break;

    case ACCOUNTSET_MEMBER:
        m_nFields = 1;
        m_nParams = 1;
        break;

    case ACCOUNTSET_RESET_EXTREF1:
    case ACCOUNTSET_RESET_EXTREF2:
    case ACCOUNTSET_RESET_MEMBERID:
        m_nFields = 2;
        m_nParams = 1;
        break;

    case ACCOUNTSET_NORMAL:
    default:
        m_nFields = 1;
        m_nParams = 2;
        m_nRecordSetType = ACCOUNTSET_NORMAL;
        break;
    }
}

//**********************************************************************

void CSQLRecordSetAccountUserID::DoFieldExchange(CFieldExchange* pFX)
{
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_BigInt(pFX, Account::UserID.Label, m_nUserID);

    switch (m_nRecordSetType)
    {
    case ACCOUNTSET_RESET_EXTREF1:
        RFX_Text(pFX, Account::ExternalRef1.Label, m_strMemberID);
        break;

    case ACCOUNTSET_RESET_EXTREF2:
        RFX_Text(pFX, Account::ExternalRef2.Label, m_strMemberID);
        break;

    case ACCOUNTSET_RESET_MEMBERID:
        RFX_Text(pFX, Account::MemberID.Label, m_strMemberID);
        break;
    }

    /*****/
    pFX->SetFieldType(CFieldExchange::inputParam);

    switch (m_nRecordSetType)
    {
    case ACCOUNTSET_SEARCH:
    {
        for (unsigned int n = 0; n < m_nParams; n++)
        {
            RFX_Text(pFX, "", m_strParamSearch[n]);
        }
        break;
    }

    case ACCOUNTSET_MEMBER:
    case ACCOUNTSET_RESET_EXTREF1:
    case ACCOUNTSET_RESET_EXTREF2:
    case ACCOUNTSET_RESET_MEMBERID:
        RFX_Text(pFX, "", m_strParamMemberID, 255, SQL_VARCHAR);
        break;

    case ACCOUNTSET_NORMAL:
    default:
        RFX_Text(pFX, "", m_strParamUserIDFrom);
        RFX_Text(pFX, "", m_strParamUserIDTo); 
        break;
    }
}

//**********************************************************************

void CSQLRecordSetAccountUserID::SetSearchBindings(CSQLBuilderBind& BuilderBind)
{
    for (int n = 0; n < BuilderBind.GetFieldCount() && n < 10; n++)
    {
        CSQLBuilderBindField Field;
        BuilderBind.GetField(n, Field);
        m_strParamSearch[n] = Field.GetFieldAsString();
    }
}

//**********************************************************************

bool CSQLRecordSetAccountUserID::StepSelect(__int64& nUserID, bool bUpdate)
{
    if (IsOpen() == FALSE)
    {
        UINT nOpenType = bUpdate ? CRecordset::dynaset : CRecordset::forwardOnly;
        DWORD dwOptions = bUpdate ? CRecordset::none : CRecordset::readOnly;

        if (Open(nOpenType, NULL, dwOptions) == FALSE)
        {
            return FALSE;
        }
    }
    else if (TRUE == bUpdate)
    {
        MoveNext();
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    nUserID = m_nUserID;
    
    if (FALSE == bUpdate)
    {
        MoveNext();
    }
    
    return TRUE;
}

//**********************************************************************
