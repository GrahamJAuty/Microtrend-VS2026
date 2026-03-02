#pragma once
//**********************************************************************
#include "IDatabase.h"
#include <afxdb.h>
//**********************************************************************

// Adapter: Makes CDatabase look like IDatabase
class CLegacyDatabaseAdapter : public IDatabase
{
public:
    CLegacyDatabaseAdapter(CDatabase* pDatabase) : m_pDatabase(pDatabase) {}
    
    virtual IResultSet* ExecuteQuery(const char* sql, 
        const std::vector<std::pair<CString, CString>>& params) override;
    
    virtual int ExecuteNonQuery(const char* sql,
        const std::vector<std::pair<CString, CString>>& params) override;

private:
    CDatabase* m_pDatabase;
};

//**********************************************************************
// Adapter: Makes CRecordset look like IResultSet

class CLegacyResultSetAdapter : public IResultSet
{
public:
    CLegacyResultSetAdapter(CRecordset* pRecordset) : m_pRecordset(pRecordset) {}
    ~CLegacyResultSetAdapter() { Close(); }
    
    virtual bool MoveNext() override;
    virtual bool IsEOF() override;
    virtual void Close() override;
    
    virtual CString GetString(const char* columnName) override;
    virtual __int64 GetInt64(const char* columnName) override;
    // ... etc

private:
    CRecordset* m_pRecordset;
};

//**********************************************************************