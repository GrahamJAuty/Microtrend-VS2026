//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "GroupPropertySheet.h"
//$$******************************************************************

CGroupPropertySheet::CGroupPropertySheet(CSQLRowGroupFull& RowGroup, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet( AutoShutdownHelper, "", pParentWnd, iSelectPage)
	,m_Page1(RowGroup)
	,m_Page2(RowGroup)
	,m_Page6(RowGroup)
	,m_Page7(RowGroup)
	,m_Page8(RowGroup)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	CString strTitle;
	strTitle.Format ( "Group %d Details", RowGroup.GetGroupNo() );
	SetTitle ( strTitle );

	Server.Read ( Filenames.GetServerDataFilename() );

	AddPage(&m_Page1);										// General
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page7);										// Max Spend
	m_Page7.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page8);										// Purses
	m_Page8.m_psp.dwFlags &= ~PSP_HASHELP;
	
	bool bRevaluationAllowed = Server.RevaluationBonusEnabled();
	bool bSpendAllowed = Server.SpendBonusEnabled();

	if ( bRevaluationAllowed == TRUE || bSpendAllowed == TRUE )
	{
		AddPage(&m_Page2);									// Bonus page
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page2.SetPointer ( Server.RevaluationPlu2Inuse(), bRevaluationAllowed, bSpendAllowed );
	}

	if ( Server.RedeemPointsEnabled() == TRUE )
	{
		AddPage(&m_Page6);									// Bonus page
		m_Page6.m_psp.dwFlags &= ~PSP_HASHELP;
	}
}

//$$******************************************************************

CGroupPropertySheet::~CGroupPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CGroupPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************
