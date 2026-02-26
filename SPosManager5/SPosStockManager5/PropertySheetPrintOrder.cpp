/**********************************************************************/
 
/**********************************************************************/
#include "PropertySheetPrintOrder.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPrintOrder, CPropertySheet)
/**********************************************************************/

CPropertySheetPrintOrder::CPropertySheetPrintOrder( bool bEmail, CProcessedOrderDetails& OrderDetails, int nDefSpIdx, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent), m_OrderDetails ( OrderDetails ) 
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_bEmail = bEmail;
	m_nDefSpIdx = nDefSpIdx;

	m_Page1.SetDefSpIdx( m_nDefSpIdx );
	m_Page1.SetOrderDetails ( &m_OrderDetails );
	m_Page2.SetOrderDetails ( &m_OrderDetails );
	AddPage(&m_Page1);
	AddPage(&m_Page2);

	if ( m_bEmail == FALSE )
		SetTitle ( "Print Purchase Orders" );
	else
		SetTitle ( "Email Purchase Orders" );
}

/**********************************************************************/

CPropertySheetPrintOrder::~CPropertySheetPrintOrder()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPrintOrder, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPrintOrder)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CPropertySheetPrintOrder::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/**********************************************************************/
