//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayExport.h"
#include "..\SmartPay4ManagerBgnd\WisePayExport.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
//*******************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************
// Send opening balances from a List file

void CWebPaymentExport::SendOpeningBalanceList ( const char* szListFilename )
{
	m_strError == "";

	if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTPAY )
	{
		CParentPayData data;
		data.Read();

		CParentPayExport tx ( &data, m_pParent );
		if ( tx.SendOpeningBalanceList ( szListFilename ) == FALSE )
			m_strError = tx.GetError();									// will be false if send cancelled by user
	}
	else if (System.GetWebPaymentType() == nWEBPAYMENT_WISEPAY)
	{
		CWisePayExport tx(m_pParent);
		if (tx.SendOpeningBalanceList(szListFilename) == FALSE)
		{
			m_strError = tx.GetError();									// will be false if send cancelled by user
		}
	}

	if (m_strError != "")
	{
		Prompter.FatalError(m_strError, "Send Opening Balance Updates");
	}
}

//*******************************************************************
// Send Diner Match update from a List files

void CWebPaymentExport::SendDinerMatchList ( const char* szListFilename )
{
	if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTPAY )
	{
		CParentPayData data;
		data.Read();

		CParentPayExport pp ( &data, m_pParent );
		if ( pp.SendDinerMatchList ( szListFilename ) == FALSE )
		{
			m_strError = pp.GetError();									// will be false if send cancelled by user
			if ( m_strError != "" )
				Prompter.FatalError( pp.GetError(), "Send Diner Match Updates" );
		}
	}
}

//*******************************************************************
