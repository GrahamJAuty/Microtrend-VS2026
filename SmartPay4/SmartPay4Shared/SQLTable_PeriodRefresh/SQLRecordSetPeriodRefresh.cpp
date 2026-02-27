//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPeriodRefresh.h"
//**********************************************************************

CSQLRecordSetMetadataPeriodRefresh::CSQLRecordSetMetadataPeriodRefresh(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nUserID, PeriodRefresh::UserID.Label);
        AddTextField(&m_strPeriod1Date, PeriodRefresh::Period1Date.Label);
        AddDecimalField(&m_strPeriod1Balance, PeriodRefresh::Period1Balance.Label);
        AddTextField(&m_strPeriod2Date, PeriodRefresh::Period2Date.Label);
        AddDecimalField(&m_strPeriod2Balance, PeriodRefresh::Period2Balance.Label);
        AddTextField(&m_strPeriod3Date, PeriodRefresh::Period3Date.Label);
        AddDecimalField(&m_strPeriod3Balance, PeriodRefresh::Period3Balance.Label);
        AddTextField(&m_strPeriod4Date, PeriodRefresh::Period4Date.Label);
        AddDecimalField(&m_strPeriod4Balance, PeriodRefresh::Period4Balance.Label);
        AddTextField(&m_strPeriod5Date, PeriodRefresh::Period5Date.Label);
        AddDecimalField(&m_strPeriod5Balance, PeriodRefresh::Period5Balance.Label);
        AddTextField(&m_strPeriod6Date, PeriodRefresh::Period6Date.Label);
        AddDecimalField(&m_strPeriod6Balance, PeriodRefresh::Period6Balance.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPeriodRefresh::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(PeriodRefresh::UserID.Label, strUserID);
}

//**********************************************************************

CSQLRecordSetPeriodRefresh::CSQLRecordSetPeriodRefresh(CDatabase* pDatabase, RSParams_PeriodRefresh_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPeriodRefresh::CSQLRecordSetPeriodRefresh(CDatabase* pDatabase, RSParams_PeriodRefresh_NormalByUserID params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByUserID(params.strUserID);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPeriodRefresh::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPeriodRefresh::GetDefaultSQL()
{
    return SQLTableNames::PeriodRefresh;
}

//**********************************************************************

void CSQLRecordSetPeriodRefresh::LoadFromPeriodRefreshRow(CSQLRowPeriodRefresh& RowPeriod)
{
    m_MD.m_nUserID = RowPeriod.GetUserID();
    m_MD.m_strPeriod1Date = RowPeriod.GetPeriod1Date();
	SetDoubleString(m_MD.m_strPeriod1Balance, RowPeriod.GetPeriod1Balance());
    m_MD.m_strPeriod2Date = RowPeriod.GetPeriod2Date();
	SetDoubleString(m_MD.m_strPeriod2Balance, RowPeriod.GetPeriod2Balance());
    m_MD.m_strPeriod3Date = RowPeriod.GetPeriod3Date();
	SetDoubleString(m_MD.m_strPeriod3Balance, RowPeriod.GetPeriod3Balance());
    m_MD.m_strPeriod4Date = RowPeriod.GetPeriod4Date();
	SetDoubleString(m_MD.m_strPeriod4Balance, RowPeriod.GetPeriod4Balance());
    m_MD.m_strPeriod5Date = RowPeriod.GetPeriod5Date();
	SetDoubleString(m_MD.m_strPeriod5Balance, RowPeriod.GetPeriod5Balance());
    m_MD.m_strPeriod6Date = RowPeriod.GetPeriod6Date();
	SetDoubleString(m_MD.m_strPeriod6Balance, RowPeriod.GetPeriod6Balance());
}

//**********************************************************************

void CSQLRecordSetPeriodRefresh::SaveToPeriodRefreshRow(CSQLRowPeriodRefresh& RowPeriod)
{
    RowPeriod.SetUserID(m_MD.m_nUserID);
    RowPeriod.SetPeriod1Date(m_MD.m_strPeriod1Date,TRUE);
    RowPeriod.SetPeriod1Balance(atof(m_MD.m_strPeriod1Balance));
    RowPeriod.SetPeriod2Date(m_MD.m_strPeriod2Date, TRUE);
	RowPeriod.SetPeriod2Balance(atof(m_MD.m_strPeriod2Balance));
    RowPeriod.SetPeriod3Date(m_MD.m_strPeriod3Date, TRUE);
    RowPeriod.SetPeriod3Balance(atof(m_MD.m_strPeriod3Balance));
    RowPeriod.SetPeriod4Date(m_MD.m_strPeriod4Date, TRUE);
    RowPeriod.SetPeriod4Balance(atof(m_MD.m_strPeriod4Balance));
    RowPeriod.SetPeriod5Date(m_MD.m_strPeriod5Date, TRUE);
	RowPeriod.SetPeriod5Balance(atof(m_MD.m_strPeriod5Balance));
    RowPeriod.SetPeriod6Date(m_MD.m_strPeriod6Date, TRUE);
    RowPeriod.SetPeriod6Balance(atof(m_MD.m_strPeriod6Balance));
}

//**********************************************************************

bool CSQLRecordSetPeriodRefresh::StepSelectAll(CSQLRowPeriodRefresh& RowPeriod)
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

    SaveToPeriodRefreshRow(RowPeriod);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
