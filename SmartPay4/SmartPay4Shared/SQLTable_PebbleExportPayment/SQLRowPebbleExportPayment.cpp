//**********************************************************************
#include "..\\ServerData.h"
//**********************************************************************
#include "SQLRowPebbleExportPayment.h"
//**********************************************************************

CSQLRowPebbleExportPayment::CSQLRowPebbleExportPayment()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleExportPayment::Reset()
{
	m_nUserID = 1;
	m_strDate = "";
	m_strTime = "";
	m_strCreateDateTime = "";
	m_nCCNo = 1;

	for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
	{
		m_nPaymentType[n] = -1;
		m_dPaymentAmount[n] = 0.0;
	}
}

//**********************************************************************

int CSQLRowPebbleExportPayment::GetPaymentType(int n)
{
	if ((n >= 0) && (n < MAX_PEBBLE_EXPORT_PAYMENT))
	{
		return m_nPaymentType[n];
	}
	else
	{
		return -1;
	}
}

//**********************************************************************

double CSQLRowPebbleExportPayment::GetPaymentAmount(int n)
{
	if ((n >= 0) && (n < MAX_PEBBLE_EXPORT_PAYMENT))
	{
		return m_dPaymentAmount[n];
	}
	else
	{
		return 0.0;
	}
}

//**********************************************************************

void CSQLRowPebbleExportPayment::SetPaymentType(int n, int nType )
{
	if ((n >= 0) && (n < MAX_PEBBLE_EXPORT_PAYMENT))
	{
		vdtr.SetPaymentType(nType, m_nPaymentType[n]);
	}
}

//**********************************************************************

void CSQLRowPebbleExportPayment::SetPaymentAmount(int n, double dAmount)
{
	if ((n >= 0) && (n < MAX_PEBBLE_EXPORT_PAYMENT))
	{
		vdtr.SetPaymentAmount(dAmount, m_dPaymentAmount[n]);
	}
}

//**********************************************************************

void CSQLRowPebbleExportPayment::ConsolidatePayment(int nType, double dAmount)
{
	for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
	{
		if (m_nPaymentType[n] == nType)
		{
			m_dPaymentAmount[n] += dAmount;
			break;
		}

		if (m_nPaymentType[n] == -1)
		{
			m_nPaymentType[n] = nType;
			m_dPaymentAmount[n] = dAmount;
			break;
		}
	}
}

//**********************************************************************
