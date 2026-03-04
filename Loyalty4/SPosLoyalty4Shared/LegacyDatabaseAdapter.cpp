//**********************************************************************
#include "SQLHelpers.h"
//**********************************************************************
#include "LegacyDatabaseAdapter.h"
//**********************************************************************

IResultSet* CLegacyDatabaseAdapter::ExecuteQuery(const char* sql, 
    const std::vector<std::pair<CString, CString>>& params)
{
    CRecordset* pRecordset = new CRecordset(m_pDatabase);
    
    // For now, simple direct execution (no parameters)
    // Later: add parameter binding
    if (pRecordset->Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
    {
        return new CLegacyResultSetAdapter(pRecordset);
    }
    
    delete pRecordset;
    return nullptr;
}

//**********************************************************************

int CLegacyDatabaseAdapter::ExecuteNonQuery(const char* sql,
    const std::vector<std::pair<CString, CString>>& params)
{
    // Use existing CSQLHelpers::ExecuteSQL
    return CSQLHelpers::ExecuteSQL(m_pDatabase, sql);
}

//**********************************************************************

// CLegacyResultSetAdapter implementations
bool CLegacyResultSetAdapter::MoveNext()
{
    m_pRecordset->MoveNext();
    return !m_pRecordset->IsEOF();
}

//**********************************************************************

bool CLegacyResultSetAdapter::IsEOF()
{
    return m_pRecordset->IsEOF();
}

//**********************************************************************

CString CLegacyResultSetAdapter::GetString(const char* columnName)
{
    CString value = "";
    m_pRecordset->GetFieldValue(columnName, value);
    return value;
}

//**********************************************************************

__int64 CLegacyResultSetAdapter::GetInt64(const char* columnName)
{
    CString value = "";
    m_pRecordset->GetFieldValue(columnName, value);
    return _ttoi64(value);
}

//**********************************************************************

int CLegacyResultSetAdapter::GetInt(const char* columnName)
{
    CString value = "";
    m_pRecordset->GetFieldValue(columnName, value);
    return _ttoi(value);
}

//**********************************************************************

double CLegacyResultSetAdapter::GetDouble(const char* columnName)
{
    CString value = "";
    m_pRecordset->GetFieldValue(columnName, value);
    return _ttof(value);
}

//**********************************************************************

bool CLegacyResultSetAdapter::GetBool(const char* columnName)
{
    CString value = "";
    m_pRecordset->GetFieldValue(columnName, value);
    return value.CompareNoCase("true") == 0 || value == "1";
}

//**********************************************************************

void CLegacyResultSetAdapter::Close()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        m_pRecordset->Close();
        delete m_pRecordset;
        m_pRecordset = nullptr;
    }
}

//**********************************************************************
