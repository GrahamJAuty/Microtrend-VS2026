//***************************************************************
// Used by	ParentPay
//			WisePay
//			sQuid
//			ParentMail
//			Pebble
//***************************************************************

//***********************************************************************
#include "..\SmartPay4ManagerBgnd\SquidExport.h"
//***********************************************************************
#include "AccountLinkParentPayDlg.h"
#include "AccountLinkPebbleDlg.h"
#include "AccountLinkWisePayDlg.h"
#include "AccountLinkParentPayDlg.h"
#include "WebPaymentLinkAccountsDlg.h"
//***********************************************************************
#include "WebPaymentLinkAccounts.h"
//***********************************************************************

CWebPaymentLinkAccounts::CWebPaymentLinkAccounts( CWnd* pParent )
{
	m_pParent = pParent;
}

//***************************************************************

void CWebPaymentLinkAccounts::LinkAccounts()
{
	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_PARENTMAIL:
	{
		CWebPaymentLinkAccountsDlg dlg(m_pParent);			// sQuid link \ Disassociated
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_PARENTPAY:
	{
		CAccountLinkParentPayDlg dlg(m_pParent);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_WISEPAY:
	{
		CAccountLinkWisePayDlg dlg(m_pParent);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_PEBBLE:
	{
		CAccountLinkPebbleDlg dlg(m_pParent);
		dlg.DoModal();
	}
	break;
	}
}

//***************************************************************
