/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "PosTrayTokenFiles.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SysInfo.h"
#include "SystemMap.h"
#include "PropertySheetSetup.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetSetup, CPropertySheet)
/**********************************************************************/

CPropertySheetSetup::CPropertySheetSetup(CWnd* pWndParent)
	: CSSPropertySheet("", pWndParent)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	SetTitle("Setup System");

	m_Page1.SetTreeType(SETUPTREE_TYPE_NETWORK);
	AddPage(&m_Page1);
	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Networks";

	if (SysInfo.IsSportsBookerSystem() == TRUE)
	{
		m_Page7.SetTreeType(SETUPTREE_TYPE_SPTBOOK);
		AddPage(&m_Page7);
		m_Page7.m_psp.dwFlags |= PSP_USETITLE;
		m_Page7.m_psp.pszTitle = "Sports Booker";
	}

	if (SysInfo.IsSmartEntertainmentSystem() == TRUE)
	{
		m_Page8.SetTreeType(SETUPTREE_TYPE_SMARTENT);
		AddPage(&m_Page8);
		m_Page8.m_psp.dwFlags |= PSP_USETITLE;
		m_Page8.m_psp.pszTitle = "Smart Entertainment";
	}

	m_Page2.SetTreeType(SETUPTREE_TYPE_SYSTEM);
	AddPage(&m_Page2);
	m_Page2.m_psp.dwFlags |= PSP_USETITLE;
	m_Page2.m_psp.pszTitle = "Databases";

#ifdef STOCKMAN_SYSTEM
	m_Page3.SetTreeType(SETUPTREE_TYPE_STOCK);
	AddPage(&m_Page3);
	m_Page3.m_psp.dwFlags |= PSP_USETITLE;
	m_Page3.m_psp.pszTitle = "Stockpoints";
#endif

	if ((EcrmanOptions.GetReportsReportpointFlag() == TRUE) || (dbReportpoint.GetSize() > 0))
	{
		m_Page4.SetTreeType(SETUPTREE_TYPE_REPPOINT);
		AddPage(&m_Page4);
		m_Page4.m_psp.dwFlags |= PSP_USETITLE;
		m_Page4.m_psp.pszTitle = "Report Points";
	}

	if ((EcrmanOptions.GetReportsLocationSetFlag() == TRUE) || (dbLocationSet.GetSize() > 0))
	{
		m_Page6.SetTreeType(SETUPTREE_TYPE_LOCSET);
		AddPage(&m_Page6);
		m_Page6.m_psp.dwFlags |= PSP_USETITLE;
		m_Page6.m_psp.pszTitle = "Location Sets";
	}

#ifdef STOCKMAN_SYSTEM
	if ((StockOptions.GetReportsSiteFlag() == TRUE) || (dbSite.GetSize() > 0))
	{
		m_Page5.SetTreeType(SETUPTREE_TYPE_SITE);
		AddPage(&m_Page5);
		m_Page5.m_psp.dwFlags |= PSP_USETITLE;
		m_Page5.m_psp.pszTitle = "Stock Sites";
	}
#endif
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetSetup, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetSetup)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**********************************************************************/

BOOL CPropertySheetSetup::OnInitDialog() 
{
	bool bResult = ( CPropertySheet::OnInitDialog() != 0 );
	CWnd* pWnd = GetDlgItem ( IDOK );
	pWnd -> ShowWindow ( SW_HIDE );
	pWnd = GetDlgItem ( IDCANCEL );
	pWnd -> SetWindowText ( "Close" );
	return bResult;
}

/**********************************************************************/

void CPropertySheetSetup::SaveSystem()
{
	SystemMapFull.WriteSystem( TRUE );
	CPosTrayTokenFiles::CreateSystemUpdateFile();
}

/**********************************************************************/
	