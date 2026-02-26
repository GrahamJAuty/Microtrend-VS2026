/**********************************************************************/
#include "AboutDlgStockman.h"
#include "DemoWarning.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnHelpAbout()
{
	CDemoWarning DemoWarning;
	CAboutDlgStockman aboutDlg;
	aboutDlg.DoModal();
}

/**********************************************************************/

void CMainFrameStockman::OnHelpLicenceDetails()
{
	Sysset.DisplayLicenceDetails( "", "", 0, this );
}

/**********************************************************************/
