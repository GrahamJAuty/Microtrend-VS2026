/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporterStatusDisplay.h"
/**********************************************************************/

CCashRSPImporterStatusDisplay::CCashRSPImporterStatusDisplay(void)
{
}

/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

CCashRSPImporterStatusDisplayPosTray::CCashRSPImporterStatusDisplayPosTray( CMainFramePosTray* pMainFrame )
{
	m_pMainFrame = pMainFrame;
}

/**********************************************************************/

void CCashRSPImporterStatusDisplayPosTray::DisplayStatus( const char* szStatus )
{
	m_pMainFrame -> DisplayCashRSPStatus( szStatus );
}

/**********************************************************************/
#endif
/**********************************************************************/
