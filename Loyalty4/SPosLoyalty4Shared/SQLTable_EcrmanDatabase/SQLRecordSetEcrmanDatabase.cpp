//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetEcrmanDatabase.h"
//**********************************************************************

CSQLRecordSetMetadataEcrmanDatabase::CSQLRecordSetMetadataEcrmanDatabase(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nDbNo, EcrmanDatabase::DbNo.Label, 1);
        AddTextField(&m_strDbName, EcrmanDatabase::DbName.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataEcrmanDatabase::AddParamsByDbNo(const int nDbNo)
{
    AddIntParam(EcrmanDatabase::DbNo.Label, nDbNo);
}

//**********************************************************************

CSQLRecordSetEcrmanDatabase::CSQLRecordSetEcrmanDatabase(CDatabase* pDatabase, RSParams_EcrmanDatabase_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetEcrmanDatabase::CSQLRecordSetEcrmanDatabase(CDatabase* pDatabase, RSParams_EcrmanDatabase_NormalByDbNo params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByDbNo(params.nDbNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetEcrmanDatabase::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetEcrmanDatabase::GetDefaultSQL()
{
    return SQLTableNames::EcrmanDatabases; 
}

//**********************************************************************

void CSQLRecordSetEcrmanDatabase::LoadFromEcrmanDatabaseRow(CSQLRowEcrmanDatabase& RowOS)
{
    m_MD.m_nDbNo = RowOS.GetDbNo();
    m_MD.m_strDbName = RowOS.GetDbName();
}

//**********************************************************************

void CSQLRecordSetEcrmanDatabase::SaveToEcrmanDatabaseRow(CSQLRowEcrmanDatabase& RowOS)
{
    RowOS.SetDbNo(m_MD.m_nDbNo);
    RowOS.SetDbName(m_MD.m_strDbName);
}

//**********************************************************************

bool CSQLRecordSetEcrmanDatabase::StepSelectAll(CSQLRowEcrmanDatabase& RowOS)
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

    SaveToEcrmanDatabaseRow(RowOS);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
