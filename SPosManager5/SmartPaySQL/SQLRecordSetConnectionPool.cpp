//**********************************************************************
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
//**********************************************************************
#include "SQLRecordSetConnectionPool.h"
//**********************************************************************

CSQLRecordSetConnectionPool::CSQLRecordSetConnectionPool(CDatabase* pDatabase) : CRecordset(SQLConnectionPool.GetPooledConnection(m_nConnection,pDatabase))
{
	m_nFields = 0;
	m_nParams = 0;
	m_pMetadata = NULL;
}

//**********************************************************************

CSQLRecordSetConnectionPool::~CSQLRecordSetConnectionPool()
{
    if (IsOpen() == TRUE)
    {
        Close();
    }

	SQLConnectionPool.FreeConnection(m_nConnection);
}

//**********************************************************************

int CSQLRecordSetConnectionPool::WrappedUpdate()
{
	int nResult = SQLCRUD_ERR_FAIL;

	try
	{
		if (IsFieldDirty(NULL) == FALSE)
		{
			nResult = SQLCRUD_ERR_NONE;
		}
		else if (Update() == TRUE)
		{
			nResult = SQLCRUD_ERR_NONE;
		}
	}
	catch (CDBException* pe)
	{
		nResult = SQLCRUD_ERR_EXCEPT_DB;
		pe->Delete();
	}
	catch (...)
	{
		nResult = SQLCRUD_ERR_EXCEPT_MISC;
	}

	return nResult;
}

//**********************************************************************

void CSQLRecordSetConnectionPool::RFX_BoolIfExists(CFieldExchange* pFX, const CString& strField, BOOL& bValue)
{
	try
	{
		RFX_Bool(pFX, strField, bValue);
	}
	catch (...)
	{
		bValue = FALSE;
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::RFX_IntIfExists(CFieldExchange* pFX, const CString& strField, int& nValue)
{
	try
	{
		RFX_Int(pFX, strField, nValue);
	}
	catch (...)
	{
		nValue = 0;
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::RFX_BigIntIfExists(CFieldExchange* pFX, const CString& strField, __int64& nValue)
{
	try
	{
		RFX_BigInt(pFX, strField, nValue);
	}
	catch (...)
	{
		nValue = 0;
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::RFX_DoubleIfExists(CFieldExchange* pFX, const CString& strField, double& dValue)
{
	try
	{
		RFX_Double(pFX, strField, dValue);
	}
	catch (...)
	{
		dValue = 0.0;
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::RFX_TextIfExists(CFieldExchange* pFX, const CString& strField, CString& strValue)
{
	try
	{
		RFX_Text(pFX, strField, strValue);
	}
	catch (...)
	{
		strValue.Empty();
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::DoFieldExchange(CFieldExchange* pFX)
{
	ExchangeFields(pFX);
	ExchangeParams(pFX);
}

//**********************************************************************

void CSQLRecordSetConnectionPool::ExchangeFields(CFieldExchange* pFX)
{
	m_pMetadata -> CheckDecimalFields();
	pFX->SetFieldType(CFieldExchange::outputColumn);

	// Exchange text fields
	const auto& textFields = m_pMetadata->GetTextFields();
	for (const auto& field : textFields)
	{
		if (TRUE == field.m_bCheckExists)
		{
			RFX_TextIfExists(pFX, field.m_strLabel, *(field.m_pField));
		}
		else
		{
			RFX_Text(pFX, field.m_strLabel, *(field.m_pField));
		}
 	}
	
	// Exchange int fields
	const auto& intFields = m_pMetadata->GetIntFields();
	for (const auto& field : intFields)
	{
		if (TRUE == field.m_bCheckExists)
		{
			RFX_IntIfExists(pFX, field.m_strLabel, *(field.m_pField));
		}
		else
		{
			RFX_Int(pFX, field.m_strLabel, *(field.m_pField));
		}
	}

	// Exchange int64 fields
	const auto& int64Fields = m_pMetadata->GetInt64Fields();
	for (const auto& field : int64Fields)
	{
		if (TRUE == field.m_bCheckExists)
		{
			RFX_BigIntIfExists(pFX, field.m_strLabel, *(field.m_pField));
		}
		else
		{
			RFX_BigInt(pFX, field.m_strLabel, *(field.m_pField));
		}
	}

	// Exchange decimal fields
	const auto& decimalFields = m_pMetadata->GetDecimalFields();
	for (const auto& field : decimalFields)
	{
		if (TRUE == field.m_bCheckExists)
		{
			RFX_TextIfExists(pFX, field.m_strLabel, *(field.m_pField));
		}
		else
		{
			RFX_Text(pFX, field.m_strLabel, *(field.m_pField));
		}
	}

	// Exchange BOOL fields
	const auto& BOOLFields = m_pMetadata->GetBOOLFields();
	for (const auto& field : BOOLFields)
	{
		if (TRUE == field.m_bCheckExists)
		{
			RFX_BoolIfExists(pFX, field.m_strLabel, *(field.m_pField));
		}
		else
		{
			RFX_Bool(pFX, field.m_strLabel, *(field.m_pField));
		}
	}
}

//**********************************************************************

void CSQLRecordSetConnectionPool::ExchangeParams(CFieldExchange* pFX)
{
	auto itText = m_pMetadata->GetTextParams().begin();
	auto itInt32 = m_pMetadata->GetInt32Params().begin();
	
	pFX->SetFieldType(CFieldExchange::param);

	const auto& paramTypes = m_pMetadata->GetIntParamTypes();

	for (auto& nType : paramTypes)
	{
		switch (nType)
		{
		case SQLSRVR_TEXT:
			RFX_Text(pFX, "", *(itText->m_pField));
			itText++;
			break;

		case SQLSRVR_INT32:
			RFX_Int(pFX, "", *(itInt32->m_pField));
			itInt32++;
			break;
		}
	}
}

//**********************************************************************

CSQLResultInfo CSQLRecordSetConnectionPool::CheckRowExists()
{
	CSQLResultInfo Result;
	Result.SetSQLError(SQLCRUD_ERR_NONE);
	Result.SetSQLResult(0);

	try
	{
		CString strSQL = "";
		strSQL += "SELECT TOP 1 1 FROM ";
		strSQL += GetDefaultSQL();

		if (Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			Result.SetSQLResult(IsEOF() ? 0 : 1);
			Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRecordSetConnectionPool::CountSQLRows()
{
	CSQLResultInfo Result;

	Result.SetSQLError(SQLCRUD_ERR_NONE);
	Result.SetSQLResult(0);

	try
	{
		SetByRangeFilterString();

		CString strSQL = "";
		strSQL += "SELECT COUNT(*) FROM ";
		strSQL += GetDefaultSQL();

		m_strSort = "";

		if (Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			int nFieldCount = GetODBCFieldCount();

			short nIndex = 0;
			CString strCount = "";
			{
				GetFieldValue(nIndex, strCount);
				Result.SetSQLResult(_atoi64(strCount));
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************