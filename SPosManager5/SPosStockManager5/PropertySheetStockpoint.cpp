/**********************************************************************/
 
/**********************************************************************/
#include "PropertySheetStockpoint.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetStockpoint, CPropertySheet)
/**********************************************************************/

CPropertySheetStockpoint::CPropertySheetStockpoint( int nSpIdx, bool bAdd, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_nSpIdx = nSpIdx;
	m_bAdd = bAdd;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	AddPage( &m_Page1 );
	
	dbStockpoint.GetAddressInfo( m_nSpIdx, m_infoAddress );
	m_Page1.SetAddressInfo( &m_infoAddress );

	CStockpointCSVRecord* pStockpoint = new CStockpointCSVRecord;
	m_StockpointCSVArray.Add( pStockpoint );
	dbStockpoint.LoadStockpointBuffer( m_nSpIdx, m_StockpointCSVArray );
	m_Page1.SetStockpointBuffer ( &m_StockpointCSVArray );
	
	if ( Sysset.GetMaxStockPoints() > 1 )
	{
		AddPage( &m_Page2 );
		m_Page2.SetStockpointBuffer ( &m_StockpointCSVArray );
	}

	CString strTitle;
	strTitle.Format ( "Edit %s", m_StockpointCSVArray.GetSystemName( 0 ) );
	SetTitle ( strTitle );
}

/**********************************************************************/

CPropertySheetStockpoint::~CPropertySheetStockpoint()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetStockpoint, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetStockpoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetStockpoint::SaveRecord()
{
	dbStockpoint.SetAddressInfo( m_nSpIdx, m_infoAddress ); 
	dbStockpoint.SaveStockpointBuffer( m_nSpIdx, m_StockpointCSVArray );
}

/**********************************************************************/
