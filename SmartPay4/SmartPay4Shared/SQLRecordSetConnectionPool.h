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
	__int64 WrappedUpdate(CString strVersion = "");
	int GetConnection() { return m_nConnection; }
	void SetMetadata(CSQLRecordSetMetadata* pMetadata) { m_pMetadata = pMetadata; }
	
public:
	CSQLResultInfo CheckRowExists();
	CSQLResultInfo RSCountRowsByRange();

public:
	void SetInitialFilterString() 
	{
		if ( m_pMetadata == NULL)
		{
			m_strFilter = "";
		}
		else if (TRUE == m_bRangeFilter)
		{
			m_strFilter = m_pMetadata->GetByRangeFilterString();
		}
		else
		{
			m_strFilter = m_pMetadata->GetBasicFilterString();
		}
	}

	void SetInsertFilterString() { m_strFilter = "1 = 0"; }

public:
	void RFX_BoolIfExists(CFieldExchange* pFX, const CString& strField, BOOL& bValue);
	void RFX_IntIfExists(CFieldExchange* pFX, const CString& strField, int& nValue);
	void RFX_BigIntIfExists(CFieldExchange* pFX, const CString& strField, __int64& nValue);
	void RFX_DoubleIfExists(CFieldExchange* pFX, const CString& strField, double& dValue);
	void RFX_TextIfExists(CFieldExchange* pFX, const CString& strField, CString& strValue);
	void RFX_FixLenTextIfExists(CFieldExchange* pFX, const CString& strField, CString& strValue, int nLen);

protected:
	virtual void DoFieldExchange(CFieldExchange* pFX);
	void ExchangeFields(CFieldExchange* pFX);
	void ExchangeParams(CFieldExchange* pFX);

protected:
	void SetDoubleString(CString& strField, double dValue) { strField.Format("%f", dValue); }

protected:
	bool m_bRangeFilter = FALSE;

private:
	CSQLRecordSetMetadata* m_pMetadata = nullptr;

private:
	//THIS WILL BE SET BY A FUNCTION THAT IS PASSED TO THE BASE CLASS CONSTRUCTOR
	//IT NUST **NOT** BE INITIALIZED IN THE HEADER OR CONSTRUCTOR OF THIS DERIVED CLASS
	int m_nConnection; 
};

//**********************************************************************

