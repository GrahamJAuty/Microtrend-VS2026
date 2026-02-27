//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPurchaseControlRule.h"
//**********************************************************************

CSQLRecordSetMetadataPurchaseControlRule::CSQLRecordSetMetadataPurchaseControlRule(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nRuleID, PurchaseControlRule::RuleID.Label, 1);
        AddBOOLField(&m_bEnabled, PurchaseControlRule::Enabled.Label);
        AddIntField(&m_nThreshold, PurchaseControlRule::Threshold.Label);
        AddIntField(&m_nExpiryDays, PurchaseControlRule::Expiry.Label);
        AddTextField(&m_strDescription, PurchaseControlRule::Description.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPurchaseControlRule::AddParamsByRuleID(const int nRuleID)
{
    AddIntParam(PurchaseControlRule::RuleID.Label, nRuleID);
}

//**********************************************************************

CSQLRecordSetPurchaseControlRule::CSQLRecordSetPurchaseControlRule(CDatabase* pDatabase, RSParams_PurchaseControlRule_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD() 
{   
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPurchaseControlRule::CSQLRecordSetPurchaseControlRule(CDatabase* pDatabase, RSParams_PurchaseControlRule_NormalByRuleId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByRuleID(params.nRuleId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPurchaseControlRule::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPurchaseControlRule::GetDefaultSQL()
{
    return SQLTableNames::PurchaseControlRules; 
}

//**********************************************************************

void CSQLRecordSetPurchaseControlRule::LoadFromPurchaseControlRuleRow(CSQLRowPurchaseControlRule& RowOS)
{
    m_MD.m_nRuleID = RowOS.GetRuleID();
    m_MD.m_bEnabled = RowOS.GetEnableFlag();
    m_MD.m_nThreshold = RowOS.GetThreshold();
    m_MD.m_nExpiryDays = RowOS.GetExpiryDays();
    m_MD.m_strDescription = RowOS.GetDescription();
}

//**********************************************************************

void CSQLRecordSetPurchaseControlRule::SaveToPurchaseControlRuleRow(CSQLRowPurchaseControlRule& RowOS)
{
    RowOS.SetRuleID(m_MD.m_nRuleID);
    RowOS.SetEnableFlag(m_MD.m_bEnabled);
    RowOS.SetThreshold(m_MD.m_nThreshold);
    RowOS.SetExpiryDays(m_MD.m_nExpiryDays);
    RowOS.SetDescription(m_MD.m_strDescription);
}

//**********************************************************************

bool CSQLRecordSetPurchaseControlRule::StepSelectAll(CSQLRowPurchaseControlRule& RowOS)
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

    SaveToPurchaseControlRuleRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
