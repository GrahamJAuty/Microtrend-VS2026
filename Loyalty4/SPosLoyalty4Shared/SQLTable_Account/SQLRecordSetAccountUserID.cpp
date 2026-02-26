//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************

CSQLRecordSetMetadataAccountUserID::CSQLRecordSetMetadataAccountUserID(RSDataType_AccountUserId type)
{
    switch (type)
    {
    case RSDataType_AccountUserId::Normal:
    case RSDataType_AccountUserId::WithQRInfo:
    case RSDataType_AccountUserId::WithExtRef:
        break;

    default:
        type = RSDataType_AccountUserId::Normal;
        break;
    }

    AddInt64Field(&m_nUserID, Account::UserID.Label);

    switch (type)
    {
    case RSDataType_AccountUserId::WithQRInfo:
        AddTextField(&m_strMemberID, Account::QRInfo.Label);
        break;

    case RSDataType_AccountUserId::WithExtRef:
        AddTextField(&m_strMemberID, Account::ExternalRef.Label);
        break;
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo)
{
    AddTextParam(Account::UserID.Label, strUserIDFrom);
    AddTextParam(Account::UserID.Label, strUserIDTo);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByInfo2(const CString strInfo2)
{
    AddTextParam(Account::Info2.Label, strInfo2);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByExtRef(const CString strExtRef)
{
    AddTextParam(Account::ExternalRef.Label, strExtRef);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByQRInfo(const CString strQRInfo)
{
    AddTextParam(Account::QRInfo.Label, strQRInfo);
}

//**********************************************************************

void CSQLRecordSetMetadataAccountUserID::AddParamsByMemberId(const CString strMemberId)
{
    //calling code may create where string with QRInfo or ExtRef label
	AddTextParam(Account::ExternalRef.Label,strMemberId);
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByParams Params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_AccountUserId::Normal)
{
	m_strFilter = Params.strWhere;
    m_MD.BindGeneralParams(Params.pBind);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByRange Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
	m_strFilter = "";
    m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(Params.strFrom, Params.strTo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByInfo2 Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
	m_strFilter = "";
    m_MD.AddParamsByInfo2(Params.strInfo2);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByExtRef Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    m_MD.AddParamsByExtRef(Params.strExtRef);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByMemberID Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    m_MD.AddParamsByMemberId(Params.strMemberID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalNoParams Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::Normal)
{
    m_strFilter = "";
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRefByExtRef Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::WithExtRef)
{
    m_strFilter = "";
    m_MD.AddParamsByExtRef(Params.strExtRef);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetAccountUserID::CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetQRInfoByQRInfo Params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_AccountUserId::WithQRInfo)
{
    m_strFilter = "";
    m_MD.AddParamsByQRInfo(Params.strQRInfo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountUserID::CommonInit()
{
    m_strSort = Account::UserID.Label;
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
    return SQLTableNames::Accounts;
}

//**********************************************************************

bool CSQLRecordSetAccountUserID::StepSelect(__int64& nUserID, bool bUpdate )
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
