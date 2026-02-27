//**********************************************************************
#include "..\SmartPay4Shared\PaymentAuditReport.h"
//**********************************************************************

CPaymentAuditReport::CPaymentAuditReport ( const char* szAuditFile, CWnd* pParent )
{
	m_strAuditFilename = szAuditFile;
	m_pParent = pParent;
	m_eposPayments.Read();
	m_eposPaymentGroups.Read();
}

//**********************************************************************

