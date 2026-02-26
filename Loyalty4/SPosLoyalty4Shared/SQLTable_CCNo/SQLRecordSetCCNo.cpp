//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetCCNo.h"
//**********************************************************************

CSQLRecordSetMetadataCCNo::CSQLRecordSetMetadataCCNo(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strType, CCNo::Type.Label);
        AddIntField(&m_nNumber, CCNo::Number.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataCCNo::AddParamsByType(const CString strType)
{
    AddTextParam(CCNo::Type.Label, strType);
}

//**********************************************************************

CSQLRecordSetCCNo::CSQLRecordSetCCNo(CDatabase* pDatabase, RSParams_CCNo_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetCCNo::CSQLRecordSetCCNo(CDatabase* pDatabase, RSParams_CCNo_NormalByType params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByType(params.strType);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetCCNo::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetCCNo::GetDefaultSQL()
{ 
    return SQLTableNames::CCNo;
}

//**********************************************************************

void CSQLRecordSetCCNo::LoadFromCCNoRow(CSQLRowCCNo& RowCCNo)
{
	m_MD.m_strType = RowCCNo.GetTypeString();
    m_MD.m_nNumber = RowCCNo.GetNumber();
}

//**********************************************************************

void CSQLRecordSetCCNo::SaveToCCNoRow(CSQLRowCCNo& RowCCNo)
{
    RowCCNo.SetTypeString(m_MD.m_strType);
    RowCCNo.SetNumber(m_MD.m_nNumber);
}

//**********************************************************************

bool CSQLRecordSetCCNo::StepSelectAll(CSQLRowCCNo& RowCCNo)
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

    SaveToCCNoRow(RowCCNo);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
