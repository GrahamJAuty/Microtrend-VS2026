/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetHiddenOptions.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetHiddenOptions, CPropertySheet)
/**********************************************************************/

CPropertySheetHiddenOptions::CPropertySheetHiddenOptions( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

#ifdef STOCKMAN_SYSTEM
	AddPage( &m_Page1 );
	AddPage( &m_Page2 );
#else
	AddPage( &m_Page1 );
#endif

	SetTitle ( "Setup Hidden Options" );
}

/**********************************************************************/

CPropertySheetHiddenOptions::~CPropertySheetHiddenOptions()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetHiddenOptions, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetHiddenOptions)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**********************************************************************/

