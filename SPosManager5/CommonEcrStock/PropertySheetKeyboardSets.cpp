/**********************************************************************/
 
/**********************************************************************/
#include "SystemMap.h"
/**********************************************************************/
#include "PropertySheetKeyboardSets.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetKeyboardSets, CPropertySheet)
/**********************************************************************/

CPropertySheetKeyboardSets::CPropertySheetKeyboardSets( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	SetTitle ( "Setup Keyboard Sets" );
	
	m_Page1.SetTreeType ( SETUPTREE_TYPE_KBSET );
	AddPage( &m_Page1 );
	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Keyboard Sets";
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetKeyboardSets, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetKeyboardSets)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**********************************************************************/

BOOL CPropertySheetKeyboardSets::OnInitDialog() 
{
	bool bResult = ( CPropertySheet::OnInitDialog() != 0 );
	CWnd* pWnd = GetDlgItem ( IDOK );
	pWnd -> ShowWindow ( SW_HIDE );
	pWnd = GetDlgItem ( IDCANCEL );
	pWnd -> SetWindowText ( "Close" );
	return bResult;
}

/**********************************************************************/

void CPropertySheetKeyboardSets::SaveSystem()
{
	SystemMapFull.WriteSystem( TRUE );
}

/**********************************************************************/
	