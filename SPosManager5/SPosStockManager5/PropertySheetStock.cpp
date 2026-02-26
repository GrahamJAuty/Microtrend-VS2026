/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\ItemTabPageTypes.h"
#include "Password.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PropertySheetStock.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetStock, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetStockInfo::CPropertySheetStockInfo()
{
	m_bReadOnly = FALSE;
	m_bEnableSpin = FALSE;
	m_bPluLinkTab = FALSE;
	m_bStockLevelTab = FALSE;
	m_bApparentTab = TRUE;
	m_bSupplierEdit = FALSE;
	m_bSingleSpIdx = FALSE;
	m_nDefaultSuppNo = 0;
	m_nApparentComboPos = 0;
	m_nSpIdxToEdit = 0;
	m_wSpinID = IDC_SPIN;
	m_pStockFilter = NULL;
}

/**********************************************************************/

CPropertySheetStock::CPropertySheetStock( CPropertySheetStockInfo& info, CWnd* pWndParent)
	 : CSpinPropertySheet( ( info.m_bEnableSpin != 0 ), info.m_wSpinID, "", pWndParent), m_config( info )
{
	m_nLastSupplierTab = -1;
	m_bDatabaseIsModified = FALSE;
	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_PageStockGeneral.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PageStockPluLink.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PageStockSupplier.SetReadOnlyFlag( m_config.m_bReadOnly );
	m_PageStockApparent.SetReadOnlyFlag( m_config.m_bReadOnly );
	
	m_PageStockGeneral.SetCategoryFilter ( m_config.m_pStockFilter -> GetCategoryFilter() );
	m_PageStockPluLink.SetStockChangesBuffer ( &m_StockChangesBuffer);	
	m_PageStockSupplier.SetSuppIdx ( 0, &m_nLastSupplierTab, m_config.m_nDefaultSuppNo );
	m_PageStockSupplier.AllowChangeSupplier( m_config.m_bSupplierEdit );
	m_PageStockSupplier.AllowMakePreferred( m_config.m_bSupplierEdit );

	AddPage (&m_PageStockGeneral);
	m_PageTypeArray.Add( ITEMTAB_STOCK_GENERAL );

	if ( m_config.m_bPluLinkTab == TRUE )
	{
		AddPage (&m_PageStockPluLink );
		m_PageTypeArray.Add( ITEMTAB_STOCK_PLULINK );
	}

	if ( StockOptions.GetStockMaxSuppliersPerItem() > 0 )
	{
		AddPage(&m_PageStockSupplier);
		m_PageTypeArray.Add( ITEMTAB_STOCK_SUPPLIER);
	}
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartIdx, nEndIdx );

	m_PageStockApparent.SetAllowEditFlag( FALSE );
	if ( ( nEndIdx >= nStartIdx ) && ( m_config.m_bReadOnly == FALSE ) )
		if ( m_config.m_bStockLevelTab == TRUE )
			if ( ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC ) || ( PasswordArray.GetLocationAccessSpIdx() != -1 ) )
				m_PageStockApparent.SetAllowEditFlag( TRUE );

	if ( nEndIdx >= nStartIdx )
	{
		if ( m_config.m_bApparentTab == TRUE )
		{
			if ( ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC ) || ( PasswordArray.GetLocationAccessSpIdx() != -1 ) )
			{
				m_PageStockApparent.SetApparentComboPos( m_config.m_nApparentComboPos );
				m_PageStockApparent.SetDefaultSpIdx( m_config.m_nSpIdxToEdit );
				m_PageStockApparent.SetSingleSpIdxFlag( m_config.m_bSingleSpIdx );
				AddPage ( &m_PageStockApparent );
				m_PageTypeArray.Add( ITEMTAB_STOCK_APPARENT );
			}
		}
	}
			
	//get record based on this index
	int nStockIdx = m_config.m_pStockFilter -> GetStockItemIndex();
	
	m_PageStockGeneral.SetStockIdx ( nStockIdx );
	m_PageStockPluLink.SetStockIdx ( nStockIdx );
	m_PageStockSupplier.SetStockIdx ( nStockIdx );
	m_PageStockApparent.SetStockIdx ( nStockIdx );

	DataManager.Stock.GetAt ( nStockIdx, m_StockRecord );

	m_StockChangesBuffer.SetStockCode ( m_StockRecord.GetStockCode(), m_config.m_bPluLinkTab );
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetStock::~CPropertySheetStock()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetStock, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetStock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetStock::SpinNext()
{
	if ( m_config.m_pStockFilter -> HasNext() == TRUE )
	{
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_STOCK_GENERAL:		bUpdate = m_PageStockGeneral.UpdateRecord();	break;
		case ITEMTAB_STOCK_PLULINK:		bUpdate = TRUE;									break;
		case ITEMTAB_STOCK_SUPPLIER:	bUpdate = m_PageStockSupplier.UpdateRecord();	break;
		case ITEMTAB_STOCK_APPARENT:	bUpdate = TRUE;									break;
		}
		
		if ( bUpdate == FALSE )
			return;

		m_bDatabaseIsModified |= m_StockChangesBuffer.CheckModified();

		m_config.m_pStockFilter -> Next();
		int nStockIdx = m_config.m_pStockFilter -> GetStockItemIndex();
		DataManager.Stock.GetAt ( nStockIdx, m_StockRecord );
		m_PageStockGeneral.SetStockIdx ( nStockIdx );
		m_PageStockPluLink.SetStockIdx ( nStockIdx );
		m_PageStockSupplier.SetStockIdx ( nStockIdx );
		m_PageStockApparent.SetStockIdx ( nStockIdx );
		m_StockChangesBuffer.SetStockCode ( m_StockRecord.GetStockCode(), m_config.m_bPluLinkTab );
		UpdateTitle();

		switch ( nPage )
		{
		case ITEMTAB_STOCK_GENERAL:		m_PageStockGeneral.Refresh();	break;	
		case ITEMTAB_STOCK_PLULINK:		m_PageStockPluLink.Refresh();	break;	
		case ITEMTAB_STOCK_SUPPLIER:	m_PageStockSupplier.Refresh();	break;	
		case ITEMTAB_STOCK_APPARENT:	m_PageStockApparent.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetStock::SpinPrevious()
{
	if ( m_config.m_pStockFilter -> HasPrevious() == TRUE )
	{
		int nPage = m_PageTypeArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case ITEMTAB_STOCK_GENERAL:		bUpdate = m_PageStockGeneral.UpdateRecord();	break;
		case ITEMTAB_STOCK_PLULINK:		bUpdate = TRUE;									break;
		case ITEMTAB_STOCK_SUPPLIER:	bUpdate = m_PageStockSupplier.UpdateRecord();	break;
		case ITEMTAB_STOCK_APPARENT:	bUpdate = TRUE;									break;
		}
		
		if ( bUpdate == FALSE )
			return;

		m_bDatabaseIsModified |= m_StockChangesBuffer.CheckModified();

		m_config.m_pStockFilter -> Previous();
		int nStockIdx = m_config.m_pStockFilter -> GetStockItemIndex();
		DataManager.Stock.GetAt ( nStockIdx, m_StockRecord );
		m_PageStockGeneral.SetStockIdx ( nStockIdx );
		m_PageStockPluLink.SetStockIdx ( nStockIdx );
		m_PageStockSupplier.SetStockIdx ( nStockIdx );
		m_PageStockApparent.SetStockIdx ( nStockIdx );
		m_StockChangesBuffer.SetStockCode ( m_StockRecord.GetStockCode(), m_config.m_bPluLinkTab );
		UpdateTitle();

		switch ( nPage )
		{
		case ITEMTAB_STOCK_GENERAL:		m_PageStockGeneral.Refresh();	break;	
		case ITEMTAB_STOCK_PLULINK:		m_PageStockPluLink.Refresh();	break;	
		case ITEMTAB_STOCK_SUPPLIER:	m_PageStockSupplier.Refresh();	break;	
		case ITEMTAB_STOCK_APPARENT:	m_PageStockApparent.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetStock::UpdateTitle()
{
	CString strAction = ( m_config.m_bReadOnly ) ? "View" : "Edit";

	CString strText; 
	strText.Format ( "%s Stock Item %s %s",
		(const char*) strAction,
		m_StockRecord.GetStockCode(),
		m_StockRecord.GetDescription() );
	
	SetTitle ( strText, 0 );
}

/**********************************************************************/
