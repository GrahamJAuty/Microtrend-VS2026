/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "FileRemover.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PropertySheetPMSOptions.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPMSOptions, CPropertySheet)
/**********************************************************************/

CPropertySheetPMSOptions::CPropertySheetPMSOptions( bool bCanWriteDatabase, CPMSEmailSignatures& EmailSignatures, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent), m_EmailSignatures( EmailSignatures )
{	
	m_page6.SetPMSOptions( &PMSOptions );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;	
	AddPage( &m_page1 );
	AddPage( &m_page2 );
	AddPage( &m_page3 );
	AddPage( &m_page4 );
	AddPage( &m_page5 );
	AddPage( &m_page6 );
	AddPage( &m_page7 );
	AddPage( &m_page8 );
	AddPage( &m_page9 );
	AddPage( &m_page10 );
	m_page8.SetCanChangeDatabaseFlag( bCanWriteDatabase );
	m_page10.SetEmailSignatures( &m_EmailSignatures );
	SetTitle ( "Setup Guest Account System" );
}

/**********************************************************************/

CPropertySheetPMSOptions::~CPropertySheetPMSOptions()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPMSOptions, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPMSOptions)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
#endif
/**********************************************************************/
