/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "ProcessedOrderQtyDlg.h"
#include "PropertySheetOrderSupplier.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ProcessedOrderPreviewEditDlg.h"
/**********************************************************************/

CProcessedOrderPreviewEditDlg::CProcessedOrderPreviewEditDlg( CStockCSVRecord& StockRecord, CProcessedOrderPreviewCSVRecord& OrderRecord, CLocationSelectorMiniLookup& LocSelMiniLookup, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessedOrderPreviewEditDlg::IDD, pParent), m_StockRecord ( StockRecord ), m_OrderRecord ( OrderRecord ), m_LocSelMiniLookup( LocSelMiniLookup )
{
	//{{AFX_DATA_INIT(CProcessedOrderPreviewEditDlg)
	//}}AFX_DATA_INIT
	m_nSupplierCount = m_StockRecord.GetSupplierCount();
	m_nStockSuppIdx = m_OrderRecord.GetStockSuppIdx();
	
	if ( m_nSupplierCount > MAX_ITEM_SUPPLIERS ) m_nSupplierCount = MAX_ITEM_SUPPLIERS;
	if ( m_nStockSuppIdx >= m_nSupplierCount ) m_nStockSuppIdx = 0;

	if ( m_nSupplierCount <= 0 )
	{
		m_OrderPreviewRecordArray [ 0 ] = OrderRecord;
		m_OrderPreviewRecordArray [ 0 ].SetStockSuppIdx ( -1 );
		m_OrderPreviewRecordArray [ 0 ].UpdateSupplierCosts( m_LocSelMiniLookup );
		m_nSupplierCount = 0;
		m_nStockSuppIdx = 0;
	}
	else
	{
		for ( int nIndex = 0; nIndex < m_nSupplierCount; nIndex++ ) 
		{
			m_OrderPreviewRecordArray [ nIndex ] = OrderRecord;	
			m_OrderPreviewRecordArray [ nIndex ].SetStockSuppIdx ( nIndex );
			m_OrderPreviewRecordArray [ nIndex ].UpdateSupplierCosts( m_LocSelMiniLookup );
		}
	}
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderPreviewEditDlg)
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_EDIT_REFERENCE, m_editReference);
	DDX_Control(pDX, IDC_EDIT_DUITEMSIZE, m_editDUItemSize);
	DDX_Control(pDX, IDC_EDIT_DUITEMS, m_editDUItems);
	DDX_Control(pDX, IDC_EDIT_DELUNIT, m_editDelUnit);
	DDX_Control(pDX, IDC_EDIT_COST, m_editCost);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SUPPLIER, m_buttonEditSupplier);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderPreviewEditDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderPreviewEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SUPPLIER, OnButtonEditSupplier)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SUPPLIERS, OnSelectSupplier)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUPPLIERS, OnDoubleClickSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderPreviewEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_listStockpoint.SubclassDlgItem ( IDC_LIST, this );
	m_listSuppliers.SubclassDlgItem ( IDC_LIST_SUPPLIERS, this );

	m_listStockpoint.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 140 );
	m_listStockpoint.InsertColumn ( 1, "Apparent", LVCFMT_RIGHT, 75 );
	m_listStockpoint.InsertColumn ( 2, "Required", LVCFMT_RIGHT, 75 );
	m_listStockpoint.InsertColumn ( 3, "Items Assigned", LVCFMT_RIGHT, 90 );
	
	m_listSuppliers.InsertColumn ( 0, "Name", LVCFMT_LEFT, 140 );
	m_listSuppliers.InsertColumn ( 1, "DU Required", LVCFMT_RIGHT, 75 );
	m_listSuppliers.InsertColumn ( 2, "Item Total", LVCFMT_RIGHT, 75 );
	m_listSuppliers.InsertColumn ( 3, "SU Total", LVCFMT_RIGHT, 90 );
	m_listSuppliers.InsertColumn ( 4, "Cost", LVCFMT_RIGHT, 90 );
	m_listSuppliers.InsertColumn ( 5, "SU Cost", LVCFMT_RIGHT, 90 );
	
	AddStockpointList();
	AddSupplierList();

	if ( m_listStockpoint.GetItemCount() > 0 )
		m_listStockpoint.SetCurSel(0);
	
	if ( m_StockRecord.GetSupplierCount() != 0 )
	{
		m_listSuppliers.SetCurSel ( m_nStockSuppIdx );
		SelectSupplier ( m_nStockSuppIdx );
	}
	else
	{
		m_listSuppliers.SetCurSel ( 0 );
		m_editSupplier.SetWindowText ( "Unspecified" );
		m_editDelUnit.SetWindowText ( "" );
		m_editReference.SetWindowText( "" );
		m_editDUItems.SetWindowText ( "1" );
		m_editDUItemSize.SetWindowText ( "1.000" );

		CString strLabel;
		strLabel.Format ( "%.*f", SysInfo.GetDPValue(), 0.0 );
		m_editCost.SetWindowText ( strLabel );

		m_buttonEditSupplier.ShowWindow ( SW_HIDE );
	}

	CString strText; 
	strText.Format ( "Edit Pending Order %s %s",
		m_StockRecord.GetStockCode(),
		m_StockRecord.GetDescription() );
	
	SetWindowText ( strText );
	UpdateData ( FALSE );
	
	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnButtonEdit() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	int nSel = m_listStockpoint.GetCurSel();
	int nCount = m_listStockpoint.GetItemCount();
	if ( nCount == 1 ) nCount = 2;

	if ( ( nSel < 0 ) || ( nSel >= nCount - 1 ) )
		return;

	int nDUItems = m_OrderPreviewRecordArray[ m_nStockSuppIdx ].GetDUItems();
	double dDUItemSize = m_OrderPreviewRecordArray[ m_nStockSuppIdx ].GetDUItemSize();

	int nSpIdx = m_listStockpoint.GetItemData( nSel );
	int nType = ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE ) ? 2 : 1;
	CProcessedOrderQtyDlg dlg ( nSpIdx, nType, nDUItems, dDUItemSize, this );
	dlg.SetDUItemOrder ( m_OrderPreviewRecordArray[ m_nStockSuppIdx ].CalculateDUItemRequired ( nSpIdx ) );
		
	if ( dlg.DoModal() != IDOK )
		return;
		
	int nListCount = m_nSupplierCount;
	if ( nListCount < 1 ) nListCount = 1;

	for ( int nIndex = 0; nIndex < nListCount; nIndex++ )
	{
		switch ( dlg.m_nType )
		{
		case 0:
			m_OrderPreviewRecordArray[ nIndex ].SetSURequired ( nSpIdx, dlg.m_dStockUnits ); 
			break;
			
		case 1:
			m_OrderPreviewRecordArray[ nIndex ].SetSURequired ( nSpIdx, dDUItemSize * dlg.m_nSupplierItems ); 
			break;

		case 2:
			m_OrderPreviewRecordArray[ nIndex ].SetSURequired ( nSpIdx, dDUItemSize * nDUItems * dlg.m_nDelUnits ); 
			break;
		}

		m_OrderPreviewRecordArray[ nIndex ].UpdateSupplierCosts( m_LocSelMiniLookup );
	}

	AddStockpointList();
	AddSupplierList();
	m_listStockpoint.SetCurSel ( nSel );
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();	
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnButtonEditSupplier() 
{
	int nStockIdx = m_OrderPreviewRecordArray[0].GetStockIdx();
	
	if ( m_nStockSuppIdx < 0 || m_nStockSuppIdx >= m_nSupplierCount )
		return;

	CPropertySheetOrderSupplier propSheet( nStockIdx, m_nStockSuppIdx, this );
	if ( propSheet.DoModal() != IDOK )
	{
		if ( propSheet.GetModifiedFlag() )
			propSheet.UndoChanges();
	
		return;
	}

	DataManager.Stock.GetAt( nStockIdx, m_StockRecord );

	for ( int nIdx = 0; nIdx < MAX_ITEM_SUPPLIERS; nIdx++ )
		m_OrderPreviewRecordArray[nIdx].UpdateSupplierCosts( m_LocSelMiniLookup );
	
	if ( propSheet.GetMakePreferredFlag() )
		m_nStockSuppIdx = 0;

	m_listSuppliers.SetCurSel ( m_nStockSuppIdx );
	AddSupplierList();
	SelectSupplier ( m_nStockSuppIdx );
	AddStockpointList();
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnDoubleClickSupplier(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnSelectSupplier( pNMHDR, pResult );
	OnButtonEditSupplier();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnSelectSupplier(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nStockSuppIdx = m_listSuppliers.GetCurSel();
		
	if ( nStockSuppIdx < 0 || nStockSuppIdx >= m_nSupplierCount )
		return;

	if ( nStockSuppIdx == m_nStockSuppIdx )
		return;

	m_nStockSuppIdx = nStockSuppIdx;
	SelectSupplier ( nStockSuppIdx );	
	
	AddStockpointList();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::SelectSupplier ( int nStockSuppIdx )
{
	int nSuppIdx = m_OrderPreviewRecordArray [ nStockSuppIdx ].GetSupplierIdx();
	m_editSupplier.SetWindowText ( DataManager.Supplier.GetName ( nSuppIdx ) );
	m_editReference.SetWindowText ( m_StockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
	m_editDelUnit.SetWindowText( m_StockRecord.GetCase( nStockSuppIdx ) );

	CString strLabel;
	strLabel.Format ( "%d", m_StockRecord.GetDUItems( nStockSuppIdx ) );
	m_editDUItems.SetWindowText( strLabel );

	strLabel.Format ( "%.3f", m_StockRecord.GetDUItemSize( nStockSuppIdx ) );
	m_editDUItemSize.SetWindowText ( strLabel );

	strLabel.Format ( "%.*f", SysInfo.GetDPQty(), m_StockRecord.GetCost( nStockSuppIdx ) );
	m_editCost.SetWindowText( strLabel );
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON_EDIT );
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::AddStockpointList()
{
	int nDbIdx = DataManager.GetActiveDbIdx();
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	int nOldSelect = m_listStockpoint.GetCurSel();

	m_listStockpoint.DeleteAllItems();
	
	double dTotalAssigned = 0;
	double dTotalAssignedOld = 0;
	double dTotalSUOrder = 0;
	double dTotalApparent = 0.0;
	int nTotalDUItemsRequired = 0;
		
	int nSpCount = 0;
	CString strOrder;

	double dDUItemSize = m_OrderPreviewRecordArray [ m_nStockSuppIdx ].GetDUItemSize();

	CArray<int,int> arraySpIdx;
	for ( int nSpIdx = 0; nSpIdx < m_OrderPreviewRecordArray [ m_nStockSuppIdx ].GetStockpointCount(); nSpIdx++ )
	{
		if ( CProcessedOrderPreviewCSVArray::IsStockpointIncluded( nSpIdx, m_LocSelMiniLookup, FALSE ) == TRUE )
			arraySpIdx.Add( nSpIdx );
	}

	//if ( EcrmanOptions.GetSortLocNameFlag() == TRUE )
		dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);
	
		CCSV csv ( '\t' );

		double dAssigned =		m_OrderPreviewRecordArray [ m_nStockSuppIdx ].GetSURequired( nSpIdx );
		int nDUItemsRequired =	m_OrderPreviewRecordArray [ m_nStockSuppIdx ].CalculateDUItemRequired ( nSpIdx );
		
		CString strItems = "0";
		if ( nDUItemsRequired != 0 )
			strItems.Format ( "%d x %.3f", nDUItemsRequired, dDUItemSize );
			
		csv.Add ( dbStockpoint.GetName( nSpIdx ) );

		CStockLevelsCSVRecord StockLevels;
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
		DataManager.Stock.GetStockpointTotals( -1, m_StockRecord, nSpNo, StockLevels );
		
		if ( StockLevels.GetActiveFlag() == FALSE )
			continue;
		
		double dApparent = StockLevels.GetApparentQty();
			
		csv.Add ( dApparent, 3 );
		csv.Add ( dAssigned, 3 );
		csv.Add ( strItems );
			
		dTotalAssigned += dAssigned;
		dTotalApparent += dApparent;
		nTotalDUItemsRequired += nDUItemsRequired;

		int nAddPos = m_listStockpoint.AddString ( csv.GetLine() );
		m_listStockpoint.SetItemData ( nAddPos, nSpIdx );
		nSpCount++;
	}

	if ( nSpCount >= 2 )
	{
		CCSV csv ( '\t' );
		csv.Add ( "Total" );

		csv.Add ( dTotalApparent, 3 );
		csv.Add ( dTotalAssigned, 3 );

		CString strItems = "0";
		if ( nTotalDUItemsRequired != 0 )
			strItems.Format ( "%d x %.3f", nTotalDUItemsRequired, dDUItemSize );	
		
		csv.Add ( strItems );
			
		m_listStockpoint.AddString ( csv.GetLine() );
	}

	m_listStockpoint.SetCurSel(nOldSelect);
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::AddSupplierList()
{
	int nOldSelect = m_listSuppliers.GetCurSel();

	m_listSuppliers.DeleteAllItems();

	int nListSize = m_nSupplierCount;
	if ( nListSize < 1 ) nListSize = 1;

	for ( int nIndex = 0; nIndex < nListSize; nIndex++ )
	{
		int nSuppIdx = m_OrderPreviewRecordArray [ nIndex ].GetSupplierIdx();
		
		CStockReportLine csv;
		
		if ( nSuppIdx >= 0 )
			csv.AppendString ( DataManager.Supplier.GetName ( nSuppIdx ) );
		else
			csv.AppendString ( "Unspecified" );

		CProcessedOrderPreviewCSVRecord* pRecord = &m_OrderPreviewRecordArray[ nIndex ];

		csv.AppendInt ( pRecord -> GetDURequired() );
		csv.AppendInt ( pRecord -> GetDURequired() * pRecord -> GetDUItems() );
		csv.AppendStockQty ( pRecord -> GetSUTotal(), 1, TRUE, 3 );
		csv.AppendVal ( pRecord -> GetTotalCost() );
		csv.AppendVal ( pRecord -> GetSUCost() );
		m_listSuppliers.AddString ( csv.GetLine() );
	}

	m_listSuppliers.SetCurSel ( nOldSelect );
}

/**********************************************************************/

void CProcessedOrderPreviewEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nStockSuppIdx = m_listSuppliers.GetCurSel();
		if ( nStockSuppIdx >= 0 && nStockSuppIdx < m_listSuppliers.GetItemCount() )
			m_OrderRecord = m_OrderPreviewRecordArray [ nStockSuppIdx ];
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

