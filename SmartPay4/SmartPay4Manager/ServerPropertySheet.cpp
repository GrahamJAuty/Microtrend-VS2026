//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "ServerPropertySheet.h"
//*******************************************************************

CServerPropertySheet::CServerPropertySheet( CWnd* pParentWnd, UINT iSelectPage)
	: CSSAutoShutdownPropertySheet(AutoShutdownHelper, "Setup SmartPay Server", pParentWnd, iSelectPage)
{
	Server.Read( Filenames.GetServerDataFilename() );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);									// Communications
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page2);									// Photo ID
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page3);									// Plu Sales
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;

	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		AddPage(&m_Page5);									// Reward Bonus spend Options
		m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;

		if (System.GetActiveEnablePointsFlag() == TRUE)
		{
			AddPage(&m_Page6);								// Points redemption
			m_Page6.m_psp.dwFlags &= ~PSP_HASHELP;
		}
	}
}

//*******************************************************************

BOOL CServerPropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetIcon ( AfxGetApp()->LoadIcon(IDI_ICON_SMARTPAYSERVER), TRUE );
	return TRUE;
}

//*******************************************************************

bool CServerPropertySheet::Save()
{
	return Server.Write ( Filenames.GetServerDataFilename() );
}

//*******************************************************************

