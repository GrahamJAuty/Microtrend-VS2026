//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "AccumulatorPropertySheet.h"
//$$******************************************************************

CAccumulatorPropertySheet::CAccumulatorPropertySheet(const char* szLabel, bool bPluPage, CWnd* pParentWnd, UINT iSelectPage)
	:CSSPropertySheet("", pParentWnd, iSelectPage)
{
	m_pReportInfo = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);										// report on
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;

	if (TRUE == bPluPage)
	{
		AddPage(&m_Page2);										// plu list
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page2.SetPointer("R106b");
	}
}

//$$******************************************************************

CAccumulatorPropertySheet::~CAccumulatorPropertySheet()
{
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CAccumulatorPropertySheet, CPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CAccumulatorPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	SetWindowText ( m_pReportInfo -> GetCaption() );
	return bResult;
}

//$$******************************************************************

void CAccumulatorPropertySheet::SetReportInfo( CReportInfoAccumulator* pReportInfo )
{
	m_pReportInfo = pReportInfo;
	m_Page1.SetReportInfo(pReportInfo);
	m_Page2.SetReportInfo(pReportInfo);
}

//$$******************************************************************
