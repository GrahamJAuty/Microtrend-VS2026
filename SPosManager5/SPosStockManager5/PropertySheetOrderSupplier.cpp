/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "PropertySheetOrderSupplier.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetOrderSupplier, CPropertySheet)
/**********************************************************************/

CPropertySheetOrderSupplier::CPropertySheetOrderSupplier( int nStockIdx, int nStockSuppIdx, CWnd* pWndParent)
	 : CSSPropertySheet( "", pWndParent, 0 )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	m_Page1.SetSuppIdx ( nStockSuppIdx, NULL, 0 );
	m_Page1.AllowMakePreferred( TRUE );
	m_Page1.AllowChangeSupplier( FALSE );
	m_Page1.SetInitialTabOnlyFlag( TRUE );
	
	AddPage(&m_Page1);
	m_PageArray.Add(1);
	
	m_Page1.SetStockIdx ( nStockIdx );
	
	DataManager.Stock.GetAt ( nStockIdx, m_StockRecord );
	m_StockChangesBuffer.SetStockCode ( m_StockRecord.GetStockCode(), TRUE );
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetOrderSupplier::~CPropertySheetOrderSupplier()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetOrderSupplier, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetOrderSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetOrderSupplier::UpdateTitle()
{
	CString strText; 
	strText.Format ( "Edit Stock Item %s %s",
		m_StockRecord.GetStockCode(),
		m_StockRecord.GetDescription() );
	
	SetTitle ( strText, 0 );
}

/**********************************************************************/



