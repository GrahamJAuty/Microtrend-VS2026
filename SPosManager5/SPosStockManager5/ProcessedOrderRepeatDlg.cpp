 /**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FileRemover.h"
#include "Password.h"
#include "ProcessedOrderExceptionsDlg.h"
#include "ProcessedOrderPreviewDatabaseDlg.h"
#include "ProcessedOrderPreviewfinaliser.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ProcessedOrderRepeatDlg.h"
/**********************************************************************/

CProcessedOrderRepeatDlg::CProcessedOrderRepeatDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent )
	: CDialog(CProcessedOrderRepeatDlg::IDD, pParent), m_arrayEntityIdx( arrayEntityIdx ),
	m_DisplayHelpers( m_listOrders, m_ContentHelpers )
{
	m_nDbLineIdx = 0;

	m_DisplayHelpers.SetTopSingleText( "saved order" );
	m_DisplayHelpers.SetTopPluralText( "saved orders" );
	m_DisplayHelpers.SetBottomSingleText( "order" );
	m_DisplayHelpers.SetBottomPluralText( "orders" );
}

/**********************************************************************/

CProcessedOrderRepeatDlg::~CProcessedOrderRepeatDlg()
{
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderRepeatDlg)
	DDX_Control( pDX, IDC_BUTTON1, m_button1 );
	DDX_Control( pDX, IDC_BUTTON2, m_button2 );
	DDX_Control( pDX, IDC_BUTTON3, m_button3 );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderRepeatDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATABASE, OnSelectDatabase)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoListOrder)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickListOrder)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderRepeatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listDatabase.SubclassDlgItem( IDC_LIST_DATABASE, this );
	m_listDatabase.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listDatabase.InsertColumn ( 0, "Database", LVCFMT_LEFT, 220 );

	m_ContentHelpers.SetColumns( m_listOrders, this );

	for ( int n = 0; n < m_arrayEntityIdx.GetSize(); n++ )
	{
		int nDbIdx = m_arrayEntityIdx.GetAt(n);

		int nListPos = m_arrayLocation.GetSize();
		m_listDatabase.InsertItem ( nListPos, "" );
		m_listDatabase.SetItem ( nListPos, 0, LVIF_TEXT, dbDatabase.GetName( nDbIdx ), 0, 0, 0, NULL );
	
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetDatabaseNode( nDbIdx );
		m_arrayLocation.Add( LocSelEntity );
	}
	
	SelectDatabaseLine(0);
	BuildOrderList( m_arrayEntityIdx.GetAt(0) );
	SelectOrderLine(0);

	return TRUE;
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )	
	{	
	case IDC_LIST_DATABASE:
		m_listDatabase.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST:	
		m_listOrders.AdjustRowHeight( lpMeasureItemStruct, 10 );	
		break;	
	}
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::SelectDatabaseLine ( int nLineIdx )
{
	m_listDatabase.Invalidate();

	for ( int n = 0; n < m_listDatabase.GetItemCount(); n++ )
	{
		if ( n != nLineIdx )
			m_listDatabase.SetItemState(n, 0, LVIS_SELECTED | LVIS_FOCUSED );
		else
		{
			m_listDatabase.SetItemState(n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
			m_listDatabase.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

int CProcessedOrderRepeatDlg::GetOrderListSize()
{
	return m_ContentHelpers.GetArraySize();
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::BuildOrderList( int nLineIdx )
{
	m_nDbLineIdx = nLineIdx;

	CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt( m_nDbLineIdx );
	m_ContentHelpers.SetDbIdx( LocSelEntity.GetDbIdx() );
	
	m_ContentHelpers.RebuildList();
	m_DisplayHelpers.PrepareDatabaseList( TRUE, FALSE );
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnGetDispInfoListOrder(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CProcessedOrderRepeatDlg::OnDoubleClickListOrder(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButton2();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		int nSel = m_listDatabase.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
		{
			BuildOrderList( nSel );
			SelectOrderLine(0);
		}
    }
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::SelectOrderLine ( int nIndex )
{
	m_DisplayHelpers.SelectDatabaseLine( nIndex );
}

/**********************************************************************/

int CProcessedOrderRepeatDlg::GetSafeOrderSelection()
{
	int nMin = 0;
	int nMax = GetOrderListSize() - 1;
	
	int nSel = m_listOrders.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listOrders.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::SelectLineByDate( CRepeatOrderInfo& info )
{
	int nLine = 0;
	for ( int nIndex = 0; nIndex < GetOrderListSize(); nIndex++ )
	{
		CRepeatOrderInfo infoArray;
		m_ContentHelpers.GetOrderInfo( nIndex, infoArray );
		
		if ( ( info.m_strDate == infoArray.m_strDate ) && ( info.m_strTime == infoArray.m_strTime ) )
		{
			nLine = nIndex;
			break;
		}
	}
	SelectOrderLine( nLine );
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnButton1()
{
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );
	arrayReadWrite.Add( NODE_PLU );

	if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_ORDER )
		arrayReadWrite.Add( NODE_ORDER );

	CDataManagerInfo infoDM;
	DataManager.CloseDatabase();
	if ( DataManager.OpenDatabaseReadWrite( m_ContentHelpers.GetDbIdx(), arrayReadWrite, infoDM ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( infoDM );
	}
	else
	{
		DataManager.Order.SortOrders();
		Button1Internal();	
		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::Button1Internal()
{
	int nSel = GetSafeOrderSelection();
	if ( ( nSel < 0 ) || ( nSel >= GetOrderListSize() ) )
		return;

	CRepeatOrderInfo info;
	m_ContentHelpers.GetOrderInfo( nSel, info );
	
	int nDbIdx = DataManager.GetActiveDbIdx();
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	
	CString strFilename = "";
	strFilename += m_ContentHelpers.GetOrderFolder();
	strFilename += "\\";
	strFilename += info.m_strDataFilename;

	CProcessedOrderPreviewCSVArray OrderPreviewArray;
	if ( RecreateOrder( OrderPreviewArray, info ) == FALSE )
		return;

	CString strTitle;
	strTitle.Format( "Repeat Order (%s  %s  %s)",
		(const char*) info.m_strDateDisplay,
		(const char*) info.m_strTimeDisplay,
		(const char*) info.m_strReference );

	info.m_bAllowSaveTemplate= TRUE;
	if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC )
	{
		int nSpIdx = PasswordArray.GetLocationAccessSpIdx();
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		if ( ( info.m_nConLevel != NODE_STOCKPOINT ) || ( info.m_nTableNo != nSpNo ) )
			info.m_bAllowSaveTemplate = FALSE;

		info.m_nConLevel = NODE_STOCKPOINT;
		info.m_nTableNo = nSpNo;
	}
	
	CLocationSelectorMiniLookup LocSelMiniLookup( 
		info.m_nConLevel,
		info.m_nTableNo );

	CProcessedOrderPreviewDatabaseDlg dlgPreview ( strTitle, OrderPreviewArray, LocSelMiniLookup, this );
	dlgPreview.SetPreviousOrderDetails( strFilename, info.m_strReference, info.m_bAllowSaveTemplate ); 

	int nResult = dlgPreview.DoModal();

	m_ContentHelpers.RebuildList();
	m_listOrders.SetItemCountEx( GetOrderListSize() );
	SelectLineByDate( info );
	
	if ( ( IDCANCEL == nResult ) || ( OrderPreviewArray.GetSize() == 0 ) )
		Prompter.Info ( "No order lines were generated" );
	else
	{
		CProcessedOrderPreviewFinaliser Finaliser( this, LocSelMiniLookup, "", "" );

		if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_ORDER )
			Finaliser.CreateStockpointOrders( OrderPreviewArray );
		else
			Finaliser.CreateDatabaseOrders( OrderPreviewArray );
	}
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnButton2()
{
	int nSel = GetSafeOrderSelection();
	if ( ( nSel < 0 ) || ( nSel >= GetOrderListSize() ) )
		return;

	CRepeatOrderInfo info;
	m_ContentHelpers.GetOrderInfo ( nSel, info );

	CString strReference = info.m_strReference;
	if ( "" == strReference )
		strReference = "Previous Order";

	CDefEditDlg dlgName( "Edit Order Name", strReference, 20, "", this );
	if ( dlgName.DoModal() != IDOK )
		return;

	CString strInfoFile = "";
	strInfoFile += m_ContentHelpers.GetOrderFolder();
	strInfoFile += "\\";
	strInfoFile += info.m_strInfoFilename;

	CSSFile fileInfo;
	if ( fileInfo.Open( strInfoFile, "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( dlgName.m_strName );
		csv.Add( info.m_nLineCount );
		csv.Add( info.m_nConLevel );
		csv.Add( info.m_nTableNo );
		fileInfo.WriteLine( csv.GetLine() );
		fileInfo.Close();

		m_ContentHelpers.RebuildList();
		m_listOrders.SetItemCountEx( GetOrderListSize() );
		SelectLineByDate( info );
	}
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnButton3()
{
	int nSel = GetSafeOrderSelection();

	m_DisplayHelpers.HandleDelete( this );
	BuildOrderList( m_ContentHelpers.GetDbIdx() );
	m_listOrders.SetItemCountEx( GetOrderListSize() );
		
	SelectOrderLine( nSel );
}

/**********************************************************************/

bool CProcessedOrderRepeatDlg::RecreateOrder( CProcessedOrderPreviewCSVArray& OrderPreviewArray, CRepeatOrderInfo& info )
{
	CString strOrderInfo;
	strOrderInfo.Format( "%s  %s  %s",
		(const char*) info.m_strDateDisplay,
		(const char*) info.m_strTimeDisplay,
		(const char*) info.m_strReference );
		
	CString strTitle = "Repeat Order Exceptions ";
	strTitle += strOrderInfo;

	CReportFile ReportFile;
	ReportFile.Open ( Super.ReportFilename() );
	ReportFile.SetStyle1 ( strTitle );
	ReportFile.AddColumn ( "Stock Code", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Supplier Key", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Supplier Name", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );
	ReportFile.AddColumn ( "Sp No", TA_RIGHT, 150 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 150 );
	
	StatusProgress.Lock( TRUE, "Generating order preview" );
	bool bGotException = RecreateOrderInternal ( OrderPreviewArray, ReportFile, info );
	StatusProgress.Unlock();

	ReportFile.Close();

	if ( TRUE == bGotException )
	{
		CProcessedOrderExceptionsDlg dlg( strOrderInfo, this );
		if ( dlg.DoModal() != IDOK )
			return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CProcessedOrderRepeatDlg::RecreateOrderInternal( CProcessedOrderPreviewCSVArray& OrderPreviewArray, CReportFile& ReportFile, CRepeatOrderInfo& info )
{
	bool bGotException = FALSE;

	CString strFilename = "";
	strFilename += m_ContentHelpers.GetOrderFolder();
	strFilename += "\\";
	strFilename += info.m_strDataFilename;

	CSSFile filePrevious;
	if ( filePrevious.Open( strFilename, "rb" ) == FALSE )
		return FALSE;

	CString strBuffer;
	while( filePrevious.ReadString( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos( filePrevious );

		CCSV csv( strBuffer );
		if ( csv.GetInt(0) != 1 )
			continue;

		CString strStockCode = csv.GetString(1);
		CString strKey = csv.GetString(2);
		
		CCSV csvProblem ( '\t' );
		csvProblem.Add( strStockCode );
		csvProblem.Add( "" );
		csvProblem.Add( "" );
		csvProblem.Add( "" );
		csvProblem.Add( "" );
		csvProblem.Add( "" );
		csvProblem.Add( "" );
			
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
		{
			csvProblem.SetAt( 4, "Unknown Stock Item" );
			ReportFile.WriteLine( csvProblem.GetLine() );
			ReportFile.WriteLine( "<LI>" );
			bGotException = TRUE;
			continue;
		}

		int nSupplierIdx = -1;
		int nStockSuppIdx = -1;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		csvProblem.SetAt( 1, StockRecord.GetDescription() );

		if ( strKey != "" )
		{
			csvProblem.SetAt( 2, strKey );

			if ( DataManager.Supplier.FindSupplierByKey( strKey, nSupplierIdx ) == FALSE )
			{
				csvProblem.SetAt( 4, "Unknown Supplier" );
				ReportFile.WriteLine( csvProblem.GetLine() );
				ReportFile.WriteLine( "<LI>" );
				bGotException = TRUE;
				continue;
			}

			csvProblem.SetAt( 3, DataManager.Supplier.GetName( nSupplierIdx ) );

			int nSuppNo = DataManager.Supplier.GetSuppNo( nSupplierIdx );

			for ( int n = 0; n < StockRecord.GetSupplierCount(); n++ )
			{
				if ( StockRecord.GetSuppNo( n ) == nSuppNo )
				{
					nStockSuppIdx = n;
					break;
				}
			}

			if ( -1 == nStockSuppIdx )
			{
				csvProblem.SetAt( 4, "Stock item not linked to supplier" );
				ReportFile.WriteLine( csvProblem.GetLine() );
				ReportFile.WriteLine( "<LI>" );
				bGotException = TRUE;
				continue;
			}
		}
		else
			csvProblem.SetAt( 3, "Unspecified" );

		int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );

		CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
		OrderPreviewRecord.SetStockIdx( nStockIdx );
		OrderPreviewRecord.SetStockSuppIdx( nStockSuppIdx );
		OrderPreviewRecord.SetSupplierIdx( nSupplierIdx );

		bool bGotLineException = FALSE;

		int nOffset = 4;
		for ( int n = 0; n < csv.GetInt(3); n++ )
		{
			int nSpNo = csv.GetInt( nOffset++ );
			double dSUReq = csv.GetDouble( nOffset++ );

			int nSpIdx;
			if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == FALSE )
			{
				csvProblem.SetAt( 4, "Unknown Stockpoint" );
				csvProblem.SetAt( 5, nSpNo );
				csvProblem.SetAt( 6, dSUReq, 3 );
				ReportFile.WriteLine( csvProblem.GetLine() );
				bGotLineException = TRUE;
				bGotException = TRUE;
			}
			else if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == FALSE )
			{
				csvProblem.SetAt( 4, "Unauthorised to access stockpoint" );
				csvProblem.SetAt( 5, nSpNo );
				csvProblem.SetAt( 6, dSUReq, 3 );
				ReportFile.WriteLine( csvProblem.GetLine() );
				bGotLineException = TRUE;
				bGotException = TRUE;
			}
			else
			{
				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
				if ( StockLevels.GetActiveFlag() == FALSE )
				{
					csvProblem.SetAt( 4, "Stock item inactive at stockpoint" );
					csvProblem.SetAt( 5, nSpNo );
					csvProblem.SetAt( 6, dSUReq, 3 );
					ReportFile.WriteLine( csvProblem.GetLine() );
					bGotLineException = TRUE;
					bGotException = TRUE;
				}
			}
			
			if ( FALSE == bGotLineException )
			{
				bool bAccept = TRUE;

				if ( StockOptions.GetOrderRestrictPreviewFlag() == TRUE )
				{
					if ( NODE_SITE == info.m_nConLevel )
					{
						int nNewSiteNo = dbStockpoint.GetSiteNo( nSpIdx );
						if ( nNewSiteNo != info.m_nTableNo )
						{
							CString strProblem;
							if ( nNewSiteNo != 0 )
								strProblem.Format( "Site link changed from %d to %d", info.m_nTableNo, nNewSiteNo );
							else
								strProblem.Format( "Site link changed from %d to none", info.m_nTableNo );

							csvProblem.SetAt( 4, strProblem );
							csvProblem.SetAt( 5, nSpNo );
							csvProblem.SetAt( 6, dSUReq, 3 );
							ReportFile.WriteLine( csvProblem.GetLine() );
							bGotLineException = TRUE;
							bGotException = TRUE;
							bAccept = FALSE;
						}
					}
				}

				if ( TRUE == bAccept )
					OrderPreviewRecord.AddSURequired ( nSpIdx, dSUReq );
			}
		}

		CLocationSelectorMiniLookup LocSelMiniLookup(
			NODE_DATABASE,
			nDbNo );
		
		if ( OrderPreviewRecord.HasOrder( LocSelMiniLookup ) == TRUE )
		{
			OrderPreviewRecord.UpdateSupplierCosts( LocSelMiniLookup );
			OrderPreviewArray.Add ( OrderPreviewRecord );
		}

		if ( TRUE == bGotLineException )
			ReportFile.WriteLine( "<LI>" );
	}

	return bGotException;
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnCancel()
{
	OnOK();
}

/**********************************************************************/

void CProcessedOrderRepeatDlg::OnOK()
{	
	for ( int n = 0; n < m_ContentHelpers.GetDeletedArraySize(); n++ )
	{
		CSortedStringItem item;
		m_ContentHelpers.GetDeletedItem( n, item );
		CString strLabel = item.m_strItem;

		int nDbIdx = atoi( strLabel.Left(4) );
		CString strOrderFolder = dbDatabase.GetFolderPathDataOrdersRoot( nDbIdx );

		CString strDate = strLabel.Mid(4,8);
		CString strTime = strLabel.Right(6);

		CString strInfoFile;
		strInfoFile.Format( "%s\\INFO_%s_%s.dat",
			(const char*) strOrderFolder,
			(const char*) strDate,
			(const char*) strTime );

		CString strDataFile;
		strDataFile.Format( "%s\\DATA_%s_%s.dat",
			(const char*) strOrderFolder,
			(const char*) strDate,
			(const char*) strTime );

		CFileRemover FileRemover;
		FileRemover.RemoveFile( strInfoFile );
		FileRemover.RemoveFile( strDataFile );
	}

	EndDialog( IDOK );
}

/**********************************************************************/
