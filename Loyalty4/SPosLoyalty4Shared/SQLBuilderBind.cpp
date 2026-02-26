//**********************************************************************
#include "SQLDefines.h"
//**********************************************************************
#include "SQLBuilderBind.h"
//**********************************************************************

CSQLBuilderBindField::CSQLBuilderBindField()
{
	m_nFieldType = SQLSRVR_TEXT;
	m_strFieldValue = "";
	m_nFieldValue32 = 0;
	m_nFieldValue64 = 0;
	m_dFieldValue = 0.0;
}

//**********************************************************************

CString CSQLBuilderBindField::GetFieldAsString()
{
	CString strResult = "";

	switch (m_nFieldType)
	{
	case SQLSRVR_INT32:
		strResult.Format("%d", m_nFieldValue32);
		break;

	case SQLSRVR_INT64:
		strResult.Format("%I64d", m_nFieldValue64);
		break;

	case SQLSRVR_DECIMAL:
		strResult.Format("%f", m_dFieldValue);
		break;

	case SQLSRVR_TEXT:
		strResult = m_strFieldValue;
		break;
	}
	
	return strResult;
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CSQLBuilderBind::CSQLBuilderBind()
{
}

//**********************************************************************

void CSQLBuilderBind::AddString(CString strFieldValue)
{
	CSQLBuilderBindField Item;
	Item.m_nFieldType = SQLSRVR_TEXT;
	Item.m_strFieldValue = strFieldValue;
	m_arrayFields.Add(Item);
}

//**********************************************************************

void CSQLBuilderBind::AddDouble(double dFieldValue)
{
	CSQLBuilderBindField Item;
	Item.m_nFieldType = SQLSRVR_DECIMAL;
	Item.m_dFieldValue = dFieldValue;
	m_arrayFields.Add(Item);
}

//**********************************************************************

void CSQLBuilderBind::AddInt32(int nFieldValue)
{
	CSQLBuilderBindField Item;
	Item.m_nFieldType = SQLSRVR_INT32;
	Item.m_nFieldValue32 = nFieldValue;
	m_arrayFields.Add(Item);
}

//**********************************************************************

void CSQLBuilderBind::AddInt64(__int64 nFieldValue)
{
	CSQLBuilderBindField Item;
	Item.m_nFieldType = SQLSRVR_INT64;
	Item.m_nFieldValue64 = nFieldValue;
	m_arrayFields.Add(Item);
}

//**********************************************************************
