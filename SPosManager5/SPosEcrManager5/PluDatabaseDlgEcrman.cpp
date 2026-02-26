/**********************************************************************/
#include "..\CommonEcrStock\BarcodeDlg.h"
#include "..\CommonEcrStock\ColumnListDlg.h"
#include "..\CommonEcrStock\CommsBatchDlg.h"
#include "..\CommonEcrStock\CSVDbImportStatsDlg.h"
#include "..\CommonEcrStock\CSVDBImportTypeDlg.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "..\CommonEcrStock\DownloadDateDlg.h"
#include "..\CommonEcrStock\JobNumbers.h"
#include "..\CommonEcrStock\KeyboardFilterSelector.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "..\CommonEcrStock\KeyboardMap.h"
#include "..\CommonEcrStock\LabelListCreator.h"
#include "..\CommonEcrStock\LabelListCSVArray.h"
#include "..\CommonEcrStock\ListLabelsDlg.h"
#include "..\CommonEcrStock\LocalPluFilterDlg.h"
#include "..\CommonEcrStock\PluCustomListDlg.h"
#include "..\CommonEcrStock\PluDefineFilterDlg.h"
#include "..\CommonEcrStock\PluDeleteSingleDlg.h"
#include "..\CommonEcrStock\PluExportDlg.h"
#include "..\CommonEcrStock\PluPrtBarcodeDlg.h"
#include "..\CommonEcrStock\PluRangeDlg.h"
#include "..\CommonEcrStock\PluRangeCopyDlg.h"
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
#include "..\CommonEcrStock\SimpleInfoDlg.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\TaxImportMap.h"
/**********************************************************************/
#include "..\SPosEcrManager5\DatabaseImportHandlerEcrman.h"
#include "..\SPosEcrManager5\PluDeliveryDlg.h"
#include "..\SPosEcrManager5\PluNumberEcrStockDlg.h"
#include "..\SPosEcrManager5\PluRangeStockLevelsDlg.h"
#include "..\SPosEcrManager5\PropertySheetPluEcrman.h"
/**********************************************************************/
#include "Append.h"
#include "BarcodePluNoTable.h"
#include "DatabaseCSVArray.h"
#include "ExportDate.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "ImportDefines.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PCOptions.h"
#include "PluAddOrCopyInfo.h"
#include "PluDatabaseHeader.h"
#include "BarcodePluNoTable.h"
#include "PluRangeAddDlgEcrman.h"
#include "PluRangeModes.h"
#include "PosTrayTokenFiles.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PluDatabaseDlgEcrman.h"
/**********************************************************************/

CPluDatabaseDlgEcrman::CPluDatabaseDlgEcrman( int nDbIdx, CWnd* pParent )
	: CDialog(CPluDatabaseDlgEcrman::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_DATABASE ), 
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CPluDatabaseDlgEcrman)
	//}}AFX_DATA_INIT
	m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
	m_strDbName = dbDatabase.GetName( nDbIdx );
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;
	m_nCacheIndex = -1;
	m_bReadOnly = FALSE;
	m_bCanDoChangesTask = FALSE;
	m_bCanDeleteBarcode = FALSE;
	m_nPluSearchMode = NODE_PLU;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
	m_nNewPluDeptNo = 0;
}

/**********************************************************************/

CPluDatabaseDlgEcrman::~CPluDatabaseDlgEcrman()
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDatabaseDlgEcrman)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDatabaseDlgEcrman, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CPluDatabaseDlgEcrman)
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
	ON_COMMAND(IDM_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_PLU_PRT_BARCODELINKS, OnPluPrtBarcodeLinks)
	ON_COMMAND(IDM_BARCODE_NEW, OnBarcodeNew)
	ON_COMMAND(IDM_SHELF_NEW, OnShelfNew)
	ON_COMMAND(IDM_BARCODE_SAVED, OnBarcodeSaved)
	ON_COMMAND(IDM_SHELF_SAVED, OnShelfSaved)
	ON_COMMAND(IDM_RANGE_STOCKLEVELS, OnRangeStockLevels)
	ON_COMMAND(IDM_IMPORT_DELIVERY, OnImportDelivery)
	ON_COMMAND(IDM_RANGE_MODIFIER_PRICES, OnPluRangeModifierPrices)
	ON_COMMAND(IDM_RANGE_MODIFIER_SETTINGS, OnPluRangeModifierSettings)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	ON_COMMAND(IDM_PLU_DATABASE_DELIVER, OnPluDatabaseDeliver)
	ON_COMMAND(IDM_PLU_EXIT, OnCancel)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnPluEdit)
	ON_COMMAND(IDM_PLU_EXPORT_EPOS, OnPluExportEpos)
	ON_COMMAND(IDM_REPORTS_CUSTOM1, OnReportsCustom1)
	ON_COMMAND(IDM_REPORTS_CUSTOM2, OnReportsCustom2)
	ON_COMMAND(IDM_REPORTS_CUSTOM3, OnReportsCustom3)
	ON_COMMAND(IDM_REPORTS_CUSTOM4, OnReportsCustom4)
	ON_COMMAND(IDM_REPORTS_CUSTOM5, OnReportsCustom5)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_COMMAND(IDM_PLU_DATABASE_SHELF, OnPluDatabaseShelf)
	ON_COMMAND(IDM_PLU_DATABASE_PRODUCT, OnPluDatabaseProduct)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDatabaseDlgEcrman::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);

	m_listPlu.SubclassDlgItem(IDC_LIST, this);
	m_listPlu.EnableColour();

	if (PriceSetItemManager.IsPluMaster())
		m_ColumnManager.Reset(TableNames::Plu);
	else
		m_ColumnManager.Reset(TableNames::PriceSet);

	m_ColumnManager.AddColumn(ColumnNames::PluNo, "Plu Number", LVCFMT_LEFT, 115, 115, COLUMN_STATUS_LOCKED);
	m_ColumnManager.AddColumn(ColumnNames::Desc, "Description", LVCFMT_LEFT, 120, 170, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::EPOSText, "EPOS Text", LVCFMT_LEFT, 120, 170, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Dept, "Dept", LVCFMT_LEFT, 40, 40, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Price1, "Price1", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Price2, "Price2", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Price3, "Price3", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Price4, "Price4", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN);

	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		m_ColumnManager.AddColumn(ColumnNames::Points, "Points", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN);
	}

	m_ColumnManager.AddColumn(ColumnNames::PluType, "Type", LVCFMT_LEFT, 60, 60, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Tax, EcrmanOptions.GetSalesTaxName(), LVCFMT_LEFT, 40, 40, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Printers, "Printers", LVCFMT_LEFT, 55, 70, COLUMN_STATUS_HIDDEN);

	if (!PriceSetItemManager.IsPluMaster())
		m_ColumnManager.AddColumn(ColumnNames::PluStatus, "Status", LVCFMT_LEFT, 70, 70, COLUMN_STATUS_SHOWN);

	else if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		m_ColumnManager.AddColumn(ColumnNames::StockCode, "Supplier Ref", LVCFMT_LEFT, 120, 120, COLUMN_STATUS_HIDDEN);
		m_ColumnManager.AddColumn(ColumnNames::Apparent, "Stock", LVCFMT_RIGHT, 70, 70, COLUMN_STATUS_SHOWN);
	}

	m_ColumnManager.AddColumn(ColumnNames::Cost, "Cost", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.LoadSettings();

	m_listPlu.SetItemCountEx(DataManager.Plu.GetSize());

	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList(&m_listPlu, this);
	CenterWindow();

	m_MenuChanger.EatMenuTopLevel(GetMenu());

	if (EcrmanOptions.GetPluCSVManualImportFlag() == FALSE)
		m_MenuChanger.KillItem(IDM_PLU_IMPORT_CSV);

	if (EcrmanOptions.GetPluCSVManualExportFlag() == FALSE)
		m_MenuChanger.KillItem(IDM_PLU_EXPORT_CSV);

	if (PasswordArray.GetEnable(PasswordTicks::PluDeliverStock) == FALSE)
		m_MenuChanger.EnableItem(IDM_IMPORT_DELIVERY, FALSE);

	if (PriceSetItemManager.IsPluMaster() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_PLU_DATABASE_ADD);
		m_MenuChanger.KillItem(IDM_PLU_DATABASE_COPY);
		m_MenuChanger.KillItem(IDM_PLU_DELETE_CURRENT);
		m_MenuChanger.KillItem(IDM_RANGE_PROFIT);
		m_MenuChanger.KillItem(IDM_PLU_RANGE_ADD);
		m_MenuChanger.KillItem(IDM_PLU_RANGE_DELETE);
		m_MenuChanger.KillItem(IDM_IMPORT_DELIVERY);

		if (CPluRangePriceDlg::IsRelevant() == FALSE)
			m_MenuChanger.KillItem(IDM_RANGE_PRICE);

		if (CPluRangePriceModifierDlg::IsRelevant() == FALSE)
			m_MenuChanger.KillItem(IDM_RANGE_MODIFIER_PRICES);

		if (CPluRangeProgramDlg::IsRelevant() == FALSE)
			m_MenuChanger.KillItem(IDM_RANGE_SETTINGS);

		if (CPluRangeProgramModifierDlg::IsRelevant() == FALSE)
			m_MenuChanger.KillItem(IDM_RANGE_MODIFIER_SETTINGS);
	}
	else
	{
		m_MenuChanger.KillItem(IDM_RANGE_STATUS);
		m_MenuChanger.KillItem(IDM_FILTER_STATUS);
	}

	if (Sysset.IsBarmanSystem() == FALSE)
	{
		m_MenuChanger.KillParent(IDM_BARCODE_NEW);
		m_MenuChanger.KillParent(IDM_SHELF_NEW);
	}

	if (PasswordArray.GetEnable(PasswordTicks::HousekeepingPluApparent) == FALSE || PasswordArray.GetEnable(PasswordTicks::ReportsPluApparent) == FALSE)
		m_MenuChanger.EnableItem(IDM_RANGE_STOCKLEVELS, FALSE);

	if (EcrmanOptions.GetFeaturesPluStockFlag() == FALSE)
	{
		m_MenuChanger.KillItem(IDM_IMPORT_DELIVERY);
		m_MenuChanger.KillItem(IDM_RANGE_STOCKLEVELS);
	}

	if (EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE)
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		CString strFile = dbDatabase.GetFilePathDataPluStockAuditTemp(nDbIdx);
		CFileRemover FileRemover(strFile);
	}

	if (PasswordArray.GetEnable(PasswordTicks::CommsDownloads) == FALSE)
		m_MenuChanger.KillItem(IDM_PLU_EXPORT_EPOS);

	if (m_bReadOnly == TRUE)
	{
		m_MenuChanger.KillItem(IDM_PLU_IMPORT_CSV);
		//Export flag requires read/write access
		m_MenuChanger.KillItem(IDM_PLU_EXPORT_CSV);
		m_MenuChanger.KillItem(IDM_IMPORT_DELIVERY);
		m_MenuChanger.KillItem(IDM_PLU_DATABASE_ADD);
		m_MenuChanger.KillItem(IDM_PLU_DATABASE_COPY);
		m_MenuChanger.KillItem(IDM_PLU_DELETE_CURRENT);
		m_MenuChanger.KillParent(IDM_PLU_RANGE_DELETE);
		m_MenuChanger.SetItemText(IDM_PLU_DATABASE_EDIT, "View");
	}

	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		m_MenuChanger.KillItem(IDM_REPORTS_CUSTOM1);
		m_MenuChanger.KillItem(IDM_REPORTS_CUSTOM2);
		m_MenuChanger.KillItem(IDM_REPORTS_CUSTOM3);
		m_MenuChanger.KillItem(IDM_REPORTS_CUSTOM4);
		m_MenuChanger.KillItem(IDM_REPORTS_CUSTOM5);
	}

	if (SysInfo.GetMaxBasePluLen() == 0)
	{
		m_MenuChanger.KillItem(IDM_RANGE_MODIFIER_PRICES);
		m_MenuChanger.KillItem(IDM_RANGE_MODIFIER_SETTINGS);
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
	else if ( EcrmanOptions.GetPluInactiveAutoHideFlag() == TRUE )
	{
		m_FilterArray.SetShowInactiveItemsFlag(FALSE);
		BuildRecordList();
	}

	SetCustomListMenuNames();
	m_MenuChanger.BuildMenu(GetMenu());
	DrawMenuBar();

	DisplayScreen ( 0 );

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	if ( PriceSetItemManager.IsPluMaster() == FALSE ) dw.Add( PriceSetItemManager.GetPriceSetNumber() );
	MessageLogger.LogSystem( 2, 1, TRUE, dw );

	return TRUE;  
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::SetCustomListMenuNames()
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

void CPluDatabaseDlgEcrman::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listPlu.MoveWindow ( 0, 0, cx, cy );
	}
}

/**********************************************************************/

bool CPluDatabaseDlgEcrman::ContextMenuSort( CPoint& point )
{
	HWND hWnd = HWND( m_listPlu );

	HDHITTESTINFO hitTest{};
	hitTest.pt.x = point.x;
	hitTest.pt.y = point.y;

	::ScreenToClient( hWnd, &hitTest.pt );

	CHeaderCtrl* headerCtrl = m_listPlu.GetHeaderCtrl();
	int nIndex = headerCtrl -> SendMessage( HDM_HITTEST, 0, (LPARAM) &hitTest );

	int nColumnType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nColumnType )
	{
	case COLUMN_PLUNO:
		SortListInternal( PLUFILTER_SORT_FORWARD_PLUNO );
		return TRUE;
	
	case COLUMN_EPOSTEXT:
		SortListInternal(PLUFILTER_SORT_FORWARD_EPOSTEXT);
		return TRUE;

	case COLUMN_DESC:
		SortListInternal( PLUFILTER_SORT_FORWARD_TEXT );
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

void CPluDatabaseDlgEcrman::OnContextMenu(CWnd*, CPoint point)
{
	if ( ContextMenuSort( point ) == TRUE )
		return;

	if( m_bReadOnly == TRUE )
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

	bool bCanDeliver = TRUE;
	int nSel = m_listPlu.GetCurSel();

	int nPluIdx = m_FilterArray.GetPluItemIndexAt( nSel );
		
	if ( EcrmanOptions.GetFeaturesPluStockFlag() == FALSE )
		bCanDeliver = FALSE;
	else if ( PasswordArray.GetEnable( PasswordTicks::ReportsPluApparent ) == FALSE )
		bCanDeliver = FALSE;
	else if ( PriceSetItemManager.IsPluMaster() == FALSE )
		bCanDeliver = FALSE;
	else if ( nSel < 0 || nSel >= m_FilterArray.GetSize() )
		bCanDeliver = FALSE;
	else
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		bCanDeliver = ( PluRecord.GetNonStockExternalFlag() == FALSE );
	}

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuPopup( pPopup );
	
	if ( bCanDeliver == FALSE )
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

void CPluDatabaseDlgEcrman::OnButtonPluNumber()
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

void CPluDatabaseDlgEcrman::OnButtonSuppRef()
{
	CFindEcrmanSuppRefDlg dlg ( this );
	
	if ( dlg.DoModal() == TRUE )
	{
		bool bFound = FALSE;
		int nSize = m_FilterArray.GetSize();

		CString strSuppRefToFind = dlg.m_strName;
		strSuppRefToFind.MakeUpper();

		StatusProgress.Lock( TRUE, "Finding Supplier Reference" );

		for ( int nFilterIdx = 0; nFilterIdx < m_FilterArray.GetSize(); nFilterIdx++ )
		{
			int nPluIdx = m_FilterArray.GetPluItemIndexAt( nFilterIdx );
	
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
			CString strSuppRefPlu = PluRecord.GetSupplierRef();
			strSuppRefPlu.MakeUpper();

			if ( strSuppRefToFind == strSuppRefPlu )
			{
				m_listPlu.SetCurSel( nFilterIdx );
				bFound = TRUE;
			}
		}

		StatusProgress.Unlock();
		if ( bFound == FALSE )
		{
			CString strMsg;
			strMsg.Format ( "Cannot find supplier reference %s", (const char*) strSuppRefToFind );
			Prompter.Error ( strMsg );
		}		
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluRangeDelete() 
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
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( 0 );
		CString strPlu = DataManager.Plu.GetPluNoString ( nPluIdx );
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

void CPluDatabaseDlgEcrman::OnPluRangeAdd() 
{
	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
		m_nNewPluDeptNo = m_FilterArray.GetDepartmentFilter();

	AddPluRange ( FALSE );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluDatabaseCopy() 
{
	AddPluRange ( TRUE );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::AddPluRange( bool bCopy ) 
{
	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );

	int nOldPluIdx = -1;
	
	if ( bCopy )
	{
		int nFilterIdx = m_listPlu.GetCurSel();
		if ( nFilterIdx >= 0 && nFilterIdx < m_FilterArray.GetSize() )
		{
			nOldPluIdx = m_FilterArray.GetPluItemIndexAt( nFilterIdx );
			if ( nOldPluIdx < 0 || nOldPluIdx >= DataManager.Plu.GetSize() )
				return;
		}
	}

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = 0;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	PluInfo.m_nSourcePluIdx = nOldPluIdx;

	if ( TRUE == bCopy )
	{
		CPluRangeCopyDlg dlgPluCopy( PluInfo, this );
		if ( dlgPluCopy.DoModal() != IDOK )
			return;
	}
	else
	{
		CPluRangeAddDlgEcrman dlgPluRange ( PluInfo, this );
		if ( dlgPluRange.DoModalWrapper() != IDOK )
			return;
	}
	
	if ( PluInfo.NewPluNoAsInt64() == 0 || PluInfo.m_lNewPluCopyCount == 0 )
		return;

	if ( PluInfo.MaxNewPluLen() > Super.MaxPluLen() )
	{	
		Prompter.Error ( "The requested range of Plu numbers is not valid" );
		return;
	}

	CString strRawPluNo = PluInfo.m_strNewPluNo;
	::TrimSpaces ( strRawPluNo, FALSE );
	CString strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );

	CArray<__int64,__int64> arrayNewNumbers;

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
		if ( DataManager.Plu.FindPluByNumber ( nNewPluNo, nArrayIdx ) == FALSE )
			arrayNewNumbers.Add ( nNewPluNo );

		GetNextPluNo ( strRawPluNo );
		strNewPluNo = BarcodePluNoTable.ConvertPlu( strRawPluNo );
	}
	StatusProgress.Unlock();

	int nFound = PluInfo.m_lNewPluCopyCount - arrayNewNumbers.GetSize();

	if ( nFound >= PluInfo.m_lNewPluCopyCount )
	{
		Prompter.Error ( "None of the Plu numbers in this range are available." );
		return;
	}
	
	if ( nFound != 0 )
	{
		CString strMsg;

		if (nFound == 1)
		{
			strMsg = "1 of the PLU numbers in this range is not available.\n\n";
		}
		else
		{
			strMsg.Format("%d of the PLU numbers in this range are not available.\n\n", nFound);
		}

		strMsg += "Do you wish to create new items for the remaining numbers.";
		
		if ( Prompter.YesNo ( strMsg, FALSE ) != IDYES )
			return;
	}

	CPluCSVRecord PluRecord;

	bool bUsedSpecimen = FALSE;

	if ( PLURANGE_MODE_ADD == PluInfo.m_nPluRangeMode )
	{
		PluRecord.SetModifiedDateTimePlu();

		m_nNewPluDeptNo = PluInfo.m_nNewPluDeptNo;

		if ( DataManager.Plu.CopyDefaultPlu( m_nNewPluDeptNo, PluRecord ) == TRUE )
			bUsedSpecimen = TRUE;

		PluRecord.SetBaseDeptNo( m_nNewPluDeptNo );
	}
	else
	{
		//RETAIN OLD DOWNLOAD DATE IF WE ARE COPYING THE RECORD
		DataManager.Plu.GetAt ( nOldPluIdx, PluRecord );
		PluRecord.SetApparentLevel(0.0);
		PluRecord.SetModifiedDateTimePlu();
	}

	StatusProgress.Lock( TRUE, "Creating PLU items" );
	int nSize = arrayNewNumbers.GetSize();
	
	__int64 nFirstNewPluNo = 0;
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nSize );

		__int64 nNewPluNo = arrayNewNumbers.GetAt( nIndex );
		PluRecord.SetPluNo( nNewPluNo );
		PluRecord.SetSupplierNo( PluInfo.m_nNewPluSuppNo );
		
		if ( ( PLURANGE_MODE_ADD == PluInfo.m_nPluRangeMode ) && ( FALSE == bUsedSpecimen ) )
			PluRecord.SetDefaultText();
		
		m_FilterArray.InsertPluRecord ( PluRecord, TRUE );
		
		if (nFirstNewPluNo == 0 )
			nFirstNewPluNo = nNewPluNo;
	}
	StatusProgress.Unlock();

	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );

	int nArrayIdx;
	if ( m_FilterArray.FindPluItemByNumber ( nFirstNewPluNo, nArrayIdx ) == FALSE )
		nArrayIdx = 0;

	dw.Add( DataManager.Plu.GetSize() );

	int nType = ( bCopy ) ? 15 : 14;
	MessageLogger.LogSystem( 2, nType, FALSE, dw );

	DisplayScreen ( nArrayIdx );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::DisplayScreen ( int nIndex )
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
	strText.Format ( "Plu Database (%s)", 
		(const char*) csvDbName.GetLine() );

	if( m_bReadOnly == TRUE ) strText += " (READ ONLY)";
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nFilterSize ) )
		m_listPlu.SetCurSel ( nIndex );
	else
		m_listPlu.SetCurSel ( nFilterSize - 1 );

	m_listPlu.Invalidate();
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluDeleteCurrent() 
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

void CPluDatabaseDlgEcrman::OnPluDatabaseAdd() 
{
	if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
	{
		m_nNewPluDeptNo = m_FilterArray.GetDepartmentFilter();
	}

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = 0;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	
	CPluNumberEcrStockDlg dlg ( PluInfo, this );
	
	while ( dlg.DoModalWrapper() == IDOK )
	{
		CString strEnteredPluNo = PluInfo.m_strNewPluNo;
		CString strConvertedPluNo = BarcodePluNoTable.ConvertPlu ( strEnteredPluNo );
		__int64 nConvertedPluNo = _atoi64( strConvertedPluNo );

		int nIndex;
		if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
		{
			int nMaxBaseLen = SysInfo.GetMaxBasePluLen();
			if ( nMaxBaseLen != 0 )
			{
				CString strTestPluNo = "";
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
			NewRecord.SetSupplierNo( PluInfo.m_nNewPluSuppNo );
		
			int nInsertionPoint = m_FilterArray.InsertPluRecord ( NewRecord, TRUE );
			m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
			DisplayScreen ( nInsertionPoint );
			EditPlu ( nInsertionPoint );

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

			if ( Prompter.YesNo ( strMsg, TRUE ) != IDYES )
				continue;

			int nProgress = 0;
			bool bGotFreeNumber = FALSE;

			StatusProgress.Lock( TRUE, "Finding next PLU number" );
			do
			{
				StatusProgress.SetPos ( nProgress++, DataManager.Plu.GetSize() );

				if ( GetNextPluNo ( strEnteredPluNo ) == FALSE )
					break;

				__int64 nConvertedPluNo = _atoi64( BarcodePluNoTable.ConvertPlu( strEnteredPluNo ) );
				if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
				{
					bGotFreeNumber = TRUE;
					break;
				}				
			}
			while ( TRUE );
			StatusProgress.Unlock();

			if ( bGotFreeNumber == FALSE )
				Prompter.Error ( "Unable to find another free Plu number" );
			else
				PluInfo.m_strNewPluNo = strEnteredPluNo;
		}
	}
}

/**********************************************************************/

bool CPluDatabaseDlgEcrman::GetNextPluNo( CString& strPluNo )
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

void CPluDatabaseDlgEcrman::OnPluRangePrices() 
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

void CPluDatabaseDlgEcrman::OnPluRangeModifierPrices() 
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

void CPluDatabaseDlgEcrman::OnPluRangeModifierSettings() 
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

void CPluDatabaseDlgEcrman::OnPluRangeProfit() 
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

void CPluDatabaseDlgEcrman::OnPluRangeSettings() 
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

void CPluDatabaseDlgEcrman::OnRangeStatus() 
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

void CPluDatabaseDlgEcrman::OnRangeStockLevels() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	int nCurrent = m_listPlu.GetCurSel();
	CPluRangeStockLevelsDlg dlg ( m_FilterArray, this );

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( nCurrent, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, dlg.m_strPluFrom );
	::FormatPluNo( nPluTo, dlg.m_strPluTo );

	dlg.DoModal();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluExportCsv() 
{
	TaxImportMap.Read();
	ExportDate.Read();

	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	CPluExportDlg dlgFields( m_FilterArray, m_listPlu.GetCurSel(), this);
	if ( dlgFields.DoModal() == IDCANCEL )
		return;

	__int64 nPluFrom = _atoi64( dlgFields.m_strPluFrom );
	__int64 nPluTo = _atoi64( dlgFields.m_strPluTo );

	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	if ( nRange == 0 )
	{
		Prompter.Error ( "There are no Plus in the selected range" );
		return;
	}

	CString strPathName = ExportDate.GetLastPluFile();

	CString strDatabaseHeader, strExportHeaderToUse;
	if ( DealerFlags.GetLegacyPluExportFlag() == FALSE )
	{
		strDatabaseHeader = PluDatabaseHeader.GetDatabaseHeader();
		strExportHeaderToUse = dlgFields.GetPluHeaderLine();
	}
	else
	{
		strDatabaseHeader = PluDatabaseHeader.GetLegacyHeader();
		strExportHeaderToUse = PluDatabaseHeader.GetLegacyHeader( dlgFields.GetPluHeaderLine() );
	}

	CSSExportFilterFile fileExportPlu ( strDatabaseHeader, strExportHeaderToUse );
	if ( fileExportPlu.Open ( strPathName, "wb" ) == FALSE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to create export file %s", (const char*) strPathName );
		Prompter.Error ( strMsg );
		return;
	}

	int nPluExported = 0;

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock( TRUE, "Exporting PLU data" );
	for ( int nProgress = 0; nProgress < nRange; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nRange );	
		
		CPluCSVRecord PluRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nProgress );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		//IGNORE RECORDS THAT HAVE NOT CHANGED
		if ( ( dlgFields.m_bChangesOnly == TRUE ) && ( PluRecord.GetExport() == FALSE ) )
			continue;

		//EXPORT THE PLU RECORD
		CCSV csv( ',', '"', FALSE, FALSE, TRUE);
		PluRecord.CreateExportLine( csv );
		fileExportPlu.WriteData ( csv.GetLine() );
		nPluExported++;

		PluRecord.SetExport ( FALSE );
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

void CPluDatabaseDlgEcrman::OnPluImportCsv() 
{
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

bool CPluDatabaseDlgEcrman::HandleCSVImportForMaster( CString& strFilename )
{
	CCSVDBImportTypeDlg dlgMethod ( NODE_PLU, this );
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;

	if ( dlgMethod.DoModal() != IDOK )
		return FALSE;

	CDatabaseImportHandlerEcrman ImportHandler;
	ImportHandler.SetPluImportMethod ( dlgMethod.m_nImportMethod );		
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
	MessageLogger.LogSystem( 2, 3, FALSE, dw );
	
	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetDepartmentFilter( DEPARTMENTSET_ALL );
	BuildRecordList();
	DisplayScreen(0);			
		
	CCSVDBImportStatsDlg dlgInfo ( NODE_PLU, this );
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nRecordsAdded = ImportHandler.GetImportPluAdded();
	dlgInfo.m_nRecordsModified = ImportHandler.GetImportPluModified();
	dlgInfo.DoModal();

	return TRUE;
}

/**********************************************************************/

bool CPluDatabaseDlgEcrman::HandleCSVImportForPriceSet(CString& strFilename)
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

void CPluDatabaseDlgEcrman::OnPluExportEpos() 
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

void CPluDatabaseDlgEcrman::OnImportDelivery() 
{
	CString strTitle = "Import Plu Deliveries";
	CFileDialog dlgFilename ( TRUE, "csv", "delivery", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	
	if ( dlgFilename.DoModal() != IDOK )
		return;

	CString strFilename = dlgFilename.GetPathName();

	CSSFile fileImport;
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )
	{
		Prompter.Error ( "Unable to open import file." );
		return;
	}

	CSSTime timeNow;
	CSSDate dateNow;
	timeNow.SetCurrentTime();
	dateNow.SetCurrentDate();

	bool bAudit = FALSE;
	
	CSSFile fileAudit;
	if ( EcrmanOptions.GetFeaturesPluAuditFlag() )
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		bAudit = ( fileAudit.Open ( dbDatabase.GetFilePathDataPluStockAuditTemp( nDbIdx ), "ab" ) );
	}

	int nLines = 0;
	int nValid = 0;

	CString strBuffer;
	StatusProgress.Lock( TRUE, "Importing deliveries" );
	while ( fileImport.ReadString( strBuffer ) )
	{
		nLines++;

		StatusProgress.SetPos( fileImport );

		CCSV csvIn ( strBuffer );
		__int64 nPluNo = _atoi64 ( csvIn.GetString(0) );
		double dQty = csvIn.GetDouble(1);

		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
			continue;

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		if ( PluRecord.GetNonStockExternalFlag() == TRUE )
			continue;

		PluRecord.SetApparentLevel ( PluRecord.GetApparentLevel() + dQty );
		DataManager.Plu.SetAt( nPluIdx, PluRecord );
		nValid++;

		if ( bAudit )
		{
			CString strPluNo = PluRecord.GetPluNoString();
			::TrimSpaces( strPluNo, TRUE );
			
			CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
			csv.Add ( dateNow.GetCSVDate() );
			csv.Add ( timeNow.GetCSVTime() );
			csv.Add ( strPluNo );
			csv.Add ( PluRecord.GetReportText() );
			csv.Add ( PluRecord.GetBaseDeptNo() );
			csv.Add ( 0 );
			csv.Add ( dQty, SysInfo.GetDPQty() );
			csv.Add ( PluRecord.GetSupplierNo() );
			fileAudit.WriteLine( csv.GetLine() );
		}
	}
	StatusProgress.Unlock();
	fileImport.Close();
	CFileRemover FileRemover ( strFilename );

	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetDepartmentFilter( DEPARTMENTSET_ALL );
	BuildRecordList();
	DisplayScreen(0);	

	CSimpleInfoDlg dlgInfo( "Import Plu Deliveries", "Number of lines in import file", "Number of valid lines imported", nLines, nValid, this );
	dlgInfo.DoModal();
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluPrtPricelist() 
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

void CPluDatabaseDlgEcrman::OnPluPrtProgram() 
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
void CPluDatabaseDlgEcrman::OnReportsCustom1(){ DoCustomListReport(1); } 
void CPluDatabaseDlgEcrman::OnReportsCustom2(){ DoCustomListReport(2); } 
void CPluDatabaseDlgEcrman::OnReportsCustom3(){ DoCustomListReport(3); } 
void CPluDatabaseDlgEcrman::OnReportsCustom4(){ DoCustomListReport(4); } 
void CPluDatabaseDlgEcrman::OnReportsCustom5(){ DoCustomListReport(5); } 
/**********************************************************************/

void CPluDatabaseDlgEcrman::DoCustomListReport( int nRepNum )
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
		m_MenuChanger.BuildMenu ( GetMenu() );
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluPrtBarcodeLinks() 
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

void CPluDatabaseDlgEcrman::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

		if ( m_FilterArray.GetEcrmanNegativeStockFlag() )
			m_listPlu.SetColour( nIndex, 0x0C );
		else
			m_listPlu.SetColour( nIndex, 0x00 );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnPluEdit();	
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CPluDatabaseDlgEcrman::OnUserButton( WPARAM wIndex, LPARAM lParam )
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
			OnButtonSuppRef();
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

void CPluDatabaseDlgEcrman::OnPluEdit()
{
	int nIndex = m_listPlu.GetCurSel();

	if ( (nIndex >= 0) && (nIndex < m_FilterArray.GetSize()))
	{
		EditPlu(nIndex);
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::EditPlu ( int nIndex )
{
	m_FilterArray.SetArrayIdx ( nIndex );
	CPropertySheetPluEcrman propSheet ( IDC_SPIN, &m_FilterArray, m_bReadOnly, this );
		
	if (propSheet.DoModal() == IDOK)
	{
		propSheet.SaveRecord();
	}
	else
	{
		propSheet.UndoChanges();
	}

	DisplayScreen ( m_FilterArray.GetArrayIdx() );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnBarcodeNew() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	CBarcodeDlg dlg ( FALSE, this );
	dlg.m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	
	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
	dlg.SetPluNoFrom( nPluFrom );
	dlg.SetPluNoTo( nPluTo );
	dlg.SetBarcodeNoFrom( 1 );
	dlg.SetBarcodeNoTo( Super.MaxBarcodeInt() );
	
	if ( dlg.DoModal() == IDOK )
	{
		CLabelListCSVArray BarcodeArray ( TRUE );
		
		CString strStockpointName = "";

		__int64 nPluFrom = dlg.GetPluNoFrom();
		__int64 nPluTo = dlg.GetPluNoTo();
		__int64 nBarcodeFrom = dlg.GetBarcodeNoFrom();
		__int64 nBarcodeTo = dlg.GetBarcodeNoTo();

		if ( dlg.m_nType == 0 )
			BarcodeArray.CreateAdhocList ( m_FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_nItemLabels );
		else
		{
			BarcodeArray.CreateApparentList ( m_FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_LocSelEntity );
			strStockpointName = dlg.m_LocSelEntity.GetEntityName();
		}

		CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, BarcodeArray, strStockpointName, this );
		dlgList.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnShelfNew() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	CBarcodeDlg dlg ( TRUE, this );
	dlg.m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( m_listPlu.GetCurSel(), nPluFrom, nPluTo );
	dlg.SetPluNoFrom( nPluFrom );
	dlg.SetPluNoTo( nPluTo );
	dlg.SetBarcodeNoFrom( 1 );
	dlg.SetBarcodeNoTo( Super.MaxBarcodeInt() );
	
	if ( dlg.DoModal() == IDOK )
	{
		CLabelListCSVArray ShelfArray( FALSE );
		
		CString strStockpointName = "";

		__int64 nPluFrom = dlg.GetPluNoFrom();
		__int64 nPluTo = dlg.GetPluNoTo();
		__int64 nBarcodeFrom = dlg.GetBarcodeNoFrom();
		__int64 nBarcodeTo = dlg.GetBarcodeNoTo();

		if ( dlg.m_nType == 0 )
			ShelfArray.CreateAdhocList ( m_FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_nItemLabels );
		else
		{
			ShelfArray.CreateApparentList ( m_FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, dlg.m_nDepartmentFilter, 0, "", dlg.m_LocSelEntity );
			strStockpointName = dlg.m_LocSelEntity.GetEntityName();
		}
		
		CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, ShelfArray, strStockpointName, this );
		dlgList.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnBarcodeSaved() 
{
	CLabelListCSVArray BarcodeArray ( TRUE );
	BarcodeArray.RestoreSavedList();	
	CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, BarcodeArray, "", this );
	dlgList.DoModal();
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnShelfSaved() 
{
	CLabelListCSVArray ShelfArray ( FALSE );
	ShelfArray.RestoreSavedList();
	CListLabelsDlg dlgList ( TRUE, m_bReadOnly, FALSE, ShelfArray, "", this );
	dlgList.DoModal();
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnCancel()
{
	m_bCanDoChangesTask = FALSE;
	m_bCanDeleteBarcode = FALSE;

	if ( m_bReadOnly == TRUE )
	{
		EndDialog( IDCANCEL );
		return;
	}

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );

	m_bCanDeleteBarcode = TRUE;

	bool bPluModified = DataManager.Plu.IsModified();
	if ( FALSE == PriceSetItemManager.IsPluMaster() )
		bPluModified |= PriceSetItemManager.GetPluStatusModifiedFlag();

	if ( ( TRUE == bPluModified ) || ( DataManager.Barcode.IsModified() == TRUE  ) || ( DataManager.ResourceWrapperPlu.IsModified() == TRUE ) || ( DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE ) )
	{
		int nChoice = Prompter.YesNoCancel ( "Plu database has been modified - save changes" );
		
		switch ( nChoice )
		{
		case IDYES:
			
			if ( PriceSetItemManager.IsPluMaster() == TRUE )
				WriteMasterDatabase();
			else
				WritePriceSetFile();

			MessageLogger.LogSystem( 2, 7, FALSE, dw );
			m_bCanDoChangesTask = TRUE;
			break;

		case IDNO:
			DataManager.ResourceWrapperPlu.UndoDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.UndoDatabaseChanges();
			MessageLogger.LogSystem( 2, 8, FALSE, dw );
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

void CPluDatabaseDlgEcrman::WriteMasterDatabase()
{
	CWaitCursor wait;
	
	CDataManagerInfo info;
	if ( DataManager.WritePlu( info ) == FALSE )
		Prompter.WriteError( info );
	else if ( DataManager.WriteBarcode( info ) == FALSE )
		Prompter.WriteError( info );
	else
	{
		DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
		DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
		SystemMapFull.WriteSystem( FALSE );
		CPosTrayTokenFiles::CreateSystemUpdateFile();
		CCustomExportManagerPlu CustomExport;
		CKeyboardHelpers::RemovePluFile ( DataManager.GetActiveDbIdx() );
		
		if ( EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE )
		{
			int nDbIdx = DataManager.GetActiveDbIdx();
			CString strSource = dbDatabase.GetFilePathDataPluStockAuditTemp( nDbIdx );
			CString strDest = dbDatabase.GetFilePathDataPluStockAudit( nDbIdx );
			::Append ( strSource, strDest );
			CFileRemover FileRemover ( strSource );
		}
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::WritePriceSetFile()
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

void CPluDatabaseDlgEcrman::BuildRecordList()
{
	m_FilterArray.BuildRecordList();
	m_listPlu.SetItemCountEx ( m_FilterArray.GetSize() );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnFilterPluRecords() 
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

void CPluDatabaseDlgEcrman::OnFilterPluStatus() 
{
	CLocalPluFilterDlg dlg ( m_FilterArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		BuildRecordList();
		DisplayScreen ( 0 );
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnNoFilterPluRecords()
{
	int nPluIdx = 0;
	int nSelection = m_listPlu.GetCurSel();

	if ((nSelection >= 0) && (nSelection < m_FilterArray.GetSize()))
	{
		nPluIdx = m_FilterArray.GetPluItemIndexAt(nSelection);
	}

	m_FilterArray.SetDescriptionFilter("", 0, FALSE);
	m_FilterArray.SetDepartmentFilter(DEPARTMENTSET_ALL);
	m_FilterArray.SetAnalysisCategoryFilter(9999);
	m_FilterArray.SetEcrmanSupplierFilter(ECRMANSUPPLIER_ALL);
	m_FilterArray.SetEcrmanShowNonStockFlag(TRUE);
	m_FilterArray.SetShowActiveItemsFlag(TRUE);
	m_FilterArray.SetShowInactiveItemsFlag(TRUE);
	m_FilterArray.SetEcrmanSuppRefFilter("");
	m_FilterArray.SetPluRangeFlag(FALSE);

	{
		CKeyboardFilterSelectorLoc Loc;
		Loc.m_nNodeType = NODE_SYSTEM;
		Loc.m_nEntityIdx = 0;
		m_FilterArray.SetKeyboardFilter(Loc, 1);
	}

	BuildRecordList();
	DisplayScreen(nPluIdx);
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnColumnEdit() 
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

void CPluDatabaseDlgEcrman::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listPlu, this );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluDatabaseDeliver() 
{
	int nIndex = m_listPlu.GetCurSel();
	if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
	{
		int nPluIdx = m_FilterArray.GetPluItemIndexAt( nIndex );
		CPluDeliveryDlg dlg	( nPluIdx, this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluDatabaseShelf() 
{
	PrintSingleLabel( FALSE );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::OnPluDatabaseProduct() 
{
	PrintSingleLabel( TRUE );
}

/**********************************************************************/

void CPluDatabaseDlgEcrman::PrintSingleLabel( bool bProduct )
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
			SSspawnl ( GetSafeOwner(this), P_WAIT, "SPOSBARCODEMANAGER.PGM", "SPOSBARCODEMANAGER.PGM", "1", protect.GetCode(), strDataPath, strName, strPos, strProgramPath, strPrint, PCOptionsClient.GetDefaultLabelPrinter(), strDevmodePath, NULL );
		}
	}
}


/**********************************************************************/

void CPluDatabaseDlgEcrman::SortListInternal( int nType )
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

