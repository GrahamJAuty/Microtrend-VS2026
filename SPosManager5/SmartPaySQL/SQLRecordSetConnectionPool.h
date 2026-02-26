#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include <vector>
//**********************************************************************
#include "SQLBuilderBind.h"	
#include "SQLDefines.h"
#include "SQLRecordSetMetadata.h"
#include "SQLResultInfo.h"
//**********************************************************************

class CSQLRecordSetConnectionPool : public CRecordset
{
public:
	CSQLRecordSetConnectionPool(CDatabase* pDatabase);
	~CSQLRecordSetConnectionPool();

public:
	int WrappedUpdate();
	int GetConnection() { return m_nConnection; }
	void SetMetadata(CSQLRecordSetMetadata* pMetadata) { m_pMetadata = pMetadata; }
	
public:
	CSQLResultInfo CheckRowExists();
	CSQLResultInfo CountSQLRows();

public:
	CString GetBasicFilterString() 
	{ 
		if (m_pMetadata != NULL)
		{
			return m_pMetadata->GetBasicFilterString();
		}
		else
		{
			return "";
		}
	}

	CString GetByRangeFilterString()
	{
		if (m_pMetadata != NULL)
		{
			return m_pMetadata->GetByRangeFilterString();
		}
		else
		{
			return "";
		}
	}

	void SetBasicFilterString() { m_strFilter = GetBasicFilterString(); }
	void SetByRangeFilterString() { m_strFilter = GetByRangeFilterString(); }
	void SetInsertFilterString() { m_strFilter = "1 = 0"; }

public:
	void RFX_BoolIfExists(CFieldExchange* pFX, const CString& strField, BOOL& bValue);
	void RFX_IntIfExists(CFieldExchange* pFX, const CString& strField, int& nValue);
	void RFX_BigIntIfExists(CFieldExchange* pFX, const CString& strField, __int64& nValue);
	void RFX_DoubleIfExists(CFieldExchange* pFX, const CString& strField, double& dValue);
	void RFX_TextIfExists(CFieldExchange* pFX, const CString& strField, CString& strValue);

protected:
	virtual void DoFieldExchange(CFieldExchange* pFX);
	void ExchangeFields(CFieldExchange* pFX);
	void ExchangeParams(CFieldExchange* pFX);

protected:
	void SetDoubleString(CString& strField, double dValue) { strField.Format("%f", dValue); }

private:
	CSQLRecordSetMetadata* m_pMetadata;
	int m_nConnection;
};

//**********************************************************************

