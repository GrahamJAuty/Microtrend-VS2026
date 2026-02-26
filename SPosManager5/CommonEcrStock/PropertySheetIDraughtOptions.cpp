/**********************************************************************/
 
/**********************************************************************/
#include "IDraughtOptions.h"
/**********************************************************************/
#include "PropertySheetIDraughtOptions.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetIDraughtOptions, CPropertySheet)
/**********************************************************************/

CPropertySheetIDraughtOptions::CPropertySheetIDraughtOptions( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	IDraughtOptions.ResetDepartmentFilters();

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage( &m_Page1 );
	AddPage( &m_Page2 );
	AddPage( &m_Page3 );

	SetTitle ( "Setup i Draught Link" );
}

/**********************************************************************/

CPropertySheetIDraughtOptions::~CPropertySheetIDraughtOptions()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetIDraughtOptions, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetIDraughtOptions)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

