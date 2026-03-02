#include "LegacyDatabaseAdapter.h"

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

int CLegacyDatabaseAdapter::ExecuteNonQuery(const char* sql,
    const std::vector<std::pair<CString, CString>>& params)
{
    // Use existing CSQLHelpers::ExecuteSQL
    return CSQLHelpers::ExecuteSQL(m_pDatabase, sql);
}

// CLegacyResultSetAdapter implementations
bool CLegacyResultSetAdapter::MoveNext()
{
    m_pRecordset->MoveNext();
    return !m_pRecordset->IsEOF();
}

bool CLegacyResultSetAdapter::IsEOF()
{
    return m_pRecordset->IsEOF();
}

CString CLegacyResultSetAdapter::GetString(const char* columnName)
{
    CString value;
    m_pRecordset->GetFieldValue(columnName, value);
    return value;
}

void CLegacyResultSetAdapter::Close()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        m_pRecordset->Close();
        delete m_pRecordset;
        m_pRecordset = nullptr;
    }
}

// ... implement other GetXXX methods similarly