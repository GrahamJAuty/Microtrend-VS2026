/**********************************************************************/
 
/**********************************************************************/
#include "DataAccessHelpersSet.h"
/**********************************************************************/

CDataAccessHelpersSet::CDataAccessHelpersSet() 
{
	SetParentWnd( NULL );
}

/**********************************************************************/

void CDataAccessHelpersSet::SetParentWnd( CWnd* pParentWnd )
{
	m_Customer.SetParentWnd( pParentWnd );
	m_EcrData.SetParentWnd( pParentWnd );
	m_EcrText.SetParentWnd( pParentWnd );
	m_Keyboard.SetParentWnd( pParentWnd );
	m_Loyalty.SetParentWnd( pParentWnd );
	m_MixMatch.SetParentWnd( pParentWnd );
	m_Plu.SetParentWnd( pParentWnd );
	m_Reports.SetParentWnd( pParentWnd );
	m_ReportSet.SetParentWnd( pParentWnd );
	m_Server.SetParentWnd( pParentWnd );
	m_Setup.SetParentWnd( pParentWnd );

#ifdef STOCKMAN_SYSTEM
	m_StockData.SetParentWnd( pParentWnd );
	m_StockText.SetParentWnd( pParentWnd );
#endif

}

/**********************************************************************/

