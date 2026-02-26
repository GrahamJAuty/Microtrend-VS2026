//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetOfferStamp.h"
//**********************************************************************

CSQLRecordSetMetadataOfferStamp::CSQLRecordSetMetadataOfferStamp(bool bForRowCount) : CSQLRecordSetMetadata()
{
    if (FALSE == bForRowCount)
    {
        AddInt64Field(&m_nUserID, OfferStamp::UserID.Label, 1);
        AddIntField(&m_nStampOfferID, OfferStamp::StampOfferID.Label, 1);
        AddIntField(&m_nEdition, OfferStamp::Edition.Label);
        AddTextField(&m_strAwardDate, OfferStamp::AwardDate.Label);
        AddTextField(&m_strExpireDate, OfferStamp::ExpireDate.Label);
        AddIntField(&m_nStampCount, OfferStamp::StampCount.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataOfferStamp::AddParamsByUserIDRowCount(const CString strUserID)
{
    AddTextParam(OfferStamp::UserID.Label, strUserID);
    AddTextParam(OfferStamp::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataOfferStamp::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(OfferStamp::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataOfferStamp::AddParamsByUserIDOfferID(const CString strUserID, const int nOfferID)
{
	AddParamsByUserID(strUserID);
	AddIntParam(OfferStamp::StampOfferID.Label, nOfferID);
}

//**********************************************************************

void CSQLRecordSetMetadataOfferStamp::AddParamsByUserIDOfferIDEdition(const CString strUserID, const int nOfferID, const int nEdition)
{
	AddParamsByUserIDOfferID(strUserID, nOfferID);
 	AddIntParam(OfferStamp::Edition.Label, nEdition);
}

//**********************************************************************

void CSQLRecordSetMetadataOfferStamp::AddParamsByUserIDOfferIDEditionDates(const CString strUserID, const int nOfferID, const int nEdition, const CString strAwardDate, const CString strExpireDate)
{
	AddParamsByUserIDOfferIDEdition(strUserID, nOfferID, nEdition);
	AddTextParam(OfferStamp::AwardDate.Label, strAwardDate);
	AddTextParam(OfferStamp::ExpireDate.Label, strExpireDate);
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	m_MD.AddParamsByUserID(params.strUserId);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_RowCountByUserId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
    m_MD.AddParamsByUserIDRowCount(params.strUserId);
    m_bRangeFilter = TRUE;
    CommonInit();
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserIdOfferId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserIDOfferID(params.strUserId, params.nOfferId);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserIdOfferIdEdition params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserIDOfferIDEdition(params.strUserId, params.nOfferId, params.nEdition);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetOfferStamp::CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByFullKey params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserIDOfferIDEditionDates(params.strUserId, params.nOfferId, params.nEdition, params.strAwardDate, params.strExpireDate);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetOfferStamp::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetOfferStamp::GetDefaultSQL()
{
    return SQLTableNames::OfferStamps; 
}

//**********************************************************************

void CSQLRecordSetOfferStamp::LoadFromOfferStampRow(CSQLRowOfferStamp& RowOS)
{
    m_MD.m_nUserID = RowOS.GetUserID();
    m_MD.m_nStampOfferID = RowOS.GetStampOfferID();
    m_MD.m_nEdition = RowOS.GetEdition();
    m_MD.m_strAwardDate = RowOS.GetAwardDate();
    m_MD.m_strExpireDate = RowOS.GetExpireDate();
    m_MD.m_nStampCount = RowOS.GetStampCount();
}

//**********************************************************************

void CSQLRecordSetOfferStamp::SaveToOfferStampRow(CSQLRowOfferStamp& RowOS)
{
    RowOS.SetUserID(m_MD.m_nUserID);
    RowOS.SetStampOfferID(m_MD.m_nStampOfferID);
    RowOS.SetEdition(m_MD.m_nEdition);
    RowOS.SetAwardDate(m_MD.m_strAwardDate);
    RowOS.SetExpireDate(m_MD.m_strExpireDate);
    RowOS.SetStampCount(m_MD.m_nStampCount);
}

//**********************************************************************

bool CSQLRecordSetOfferStamp::StepSelectAll(CSQLRowOfferStamp& RowOS)
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

    SaveToOfferStampRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
