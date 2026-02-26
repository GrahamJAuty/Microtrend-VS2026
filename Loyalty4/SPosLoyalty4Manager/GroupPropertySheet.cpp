//$$******************************************************************
#include "GroupComboHelpers.h"
//$$******************************************************************
#include "GroupPropertySheet.h"
//$$******************************************************************

CGroupPropertySheet::CGroupPropertySheet(CSQLRowGroup& RowGroup, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopupBonus, CWnd* pParentWnd, UINT iSelectPage)
	:CSSPropertySheet("", pParentWnd, iSelectPage), m_Page1(RowGroup), m_Page2(RowGroup), m_Page4(RowGroup,arrayGroupTopupBonus)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	CString strTitle;
	strTitle.Format ( "Group %d Details", RowGroup.GetGroupNo() );
	SetTitle ( strTitle );

	Server.Read ( Filenames.GetServerDataFilename() );

	AddPage(&m_Page1);										// General
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page4);										// Revaluation bonus
	m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page2);										// Spend Bonus
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
}

//$$******************************************************************

CGroupPropertySheet::~CGroupPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CGroupPropertySheet, CPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************
