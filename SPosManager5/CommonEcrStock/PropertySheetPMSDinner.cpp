/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PropertySheetPMSDinner.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPMSDinner, CPropertySheet)
/**********************************************************************/

CPropertySheetPMSDinner::CPropertySheetPMSDinner( CPMSOptions* pPMSOptions, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_page1.SetPMSOptions( pPMSOptions );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;	
	AddPage( &m_page1 );
	SetTitle ( "Edit Dinner Settings" );
}

/**********************************************************************/

CPropertySheetPMSDinner::~CPropertySheetPMSDinner()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPMSDinner, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPMSDinner)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
#endif
/**********************************************************************/
