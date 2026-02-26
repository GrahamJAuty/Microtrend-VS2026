/**********************************************************************/
#include "AboutDlgEcrman.h"
#include "DemoWarning.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnHelpAbout()
{
	CDemoWarning DemoWarning;
	CAboutDlgEcrman aboutDlg;
	aboutDlg.DoModal();
}

/**********************************************************************/

void CMainFrameEcrman::OnHelpLicenceDetails()
{
	Sysset.DisplayLicenceDetails( "", "", 0, this );
}

/**********************************************************************/
