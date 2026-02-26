//**********************************************************************
#include "SQLRowChartwellExportPayment.h"
//**********************************************************************

CSQLRowChartwellExportPayment::CSQLRowChartwellExportPayment()
{
	Reset();
}

//**********************************************************************

void CSQLRowChartwellExportPayment::Reset()
{
	m_nUserID = 1;
	m_nTransactionID = 1;
	m_nTerminalNo = 1;
	m_strDate = "";
	m_strCreateDateTime = "";
	m_nGroupNo = 1;
	m_dPaymentPurse1 = 0.0;
	m_dPaymentPurse2 = 0.0;
	m_dPaymentPurse3 = 0.0;
}

//**********************************************************************

void CSQLRowChartwellExportPayment::ConsolidatePayment(double dPurse1, double dPurse2, double dPurse3)
{
	m_dPaymentPurse1 += dPurse1;
	m_dPaymentPurse2 += dPurse2;
	m_dPaymentPurse3 += dPurse3;
}

//**********************************************************************

