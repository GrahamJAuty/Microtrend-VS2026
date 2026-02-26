/**********************************************************************/
#include "SPOSKeyboardDlgViewLink.h"
/**********************************************************************/

CSPOSKeyboardDlgViewLink::CSPOSKeyboardDlgViewLink(void)
{
	m_pDlg = NULL;
	m_pView = NULL;
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount )
{
	m_pDlg -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::SetDialogTitle( const char* sz )
{
	m_pDlg -> SetWindowText( sz );
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::RefreshPageCombo()
{
	m_pDlg -> RefreshPageCombo();
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::RefreshListCombo()
{
	m_pDlg -> RefreshListCombo();
}

/**********************************************************************/

bool CSPOSKeyboardDlgViewLink::GetInvisibleTick()
{
	return m_pDlg -> GetInvisibleTick();
}

/**********************************************************************/

bool CSPOSKeyboardDlgViewLink::GetImagesTick()
{
	return m_pDlg -> GetImagesTick();
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::SetPageNo( int nPageNo )
{
	return m_pDlg -> SetPageNo( nPageNo );
}

/**********************************************************************/

int CSPOSKeyboardDlgViewLink::GetViewMode()
{
	return m_pDlg -> GetViewMode();
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::SetListNo( __int64 nListNo )
{
	return m_pDlg -> SetListNo( nListNo );
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::GetPreviousComboItem( CSPOSKeyboardListComboItem& item )
{
	m_pDlg -> GetPreviousComboItem( item );
}

/**********************************************************************/

void CSPOSKeyboardDlgViewLink::SetPreviousComboItem( CSPOSKeyboardListComboItem& item )
{
	m_pDlg -> SetPreviousComboItem( item );
}

/**********************************************************************/
