//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPluInfo.h"
//**********************************************************************

CSQLRecordSetMetadataPluInfo::CSQLRecordSetMetadataPluInfo(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nPluNo, PluInfo::PluNo.Label);
        AddTextField(&m_strDescription, PluInfo::Description.Label);
        AddIntField(&m_nDeptNo, PluInfo::DeptNo.Label);
        AddTextField(&m_strTaxCode, PluInfo::TaxCode.Label);
        AddIntField(&m_nModType, PluInfo::ModType.Label);
        AddIntField(&m_nPoints, PluInfo::Points.Label);
        AddIntField(&m_nPCtrlRuleID, PluInfo::PCtrlRuleID.Label);
        AddIntField(&m_nStampWeight, PluInfo::StampWeight.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPluInfo::AddParamsByPluNo(const CString strPluNo)
{
    AddTextParam(PluInfo::PluNo.Label, strPluNo);
}

//**********************************************************************

CSQLRecordSetPluInfo::CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPluInfo::CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByPluNo params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByPluNo(params.strPluNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPluInfo::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPluInfo::GetDefaultSQL()
{
    return SQLTableNames::PluInfo; 
}

//**********************************************************************

void CSQLRecordSetPluInfo::LoadFromPluInfoRow(CSQLRowPluInfo& RowPlu)
{
    m_MD.m_nPluNo = RowPlu.GetPluNo();
    m_MD.m_strDescription = RowPlu.GetDescription();
    m_MD.m_nDeptNo = RowPlu.GetDeptNo();
    m_MD.m_strTaxCode = RowPlu.GetTaxCode();
    m_MD.m_nModType = RowPlu.GetModType();
    m_MD.m_nPoints = RowPlu.GetPoints();
    m_MD.m_nPCtrlRuleID = RowPlu.GetPCtrlRuleID();
    m_MD.m_nStampWeight = RowPlu.GetStampWeight();
}

//**********************************************************************

void CSQLRecordSetPluInfo::SaveToPluInfoRow(CSQLRowPluInfo& RowPlu)
{
    RowPlu.SetPluNo(m_MD.m_nPluNo);
    RowPlu.SetDescription(m_MD.m_strDescription);
    RowPlu.SetDeptNo(m_MD.m_nDeptNo);
    RowPlu.SetTaxCode(m_MD.m_strTaxCode);
	RowPlu.SetModType(m_MD.m_nModType);
    RowPlu.SetPoints(m_MD.m_nPoints);
    RowPlu.SetPCtrlRuleID(m_MD.m_nPCtrlRuleID);
    RowPlu.SetStampWeight(m_MD.m_nStampWeight);
}

//**********************************************************************

bool CSQLRecordSetPluInfo::StepSelectAll(CSQLRowPluInfo& RowPlu)
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

    SaveToPluInfoRow(RowPlu);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
