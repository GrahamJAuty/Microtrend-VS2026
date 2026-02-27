//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAuditPeriod.h"
//**********************************************************************

CSQLRecordSetMetadataAuditPeriod::CSQLRecordSetMetadataAuditPeriod(bool bForExistenceCheck)
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nPeriodID, AuditPeriod::PeriodID.Label);
        AddTextField(&m_strPeriodName, AuditPeriod::PeriodName.Label);
        AddIntField(&m_nStartTime, AuditPeriod::StartTime.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataAuditPeriod::AddParamsByPeriodID(const int nPeriodID)
{
    AddIntParam(AuditPeriod::PeriodID.Label, nPeriodID);
}

//**********************************************************************

CSQLRecordSetAuditPeriod::CSQLRecordSetAuditPeriod(CDatabase* pDatabase, RSParams_AuditPeriod_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetAuditPeriod::CSQLRecordSetAuditPeriod(CDatabase* pDatabase, RSParams_AuditPeriod_NormalByPeriodId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByPeriodID(params.nPeriodID);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetAuditPeriod::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetAuditPeriod::GetDefaultSQL()
{
    return SQLTableNames::AuditPeriods; 
}

//**********************************************************************

void CSQLRecordSetAuditPeriod::LoadFromAuditPeriodRow(CSQLRowAuditPeriod& RowAudit)
{
    m_MD.m_nPeriodID = RowAudit.GetPeriodID();
    m_MD.m_strPeriodName = RowAudit.GetPeriodName();
    m_MD.m_nStartTime = RowAudit.GetStartTime();
}

//**********************************************************************

void CSQLRecordSetAuditPeriod::SaveToAuditPeriodRow(CSQLRowAuditPeriod& RowAudit)
{
    RowAudit.SetPeriodID(m_MD.m_nPeriodID );
    RowAudit.SetPeriodName(m_MD.m_strPeriodName);
    RowAudit.SetStartTime(m_MD.m_nStartTime);
}

//**********************************************************************

bool CSQLRecordSetAuditPeriod::StepSelectAll(CSQLRowAuditPeriod& RowPlu)
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

    SaveToAuditPeriodRow(RowPlu);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
