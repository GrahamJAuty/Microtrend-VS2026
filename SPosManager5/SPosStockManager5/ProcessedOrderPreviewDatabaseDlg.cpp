/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "Password.h"
#include "ProcessedOrderAutoPrintDlg.h"
#include "ProcessedOrderAutoPrintBasicDlg.h"
#include "ProcessedOrderPreviewEditDlg.h"
#include "RepSpawn.h"
#include "SelectStockpointSurplusDlg.h"
#include "StatusProgress.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockRangeDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ProcessedOrderPreviewDatabaseDlg.h"
/**********************************************************************/
static const int TOOLBAR_SPACE = 28;
/**********************************************************************/

CProcessedOrderPreviewDatabaseDlg::CProcessedOrderPreviewDatabaseDlg( const char* szTitle, CProcessedOrderPreviewCSVArray& OrderArray, CLocationSelectorMiniLookup& LocSelMiniLookup, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessedOrderPreviewDatabaseDlg::IDD, pParent), 
	m_OrderArray ( OrderArray ), 
	m_StockFilterArray ( FILTERTYPE_ORDERBROWSE_PREVIEW ),
	m_CacheCSV ( '\t' ),
	m_LocSelMiniLookup( LocSelMiniLookup )
{
	//{{AFX_DATA_INIT(CProcessedOrderPreviewDatabaseDlg)
	//}}AFX_DATA_INIT
	m_nCacheIndex = -1;

	m_bInitDialog = FALSE;
	m_strTitle = szTitle;
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );

	m_strPreviousOrderFilename = "";
	m_strPreviousOrderReference = "";
	m_bAllowSaveTemplate = TRUE;

	m_StockpointArray.RemoveAll();
	m_nDefaultSpIdx = -1;

	int nSpStartIdx, nSpEndIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nSpStartIdx, nSpEndIdx );
	
	for ( int nSpIdx = nSpStartIdx; nSpIdx <= nSpEndIdx; nSpIdx++ )
	{
		bool bAddToArray = FALSE;
		
		if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == TRUE )
		{
			//FIND OUT IF THIS STOCKPOINT IS INCLUDED IN THE ORDER
			switch ( m_LocSelMiniLookup.GetConType() )
			{
			case NODE_STOCKPOINT:		
				if ( dbStockpoint.GetSpNo( nSpIdx ) == m_LocSelMiniLookup.GetEntityNo() )
					bAddToArray = TRUE;
				break;
				
			case NODE_SITE:			
				if ( dbStockpoint.GetSiteNo( nSpIdx ) == m_LocSelMiniLookup.GetEntityNo() )	
					bAddToArray = TRUE;
				break;
				
			case NODE_DATABASE:		
				bAddToArray = TRUE;	
				break;
			}

			if ( TRUE == bAddToArray )
				if ( -1 == m_nDefaultSpIdx )
					m_nDefaultSpIdx = nSpIdx;

			if ( StockOptions.GetOrderRestrictPreviewFlag() == FALSE )
				bAddToArray = TRUE;
		}

		if ( TRUE == bAddToArray ) 
			m_StockpointArray.Add ( nSpIdx );
	}

	//FALL BACK TO FIRST SP IN DATABASE
	if ( -1 == m_nDefaultSpIdx )
	{
		if ( nSpStartIdx <= nSpEndIdx )
			m_nDefaultSpIdx = nSpStartIdx;
	}

	m_StockFilterArray.SetOrderPreviewTableNo ( m_LocSelMiniLookup.GetEntityNo() );
	m_StockFilterArray.SetActiveItemOnlyFlag( TRUE );
	m_StockFilterArray.SetActiveStockpointsArray( m_StockpointArray );
		
	if ( StockOptions.GetOrderRestrictPreviewFlag() == TRUE )
		m_StockFilterArray.SetOrderPreviewConLevel ( m_LocSelMiniLookup.GetConType() );
	else
		m_StockFilterArray.SetOrderPreviewConLevel ( NODE_DATABASE );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::SetPreviousOrderDetails( const char* szFilename, const char* szReference, bool bAllowSaveTemplate )
{
	m_strPreviousOrderFilename = szFilename;
	m_strPreviousOrderReference = szReference;
	m_bAllowSaveTemplate = bAllowSaveTemplate;
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderPreviewDatabaseDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderPreviewDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderPreviewDatabaseDlg)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_WM_SIZE()
	ON_COMMAND(IDM_DELETE_CURRENT, OnDeleteCurrent)
	ON_COMMAND(IDM_DELETE_RANGE, OnDeleteRange)
	ON_BN_CLICKED(IDC_BUTTON_ABANDON, OnButtonAbandon)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnButtonContinue)
	ON_COMMAND(IDM_EDIT_ORDER, OnEditOrder)
	ON_COMMAND(IDM_ADD_ORDER, OnAddOrder)
	ON_COMMAND(IDM_PRINT_ORDER, OnPrintOrder)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnEditOrder)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderPreviewDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );
	m_listOrder.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::OrderPreview );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 130, 160 );
	m_ColumnManager.AddColumn( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn( ColumnNames::Supplier, "Supplier", LVCFMT_LEFT, 100, 140 );
	m_ColumnManager.AddColumn( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 60, 70 );
	m_ColumnManager.AddColumn( ColumnNames::DUReq, "DU Req", LVCFMT_RIGHT, 60, 60 );
	m_ColumnManager.AddColumn( ColumnNames::TotalCost, "Total Cost", LVCFMT_RIGHT, 60, 60 );
	m_ColumnManager.LoadSettings();
	
	m_listOrder.SetItemCountEx ( m_OrderArray.GetSize() );
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listOrder, this );
	CenterWindow();

	DisplayScreen(0);
	SetWindowText ( m_strTitle );
	return TRUE; 
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_nCacheIndex = nIndex;
			m_CacheCSV.RemoveAll();

			CProcessedOrderPreviewCSVRecord OrderRecord;
			CStockCSVRecord StockRecord;

			m_OrderArray.GetAt ( nIndex, OrderRecord );
			DataManager.Stock.GetAt ( OrderRecord.GetStockIdx(), StockRecord );

			m_CacheCSV.Add ( StockRecord.GetStockCode() );
			m_CacheCSV.Add ( StockRecord.GetDescription() );
			m_CacheCSV.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );

			int nSuppIdx = OrderRecord.GetSupplierIdx();

			if ( nSuppIdx < 0 )
				m_CacheCSV.Add ( "Unspecified" );
			else
				m_CacheCSV.Add ( DataManager.Supplier.GetName ( nSuppIdx ) );

			m_CacheCSV.Add ( GetApparentStockString( OrderRecord, StockRecord ) );
			m_CacheCSV.Add ( OrderRecord.GetDURequired() );
			m_CacheCSV.Add ( OrderRecord.GetTotalCost(), SysInfo.GetDPValue() );
		}
		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}
	
/**********************************************************************/

const char* CProcessedOrderPreviewDatabaseDlg::GetApparentStockString( CProcessedOrderPreviewCSVRecord& OrderRecord, CStockCSVRecord& StockRecord )
{
	CStockLevelsCSVRecord StockLevels;

	if ( ( StockOptions.GetOrderRestrictPreviewFlag() == FALSE ) && ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC ) )
		DataManager.Stock.GetDatabaseTotals( OrderRecord.GetStockIdx(), StockRecord, StockLevels );
	else
	{
		switch ( m_LocSelMiniLookup.GetConType() )
		{
		case NODE_STOCKPOINT:
			DataManager.Stock.GetStockpointTotals( OrderRecord.GetStockIdx(), StockRecord, m_LocSelMiniLookup.GetEntityNo(), StockLevels );
			break;

		case NODE_SITE:
			DataManager.Stock.GetSiteTotals( OrderRecord.GetStockIdx(), StockRecord, m_LocSelMiniLookup.GetEntityNo(), StockLevels );
			break;

		case NODE_DATABASE:
		default:
			DataManager.Stock.GetDatabaseTotals( OrderRecord.GetStockIdx(), StockRecord, StockLevels );
			break;
		}
	}

	m_strApparentStock = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetApparentQty() );
	return m_strApparentStock;
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditOrder();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_OrderArray.GetSize();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listOrder.SetCurSel ( nIndex );
	else
		m_listOrder.SetCurSel ( nSize - 1 );

	m_listOrder.Invalidate();
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listOrder.MoveWindow ( 0, TOOLBAR_SPACE, cx, cy - TOOLBAR_SPACE );
	}
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnDeleteCurrent() 
{
	int nCurrent = m_listOrder.GetCurSel();

	if ( nCurrent < 0 || nCurrent >= m_OrderArray.GetSize() )
		return;

	if ( Prompter.YesNo ( "Delete current selection", TRUE ) == IDNO )
		return;

	m_OrderArray.RemoveAt ( nCurrent );
	m_listOrder.SetItemCountEx ( m_OrderArray.GetSize() );
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnDeleteRange() 
{
	if ( m_OrderArray.GetSize() == 0 )
		return;

	int nCurrent = m_listOrder.GetCurSel();
	const char* szTitle = "Range Delete";
	
	CStockRangeDlg dlg ( szTitle, this );

	if ( nCurrent >= 0 && nCurrent < m_OrderArray.GetSize() )
	{
		CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
		m_OrderArray.GetAt ( nCurrent, OrderPreviewRecord );
		CString strStockCode = DataManager.Stock.GetStockCode ( OrderPreviewRecord.GetStockIdx () ); 
		dlg.m_strStockCodeFrom = strStockCode; 
		dlg.m_strStockCodeTo = strStockCode;
	}
	else
	{
		dlg.m_strStockCodeFrom = "";
		dlg.m_strStockCodeTo = "";
	}
		
	if ( dlg.DoModal() == IDOK )
	{
		int nStart;
		int nEnd;
		
		DataManager.Stock.FindStockItemByCode ( dlg.m_strStockCodeFrom, nStart );
		
		if ( DataManager.Stock.FindStockItemByCode ( dlg.m_strStockCodeTo, nEnd ) == FALSE )
			nEnd--;

		for ( int nPos = 0; nPos < m_OrderArray.GetSize(); nPos++ )
		{
			CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
			m_OrderArray.GetAt ( nPos, OrderPreviewRecord );
		
			if ( OrderPreviewRecord.GetStockIdx() >= nStart && OrderPreviewRecord.GetStockIdx() <= nEnd )
			{
				m_OrderArray.RemoveAt ( nPos );
				nPos--;
			}
			else if ( OrderPreviewRecord.GetStockIdx() >= nEnd )
				break;
		}
		m_listOrder.SetItemCountEx ( m_OrderArray.GetSize() );
		DisplayScreen ( nCurrent );
	}
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnButtonAbandon() 
{
	if ( m_OrderArray.GetSize() != 0 )
	{
		CString strMsg = "Are you sure you wish to abandon these orders.";
		if ( Prompter.YesNo ( strMsg ) == IDNO )
			return;
	}
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnButtonContinue() 
{
	if ( ( StockOptions.GetOrderTemplateFlag() == TRUE ) && ( TRUE == m_bAllowSaveTemplate ) )
		HandleContinueWithTemplates();
	else
		HandleContinueWithoutTemplates();
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::HandleContinueWithTemplates()
{
	bool bGenerate = TRUE;

	if ( m_OrderArray.GetSize() != 0 )
	{
		CString strDataFilename; int nLineCount;
		CreateOrderTemplate( strDataFilename, nLineCount );

		if ( AssignSurplusStock() == FALSE )
		{
			CFileRemover FileRemover;
			FileRemover.RemoveFile( strDataFilename );
			return;
		}

		CProcessedOrderAutoPrintDlg dlg ( m_strPreviousOrderFilename, m_strPreviousOrderReference, this );

		dlg.m_bReports = StockOptions.GetOrderAutoPrintOrderFlag();
		dlg.m_bPurchase = StockOptions.GetOrderAutoPrintPurchaseFlag();
		dlg.m_bEmail = StockOptions.GetOrderAutoEmailPurchaseFlag();

		if ( dlg.DoModal() == IDCANCEL )
		{
			CFileRemover FileRemover;
			FileRemover.RemoveFile( strDataFilename );
			return;
		}
		
		bGenerate = ( dlg.m_bGenerate != 0 );
		
		if ( TRUE == bGenerate )
		{
			StockOptions.SetOrderAutoPrintOrderFlag ( dlg.m_bReports != 0 );
			StockOptions.SetOrderAutoPrintPurchaseFlag ( dlg.m_bPurchase != 0 );
			StockOptions.SetOrderAutoEmailPurchaseFlag ( dlg.m_bEmail != 0 );
		}

		CString strNewReference = dlg.m_strReference;
		CString strBaseFilename = "";

		switch( dlg.m_nAction )
		{
		case 1:
			if ( m_strPreviousOrderFilename != "" )
				strBaseFilename = strDataFilename;
			break;

		case 2:
			break;

		case 0:
		default:
			if ( m_strPreviousOrderFilename != "" )
				strBaseFilename = m_strPreviousOrderFilename;
			else
				strBaseFilename = strDataFilename;	
			break;
		}

		if ( strBaseFilename != "" )
		{
			int nLen = strBaseFilename.GetLength();
			
			CString strDataFilename2 = strBaseFilename.Left( nLen - 24 );
			strDataFilename2 += "DATA_";
			strDataFilename2 += strBaseFilename.Mid( nLen - 19, 15 );
			strDataFilename2 += ".DAT";

			CString strInfoFilename2 = strBaseFilename.Left( nLen - 24 );
			strInfoFilename2 += "INFO_";
			strInfoFilename2 += strBaseFilename.Mid( nLen - 19, 15 );
			strInfoFilename2 += ".DAT";
			
			CFileRemover FileRemover;
			FileRemover.RemoveFile( strDataFilename2 );
			FileRemover.RemoveFile( strInfoFilename2 );
			
			rename( strDataFilename, strDataFilename2 );

			CSSFile fileInfo;
			if ( fileInfo.Open( strInfoFilename2, "wb" ) == TRUE )
			{
				CCSV csv;
				csv.Add( 1 );
				csv.Add( strNewReference );
				csv.Add( nLineCount );
				csv.Add( m_LocSelMiniLookup.GetConType() );
				csv.Add( m_LocSelMiniLookup.GetEntityNo() );
				fileInfo.WriteLine( csv.GetLine() );
			}
		}

		CFileRemover FileRemover;
		FileRemover.RemoveFile( strDataFilename );	
	
	}

	if ( TRUE == bGenerate )
		EndDialog ( IDOK );
	else
		EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::HandleContinueWithoutTemplates()
{
	if ( m_OrderArray.GetSize() != 0 )
	{
		if ( AssignSurplusStock() == FALSE )
			return;
		
		CProcessedOrderAutoPrintBasicDlg dlg ( this );

		dlg.m_bReports = StockOptions.GetOrderAutoPrintOrderFlag();
		dlg.m_bPurchase = StockOptions.GetOrderAutoPrintPurchaseFlag();
		dlg.m_bEmail = StockOptions.GetOrderAutoEmailPurchaseFlag();

		if ( dlg.DoModal() == IDCANCEL )
			return;
		
		StockOptions.SetOrderAutoPrintOrderFlag ( dlg.m_bReports != 0 );
		StockOptions.SetOrderAutoPrintPurchaseFlag ( dlg.m_bPurchase != 0 );
		StockOptions.SetOrderAutoEmailPurchaseFlag ( dlg.m_bEmail != 0 );
	}

	EndDialog ( IDOK );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnCancel()
{
	OnButtonContinue();
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnEditOrder() 
{
	int nCurrent = m_listOrder.GetCurSel();

	if ( nCurrent < 0 || nCurrent >= m_OrderArray.GetSize() )
		return;

	EditOrder ( nCurrent );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::EditOrder ( int nIndex )
{
	CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
	CProcessedOrderPreviewCSVRecord OrderPreviewRecordOld;
	CStockCSVRecord StockRecord;
	CStockCSVRecord StockRecordOld;

	m_OrderArray.GetAt ( nIndex, OrderPreviewRecord );
	int nStockIdx = OrderPreviewRecord.GetStockIdx();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	DataManager.Stock.GetAt ( nStockIdx, StockRecordOld );

	CProcessedOrderPreviewEditDlg dlg ( StockRecord, OrderPreviewRecord, m_LocSelMiniLookup, this );
	
	if ( dlg.DoModal() == IDOK )
		m_OrderArray.SetAt ( nIndex, OrderPreviewRecord );
	else
		DataManager.Stock.SetAt ( nStockIdx, StockRecordOld );
		
	DisplayScreen ( nIndex );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnAddOrder() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CArray<int,int> arrayStockIdx;
		CStockFilterDlg dlg ( m_StockFilterArray, &arrayStockIdx, this );
		
		if ( dlg.DoModal() != IDOK )
			return;

		for ( int n = arrayStockIdx.GetSize() - 1; n >= 0 ; n-- )
		{
			int nStockIdx = arrayStockIdx.GetAt(n);
			if ( ( nStockIdx < 0  ) || ( nStockIdx >= DataManager.Stock.GetSize() ) )
				arrayStockIdx.RemoveAt(n);
		}

		if ( arrayStockIdx.GetSize() == 0 )
			return;

		int nNewSelection = -1;
		for ( int n = 0; n < arrayStockIdx.GetSize(); n++ )
		{
			int nStockIdx = arrayStockIdx.GetAt(n);

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );

			int nSuppNo = m_StockFilterArray.GetSupplierFilter();
			int nYieldIdx = -1;
			int nYield = 1;

			for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
			{
				if ( StockRecord.GetSuppNo ( nStockSuppIdx ) == nSuppNo )
				{
					nYieldIdx = nStockSuppIdx;
					break;
				}
			}

			int nSpIdxToUse = GetDefaultStockpointForItem( nStockIdx, StockRecord, m_nDefaultSpIdx );
			if ( nSpIdxToUse < 0 )
				continue;

			CProcessedOrderPreviewCSVRecord NewRecord;
			NewRecord.SetStockIdx ( nStockIdx );
			
			if ( nYieldIdx == -1 )
			{
				NewRecord.SetStockSuppIdx(0);

				if ( StockRecord.GetSupplierCount() == 0 )
					NewRecord.AddSURequired ( nSpIdxToUse, 1.0 );
				else
					NewRecord.AddSURequired ( nSpIdxToUse, StockRecord.GetYield(0) );
			}
			else
			{
				NewRecord.SetStockSuppIdx ( nYieldIdx );
				NewRecord.AddSURequired ( nSpIdxToUse, StockRecord.GetYield(nYieldIdx) );
			}
			
			NewRecord.UpdateSupplierCosts( m_LocSelMiniLookup );
				
			int nOrderIdx;
			if ( m_OrderArray.FindStockIdx ( nStockIdx, nOrderIdx ) == FALSE )
			{
				m_OrderArray.InsertAt ( nOrderIdx, NewRecord );
				m_listOrder.SetItemCountEx ( m_OrderArray.GetSize() );
			}

			nNewSelection = nOrderIdx;
		}

		if ( nNewSelection != -1 )
			DisplayScreen( nNewSelection );

		if ( arrayStockIdx.GetSize() == 1 )
		{
			int nStockIdx = arrayStockIdx.GetAt(0);

			int nOrderIdx;
			if ( m_OrderArray.FindStockIdx ( nStockIdx, nOrderIdx ) == TRUE )
				EditOrder ( nOrderIdx );
		}
	}
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnPrintOrder() 
{
	if ( m_OrderArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must request some orders\nbefore you can preview them." );
		return;
	}

	CReportFile ReportFile;

	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( "Order Preview" );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
		ReportFile.AddColumn ( "Supplier", TA_LEFT, 320 );
		ReportFile.AddColumn ( "Apparent", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "DU Req", TA_RIGHT, 180 );
		ReportFile.AddColumn ( "Total Cost", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "Item Surplus", TA_RIGHT, 250 );

		for ( int nIndex = 0; nIndex < m_OrderArray.GetSize(); nIndex++ )
		{
			CProcessedOrderPreviewCSVRecord OrderRecord;
			CStockCSVRecord StockRecord;
			m_OrderArray.GetAt ( nIndex, OrderRecord );
			DataManager.Stock.GetAt ( OrderRecord.GetStockIdx(), StockRecord );

			CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
			csv.Add ( StockRecord.GetStockCode() );
			csv.Add ( StockRecord.GetDescription() );
			csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );

			int nSuppIdx = OrderRecord.GetSupplierIdx();

			if ( nSuppIdx < 0 )
				csv.Add ( "Unspecified" );
			else
				csv.Add ( DataManager.Supplier.GetName ( nSuppIdx ) );

			csv.Add ( GetApparentStockString( OrderRecord, StockRecord ) );
			csv.Add ( OrderRecord.GetDURequired() );
			csv.Add ( OrderRecord.GetTotalCost(), SysInfo.GetDPValue() );
			csv.Add ( OrderRecord.GetDUItemSurplusString() );
			ReportFile.WriteLine ( csv.GetLine() );
		}
		ReportFile.Close();
		
		CFilenameUpdater FnUp ( SysFiles::OrderPreviewPrm );
		
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/

bool CProcessedOrderPreviewDatabaseDlg::AssignSurplusStock()
{
	//NO NEED TO ASSIGN SURPLUS STOCK IN SEPARATE DU MODE
	if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
		return TRUE;

	int nSurplusSpIdx = m_nDefaultSpIdx;

	if ( nSurplusSpIdx == -1 ) 
		return FALSE;

	if ( m_StockpointArray.GetSize() == 0 )
		return FALSE;

	bool bGotSurplusStock = FALSE;
	int nCount = m_OrderArray.GetSize();

	StatusProgress.Lock( TRUE, "Checking for surplus stock" );
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CProcessedOrderPreviewCSVRecord OrderRecord;
		m_OrderArray.GetAt ( nIndex, OrderRecord );

		if ( OrderRecord.GetDUItemSurplus() != 0 )
		{
			bGotSurplusStock = TRUE;
			break;
		}
	}
	StatusProgress.Unlock();

	if ( bGotSurplusStock == FALSE )
		return TRUE;

	CSelectStockpointSurplusDlg dlg ( m_StockpointArray, nSurplusSpIdx, this );

	if ( dlg.DoModal() == IDCANCEL )
		return FALSE;
		
	nSurplusSpIdx = dlg.m_nSpIdx;

	StatusProgress.Lock( TRUE, "Allocating surplus stock" );
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );		
		CProcessedOrderPreviewCSVRecord OrderRecord;
		m_OrderArray.GetAt ( nIndex, OrderRecord );

		CStockCSVRecord StockRecord;
		int nStockIdx = OrderRecord.GetStockIdx();
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		int nSpIdx = GetDefaultStockpointForItem( nStockIdx, StockRecord, nSurplusSpIdx );

		if ( nSpIdx >= 0 )
		{
			OrderRecord.AllocateSurplusStock ( nSpIdx );
			m_OrderArray.SetAt ( nIndex, OrderRecord );
		}
	}
	StatusProgress.Unlock();

	return TRUE;
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listOrder, this );
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPreviewDatabaseDlg::CreateOrderTemplate( CString& strDataFilename, int& nLineCount )
{
	strDataFilename = "";
	nLineCount = 0;

	int nDbIdx = DataManager.GetActiveDbIdx();
	CString strPath = dbDatabase.GetFolderPathDataOrdersRoot( nDbIdx );
	::CreateSubdirectory( strPath );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strTime;
	strTime.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );
		
	strDataFilename.Format( "%s\\DATA_%s.tmp",
		(const char*) strPath,
		(const char*) strTime );
	
	CSSFile fileTemplate;
	if ( fileTemplate.Open( strDataFilename, "wb" ) == FALSE )
		return;

	for ( int n = 0; n < m_OrderArray.GetSize(); n++ )
	{
		CProcessedOrderPreviewCSVRecord Order;
		m_OrderArray.GetAt( n, Order );

		CArray<int,int> arraySpNo;
		CArray<double,double> arraySUReq;

		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			double d = Order.GetSURequired( nSpIdx );

			if ( d != 0.0 )
			{
				arraySpNo.Add( dbStockpoint.GetSpNo( nSpIdx ) );
				arraySUReq.Add( d );
			}
		}

		int nSpCount = arraySpNo.GetCount();

		if ( nSpCount != 0 )
		{
			CCSV csvOut;
			csvOut.Add( 1 );
			csvOut.Add( DataManager.Stock.GetStockCode( Order.GetStockIdx() ) );
			csvOut.Add( DataManager.Supplier.GetKey( Order.GetSupplierIdx() ) );		
			csvOut.Add( nSpCount );

			for ( int x = 0; x < nSpCount; x++ )
			{
				csvOut.Add( arraySpNo.GetAt(x) );
				csvOut.Add( arraySUReq.GetAt(x), 5 );
			}

			fileTemplate.WriteLine( csvOut.GetLine() );
			nLineCount++;
		}
	}
}

/**********************************************************************/

int CProcessedOrderPreviewDatabaseDlg::GetDefaultStockpointForItem( int nStockIdx, CStockCSVRecord& StockRecord, int nSpIdxPreferred )
{
	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( nSpIdxPreferred ), StockLevels );
	if ( StockLevels.GetActiveFlag() == TRUE )
		return nSpIdxPreferred;

	for ( int n = 0; n < m_StockpointArray.GetSize(); n++ ) 
	{
		int nSpIdx = m_StockpointArray.GetAt(n);

		if ( nSpIdx != nSpIdxPreferred )
		{
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo(nSpIdx), StockLevels );
			if ( StockLevels.GetActiveFlag() == TRUE )
				return nSpIdx;
		}
	}

	return -1;
}

/**********************************************************************/
