#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SPOSKeyboardDlg.h"
#include "SPOSKeyboardView.h"
/**********************************************************************/

class CSPOSKeyboardDlgViewLink
{
public:
	CSPOSKeyboardDlgViewLink(void);

public:
	void SetDlgPointer( CSPOSKeyboardDlg* p ){ m_pDlg = p; }
	void SetViewPointer( CSPOSKeyboardView* p ){ m_pView = p; }

public:
	void GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount );
	void SetDialogTitle( const char* sz );
	void SetPageNo( int nPageNo );
	void RefreshPageCombo();
	void RefreshListCombo();
	bool GetInvisibleTick();
	bool GetImagesTick();
	int GetViewMode();
	void SetListNo( __int64 nListNo );
	void GetPreviousComboItem( CSPOSKeyboardListComboItem& item );
	void SetPreviousComboItem( CSPOSKeyboardListComboItem& item );

private:
	CSPOSKeyboardView* m_pView;
	CSPOSKeyboardDlg* m_pDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
