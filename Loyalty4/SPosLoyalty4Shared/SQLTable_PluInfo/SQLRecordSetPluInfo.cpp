//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPluInfo.h"
//**********************************************************************

CSQLRecordSetMetadataPluInfo::CSQLRecordSetMetadataPluInfo(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nDbNo, PluInfo::DbNo.Label, 1);
        AddInt64Field(&m_nPluNo, PluInfo::PluNo.Label, 1);
        AddTextField(&m_strDescription, PluInfo::Description.Label);
        AddIntField(&m_nDeptNo, PluInfo::DeptNo.Label, 1);
        AddTextField(&m_strTaxCode, PluInfo::TaxCode.Label, "A");
        AddIntField(&m_nPoints, PluInfo::Points.Label);
        AddIntField(&m_nModType, PluInfo::ModType.Label);
        AddIntField(&m_nStampOfferID, PluInfo::StampOfferID.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPluInfo::AddParamsByDbNo(const int nDbNo)
{
    AddIntParam(PluInfo::DbNo.Label, nDbNo);
}

//**********************************************************************

void CSQLRecordSetMetadataPluInfo::AddParamsByDbNoPluNo(const int nDbNo, const CString strPluNo)
{
	AddParamsByDbNo(nDbNo);
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

CSQLRecordSetPluInfo::CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByDbNo params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	m_MD.AddParamsByDbNo(params.nDbNo);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPluInfo::CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByDbNoPluNo params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByDbNoPluNo(params.nDbNo, params.strPluNo);
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
	m_MD.m_nDbNo = RowPlu.GetDbNo();
    m_MD.m_nPluNo = RowPlu.GetPluNo();
    m_MD.m_strDescription = RowPlu.GetDescription();
    m_MD.m_nDeptNo = RowPlu.GetDeptNo();
    m_MD.m_strTaxCode = RowPlu.GetTaxCode();
    m_MD.m_nPoints = RowPlu.GetPoints();
    m_MD.m_nModType = RowPlu.GetModType();
    m_MD.m_nStampOfferID = RowPlu.GetStampOfferID();
}

//**********************************************************************

void CSQLRecordSetPluInfo::SaveToPluInfoRow(CSQLRowPluInfo& RowPlu)
{
	RowPlu.SetDbNo(m_MD.m_nDbNo);
    RowPlu.SetPluNo(m_MD.m_nPluNo);
    RowPlu.SetDescription(m_MD.m_strDescription);
    RowPlu.SetDeptNo(m_MD.m_nDeptNo);
    RowPlu.SetTaxCode(m_MD.m_strTaxCode);
    RowPlu.SetPoints(m_MD.m_nPoints);
    RowPlu.SetModType(m_MD.m_nModType);
    RowPlu.SetStampOfferID(m_MD.m_nStampOfferID);
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
