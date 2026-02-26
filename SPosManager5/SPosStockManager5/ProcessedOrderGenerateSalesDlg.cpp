/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "ProcessedOrderPreviewDatabaseDlg.h"
#include "ProcessedOrderGenerateInfoDlg.h"
#include "ProcessedOrderPreviewFinaliser.h"
#include "ProcessedOrderReportDlg.h"
#include "RepCsv.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockTreeReportNewReorder.h"
/**********************************************************************/
#include "ProcessedOrderGenerateSalesDlg.h"
/**********************************************************************/

CProcessedOrderGenerateSalesDlg::CProcessedOrderGenerateSalesDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent)
	: CSSDialog(CProcessedOrderGenerateSalesDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo ),
	m_arrayEntityIdx( arrayEntityIdx ) 
{
	m_Helpers.m_pListDatabase = &m_listDatabase;
	m_Helpers.m_pComboSupplier = &m_comboSupplier;
	m_Helpers.m_pListSupplier = &m_listSupplier;
	m_Helpers.m_pComboCategory = &m_comboCategory;
	m_Helpers.m_pCategorySelector = &m_CategorySelector;
	m_Helpers.m_pSuppSelectArray = &m_wSuppSelectArray;
	m_Helpers.m_pLocationSelector = &m_LocationSelector;
	m_Helpers.m_pComboConLevel = &m_comboConLevel;
	m_Helpers.m_pArrayEntityIdx = &m_arrayEntityIdx;
	m_Helpers.m_pArrayLocation = &m_arrayLocation;

	//{{AFX_DATA_INIT(CProcessedOrderGenerateSalesDlg)
	m_nConSelect = -1;
	m_bReorderItems = FALSE;
	m_bConsolidate = FALSE;
	//}}AFX_DATA_INIT
	m_strMonthFrom = "000000";
	m_strMonthTo = "999999";
	m_strDisplayDateFrom = "";
	m_strDisplayDateTo = "";
	m_nMDayFrom = 1;
	m_nMDayTo = 31;
}

/**********************************************************************/

CProcessedOrderGenerateSalesDlg::~CProcessedOrderGenerateSalesDlg()
{
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderGenerateSalesDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Control(pDX, IDC_BUTTON_NONE, m_buttonNone);
	DDX_Control(pDX, IDC_STATIC_PREPARE1, m_staticPrepare1);
	DDX_Control(pDX, IDC_STATIC_PREPARE2, m_staticPrepare2);
	DDX_Control(pDX, IDC_COMBO_CONLEVEL, m_comboConLevel);
	DDX_Control(pDX, IDC_STATIC_PREPARE3, m_staticPrepare3);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_STATIC_PREPARE4, m_staticPrepare4);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_STATIC_PREPARE5, m_staticPrepare5);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_STATIC_PREPARE6, m_staticPrepare6);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_CHECK_INCLUDE, m_checkReorderItems);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_STATIC_ORDER1, m_staticOrder1);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_comboMethod);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_CBIndex(pDX, IDC_COMBO_CONLEVEL, m_nConSelect);
	DDX_Check(pDX, IDC_CHECK_INCLUDE, m_bReorderItems);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderGenerateSalesDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderGenerateSalesDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCATION, OnSelectDatabase)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplierSet)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderGenerateSalesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strTitle = "Generate Orders from Sales History";
	m_Helpers.HandleInitDialog( this, strTitle );

	m_comboMethod.AddString( "Replace sales for specified period only" );
	m_comboMethod.AddString( "Replace sales and suggest zero level orders for unsold items" );
	m_comboMethod.AddString( "Suggest zero level orders for all items" );
	
	int nSel = StockOptions.GetOrderSuggestionMethod();
	if ( ( nSel < 0 ) || ( nSel > 2 ) )
		nSel = 0;
	m_comboMethod.SetCurSel( nSel );

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	if ( m_arrayLocation.GetSize() == 1 )
	{
		CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt(0);
		
		CString strTitle;
		strTitle.Format( "Generate Orders to Replace Sales (%s)", dbDatabase.GetName( LocSelEntity.GetDbIdx() ) );
		SetWindowText( strTitle );

		m_LocationSelector.ForceSelection( LocSelEntity );
		ShowAndEnableWindow( &m_listDatabase, FALSE );
		MoveControl( &m_comboSupplier, 7, 7 );
		MoveControl( &m_listSupplier, 7, 24 );
		MoveControl( &m_buttonAll, 7, 289 );
		MoveControl( &m_buttonNone, 61, 289 );
		MoveControl( &m_staticPrepare1, 186, 24 );
		MoveControl( &m_staticPrepare2, 198, 41 );
		MoveControl( &m_comboConLevel, 239, 40 );
		MoveControl( &m_staticPrepare3, 198, 60 );
		MoveControl( &m_comboCategory, 239, 59 );
		MoveControl( &m_staticPrepare4, 198, 89 );
		MoveControl( &m_comboDates, 239, 87 );
		MoveControl( &m_staticPrepare5, 198, 109 );
		MoveControl( &m_DatePickerFrom, 239, 105 );
		MoveControl( &m_staticPrepare6, 198, 130 );
		MoveControl( &m_DatePickerTo, 239, 126 );
		MoveControl( &m_checkReorderItems, 198, 152 );
		MoveControl( &m_buttonDisplay, 373, 133 );
		MoveControl( &m_staticOrder1, 186, 188 );
		MoveControl( &m_comboMethod, 198, 209 );
		MoveControl( &m_buttonOK, 198, 237 );
		MoveControl( &m_buttonCancel, 388, 289 );
		ResizeDialog( 445, 310 );
	}
	
	return TRUE;
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult) 
{
    m_Helpers.HandleSelectDatabase();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_LOCATION:
		m_listDatabase.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_SUPPLIER:
		m_listSupplier.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnButtonAll() 
{
	m_Helpers.SelectSuppliersAll();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnButtonNone() 
{
	m_Helpers.SelectSuppliersNone();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::BuildSupplierArray()
{
	m_Helpers.BuildSupplierArray();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnSelectSupplierSet() 
{
	m_Helpers.SelectSupplierSet();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CProcessedOrderReportDlg dlgReport;
	dlgReport.m_bConsolidate = m_bConsolidate;
	dlgReport.m_bSort = m_bAlphaSort;
	if ( dlgReport.DoModal() != IDOK )
		return;

	m_bConsolidate = dlgReport.m_bConsolidate;
	m_bAlphaSort = dlgReport.m_bSort;

	BuildSupplierArray();
	
	int nCategoryFilter = m_comboCategory.GetItemData ( m_comboCategory.GetCurSel() );

	CTime time;
	m_DatePickerFrom.GetTime ( time );
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nMDay = time.GetDay();	
	m_strDisplayDateFrom.Format ( "%2.2d/%2.2d/%4.4d", nMDay, nMonth, nYear );
	
	m_DatePickerTo.GetTime ( time );
	nYear = time.GetYear();
	nMonth = time.GetMonth();
	nMDay = time.GetDay();
	m_strDisplayDateTo.Format ( "%2.2d/%2.2d/%4.4d", nMDay, nMonth, nYear );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CStockLevelTreeNew tree;
	CWordArray arraySortOrder;
	arraySortOrder.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
	tree.Configure( ( m_bAlphaSort ? 1 : 0 ), NULL, LocSelEntity, 0, arraySortOrder, TRUE, nCategoryFilter, "", "", FALSE, FALSE, m_wSuppSelectArray, FALSE, FALSE, FALSE );
	tree.Consolidate();

	m_Helpers.OpenStockLevels();

	DataManager.Stock.GenerateFromAuditedSales( LocSelEntity, m_strDisplayDateFrom, m_strDisplayDateTo, ( m_bReorderItems != 0 ) );
	DataManager.Stock.SetQuickModeFlag(TRUE);
	
	CStockTreeReportNewReorder ReorderReport( tree );
	ReorderReport.SetConLevelText( LocSelEntity.GetEntityName() );
	ReorderReport.SetSeparateStockpointsFlag ( ( FALSE == m_bConsolidate ) && ( LocSelEntity.GetConType() != NODE_STOCKPOINT ) );
	ReorderReport.SetSupplierNameFlag( FALSE );
	ReorderReport.SetSupplierRefFlag( FALSE );
	ReorderReport.SetDateFrom( m_strDisplayDateFrom );
	ReorderReport.SetDateTo( m_strDisplayDateTo );
	ReorderReport.PrepareArrays();
	ReorderReport.PrepareReport();
	ReorderReport.CreateReport();
			
	DataManager.Stock.SetQuickModeFlag(FALSE);

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), ReorderReport.GetReportParamsFilename(), ReorderReport.GetReportParamsKey(), this );
}

/**********************************************************************/

bool CProcessedOrderGenerateSalesDlg::GenerateFromAuditedSales( CProcessedOrderPreviewCSVArray& OrderArray )
{
	int nMatched = 0;
	int nGenerated = 0;

	StatusProgress.Lock( TRUE, "Generating order preview" );
	
	switch( m_comboMethod.GetCurSel() )
	{
	case 2:
		m_Helpers.GenerateOrder( 2, OrderArray );
		break;

	case 1:
		m_Helpers.GenerateOrder( 2, OrderArray );
		GenerateFromAuditedSalesInternal ( nMatched, nGenerated, OrderArray );
		break;

	case 0:
	default:
		GenerateFromAuditedSalesInternal ( nMatched, nGenerated, OrderArray );
		break;
	}
	
	StatusProgress.Unlock();
	
	if ( ( m_bReorderItems == TRUE ) && ( nGenerated > 0 ) )
	{
		CProcessedOrderGenerateInfoDlg dlg ( nMatched, nGenerated, this );
		if ( dlg.DoModal() == IDCANCEL )
			return FALSE;
	}

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	//FINALIZE ORDER REQUIREMENTS
	for ( int nOrderIdx = 0; nOrderIdx < OrderArray.GetSize(); nOrderIdx++ )
	{
		CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
		OrderArray.GetAt ( nOrderIdx, OrderPreviewRecord );
		OrderPreviewRecord.UpdateSupplierCosts( LocSelEntity );
		OrderArray.SetAt ( nOrderIdx, OrderPreviewRecord );
	}

	return TRUE;
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::GenerateFromAuditedSalesInternal ( int& nMatched, int& nGenerated, CProcessedOrderPreviewCSVArray& OrderArray )
{
	nMatched = 0;
	nGenerated = 0;

	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet ( m_CategorySelector.GetSelection() );
		
	//REMOVE EXISTING TEMPORARY FILES
	RemoveTemporarySalesFiles( FALSE );

	int nItemsMatched = 0;
	int nItemsReordered = 0;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
		
	//LOOP THROUGH ALL THE STOCKPOINTS IN THE SYSTEM
	int nSpStartIdx, nSpEndIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nSpStartIdx, nSpEndIdx );
	for ( int nSpIdx = nSpStartIdx; nSpIdx <= nSpEndIdx; nSpIdx++ )
	{
		bool bMatch = FALSE;

		//FIND OUT IF THIS STOCKPOINT IS INCLUDED IN THE ORDER
		switch ( LocSelEntity.GetConType() )
		{
		case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == LocSelEntity.GetEntityNo() );		break;
		case NODE_SITE:			bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == LocSelEntity.GetEntityNo() );	break;
		case NODE_DATABASE:
		default:				bMatch = ( dbStockpoint.GetDbNo( nSpIdx ) == LocSelEntity.GetEntityNo() );		break;
		}
					
		//NOTHING TO DO IF THIS STOCKPOINT IS NOT REQUIRED
		if ( bMatch == FALSE )
			continue;

		//WORK OUT THE SALES AUDIT FILENAMES
		__int64 nTotalLength = 0;
		CStringArray FileArray, MonthArray;
		dbStockpoint.GetHistoryFileList( nSpIdx, m_strMonthFrom, m_strMonthTo, FileArray, MonthArray, nTotalLength, FALSE );

		__int64 nProgress = 0;

		for ( int nFileIdx = 0; nFileIdx < FileArray.GetSize(); nFileIdx++ )
		{
			//NOTHING TO DO IF WE CANNOT OPEN THE SALES AUDIT FILE
			CSSFile fileSales;
			if ( fileSales.Open ( FileArray.GetAt( nFileIdx ), "rb" ) == FALSE )
				continue;

			CString strTempFile = FileArray.GetAt( nFileIdx );
			strTempFile = strTempFile.Left ( strTempFile.GetLength() - 4 );
			strTempFile += ".tmp";

			//NOTHING TO DO IF WE CANNOT OPEN THE TEMP AUDIT FILE
			CSSFile fileTemp;
			if ( fileTemp.Open ( strTempFile, "wb" ) == FALSE )
				continue;

			CString strMonth = MonthArray.GetAt( nFileIdx );

			CString strBuffer;
			while ( fileSales.ReadString ( strBuffer ) == TRUE )
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalLength );
			
				//EXTRACT DETAILS FROM THIS SALES LINE
				CCSVSalesHistory csvHistory ( strBuffer );
				CString strStockCode = csvHistory.GetStockCode();
				int nMDay = csvHistory.GetMDay();
				double dStockQty = csvHistory.GetStockQty();
				bool bReordered = csvHistory.GetReorderedFlag();

				if ( ( bReordered == TRUE ) && ( m_bReorderItems == FALSE ) )
				{				
					fileTemp.WriteLine ( strBuffer );
					continue;
				}

				if ( strMonth == m_strMonthFrom )
				{
					if ( m_nMDayFrom > nMDay )
					{	
						fileTemp.WriteLine ( strBuffer );
						continue;
					}
				}

				if ( strMonth == m_strMonthTo )
				{
					if ( m_nMDayTo < nMDay )	
					{				
						fileTemp.WriteLine ( strBuffer );
						continue;
					}
				}
			
				//NOTHING TO DO IF THE STOCK CODE COULD NOT BE FOUND
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
				{	
					fileTemp.WriteLine ( strBuffer );
					continue;
				}

				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );

				//NOTHING TO DO IF THIS RECORD IS DISABLED FOR AUTOMATIC ORDERING
				if ( StockRecord.GetDisableAutoOrderFlag() == TRUE )
				{	
					fileTemp.WriteLine ( strBuffer );
					continue;
				}

				//NOTHING TO DO IF THE STOCK ITEM IS INACTIVE AT THIS STOCKPOINT
				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( nSpIdx ), StockLevels );
				if ( StockLevels.GetActiveFlag() == FALSE )
				{
					fileTemp.WriteLine ( strBuffer );
					continue;
				}
	
				//NOTHING TO DO IF WE FAIL TO MATCH CATEGORY
				if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
				{	
					fileTemp.WriteLine ( strBuffer );
					continue;
				}

				bool bSupplierMatch = FALSE;
	
				//SPECIAL CASE UNSPECIFIED SUPPLIER
				if ( StockRecord.GetSupplierCount() == 0 )
					bSupplierMatch = ( m_wSuppSelectArray.GetAt ( 0 ) == 0 );
				else
				{
					int nSuppIdx;
					int nSuppNo = StockRecord.GetSuppNo(0);
					if ( DataManager.Supplier.FindSupplierByNumber ( nSuppNo, nSuppIdx ) == TRUE )
					{
						for ( int nCheck = 0; nCheck < m_wSuppSelectArray.GetSize(); nCheck++ )
						{
							if ( nSuppIdx == m_wSuppSelectArray.GetAt ( nCheck ) - 1 )
							{
								bSupplierMatch = TRUE;
								break;
							}
						}
					}
				}
				
				//NOTHING TO DO IF WE FAIL TO MATCH SUPPLIER
				if ( bSupplierMatch == FALSE )
				{	
					fileTemp.WriteLine ( strBuffer );
					continue;
				}

				nMatched++;
				if ( bReordered == TRUE ) nGenerated++;

				//FLAG THAT ITEM HAS BEEN ORDERED
				csvHistory.SetReorderedFlag( TRUE );
				fileTemp.WriteLine ( csvHistory.GetLine() );
				
				int nOrderIdx;
				if ( OrderArray.FindStockIdx ( nStockIdx, nOrderIdx ) == FALSE )
				{
					//SET UP ORDER FOR NEW STOCK ITEM
					CProcessedOrderPreviewCSVRecord OrderPreviewRecord;	
					OrderPreviewRecord.SetStockIdx ( nStockIdx );
					OrderPreviewRecord.AddSURequired ( nSpIdx, dStockQty );
					OrderArray.InsertAt ( nOrderIdx, OrderPreviewRecord );
				}
				else
				{
					//ADD TO EXISTING ORDER FOR STOCK ITEM
					CProcessedOrderPreviewCSVRecord OrderPreviewRecord;
					OrderArray.GetAt ( nOrderIdx, OrderPreviewRecord );
					OrderPreviewRecord.AddSURequired ( nSpIdx, dStockQty );
					OrderArray.SetAt ( nOrderIdx, OrderPreviewRecord );
				}
			}
			nProgress += fileSales.GetLength();
		}
	}
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::RemoveTemporarySalesFiles( bool bCopy )
{
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		__int64 nTotalLength = 0;
		CStringArray FileArray, MonthArray;
		dbStockpoint.GetHistoryFileList( nSpIdx, m_strMonthFrom, m_strMonthTo, FileArray, MonthArray, nTotalLength, FALSE );

		for ( int nFileIdx = 0; nFileIdx < FileArray.GetSize(); nFileIdx++ )
		{
			CString strHistoryFile = FileArray.GetAt( nFileIdx );
			
			CString strTempFile = strHistoryFile;
			strTempFile = strTempFile.Left ( strTempFile.GetLength() - 4 );
			strTempFile += ".tmp";

			if ( ::FileExists ( strTempFile ) )
			{
				if ( bCopy )
				{
					CFileRemover FileRemover ( strHistoryFile );
					rename ( strTempFile, strHistoryFile );
				}
				else
					CFileRemover FileRemover ( strTempFile );
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo infoDM;
	if ( m_Helpers.OpenDatabaseReadWrite( infoDM ) == TRUE )
	{
		HandleOKInternal();
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
	}
	else
	{
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::HandleOKInternal() 
{
	BuildSupplierArray();
	
	int nSpStartIdx, nSpEndIdx;
	dbStockpoint.GetSpIdxRange( m_Helpers.GetDbIdx(), nSpStartIdx, nSpEndIdx );
	if ( nSpEndIdx < nSpStartIdx )
		return;

	int nDefSp = dbStockpoint.GetSpNo( nSpStartIdx );
	
	//THIS ARRAY WILL HOLD THE ITEMS TO BE ORDERED
	CProcessedOrderPreviewCSVArray OrderArray;

	CString strTitle;
	CString strCategory = m_CategorySelector.GetCategoryText();
	
	CTime time;
	m_DatePickerFrom.GetTime ( time );
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nMDay = time.GetDay();
		
	m_strDisplayDateFrom.Format ( "%2.2d/%2.2d/%4.4d", nMDay, nMonth, nYear );
	m_strMonthFrom.Format ( "%4.4d%2.2d", nYear, nMonth );
	m_nMDayFrom = nMDay;

	m_DatePickerTo.GetTime ( time );
	nYear = time.GetYear();
	nMonth = time.GetMonth();
	nMDay = time.GetDay();
		
	m_strDisplayDateTo.Format ( "%2.2d/%2.2d/%4.4d", nMDay, nMonth, nYear );
	m_strMonthTo.Format ( "%4.4d%2.2d", nYear, nMonth );
	m_nMDayTo = nMDay;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	strTitle.Format ( "Order Preview (%s, %s, %s to %s)",
		LocSelEntity.GetEntityName(),
		( const char* ) strCategory,
		( const char* ) m_strDisplayDateFrom,
		( const char* ) m_strDisplayDateTo );
	
	bool bContinue = TRUE;

	//NOTHING TO DO IF NO SUPPLIERS WERE SPECIFIED
	if ( m_wSuppSelectArray.GetSize() != 0 )
		bContinue = GenerateFromAuditedSales ( OrderArray ); 
	
	if ( bContinue == FALSE )
	{
		RemoveTemporarySalesFiles( FALSE );
		Prompter.Info ( "No order lines were generated" );
		return;
	}

	CProcessedOrderPreviewDatabaseDlg dlgPreview ( strTitle, OrderArray, LocSelEntity, this );

	bool bGotOrder = ( dlgPreview.DoModal() == IDOK );

	if ( TRUE == bGotOrder )
	{	
		for ( int n = OrderArray.GetSize() - 1; n >= 0; n-- )
		{
			CProcessedOrderPreviewCSVRecord OrderItem;
			OrderArray.GetAt( n, OrderItem );

			if ( OrderItem.HasOrder( LocSelEntity ) == FALSE )
				OrderArray.RemoveAt( n );
		}

		bGotOrder = ( OrderArray.GetSize() > 0 );
	}
		
	if ( FALSE == bGotOrder )
	{
		Prompter.Info ( "No order lines were generated" );
		return;
	}

	CProcessedOrderPreviewFinaliser Finaliser( this, LocSelEntity, m_strMonthFrom, m_strMonthTo );

	if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_ORDER )
		Finaliser.CreateStockpointOrders( OrderArray );
	else
		Finaliser.CreateDatabaseOrders( OrderArray );
}

/**********************************************************************/

void CProcessedOrderGenerateSalesDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
