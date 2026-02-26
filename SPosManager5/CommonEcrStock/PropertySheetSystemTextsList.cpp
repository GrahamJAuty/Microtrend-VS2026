/**********************************************************************/
#include "LocationCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetSystemTextsList.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetSystemTextsList, CPropertySheet)
/**********************************************************************/

CPropertySheetSystemTextsList::CPropertySheetSystemTextsList( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_PageEcrText.SetHelpers( &m_Helpers );
	m_PageStockText.SetHelpers( &m_Helpers );
	m_PageReportSet.SetHelpers( &m_Helpers );
	m_PageMacro.SetHelpers( &m_Helpers );
	m_PageReason.SetHelpers(&m_Helpers);
	m_PageTable.SetHelpers( &m_Helpers );
	m_PageBillText.SetHelpers( &m_Helpers );
	m_PageFunction.SetHelpers( &m_Helpers );
	m_PageOPGroup.SetHelpers( &m_Helpers );
	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	SetTitle ( "Edit System Texts" );

	int nAccessType = PasswordArray.GetLocationAccessType();
	
	m_PageEcrText.SetDataPageType ( DATAPAGE_TYPE_ECRTEXT );
	if ( m_PageEcrText.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_PageEcrText );
		m_PageEcrText.m_psp.dwFlags |= PSP_USETITLE;
		m_PageEcrText.m_psp.pszTitle = "EPOS";
	}
	
	m_PageStockText.SetDataPageType ( DATAPAGE_TYPE_STOCKTEXT );
	if ( m_PageStockText.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_PageStockText );
		m_PageStockText.m_psp.dwFlags |= PSP_USETITLE;
		m_PageStockText.m_psp.pszTitle = "Stock";
	}

	m_PageReportSet.SetDataPageType ( DATAPAGE_TYPE_REPORTSET );
	if ( m_PageReportSet.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_PageReportSet );
		m_PageReportSet.m_psp.dwFlags |= PSP_USETITLE;
		m_PageReportSet.m_psp.pszTitle = "Reports";
	}

	m_PageMacro.SetDataPageType ( DATAPAGE_TYPE_MACRO );
	if ( m_PageMacro.IsRelevant( nAccessType ) == TRUE )
	{
		AddPage( &m_PageMacro );
		m_PageMacro.m_psp.dwFlags |= PSP_USETITLE;
		m_PageMacro.m_psp.pszTitle = "Macros";
	}

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_PageReason.SetDataPageType(DATAPAGE_TYPE_REASON);
		if (m_PageReason.IsRelevant(nAccessType) == TRUE)
		{
			AddPage(&m_PageReason);
			m_PageReason.m_psp.dwFlags |= PSP_USETITLE;
			m_PageReason.m_psp.pszTitle = "Reasons";
		}

		if ( dbLocation.GetSize() > 0 )
		{
			m_PageTable.SetDataPageType ( DATAPAGE_TYPE_TABLES );
			if ( m_PageTable.IsRelevant( nAccessType ) == TRUE )
			{
				AddPage( &m_PageTable );
				m_PageTable.m_psp.dwFlags |= PSP_USETITLE;
				m_PageTable.m_psp.pszTitle = "Tables";
			}
		}

		m_PageBillText.SetDataPageType ( DATAPAGE_TYPE_BILLTEXT );
		if ( m_PageBillText.IsRelevant( nAccessType ) == TRUE )
		{
			AddPage( &m_PageBillText );
			m_PageBillText.m_psp.dwFlags |= PSP_USETITLE;
			m_PageBillText.m_psp.pszTitle = "Receipt/Bill";
		}

		m_PageFunction.SetDataPageType ( DATAPAGE_TYPE_FUNCTION );
		if ( m_PageFunction.IsRelevant( nAccessType ) == TRUE )
		{
			AddPage( &m_PageFunction );
			m_PageFunction.m_psp.dwFlags |= PSP_USETITLE;
			m_PageFunction.m_psp.pszTitle = "Functions";
		}

		m_PageOPGroup.SetDataPageType ( DATAPAGE_TYPE_OPGROUP );
		if ( m_PageOPGroup.IsRelevant( nAccessType ) == TRUE )
		{
			AddPage( &m_PageOPGroup );
			m_PageOPGroup.m_psp.dwFlags |= PSP_USETITLE;
			m_PageOPGroup.m_psp.pszTitle = "Order Printer Groups";
		}
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetSystemTextsList, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetSystemTextsList)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**********************************************************************/

BOOL CPropertySheetSystemTextsList::OnInitDialog() 
{
	bool bResult = ( CPropertySheet::OnInitDialog() != 0 );
	CWnd* pWnd = GetDlgItem ( IDOK );
	pWnd -> ShowWindow ( SW_HIDE );
	pWnd = GetDlgItem ( IDCANCEL );
	pWnd -> SetWindowText ( "Close" );
	return bResult;
}

/**********************************************************************/

bool CPropertySheetSystemTextsList::IsRelevant()
{
	int nAccessType = PasswordArray.GetLocationAccessType();

	if ( LOCATION_ACCESS_NONE == nAccessType )
		return FALSE;

	CPropPageSystemDataList propPage;
	propPage.SetDataPageType ( DATAPAGE_TYPE_ECRTEXT );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;	

	propPage.SetDataPageType ( DATAPAGE_TYPE_STOCKTEXT );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;	

	propPage.SetDataPageType ( DATAPAGE_TYPE_REPORTSET );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;

	propPage.SetDataPageType ( DATAPAGE_TYPE_MACRO );
	if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
		return TRUE;
	
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		propPage.SetDataPageType(DATAPAGE_TYPE_REASON);
		if (propPage.IsRelevant(nAccessType) == TRUE)
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_BILLTEXT );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_FUNCTION );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_OPGROUP );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;

		propPage.SetDataPageType ( DATAPAGE_TYPE_TABLES );
		if ( propPage.IsRelevant( nAccessType ) == TRUE ) 
			return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

