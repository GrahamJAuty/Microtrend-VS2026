//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\GlobalState.h"
//**********************************************************************
#include "SQLRepositoryAccount.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************

CSQLRecordSetMetadataAccountUserID::CSQLRecordSetMetadataAccountUserID(RSDataType_AccountUserId type)
{
    switch (type)
    {
    case RSDataType_AccountUserId::Normal:
    case RSDataType_AccountUserId::ResetMemberId:
    case RSDataType_AccountUserId::ResetExtRef1:
    case RSDataType_AccountUserId::ResetExtRef2:
        break;

    default:
        type = RSDataType_AccountUserId::Normal;
        break;
    }

    AddInt64Field(&m_nUserID, Account::UserID.Label);

    switch (type)
    {
    case RSDataType_AccountUserId::ResetExtRef1:
        AddTextField(&m_strMemberID, Account::ExternalRef1.Label);
        break;

    case RSDataType_AccountUserId::ResetExtRef2:
        AddTextField(&m_strMemberID, Account::ExternalRef2.Label);
        break;

    case RSDataType_AccountUserId::ResetMemberId:
        AddTextField(&m_strMemberID, Account::MemberID.Label);
        break;
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddNoParams()
{
    if(g_GlobalState.IsLeaverMode() == TRUE)
    {
        AddIntParam(Account::LeaverYear.Label, g_GlobalState.GetLeaverYear());
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByMemberID(const CString strMemberID)
{
    AddTextParam(Account::MemberID.Label, strMemberID);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByExtRef1(const CString strExtRef1)
{
    AddTextParam(Account::ExternalRef1.Label, strExtRef1);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByExtRef2(const CString strExtRef2)
{
    AddTextParam(Account::ExternalRef2.Label, strExtRef2);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo)
{
    AddTextParam(Account::UserID.Label, strUserIDFrom);
    AddTextParam(Account::UserID.Label, strUserIDTo);
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    m_MD.AddNoParams();
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_DetailByRange params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(params.strUserIDFrom, params.strUserIDTo);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = params.strWhere;
    m_MD.BindGeneralParams(params.pBind);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByMemberId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    m_MD.AddParamsByMemberID(params.strMemberID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetMemberIdByMemberId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::ResetMemberId)
{
    m_strFilter = "";
    m_MD.AddParamsByMemberID(params.strMemberID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRef1ByExtRef1 params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::ResetExtRef1)
{
    m_strFilter = "";
    m_MD.AddParamsByExtRef1(params.strExtRef1);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRef2ByExtRef2 params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::ResetExtRef2)
{
    m_strFilter = "";
    m_MD.AddParamsByExtRef2(params.strExtRef2);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountUserID::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);

    if (m_strFilter == "")
    {
        SetInitialFilterString();
    }
}

//**********************************************************************

CString CSQLRecordSetAccountUserID::GetDefaultSQL()
{
    if (g_GlobalState.IsLeaverMode() == TRUE)
    {
        return SQLTableNames::Leavers;
    }
    else
    {
        return SQLTableNames::Accounts;
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

    nUserID = m_MD.m_nUserID;
    
    if (FALSE == bUpdate)
    {
        MoveNext();
    }
    
    return TRUE;
}

//**********************************************************************
