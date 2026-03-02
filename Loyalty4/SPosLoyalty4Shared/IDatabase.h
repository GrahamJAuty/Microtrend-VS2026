#pragma once
//**********************************************************************
#include <vector>
#include <utility>
//**********************************************************************
// Forward declarations
class CSQLTranBase;
//********************************************************************** 
// Simple result set interface

class IResultSet
{
public:
    virtual ~IResultSet() = default;
    
    virtual bool MoveNext() = 0;
    virtual bool IsEOF() = 0;
    virtual void Close() = 0;
    
    virtual CString GetString(const char* columnName) = 0;
    virtual __int64 GetInt64(const char* columnName) = 0;
    virtual int GetInt(const char* columnName) = 0;
    virtual double GetDouble(const char* columnName) = 0;
    virtual bool GetBool(const char* columnName) = 0;
};

//**********************************************************************
// Simple connection interface

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    
    // Execute SELECT query
    virtual IResultSet* ExecuteQuery(const char* sql, 
        const std::vector<std::pair<CString, CString>>& params) = 0;
    
    // Execute INSERT/UPDATE/DELETE
    virtual int ExecuteNonQuery(const char* sql,
        const std::vector<std::pair<CString, CString>>& params) = 0;
};

//**********************************************************************