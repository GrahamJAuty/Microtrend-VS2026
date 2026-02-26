/**********************************************************************/
#include "SPOSKeyboardListRecord.h"
/**********************************************************************/
#include "SPOSKeyboardPropertySheetListSelect.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardPropertySheetListSelect, CPropertySheet)
/**********************************************************************/

CSPOSKeyboardPropertySheetListSelect::CSPOSKeyboardPropertySheetListSelect( bool bSI, bool bLI, bool bSF, bool bLF, bool bBB, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Standard List";
	m_Page1.m_nListType = LISTTYPE_ITEM_STANDARD;

	m_Page2.m_psp.dwFlags |= PSP_USETITLE;
	m_Page2.m_psp.pszTitle = "Large List";
	m_Page2.m_nListType = LISTTYPE_ITEM_LARGE;

	m_Page3.m_psp.dwFlags |= PSP_USETITLE;
	m_Page3.m_psp.pszTitle = "Standard Function";
	m_Page3.m_nListType = LISTTYPE_FUNCTION_STANDARD;

	m_Page4.m_psp.dwFlags |= PSP_USETITLE;
	m_Page4.m_psp.pszTitle = "Large Function";
	m_Page4.m_nListType = LISTTYPE_FUNCTION_LARGE;

	m_Page5.m_psp.dwFlags |= PSP_USETITLE;
	m_Page5.m_psp.pszTitle = "Bottom Bar";
	m_Page5.m_nListType = LISTTYPE_FUNCTION_BOTTOM;

	if ( TRUE == bSI )	AddPage( &m_Page1 );
	if ( TRUE == bLI )	AddPage( &m_Page2 );
	if ( TRUE == bSF )	AddPage( &m_Page3 );
	if ( TRUE == bLF )	AddPage( &m_Page4 );
	if ( TRUE == bBB )	AddPage( &m_Page5 );

	SetTitle ( "Select Lists to Change" );
}

/**********************************************************************/

CSPOSKeyboardPropertySheetListSelect::~CSPOSKeyboardPropertySheetListSelect()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropertySheetListSelect, CPropertySheet)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropertySheetListSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CSPOSKeyboardPropertySheetListSelect::SaveRecord()
{
}

/**********************************************************************/

BOOL CSPOSKeyboardPropertySheetListSelect::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;
}

/**********************************************************************/
