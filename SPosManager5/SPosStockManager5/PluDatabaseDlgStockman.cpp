/**********************************************************************/
#include "..\CommonEcrStock\BarcodeDlg.h"
#include "..\CommonEcrStock\ColumnListDlg.h"
#include "..\CommonEcrStock\CommsBatchDlg.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "..\CommonEcrStock\CSVDBImportStatsDlg.h"
#include "..\CommonEcrStock\DownloadDateDlg.h"
#include "..\CommonEcrStock\KeyboardFilterSelector.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "..\CommonEcrStock\KeyboardMap.h"
#include "..\CommonEcrStock\LabelListCSVArray.h"
#include "..\CommonEcrStock\LabelListCreator.h"
#include "..\CommonEcrStock\ListLabelsDlg.h"
#include "..\CommonEcrStock\LocalPluFilterDlg.h"
#include "..\CommonEcrStock\PluCustomListDlg.h"
#include "..\CommonEcrStock\PluDefineFilterDlg.h"
#include "..\CommonEcrStock\PluDeleteSingleDlg.h"
#include "..\CommonEcrStock\PluExportDlg.h"
#include "..\CommonEcrStock\PluImportStatsComplexDlg.h"
#include "..\CommonEcrStock\CSVDBImportTypeDlg.h"
#include "..\CommonEcrStock\PluNumberDlg.h"
#include "..\CommonEcrStock\PluPrtBarcodeDlg.h"
#include "..\CommonEcrStock\PluRangeCopyDlg.h"
#include "..\CommonEcrStock\PluRangeDlg.h"
#include "..\CommonEcrStock\PluRangePriceDlg.h"
#include "..\CommonEcrStock\PluRangePriceModifierDlg.h"
#include "..\CommonEcrStock\PluRangeProfitDlg.h"
#include "..\CommonEcrStock\PluRangeProgramDlg.h"
#include "..\CommonEcrStock\PluRangeProgramModifierDlg.h"
#include "..\CommonEcrStock\PluRangeStatusDlg.h"
#include "..\CommonEcrStock\PluSearchDlg.h"
#include "..\CommonEcrStock\PriceSetImportHandler.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "..\CommonEcrStock\PrintSingleLabelDlg.h"
#include "..\CommonEcrStock\ReportPluCustomList.h"
#include "..\CommonEcrStock\SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\EcrmanSupplierCSVArray.h"
#include "..\CommonEcrStockTray\TaxImportMap.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluImportStatsDlgStockman.h"
#include "..\SPosStockmanager5\PluImportTypeDlgStockman.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "ColumnManager.h"
#include "DatabaseCSVArray.h"
#include "DatabaseImportHandlerStockman.h"
#include "EditStockTypes.h"
#include "ExportDate.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "ImportDefines.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "OpeningStockAuditer.h"
#include "Password.h"
#include "PCOptions.h"
#include "PluDatabaseHeader.h"
#include "PosTrayTokenFiles.h"
#include "PluNumberFullStockDlg.h"
#include "PluPrtStockLinkDlg.h"
#include "PluRangeAddDlgStockman.h"
#include "PluRangeStockDlg.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetPluStock.h"
#include "PropertySheetPluStockman.h"
#include "StockCodeDlg.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockPluArray.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PluDatabaseDlgStockman.h"
/**********************************************************************/

CPluDatabaseDlgStockman::CPluDatabaseDlgStockman( int nDbIdx, CWnd* pParent )
	: CDialog(CPluDatabaseDlgStockman::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_DATABASE ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CPluDatabaseDlgStockman)
	//}}AFX_DATA_INIT
	m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
	m_strDbName = dbDatabase.GetName( nDbIdx );
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;	
	m_StockCodeSearch.m_bSearchFromStartTick = TRUE;
	m_nCacheIndex = -1;
	m_nSuppNo = 0;
	m_bReadOnly = FALSE;
	m_bCanDoChangesTask = FALSE;
	m_bCanDeleteBarcode = FALSE;
	m_bCanDeleteStock = FALSE;
	m_nPluSearchMode = NODE_PLU;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
	m_nNewPluDeptNo = 0;
}

/**********************************************************************/

CPluDatabaseDlgStockman::~CPluDatabaseDlgStockman()
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

void CPluDatabaseDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDatabaseDlgStockman)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDatabaseDlgStockman, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CPluDatabaseDlgStockman)
	ON_WM_SIZE()
	ON_COMMAND(IDM_PLU_RANGE_DELETE, OnPluRangeDelete)
	ON_COMMAND(IDM_PLU_DELETE_CURRENT, OnPluDeleteCurrent)
	ON_COMMAND(IDM_PLU_DATABASE_ADD, OnPluDatabaseAdd)
	ON_COMMAND(IDM_PLU_EXPORT_CSV, OnPluExportCsv)
	ON_COMMAND(IDM_PLU_IMPORT_CSV, OnPluImportCsv)
	ON_COMMAND(IDM_PLU_PRT_PRICELIST, OnPluPrtPricelist)
	ON_COMMAND(IDM_RANGE_PRICE, OnPluRangePrices)
	ON_COMMAND(IDM_RANGE_SETTINGS, OnPluRangeSettings)
	ON_COMMAND(IDM_PLU_PRT_PROGRAM, OnPluPrtProgram)
	ON_COMMAND(IDM_RANGE_PROFIT, OnPluRangeProfit)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(IDM_PLU_RANGE_ADD, OnPluRangeAdd)
	ON_COMMAND(IDM_PLU_DATABASE_EDIT, OnPluEdit)
	ON_COMMAND(IDM_FILTER_PLURECORDS, OnFilterPluRecords)
	ON_COMMAND(IDM_NOFILTER_PLURECORDS, OnNoFilterPluRecords)
	ON_COMMAND(IDM_RANGE_STATUS, OnRangeStatus)
	ON_COMMAND(IDM_FILTER_STATUS, OnFilterPluStatus)
	ON_COMMAND(IDM_PLU_DATABASE_COPY, OnPluDatabaseCopy)
	ON_COMMAND(IDM_RANGE_STOCK, OnPluRangeStock)
	ON_BN_CLICKED(IDC_BUTTON_STOCK, OnButtonStock)
	ON_COMMAND(IDM_PLU_PRT_STOCKLINKS, OnPluPrtStocklinks)
	ON_COMMAND(IDM_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_PLU_PRT_BARCODELINKS, OnPluPrtBarcodeLinks)
	ON_COMMAND(IDM_BARCODE_NEW, OnBarcodeNew)
	ON_COMMAND(IDM_SHELF_NEW, OnShelfNew)
	ON_COMMAND(IDM_BARCODE_SAVED, OnBarcodeSaved)
	ON_COMMAND(IDM_SHELF_SAVED, OnShelfSaved)
	ON_COMMAND(IDM_RANGE_MODIFIER_PRICES, OnPluRangeModifierPrices)
	ON_COMMAND(IDM_RANGE_MODIFIER_SETTINGS, OnPluRangeModifierSettings)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	ON_COMMAND(IDM_PLU_EXPORT_EPOS, OnPluExportEpos)
	ON_COMMAND(IDM_PLUSTOCK_IMPORT_CSV, OnPluStockImportCSV)
	ON_COMMAND(IDM_PLU_EXIT, OnCancel)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnPluEdit)
	ON_COMMAND(IDM_REPORTS_CUSTOM1, OnReportsCustom1)
	ON_COMMAND(IDM_REPORTS_CUSTOM2, OnReportsCustom2)
	ON_COMMAND(IDM_REPORTS_CUSTOM3, OnReportsCustom3)
	ON_COMMAND(IDM_REPORTS_CUSTOM4, OnReportsCustom4)
	ON_COMMAND(IDM_REPORTS_CUSTOM5, OnReportsCustom5)
	ON_COMMAND(IDM_PLU_DATABASE_SHELF, OnPluDatabaseShelf)
	ON_COMMAND(IDM_PLU_DATABASE_PRODUCT, OnPluDatabaseProduct)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDatabaseDlgStockman::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listPlu.SubclassDlgItem ( IDC_LIST, this );
	
	if ( PriceSetItemManager.IsPluMaster() )
		m_ColumnManager.Reset( TableNames::Plu );
	else
		m_ColumnManager.Reset( TableNames::PriceSet );

	m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu Number", LVCFMT_LEFT, 115 ,115, COLUMN_STATUS_LOCKED );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 120, 170, COLUMN_STATUS_SHOWN );
	m_ColumnManager.AddColumn( ColumnNames::EPOSText, "EPOS Text", LVCFMT_LEFT, 120, 170, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn( ColumnNames::StockCode,"Stock Code", LVCFMT_LEFT, 115, 115, COLUMN_STATUS_SHOWN );			
	m_ColumnManager.AddColumn( ColumnNames::Dept, "Dept", LVCFMT_LEFT, 40, 40, COLUMN_STATUS_SHOWN );	
	m_ColumnManager.AddColumn( ColumnNames::Price1, "Price1", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_SHOWN );
	m_ColumnManager.AddColumn( ColumnNames::Price2, "Price2",	LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_SHOWN );					
	m_ColumnManager.AddColumn( ColumnNames::Price3, "Price3", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.AddColumn( ColumnNames::Price4, "Price4",	LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	
	if ( SysInfo.GotLoyaltyOrSmartPayLink() )
		m_ColumnManager.AddColumn( ColumnNames::Points, "Points",	LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	
	m_ColumnManager.AddColumn( ColumnNames::PluType, "Type", LVCFMT_LEFT, 60, 60, COLUMN_STATUS_SHOWN );					
	m_ColumnManager.AddColumn( ColumnNames::Tax, EcrmanOptions.GetSalesTaxName(), LVCFMT_LEFT, 40, 40, COLUMN_STATUS_SHOWN );
	m_ColumnManager.AddColumn( ColumnNames::Printers, "Printers", LVCFMT_LEFT, 55, 70, COLUMN_STATUS_HIDDEN );

	if ( !PriceSetItemManager.IsPluMaster() )
		m_ColumnManager.AddColumn( ColumnNames::PluStatus, "Status", LVCFMT_LEFT, 70, 70, COLUMN_STATUS_SHOWN );

	m_ColumnManager.AddColumn( ColumnNames::Supplier, "Supplier", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.AddColumn( ColumnNames::Cost, "Cost", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.LoadSettings();

	m_listPlu.SetItemCountEx ( DataManager.Plu.GetSize() );

	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listPlu, this );
	CenterWindow();

	m_MenuChanger.EatMenuTopLevel ( GetMenu() );

	if ( EcrmanOptions.GetPluCSVManualImportFlag() == FALSE )
	{
		m_MenuChanger.KillItem ( IDM_PLU_IMPORT_CSV );
		m_MenuChanger.KillItem ( IDM_PLUSTOCK_IMPORT_CSV );
	}

	if ( EcrmanOptions.GetPluCSVManualExportFlag() == FALSE )
		m_MenuChanger.KillItem ( IDM_PLU_EXPORT_CSV );

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		m_MenuChanger.KillItem ( IDM_PLU_DATABASE_ADD );
		m_MenuChanger.KillItem ( IDM_PLU_DATABASE_COPY );
		m_MenuChanger.KillItem ( IDM_PLU_DELETE_CURRENT );
		m_MenuChanger.KillItem ( IDM_RANGE_PROFIT );
		m_MenuChanger.KillItem ( IDM_RANGE_STOCK );
		m_MenuChanger.KillItem ( IDM_PLU_RANGE_ADD );
		m_MenuChanger.KillItem ( IDM_PLU_RANGE_DELETE );
		m_MenuChanger.KillItem ( IDM_PLU_EXPORT_EPOS );
		m_MenuChanger.KillItem ( IDM_PLUSTOCK_IMPORT_CSV );

		if ( CPluRangePriceDlg::IsRelevant() == FALSE )
			m_MenuChanger.KillItem ( IDM_RANGE_PRICE );

		if ( CPluRangePriceModifierDlg::IsRelevant() == FALSE )
			m_MenuChanger.KillItem ( IDM_RANGE_MODIFIER_PRICES );

		if ( CPluRangeProgramDlg::IsRelevant() == FALSE )
			m_MenuChanger.KillItem ( IDM_RANGE_SETTINGS );

		if ( CPluRangeProgramModifierDlg::IsRelevant() == FALSE )
			m_MenuChanger.KillItem ( IDM_RANGE_MODIFIER_SETTINGS );
	}
	else
	{
		m_MenuChanger.KillItem ( IDM_RANGE_STATUS );
		m_MenuChanger.KillItem ( IDM_FILTER_STATUS );
		m_MenuChanger.KillItem ( IDM_PLU_IMPORT_CSV );
	}

	if ( Sysset.IsBarmanSystem() == FALSE )
	{
		m_MenuChanger.KillParent ( IDM_BARCODE_NEW );
		m_MenuChanger.KillParent ( IDM_SHELF_NEW );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
		m_MenuChanger.KillItem ( IDM_PLU_EXPORT_EPOS );

	if ( m_bReadOnly == TRUE )
	{
		m_MenuChanger.KillItem( IDM_PLU_IMPORT_CSV );
		m_MenuChanger.KillItem ( IDM_PLUSTOCK_IMPORT_CSV );
		//Export flag requires read/write access
		m_MenuChanger.KillItem ( IDM_PLU_EXPORT_CSV );
		m_MenuChanger.KillItem( IDM_IMPORT_DELIVERY );
		m_MenuChanger.KillItem( IDM_PLU_DATABASE_ADD );
		m_MenuChanger.KillItem( IDM_PLU_DATABASE_COPY );
		m_MenuChanger.KillItem( IDM_PLU_DELETE_CURRENT );
		m_MenuChanger.KillParent( IDM_PLU_RANGE_DELETE );
		m_MenuChanger.SetItemText( IDM_PLU_DATABASE_EDIT, "View" );
	}

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_MenuChanger.KillItem( IDM_RANGE_MODIFIER_PRICES );
		m_MenuChanger.KillItem( IDM_RANGE_MODIFIER_SETTINGS );
	}

	int nPresetNo = PasswordArray.GetPluFilterPresetNo(DataManager.GetActiveDbIdx());

	if ((nPresetNo >= 1) && (nPresetNo < 20))
	{
		if (PriceSetItemManager.IsPluMaster())
		{
			m_MenuChanger.KillParent(IDM_FILTER_PLURECORDS);
		}
		else
		{
			m_MenuChanger.KillItem(IDM_FILTER_PLURECORDS);
			m_MenuChanger.KillItem(IDM_NOFILTER_PLURECORDS);
		}

		CPresetPluFilter Preset;
		Preset.SetFilename(DataManager.GetActiveDbIdx(), nPresetNo);
		Preset.Read();
		m_FilterArray.CopyFromPreset(Preset);
		BuildRecordList();
	}
	else if (EcrmanOptions.GetPluInactiveAutoHideFlag() == TRUE)
	{
		m_FilterArray.SetShowInactiveItemsFlag(FALSE);
		BuildRecordList();
	}

	SetCustomListMenuNames();
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );
	if ( PriceSetItemManager.IsPluMaster() == FALSE ) dw.Add( PriceSetItemManager.GetPriceSetNumber() );
	MessageLogger.LogSystem( 2, 1, TRUE, dw );

	CFileRemover FileRemover;
	CFilenameUpdater FnUp( SysFiles::OpeningTemp );
	FileRemover.RemoveFile( FnUp.GetFilenameToUse() );

	if ( EcrmanOptions.GetPluInactiveAutoHideFlag() == TRUE )
	{
		m_FilterArray.SetShowInactiveItemsFlag(FALSE);
		BuildRecordList();
	}

	DisplayScreen ( 0 );
	return TRUE;  
}

/**********************************************************************/

void CPluDatabaseDlgStockman::SetCustomListMenuNames()
{
	CString strName;
	CReportPluCustomList::GetCSVReportName( 1, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM1, strName );
	CReportPluCustomList::GetCSVReportName( 2, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM2, strName );
	CReportPluCustomList::GetCSVReportName( 3, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM3, strName );
	CReportPluCustomList::GetCSVReportName( 4, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM4, strName );
	CReportPluCustomList::GetCSVReportName( 5, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM5, strName );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listPlu.MoveWindow ( 0, 0, cx, cy );
	}
}

/**********************************************************************/

bool CPluDatabaseDlgStockman::ContextMenuSort( CPoint& point )
{
	HWND hWnd = HWND( m_listPlu );

	HDHITTESTINFO hitTest{};
	hitTest.pt.x = point.x;
	hitTest.pt.y = point.y;

	::ScreenToClient( hWnd, &hitTest.pt );

	CHeaderCtrl* headerCtrl = m_listPlu.GetHeaderCtrl();
	int nIndex = headerCtrl -> SendMessage( HDM_HITTEST, 0, (LPARAM) &hitTest );

	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_PLUNO:
		SortListInternal( PLUFILTER_SORT_FORWARD_PLUNO );
		return TRUE;
	
	case COLUMN_DESC:
		SortListInternal( PLUFILTER_SORT_FORWARD_TEXT );
		return TRUE;

	case COLUMN_EPOSTEXT:
		SortListInternal(PLUFILTER_SORT_FORWARD_EPOSTEXT);
		return TRUE;

	case COLUMN_DEPT:
		SortListInternal( PLUFILTER_SORT_FORWARD_DEPT );
		return TRUE;
	
	case COLUMN_STOCKCODE:
		SortListInternal( PLUFILTER_SORT_FORWARD_STOCKCODE );
		return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnContextMenu(CWnd*, CPoint point)
{
	if ( ContextMenuSort( point ) == TRUE )
		return;

	if ( m_bReadOnly == TRUE )
		return;

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		return;

	//NOTHING TO DO IF NO RECORDS IN DATABASE
	if ( DataManager.Plu.GetSize() == 0 )
		return;

	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_PLU_DATABASE));
	CMenu* pPopup = menu.GetSubMenu(0);

	int nSel = m_listPlu.GetCurSel();
	int nPluIdx = m_FilterArray.GetPluItemIndexAt( nSel );

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuPopup( pPopup );

	MenuChanger.KillItem( IDM_PLU_DATABASE_DELIVER );
	
	bool bCanDoShelf = TRUE;
	if ( EcrmanOptions.GetLabelsShelfPrintSingleFlag() == FALSE )
		bCanDoShelf = FALSE;
	else
	{
		CLabelListCSVArray arrayLabels( FALSE );
		arrayLabels.CreateSingleItemList( nPluIdx );
		bCanDoShelf = ( arrayLabels.GetSize() != 0 );
	}

	if ( FALSE == bCanDoShelf )
		MenuChanger.KillItem( IDM_PLU_DATABASE_SHELF );

	bool bCanDoProduct = TRUE;
	if ( EcrmanOptions.GetLabelsProductPrintSingleFlag() == FALSE )
		bCanDoProduct = FALSE;
	else
	{
		CLabelListCSVArray arrayLabels( TRUE );
		arrayLabels.CreateSingleItemList( nPluIdx );
		bCanDoProduct = ( arrayLabels.GetSize() != 0 );
	}

	if ( FALSE == bCanDoProduct )
		MenuChanger.KillItem( IDM_PLU_DATABASE_PRODUCT );

	MenuChanger.BuildMenu( pPopup );

	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnButtonPluNumber()
{
	CPluSearchDlg dlg ( m_nPluSearchMode, this );
	dlg.SetPluNo(0);
	dlg.SetBarcodeNo(0);
	
	if ( dlg.DoModal() == IDOK )
	{
		m_nPluSearchMode = dlg.m_nMode;
		__int64 nPluNo = dlg.GetPluNo();
		__int64 nBarcodeNo = dlg.GetBarcodeNo();

		if ( NODE_BARCODE == m_nPluSearchMode )
		{
			int nBarcodeIdx;
			bool bFound = DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx );
			
			CString strBarcodeNo;
			::FormatBarcodeNo( nBarcodeNo, strBarcodeNo );

			if ( bFound == FALSE )
			{
				CString strMsg;
				strMsg.Format ( "Cannot find Barcode number %s", 
					(const char*) strBarcodeNo );

				Prompter.Error ( strMsg );
			}
			else
			{
				CBarcodeCSVRecord Barcode;
				nPluNo = DataManager.Barcode.GetPluFromBarcode( nBarcodeNo );
				
				int nPluIdx;
				bFound = m_FilterArray.FindPluItemByNumber ( nPluNo, nPluIdx );

				if ( bFound == FALSE )
				{
					CString strPluNo;
					::FormatPluNo( nPluNo, strPluNo );
					
					CString strMsg;
					strMsg.Format ( "Cannot find Plu number %s linked to barcode %s", 
						(const char*) strPluNo,
						(const char*) strBarcodeNo );

					Prompter.Error ( strMsg );
				}
				
				m_listPlu.SetCurSel ( nPluIdx );
			}
		}
		else
		{
			CString strPluNo;
			::FormatPluNo( nPluNo, strPluNo );

			int nPluIdx;
			bool bFound = m_FilterArray.FindPluItemByNumber ( nPluNo, nPluIdx );

			if ( bFound == FALSE )
			{
				CString strMsg;
				strMsg.Format ( "Cannot find Plu number %s", 
					(const char*) strPluNo );
				
				Prompter.Error ( strMsg );
			}		
			m_listPlu.SetCurSel ( nPluIdx );
		}
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnButtonPluStockCode()
{
	CStockCodeDlg dlg ( "Find Stock Code", m_StockCodeSearch, this );
	
	if ( dlg.DoModal() != IDOK )
		return;
	
	CString strStockCode = m_StockCodeSearch.m_strStockCodeToUse;

	int nStartPos = ( m_StockCodeSearch.m_bSearchFromStart ) ? 0 : m_listPlu.GetCurSel() + 1;
	int nSize = ( m_FilterArray.GetSize() - nStartPos );
	bool bFound = FALSE;
	int nOffset;

	CPluCSVRecord PluRecord;
	CString strPluStockCode;
		
	StatusProgress.Lock( TRUE, "Searching for stock code" );
	for ( nOffset = 0; nStartPos + nOffset < m_FilterArray.GetSize(); nOffset++ )
	{
		DataManager.Plu.GetAt ( m_FilterArray.GetPluItemIndexAt ( nStartPos + nOffset ), PluRecord );
		DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strPluStockCode, FALSE );
		
		if ( strPluStockCode == strStockCode )
		{
			bFound = TRUE;
			break;
		}

		StatusProgress.SetPos ( nOffset, nSize );
	}
	StatusProgress.Unlock();
		
	if ( bFound == TRUE )
		m_listPlu.SetCurSel ( nStartPos + nOffset );
	else
	{
		CString strMsg;
		strMsg.Format ( "Cannot find Plu with Stock Code '%s'", (const char*) strStockCode );
		Prompter.Error ( strMsg );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeDelete() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	const char* szTitle = "Delete Plu Range";
	CPluRangeDlg dlg ( szTitle, this );

	int nCurrent = m_listPlu.GetCurSel();
	
	if ( nCurrent >= 0 )
	{
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nCurrent );
		CString strPlu = DataManager.Plu.GetPluNoString ( nPluIdx );
		dlg.m_strPluFrom = strPlu; 
		dlg.m_strPluTo = strPlu;
	}
	else
	{
		int nPluIdx = m_FilterArray.GetPluItemIndexAt(0);
		CString strPlu = DataManager.Plu.GetPluNoString( nPluIdx );
		dlg.m_strPluFrom = strPlu;
		dlg.m_strPluTo = strPlu;
	}
	
	if ( dlg.DoModal() != IDOK )
		return;

	CStringArray str;

	CString strTemp1 = dlg.m_strPluFrom;
	::TrimSpaces( strTemp1, TRUE );
	str.Add( strTemp1 );

	CString strTemp2 = dlg.m_strPluTo;
	::TrimSpaces( strTemp2, TRUE );
	str.Add( strTemp2 );

	__int64 nPluNoFrom = _atoi64( dlg.m_strPluFrom );
	__int64 nPluNoTo = _atoi64( dlg.m_strPluTo );

	int nStartIdx, nEndIdx;
	DataManager.Plu.FindPluByNumber( nPluNoFrom, nStartIdx );
	DataManager.Plu.FindPluByNumber( nPluNoTo + 1, nEndIdx );

	for ( int nPluIdx = nStartIdx; nPluIdx < nEndIdx; nPluIdx++ )
	{
		DataManager.ResourceWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
		DataManager.ListKeyImageWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
	}

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	m_FilterArray.DeletePluItemRange ( nPluNoFrom, nPluNoTo );
	dw.Add( DataManager.Plu.GetSize() );

	MessageLogger.LogSystem( 2, 13, FALSE, dw, str );
		
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
	DisplayScreen ( nCurrent );	
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeAdd() 
{			
	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );

	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
		m_nNewPluDeptNo = m_FilterArray.GetDepartmentFilter();

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	PluInfo.m_nNewPluSuppNo = m_nSuppNo;

	CPluRangeAddDlgStockman dlgPluRange ( PluInfo, this );
	if ( dlgPluRange.DoModal() != IDOK )
		return;

	m_nSuppNo = PluInfo.m_nNewPluSuppNo;

	if ( PluInfo.NewPluNoAsInt64() == 0 || PluInfo.m_lNewPluCopyCount == 0 )
		return;

	if ( PluInfo.MaxNewPluLen() > Super.MaxPluLen() )
	{	
		Prompter.Error ( "The requested range of Plu numbers is not valid" );
		return;
	}

	CArray<__int64,__int64> arrayNewPlu;
	CStringArray arrayNewStock;

	CString strRawPluNo = PluInfo.m_strNewPluNo;
	::TrimSpaces( strRawPluNo, FALSE );
	CString strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
	CString strNewStockCode = strNewPluNo;
	
	//LOOK THROUGH THE REQUESTED RANGE OF NUMBERS FOR EXISTING ITEMS
	StatusProgress.Lock( TRUE, "Checking item range" );
	for ( long lProgress = 0; lProgress < PluInfo.m_lNewPluCopyCount; lProgress++ )
	{
		StatusProgress.SetPos ( lProgress, PluInfo.m_lNewPluCopyCount );
		
		__int64 nNewPluNo = _atoi64( strNewPluNo );

		if ( SysInfo.GetMaxBasePluLen() != 0 )
		{
			CString strTestPlu;
			strTestPlu.Format( "%I64d", nNewPluNo );
			if ( strTestPlu.GetLength() == SysInfo.GetMaxBasePluLen() + 1 )
				break;
		}

		int nArrayIdx;
		if ( DataManager.Plu.FindPluByNumber ( nNewPluNo, nArrayIdx ) )
		{
			GetNextPluNo ( strRawPluNo );
			strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
			continue;
		}
	
		if ( PluInfo.m_bNewPluCreateStock )
		{	
			if ( StockOptions.GetStockAddLeadingZerosFlag() )
				strNewStockCode = strNewPluNo;
			else
				strNewStockCode = strRawPluNo;

			if ( DataManager.Stock.FindStockItemByCode ( strNewStockCode, nArrayIdx ) == TRUE )
			{
				GetNextPluNo ( strRawPluNo );
				strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
				continue;
			}
		}
		
		arrayNewPlu.Add ( nNewPluNo );
		arrayNewStock.Add ( strNewStockCode );
		GetNextPluNo ( strRawPluNo );
		strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
	}
	StatusProgress.Unlock();

	int nFound = PluInfo.m_lNewPluCopyCount - arrayNewPlu.GetSize();

	CString strStock = "";
	if ( TRUE == PluInfo.m_bNewPluCreateStock ) 
		strStock += " or stock";

	if ( nFound >= PluInfo.m_lNewPluCopyCount )
	{
		CString strMsg;
		strMsg.Format( "None of the PLU%s numbers in this range are available.",
			(const char*) strStock );

		Prompter.Error( strMsg );

		return;
	}	
	
	if ( nFound != 0 )
	{
		CString strMsg;

		if ( nFound == 1 )
			strMsg.Format ( "1 of the PLU%s numbers from this range is not available.\n\n",
				(const char*) strStock );
		else
			strMsg.Format ( "%d of the PLU%s numbers from this range are not available.\n\n", 
				nFound, 
				(const char*) strStock );

		strMsg += "Do you wish to create new items for the remaining numbers.";
		
		if ( Prompter.YesNo ( strMsg, FALSE ) != IDYES )
			return;
	}

	bool bUsedSpecimen = FALSE;

	CPluCSVRecord PluRecord;
	CStockCSVRecord StockRecord;
	
	m_nNewPluDeptNo = PluInfo.m_nNewPluDeptNo;

	if ( DataManager.Plu.CopyDefaultPlu( m_nNewPluDeptNo, PluRecord ) == TRUE )
		bUsedSpecimen = TRUE;

	PluRecord.SetBaseDeptNo( m_nNewPluDeptNo );
	PluRecord.SetModifiedDateTimePlu();
	
	StatusProgress.Lock( TRUE, "Creating PLU items" );
	int nSize = arrayNewPlu.GetSize();
	__int64 nFirstNewPluNo = 0;

	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nSize );
		
		__int64 nNewPluNo = arrayNewPlu.GetAt ( nIndex );
		CString strNewStockCode = arrayNewStock.GetAt ( nIndex );
	
		PluRecord.SetPluNo ( nNewPluNo );
		
		if ( FALSE == bUsedSpecimen )
			PluRecord.SetDefaultText();
		
		if ( nFirstNewPluNo == 0 )
			nFirstNewPluNo = nNewPluNo;
		
		if ( PluInfo.m_bNewPluCreateStock )
		{
			StockRecord.SetStockCode ( strNewStockCode );
			StockRecord.SetDescription ( "Plu" );
			StockRecord.SetCategory( m_nNewPluDeptNo );

			if ( PluInfo.m_nNewPluSuppNo != 0 )
			{
				StockRecord.SetSuppNo( 0, PluInfo.m_nNewPluSuppNo );
				StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
			}

			int nStockIdx;
			DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
			m_FilterArray.InsertPluRecord ( PluRecord, strNewStockCode, TRUE );
		}
		else
		{
			m_FilterArray.InsertPluRecord(PluRecord, "", TRUE);
		}
	}
	StatusProgress.Unlock();

	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	int nArrayIdx;
	if ( m_FilterArray.FindPluItemByNumber ( nFirstNewPluNo, nArrayIdx ) == FALSE )
		nArrayIdx = 0;

	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );
	MessageLogger.LogSystem( 2, 14, FALSE, dw );

	DisplayScreen ( nArrayIdx );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluDatabaseCopy() 
{
	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nSourcePluIdx = -1;
	
	int nFilterIdx = m_listPlu.GetCurSel();
	if ( nFilterIdx >= 0 && nFilterIdx < m_FilterArray.GetSize() )
	{
		PluInfo.m_nSourcePluIdx = m_FilterArray.GetPluItemIndexAt( nFilterIdx );
		if ( PluInfo.m_nSourcePluIdx < 0 || PluInfo.m_nSourcePluIdx >= DataManager.Plu.GetSize() )
			return;
	}
			
	CPluRangeCopyDlg dlgPluRange ( PluInfo, this );
	if ( dlgPluRange.DoModal() != IDOK )
		return;

	if ( PluInfo.NewPluNoAsInt64() == 0 || PluInfo.m_lNewPluCopyCount == 0 )
		return;

	if ( PluInfo.MaxNewPluLen() > Super.MaxPluLen() )
	{	
		Prompter.Error ( "The requested range of Plu numbers is not valid" );
		return;
	}

	CArray<__int64,__int64> arrayNewPlu;
	CStringArray arrayNewStock;

	CString strRawPluNo = PluInfo.m_strNewPluNo;
	::TrimSpaces( strRawPluNo, FALSE );
	CString strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
	CString strNewStockCode = "";
	
	//LOOK THROUGH THE REQUESTED RANGE OF NUMBERS FOR EXISTING ITEMS
	StatusProgress.Lock( TRUE, "Checking item range" );
	for ( long lProgress = 0; lProgress < PluInfo.m_lNewPluCopyCount; lProgress++ )
	{
		StatusProgress.SetPos ( lProgress, PluInfo.m_lNewPluCopyCount );

		__int64 nNewPluNo = _atoi64( strNewPluNo );

		if ( SysInfo.GetMaxBasePluLen() != 0 )
		{
			CString strTestPlu;
			strTestPlu.Format( "%I64d", nNewPluNo );
			if ( strTestPlu.GetLength() == SysInfo.GetMaxBasePluLen() + 1 )
				break;
		}
		
		int nArrayIdx;
		if ( DataManager.Plu.FindPluByNumber ( nNewPluNo, nArrayIdx ) ) 
		{
			GetNextPluNo ( strRawPluNo );
			strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
			continue;
		}
			
		if ( TRUE == PluInfo.m_bNewPluCreateStock )
		{
			if ( StockOptions.GetStockAddLeadingZerosFlag() )
				strNewStockCode = strNewPluNo;
			else
				strNewStockCode = strRawPluNo;

			if ( DataManager.Stock.FindStockItemByCode ( strNewStockCode, nArrayIdx ) == TRUE )
			{
				GetNextPluNo ( strRawPluNo );
				strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
				continue;
			}	
		}
		arrayNewPlu.Add ( nNewPluNo );
		arrayNewStock.Add ( strNewStockCode );
		GetNextPluNo ( strRawPluNo );
		strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );		
	}
	StatusProgress.Unlock();

	int nFound = PluInfo.m_lNewPluCopyCount - arrayNewPlu.GetSize();

	CString strStock = "";
	if ( TRUE == PluInfo.m_bNewPluCreateStock ) 
		strStock += " or stock";

	if ( nFound >= PluInfo.m_lNewPluCopyCount )
	{
		CString strMsg;
		strMsg.Format( "None of the PLU%s numbers in this range are available.",
			(const char*) strStock );

		Prompter.Error( strMsg );

		return;
	}
	
	if ( nFound != 0 )
	{
		CString strMsg;

		if ( nFound == 1 )
			strMsg.Format ( "1 of the PLU%s numbers from this range is not available.\n\n",
				(const char*) strStock );
		else
			strMsg.Format ( "%d of the PLU%s numbers from this range are not available.\n\n", 
				nFound, 
				(const char*) strStock );

		strMsg += "Do you wish to create new items for the remaining numbers.";
		
		if ( Prompter.YesNo ( strMsg, FALSE ) != IDYES )
			return;
	}

	
	CPluCSVRecord PluRecord;
	CStockCSVRecord StockRecord;
	CRecipeCSVRecord RecipeRecord;

	//READ THE SOURCE RECORD AND CLEAR SPECIAL FIELDS
	//RETAIN THE OLD DOWNLOAD DATE AS WE ARE COPYING THE RECORD
	DataManager.Plu.GetAt ( PluInfo.m_nSourcePluIdx, PluRecord );
	PluRecord.SetModifiedDateTimePlu();
	DataManager.Plu.GetRecipe ( PluInfo.m_nSourcePluIdx, PluRecord.GetPluNoInt(), RecipeRecord );

	bool bCopyStock = !(PluInfo.m_bNewPluCreateStock);

	if ( TRUE == PluInfo.m_bNewPluCreateStock ) 
	{	
		if ( RecipeRecord.GetSize() != 1 )
		{
			PluInfo.m_bNewPluCreateStock = FALSE;
			bCopyStock = TRUE;
		}
		else
		{
			int nStockIdx;
			CString strStockCode = RecipeRecord.GetStockCode(0);
			if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) )
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			else
				PluInfo.m_bNewPluCreateStock = FALSE;
		}
	}

	StatusProgress.Lock( TRUE, "Creating PLU items" );
	int nSize = arrayNewPlu.GetSize();
	
	__int64 nFirstNewPluNo = 0;
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nSize );
		__int64 nNewPluNo = arrayNewPlu.GetAt ( nIndex );
		CString strNewStockCode = arrayNewStock.GetAt ( nIndex );
		
		PluRecord.SetPluNo ( nNewPluNo );
		
		if ( nFirstNewPluNo == 0 )
			nFirstNewPluNo = nNewPluNo;

		if ( TRUE == PluInfo.m_bNewPluCreateStock )
		{
			int nStockIdx;
			StockRecord.SetStockCode( strNewStockCode );
			DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
			m_FilterArray.InsertPluRecord( PluRecord, strNewStockCode, TRUE );
		}
		else if (TRUE == bCopyStock)
		{
			m_FilterArray.InsertPluRecord(PluRecord, RecipeRecord, TRUE);
		}
		else
		{
			m_FilterArray.InsertPluRecord(PluRecord, "", TRUE);
		}
	}
	StatusProgress.Unlock();

	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	int nArrayIdx;
	if ( m_FilterArray.FindPluItemByNumber ( nFirstNewPluNo, nArrayIdx ) == FALSE )
		nArrayIdx = 0;

	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );
	MessageLogger.LogSystem( 2, 15, FALSE, dw );

	DisplayScreen ( nArrayIdx );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::DisplayScreen ( int nIndex )
{
	int nFilterSize = m_FilterArray.GetSize();
	int nArraySize = DataManager.Plu.GetSize();

	bool bWantSpace = FALSE;
	CCSV csvDbName;
		
	if ( SysInfo.GetMaxDatabases() > 1 )
	{
		csvDbName.Add ( m_strDbName );
		bWantSpace = TRUE;
	}

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		CString strName = ( bWantSpace == TRUE ) ? " " : "";
		csvDbName.Add ( strName + PriceSetItemManager.GetPriceSetNameAndNumber() );
		bWantSpace = TRUE;
	}
	
	CString strTemp;
	strTemp.Format ( "%d of %d records", nFilterSize, nArraySize );
	CString strCount = ( bWantSpace == TRUE ) ? " " : "";
	strCount += strTemp;
	csvDbName.Add ( strCount );

	if ( m_FilterArray.GetFilteredFlag() == TRUE ) csvDbName.Add ( " Filtered" );

	CString strText;	
	strText.Format ( "Plu Database (%s)", csvDbName.GetLine() );
	if ( m_bReadOnly == TRUE ) strText += " (READ ONLY)";
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nFilterSize ) )
		m_listPlu.SetCurSel ( nIndex );
	else
		m_listPlu.SetCurSel ( nFilterSize - 1 );

	m_listPlu.Invalidate();
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluDeleteCurrent() 
{
	int nCurrent = m_listPlu.GetCurSel();

	if ( nCurrent < 0 || nCurrent >= m_FilterArray.GetSize() )
		return;

	CPluCSVRecord PluRecord;
	int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nCurrent );
	DataManager.Plu.GetAt( nPluIdx, PluRecord );

	CPluDeleteSingleDlg dlg ( PluRecord, this );
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.Plu.RemoveAt ( nPluIdx );
		m_FilterArray.RemoveIndex ( nCurrent );
		m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
		DataManager.ResourceWrapperPlu.FlagDeletedPlu( PluRecord.GetPluNoInt() );
		DataManager.ListKeyImageWrapperPlu.FlagDeletedPlu( PluRecord.GetPluNoInt() );
		DisplayScreen ( nCurrent );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluDatabaseAdd() 
{
	if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
	{
		m_nNewPluDeptNo = m_FilterArray.GetDepartmentFilter();
	}

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = m_nSuppNo;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	
	CPluNumberFullStockDlg dlg ( TRUE, PluInfo, this );
	
	while ( dlg.DoModal() == IDOK )
	{
		m_nSuppNo = PluInfo.m_nNewPluSuppNo;

		CString strEnteredPluNo = PluInfo.m_strNewPluNo;
		CString strConvertedPluNo = BarcodePluNoTable.ConvertPlu ( strEnteredPluNo );
		__int64 nConvertedPluNo = _atoi64( strConvertedPluNo );

		CString strNewStockCode = PluInfo.m_strNewPluStockCode;
		
		int nIndex = 0;
		if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
		{
			int nMaxBaseLen = SysInfo.GetMaxBasePluLen();
			if ( nMaxBaseLen != 0 )
			{
				CString strTestPluNo;
				strTestPluNo.Format( "%I64d", nConvertedPluNo );

				if ( strTestPluNo.GetLength() == nMaxBaseLen + 1 )
				{
					CString strMessage;
					strMessage.Format( "%s is not configured to allow %d digit PLU items.", 
						AfxGetAppName(),
						nMaxBaseLen + 1 );
					
					Prompter.ContactDealer( strMessage );
					continue;
				}
			}

			m_nNewPluDeptNo = PluInfo.m_nNewPluDeptNo;

			CPluCSVRecord NewRecord;
			NewRecord.SetPluNo ( nConvertedPluNo );

			if (DataManager.Plu.CopyDefaultPlu(m_nNewPluDeptNo, NewRecord) == FALSE)
			{
				NewRecord.SetDefaultText();
			}

			NewRecord.SetBaseDeptNo( m_nNewPluDeptNo );
			NewRecord.SetModifiedDateTimePlu();
			
			bool bNewStockCode = FALSE;

			if ( strNewStockCode != "" )
			{
				::TrimSpaces ( strNewStockCode, FALSE );
					
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode( strNewStockCode, nStockIdx ) == FALSE )
				{
					bNewStockCode = TRUE;

					CStockCSVRecord StockRecord;
					StockRecord.SetStockCode ( strNewStockCode );
					StockRecord.SetDescription ( "Plu" );

					if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
					{
						StockRecord.SetCategory(m_FilterArray.GetDepartmentFilter());
					}

					if ( PluInfo.m_nNewPluSuppNo != 0 )
					{
						StockRecord.SetSuppNo( 0, PluInfo.m_nNewPluSuppNo );
						StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
					}

					DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
				}
			}
			
			int nInsertionPoint = m_FilterArray.InsertPluRecord ( NewRecord, strNewStockCode, TRUE );
			m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
			DisplayScreen ( nInsertionPoint );
			EditPlu ( nInsertionPoint, bNewStockCode, TRUE );

			GetNextPluNo ( strEnteredPluNo );
			switch ( DealerFlags.GetAddPluMode() )
			{
			case 1:		
				PluInfo.m_strNewPluNo = "";		
				break;

			case 2:		
				return;

			case 0:
			default:		
				PluInfo.m_strNewPluNo = strEnteredPluNo;	
				break;
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format ( "Plu number %s already exists in your database\n\nDo you wish to search for the next free number", 
				(const char*) strConvertedPluNo );

			if (Prompter.YesNo(strMsg, TRUE) != IDYES)
			{
				continue;
			}

			int nProgress = 0;
			bool bGotFreeNumber = FALSE;

			StatusProgress.Lock( TRUE, "Finding next PLU number" );
			do
			{
				StatusProgress.SetPos ( nProgress++, DataManager.Plu.GetSize() );

				if (GetNextPluNo(strEnteredPluNo) == FALSE)
				{
					break;
				}

				__int64 nConvertedPluNo = _atoi64(BarcodePluNoTable.ConvertPlu( strEnteredPluNo ) );
				if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
				{
					bGotFreeNumber = TRUE;
					break;
				}				
			}
			while ( TRUE );
			StatusProgress.Unlock();

			if (bGotFreeNumber == FALSE)
			{
				Prompter.Error("Unable to find another free Plu number");
			}
			else
			{
				PluInfo.m_strNewPluNo = strEnteredPluNo;
			}
		}
	}
}

/**********************************************************************/

bool CPluDatabaseDlgStockman::GetNextPluNo( CString& strPluNo )
{
	int nLength = strPluNo.GetLength();
	
	CString strNewPluNo = strPluNo;
	::TrimSpaces ( strNewPluNo, TRUE );

	if ( strNewPluNo == "" )
		return FALSE;

	__int64 nNewPluNo = _atoi64 ( strNewPluNo ) + 1;
	strNewPluNo.Format ( "%I64d", nNewPluNo );

	::AddLeading ( strNewPluNo, nLength, '0' );
	if ( strNewPluNo.GetLength() > Super.MaxPluLen() )
		return FALSE;	
			
	strPluNo = strNewPluNo;
	return TRUE;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangePrices() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listPlu.GetCurSel();
	CPluRangePriceDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeModifierPrices() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listPlu.GetCurSel();
	CPluRangePriceModifierDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo, TRUE );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom, TRUE );
	::FormatPluNo( nPluTo, dlg.m_strPluTo, TRUE );

	dlg.DoModal();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeModifierSettings() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listPlu.GetCurSel();
	CPluRangeProgramModifierDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeProfit() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	int nCurrent = m_listPlu.GetCurSel();
	CPluRangeProfitDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeSettings() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listPlu.GetCurSel();
	CPluRangeProgramDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluRangeStock() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listPlu.GetCurSel();
	CPluRangeStockDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnRangeStatus() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	int nCurrent = m_listPlu.GetCurSel();	
	CPluRangeStatusDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluExportCsv() 
{
	CPtrArray CustomFieldPtrArray;
	DataManager.GetCustomFieldPtrArray( CustomFieldPtrArray );
	
	ExportDate.Read();

	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	CPluExportDlg dlgFields( m_FilterArray, m_listPlu.GetCurSel(), this);
	if ( dlgFields.DoModal() == IDCANCEL )
		return;

	int nStart, nCount, nEnd;
	m_FilterArray.FindPluItemByNumber ( _atoi64(dlgFields.m_strPluFrom), nStart );
	bool bEndFound = m_FilterArray.FindPluItemByNumber ( _atoi64(dlgFields.m_strPluTo), nEnd );
	nCount = ( bEndFound == FALSE ) ? nEnd - nStart : (nEnd - nStart) + 1;

	if ( nCount == 0 )
	{
		Prompter.Error ( "There are no Plus in the selected range" );
		return;
	}

	CString strPathName = ExportDate.GetLastPluFile();

	int nSuppCount = StockOptions.GetStockMaxSuppliersPerItem();

	CString strPluDatabaseHeader, strExportHeaderToUse;
	if ( DealerFlags.GetLegacyPluExportFlag() == FALSE )
	{
		strPluDatabaseHeader = PluDatabaseHeader.GetDatabaseHeader();
		strExportHeaderToUse = dlgFields.GetPluHeaderLine();
	}
	else
	{
		strPluDatabaseHeader = PluDatabaseHeader.GetLegacyHeader();
		strExportHeaderToUse = PluDatabaseHeader.GetLegacyHeader( dlgFields.GetPluHeaderLine() );
	}

	CString strDatabaseHeader = "StockQty,PluStockLink,";
	strDatabaseHeader += CStockCSVRecord::GetHeader( nSuppCount );
	strDatabaseHeader += ",";
	strDatabaseHeader += strPluDatabaseHeader;
	
	CSSExportFilterFile fileExportPlu ( strDatabaseHeader, strExportHeaderToUse );
	if ( fileExportPlu.Open ( strPathName, "wb" ) == FALSE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to create export file %s", (const char*) strPathName );
		Prompter.Error ( strMsg );
		return;
	}

	//DUMMY STOCK RECORD TO FILL SPACE FOR UNLINKED PLU ITEMS
	CStockCSVRecord stockDummy;
	CCSV csvDummy( ',', '"', FALSE, FALSE, TRUE);
	stockDummy.ConvertToCSVSimpleExport ( csvDummy, DataManager.Supplier, CustomFieldPtrArray, nSuppCount );
	CString strStockDummy = csvDummy.GetLine();	

	int nPluExported = 0;

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting PLU records" );
	for ( int nProgress = 0; nProgress < nCount; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nCount );	
		
		CPluCSVRecord PluRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStart + nProgress );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		//IGNORE RECORDS THAT HAVE NOT CHANGED
		if ( ( dlgFields.m_bChangesOnly == TRUE ) && ( PluRecord.GetExport() == FALSE ) )
			continue;

		CRecipeCSVRecord RecipeRecord;
		DataManager.Plu.GetRecipe( nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord );

		//FIDDLE : USE COST PRICE FROM STOCK RECORD FOR EXPORT UNLESS NON STOCK ITEM
		double dOldCost = PluRecord.GetPurchaseCost();
		if ( RecipeRecord.GetPluStockLinkFlag() == TRUE )
			PluRecord.SetPurchaseCost( DataManager.Plu.GetPluPurchaseCost( PluRecord ) );

		//EXPORT THE PLU RECORD
		CCSV csvPlu( ',', '"', FALSE, FALSE, TRUE);
		PluRecord.CreateExportLine( csvPlu );
		CString strPluLine = csvPlu.GetLine();

		CString strStockLine = strStockDummy;
		double dStockQty = 0.0;

		if ( RecipeRecord.GetSize() == 1 )
		{
			CString strStockCode = RecipeRecord.GetStockCode(0);

			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );
				
				CCSV csvStock( ',', '"', FALSE, FALSE, TRUE);
				StockRecord.ConvertToCSVSimpleExport ( csvStock, DataManager.Supplier, CustomFieldPtrArray, nSuppCount );
				strStockLine = csvStock.GetLine();

				dStockQty = RecipeRecord.GetStockQty(0);
			}
		}

		CCSV csvExtra( ',', '"', FALSE, FALSE, TRUE);
		csvExtra.Add( dStockQty, 5 );
		csvExtra.Add( RecipeRecord.GetPluStockLinkFlag() );
		CString strExtra = csvExtra.GetLine();
		
		CString strLine;
		strLine.Format ( "%s,%s,%s",
			(const char*) strExtra,
			(const char*) strStockLine,
			(const char*) strPluLine );

		fileExportPlu.WriteData ( strLine );
		nPluExported++;

		PluRecord.SetExport ( FALSE );
		PluRecord.SetPurchaseCost( dOldCost );
		DataManager.Plu.SetAt ( nPluIdx, PluRecord );
	}
	
	StatusProgress.Unlock();
	fileExportPlu.Close();
	
	//UPDATE THE PREVIOUS EXPORT DATE
	CTime time = CTime::GetCurrentTime();
	ExportDate.SetDate ( time );
	ExportDate.Write();

	//SHOW THE RESULTS
	CString strMsg;
	switch ( nPluExported )
	{
	case 0:	strMsg = "No Plu items were exported.";	break;
	case 1:	strMsg = "1 Plu item was exported.";	break;
	default:	strMsg.Format ( "%d Plu items were exported.", nPluExported );	break;
	}
	
	Prompter.Info ( strMsg );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluImportCsv() 
{
	TaxImportMap.Read();

	CString strImportFolder = "";
	CFilenameUpdater FnUp( SysFiles::PluImport, DataManager.GetActiveDbIdx() );
	
	{
		CSSFile fileImportFolder;
		if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			fileImportFolder.ReadString( strImportFolder );
	}

	CString strTitle = "Import Plu CSV File";
	CFileDialog dlgFilename ( TRUE, "csv", "plu", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	dlgFilename.m_ofn.lpstrInitialDir = strImportFolder;
	
	if ( dlgFilename.DoModal() != IDOK )
		return;

	CString strFilename = dlgFilename.GetPathName();

	bool bComplete = FALSE;
	if ( PriceSetItemManager.IsPluMaster() == TRUE )
		bComplete = HandleCSVImportForMaster( strFilename );
	else
		bComplete = HandleCSVImportForPriceSet( strFilename );

	if ( bComplete == TRUE )
	{
		CString strPath = dlgFilename.GetPathName();
		CString strFile = dlgFilename.GetFileName();

		if ( strPath.GetLength() >= strFile.GetLength() )
		{
			strPath = strPath.Left ( strPath.GetLength() - strFile.GetLength() );

			CSSFile fileImportFolder;
			if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
				fileImportFolder.WriteLine( strPath );
		}
	}
}

/**********************************************************************/

bool CPluDatabaseDlgStockman::HandleCSVImportForMaster( CString& strFilename )
{
	CPluImportTypeDlgStockman dlgMethod ( this );
	dlgMethod.m_bCreateStock = StockOptions.GetStockAutoCreateFlag();
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;

	if ( dlgMethod.DoModal() != IDOK )
		return FALSE;

	CDatabaseImportHandlerStockman ImportHandler;
	ImportHandler.SetPluImportMethod ( dlgMethod.m_nImportMethod );
	ImportHandler.SetCreateStockFlag ( dlgMethod.m_bCreateStock != 0 );
	int nResult = ImportHandler.ImportPluFile ( strFilename );
	
	if ( nResult != IMPORTERROR_NONE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to process import file\n(error code %d)", nResult );
		Prompter.Error ( strMsg );
		return FALSE;
	}

	CDWordArray dw;
	dw.Add( ImportHandler.GetImportTotalLines() );
	dw.Add( ImportHandler.GetImportIgnoredLines() );
	dw.Add( ImportHandler.GetImportPluAdded() );
	dw.Add( ImportHandler.GetImportPluModified() );
	dw.Add( ImportHandler.GetImportStockAdded() );
	MessageLogger.LogSystem( 2, 3, FALSE, dw );

	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetStockmanStockCodeFilter ( "" );
	m_FilterArray.SetDepartmentFilter( DEPARTMENTSET_ALL );
	BuildRecordList();
	DisplayScreen(0);			
	
	CPluImportStatsDlgStockman dlgInfo ( FALSE, this );
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nPluAdded = ImportHandler.GetImportPluAdded();
	dlgInfo.m_nPluModified = ImportHandler.GetImportPluModified();
	dlgInfo.m_nStockAdded = ImportHandler.GetImportStockAdded();	
	dlgInfo.DoModal();

	return TRUE;
}

/**********************************************************************/

bool CPluDatabaseDlgStockman::HandleCSVImportForPriceSet(CString& strFilename)
{
	CString strPrompt = "";
	strPrompt += "Would you like to add items to the price set if they are\n";
	strPrompt += "present in the import file but not already included.";

	int nPromptResult = Prompter.YesNoCancel(strPrompt, FALSE);
	if (nPromptResult == IDCANCEL)
	{
		return FALSE;
	}

	CPriceSetImportHandler ImportHandler;
	int nResult = ImportHandler.ImportFile(strFilename, (IDYES == nPromptResult));

	if (nResult != IMPORTERROR_NONE)
	{
		CString strMsg;
		strMsg.Format("Unable to process import file\n(error code %d)", nResult);
		Prompter.Error(strMsg);
		return FALSE;
	}

	CDWordArray dw;
	dw.Add(ImportHandler.GetImportTotalLines());
	dw.Add(ImportHandler.GetImportIgnoredLines());
	dw.Add(ImportHandler.GetImportPluModified());
	MessageLogger.LogSystem(2, 12, FALSE, dw);

	m_FilterArray.SetDescriptionFilter("", 0, FALSE);
	m_FilterArray.SetStockmanStockCodeFilter("");
	m_FilterArray.SetDepartmentFilter(DEPARTMENTSET_ALL);
	BuildRecordList();
	DisplayScreen(0);

	CCSVDBImportStatsDlg dlgInfo(NODE_PLU, this);
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nRecordsAdded = 0;
	dlgInfo.m_nRecordsModified = ImportHandler.GetImportPluModified();
	dlgInfo.DoModal();

	return TRUE;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluStockImportCSV()
{
	TaxImportMap.Read();

	CString strImportFolder = "";
	CFilenameUpdater FnUp(SysFiles::PluImport, DataManager.GetActiveDbIdx());

	{
		CSSFile fileImportFolder;
		if (fileImportFolder.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
			fileImportFolder.ReadString(strImportFolder);
	}

	CString strTitle = "Import Plu/Stock CSV File";
	CFileDialog dlgFilename(TRUE, "csv", "plustock", OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this);
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	dlgFilename.m_ofn.lpstrInitialDir = strImportFolder;

	if (dlgFilename.DoModal() != IDOK)
		return;

	CDataManagerInfo info;
	if (DataManager.OpenSupplierBuffer(info) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	for (int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++)
	{
		if (DataManager.OpenCustomFieldValueBuffer(nFieldNo, info) == FALSE)
		{
			Prompter.ShareError(info);
			DataManager.CloseSupplierBuffer(info);

			for (int x = 1; x < nFieldNo; x++)
				DataManager.CloseCustomFieldValueBuffer(x, info);

			return;
		}
	}

	CString strFilename = dlgFilename.GetPathName();

	if (HandlePluStockCSVImport(strFilename) == TRUE)
	{
		CString strPath = dlgFilename.GetPathName();
		CString strFile = dlgFilename.GetFileName();

		if (strPath.GetLength() >= strFile.GetLength())
		{
			strPath = strPath.Left(strPath.GetLength() - strFile.GetLength());

			CSSFile fileImportFolder;
			if (fileImportFolder.Open(FnUp.GetFilenameToUse(), "wb") == TRUE)
			{
				fileImportFolder.WriteLine(strPath);
			}
		}
	}

	if (DataManager.WriteSupplierBuffer(info) == FALSE)
	{
		Prompter.WriteError(info);
	}

	for (int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++)
	{
		if (DataManager.WriteCustomFieldValueBuffer(nFieldNo, info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseSupplierBuffer(info);

	for (int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++)
	{
		DataManager.CloseCustomFieldValueBuffer(nFieldNo, info);
	}
}

/**********************************************************************/

bool CPluDatabaseDlgStockman::HandlePluStockCSVImport( CString& strFilename )
{
	CCSVDBImportTypeDlg dlgMethod ( NODE_STOCK, this );
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;

	if (dlgMethod.DoModal() != IDOK)
	{
		return FALSE;
	}

	CDatabaseImportHandlerStockman ImportHandler;
	ImportHandler.SetPluImportMethod ( dlgMethod.m_nImportMethod );
	int nResult = ImportHandler.ImportPluStockFile ( strFilename );
	
	if ( nResult != IMPORTERROR_NONE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to process import file\n(error code %d)", nResult );
		Prompter.Error ( strMsg );
		return FALSE;
	}

	CDWordArray dw;
	dw.Add( ImportHandler.GetImportTotalLines() );
	dw.Add( ImportHandler.GetImportIgnoredLines() );
	dw.Add( ImportHandler.GetImportPluAdded() );
	dw.Add( ImportHandler.GetImportPluModified() );
	dw.Add( ImportHandler.GetImportStockAdded() );
	dw.Add( ImportHandler.GetImportStockModified() );
	dw.Add( ImportHandler.GetImportSupplierAdded() );
	MessageLogger.LogSystem( 2, 16, FALSE, dw );
	
	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetStockmanStockCodeFilter ( "" );
	m_FilterArray.SetDepartmentFilter( DEPARTMENTSET_ALL );
	BuildRecordList();
	DisplayScreen(0);			
	
	CPluImportStatsComplexDlg dlgInfo ( this );
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nPluAdded = ImportHandler.GetImportPluAdded();
	dlgInfo.m_nPluModified = ImportHandler.GetImportPluModified();
	dlgInfo.m_nStockAdded = ImportHandler.GetImportStockAdded();	
	dlgInfo.m_nStockModified = ImportHandler.GetImportStockModified();
	dlgInfo.m_nSupplierAdded = ImportHandler.GetImportSupplierAdded();
	dlgInfo.DoModal();

	return TRUE;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluExportEpos() 
{
	int nDbIdx = DataManager.GetActiveDbIdx();
	
	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );
	
	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	int nJob = DN_PLU;
	CString strTask = "Download Plu";

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		switch( PriceSetItemManager.GetPriceSetNumber() )
		{
		case 1:	
			nJob = DN_PLU_SET1;	
			strTask = "Download Price Set 1";
			break;

		case 2:	
			nJob = DN_PLU_SET2;	
			strTask = "Download Price Set 2";
			break;

		case 3:	
			nJob = DN_PLU_SET3;	
			strTask = "Download Price Set 3";
			break;
		}
	}

	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, strTask, this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( nJob, arrayLocIdx );

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( strTask );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluPrtPricelist() 
{
	if ( m_FilterArray.GetSize() == 0 )
		Prompter.Error ( "You must select some records before\nyou can print this report" );
	else
	{
		CPluCustomListDlg dlg ( m_FilterArray, PLUCUSTOMLIST_PRICE, 0, this );

		__int64 nPluFrom, nPluTo;
		m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
		::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
		::FormatPluNo( nPluTo, dlg.m_strPluTo );

		dlg.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluPrtProgram() 
{
	if ( m_FilterArray.GetSize() == 0 )
		Prompter.Error ( "You must select some records before\nyou can print this report" );
	else
	{
		CPluCustomListDlg dlg ( m_FilterArray, PLUCUSTOMLIST_PROGRAM, 0, this );
	
		__int64 nPluFrom, nPluTo;
		m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
		::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
		::FormatPluNo( nPluTo, dlg.m_strPluTo );

		dlg.DoModal();
	}
}

/**********************************************************************/
void CPluDatabaseDlgStockman::OnReportsCustom1(){ DoCustomListReport(1); } 
void CPluDatabaseDlgStockman::OnReportsCustom2(){ DoCustomListReport(2); } 
void CPluDatabaseDlgStockman::OnReportsCustom3(){ DoCustomListReport(3); } 
void CPluDatabaseDlgStockman::OnReportsCustom4(){ DoCustomListReport(4); } 
void CPluDatabaseDlgStockman::OnReportsCustom5(){ DoCustomListReport(5); } 
/**********************************************************************/

void CPluDatabaseDlgStockman::DoCustomListReport( int nRepNum )
{
	if ( m_FilterArray.GetSize() == 0 )
		Prompter.Error ( "You must select some records before\nyou can print this report" );
	else
	{
		CPluCustomListDlg dlg ( m_FilterArray, PLUCUSTOMLIST_FULL, nRepNum, this );

		__int64 nPluFrom, nPluTo;
		m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
		::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
		::FormatPluNo( nPluTo, dlg.m_strPluTo );

		dlg.DoModal();

		SetCustomListMenuNames();
		m_MenuChanger.BuildMenu( GetMenu() );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluPrtStocklinks() 
{
	if ( m_FilterArray.GetSize() == 0 )
		Prompter.Error ( "You must select some records before\nyou can print this report" );
	else
	{
		CPluPrtStockLinkDlg dlg ( m_FilterArray, this );

		__int64 nPluFrom, nPluTo;
		m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
		::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
		::FormatPluNo( nPluTo, dlg.m_strPluTo );

		dlg.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluPrtBarcodeLinks() 
{
	if ( m_FilterArray.GetSize() == 0 )
		Prompter.Error ( "You must select some records before\nyou can print this report" );
	else
	{
		CPluPrtBarcodeDlg dlg ( m_FilterArray, this );
		
		__int64 nPluFrom, nPluTo;
		m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
		::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
		::FormatPluNo( nPluTo, dlg.m_strPluTo );

		dlg.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex )
		{
			m_CacheCSV.ParseLine( m_FilterArray.GetColumnManagerDisplayLine ( m_ColumnManager, nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnPluEdit();	
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CPluDatabaseDlgStockman::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_PLUNO:
		if ( m_FilterArray.GetSize() > 0 )
			OnButtonPluNumber();
		break;

	case COLUMN_STOCKCODE:
		if ( m_FilterArray.GetSize() > 0 )
			OnButtonPluStockCode();
		break;

	case COLUMN_DESC:
	case COLUMN_EPOSTEXT:
	case COLUMN_DEPT:
		OnFilterPluRecords();
		break;

	case COLUMN_PLUSTATUS:
		OnFilterPluStatus();
		break;
	}	
	
	return 0L;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluEdit()
{
	int nIndex = m_listPlu.GetCurSel();

	if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
		EditPlu( nIndex, FALSE, TRUE );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnButtonStock() 
{
	if ( StockOptions.GetPluSimplifiedEditingFlag() == TRUE )
	{
		int nIndex = m_listPlu.GetCurSel();

		if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
			EditPlu( nIndex, FALSE, FALSE );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::EditPlu ( int nIndex, bool bNewStock, bool bAllowSimple )
{
	m_FilterArray.SetArrayIdx ( nIndex );

	int nStockIdx;
	bool bAllowSpin; 
	bool bSimpleEditMode; 

	if ( StockOptions.GetPluSimplifiedEditingFlag() == FALSE || PriceSetItemManager.IsPluMaster() == FALSE )
	{
		bAllowSpin = TRUE;
		bSimpleEditMode = FALSE;
	}
	else if ( bAllowSimple == FALSE )
	{
		bAllowSpin = FALSE;
		bSimpleEditMode = FALSE;
	}
	else
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( m_FilterArray.GetPluItemIndex(), PluRecord );
	
		if ( DataManager.Plu.IsRecipe ( PluRecord.GetPluNoInt() ) )
		{	
			bAllowSpin = FALSE;
			bSimpleEditMode = FALSE;
		}
		else
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );

			if ( strStockCode == "" )
			{
				bAllowSpin = FALSE;
				bSimpleEditMode = FALSE;
			}
			else 
			{
				bAllowSpin = DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx );
				bSimpleEditMode = bAllowSpin;
			}
		}		
	}
	
	if ( bSimpleEditMode == TRUE )
	{	
		CPropertySheetPluStockInfo info;
		info.m_bEnableSpin = TRUE;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nStockIdx = nStockIdx;
		info.m_bNewStock = bNewStock;
		info.m_bStockLevelTab = TRUE;
		info.m_nDefaultSuppNo = 0;
		info.m_bReadOnly = m_bReadOnly;
		info.m_bStockLevelTab = ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE );
		info.m_bApparentTab = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
		
		CPropertySheetPluStock propSheet ( info, this );
		propSheet.EnableStackedTabs ( FALSE );

		if ( propSheet.DoModal() == IDOK )
			propSheet.SavePluRecord();
		else
			propSheet.UndoChanges();
	}
	else
	{
		int nEditStock = ( PriceSetItemManager.IsPluMaster() == TRUE ) ? EDIT_STOCK_FULL : EDIT_STOCK_NONE;
		
		CPropertySheetPluStockmanInfo info;
		info.m_bEnableSpin = bAllowSpin;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nEditMode = nEditStock;
		info.m_bNewStock = bNewStock;
		info.m_bReadOnly = m_bReadOnly;
		info.m_bStockLevelTab = ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE );
		info.m_bApparentTab = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
		
		CPropertySheetPluStockman propSheet ( info, this );
		
		if ( propSheet.DoModal() == IDOK )
			propSheet.SaveRecord();
		else
			propSheet.UndoChanges();
	}

	DisplayScreen ( m_FilterArray.GetArrayIdx() );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnBarcodeNew() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	CBarcodeDlg dlg ( FALSE, this );
	dlg.m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	
	__int64 nPluNoFrom, nPluNoTo;
	m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluNoFrom, nPluNoTo );
	dlg.SetPluNoFrom( nPluNoFrom );
	dlg.SetPluNoTo( nPluNoTo );
	dlg.SetBarcodeNoFrom( 1 );
	dlg.SetBarcodeNoTo( Super.MaxBarcodeInt() );
	
	if ( dlg.DoModal() == IDOK )
	{
		CLabelListCSVArray BarcodeArray ( TRUE );

		CString strStockpointName = "";

		__int64 nPluNoFrom = dlg.GetPluNoFrom();
		__int64 nPluNoTo = dlg.GetPluNoTo();
		__int64 nBarcodeNoFrom = dlg.GetBarcodeNoFrom();
		__int64 nBarcodeNoTo = dlg.GetBarcodeNoTo();

		if ( dlg.m_nType == 0 )
			BarcodeArray.CreateAdhocList ( m_FilterArray, nPluNoFrom, nPluNoTo, nBarcodeNoFrom, nBarcodeNoTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_nItemLabels );
		else
		{
			BarcodeArray.CreateApparentList ( m_FilterArray, nPluNoFrom, nPluNoTo, nBarcodeNoFrom, nBarcodeNoTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_LocSelEntity );
			strStockpointName = dlg.m_LocSelEntity.GetEntityName();
		}

		CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, BarcodeArray, strStockpointName, this );
		dlgList.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnShelfNew() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	CBarcodeDlg dlg ( TRUE, this );
	dlg.m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	
	__int64 nPluNoFrom, nPluNoTo;
	m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluNoFrom, nPluNoTo );
	dlg.SetPluNoFrom( nPluNoFrom );
	dlg.SetPluNoTo( nPluNoTo );
	dlg.SetBarcodeNoFrom( 1 );
	dlg.SetBarcodeNoTo( Super.MaxBarcodeInt() );

	if ( dlg.DoModal() == IDOK )
	{
		CLabelListCSVArray ShelfArray( FALSE );

		CString strStockpointName = "";

		__int64 nPluNoFrom = dlg.GetPluNoFrom();
		__int64 nPluNoTo = dlg.GetPluNoTo();
		__int64 nBarcodeNoFrom = dlg.GetBarcodeNoFrom();
		__int64 nBarcodeNoTo = dlg.GetBarcodeNoTo();

		if ( dlg.m_nType == 0 )
			ShelfArray.CreateAdhocList ( m_FilterArray, nPluNoFrom, nPluNoTo, nBarcodeNoFrom, nBarcodeNoTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_nItemLabels );
		else
		{
			ShelfArray.CreateApparentList ( m_FilterArray, nPluNoFrom, nPluNoTo, nBarcodeNoFrom, nBarcodeNoTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_LocSelEntity );
			strStockpointName = dlg.m_LocSelEntity.GetEntityName();
		}

		CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, ShelfArray, strStockpointName, this );
		dlgList.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnBarcodeSaved() 
{
	CLabelListCSVArray BarcodeArray ( TRUE );
	BarcodeArray.RestoreSavedList();	
	CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, BarcodeArray, "", this );
	dlgList.DoModal();
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnShelfSaved() 
{
	CLabelListCSVArray ShelfArray ( FALSE );
	ShelfArray.RestoreSavedList();
	CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, ShelfArray, "", this );
	dlgList.DoModal();
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnCancel()
{
	m_bCanDoChangesTask = FALSE;
	m_bCanDeleteBarcode = FALSE;
	m_bCanDeleteStock = FALSE;

	if ( m_bReadOnly == TRUE )
	{
		EndDialog( IDCANCEL );
		return;
	}
		
	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );

	m_bCanDeleteBarcode = TRUE;
	m_bCanDeleteStock = TRUE;

	bool bPluModified = DataManager.Plu.IsModified();
	if ( FALSE == PriceSetItemManager.IsPluMaster() )
		bPluModified |= PriceSetItemManager.GetPluStatusModifiedFlag();

	if ( ( TRUE == bPluModified ) || ( DataManager.Stock.IsModified() == TRUE ) || ( DataManager.ResourceWrapperPlu.IsModified() == TRUE ) || ( DataManager.Barcode.IsModified() == TRUE ) || ( DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE ) )
	{
		int nChoice = Prompter.YesNoCancel ( "Plu database has been modified - save changes" );
		
		switch ( nChoice )
		{
		case IDYES:
			{
				DataManager.StockPlu.Invalidate();
				if ( PriceSetItemManager.IsPluMaster() == TRUE )
					WriteMasterDatabase();
				else
					WritePriceSetFile();

				COpeningStockAuditer OpeningStockAuditer;

				MessageLogger.LogSystem( 2, 7, FALSE, dw );
				m_bCanDoChangesTask = TRUE;
			}
			break;

		case IDNO:
			DataManager.ResourceWrapperPlu.UndoDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.UndoDatabaseChanges();
			MessageLogger.LogSystem( 2, 8, FALSE, dw );
			m_bCanDeleteStock = FALSE;
			m_bCanDeleteBarcode = FALSE;
			break;

		case IDCANCEL:
			return;
		}
	}
	else
		MessageLogger.LogSystem( 2, 9, FALSE );
		
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::WriteMasterDatabase()
{			
	CWaitCursor wait;
	
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
		Prompter.WriteError( info );
	else if ( DataManager.WriteBarcode( info ) == FALSE )
		Prompter.WriteError( info );
	else
	{
		DataManager.Plu.UpdateSalesManCostPrice();
		if ( DataManager.WritePlu( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
			SystemMapFull.WriteSystem( FALSE );
			CPosTrayTokenFiles::CreateSystemUpdateFile();
			CCustomExportManagerPlu CustomExport ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
			CKeyboardHelpers::RemovePluFile ( DataManager.GetActiveDbIdx() );
		}
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::WritePriceSetFile()
{
	PriceSetItemManager.WritePriceSetFile();
	DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
	DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
	CPriceSetExportInfo info;
	PriceSetItemManager.GetPriceSetExportInfo( info );
	CCustomExportManagerPriceSet CustomExport( info );
	CKeyboardHelpers::RemovePluFile ( DataManager.GetActiveDbIdx() );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::BuildRecordList()
{
	m_FilterArray.BuildRecordList();
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnFilterPluRecords()
{
	int nPluIdx = 0;
	__int64 nPluNo = 0;
	int nSelection = m_listPlu.GetCurSel();

	if ((nSelection >= 0) && (nSelection < m_FilterArray.GetSize()))
	{
		nPluIdx = m_FilterArray.GetPluItemIndexAt(nSelection);
		nPluNo = DataManager.Plu.GetPluNoInt(nPluIdx);
	}

	CPluDefineFilterDlg dlg(m_FilterArray, this);

	if (dlg.DoModal() == IDOK)
	{
		BuildRecordList();

		int nArrayIdx = 0;
		m_FilterArray.FindPluItemByNumber(nPluNo, nArrayIdx);
		DisplayScreen(nArrayIdx);
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnFilterPluStatus() 
{
	CLocalPluFilterDlg dlg ( m_FilterArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		BuildRecordList();
		DisplayScreen ( 0 );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnNoFilterPluRecords()
{
	int nPluIdx = 0;
	int nSelection = m_listPlu.GetCurSel();

	if ((nSelection >= 0) && (nSelection < m_FilterArray.GetSize()))
	{
		nPluIdx = m_FilterArray.GetPluItemIndexAt(nSelection);
	}

	m_FilterArray.SetDescriptionFilter("", 0, FALSE);
	m_FilterArray.SetStockmanStockCodeFilter("");
	m_FilterArray.SetDepartmentFilter(DEPARTMENTSET_ALL);
	m_FilterArray.SetAnalysisCategoryFilter(9999);

	{
		CKeyboardFilterSelectorLoc Loc;
		Loc.m_nNodeType = NODE_SYSTEM;
		Loc.m_nEntityIdx = 0;
		m_FilterArray.SetKeyboardFilter(Loc, 1);
	}

	m_FilterArray.SetStockmanShowUnlinkedPluFlag(TRUE);
	m_FilterArray.SetStockmanShowLinkedPluFlag(TRUE);
	m_FilterArray.SetStockmanShowRecipePluFlag(TRUE);
	m_FilterArray.SetShowActiveItemsFlag(TRUE);
	m_FilterArray.SetShowInactiveItemsFlag(TRUE);
	m_FilterArray.SetPluRangeFlag(FALSE);

	BuildRecordList();
	DisplayScreen(nPluIdx);
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnColumnEdit() 
{
	m_ColumnManager.RememberCurrentLine( &m_listPlu, this );
	CArray<CColumnStatusInfo,CColumnStatusInfo> arrayPos;
	m_ColumnManager.GetListColumnDetails( arrayPos );

	CColumnListDlg dlg( "Plu Database", arrayPos, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_ColumnManager.SetListColumnDetails( arrayPos );
		m_ColumnManager.RedrawList( &m_listPlu, this );
		m_ColumnManager.SaveCurrentLine( &m_listPlu, this );
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluDatabaseShelf() 
{
	PrintSingleLabel( FALSE );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::OnPluDatabaseProduct() 
{
	PrintSingleLabel( TRUE );
}

/**********************************************************************/

void CPluDatabaseDlgStockman::PrintSingleLabel( bool bProduct )
{
	int nIndex = m_listPlu.GetCurSel();
	if ( ( nIndex >= 0 ) && ( nIndex < m_FilterArray.GetSize() ) )
	{
		int nPluIdx = m_FilterArray.GetPluItemIndexAt( nIndex );
		
		CLabelListCSVArray arrayLabels( bProduct );
		arrayLabels.CreateSingleItemList( nPluIdx );

		if ( arrayLabels.GetSize() == 0 )
		{
			Prompter.Error ( "No valid barcode links were\nfound for this item" );
			return;
		}

		bool bPrompt = FALSE;
		if ( TRUE == bProduct )
			bPrompt = EcrmanOptions.GetLabelsProductPromptSingleFlag();
		else
			bPrompt = EcrmanOptions.GetLabelsShelfPromptSingleFlag();
		
		int nPos = 1;

		if ( TRUE == bPrompt )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			CPrintSingleLabelDlg dlg( bProduct, PluRecord, this );

			if ( dlg.DoModal() != IDOK )
				return;

			nPos = dlg.m_nPos;

			if ( dlg.m_nQty != 1 )	
			{
				for ( int n = 0; n < arrayLabels.GetSize(); n++ )
				{
					CLabelListCSVRecord record;
					arrayLabels.GetAt( n, record );
					record.SetQtyToPrint( dlg.m_nQty );
					arrayLabels.SetAt( n, record );
				}
			}
		}

		CLabelListCreator LabelListCreator( arrayLabels );
		int nResult = LabelListCreator.CreateLabelList();

		switch ( nResult )
		{
		case 1:	
			Prompter.Error ( "Unable to create temporary\nfile for Barcode Manager" );	
			break;

		case 2:
			Prompter.Error ( "No valid barcode links were\nfound for this item" );
			break;
		}

		if ( nResult == 0 )
		{
			CString strDataPath = "Labels.dat";
			GetDataProgramPath( strDataPath );
			
			CString strName = "";

			if ( TRUE == bProduct )
				strName = EcrmanOptions.GetLabelsProductNameSingle();
			else
				strName = EcrmanOptions.GetLabelsShelfNameSingle();

			CString strPos;
			strPos.Format( "%d", nPos );

			CString strPrint = "/p";
			
			CString strProgramPath = "";
			GetDataProgramPath( strProgramPath );

			CString strDevmodePath = "PRINTA_";
			GetPCPathClient( strDevmodePath );

			CProtect1 protect;
			SSspawnl(GetSafeOwner(this), P_WAIT, "SPOSBARCODEMANAGER.PGM", "SPOSBARCODEMANAGER.PGM", "1", protect.GetCode(), strDataPath, strName, strPos, strProgramPath, strPrint, PCOptionsClient.GetDefaultLabelPrinter(), strDevmodePath, NULL);
		}
	}
}

/**********************************************************************/

void CPluDatabaseDlgStockman::SortListInternal( int nType )
{
	__int64 nPluNo = 0;
	
	{
		int nIndex = m_listPlu.GetCurSel();
		if ( ( nIndex >= 0 ) && ( nIndex < m_FilterArray.GetSize() ) )
			nPluNo = DataManager.Plu.GetPluNoInt( m_FilterArray.GetPluItemIndexAt( nIndex ) );
	}

	m_FilterArray.ChangeSortOrder( nType, TRUE );
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	if ( nPluNo == 0 )
		DisplayScreen(0);
	else
	{
		int nSel;
		m_FilterArray.FindPluItemByNumber( nPluNo, nSel );
		DisplayScreen( nSel );
	}
}

/**********************************************************************/
