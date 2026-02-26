//**********************************************************************
#include "SQLDefines.h"
//**********************************************************************
#include "SQLRecordSetMetadata.h"
//**********************************************************************

CSQLRecordSetMetadata::CSQLRecordSetMetadata() 
{
	m_nFields = 0;
	m_nParams = 0;
}

//**********************************************************************

CString CSQLRecordSetMetadata::GetBasicFilterString()
{
	m_strBasicFilterString = "";

	auto itText = m_vTextParams.begin();
	auto itInt32 = m_vInt32Params.begin();

	bool bFirst = TRUE;
	for (int& nType : m_vIntParamTypes)
	{
		if (FALSE == bFirst)
		{
			m_strBasicFilterString += " AND ";
		}
		else
		{
			bFirst = FALSE;
		}

		switch (nType)
		{
		case SQLSRVR_TEXT:
			m_strBasicFilterString += itText->m_strLabel + " = ?";
			itText++;
			break;

		case SQLSRVR_INT32:
			m_strBasicFilterString += itInt32->m_strLabel + " = ?";
			itInt32++;
			break;
		}
	}

	return m_strBasicFilterString;
}

//**********************************************************************

CString CSQLRecordSetMetadata::GetByRangeFilterString()
{
	m_strByRangeFilterString = "";

	auto itText = m_vTextParams.begin();
	auto itInt32 = m_vInt32Params.begin();

	int nMatchType = 0;

	{
		size_t n = m_vTextParams.size();
		if (2 == n)
		{
			nMatchType = 1;
		}
		else if (n != 0)
		{
			nMatchType = -1;
		}
	}

	if ( nMatchType != -1 )
	{
		size_t n = m_vInt32Params.size();
		if ((2 == n) && (0 == nMatchType))
		{
			nMatchType = 2;
		}
		else if (n != 0)
		{
			nMatchType = -1;
		}
	}

	if (nMatchType > 0)
	{
		CString strLabel = "";

		switch (nMatchType)
		{
		case 1:
			strLabel = itText->m_strLabel;
			break;

		case 2:
			strLabel = itInt32->m_strLabel;
			break;
		}

		//Intentionally using the same label for both lower and upper bound
		m_strByRangeFilterString += strLabel + " >= ? AND ";
		m_strByRangeFilterString += strLabel + " <= ?";
	}

	return m_strByRangeFilterString;
}

//**********************************************************************

void CSQLRecordSetMetadata::BindGeneralParams(CSQLBuilderBind* pBind)
{
	if (pBind != NULL)
	{
		CString strDummy = "";	

		for (int n = 0; (n < pBind->GetFieldCount()) && (n < MAX_GENERAL_SQLPARAMS); n++)
		{
			CSQLBuilderBindField Field;
			pBind->GetField(n, Field);

			if (Field.m_nFieldType == SQLSRVR_INT32)
			{
				AddIntParam(strDummy, Field.m_nFieldValue32);
			}
			else
			{
				AddTextParam(strDummy, Field.GetFieldAsString());
			}
		}
	}
}

//**********************************************************************

void CSQLRecordSetMetadata::CheckDecimalFields()
{
	for (auto& field : m_vDecimalFields)
	{
		if (field.m_pField)
		{
			CheckValidDecimalString(*(field.m_pField));
		}
	}
}

//**********************************************************************

void CSQLRecordSetMetadata::CheckValidDecimalString(CString& strField, int nDPs)
{
	bool bCreateDefaultString = FALSE;

	if (strField.IsEmpty())
	{
		bCreateDefaultString = TRUE;
	}
	else
	{
		TCHAR* endPtr = nullptr;
		double d = _tcstod(strField, &endPtr);
		bCreateDefaultString = ((endPtr == strField) || (*endPtr != _T('\0')));
		bCreateDefaultString |= (0.0 == d);
	}

	if (TRUE == bCreateDefaultString)
	{
		strField.Format("%.*f", nDPs, 0.0);
	}
}

//**********************************************************************

void CSQLRecordSetMetadata::AddTextField(CString* pField, const CString strLabel, const CString strDef, const bool bCheckExists)
{
	m_vTextFields.emplace_back(pField, strLabel, bCheckExists);

	if (pField)
	{
		*pField = strDef;
	}

	m_nFields++;
}

//**********************************************************************

void CSQLRecordSetMetadata::AddIntField(int* pField, const CString strLabel, const int nDef, const bool bCheckExists)
{
	m_vIntFields.emplace_back(pField, strLabel, bCheckExists);

	if (pField)
	{
		*pField = nDef;
	}

	m_nFields++;
}

//**********************************************************************

void CSQLRecordSetMetadata::AddInt64Field(__int64* pField, const CString strLabel, const __int64 nDef, const bool bCheckExists)
{
	m_vInt64Fields.emplace_back(pField, strLabel, bCheckExists);

	if (pField)
	{
		*pField = nDef;
	}

	m_nFields++;
}

//**********************************************************************

void CSQLRecordSetMetadata::AddDecimalField(CString* pField, const CString strLabel, const CString strDef, const bool bCheckExists)
{
	m_vDecimalFields.emplace_back(pField, strLabel, bCheckExists);

	if (pField)
	{
		*pField = strDef;
	}

	m_nFields++;
}

//**********************************************************************

void CSQLRecordSetMetadata::AddBOOLField(BOOL* pField, const CString strLabel, const BOOL bDef, const bool bCheckExists)
{
	m_vBOOLFields.emplace_back(pField, strLabel, bCheckExists);

	if (pField)
	{
		*pField = bDef;
	}

	m_nFields++;
}

//**********************************************************************

void CSQLRecordSetMetadata::AddTextParam(CString strLabel, const CString strDef)
{
	if (m_nParams < MAX_GENERAL_SQLPARAMS)
	{
		m_vIntParamTypes.emplace_back(SQLSRVR_TEXT);
		m_vTextParams.emplace_back(&m_strParamValue[m_nParams], strLabel, FALSE);
		m_strParamValue[m_nParams] = strDef;
		m_nParams++;
	}
}

//**********************************************************************

void CSQLRecordSetMetadata::AddIntParam(CString strLabel, const int nDef)
{
	if (m_nParams < MAX_GENERAL_SQLPARAMS)
	{
		m_vIntParamTypes.emplace_back(SQLSRVR_INT32);
		m_vInt32Params.emplace_back(&m_nParamValue[m_nParams], strLabel, FALSE);
		m_nParamValue[m_nParams] = nDef;
		m_nParams++;
	}
}

//**********************************************************************
