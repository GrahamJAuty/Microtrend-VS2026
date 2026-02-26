/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "ProcessedOrderDatabaseDlg.h"
#include "ProcessedOrderNum.h"
 
/**********************************************************************/
#include "ProcessedOrderListDlg.h"
/**********************************************************************/

CProcessedOrderListDlg::CProcessedOrderListDlg( bool bClosed, bool bDbOrders, CArray<int,int>& arrayEntityIdx, CWnd* pParent )
	: CSSDialog(CProcessedOrderListDlg::IDD, pParent), m_arrayEntityIdx( arrayEntityIdx ),
	m_ContentHelpers( bClosed, m_bFiltered ),
	m_DisplayHelpers( m_listSupplier, m_ContentHelpers )
{
	m_bDbOrders = bDbOrders;
	m_bFiltered = FALSE;
	m_nLocLineIdx = 0;

	CDataManagerInfo info;
	DataManager.OpenDatabaseQuickNoFiles(0,FALSE);
	DataManager.OpenSupplier( DB_READONLY, info );
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
}

/**********************************************************************/

CProcessedOrderListDlg::~CProcessedOrderListDlg()
{
	CDataManagerInfo infoDM;
	DataManager.OpenDatabaseQuickNoFiles(0,FALSE);
	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();
	DataManager.CloseSupplier( infoDM );
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CProcessedOrderListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderListDlg)
	DDX_Control( pDX, IDC_BUTTON1, m_button1 );
	DDX_Control( pDX, IDC_BUTTON2, m_button2 );
	DDX_Control( pDX, IDC_BUTTON3, m_button3 );
	DDX_Control( pDX, IDC_BUTTON4, m_button4 );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderListDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCATION, OnSelectLocation)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SUPPLIER, OnGetDispInfoListSupplier)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUPPLIER, OnDoubleClickListSupplier)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ConfigureDialog();
	
	m_listLocation.SubclassDlgItem( IDC_LIST_LOCATION, this );
	m_listLocation.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listLocation.InsertColumn ( 0, m_bDbOrders ? "Database" : "Stockpoint", LVCFMT_LEFT, 220 );

	m_listSupplier.SubclassDlgItem( IDC_LIST_SUPPLIER, this );
	m_listSupplier.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listSupplier.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSupplier.InsertColumn ( 0, "Supplier", LVCFMT_LEFT, 250 );
	m_listSupplier.InsertColumn ( 1, "Lines", LVCFMT_RIGHT, 50 );

	if ( FALSE == m_bDbOrders )
	{
		for ( int n = 0; n < m_arrayEntityIdx.GetSize(); n++ )
		{
			int nSpIdx = m_arrayEntityIdx.GetAt(n);

			int nListPos = m_arrayLocation.GetSize();
			m_listLocation.InsertItem ( nListPos, "" );

			CString strName = dbStockpoint.GetName( nSpIdx );
			m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
			
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetStockpointNode( nSpIdx );
			m_arrayLocation.Add( LocSelEntity );
		}
	}
	else
	{
		for ( int n = 0; n < m_arrayEntityIdx.GetSize(); n++ )
		{
			int nDbIdx = m_arrayEntityIdx.GetAt(n);

			int nListPos = m_arrayLocation.GetSize();
			m_listLocation.InsertItem ( nListPos, "" );
			m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, dbDatabase.GetName( nDbIdx ), 0, 0, 0, NULL );
		
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetDatabaseNode( nDbIdx );
			m_arrayLocation.Add( LocSelEntity );
		}
	}

	SelectLocationLine(0);
	BuildSupplierList( m_arrayEntityIdx.GetAt(0) );

	if ( m_arrayLocation.GetSize() == 1 )
	{
		CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt(0);
		
		CString strTitle;
		GetWindowText( strTitle );

		CString strExtra;
		if ( FALSE == m_bDbOrders )
			strExtra.Format( " (%s)", dbStockpoint.GetName( LocSelEntity.GetEntityIdx() ) );
		else
			strExtra.Format( " (%s)", dbDatabase.GetName( LocSelEntity.GetDbIdx() ) );
 
		SetWindowText( strTitle + strExtra );

		m_listLocation.SetCurSel(0);
		ShowAndEnableWindow( &m_listLocation, FALSE );
		MoveControl( &m_listSupplier, 7, 7 );
		MoveControl( &m_button1, 249, 7 );
		MoveControl( &m_button2, 249, 47 );
		MoveControl( &m_button3, 249, 87 );
		MoveControl( &m_button4, 249, 147 );
		MoveControl( &m_buttonCancel, 249, 278 );
		ResizeDialog( 306, 299 );
	}

	return TRUE;
}

/**********************************************************************/

void CProcessedOrderListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   switch( nIDCtl )	
   {	
   case IDC_LIST_LOCATION:
	   m_listLocation.AdjustRowHeight( lpMeasureItemStruct, 10 );
	   break;
	
   case IDC_LIST_SUPPLIER:	
	   m_listSupplier.AdjustRowHeight( lpMeasureItemStruct, 10 );	
	   break;	
   }
}

/**********************************************************************/

void CProcessedOrderListDlg::SelectLocationLine ( int nLineIdx )
{
	m_listLocation.Invalidate();

	for ( int n = 0; n < m_listLocation.GetItemCount(); n++ )
	{
		if ( n != nLineIdx )
			m_listLocation.SetItemState(n, 0, LVIS_SELECTED | LVIS_FOCUSED );
		else
		{
			m_listLocation.SetItemState(n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
			m_listLocation.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

int CProcessedOrderListDlg::GetSupplierListSize()
{
	return m_ContentHelpers.GetArraySize();
}

/**********************************************************************/

void CProcessedOrderListDlg::BuildSupplierList( int nLineIdx )
{
	m_nLocLineIdx = nLineIdx;

	CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt( m_nLocLineIdx );
	m_ContentHelpers.SetDbIdx( LocSelEntity.GetDbIdx() );
	m_ContentHelpers.SetSpIdx( m_bDbOrders ? -1 : LocSelEntity.GetEntityIdx() );

	m_ContentHelpers.BuildSupplierList();
	m_DisplayHelpers.PrepareDatabaseList( FALSE, FALSE );
}

/**********************************************************************/

void CProcessedOrderListDlg::OnGetDispInfoListSupplier(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		m_ContentHelpers.PrepareLine( nIndex, m_strDisplayLine );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderListDlg::OnDoubleClickListSupplier(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButton2();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderListDlg::OnSelectLocation(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
		{
			int nSuppIdx = GetSelectedSuppIdx();
			BuildSupplierList( nSel );
			SelectSupplier( nSuppIdx );
		}
    }
}

/**********************************************************************/

void CProcessedOrderListDlg::SelectSupplierLine ( int nIndex )
{
	m_DisplayHelpers.SelectDatabaseLine( nIndex );
}

/**********************************************************************/

int CProcessedOrderListDlg::GetSafeSupplierSelection()
{
	int nMin = 0;
	int nMax = GetSupplierListSize() - 1;
	
	int nSel = m_listSupplier.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listSupplier.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CProcessedOrderListDlg::GetOrderInfo( int nIndex, CSupplierOrderInfo& info )
{
	m_ContentHelpers.GetOrderInfo( nIndex, info );
}

/**********************************************************************/

void CProcessedOrderListDlg::SelectSupplier( int nSuppIdx )
{
	for ( int n = 0; n < GetSupplierListSize(); n++ )
	{
		CSupplierOrderInfo info;
		GetOrderInfo( n, info );

		if ( nSuppIdx == info.m_nSuppIdx )
		{
			SelectSupplierLine( n );
			return;
		}
	}

	SelectSupplierLine(0);
}

/**********************************************************************/

int CProcessedOrderListDlg::GetSelectedSuppIdx()
{
	int nSuppIdx = -1;

	int nIndex = GetSafeSupplierSelection();
	if ( nIndex != -1 )
	{
		CSupplierOrderInfo info;
		GetOrderInfo( nIndex, info );
		nSuppIdx = info.m_nSuppIdx;
	}

	return nSuppIdx;
}

/**********************************************************************/

void CProcessedOrderListDlg::UpdateOrderLines( int nSuppIdx )
{
	m_ContentHelpers.UpdateOrderLines( nSuppIdx );
}

/**********************************************************************/

bool CProcessedOrderListDlg::CanProcessOrders( int nSuppIdx, int nLineCount, CString& strError )
{
	strError = "";

	if ( nLineCount <= 0 )
	{
		switch( nSuppIdx )
		{
		case -2:
			if ( TRUE == m_bDbOrders )
				strError = "There are no orders on file for this database";
			else
				strError = "There are no orders on file for this stockpoint";
			break;

		case -1:
			strError = "There are no orders on file for the unspecified supplier";
			break;

		default:
			strError.Format( "There are no orders on file for %s", DataManager.Supplier.GetName( nSuppIdx ) );
			break;
		}

		return FALSE;
	}

	if ( DataManager.Stock.GetSize() == 0 )
	{
		strError = "You must create some stock records before you can use this option.";
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CProcessedOrderListDlg::OnButton1()
{
	HandleButton1();
}

/**********************************************************************/

void CProcessedOrderListDlg::OnButton2()
{
	HandleButton2();
}

/**********************************************************************/

void CProcessedOrderListDlg::OnButton3()
{
	HandleButton3();
}

/**********************************************************************/

void CProcessedOrderListDlg::OnButton4()
{
	int nSuppIdx = GetSelectedSuppIdx();

	if ( m_bFiltered == FALSE )
		m_bFiltered = TRUE;
	else
		m_bFiltered = FALSE;

	BuildSupplierList( m_nLocLineIdx );

	m_listSupplier.SetItemCountEx( GetSupplierListSize() );
	SelectSupplier( nSuppIdx );

	if ( m_bFiltered == FALSE )
		m_button4.SetWindowText( "&Filter" );
	else
		m_button4.SetWindowText( "&Show All" );
}

/**********************************************************************/

bool CProcessedOrderListDlg::ReadDatabaseFiles( CDataManagerInfo& infoDM )
{
	if ( DataManager.OpenDatabaseReadOnly( m_ContentHelpers.GetDbIdx(), infoDM, FALSE ) == FALSE )
		return FALSE;
	
	if ( FALSE == m_bDbOrders )
	{
		if ( DataManager.OpenStockLevels( m_ContentHelpers.GetSpIdx(), DATAMANAGER_OPEN_READWRITE, infoDM ) == FALSE )
			return FALSE;
	}
	else
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( m_ContentHelpers.GetDbIdx(), nStartIdx, nEndIdx );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, infoDM ) == FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/