//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetDbVersion.h"
//**********************************************************************

CSQLRecordSetMetadataDbVersion::CSQLRecordSetMetadataDbVersion() : CSQLRecordSetMetadata()
{
    AddIntField(&m_nLineID, DbVersion::LineID.Label);   
    AddIntField(&m_nVersion, DbVersion::Version.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataDbVersion::AddParamsByLineId(const int nLineId)
{    
    AddIntParam(DbVersion::LineID.Label, nLineId);
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalByLineId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	m_MD.AddParamsByLineId(params.nLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetDbVersion::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetDbVersion::GetDefaultSQL()
{ 
    return SQLTableNames::DbVersion; 
}

//**********************************************************************

void CSQLRecordSetDbVersion::LoadFromDbVersionRow(CSQLRowDbVersion& RowDbVersion)
{
    m_MD.m_nLineID = RowDbVersion.GetLineID();
    m_MD.m_nVersion = RowDbVersion.GetVersion();
}

//**********************************************************************

void CSQLRecordSetDbVersion::SaveToDbVersionRow(CSQLRowDbVersion& RowDbVersion)
{
    RowDbVersion.SetLineID(m_MD.m_nLineID);
    RowDbVersion.SetVersion(m_MD.m_nVersion);
}

//**********************************************************************

bool CSQLRecordSetDbVersion::StepSelectAll(CSQLRowDbVersion& RowDbVersion)
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

    SaveToDbVersionRow(RowDbVersion);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
