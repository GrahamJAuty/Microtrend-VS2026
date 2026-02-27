//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetDbVersion.h"
//**********************************************************************

CSQLRecordSetMetadataDbVersion::CSQLRecordSetMetadataDbVersion(RSDataType_DbVersion type) : CSQLRecordSetMetadata()
{
    switch (type)
    {
    case RSDataType_DbVersion::Normal:
    case RSDataType_DbVersion::ExtVersion:
        break;

    default:
        type = RSDataType_DbVersion::Normal;
        break;
    }

    m_type = type;

    AddIntField(&m_nLineID, DbVersion::LineID.Label);
    AddIntField(&m_nVersion, DbVersion::Version.Label);

    switch (m_type)
    {
    case RSDataType_DbVersion::ExtVersion:
        AddIntField(&m_nExternalVersion1, DbVersion::ExternalVersion1.Label);
        break;
    }
}

//**********************************************************************

void CSQLRecordSetMetadataDbVersion::AddParamsByLineID(const int nLineID)
{
    AddIntParam(DbVersion::LineID.Label, nLineID);
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD(RSDataType_DbVersion::Normal)
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_NormalByLineId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_DbVersion::Normal)
{
	m_MD.AddParamsByLineID(params.nLineId);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_ExtVersionNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_DbVersion::ExtVersion)
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetDbVersion::CSQLRecordSetDbVersion(CDatabase* pDatabase, RSParams_DbVersion_ExtVersionByLineId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(RSDataType_DbVersion::ExtVersion)
{
    m_MD.AddParamsByLineID(params.nLineId);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetDbVersion::CommonInit()
{
    m_type = m_MD.m_type;
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

    switch (m_type)
    {
    case RSDataType_DbVersion::ExtVersion:
        m_MD.m_nExternalVersion1 = RowDbVersion.GetExternalVersion1();
        break;
    }
}

//**********************************************************************

void CSQLRecordSetDbVersion::SaveToDbVersionRow(CSQLRowDbVersion& RowDbVersion)
{
    RowDbVersion.SetLineID(m_MD.m_nLineID);
    RowDbVersion.SetVersion(m_MD.m_nVersion);

    switch (m_type)
    {
    case RSDataType_DbVersion::ExtVersion:
        RowDbVersion.SetExternalVersion1(m_MD.m_nExternalVersion1);
        break;
    }
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
