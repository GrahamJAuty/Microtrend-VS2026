/**********************************************************************/
#include "Append.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "EmailOptions.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "ProcessedOrderDetails.h"
#include "ProcessedOrderDefineFilterDlg.h"
#include "ProcessedOrderEditDlg.h"
#include "ProcessedOrderNum.h"
#include "ProcessedOrderPrintDlg.h"
#include "ProcessedOrderReportOnOrderSupplier.h"
#include "ProcessedOrderSheetMaker.h"
#include "PropertySheetPrintOrder.h"
#include "PropertySheetStock.h"
#include "RepSpawn.h"
#include "Resource.h"
#include "..\CommonEcrStock\SMTPResultsDlg.h"
#include "StatusProgress.h"
#include "StockFilterDlg.h"
#include "StockReportNames.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "ProcessedOrderDatabaseDlg.h"
/**********************************************************************/

CProcessedOrderDatabaseDlg::CProcessedOrderDatabaseDlg( CProcessedOrderArray& OrderArray, int nMode, int nSuppIdx, int nDbIdx, int nSingleSpIdx, const char* szReference, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessedOrderDatabaseDlg::IDD, pParent), 
	m_StockFilterArray ( FILTERTYPE_ORDERBROWSE_NEWITEM ),
	m_OrderArray( OrderArray ),
	m_OrderFilterArray( OrderArray ),
	m_PreviousOrderFilterArray( OrderArray ),
	m_arrayLabels( TRUE ),
	m_CacheCSV ( '\t' )
{
	m_bInitDialog = FALSE;
	m_nDbIdx = nDbIdx;
	m_nSelectedSuppIdx = nSuppIdx;
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_nNewOrderNo = 0;
	
	switch( m_nSelectedSuppIdx )
	{
	case -2:
		m_nSelectedSuppNo = -1;
		break;

	case -1:
		m_nSelectedSuppNo = 0;
		break;

	default:
		m_nSelectedSuppNo = DataManager.Supplier.GetSuppNo( m_nSelectedSuppIdx );
		break;
	}

	m_strReference = szReference;
	m_bWrittenHeader = FALSE;
	m_SelectedHeaderEntity.Reset();
	
	//delete the temporary audit file
	m_strTempFile = dbDatabase.GetFolderPathData ( m_nDbIdx );
	m_strTempFile += "\\";
	m_strTempFile += DataManager.Supplier.GetTempFilename ( m_nSelectedSuppIdx );
	CFileRemover FileRemover ( m_strTempFile );

	m_strAuditFile = dbDatabase.GetFolderPathData ( m_nDbIdx );
	m_strAuditFile += "\\";
	m_strAuditFile += DataManager.Supplier.GetAuditFilename ( m_nSelectedSuppIdx );

	m_nMode = nMode;
	m_nSingleSpIdx = nSingleSpIdx;

	m_StockFilterArray.SetSupplierFilter ( m_nSelectedSuppNo );
	m_StockFilterArray.SetActiveItemOnlyFlag( TRUE );

	CArray<int,int> arraySpIdx;
	if ( m_nSingleSpIdx != -1 )
		arraySpIdx.Add( m_nSingleSpIdx );
	else
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			arraySpIdx.Add( nSpIdx );
	}
	m_StockFilterArray.SetActiveStockpointsArray( arraySpIdx );

	SetStockItemIndexForOrderLines();
	
	m_nCacheIndex = -1;

	//{{AFX_DATA_INIT(CProcessedOrderDatabaseDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::SetStockItemIndexForOrderLines()
{
	int nCount = m_OrderArray.GetItemCount();
	StatusProgress.Lock( TRUE, "Preparing order list" );
	for ( int nItemIdx = 0; nItemIdx <= nCount; nItemIdx++ )
	{
		StatusProgress.SetPos ( nItemIdx, nCount );

		CProcessedOrderItem item;
		m_OrderArray.GetItemRecord( nItemIdx, item );

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( item.GetStockCode(), nStockIdx ) == TRUE )
			item.SetStockIdx ( nStockIdx );
		else
			item.SetStockIdx ( -1 );

		m_OrderArray.SetItemRecord( nItemIdx, item );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::SaveChangedStockRecord( CStockCSVRecord& StockRecord ) 
{
	CDataManagerInfo infoDM;
	int nResult = SaveChangedStockRecordInternal( StockRecord, infoDM );

	{
		CDataManagerInfo infoTemp;
		DataManager.CloseStockOnly( infoTemp );
		DataManager.ClosePlu( infoTemp );
		DataManager.OpenPlu( DB_READONLY, infoTemp );
		DataManager.OpenStockOnly( DB_READONLY, infoTemp );
	}

	SetStockItemIndexForOrderLines();

	switch( nResult )
	{
	case 1:
		Prompter.ShareError( infoDM );
		break;

	case 2:
		Prompter.Error ( "Unable to edit this item - stock code\ndoes not exist in database" );
		break;

	case 3:
		Prompter.WriteError( infoDM );
		break;
	}
}

/**********************************************************************/

int CProcessedOrderDatabaseDlg::SaveChangedStockRecordInternal( CStockCSVRecord& StockRecord, CDataManagerInfo& infoDM ) 
{	
	{
		CDataManagerInfo infoTemp;
		DataManager.CloseStockOnly( infoTemp );
		DataManager.ClosePlu( infoTemp );
	}

	if ( DataManager.OpenPlu( DB_READWRITE, infoDM ) == FALSE )
		return 1;
	
	if ( DataManager.OpenStockOnly( DB_READWRITE, infoDM ) == FALSE )
		return 1;

	CString strStockCode = StockRecord.GetStockCode();
		
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
		return 2;

	CReportConsolidationArray<CSortedInt64Item> arrayPluNo;
	DataManager.StockPlu.MergePluNoArray( strStockCode, arrayPluNo );

	DataManager.Stock.SetAt( nStockIdx, StockRecord );

	bool bWantWritePlu = FALSE;
	if ( arrayPluNo.GetSize() > 0 )
	{
		DataManager.Plu.UpdatePrices( arrayPluNo );
		bWantWritePlu = TRUE;
	}

	if ( DataManager.WriteStock( infoDM, TRUE, FALSE ) == FALSE )
		return 3;

	if ( TRUE == bWantWritePlu )
	{
		if ( DataManager.WritePlu( infoDM ) == FALSE )	
			return 3;
	}

	return 0;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderDatabaseDlg)
	DDX_Control(pDX, IDC_BUTTON_REPLACE, m_buttonReplace);
	DDX_Control(pDX, IDC_BUTTON_UNDELIVER_ORDER, m_buttonUndeliverOrder);
	DDX_Control(pDX, IDC_BUTTON_UNDELIVER_ITEM, m_buttonUndeliverItem);
	DDX_Control(pDX, IDC_BUTTON_DELIVER_PART, m_buttonDeliverPart);
	DDX_Control(pDX, IDC_BUTTON_DELIVER_ALL, m_buttonDeliverAll);
	DDX_Control(pDX, IDC_BUTTON_DELIVER, m_buttonDeliver);
	DDX_Control(pDX, IDC_COMBO_ORDERNUM, m_comboOrderNum);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderDatabaseDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_COMMAND(IDM_EDIT_ORDER, OnEditOrder)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_COMMAND(IDM_DELETE_ORDER, OnDeleteItem)
	ON_COMMAND(IDM_ADD_ORDER, OnAddOrder)
	ON_BN_CLICKED(IDC_BUTTON_DELIVER, OnDeliverItem)
	ON_COMMAND(IDM_PRINT_ORDER, OnPrintOrder)
	ON_COMMAND(IDM_DELETE_ALLORDER, OnDeleteOrder)
	ON_COMMAND(IDM_EDIT_STOCKITEM, OnEditStockitem)
	ON_BN_CLICKED(IDC_BUTTON_DELIVER_ALL, OnDeliverOrder)
	ON_CBN_SELCHANGE(IDC_COMBO_ORDERNUM, OnSelectOrderNumber)
	ON_BN_CLICKED(IDC_BUTTON_DELIVER_PART, OnDeliverPart)
	ON_COMMAND(IDM_DELETE_ALLORDERS, OnDeleteAllOrders)
	ON_COMMAND(IDM_PRINT_PURCHASE, OnPrintPurchaseOrder)
	ON_BN_CLICKED(IDC_BUTTON_UNDELIVER_ITEM, OnButtonUndeliverItem)
	ON_BN_CLICKED(IDC_BUTTON_UNDELIVER_ORDER, OnButtonUndeliverOrder)
	ON_COMMAND(IDM_EMAIL_PURCHASE, OnEmailPurchaseOrder)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, HandleDoubleClick)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnButtonReplace)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_FILTER_ORDERS, OnFilterOrders)
	ON_COMMAND(IDM_NOFILTER_ORDERS, OnNoFilterOrders)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );
	m_listOrder.SubclassDlgItem ( IDC_LIST, this );

	TableName name;
	switch ( m_nMode )
	{
	case PROCESSMODE_DELIVER:	name = TableNames::OrderDeliver;	break;
	case PROCESSMODE_NEWORDER:	name = TableNames::OrderNew;		break;
	case PROCESSMODE_COSTPRICE:	name = TableNames::OrderCost;		break;
	case PROCESSMODE_EDIT:		
	default:					name = TableNames::OrderEdit;		break;
	}
	
	m_ColumnManager.Reset( name, 400, 1000 );
	m_ColumnManager.AddColumn( ColumnNames::OrderNo, "Order No", LVCFMT_RIGHT, 60, 60 );
	m_ColumnManager.AddColumn( ColumnNames::Date, "Date", LVCFMT_LEFT, 70, 75 );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 120, 170 );
	m_ColumnManager.AddColumn( ColumnNames::Reference, "Reference", LVCFMT_LEFT, 120, 170 );
	m_ColumnManager.AddColumn( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 55, 60 );
	m_ColumnManager.AddColumn( ColumnNames::DUQty, "DU Qty", LVCFMT_RIGHT, 50, 50 );
	m_ColumnManager.AddColumn( ColumnNames::Yield, "Yield", LVCFMT_RIGHT, 90, 90 );
	
	if ( m_nMode == PROCESSMODE_DELIVER )
		m_ColumnManager.AddColumn ( ColumnNames::Delivered, "Status", LVCFMT_LEFT, 60, 60 );

	m_ColumnManager.LoadSettings();
	
	switch ( m_nMode )
	{
	case PROCESSMODE_COSTPRICE:
		m_OrderFilterArray.SetIncludeDeliveredItemsFlag( TRUE );
		m_OrderFilterArray.SetIncludePendingItemsFlag( FALSE );
		m_PreviousOrderFilterArray.SetIncludeDeliveredItemsFlag( TRUE );
		m_PreviousOrderFilterArray.SetIncludePendingItemsFlag( FALSE );
		break;

	case PROCESSMODE_DELIVER:
	case PROCESSMODE_NEWORDER:
	case PROCESSMODE_EDIT:
	default:
		m_OrderFilterArray.SetIncludeDeliveredItemsFlag( FALSE );
		m_OrderFilterArray.SetIncludePendingItemsFlag( TRUE );
		m_PreviousOrderFilterArray.SetIncludeDeliveredItemsFlag( FALSE );
		m_PreviousOrderFilterArray.SetIncludePendingItemsFlag( TRUE );
		break;
	}

	m_OrderFilterArray.SetSuppNo( m_nSelectedSuppNo );
	m_PreviousOrderFilterArray.CopySettingsFrom( m_OrderFilterArray );

	BuildRecordList();

	m_MenuChanger.EatMenuTopLevel ( GetMenu() );

	switch ( m_nMode )
	{
	case PROCESSMODE_DELIVER:
		m_MenuChanger.KillParent ( IDM_PRINT_ORDER );
		m_MenuChanger.KillParent ( IDM_EMAIL_PURCHASE );
		m_MenuChanger.KillItem	( IDM_ADD_ORDER );
		m_MenuChanger.KillParent ( IDM_EDIT_ORDER );
		m_MenuChanger.KillParent ( IDM_DELETE_ORDER );
		break;

	case PROCESSMODE_NEWORDER:
		{
			m_MenuChanger.KillItem ( IDM_DELETE_ALLORDERS );
			m_MenuChanger.KillItem ( IDM_PRINT_ORDER );
			m_MenuChanger.KillParent( IDM_FILTER_ORDERS );
			m_MenuChanger.SetItemText ( IDM_DELETE_ALLORDER, "Entire &Order" );
			
			CProcessedOrderNum OrderNum;
			m_nNewOrderNo = OrderNum.GetOrderNum();
			OrderNum.NextOrder();
		}
		break;

	case PROCESSMODE_COSTPRICE:
		m_MenuChanger.KillItem	( IDM_ADD_ORDER );
		m_MenuChanger.SetItemText( IDM_EDIT_ORDER, "&Cost Price" );
		m_MenuChanger.KillParent( IDM_DELETE_ORDER );
		m_MenuChanger.KillItem ( IDM_PRINT_ORDER );
		m_MenuChanger.KillParent ( IDM_EMAIL_PURCHASE );
		break;

	case PROCESSMODE_EDIT:
	default:
		break;
	}

	if ( m_nSelectedSuppIdx == -1 ) m_MenuChanger.KillParent ( IDM_EMAIL_PURCHASE );		
	m_MenuChanger.BuildMenu ( GetMenu() );

	switch ( m_nMode )
	{
	case PROCESSMODE_DELIVER:
		break;

	case PROCESSMODE_NEWORDER:
	case PROCESSMODE_EDIT:
	case PROCESSMODE_COSTPRICE:
	default:
		m_buttonReplace.ShowWindow( SW_HIDE );
		m_buttonDeliver.ShowWindow ( SW_HIDE );
		m_buttonDeliverAll.ShowWindow ( SW_HIDE );
		m_buttonDeliverPart.ShowWindow ( SW_HIDE );
		m_buttonUndeliverItem.ShowWindow ( SW_HIDE );
		m_buttonUndeliverOrder.ShowWindow ( SW_HIDE );
	}
		
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listOrder, this );
	CenterWindow();
	
	switch ( m_nMode )
	{
	case PROCESSMODE_NEWORDER:
		{
			CProcessedOrderEntity HeaderEntity;
			HeaderEntity.m_nOrderNo = m_nNewOrderNo;
			m_OrderArray.AddHeader( HeaderEntity, m_nSelectedSuppNo );

			m_OrderFilterArray.SetHeaderEntity( HeaderEntity );
			m_listOrder.SetItemCountEx(0);

			m_comboOrderNum.ResetContent();
			m_comboOrderNum.AddString ( HeaderEntity.GetComboText() );
			m_comboOrderNum.SetCurSel(0);
			m_comboOrderNum.EnableWindow ( FALSE ); 
			
			m_arrayHeaderEntities.RemoveAll();
			m_arrayHeaderEntities.Add( HeaderEntity );
			m_SelectedHeaderEntity = HeaderEntity;
		}
		break;

	case PROCESSMODE_DELIVER:
	case PROCESSMODE_EDIT:
	case PROCESSMODE_COSTPRICE:
	default:
		FillOrderNumCombo();	
		break;
	}

	DisplayScreen(0);
	return TRUE; 
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listOrder.MoveWindow ( 0, 80, cx, cy - 80 );
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_nCacheIndex = nIndex;
			m_CacheCSV.RemoveAll();

			int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nIndex );
			int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nIndex );

			CProcessedOrderHeader HeaderRecord;
			m_OrderArray.GetHeaderRecord( nHeaderIdx, HeaderRecord );

			CProcessedOrderItem ItemRecord;
			m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

			CString strOrderNum;
			strOrderNum.Format ( "%6.6d", HeaderRecord.GetOrderNum() );
			m_CacheCSV.Add ( strOrderNum );
			
			m_CacheCSV.Add ( HeaderRecord.GetDisplayDate() );
			m_CacheCSV.Add ( ItemRecord.GetStockCode() );
			m_CacheCSV.Add ( ItemRecord.GetDescription() );
			m_CacheCSV.Add ( ItemRecord.GetSuppRef() );
			
			int nStockIdx = ItemRecord.GetStockIdx();

			if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );
				
				CStockLevelsCSVRecord StockLevels;
				
				if ( m_nSingleSpIdx == -1 )
					DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );
				else
					DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, dbStockpoint.GetSpNo( m_nSingleSpIdx ), StockLevels );

				m_CacheCSV.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetApparentQty() ) );
			}
			else
				m_CacheCSV.Add ( "" );

			m_CacheCSV.Add ( ItemRecord.GetDUQty() );

			CString strItems;
			strItems.Format ( "%d x %.3f", ItemRecord.GetDUItems(), ItemRecord.GetDUItemSize() );
			m_CacheCSV.Add ( strItems );

			if ( ( m_nMode == PROCESSMODE_DELIVER ) && ItemRecord.GetDeliveredFlag() == TRUE )
				m_CacheCSV.Add ( "Delivered" );
		}
		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeleteItem() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	int nListSelection = m_listOrder.GetCurSel();

	if ( ( nListSelection >= 0 ) && ( nListSelection < m_OrderFilterArray.GetSize() ) )
	{	
		if ( Prompter.YesNo ( "Are you sure you wish to remove this item from the selected order." ) == IDYES )
		{
			int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
			int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );
		
			CProcessedOrderEntity HeaderEntity;
			m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
			m_OrderArray.RemoveItem( HeaderEntity, nItemIdx );

			BuildRecordList();
			m_OrderFilterArray.ShowDebugInfo();

			m_listOrder.SetItemCountEx( m_OrderFilterArray.GetSize() );
			DisplayScreen ( nListSelection );
		}
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeleteOrder() 
{	
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	if ( m_OrderFilterArray.GetSize() == 0 )
		return;

	if ( m_OrderFilterArray.IsFilteredByDescription() == TRUE )
	{
		Prompter.Error ( "You cannot delete an entire order when the filter is in use." );
		return;
	}

	CProcessedOrderHeader HeaderRecord;
	CProcessedOrderEntity HeaderEntity;
	if ( GetSelectedHeader( HeaderRecord, HeaderEntity ) == FALSE )
	{	
		Prompter.Error ( "You must select an item from the list\nbefore you can delete its order." );
		return;
	}
		
	CString strMsg;
	strMsg.Format ( "Are you sure you wish to delete ALL items from order number %6.6d", HeaderEntity.m_nOrderNo );
	if ( Prompter.YesNo ( strMsg ) == IDNO )
		return;

	m_OrderArray.RemoveAllUndeliveredItems( HeaderEntity );

	if ( m_nMode != PROCESSMODE_NEWORDER )
	{
		BuildRecordList();
		FillOrderNumCombo();
		DisplayScreen(0);
	}
	else
	{
		BuildRecordList();
		m_listOrder.SetItemCountEx(0);
		DisplayScreen(0);
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeleteAllOrders() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	if ( m_OrderArray.GetItemCount() == 0 )
		return;

	if ( m_OrderFilterArray.IsFilteredByUser() == TRUE )
	{
		Prompter.Error ( "You cannot delete all items from all orders when the filter is in use." );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to delete ALL items from ALL orders." ) == IDNO )
		return;

	m_OrderArray.RemoveAllUndeliveredItems();
				
	m_SelectedHeaderEntity.Reset();
	BuildRecordList();
	FillOrderNumCombo();
	DisplayScreen(0);
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnEditStockitem() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nListSelection = m_listOrder.GetCurSel();
		if ( ( nListSelection >= 0 ) && ( nListSelection < m_OrderFilterArray.GetSize() ) )
		{
			int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );

			CDataManagerInfo infoDM;
			int nResult = EditStockItemInternal( nItemIdx, infoDM );

			{
				CDataManagerInfo infoTemp;
				DataManager.CloseStockOnly( infoTemp );
				DataManager.ClosePlu( infoTemp );
				DataManager.OpenPlu( DB_READONLY, infoTemp );
				DataManager.OpenStockOnly( DB_READONLY, infoTemp );
			}

			SetStockItemIndexForOrderLines();

			switch( nResult )
			{
			case 1:
				Prompter.ShareError( infoDM );
				break;

			case 2:
				Prompter.Error ( "Unable to edit this item - stock code\ndoes not exist in database" );
				break;

			case 3:
				Prompter.WriteError( infoDM );
				break;
			}
		}
	}
}

/**********************************************************************/

int CProcessedOrderDatabaseDlg::EditStockItemInternal( int nItemIdx, CDataManagerInfo& infoDM ) 
{	
	{
		CDataManagerInfo infoTemp;
		DataManager.CloseStockOnly( infoTemp );
		DataManager.ClosePlu( infoTemp );
	}

	if ( DataManager.OpenPlu( DB_READWRITE, infoDM ) == FALSE )
		return 1;
	
	if ( DataManager.OpenStockOnly( DB_READWRITE, infoDM ) == FALSE )
		return 1;
	
	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );
		
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( ItemRecord.GetStockCode(), nStockIdx ) == FALSE )
		return 2;

	CReportConsolidationArray<CSortedInt64Item> arrayPluNo;
	DataManager.StockPlu.MergePluNoArray( ItemRecord.GetStockCode(), arrayPluNo );

	CStockFilterArray FilterArray ( FILTERTYPE_DATABASE );
	FilterArray.SetArrayIdx ( nStockIdx );

	CPropertySheetStockInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pStockFilter = &FilterArray;
	info.m_bPluLinkTab = TRUE;
	info.m_bSupplierEdit = FALSE;
	info.m_bStockLevelTab = FALSE;
	info.m_nDefaultSuppNo = 0;
	info.m_bReadOnly = FALSE;

	CPropertySheetStock propSheet ( info, this );	
	if ( propSheet.DoModal() != IDOK )
	{
		propSheet.UndoChanges();
	}
	else
	{
		DataManager.StockPlu.MergePluNoArray( ItemRecord.GetStockCode(), arrayPluNo );
		DataManager.Plu.UpdatePrices( arrayPluNo );

		if ( DataManager.WriteStock( infoDM, TRUE, FALSE ) == FALSE )
			return 3;

		if ( DataManager.WritePlu( infoDM ) == FALSE )
			return 3;
	}

	return 0;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnEditOrder() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nListSelection = m_listOrder.GetCurSel();
	if ( ( nListSelection < 0 ) || ( nListSelection >= m_OrderFilterArray.GetSize() ) )
		return;

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
	int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );

	CProcessedOrderItem OrderItemOld;
	m_OrderArray.GetItemRecord( nItemIdx, OrderItemOld );

	if ( m_nMode != PROCESSMODE_COSTPRICE )
	{
		if ( OrderItemOld.GetDeliveredFlag() == TRUE )
		{
			Prompter.Error ( "You cannot edit an item that is already delivered." );
			return;
		}
	}

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( OrderItemOld.GetStockCode(), nStockIdx ) == FALSE )
	{
		Prompter.Error ( "Unable to edit this order - stock code\ndoes not exist in database" );
		return;
	}

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	OrderItemOld.ExpandStockpointOrders( nStockIdx, StockRecord );

	CProcessedOrderItem OrderItemNew;
	OrderItemNew = OrderItemOld;
		
	int nEditMode = 0;
	
	switch( m_nMode )
	{
	case PROCESSMODE_COSTPRICE:
		nEditMode = ORDER_EDIT_COST;
		break;

	default:
		nEditMode = ORDER_EDIT_NORMAL;
		break;
	}

	CProcessedOrderEditDlg dlg ( OrderItemNew, OrderItemOld, nStockIdx, StockRecord, m_nDbIdx, nEditMode, m_nSelectedSuppNo, m_nSingleSpIdx, this );		
	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.GetChangedStockRecordFlag() == TRUE )
		SaveChangedStockRecord( StockRecord );

	//MARK ORDER ARRAY AS MODIFIED
	{
		CProcessedOrderEntity HeaderEntity;
		m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
		m_OrderArray.MarkModifiedOrder( HeaderEntity );
	}
						
	//apply stock point changes
	for ( int nIndex = 0; nIndex < OrderItemNew.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = OrderItemNew.GetSpNo( nIndex );
		double dOrderOld = OrderItemOld.GetDUItemSize() * OrderItemOld.GetDUItems( nIndex );
		double dOrderNew = OrderItemNew.GetDUItemSize() * OrderItemNew.GetDUItems( nIndex );
			
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.AddOrder ( dOrderNew - dOrderOld );
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
	}

	OrderItemNew.ContractStockpointOrders();
	m_OrderArray.SetItemRecord( nItemIdx, OrderItemNew );
	DisplayScreen ( nListSelection );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnAddOrder()
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	if ( DataManager.Stock.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some stock records before you can use this option." );
		return;
	}

	CProcessedOrderHeader HeaderRecord;
	CProcessedOrderEntity HeaderEntity;
	if ( GetSelectedHeader( HeaderRecord, HeaderEntity ) == FALSE )
	{
		Prompter.Error ( "You must select an order from the list before you can add new items." );
		return;
	}

	int nSuppNo = HeaderRecord.GetSuppNo();
	
	m_StockFilterArray.SetFilterType ( FILTERTYPE_ORDERBROWSE_NEWITEM );
	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );
	dlgBrowse.SetSimpleSupplierMode();

	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	CProcessedOrderItem itemNew;
	itemNew.SetStockCode ( StockRecord.GetStockCode() );
	itemNew.SetStockIdx ( nStockIdx );
	itemNew.SetDescription ( StockRecord.GetDescription() );

	if ( m_nSingleSpIdx == -1 )
	{
		itemNew.ExpandStockpointOrders( nStockIdx, StockRecord );
		if ( itemNew.GetStockpointCount() == 0 )
			return;
	}

	m_OrderArray.MarkModifiedOrder( HeaderEntity );

	int nStockSuppIdx = -1;
	for ( int nPos = 0; nPos < StockRecord.GetSupplierCount(); nPos++ )
	{
		if ( StockRecord.GetSuppNo( nPos ) == nSuppNo )
		{
			nStockSuppIdx = nPos;
			break;
		}
	}
	
	if ( nStockSuppIdx >= 0 )
	{
		itemNew.SetDUItems	( StockRecord.GetDUItems( nStockSuppIdx ) );
		itemNew.SetDUItemSize( StockRecord.GetDUItemSize( nStockSuppIdx ) );
		itemNew.SetDUQty		( 1 );
		itemNew.SetSuppRef	( StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
		itemNew.SetCase		( StockRecord.GetCase ( nStockSuppIdx ) );
		itemNew.SetDUCost	( StockRecord.GetCost ( nStockSuppIdx ) );
		
		if ( m_nSingleSpIdx == -1 )
			itemNew.SetDUItems( 0, itemNew.GetDUItems() );
		else
			itemNew.AddOrder ( dbStockpoint.GetSpNo( m_nSingleSpIdx ), itemNew.GetDUItems() );	
	}
	else
	{
		itemNew.SetDUItems	( 1 );
		itemNew.SetDUItemSize( 1.0 );
		itemNew.SetDUQty		( 1 );
		itemNew.SetSuppRef	( StockRecord.GetSuppRefExternal( -1, DealerFlags.GetSuppRefFlag() ) );
		itemNew.SetCase		( "" );
		itemNew.SetDUCost	( 0.0 );

		if ( m_nSingleSpIdx == -1 )
			itemNew.SetDUItems( 0, 1 );
		else
			itemNew.AddOrder ( dbStockpoint.GetSpNo( m_nSingleSpIdx ), 1 );
	}

	int nSpNo = itemNew.GetSpNo(0);

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
	StockLevels.AddOrder ( itemNew.GetDUItemSize() * itemNew.GetDUItems(0) );
	DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
				
	int nItemIdx = m_OrderArray.AddItem( HeaderEntity, itemNew );

	BuildRecordList();
	m_OrderFilterArray.ShowDebugInfo();

	int nPos;
	m_OrderFilterArray.FindItemIdx( nItemIdx, nPos );

	m_listOrder.SetItemCountEx ( m_OrderFilterArray.GetSize() );
	DisplayScreen ( nPos );
				
	CProcessedOrderItem itemOld;
	itemOld = itemNew;
	CProcessedOrderEditDlg dlg ( itemNew, itemOld, nStockIdx, StockRecord, m_nDbIdx, ORDER_EDIT_NORMAL, m_nSelectedSuppNo, m_nSingleSpIdx, this );
	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.GetChangedStockRecordFlag() == TRUE )
		SaveChangedStockRecord( StockRecord );
				
	//apply stock point changes
	for ( int nIndex = 0; nIndex < itemNew.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = itemNew.GetSpNo( nIndex );
		double dSUOld = itemOld.GetDUItemSize() * itemOld.GetDUItems( nIndex );
		double dSUNew = itemNew.GetDUItemSize() * itemNew.GetDUItems( nIndex );
	
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.AddOrder ( dSUNew - dSUOld );
		DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, nSpNo, StockLevels );
	}
		
	m_OrderArray.SetItemRecord( nItemIdx, itemNew );
	m_listOrder.Invalidate();
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HandleDoubleClick();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::HandleDoubleClick()
{
	if ( m_nMode != PROCESSMODE_DELIVER )
	{
		OnEditOrder();
		return;
	}

	int nSelection = m_listOrder.GetCurSel();
	if ( ( nSelection < 0 ) || ( nSelection >= m_OrderFilterArray.GetSize() ) )
		return;

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nSelection );
	int nItemIdx = m_OrderFilterArray.GetItemIndexAt ( nSelection );

	CProcessedOrderItem item;
	m_OrderArray.GetItemRecord( nItemIdx, item );

	if ( item.GetDeliveredFlag() == FALSE )
	{
		if ( Prompter.YesNo ( "Deliver selected item", TRUE ) == IDYES )
		{
			if ( CanDeliver ( nSelection ) == FALSE )
				Prompter.Error ( "Unable to deliver this order - stock code\ndoes not exist in database" );
			else
			{
				ToggleDelivery ( nHeaderIdx, nItemIdx );
				DisplayScreen ( nSelection );
			}
		}
	}
	else
	{
		if ( Prompter.YesNo ( "Undeliver selected item", TRUE ) == IDYES )
		{
			ToggleDelivery ( nHeaderIdx, nItemIdx );
			DisplayScreen ( nSelection );
		}
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnPrintOrder() 
{
	CWordArray word;
	word.Add ( m_nSelectedSuppIdx + 1 );
	
	CProcessedOrderPrintDlg dlg ( "Print Orders", FALSE, this );
	
	if ( dlg.DoModal() != IDOK )
		return;

	CProcessedOrderReportOnOrderSupplier OrderReport ( word, m_OrderArray );

	OrderReport.SetReportType ( ORDER_ONORDER_SUPPLIER );

	if ( m_nSingleSpIdx == -1 )
	{
		OrderReport.SetConLevel ( NODE_DATABASE );
		OrderReport.SetTableNo ( dbDatabase.GetDbNo( m_nDbIdx ) );
	}
	else
	{
		OrderReport.SetConLevel ( NODE_STOCKPOINT );
		OrderReport.SetTableNo ( dbStockpoint.GetSpNo( m_nSingleSpIdx ) );
	}

	OrderReport.SetConLevelText ( dbDatabase.GetName( m_nDbIdx ) );
	OrderReport.SetDateFrom ( dlg.m_strDateFrom );
	OrderReport.SetDateTo ( dlg.m_strDateTo );
	OrderReport.SetCheckAllOrderFoldersFlag( FALSE );

	if ( OrderReport.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), CProcessedOrderBaseReport::GetReportParamsFilename( ORDER_ONORDER_SUPPLIER ), "", this );
	}
}
		
/**********************************************************************/

void CProcessedOrderDatabaseDlg::DisplayScreen ( int nIndex )
{
	CString strAction;
	switch ( m_nMode )
	{
	case PROCESSMODE_DELIVER:	strAction = "Deliver Orders";		break;
	case PROCESSMODE_NEWORDER:	strAction = "Create New Order";		break;
	case PROCESSMODE_COSTPRICE:	strAction = "Adjust Cost Prices";	break;
	case PROCESSMODE_EDIT:
	default:					strAction = "Edit Orders";			break;
	}

	CString strSupplier = "";

	switch( m_nSelectedSuppIdx )
	{
	case -2:
		strSupplier = "All Suppliers";
		break;

	case -1:
		strSupplier = "Unspecified Supplier";
		break;

	default:
		strSupplier = DataManager.Supplier.GetName( m_nSelectedSuppIdx );
		break;
	}

	CString strPlace;
	if ( m_nSingleSpIdx < 0 )
		strPlace = dbDatabase.GetName( m_nDbIdx );
	else
		strPlace = dbStockpoint.GetName( m_nSingleSpIdx );
	
	CString strFiltered = ( m_OrderFilterArray.IsFilteredByDescription() ) ? ", Filtered" : "";

	CString strTitle;

	if ( m_nMode != PROCESSMODE_NEWORDER )
	{
		strTitle.Format ( "%s for %s at %s (%d of %d lines%s)", 
			( const char* ) strAction, 
			( const char* ) strSupplier,
			( const char* ) strPlace,
			m_OrderFilterArray.GetSize(),
			m_OrderFilterArray.GetItemsMatchingOrderStatus(),
			( const char* ) strFiltered );
	}
	else
	{
		strTitle.Format ( "%s for %s at %s", 
			( const char* ) strAction, 
			( const char* ) strSupplier,
			( const char* ) strPlace );
	}

	SetWindowText ( strTitle );

	int nSize = m_OrderFilterArray.GetSize();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listOrder.SetCurSel ( nIndex );
	else
		m_listOrder.SetCurSel ( nSize - 1 );

	m_listOrder.Invalidate();
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::FillOrderNumCombo()
{
	m_comboOrderNum.ResetContent();
	m_arrayHeaderEntities.RemoveAll();
	
	for ( int nIndex = 0; nIndex < m_OrderArray.GetOrderCount(); nIndex++ )
	{
		CProcessedOrderHeader HeaderRecord;
		m_OrderArray.GetHeaderRecord( nIndex, HeaderRecord );

		if ( ( m_nSelectedSuppNo == -1 ) || ( HeaderRecord.GetSuppNo() == m_nSelectedSuppNo ) )
		{
			CProcessedOrderEntity HeaderEntity;
			m_OrderArray.GetHeaderEntity( nIndex, HeaderEntity );
			m_arrayHeaderEntities.Add( HeaderEntity );
		}
	}
		
	//ADD ENTRY FOR "ALL" IF WE HAVE NO ORDERS OR MORE THAN ONE ORDER
	if ( m_arrayHeaderEntities.GetSize() != 1 )	
	{
		CProcessedOrderEntity HeaderEntityAll;
		HeaderEntityAll.m_nOrderNo = 0;
		HeaderEntityAll.m_strDate = "";
		HeaderEntityAll.m_strTime = "";
		m_arrayHeaderEntities.InsertAt( 0, HeaderEntityAll );
	}

	for ( int nIndex = 0; nIndex < m_arrayHeaderEntities.GetSize(); nIndex++ )
	{
		CProcessedOrderEntity entityHeader = m_arrayHeaderEntities.GetAt( nIndex );
		m_comboOrderNum.AddString ( entityHeader.GetComboText() );
	}

	m_comboOrderNum.EnableWindow ( m_comboOrderNum.GetCount() >= 2  );
	
	bool bFoundHeader = FALSE;
	for ( int n = 0; n < m_arrayHeaderEntities.GetSize(); n++ )
	{
		CProcessedOrderEntity HeaderEntity = m_arrayHeaderEntities.GetAt(n);
		if ( m_SelectedHeaderEntity.Compare( HeaderEntity ) == 0 )
		{
			m_comboOrderNum.SetCurSel(n);
			bFoundHeader = TRUE;
			break;
		}
	}

	if ( FALSE == bFoundHeader )
	{
		m_comboOrderNum.SetCurSel(0);
		m_SelectedHeaderEntity = m_arrayHeaderEntities.GetAt(0);
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::UpdateDeleteMenu()
{
	bool bFiltered = m_OrderFilterArray.IsFilteredByUser();
	bool bFilteredByDescription = m_OrderFilterArray.IsFilteredByDescription();

	m_MenuChanger.ShowItem ( IDM_DELETE_ALLORDER, ( bFilteredByDescription == FALSE ) );
	m_MenuChanger.ShowItem ( IDM_DELETE_ALLORDERS, ( bFiltered == FALSE ) );
	m_MenuChanger.BuildMenu ( GetMenu() );
	m_buttonDeliverAll.EnableWindow( bFilteredByDescription == FALSE );
	m_buttonUndeliverOrder.EnableWindow( bFilteredByDescription == FALSE );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnSelectOrderNumber() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nSelection = m_comboOrderNum.GetCurSel();
	if ( nSelection <= -1 )
		return;

	CProcessedOrderEntity HeaderEntityNew = m_arrayHeaderEntities.GetAt( nSelection );
		
	if ( HeaderEntityNew.Compare( m_SelectedHeaderEntity ) != 0 )
	{
		m_SelectedHeaderEntity = HeaderEntityNew;
		BuildRecordList();
		DisplayScreen ( 0 );
		UpdateDeleteMenu();
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::BuildRecordList()
{
	StatusProgress.Lock( TRUE, "Filtering orders" );
	m_OrderFilterArray.SetHeaderEntity( m_SelectedHeaderEntity );
	m_OrderFilterArray.BuildList();
	m_listOrder.SetItemCountEx ( m_OrderFilterArray.GetSize() );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnPrintPurchaseOrder() 
{
	CreatePurchaseOrder ( FALSE );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnEmailPurchaseOrder() 
{
	CreatePurchaseOrder ( TRUE );
}

/**********************************************************************/

bool CProcessedOrderDatabaseDlg::GetSelectedHeader( CProcessedOrderHeader& HeaderRecord, CProcessedOrderEntity& HeaderEntity )
{
	bool bResult = FALSE;

	if ( PROCESSMODE_NEWORDER == m_nMode )
	{
		HeaderEntity = m_SelectedHeaderEntity;
		bResult = TRUE;
	}
	else if ( m_OrderFilterArray.IsFilteredByHeaderEntity() == TRUE )
	{
		HeaderEntity = m_SelectedHeaderEntity;
		bResult = TRUE;
	}
	else 
	{
		int nListSelection = m_listOrder.GetCurSel();	
		if ( ( nListSelection >= 0 ) && ( nListSelection < m_OrderFilterArray.GetSize() ) )
		{
			int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
			m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
			bResult = TRUE;
		}
	}

	if ( TRUE == bResult )
	{
		int nPos;
		if ( m_OrderArray.FindHeader( HeaderEntity, nPos ) == TRUE )
			m_OrderArray.GetHeaderRecord( nPos, HeaderRecord );
		else
			bResult = FALSE;
	}

	return bResult;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::CreatePurchaseOrder( bool bEmail ) 
{	
	CProcessedOrderHeader HeaderRecord;
	CProcessedOrderEntity HeaderEntity;
	if ( GetSelectedHeader( HeaderRecord, HeaderEntity ) == FALSE )
	{	
		Prompter.Error ( "You must select an item from the list\nbefore you can create a purchase order." );
		return;
	}
	
	CProcessedOrder Order;
	m_OrderArray.GetOrder( HeaderEntity, Order );

	CProcessedOrderDetails OrderDetails ( bEmail );
	CPropertySheetPrintOrder propSheet( bEmail, OrderDetails, m_nSingleSpIdx, this );
		
	if ( propSheet.DoModal() == IDCANCEL )
		return;
	
	CProcessedOrderSheetMaker OrderSheetMaker;
	OrderSheetMaker.SetDate();
	OrderSheetMaker.SetDeliverAsOrderedFlag ( OrderDetails.m_bDeliverAsOrdered );
	OrderSheetMaker.SetTopAddress1 ( OrderDetails.m_strTopAddress1 );
	OrderSheetMaker.SetTopAddress2 ( OrderDetails.m_strTopAddress2 );
	OrderSheetMaker.SetTopAddress3 ( OrderDetails.m_strTopAddress3 );
	OrderSheetMaker.SetTopPostcode ( OrderDetails.m_strTopPostcode );
	OrderSheetMaker.SetTopFax ( OrderDetails.m_strTopFax );
	OrderSheetMaker.SetTopName ( OrderDetails.m_strTopName );
	OrderSheetMaker.SetTopPhone ( OrderDetails.m_strTopPhone );
	OrderSheetMaker.SetName ( OrderDetails.m_strName );
	OrderSheetMaker.SetAddress1 ( OrderDetails.m_strAddress1 );
	OrderSheetMaker.SetAddress2 ( OrderDetails.m_strAddress2 );
	OrderSheetMaker.SetAddress3 ( OrderDetails.m_strAddress3 );
	OrderSheetMaker.SetPostcode ( OrderDetails.m_strPostcode );
	OrderSheetMaker.SetInstructions1 ( OrderDetails.m_strInstructions1 );
	OrderSheetMaker.SetInstructions2 ( OrderDetails.m_strInstructions2 );
	OrderSheetMaker.SetInstructions3 ( OrderDetails.m_strInstructions3 );
	OrderSheetMaker.SetInstructions4 ( OrderDetails.m_strInstructions4 );
	OrderSheetMaker.SetInstructions5 ( OrderDetails.m_strInstructions5 );

	if ( Order.GetItemCount() > 0 )
	{
		if ( bEmail == FALSE )
		{
			OrderSheetMaker.CreateOrderSheet ( Order, FALSE );

			if ( OrderSheetMaker.GetRepmanWarningFlag() == TRUE )
			{
				CReportManager ReportManager;
				ReportManager.ReportManagerError();
			}
		}
		else
		{
			if (EmailOptions.GetEmailMethodToUse() == 0)
			{
				OrderSheetMaker.EmailOrderSheetMAPI(Order, FALSE, FALSE);
			}
			else
			{
				CSMTPEmailHelpers SMTPEmailHelpers;
				SMTPEmailHelpers.OpenSession(3);
	
				OrderSheetMaker.EmailOrderSheetSMTP ( SMTPEmailHelpers, Order );

				if ( SMTPEmailHelpers.GetSessionSize() != 0 )
				{
					SMTPEmailHelpers.CreateIndexFile();
					CSMTPResultsDlg dlg( 3, SMTPEmailHelpers, this );
					dlg.DoModal();
					SMTPEmailHelpers.DeleteSession();
				}
			}
		}
	}

	if ( bEmail == TRUE )
		BringWindowToTop();
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnFilterOrders() 
{
	CProcessedOrderDefineFilterDlg dlg ( m_PreviousOrderFilterArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_OrderFilterArray.CopySettingsFrom ( m_PreviousOrderFilterArray );
		BuildRecordList();
		DisplayScreen ( 0 );
		UpdateDeleteMenu();
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnNoFilterOrders() 
{
	int nItemIdx = 0;
	int nSelection = m_listOrder.GetCurSel();

	if ( nSelection >= 0 && nSelection < m_OrderFilterArray.GetSize() )
		nItemIdx = m_OrderFilterArray.GetItemIndexAt ( nSelection );

	m_OrderFilterArray.SetDescriptionFilter ( "", 0, FALSE );
	BuildRecordList();
	DisplayScreen(nItemIdx);
	UpdateDeleteMenu();	
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CProcessedOrderDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	
	switch ( m_ColumnManager.GetListColumnType( nIndex ) )
	{
	case COLUMN_STOCKCODE:
	case COLUMN_DESC:
	case COLUMN_REFERENCE:
		OnFilterOrders();
		break;
	}
	
	return 0L;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnCancel()
{
	bool bPrompt = ( DataManager.Stock.IsModified() || DataManager.Plu.IsModified() );
	
	switch ( m_nMode )
	{
	case PROCESSMODE_NEWORDER:	
		bPrompt |= ( m_OrderFilterArray.GetSize() > 0 ); 
		break;
	
	case PROCESSMODE_DELIVER:
	case PROCESSMODE_EDIT:
	case PROCESSMODE_COSTPRICE:
	default:					
		bPrompt |= m_OrderArray.IsModified();
		break;
	}

	if ( bPrompt == FALSE )
	{
		EndDialog ( IDOK );
		return;
	}

	int nChoice = Prompter.YesNoCancel ( "Stock database has been modified - save changes" );	
	if ( nChoice == IDCANCEL )
		return;

	if ( nChoice == IDNO )
	{
		EndDialog ( IDCANCEL );
		return;
	}
					
	if ( m_nMode == PROCESSMODE_DELIVER )
	{
		AuditDeliveries();

		if ( m_arrayLabels.GetSize() > 0 )
		{
			bool bWrittenLabels = FALSE;
			
			CDataManagerInfo infoDM;
			DataManager.CloseProductLabels( infoDM );
			if ( DataManager.OpenProductLabels( DB_READWRITE, infoDM ) == TRUE )
			{
				DataManager.ProductLabels.MergeLabels( m_arrayLabels );
				bWrittenLabels = DataManager.WriteProductLabels( infoDM );
				DataManager.CloseProductLabels( infoDM );
			}

			if ( FALSE == bWrittenLabels )
			{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();

				CString strFilename = DataManager.GetFilePathProductLabels( DataManager.GetActiveDbIdx() );
				
				CString strPendingFilename;
				strPendingFilename.Format( "%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.dat",
					strFilename.Left( strFilename.GetLength() - 4 ),
					timeNow.GetYear(),
					timeNow.GetMonth(),
					timeNow.GetDay(),
					timeNow.GetHour(),
					timeNow.GetMinute(),
					timeNow.GetSecond() );

				m_arrayLabels.Write( strPendingFilename );
			}
		}
	}

	m_OrderArray.SaveOrders();
		
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, FALSE, TRUE ) == FALSE )
		Prompter.WriteError( info );

	CCustomExportManagerPlu CustomExport;
	::Append ( m_strTempFile, m_strAuditFile );
	EndDialog ( IDOK );
}

/**********************************************************************/
