//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetClosingBalance.h"
//**********************************************************************

CSQLRecordSetMetadataClosingBalance::CSQLRecordSetMetadataClosingBalance(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strKeyDate, ClosingBalance::KeyDate.Label);
        AddTextField(&m_strDate, ClosingBalance::Date.Label);
        AddTextField(&m_strTime, ClosingBalance::Time.Label);
        AddTextField(&m_strArchiveDate, ClosingBalance::ArchiveDate.Label);
        AddTextField(&m_strArchiveTime, ClosingBalance::ArchiveTime.Label);
        AddDecimalField(&m_strPurse1LiabilityAudit, ClosingBalance::Purse1LiabilityAudit.Label);
        AddDecimalField(&m_strPurse1CreditAudit, ClosingBalance::Purse1CreditAudit.Label);
        AddDecimalField(&m_strPurse2BalanceAudit, ClosingBalance::Purse2BalanceAudit.Label);
        AddDecimalField(&m_strPurse3LiabilityAudit, ClosingBalance::Purse3LiabilityAudit.Label);
        AddDecimalField(&m_strPurse3CreditAudit, ClosingBalance::Purse3CreditAudit.Label);
        AddBOOLField(&m_bHaveAdjust, ClosingBalance::HaveAdjust.Label);
        AddDecimalField(&m_strPurse1LiabilityAdjust, ClosingBalance::Purse1LiabilityAdjust.Label);
        AddDecimalField(&m_strPurse1CreditAdjust, ClosingBalance::Purse1CreditAdjust.Label);
        AddDecimalField(&m_strPurse3LiabilityAdjust, ClosingBalance::Purse3LiabilityAdjust.Label);
        AddDecimalField(&m_strPurse3CreditAdjust, ClosingBalance::Purse3CreditAdjust.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataClosingBalance::AddParamsByDateSingle(const CString strDate)
{
    AddTextParam(ClosingBalance::KeyDate.Label, strDate);
}

//**********************************************************************

void CSQLRecordSetMetadataClosingBalance::AddParamsByDateRange(const CString strDateFrom, const CString strDateTo)
{
	AddParamsByDateSingle(strDateFrom); 
    AddTextParam(ClosingBalance::KeyDate.Label, strDateTo);
}

//**********************************************************************

CSQLRecordSetClosingBalance::CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetClosingBalance::CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_NormalByDateSingle params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByDateSingle(params.strDate);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetClosingBalance::CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_DetailByRange params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(FALSE)
{
	m_bRangeFilter = TRUE;
    m_MD.AddParamsByDateRange(params.strDateFrom, params.strDateTo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetClosingBalance::CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_CountRowsByRange params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
    m_bRangeFilter = TRUE;
    m_MD.AddParamsByDateRange(params.strDateFrom, params.strDateTo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetClosingBalance::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetClosingBalance::GetDefaultSQL()
{
    return SQLTableNames::ClosingBalances; 
}

//**********************************************************************

void CSQLRecordSetClosingBalance::LoadFromClosingBalanceRow(CSQLRowClosingBalance& RowClosing)
{
    m_MD.m_strKeyDate = RowClosing.GetKeyDate();
    m_MD.m_strDate = RowClosing.GetDate();
    m_MD.m_strTime = RowClosing.GetTime();
    m_MD.m_strArchiveDate = RowClosing.GetArchiveDate();
    m_MD.m_strArchiveTime = RowClosing.GetArchiveTime();
	SetDoubleString(m_MD.m_strPurse1LiabilityAudit, RowClosing.GetPurse1LiabilityAudit());
	SetDoubleString(m_MD.m_strPurse1CreditAudit, RowClosing.GetPurse1CreditAudit());
	SetDoubleString(m_MD.m_strPurse2BalanceAudit, RowClosing.GetPurse2BalanceAudit());
	SetDoubleString(m_MD.m_strPurse3LiabilityAudit, RowClosing.GetPurse3LiabilityAudit());
	SetDoubleString(m_MD.m_strPurse3CreditAudit, RowClosing.GetPurse3CreditAudit());
    m_MD.m_bHaveAdjust = RowClosing.GetHaveAdjustFlag();
	SetDoubleString(m_MD.m_strPurse1LiabilityAdjust, RowClosing.GetPurse1LiabilityAdjust());
	SetDoubleString(m_MD.m_strPurse1CreditAdjust, RowClosing.GetPurse1CreditAdjust());
	SetDoubleString(m_MD.m_strPurse3LiabilityAdjust, RowClosing.GetPurse3LiabilityAdjust());
	SetDoubleString(m_MD.m_strPurse3CreditAdjust, RowClosing.GetPurse3CreditAdjust());
}

//**********************************************************************

void CSQLRecordSetClosingBalance::SaveToClosingBalanceRow(CSQLRowClosingBalance& RowClosing)
{
    RowClosing.SetKeyDate(m_MD.m_strKeyDate);
    RowClosing.SetDate(m_MD.m_strDate);
    RowClosing.SetTime(m_MD.m_strTime);
    RowClosing.SetArchiveDate(m_MD.m_strArchiveDate);
    RowClosing.SetArchiveTime(m_MD.m_strArchiveTime);
	RowClosing.SetPurse1LiabilityAudit(atof(m_MD.m_strPurse1LiabilityAudit));
	RowClosing.SetPurse1CreditAudit(atof(m_MD.m_strPurse1CreditAudit));
	RowClosing.SetPurse2BalanceAudit(atof(m_MD.m_strPurse2BalanceAudit));
	RowClosing.SetPurse3LiabilityAudit(atof(m_MD.m_strPurse3LiabilityAudit));
	RowClosing.SetPurse3CreditAudit(atof(m_MD.m_strPurse3CreditAudit));
    RowClosing.SetHaveAdjustFlag(m_MD.m_bHaveAdjust);
	RowClosing.SetPurse1LiabilityAdjust(atof(m_MD.m_strPurse1LiabilityAdjust));
	RowClosing.SetPurse1CreditAdjust(atof(m_MD.m_strPurse1CreditAdjust));
	RowClosing.SetPurse3LiabilityAdjust(atof(m_MD.m_strPurse3LiabilityAdjust));
	RowClosing.SetPurse3CreditAdjust(atof(m_MD.m_strPurse3CreditAdjust));
}

//**********************************************************************

bool CSQLRecordSetClosingBalance::StepSelectAll(CSQLRowClosingBalance& RowClosing)
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

    SaveToClosingBalanceRow(RowClosing);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
