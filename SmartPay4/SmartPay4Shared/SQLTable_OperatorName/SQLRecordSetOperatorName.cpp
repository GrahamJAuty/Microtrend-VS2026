//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetOperatorName.h"
//**********************************************************************

CSQLRecordSetMetadataOperatorName::CSQLRecordSetMetadataOperatorName(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nOperatorNo, OperatorName::OperatorNo.Label, 1);
        AddTextField(&m_strOperatorName, OperatorName::OperatorName.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataOperatorName::AddParamsByOperatorNo(const int nOpNo)
{
    AddIntParam(OperatorName::OperatorNo.Label, nOpNo);
}

//**********************************************************************

CSQLRecordSetOperatorName::CSQLRecordSetOperatorName(CDatabase* pDatabase, RSParams_OperatorName_NormalNoParams params) :
    CSQLRecordSetConnectionPool( pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetOperatorName::CSQLRecordSetOperatorName(CDatabase* pDatabase, RSParams_OperatorName_NormalByOperatorNo params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByOperatorNo(params.nOperatorNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetOperatorName::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetOperatorName::GetDefaultSQL()
{
    return SQLTableNames::OperatorNames;
}

//**********************************************************************

void CSQLRecordSetOperatorName::LoadFromOperatorNameRow(CSQLRowOperatorName& RowON)
{
    m_MD.m_nOperatorNo = RowON.GetOperatorNo();
    m_MD.m_strOperatorName = RowON.GetOperatorName();
}

//**********************************************************************

void CSQLRecordSetOperatorName::SaveToOperatorNameRow(CSQLRowOperatorName& RowON)
{
    RowON.SetOperatorNo(m_MD.m_nOperatorNo);
    RowON.SetOperatorName(m_MD.m_strOperatorName);
}

//**********************************************************************

bool CSQLRecordSetOperatorName::StepSelectAll(CSQLRowOperatorName& RowON)
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

    SaveToOperatorNameRow(RowON);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
