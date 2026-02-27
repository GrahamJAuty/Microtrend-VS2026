//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPurchaseControlStamp.h"
//**********************************************************************

CSQLRecordSetMetadataPurchaseControlStamp::CSQLRecordSetMetadataPurchaseControlStamp(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nUserID, PurchaseControlStamp::UserID.Label, 1);
        AddIntField(&m_nRuleID, PurchaseControlStamp::RuleID.Label, 1);
        AddTextField(&m_strAwardDate, PurchaseControlStamp::AwardDate.Label);
        AddTextField(&m_strExpireDate, PurchaseControlStamp::ExpireDate.Label);
        AddIntField(&m_nStampCount, PurchaseControlStamp::StampCount.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseControlStamp::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(PurchaseControlStamp::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseControlStamp::AddParamsForRowCount(const CString strUserID)
{
    AddTextParam(PurchaseControlStamp::UserID.Label, strUserID);
    AddTextParam(PurchaseControlStamp::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseControlStamp::AddParamsByUserIDRuleID(const CString strUserID, const int nRuleID)
{
	AddParamsByUserID(strUserID);
    AddIntParam(PurchaseControlStamp::RuleID.Label, nRuleID);
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseControlStamp::AddParamsByUserIDRuleIDDates(const CString strUserID, const int nRuleID, const CString strAwardDate, const CString strExpireDate)
{
    AddParamsByUserIDRuleID(strUserID, nRuleID);
    AddTextParam(PurchaseControlStamp::AwardDate.Label, strAwardDate);
    AddTextParam(PurchaseControlStamp::ExpireDate.Label, strExpireDate);
}

//**********************************************************************

CSQLRecordSetPurchaseControlStamp::CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseControlStamp::CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserID params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserID(params.strUserID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseControlStamp::CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_CountRowsByUserID params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
    m_bRangeFilter = TRUE;
    m_MD.AddParamsForRowCount(params.strUserID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseControlStamp::CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserIDRuleID params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserIDRuleID(params.strUserID, params.nRuleID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseControlStamp::CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByUserIDRuleIDDates(params.strUserID, params.nRuleID, params.strAwardDate, params.strExpireDate);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetPurchaseControlStamp::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPurchaseControlStamp::GetDefaultSQL()
{
    return SQLTableNames::PurchaseControlStamps; 
}

//**********************************************************************

void CSQLRecordSetPurchaseControlStamp::LoadFromPurchaseControlStampRow(CSQLRowPurchaseControlStamp& RowOS)
{
    m_MD.m_nUserID = RowOS.GetUserID();
    m_MD.m_nRuleID = RowOS.GetRuleID();
    m_MD.m_strAwardDate = RowOS.GetAwardDate();
    m_MD.m_strExpireDate = RowOS.GetExpireDate();
    m_MD.m_nStampCount = RowOS.GetStampCount();
}

//**********************************************************************

void CSQLRecordSetPurchaseControlStamp::SaveToPurchaseControlStampRow(CSQLRowPurchaseControlStamp& RowOS)
{
    RowOS.SetUserID(m_MD.m_nUserID);
    RowOS.SetRuleID(m_MD.m_nRuleID);
    RowOS.SetAwardDate(m_MD.m_strAwardDate);
    RowOS.SetExpireDate(m_MD.m_strExpireDate);
    RowOS.SetStampCount(m_MD.m_nStampCount);
}

//**********************************************************************

bool CSQLRecordSetPurchaseControlStamp::StepSelectAll(CSQLRowPurchaseControlStamp& RowOS)
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

    SaveToPurchaseControlStampRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
