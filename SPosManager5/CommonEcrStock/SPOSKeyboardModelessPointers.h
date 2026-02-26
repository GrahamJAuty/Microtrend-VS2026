#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SPOSKeyboardColourDropperDlg.h"
#include "SPOSKeyboardView.h"
/**********************************************************************/

class CSPOSKeyboardModelessPointers
{
public:
	CSPOSKeyboardModelessPointers();

	void SetView( CSPOSKeyboardView* pView ) { m_pView = pView; }

	void OpenColourDropper( int nMode );
	int GetColourDropperMode();
	bool IsColourDropperActive();
	void CloseColourDropper();

	void SetColoursToDropAsShown();
	void SetColoursToNoChange();

	bool CanDropKeyNow();

	void SetForeground( int n );
	void SetBackground( int n );
	void SetFontSize( int n );
	void SetHAlign( int n );
	void SetVAlign( int n );
	void SetBoldStatus( int n );
	void SetDefaultTextAlwaysStatus( int n );
	void SelectImageTab();
	void SetImageFlag( bool b );
	void SetImageFilename( const char* sz );
	void SetImageScale( int n );
	void UpdateImage();
	
	bool GetFilterFlag() { return m_bFilter; }
	void SetFilterFlag( bool bFlag ) { m_bFilter = bFlag; }

	int GetDepartmentFilter() { return m_nDepartmentFilter; }
	void SetDepartmentFilter( int n ) { m_nDepartmentFilter = n; }

	int GetCategoryFilter() { return m_nCategoryFilter; }
	void SetCategoryFilter( int n ) { m_nCategoryFilter = n; }

	void UpdateKeyRecord( CSPOSKeyboardKeyRecord& KeyRecord );
	void SetKeyToCopy( CSPOSKeyboardKeyRecord& KeyRecord );

	void RedrawView();

public:
	void UpdateTitle();
	void SetCopyImageTitle();
	
private:
	void NullifyColourDropper();
	
private:
	CSPOSKeyboardView* m_pView;
	CSPOSKeyboardColourDropperDlg* m_pColourDropper;

private:
	bool m_bFilter;
	int m_nDepartmentFilter;
	int m_nCategoryFilter;

	friend class CSPOSKeyboardColourDropperDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
