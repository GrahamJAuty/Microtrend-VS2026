//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPurchaseHistoryHeader.h"
//**********************************************************************

CSQLRecordSetMetadataPurchaseHistoryHeader::CSQLRecordSetMetadataPurchaseHistoryHeader(bool bForExistenceCheck, bool bGetLastId) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, PurchaseHistoryHeader::LineID.Label);

        if (FALSE == bGetLastId)
        {
            AddTextField(&m_strDate, PurchaseHistoryHeader::Date.Label);
            AddTextField(&m_strTime, PurchaseHistoryHeader::Time.Label);
        }
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseHistoryHeader::AddLineIDField()
{
    AddInt64Field(&m_nLineID, PurchaseHistoryHeader::LineID.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseHistoryHeader::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(PurchaseHistoryHeader::LineID.Label, strLineID);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseHistoryHeader::AddTextParam(CString strLabel, const CString strDef)
{
    CSQLRecordSetMetadata::AddTextParam(strLabel, strDef);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseHistoryHeader::AddIntParam(CString strLabel, const int nDef)
{
    CSQLRecordSetMetadata::AddIntParam(strLabel, nDef);
}

//**********************************************************************

CSQLRecordSetPurchaseHistoryHeader::CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseHistoryHeader::CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_AddWithAutoLineID params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
	m_MD.AddLineIDField();
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseHistoryHeader::CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.strLineId);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseHistoryHeader::CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_GetLastId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(FALSE, TRUE)
{
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetPurchaseHistoryHeader::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPurchaseHistoryHeader::GetDefaultSQL()
{
    return SQLTableNames::PurchaseHistoryHeader;
}

//**********************************************************************

void CSQLRecordSetPurchaseHistoryHeader::LoadFromPurchaseHistoryHeaderRow(CSQLRowPurchaseHistoryHeader& RowBC)
{
    m_MD.m_nLineID = RowBC.GetLineID();
    m_MD.m_strDate = RowBC.GetDate();
	m_MD.m_strTime = RowBC.GetTime();
}

//**********************************************************************

void CSQLRecordSetPurchaseHistoryHeader::SaveToPurchaseHistoryHeaderRow(CSQLRowPurchaseHistoryHeader& RowBC)
{
    RowBC.SetLineID(m_MD.m_nLineID);
    RowBC.SetDate(m_MD.m_strDate);
    RowBC.SetTime(m_MD.m_strTime);
}

//**********************************************************************

bool CSQLRecordSetPurchaseHistoryHeader::StepSelectAll(CSQLRowPurchaseHistoryHeader& RowBC)
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

    SaveToPurchaseHistoryHeaderRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
