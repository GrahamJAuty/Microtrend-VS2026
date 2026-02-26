//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetStampOfferInfo.h"
//**********************************************************************

CSQLRecordSetMetadataStampOfferInfo::CSQLRecordSetMetadataStampOfferInfo() : CSQLRecordSetMetadata()
{
    AddIntField(&m_nStampOfferID, StampOfferInfo::StampOfferID.Label, 1);
    AddIntField(&m_nEdition, StampOfferInfo::Edition.Label);
    AddBOOLField(&m_bEnabled, StampOfferInfo::Enabled.Label, FALSE);
    AddIntField(&m_nThreshold, StampOfferInfo::Threshold.Label);
    AddIntField(&m_nExpiry, StampOfferInfo::Expiry.Label);
    AddTextField(&m_strDescription, StampOfferInfo::Description.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataStampOfferInfo::AddParamsByOfferID(const int nOfferID)
{
    AddIntParam(StampOfferInfo::StampOfferID.Label, nOfferID);
}

//**********************************************************************

CSQLRecordSetStampOfferInfo::CSQLRecordSetStampOfferInfo(CDatabase* pDatabase, RSParams_StampOffer_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetStampOfferInfo::CSQLRecordSetStampOfferInfo(CDatabase* pDatabase, RSParams_StampOffer_NormalByOfferId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByOfferID(params.nOfferId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetStampOfferInfo::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetStampOfferInfo::GetDefaultSQL()
{
    return SQLTableNames::StampOfferInfo; 
}

//**********************************************************************

void CSQLRecordSetStampOfferInfo::LoadFromStampOfferInfoRow(CSQLRowStampOfferInfo& RowOS)
{
    m_MD.m_nStampOfferID = RowOS.GetStampOfferID();
    m_MD.m_nEdition = RowOS.GetEdition();
    m_MD.m_bEnabled = RowOS.GetEnableFlag();
    m_MD.m_nThreshold = RowOS.GetThreshold();
    m_MD.m_nExpiry = RowOS.GetExpiry();
    m_MD.m_strDescription = RowOS.GetDescription();
}

//**********************************************************************

void CSQLRecordSetStampOfferInfo::SaveToStampOfferInfoRow(CSQLRowStampOfferInfo& RowOS)
{
    RowOS.SetStampOfferID(m_MD.m_nStampOfferID);
    RowOS.SetEdition(m_MD.m_nEdition);
    RowOS.SetEnableFlag(m_MD.m_bEnabled);
    RowOS.SetThreshold(m_MD.m_nThreshold);
    RowOS.SetExpiry(m_MD.m_nExpiry);
    RowOS.SetDescription(m_MD.m_strDescription);
}

//**********************************************************************

bool CSQLRecordSetStampOfferInfo::StepSelectAll(CSQLRowStampOfferInfo& RowOS)
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

    SaveToStampOfferInfoRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
