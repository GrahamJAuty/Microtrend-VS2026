//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetScheme.h"
//**********************************************************************

CSQLRecordSetMetadataScheme::CSQLRecordSetMetadataScheme(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nSchemeNo, Scheme::SchemeNo.Label, 1);
        AddTextField(&m_strSchemeName, Scheme::SchemeName.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataScheme::AddParamsBySchemeNo(const int nSchemeNo)
{
    AddIntParam(Scheme::SchemeNo.Label, nSchemeNo);
}

//**********************************************************************

CSQLRecordSetScheme::CSQLRecordSetScheme(CDatabase* pDatabase, RSParams_Scheme_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{   	
    CommonInit();
}

//**********************************************************************

CSQLRecordSetScheme::CSQLRecordSetScheme(CDatabase* pDatabase, RSParams_Scheme_NormalBySchemeNo params, bool bForExistenceCheck ) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsBySchemeNo(params.nSchemeNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetScheme::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetScheme::GetDefaultSQL()
{
    return SQLTableNames::Schemes; 
}

//**********************************************************************

void CSQLRecordSetScheme::LoadFromSchemeRow(CSQLRowScheme& RowSC)
{
    m_MD.m_nSchemeNo = RowSC.GetSchemeNo();
    m_MD.m_strSchemeName = RowSC.GetSchemeName();
}

//**********************************************************************

void CSQLRecordSetScheme::SaveToSchemeRow(CSQLRowScheme& RowSC)
{
    RowSC.SetSchemeNo(m_MD.m_nSchemeNo);
    RowSC.SetSchemeName(m_MD.m_strSchemeName);
}

//**********************************************************************

bool CSQLRecordSetScheme::StepSelectAll(CSQLRowScheme& RowOS)
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

    SaveToSchemeRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
