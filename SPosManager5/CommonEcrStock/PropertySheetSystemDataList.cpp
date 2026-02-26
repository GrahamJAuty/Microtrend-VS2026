/**********************************************************************/
#include "LocationCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "PropertySheetSystemDataList.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetSystemDataList, CPropertySheet)
/**********************************************************************/

CPropertySheetSystemDataList::CPropertySheetSystemDataList( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_Page1.SetHelpers( &m_Helpers );
	m_Page2.SetHelpers( &m_Helpers );
	m_Page3.SetHelpers( &m_Helpers );
	m_Page4.SetHelpers( &m_Helpers );
	m_Page5.SetHelpers( &m_Helpers );
	m_Page6.SetHelpers( &m_Helpers );
	m_Page7.SetHelpers( &m_Helpers );
	m_Page8.SetHelpers( &m_Helpers );
	
	m_bDataValidationError = FALSE;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	SetTitle ( "Edit System Data" );

	int nAccessType = PasswordArray.GetLocationAccessType();
	
	m_Page1.SetDataPageType ( DATAPAGE_TYPE_ECRDATA );
	if ( m_Page1.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_Page1 );
		m_Page1.m_psp.dwFlags |= PSP_USETITLE;
		m_Page1.m_psp.pszTitle = "Epos Data";
		m_Page1.SetPropSheetPointer( this );
	}

	m_Page2.SetDataPageType ( DATAPAGE_TYPE_PRICESET );
	if ( m_Page2.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_Page2 );
		m_Page2.m_psp.dwFlags |= PSP_USETITLE;
		m_Page2.m_psp.pszTitle = "Price Sets";
		m_Page2.SetPropSheetPointer( this );
	}

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_Page3.SetDataPageType ( DATAPAGE_TYPE_MIXMATCH );
		if ( m_Page3.IsRelevant( nAccessType ) == TRUE )
		{
			AddPage( &m_Page3 );
			m_Page3.m_psp.dwFlags |= PSP_USETITLE;
			m_Page3.m_psp.pszTitle = "Mix Match";
			m_Page3.SetPropSheetPointer( this );
		}
	}

	m_Page4.SetDataPageType ( DATAPAGE_TYPE_STOCKDATA );
	if ( m_Page4.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_Page4 );
		m_Page4.m_psp.dwFlags |= PSP_USETITLE;
		m_Page4.m_psp.pszTitle = "Stock Data";
		m_Page4.SetPropSheetPointer( this );
	}

	m_Page5.SetDataPageType ( DATAPAGE_TYPE_KEYBOARD );
	if ( m_Page5.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_Page5 );
		m_Page5.m_psp.dwFlags |= PSP_USETITLE;
		m_Page5.m_psp.pszTitle = "Keyboards";
		m_Page5.SetPropSheetPointer( this );
	}
	
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_Page6.SetDataPageType ( DATAPAGE_TYPE_SERVER );
		if ( m_Page6.IsRelevant( nAccessType ) == TRUE )
		{
			m_Page6.SetDataPageType ( DATAPAGE_TYPE_SERVER );
			AddPage( &m_Page6 );
			m_Page6.m_psp.dwFlags |= PSP_USETITLE;
			m_Page6.m_psp.pszTitle = "Servers";
		}

		m_Page8.SetDataPageType ( DATAPAGE_TYPE_SVRPRESET );
		if ( m_Page8.IsRelevant( nAccessType ) == TRUE )
		{
			m_Page8.SetDataPageType ( DATAPAGE_TYPE_SVRPRESET );
			AddPage( &m_Page8 );
			m_Page8.m_psp.dwFlags |= PSP_USETITLE;
			m_Page8.m_psp.pszTitle = "Server Presets";
		}
	}
	
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_Page7.SetDataPageType ( DATAPAGE_TYPE_CUSTOMER );
		if ( m_Page7.IsRelevant( nAccessType ) == TRUE )
		{
			m_Page7.SetDataPageType ( DATAPAGE_TYPE_CUSTOMER );
			AddPage( &m_Page7 );
			m_Page7.m_psp.dwFlags |= PSP_USETITLE;
			m_Page7.m_psp.pszTitle = "Customers";
		}
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetSystemDataList, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetSystemDataList)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**********************************************************************/

BOOL CPropertySheetSystemDataList::OnInitDialog() 
{
	bool bResult = ( CPropertySheet::OnInitDialog() != 0 );
	CWnd* pWnd = GetDlgItem ( IDOK );
	pWnd -> ShowWindow ( SW_HIDE );
	pWnd = GetDlgItem ( IDCANCEL );
	pWnd -> SetWindowText ( "Close" );
	return bResult;
}

/**********************************************************************/

bool CPropertySheetSystemDataList::IsRelevant()
{
	int nAccessType = PasswordArray.GetLocationAccessType();

	if ( LOCATION_ACCESS_NONE == nAccessType )
		return FALSE;

	CPropPageSystemDataList propPage;
	propPage.SetDataPageType ( DATAPAGE_TYPE_ECRDATA );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;
	
	propPage.SetDataPageType ( DATAPAGE_TYPE_STOCKDATA );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;

	propPage.SetDataPageType ( DATAPAGE_TYPE_KEYBOARD );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		propPage.SetDataPageType ( DATAPAGE_TYPE_PRICESET );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_MIXMATCH );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_SERVER );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_SVRPRESET );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_CUSTOMER );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/
