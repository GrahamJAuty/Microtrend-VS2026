//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "ServerPropertySheet.h"
//$$******************************************************************

CServerPropertySheet::CServerPropertySheet( const char* szFilename, CWnd* pParentWnd )
	: CSSPropertySheet( "Setup Server", pParentWnd )
{
	m_strServerDatafile	= szFilename;
	Server.Read ( m_strServerDatafile );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);									// communication
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		break;

	default:
		AddPage(&m_Page9);									// new account
		m_Page9.m_psp.dwFlags &= ~PSP_HASHELP;
		break;
	}

	AddPage(&m_Page2);									// response detail
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page3);									// plu sales
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page8);									// revaluation items
	m_Page8.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page4);									// renewal items
	m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page5);									// Spend Bonus
	m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page6);									// Points Redeem
	m_Page6.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page7);									// Search
	m_Page7.m_psp.dwFlags &= ~PSP_HASHELP;
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CServerPropertySheet, CPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

CServerPropertySheet::~CServerPropertySheet()
{
}

//$$******************************************************************

bool CServerPropertySheet::Save()
{
	bool bResult = Server.Write ( m_strServerDatafile );
	return bResult;
}

//$$******************************************************************

BOOL CServerPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	SetIcon ( AfxGetApp()->LoadIcon(IDI_ICON_SMARTSERVER), TRUE );
	return bResult;
}

//$$******************************************************************

