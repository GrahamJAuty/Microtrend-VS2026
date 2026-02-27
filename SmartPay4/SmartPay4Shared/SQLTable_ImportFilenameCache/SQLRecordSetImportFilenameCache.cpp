//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetImportFilenameCache.h"
//**********************************************************************

CSQLRecordSetMetadataImportFilenameCache::CSQLRecordSetMetadataImportFilenameCache(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, ImportFilenameCache::LineID.Label);
        AddIntField(&m_nType, ImportFilenameCache::Type.Label);
        AddTextField(&m_strFilename, ImportFilenameCache::Filename.Label);
        AddTextField(&m_strDateTime, ImportFilenameCache::DateTime.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataImportFilenameCache::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(ImportFilenameCache::LineID.Label, strLineID);
}

//**********************************************************************

void CSQLRecordSetMetadataImportFilenameCache::AddParamsByTypeAndFilename(const CString strType, const CString strFilename)
{
    AddTextParam(ImportFilenameCache::Type.Label, strType);
    AddTextParam(ImportFilenameCache::Filename.Label, strFilename);
}

//**********************************************************************

CSQLRecordSetImportFilenameCache::CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetImportFilenameCache::CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.strLineId);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetImportFilenameCache::CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalByTypeAndFilename params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByTypeAndFilename(params.strType, params.strFilename);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetImportFilenameCache::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetImportFilenameCache::GetDefaultSQL()
{
    return SQLTableNames::ImportFilenameCache;
}

//**********************************************************************

void CSQLRecordSetImportFilenameCache::LoadFromImportFilenameCacheRow(CSQLRowImportFilenameCache& RowBC)
{
    m_MD.m_nLineID = RowBC.GetLineID();
    m_MD.m_nType = RowBC.GetType();
    m_MD.m_strFilename = RowBC.GetFilename();
    m_MD.m_strDateTime = RowBC.GetDateTime();
}

//**********************************************************************

void CSQLRecordSetImportFilenameCache::SaveToImportFilenameCacheRow(CSQLRowImportFilenameCache& RowBC)
{
    RowBC.SetLineID(m_MD.m_nLineID);
    RowBC.SetType(m_MD.m_nType);
    RowBC.SetFilename(m_MD.m_strFilename);
    RowBC.SetDateTime(m_MD.m_strDateTime);
}

//**********************************************************************

bool CSQLRecordSetImportFilenameCache::StepSelectAll(CSQLRowImportFilenameCache& RowBC)
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

    SaveToImportFilenameCacheRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
