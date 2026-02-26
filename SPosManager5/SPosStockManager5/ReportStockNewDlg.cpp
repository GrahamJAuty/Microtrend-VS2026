/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataPresetDlg.h"
#include "ListDataStockReportSortOrderDlg.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "ProcessedOrderArray.h"
#include "ProcessedOrderReportOnOrderCategory.h"
#include "ProcessedOrderReportOnOrderSupplier.h"
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "..\CommonEcrStock\SelectPresetActionDlg.h"
#include "SiteCSVArray.h"
#include "StatusProgress.h"
#include "StockFilterArray.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "StockReportAdjustmentDlg.h"
#include "StockReportAllowanceDlg.h"
#include "StockReportApparentDlg.h"
#include "StockReportNames.h"
#include "StockReportReorderDateDlg.h"
#include "StockReportSalesDlg.h"
#include "StockReportSelect.h"
#include "StockReportSortOrderEditDlg.h"
#include "StockReportSupplierDlg.h"
#include "..\CommonEcrStock\StockReportTop100Dlg.h"
#include "StockReportVarianceDlg.h"
#include "SupplierSetCSVArray.h"
/**********************************************************************/
#include "ReportStockNewDlg.h"
/**********************************************************************/

CReportStockNewDlg::CReportStockNewDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent )
	: CSSDialog("IDD_STOCK_REPORT_LEVELS", pParent), 
	m_StockTreeReportAdjustment( m_StockLevelTree ),
	m_StockTreeReportApparent( TRUE, m_StockLevelTree ),
	m_StockTreeReportSummary( FALSE, m_StockLevelTree ),
	m_StockTreeReportApparentDU( TRUE, m_StockLevelTree ),
	m_StockTreeReportSummaryDU( FALSE, m_StockLevelTree ),
	m_StockTreeReportCostSummary( m_StockLevelTree ),
	m_StockTreeReportDelivery( m_StockLevelTree ),
	m_StockTreeReportEstimatedProfit( m_StockLevelTree ),
	m_StockTreeReportPriceList( m_StockLevelTree ),
	m_StockTreeReportRetailSummary( m_StockLevelTree ),
	m_StockTreeReportReturns( m_StockLevelTree ),
	m_StockTreeReportReorder( m_StockLevelTree ),
	m_StockTreeReportSales( m_StockLevelTree ),
	m_StockTreeReportStockSheet( m_StockLevelTree ),
	m_StockTreeReportSupplier( m_StockLevelTree ),
	m_StockTreeReportValuationOpening( m_StockLevelTree ),
	m_StockTreeReportValuationApparent( FALSE, m_StockLevelTree ),
	m_StockTreeReportValuationOverStock( TRUE, m_StockLevelTree ),
	m_StockTreeReportValuationTransfer( m_StockLevelTree ),
	m_StockTreeReportTransferPeriod( m_StockLevelTree ),
	m_StockTreeReportTop100Apparent( m_StockLevelTree ),
	m_StockTreeReportTop100Sales( m_StockLevelTree ),
	m_StockTreeReportYtdSales( m_StockLevelTree ),
	m_CategorySelector( m_comboCategory )
{
	m_IDStaticCustomField[0] = IDC2_STATIC_CUSTOM_FIELD1;
	m_IDStaticCustomField[1] = IDC2_STATIC_CUSTOM_FIELD2;
	m_IDStaticCustomField[2] = IDC2_STATIC_CUSTOM_FIELD3;
	m_IDStaticCustomField[3] = IDC2_STATIC_CUSTOM_FIELD4;
	m_IDStaticCustomField[4] = IDC2_STATIC_CUSTOM_FIELD5;

	m_IDComboCustomField[0] = IDC2_COMBO_CUSTOM_FIELD1;
	m_IDComboCustomField[1] = IDC2_COMBO_CUSTOM_FIELD2;
	m_IDComboCustomField[2] = IDC2_COMBO_CUSTOM_FIELD3;
	m_IDComboCustomField[3] = IDC2_COMBO_CUSTOM_FIELD4;
	m_IDComboCustomField[4] = IDC2_COMBO_CUSTOM_FIELD5;

	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;

	//{{AFX_DATA_INIT(CReportStockNewDlg)
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	m_bStockCodes = FALSE;
	m_bSuppRef = FALSE;
	m_bSuppName = FALSE;
	m_bStockUnit = FALSE;
	m_bExcludeInactive = FALSE;
	m_bItemFilter = TRUE;
	m_bShowItems = TRUE;
	m_bDrawLines = FALSE;
	m_bAlphaSort = FALSE;
	//}}AFX_DATA_INIT
	m_bSelectShowItem = TRUE;
	m_bSelectTimeType = TRUE;
	m_bSelectDrawLines = FALSE;
	m_bSelectSuppRef = TRUE;
	m_bSelectStockUnit = TRUE;
	m_bSelectExcludeInactive = TRUE;
	m_bSelectConLevel = TRUE;
	m_nReportType = STOCK_SALES;
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_bStockCodes = FALSE;
	m_nDefSelectIdx = 0;
	m_nDefSelectType = NODE_SYSTEM;
}

/**********************************************************************/

void CReportStockNewDlg::SetDefaultSelection( int nDefSelectIdx, int nDefSelectType )
{
	m_nDefSelectIdx = nDefSelectIdx;
	m_nDefSelectType = nDefSelectType;
}

/**********************************************************************/

void CReportStockNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportStockNewDlg)

	DDX_Control(pDX, IDC2_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC2_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC2_COMBO_PRESET, m_comboPreset);
	
	DDX_Control(pDX, IDC2_BUTTON_SORTORDER, m_buttonSortOrder);
	DDX_Control(pDX, IDC2_COMBO_SORTORDER, m_comboSortOrder);
	DDX_Control(pDX, IDC2_CHECK_SHOWITEMS, m_checkShowItems);
	
	DDX_Control(pDX, IDC2_COMBO_PERIOD, m_comboPeriod);
	DDX_Control(pDX, IDC2_COMBO_CONLEVEL, m_comboConLevel);
	
	DDX_Control(pDX, IDC2_CHECK_SUPPNAME, m_checkSuppName);
	DDX_Control(pDX, IDC2_CHECK_SUPPREF, m_checkSuppRef);
	DDX_Control(pDX, IDC2_CHECK_STOCKUNIT, m_checkStockUnit);
	DDX_Control(pDX, IDC2_CHECK_DRAWLINES, m_checkDrawLines);
	DDX_Control(pDX, IDC2_CHECK_ALPHASORT, m_checkAlphaSort);
	
	DDX_Control(pDX, IDC2_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC2_COMBO_CATEGORY, m_comboCategory);
	
	for ( int n = 0; n < 5; n++ )
	{
		DDX_Control(pDX, m_IDStaticCustomField[n], m_staticCustomField[n] );
		DDX_Control(pDX, m_IDComboCustomField[n], m_comboCustomField[n] );
	}

	DDX_Control(pDX, IDC2_CHECK_ALLSTOCK, m_checkStockCodes);	
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE_TO, m_editStockCodeTo);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE_FROM, m_editStockCodeFrom);
	DDX_Control(pDX, IDC2_BUTTON_BROWSETO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC2_BUTTON_BROWSEFROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC2_CHECK_INACTIVE, m_checkExcludeInactive);
	DDX_Control(pDX, IDC2_CHECK_ITEMFILTER, m_checkItemFilter);
	
	DDX_Check(pDX, IDC2_CHECK_SHOWITEMS, m_bShowItems);
	
	DDX_Check(pDX, IDC2_CHECK_SUPPNAME, m_bSuppName);
	DDX_Check(pDX, IDC2_CHECK_SUPPREF, m_bSuppRef);
	DDX_Check(pDX, IDC2_CHECK_STOCKUNIT, m_bStockUnit);
	DDX_Check(pDX, IDC2_CHECK_DRAWLINES, m_bDrawLines);
	DDX_Check(pDX, IDC2_CHECK_ALPHASORT, m_bAlphaSort);
	
	DDX_Check(pDX, IDC2_CHECK_ALLSTOCK, m_bStockCodes);
	DDX_Text(pDX, IDC2_EDIT_STOCKCODE_FROM, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max );
	DDX_Text(pDX, IDC2_EDIT_STOCKCODE_TO, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max );
	DDX_Check(pDX, IDC2_CHECK_INACTIVE, m_bExcludeInactive);
	DDX_Check(pDX, IDC2_CHECK_ITEMFILTER, m_bItemFilter);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportStockNewDlg, CDialog)
	//{{AFX_MSG_MAP(CReportStockNewDlg)

	ON_WM_MEASUREITEM()
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC2_LIST, OnSelectReport)
	ON_NOTIFY(NM_DBLCLK, IDC2_LIST, OnDoubleClickList)
	
	ON_BN_CLICKED(IDC2_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC2_COMBO_PRESET, OnSelectPreset)
	
	ON_BN_CLICKED(IDC2_BUTTON_SORTORDER, OnButtonSortOrder)
	
	ON_CBN_SELCHANGE(IDC2_COMBO_CONLEVEL, OnSelectConLevel)
	
	ON_CBN_SELCHANGE(IDC2_COMBO_CATEGORY, OnSelectCategory)
	ON_BN_CLICKED(IDC2_CHECK_ALLSTOCK, OnToggleAllStock)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSEFROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSETO, OnButtonBrowseTo)
	
	ON_BN_CLICKED(IDC2_BUTTON_CONTINUE, OnButtonPrint)
	ON_MESSAGE ( WM_APP, OnReportMessage)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReportStockNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CategoryComboHelpers.SetControlPointers( &m_LocationSelector, &m_CategorySelector );

	int nSelection = 0;

	m_listReports.SubclassDlgItem ( IDC2_LIST, this );
	m_listReports.InsertColumn ( 0, "", LVCFMT_LEFT, 180 );
	m_listReports.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	CWordArray arrayTypes;
	
	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.SetDatabaseMode ( NODE_STOCKPOINT );
	
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboConLevel );
	m_LocationSelector.ForceSelection ( m_nDefSelectType, m_nDefSelectIdx );
	
	m_nCategoryDbIdx = -1;
	m_nCustomFieldDbIdx = -1;
	
	PresetReportManager.StockLevel.SetPasswordAccess();
	FillPresetCombo(-1);
	
	FillSortOrderCombo( SORTORDER_SELECT_INDEX, 0 );
	m_CategoryComboHelpers.UpdateCombos();
	FillSupplierCombo();
	InitialiseCustomFieldCombos();
	FillCustomFieldCombos();
	
	CStockReportSelect StockReportSelect;
	StockReportSelect.LoadReportSelection();

	CWordArray arrayReportTypes;
	
	for ( int n = 0; n < StockReportSelect.GetReportCount(); n++ )
	{
		int nReportType = StockReportSelect.GetReportType(n);

		switch( nReportType )
		{
		case STOCK_SALES:
		case STOCK_TOP_100_SALES:
		case STOCK_ESTIMATED_PROFIT:
		case STOCK_REORDER:
		case ORDER_ONORDER_SUPPLIER:
		case ORDER_ONORDER_CATEGORY:
		case STOCK_APPARENT_QTY:
		case STOCK_APPARENT_QTY_DU:
		case STOCK_APPARENT_SUMMARY:
		case STOCK_APPARENT_SUMMARY_DU:
		case STOCK_TOP_100_APPARENT:
		case STOCK_APPARENT_VALUE:
		case STOCK_OPENING_VALUE:
		case STOCK_OVERSTOCK_VALUE:
		case STOCK_DELIVERY:
		case STOCK_RETURNS:
		case STOCK_ADJUST:
		case STOCK_SHEET:
		case STOCK_PRICE_LIST:
		case STOCK_RETAIL_SUMMARY:
		case STOCK_COST_SUMMARY:
			arrayReportTypes.Add( nReportType );
			break;

		case STOCK_TRANSFER_VALUE:
		case STOCK_TRANSFER_PERIOD:
			if ( Sysset.GetMaxStockPoints() >= 2 )
				arrayReportTypes.Add( nReportType );
			break;

		case STOCK_SUPPLIER:
			if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
				arrayReportTypes.Add( nReportType );
		}
	}

	if ( arrayReportTypes.GetSize() == 0 )
		arrayReportTypes.Add( STOCK_APPARENT_QTY );
	
	CStockReportNames ReportNames;
	for ( int n = 0; n < arrayReportTypes.GetSize(); n++ )
	{
		int nReportType = arrayReportTypes.GetAt(n);
		CString strName = ReportNames.GetComboName( nReportType );
		
		m_listReports.InsertItem ( n, "" );
		m_listReports.SetItem ( n, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );

		m_listReports.SetItemData( n, nReportType );
	}

	m_listReports.SetCurSel ( 0 );
	SelectReport();
	OnSelectConLevel();

	m_comboPeriod.AddString( "Period" );
	m_comboPeriod.AddString( "Year to Date" );
	m_comboPeriod.AddString( "Year to Date + Period" );
	m_comboPeriod.SetCurSel( 0 );

	m_editStockCodeFrom.EnableWindow ( FALSE );
	m_editStockCodeTo.EnableWindow ( FALSE );
	m_buttonBrowseFrom.EnableWindow ( FALSE );
	m_buttonBrowseTo.EnableWindow ( FALSE );

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_STOCK_LEVEL, m_comboPreset, m_nPresetNo );

		if ( 0 == m_nPresetError )
		{
			OnSelectPreset();

			if ( TRUE == m_bAutoRunPreset )
				OnButtonPrint();
		}

		if ( TRUE == m_bAutoRunPreset )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}			
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );

	return TRUE;  
}

/**********************************************************************/

void CReportStockNewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC2_LIST )	
	{
		m_listReports.AdjustRowHeight( lpMeasureItemStruct, 8 );
	}
}

/**********************************************************************/

void CReportStockNewDlg::InitialiseCustomFieldCombos()
{
	DataManager.CustomFieldManager.Initialise();

	for ( int nSlot = 0; nSlot < 5; nSlot++ )
	{
		if ( nSlot < DataManager.CustomFieldManager.GetSlotCount() )
			m_staticCustomField[nSlot].SetWindowText( DataManager.CustomFieldManager.GetFieldNameBySlot( nSlot ) );
		else
		{
			m_staticCustomField[nSlot].ShowWindow( SW_HIDE );
			m_comboCustomField[nSlot].ShowWindow( SW_HIDE );
		}
	}

	int nOffset = 20 * DataManager.CustomFieldManager.GetSlotCount();

	MoveControl( &m_checkStockCodes, 401, 77 + nOffset );
	MoveControl( &m_editStockCodeFrom, 460, 80 + nOffset );
	MoveControl( &m_editStockCodeTo, 460, 96 + nOffset );
	MoveControl( &m_buttonBrowseFrom, 563, 80 + nOffset );
	MoveControl( &m_buttonBrowseTo, 563, 96 + nOffset );
	MoveControl( &m_checkExcludeInactive, 401, 121 + nOffset );
	MoveControl( &m_checkItemFilter, 401, 135 + nOffset );

	DataManager.CustomFieldManager.SetIncludeAllFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeNAFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeSetsFlag( TRUE );

	for ( int nSlot = 0; nSlot < 5; nSlot++ )
		DataManager.CustomFieldManager.SetComboSelector( nSlot, &m_comboCustomField[ nSlot ] );
}

/**********************************************************************/

void CReportStockNewDlg::SetEnables()
{	
	//we can only select categories if we have two or more of them
	m_comboCategory.EnableWindow ( m_comboCategory.GetCount() > 1 );
	m_comboConLevel.EnableWindow ( ( m_bSelectConLevel ) && ( m_comboConLevel.GetCount() > 1 ) );

	m_checkSuppRef.EnableWindow ( m_bSelectSuppRef );
	m_checkSuppName.EnableWindow ( m_bSelectSuppRef );
	m_checkStockUnit.EnableWindow ( m_bSelectStockUnit );
	m_checkShowItems.EnableWindow ( m_bSelectShowItem );
	m_checkExcludeInactive.EnableWindow ( m_bSelectExcludeInactive );
	m_checkDrawLines.EnableWindow( m_bSelectDrawLines );

	m_comboPeriod.EnableWindow( m_bSelectTimeType );
}

/**********************************************************************/

void CReportStockNewDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_STOCK_LEVEL, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

int CReportStockNewDlg::GetSortOrderTypeFromReportType( int nReportType )
{
	int nSortOrderType = STOCKREPORT_SORTORDER_SALES;
	switch( nReportType )
	{
	case ORDER_ONORDER_CATEGORY:		nSortOrderType = -1;		break;
	case ORDER_ONORDER_SUPPLIER:		nSortOrderType = -1;		break;
	case STOCK_SALES:					nSortOrderType = STOCKREPORT_SORTORDER_SALES;				break;
	case STOCK_TOP_100_SALES:			nSortOrderType = STOCKREPORT_SORTORDER_TOP_100_SALES;		break;
	case STOCK_ESTIMATED_PROFIT:		nSortOrderType = STOCKREPORT_SORTORDER_ESTIMATED_PROFIT;	break;
	case STOCK_REORDER:					nSortOrderType = STOCKREPORT_SORTORDER_REORDER;				break;
	case STOCK_APPARENT_QTY:			nSortOrderType = STOCKREPORT_SORTORDER_APPARENT_QTY;		break;
	case STOCK_APPARENT_QTY_DU:			nSortOrderType = STOCKREPORT_SORTORDER_APPARENT_QTY_DU;		break;
	case STOCK_APPARENT_SUMMARY:		nSortOrderType = STOCKREPORT_SORTORDER_APPARENT_SUMMARY;	break;
	case STOCK_APPARENT_SUMMARY_DU:		nSortOrderType = STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU;	break;
	case STOCK_TOP_100_APPARENT:		nSortOrderType = STOCKREPORT_SORTORDER_TOP_100_APPARENT;	break;
	case STOCK_APPARENT_VALUE:			nSortOrderType = STOCKREPORT_SORTORDER_APPARENT_VALUE;		break;
	case STOCK_OPENING_VALUE:			nSortOrderType = STOCKREPORT_SORTORDER_OPENING_VALUE;		break;
	case STOCK_OVERSTOCK_VALUE:			nSortOrderType = STOCKREPORT_SORTORDER_OVERSTOCK_VALUE;		break;
	case STOCK_DELIVERY:				nSortOrderType = STOCKREPORT_SORTORDER_DELIVERY;			break;
	case STOCK_RETURNS:					nSortOrderType = STOCKREPORT_SORTORDER_RETURNS;				break;
	case STOCK_ADJUST:					nSortOrderType = STOCKREPORT_SORTORDER_ADJUST;				break;
	case STOCK_SHEET:					nSortOrderType = STOCKREPORT_SORTORDER_SHEET;				break;
	case STOCK_PRICE_LIST:				nSortOrderType = STOCKREPORT_SORTORDER_PRICE_LIST;			break;
	case STOCK_RETAIL_SUMMARY:			nSortOrderType = STOCKREPORT_SORTORDER_RETAIL_SUMMARY;		break;
	case STOCK_COST_SUMMARY:			nSortOrderType = STOCKREPORT_SORTORDER_COST_SUMMARY;		break;
	case STOCK_TRANSFER_VALUE:			nSortOrderType = STOCKREPORT_SORTORDER_TRANSFER_VALUE;		break;
	case STOCK_TRANSFER_PERIOD:			nSortOrderType = STOCKREPORT_SORTORDER_TRANSFER_PERIOD;		break;
	case STOCK_SUPPLIER:				nSortOrderType = STOCKREPORT_SORTORDER_SUPPLIER;			break;
	}

	return nSortOrderType;
}

/**********************************************************************/

void CReportStockNewDlg::FillSortOrderCombo( int nType, int nSelection )
{
	int nItemData = 0;

	switch( nType )
	{
	case SORTORDER_SELECT_CURRENT:
		{
			int nSel = m_comboSortOrder.GetCurSel();
			if ( nSel >= 0 && nSel < m_comboSortOrder.GetCount() )
				nItemData = m_comboSortOrder.GetItemData( nSel ); 
		}
		break;

	case SORTORDER_SELECT_ITEMDATA:
		nItemData = nSelection;
		break;
	}

	m_comboSortOrder.ResetContent();

	m_arraySortOrder.RemoveAt( 0, m_arraySortOrder.GetSize() );
	
	bool bAllowAdhoc = FALSE;

	switch ( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
		//ON ORDER REPORTS DO NOT USE THE STOCK TREE BASE CLASS
		m_arraySortOrder.AddSortOrder( SH_STKLVL_SUPP );
		break;

	case ORDER_ONORDER_CATEGORY:
		//ON ORDER REPORTS DO NOT USE THE STOCK TREE BASE CLASS
		m_arraySortOrder.AddSortOrder( SH_STKLVL_CAT );
		break;

	default:
		{
			int nSortOrderType = GetSortOrderTypeFromReportType( m_nReportType );
			DataManagerNonDb.SetActiveStockReportSortOrderType( nSortOrderType );
			m_arraySortOrder.CopyFrom( *DataManagerNonDb.GetActiveStockReportSortOrder() );
			bAllowAdhoc = TRUE;
		}
		break;
	}

	int n = 0;
	int nCustomCount = 0;
	for ( int nPos = 0; nPos < m_arraySortOrder.GetSize(); nPos++ )
	{
		CStockReportSortOrderCSVRecord SortOrderRecord;
		m_arraySortOrder.GetAt( nPos, SortOrderRecord );

		switch ( SortOrderRecord.GetSortOrderType() )
		{
		case SH_CUSTOM:
			m_comboSortOrder.AddString ( SortOrderRecord.GetListName() );
			m_comboSortOrder.SetItemData( n++, 500 + nCustomCount++ );
			break;

		case SH_STKLVL_NONE:		
		case SH_STKLVL_SUPP:		
		case SH_STKLVL_SUPP_SP:		
		case SH_STKLVL_SUPP_SP_CAT:	
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_SUPP_CAT_SP:	
		case SH_STKLVL_SP:			
		case SH_STKLVL_SP_SUPP:		
		case SH_STKLVL_SP_SUPP_CAT:	
		case SH_STKLVL_SP_CAT:		
		case SH_STKLVL_SP_CAT_SUPP:	
		case SH_STKLVL_CAT:			
		case SH_STKLVL_CAT_SUPP:	
		case SH_STKLVL_CAT_SUPP_SP:	
		case SH_STKLVL_CAT_SP:		
		case SH_STKLVL_CAT_SP_SUPP:	
			m_comboSortOrder.AddString ( SortOrderRecord.GetListName() );
			m_comboSortOrder.SetItemData( n++, SortOrderRecord.GetSortOrderType() );
			break;
		}
	}

	if ( TRUE == bAllowAdhoc )
	{
		m_comboSortOrder.AddString ( "Adhoc Sort Order" );
		m_comboSortOrder.SetItemData( n++, SH_CUSTOM );
	}

	int nCurSel = 0;
	
	switch( nType )
	{
	case SORTORDER_SELECT_CURRENT:
	case SORTORDER_SELECT_ITEMDATA:
		{
			for ( int n = 0; n < m_comboSortOrder.GetCount(); n++ )
			{
				if ( m_comboSortOrder.GetItemData( n ) == nItemData )
				{
					nCurSel = n;
					break;
				}
			}
		}
		break;

	case SORTORDER_SELECT_INDEX:
	default:
		if ( ( nSelection >= 0 ) && ( nSelection < m_comboSortOrder.GetCount() ) )
			nCurSel = nSelection;
		break;
	}

	m_comboSortOrder.SetCurSel( nCurSel );
	m_comboSortOrder.EnableWindow( m_comboSortOrder.GetCount() > 1 );
}

/**********************************************************************/

void CReportStockNewDlg::FillSupplierCombo()
{
	//ITEM DATA 0 =          CUSTOM SUPPLIERS
	//ITEM DATA 1 =		UNSPECIFIED SUPPLIER
	//ITEM DATA 2 =		ALL SUPPLIERS
	//ITEM DATA <= 9999  =	SUPPLIER NUMBER + 2
	//ITEM DATA >= 10000 =	SUPPLIER SET + 10000

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nInsertPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nInsertPos, DataManager.Supplier.GetSuppNo ( nIndex ) + 2 );
	}

	m_comboSupplier.InsertString ( 0, "All" );
	m_comboSupplier.SetItemData ( 0, 2 );

	int nSupplierPos = 1;

	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
	{
		m_comboSupplier.InsertString ( nSupplierPos, DataManager.SupplierSet.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nSupplierPos, FIRST_SUPPLIERSET_FILTER + nIndex );
		nSupplierPos++;
	}

	m_comboSupplier.InsertString ( nSupplierPos, "Unspecified" );
	m_comboSupplier.SetItemData ( nSupplierPos, 1 );
	nSupplierPos = m_comboSupplier.InsertString ( -1, "Custom" );
	m_comboSupplier.SetItemData ( nSupplierPos, 0 );
	m_comboSupplier.SetCurSel(0);
}

/**********************************************************************/

void CReportStockNewDlg::FillCustomFieldCombos()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetDbIdx() != m_nCustomFieldDbIdx )
	{
		m_nCustomFieldDbIdx = LocSelEntity.GetDbIdx();

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

		for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
		{
			DataManager.CustomFieldManager.FillComboSelectorDatabase( nSlot );
			DataManager.CustomFieldManager.ForceComboSelection( nSlot, 0 );
		}
	}
}

/**********************************************************************/

bool CReportStockNewDlg::GetSupplierIdxArray( CWordArray& wArray )
{
	wArray.RemoveAll();
	
	int nSelection = m_comboSupplier.GetCurSel();
	if ( nSelection < 0 || nSelection >= m_comboSupplier.GetCount() )
		return FALSE;

	int nSupplierFilter = m_comboSupplier.GetItemData( nSelection );
	
	//CUSTOM LIST OF SUPPLIERS
	if ( nSupplierFilter == 0 )
	{
		CStockReportSupplierDlg dlg ( m_wSuppSelectArray, this );
		if ( dlg.DoModal() != IDOK )	
			return FALSE;

		for ( int nIndex = 0; nIndex < m_wSuppSelectArray.GetSize(); nIndex++ )
			wArray.Add ( m_wSuppSelectArray.GetAt ( nIndex ) );
	}
	//ALL SUPPLIERS OR SELECTED SUPPLIER
	else 
	{
		if ( nSupplierFilter < FIRST_SUPPLIERSET_FILTER ) 
			nSupplierFilter -= 2;

		DataManager.SupplierSet.CreateSuppSelectArray ( nSupplierFilter, wArray );	
	}

	if ( wArray.GetSize() < 1 )
	{
		Prompter.Error ( "You must select at least one supplier\nbefore you can view this report." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CReportStockNewDlg::OpenStockLevels()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CWaitCursor wait;
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nStartSpIdx, nEndSpIdx );

	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		{
			CDataManagerInfo info;
			DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ); 
		}
		break;

	case NODE_SITE:
		for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		{
			if ( dbStockpoint.GetSiteNo( nSpIdx ) == LocSelEntity.GetEntityNo() )
			{
				CDataManagerInfo info;
				DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ); 
			}
		}
		break;

	case NODE_STOCKPOINT:
		{
			int nSpIdx;
			int nDbNo = dbDatabase.GetDbNo( LocSelEntity.GetDbIdx() );
			if ( dbStockpoint.FindStockpointByNumber( nDbNo, LocSelEntity.GetEntityNo(), nSpIdx ) == TRUE )
			{
				CDataManagerInfo info;
				DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ); 
			}
		}
		break;
	}
}

/**********************************************************************/

void CReportStockNewDlg::OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectReport();
	}
}

/**********************************************************************/

void CReportStockNewDlg::SelectReport()
{
	UpdateData( TRUE, FALSE );

	//default settings
	m_bSelectShowItem = TRUE;
	m_bSelectTimeType = FALSE;
	m_bSelectDrawLines = FALSE;
	m_bSelectSuppRef = TRUE;
	m_bSelectStockUnit = TRUE;
	m_bSelectExcludeInactive = TRUE;
	m_bSelectConLevel = TRUE;

	int nSelected = m_listReports.GetCurSel();
	if ( nSelected >= 0 && nSelected < m_listReports.GetItemCount() )
		m_nReportType = m_listReports.GetItemData ( nSelected );

	//special settings for categories
	switch ( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
	case ORDER_ONORDER_CATEGORY:
		m_bSelectShowItem = FALSE;
		break;
		
	case STOCK_SUPPLIER:						//disable category summary
	case STOCK_REORDER:
	case STOCK_TOP_100_SALES:
	case STOCK_TOP_100_APPARENT:
	case STOCK_SHEET:
	case STOCK_PRICE_LIST:
		m_bSelectShowItem = FALSE;
		break;
	}
	
	//special settings for report timespan
	switch ( m_nReportType )
	{
	case STOCK_ADJUST:							
	case STOCK_TOP_100_SALES:
	case STOCK_SALES:
	case STOCK_ESTIMATED_PROFIT:
		m_bSelectTimeType = TRUE;	//YTD SELECTION IS ALLOWED
		break;

	case STOCK_PRICE_LIST:
		m_bSelectTimeType = FALSE;	//PERIOD/YTD SELECTION IS IRRELEVANT
		break;

	default:
		m_bSelectTimeType = FALSE;
		m_comboPeriod.SetCurSel(0);	//MUST BE A PERIOD REPORT
		break;
	}

	//special settings for supplier reference
	switch ( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
	case ORDER_ONORDER_CATEGORY:
		m_bSelectSuppRef = FALSE;
		break;
	}

	//special settings for stock unit
	switch ( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
	case ORDER_ONORDER_CATEGORY:
	case STOCK_SHEET:
		m_bSelectStockUnit = FALSE;
		break;
	}

	//special settings for exclude inactive items
	switch ( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
	case ORDER_ONORDER_CATEGORY:
	case STOCK_REORDER:
	case STOCK_DELIVERY:
	case STOCK_RETURNS:
	case STOCK_ADJUST:
	case STOCK_SUPPLIER:
	case STOCK_TRANSFER_VALUE:
	case STOCK_TRANSFER_PERIOD:
		m_bSelectExcludeInactive = FALSE;
		break;
	} 
	
	//special settings for lines between items
	switch( m_nReportType )
	{
	case STOCK_SHEET:
	case STOCK_PRICE_LIST:
	case STOCK_SUPPLIER:
	case STOCK_APPARENT_QTY:
	case STOCK_APPARENT_QTY_DU:
	case STOCK_APPARENT_SUMMARY:
	case STOCK_APPARENT_SUMMARY_DU:
		m_bSelectDrawLines = TRUE;
		break;
	}

	//special settings for stockpoint selection
	switch( m_nReportType )
	{
	case STOCK_PRICE_LIST:
		m_bSelectConLevel = FALSE;
		break;
	}

	SetEnables();
	FillSortOrderCombo( SORTORDER_SELECT_CURRENT, 0 );
}

/**********************************************************************/

void CReportStockNewDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_STOCK_LEVEL, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportStockNewDlg::CreatePreset( CPresetReportInfo& Base ) 
{
	CPresetReportInfoStockLevel Preset;

	int nSel = m_listReports.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listReports.GetItemCount() ) )
		Preset.SetReportType( m_listReports.GetItemData( nSel ) );

	nSel = m_comboSortOrder.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboSortOrder.GetCount() ) )
		Preset.SetSortOrder( m_comboSortOrder.GetItemData( nSel ) );

	Preset.SetShowItemsFlag( m_checkShowItems.GetCheck() != 0 );
	Preset.SetTimeType( m_comboPeriod.GetCurSel() );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	Preset.SetSuppNameFlag( m_checkSuppName.GetCheck() != 0 );
	Preset.SetSuppRefFlag( m_checkSuppRef.GetCheck() != 0 );
	Preset.SetStockUnitFlag( m_checkStockUnit.GetCheck() != 0 );
	Preset.SetDrawLinesFlag( m_checkDrawLines.GetCheck() != 0 );
	Preset.SetAlphaSortFlag( m_checkAlphaSort.GetCheck() != 0 );

	nSel = m_comboSupplier.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboSupplier.GetCount() ) )
		Preset.SetSupplierFilter( m_comboSupplier.GetItemData(nSel) );

	Preset.SetCategoryFilter( m_CategorySelector.GetCategoryFilter() );

	for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
	{
		int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
		int nFilter = DataManager.CustomFieldManager.GetComboSelection( nSlot );
		Preset.SetCustomFieldFilter( nFieldNo, nFilter );
	}
	
	Preset.SetStockRangeFlag( m_checkStockCodes.GetCheck() != 0 );

	CString strStockFrom;
	m_editStockCodeFrom.GetWindowText( strStockFrom );
	Preset.SetStockFrom( strStockFrom );

	CString strStockTo;
	m_editStockCodeTo.GetWindowText( strStockTo );
	Preset.SetStockTo( strStockTo );

	Preset.SetExcludeInactiveFlag( m_checkExcludeInactive.GetCheck() != 0 );
	Preset.SetItemFilterFlag( m_checkItemFilter.GetCheck() != 0 );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportStockNewDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.StockLevel.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.StockLevel.GetAt( nSel, Base );

	CPresetReportInfoStockLevel Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	//DO THIS BEFORE REPORT TYPE, SO THAT ONSELECTREPORT()
	//WILL VALIDATE THE NEW PERIOD/YTD SETTING		
	int nTimeType = Preset.GetTimeType();
	if ( ( nTimeType >= 0 ) && ( nTimeType < m_comboPeriod.GetCount() ) )
		m_comboPeriod.SetCurSel( nTimeType );

	int nReportType = Preset.GetReportType();
	for ( int n = 0; n < m_listReports.GetItemCount(); n++ )
	{
		if ( m_listReports.GetItemData(n) == nReportType )
		{
			m_listReports.SetCurSel(n);
			break;
		}
	}

	SelectReport();
			
	int nSortOrder = Preset.GetSortOrder();
	for ( int n = 0; n < m_comboSortOrder.GetCount(); n++ )
	{
		if ( m_comboSortOrder.GetItemData(n) == nSortOrder )
		{
			m_comboSortOrder.SetCurSel(n);
			break;
		}
	}

	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	
	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );

	m_checkShowItems.SetCheck( Preset.GetShowItemsFlag() );
	
	m_checkSuppName.SetCheck( Preset.GetSuppNameFlag() );
	m_checkSuppRef.SetCheck( Preset.GetSuppRefFlag() );
	m_checkStockUnit.SetCheck( Preset.GetStockUnitFlag() );
	m_checkDrawLines.SetCheck( Preset.GetDrawLinesFlag() );
	m_checkAlphaSort.SetCheck( Preset.GetAlphaSortFlag() );

	int nType = Preset.GetSupplierFilter();
	for ( int n = 0; n < m_comboSupplier.GetCount(); n++ )
	{
		if ( m_comboSupplier.GetItemData(n) == (WORD) nType )
		{
			m_comboSupplier.SetCurSel(n);
			break;
		}
	}

	m_CategorySelector.UpdateCategoryCombo( Preset.GetCategoryFilter() );
	
	for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
	{
		int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
		int nFilter = Preset.GetCustomFieldFilter( nFieldNo );
		DataManager.CustomFieldManager.ForceComboSelection( nSlot, nFilter );
	}

	m_checkStockCodes.SetCheck( Preset.GetStockRangeFlag() );
	OnToggleAllStock();

	CString strStockFrom = Preset.GetStockFrom();
	if ( strStockFrom.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeFrom.SetWindowText( strStockFrom );

	CString strStockTo = Preset.GetStockTo();
	if ( strStockTo.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeTo.SetWindowText( strStockTo );

	m_checkExcludeInactive.SetCheck( Preset.GetExcludeInactiveFlag() );
	m_checkItemFilter.SetCheck( Preset.GetItemFilterFlag() );
}

/**********************************************************************/

void CReportStockNewDlg::OnButtonSortOrder() 
{
	int nRepSel = m_listReports.GetCurSel();
	if ( ( nRepSel < 0 ) || ( nRepSel >= m_listReports.GetItemCount() ) )
		return;

	int nReportType = m_listReports.GetItemData ( nRepSel );

	int nSel = m_comboSortOrder.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboSortOrder.GetCount() )
		return;

	//SELECT FOR ADHOC REPORT SHOULD JUST GO THROUGH DISPLAY ROUTINE
	if ( m_comboSortOrder.GetItemData( nSel ) == SH_CUSTOM )
	{
		switch( nReportType )
		{
			case STOCK_REORDER:
			case ORDER_ONORDER_CATEGORY:
			case ORDER_ONORDER_SUPPLIER:
				break;

			default:
				OnButtonPrint();
				break;
		}
		
		return;
	}

	int nSortOrderType = GetSortOrderTypeFromReportType( nReportType );

	if ( nSortOrderType != -1 )
	{
		int nOldSelect = m_comboSortOrder.GetItemData( nSel );
	
		DataManagerNonDb.SetActiveStockReportSortOrderType( nSortOrderType );
	
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenActiveStockReportSortOrderBuffer( info ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			CListDataStockReportSortOrderDlg dlg ( nOldSelect, this );
	
			if ( dlg.DoModal() == IDOK )
			{
				if ( DataManagerNonDb.WriteActiveStockReportSortOrderBuffer( info ) == FALSE )
				{
					DataManagerNonDb.CloseActiveStockReportSortOrderBuffer( info );
					Prompter.WriteError( info );
					FillSortOrderCombo( SORTORDER_SELECT_ITEMDATA, nOldSelect );
				}
				else
				{
					FillSortOrderCombo( SORTORDER_SELECT_INDEX, dlg.GetListSelection() );
					DataManagerNonDb.CloseActiveStockReportSortOrderBuffer( info );
				}
			}
			else
			{
				DataManagerNonDb.CloseActiveStockReportSortOrderBuffer( info );
				FillSortOrderCombo( SORTORDER_SELECT_ITEMDATA, nOldSelect );
			}
		}	
	}
}

/**********************************************************************/

void CReportStockNewDlg::OnSelectConLevel() 
{	
	UpdateData( TRUE, FALSE );

	m_CategoryComboHelpers.UpdateCombos();

	if ( StockOptions.GetGlobalCustomFieldFlag() == FALSE )
		FillCustomFieldCombos();
		
	SetEnables();

	UpdateData( FALSE );
}

/**********************************************************************/

void CReportStockNewDlg::OnSelectCategory() 
{
	UpdateData( TRUE, FALSE );
	SetEnables();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportStockNewDlg::OnToggleAllStock() 
{
	UpdateData( TRUE, FALSE );
	m_editStockCodeFrom.EnableWindow ( m_bStockCodes );
	m_editStockCodeTo.EnableWindow ( m_bStockCodes );
	m_buttonBrowseFrom.EnableWindow ( m_bStockCodes );
	m_buttonBrowseTo.EnableWindow ( m_bStockCodes );
}

/**********************************************************************/

void CReportStockNewDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportStockNewDlg::OnButtonBrowseFrom() 
{
	UpdateData( TRUE, FALSE );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}

	CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
	StockFilterArray.SetSupplierFilter ( 0 );
	StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetCategoryFilter() );
		
	CStockFilterDlg dlgBrowse ( StockFilterArray, this );	
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strStockCodeFrom = StockRecord.GetStockCode ();	
	UpdateData ( FALSE );
	m_editStockCodeFrom.SetFocus();
	m_editStockCodeFrom.SetSel ( 0, -1 );	
}

/**********************************************************************/

void CReportStockNewDlg::OnButtonBrowseTo() 
{
	UpdateData( TRUE, FALSE );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
	
	CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
	StockFilterArray.SetSupplierFilter ( 0 );
	StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetCategoryFilter() );
	
	CStockFilterDlg dlgBrowse ( StockFilterArray, this );
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strStockCodeTo = StockRecord.GetStockCode ();	
	UpdateData ( FALSE );
	m_editStockCodeTo.SetFocus();
	m_editStockCodeTo.SetSel ( 0, -1 );	
}

/**********************************************************************/

void CReportStockNewDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonPrint();
	*pResult = 0;
}

/**********************************************************************/

void CReportStockNewDlg::OnButtonPrint() 
{
	UpdateData( TRUE, FALSE );
	CommsHandshaker.ProcessSales();
	OnKillFocusStockCode();

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
	
	switch( m_nReportType )
	{
	case ORDER_ONORDER_SUPPLIER:
		PrintOnOrderReportSupplier();
		return;

	case ORDER_ONORDER_CATEGORY:
		PrintOnOrderReportCategory();
		return;
	}
	
	if ( STOCK_TRANSFER_PERIOD == m_nReportType )
	{
		if ( NODE_STOCKPOINT != LocSelEntity.GetConType() )
		{
			CString str;
			str.Format( "The transfer detail report cannot be consolidated\nfor more than one %s stockpoint.",
				DealerFlags.GetWarehouseType( FALSE ) );
			Prompter.Error( str );

			return;
		}
	}
	
	int nCatFilter = m_CategorySelector.GetCategoryFilter();

	CString strFrom = "";
	CString strTo = "";

	if (TRUE == m_bStockCodes)
	{
		strFrom = m_strStockCodeFrom;
		strTo = m_strStockCodeTo;
	}

	bool bExcludeInactive = ( ( m_bExcludeInactive != 0 ) && m_bSelectExcludeInactive );

	CWordArray arraySuppNo;
	if ( GetSupplierIdxArray( arraySuppNo ) == FALSE )
		return;

	bool bAllSupp = ( m_nReportType == STOCK_SUPPLIER );

	switch( m_nReportType )
	{
	case STOCK_PRICE_LIST:
		LocSelEntity.SetDatabaseNode( LocSelEntity.GetDbIdx() );
		break;
	}
	
	bool bShowItems = ( m_bShowItems != 0 );

	switch( m_nReportType )
	{
	case STOCK_TOP_100_SALES:
	case STOCK_TOP_100_APPARENT:
	case STOCK_REORDER:
	case STOCK_SUPPLIER:
	case STOCK_SHEET:
	case STOCK_PRICE_LIST:
		bShowItems = TRUE;
		break;
	}

	int nSel = m_comboSortOrder.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= m_comboSortOrder.GetCount() ) )
		return;

	CStockReportSortOrderCSVRecord SortOrderRecord;
	if ( nSel < m_arraySortOrder.GetSize() )
		m_arraySortOrder.GetAt( nSel, SortOrderRecord );

	int nSortOrder = m_comboSortOrder.GetItemData( m_comboSortOrder.GetCurSel() );

	if ( SH_CUSTOM == nSortOrder )
	{
		int nSortOrderType = GetSortOrderTypeFromReportType( m_nReportType );
		
		if ( nSortOrderType < 0 )
			return;

		DataManagerNonDb.SetActiveStockReportSortOrderType( nSortOrderType );

		DataManagerNonDb.GetActiveStockReportSortOrder() -> LoadAdhocSortOrder( SortOrderRecord );

		CStockReportSortOrderEditDlg dlg( SortOrderRecord, TRUE, this );
		if ( dlg.DoModal() != IDOK )
			return;
	
		DataManagerNonDb.GetActiveStockReportSortOrder() -> SaveAdhocSortOrder( SortOrderRecord );
	}

	CWordArray arraySortOrder;
	SortOrderRecord.TidySortOrderFields();
	SortOrderRecord.GetSortOrderFieldsExternal( arraySortOrder );

	if ( m_nReportType == STOCK_REORDER )
	{
		bool bGotStockpoint = FALSE;

		int nSize = arraySortOrder.GetSize();
		if ( nSize > 0 )
		{
			int nField = ( arraySortOrder.GetAt(nSize - 1) % 100 );

			if ( SH_FIELD_SP == nField )
			{
				arraySortOrder.RemoveAt(nSize - 1);
				bGotStockpoint = TRUE;		
			}
		}

		m_StockTreeReportReorder.SetSeparateStockpointsFlag( bGotStockpoint );
	}

	bool bWantTransferDetail = ( STOCK_TRANSFER_PERIOD == m_nReportType );
	bool bWantSalesCost = ( ( STOCK_ESTIMATED_PROFIT == m_nReportType ) && ( DealerFlags.GetPeriodDeliveryCostProfitFlag() == TRUE ) );
	m_StockLevelTree.Configure( ( m_bAlphaSort ? 1 : 0 ), NULL, LocSelEntity, 0, arraySortOrder, bShowItems, nCatFilter, strFrom, strTo, bExcludeInactive, ( m_bItemFilter != 0 ), arraySuppNo, bAllSupp, bWantTransferDetail, bWantSalesCost );
	
	if( m_nReportType == STOCK_REORDER ) m_StockLevelTree.Invalidate();
	m_StockLevelTree.Consolidate();
	if( m_nReportType == STOCK_REORDER ) m_StockLevelTree.Invalidate();
	
	switch ( m_nReportType )
	{
	case STOCK_SALES:				PrintSalesReport();				break;							
	case STOCK_TOP_100_SALES:		PrintTop100ReportSales();		break;
	case STOCK_APPARENT_QTY:		PrintApparentQtyReport();		break;
	case STOCK_APPARENT_QTY_DU:		PrintApparentQtyDUReport();		break;
	case STOCK_APPARENT_SUMMARY:	PrintApparentSummaryReport();	break;
	case STOCK_APPARENT_SUMMARY_DU:	PrintApparentSummaryDUReport();	break;
	case STOCK_TOP_100_APPARENT:	PrintTop100ReportApparent();	break;
	case STOCK_ESTIMATED_PROFIT:	PrintEstimatedProfitReport();	break;
	case STOCK_RETAIL_SUMMARY:		PrintRetailSummaryReport();		break;
	case STOCK_COST_SUMMARY:		PrintCostSummaryReport();		break;
	case STOCK_APPARENT_VALUE:		PrintApparentValuationReport();	break;
	case STOCK_OPENING_VALUE:		PrintOpeningValuationReport();	break;
	case STOCK_OVERSTOCK_VALUE:		PrintOverstockValuationReport();break;
	case STOCK_TRANSFER_VALUE:		PrintTransferValuationReport();	break;
	case STOCK_TRANSFER_PERIOD:		PrintTransferPeriodReport();	break;
	case STOCK_ADJUST:				PrintAdjustmentReport();		break;
	case STOCK_DELIVERY:			PrintDeliveryReport();			break;
	case STOCK_RETURNS:				PrintReturnsReport();			break;
	case STOCK_SUPPLIER:			PrintSupplierReport();			break;
	case STOCK_SHEET:				PrintStockSheet();				break;
	case STOCK_PRICE_LIST:			PrintStockPriceList();			break;
	case STOCK_REORDER:				PrintReorderReport();			break;
	}
}		

/**********************************************************************/

long CReportStockNewDlg::OnReportMessage( WPARAM wIndex, LPARAM lParam )
{ 
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	switch ( wIndex )
	{
	case STOCK_SALES:
		{
			CStockReportSalesDlg* pDlg = (CStockReportSalesDlg*) lParam;

			m_StockTreeReportSales.PrepareArrays();
			m_StockTreeReportSales.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportSales.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportSales.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportSales.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportSales.PrepareReport( pDlg );
			m_StockTreeReportSales.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSales.GetReportParamsFilename(), m_StockTreeReportSales.GetReportParamsKey(), this );
		}
		break;

	case STOCK_APPARENT_QTY:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportApparent.PrepareArrays();
			m_StockTreeReportApparent.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportApparent.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportApparent.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportApparent.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportApparent.SetDrawLinesFlag( m_bDrawLines != 0 );
			m_StockTreeReportApparent.SetHideAboveMinFlag( pDlg -> m_bHideAboveMin != 0 );
			m_StockTreeReportApparent.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportApparent.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportApparent.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportApparent.PrepareReport();
			m_StockTreeReportApparent.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportApparent.GetReportParamsFilename(), m_StockTreeReportApparent.GetReportParamsKey(), this );
		}
		break;

	case STOCK_APPARENT_SUMMARY:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportSummary.PrepareArrays();
			m_StockTreeReportSummary.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportSummary.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportSummary.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportSummary.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportSummary.SetDrawLinesFlag( m_bDrawLines != 0 );
			m_StockTreeReportSummary.SetHideAboveMinFlag( pDlg -> m_bHideAboveMin != 0 );
			m_StockTreeReportSummary.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportSummary.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportSummary.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportSummary.PrepareReport();
			m_StockTreeReportSummary.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSummary.GetReportParamsFilename(), m_StockTreeReportSummary.GetReportParamsKey(), this );
		}
		break;

	case STOCK_APPARENT_QTY_DU:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportApparentDU.PrepareArrays();
			m_StockTreeReportApparentDU.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportApparentDU.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportApparentDU.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportApparentDU.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportApparentDU.SetDrawLinesFlag( m_bDrawLines != 0 );
			m_StockTreeReportApparentDU.SetHideAboveMinFlag( pDlg -> m_bHideAboveMin != 0 );
			m_StockTreeReportApparentDU.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportApparentDU.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportApparentDU.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportApparentDU.PrepareReport();
			m_StockTreeReportApparentDU.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportApparentDU.GetReportParamsFilename(), m_StockTreeReportApparentDU.GetReportParamsKey(), this );
		}
		break;

	case STOCK_APPARENT_SUMMARY_DU:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportSummaryDU.PrepareArrays();
			m_StockTreeReportSummaryDU.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportSummaryDU.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportSummaryDU.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportSummaryDU.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportSummaryDU.SetDrawLinesFlag( m_bDrawLines != 0 );
			m_StockTreeReportSummaryDU.SetHideAboveMinFlag( pDlg -> m_bHideAboveMin != 0 );
			m_StockTreeReportSummaryDU.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportSummaryDU.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportSummaryDU.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportSummaryDU.PrepareReport();
			m_StockTreeReportSummaryDU.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSummaryDU.GetReportParamsFilename(), m_StockTreeReportSummaryDU.GetReportParamsKey(), this );
		}
		break;

	case STOCK_APPARENT_VALUE:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportValuationApparent.PrepareArrays();
			m_StockTreeReportValuationApparent.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportValuationApparent.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportValuationApparent.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportValuationApparent.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportValuationApparent.SetHideAboveMinFlag( pDlg -> m_bHideAboveMin != 0 );
			m_StockTreeReportValuationApparent.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportValuationApparent.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportValuationApparent.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportValuationApparent.SetZeroCostFlag( pDlg -> m_bZeroCost != 0 );
			m_StockTreeReportValuationApparent.PrepareReport();
			m_StockTreeReportValuationApparent.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationApparent.GetReportParamsFilename(), m_StockTreeReportValuationApparent.GetReportParamsKey(), this );
		}
		break;

	case STOCK_OVERSTOCK_VALUE:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportValuationOverStock.PrepareArrays();
			m_StockTreeReportValuationOverStock.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportValuationOverStock.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportValuationOverStock.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportValuationOverStock.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportValuationOverStock.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportValuationOverStock.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportValuationOverStock.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportValuationOverStock.SetZeroCostFlag( pDlg -> m_bZeroCost != 0 );
			m_StockTreeReportValuationOverStock.PrepareReport();
			m_StockTreeReportValuationOverStock.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationOverStock.GetReportParamsFilename(), m_StockTreeReportValuationOverStock.GetReportParamsKey(), this );
		}
		break;

	case STOCK_TRANSFER_VALUE:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportValuationTransfer.PrepareArrays();
			m_StockTreeReportValuationTransfer.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportValuationTransfer.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportValuationTransfer.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportValuationTransfer.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportValuationTransfer.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportValuationTransfer.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportValuationTransfer.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportValuationTransfer.SetZeroCostFlag( pDlg -> m_bZeroCost != 0 );
			m_StockTreeReportValuationTransfer.PrepareReport();
			m_StockTreeReportValuationTransfer.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationTransfer.GetReportParamsFilename(), m_StockTreeReportValuationTransfer.GetReportParamsKey(), this );
		}
		break;

	case STOCK_OPENING_VALUE:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportValuationOpening.PrepareArrays();
			m_StockTreeReportValuationOpening.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportValuationOpening.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportValuationOpening.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportValuationOpening.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportValuationOpening.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportValuationOpening.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportValuationOpening.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportValuationOpening.SetZeroCostFlag( pDlg -> m_bZeroCost != 0 );
			m_StockTreeReportValuationOpening.PrepareReport();
			m_StockTreeReportValuationOpening.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationOpening.GetReportParamsFilename(), m_StockTreeReportValuationOpening.GetReportParamsKey(), this );
		}
		break;

	case STOCK_ADJUST:
		{
			CStockReportAdjustmentDlg* pDlg = (CStockReportAdjustmentDlg*) lParam;
			
			m_StockTreeReportAdjustment.SetSalesFlag( pDlg -> m_bSales != 0 );
			m_StockTreeReportAdjustment.SetBreakdownFlag( pDlg -> m_bBreakdown != 0 );
			m_StockTreeReportAdjustment.SetAdjustFlag( pDlg -> m_bAdjust != 0 );
			m_StockTreeReportAdjustment.SetWasteAutoFlag( pDlg -> m_bWasteAuto != 0 );
			m_StockTreeReportAdjustment.SetWasteEcrFlag( pDlg -> m_bWasteEcr != 0 );
			m_StockTreeReportAdjustment.PrepareArrays();
			m_StockTreeReportAdjustment.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportAdjustment.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportAdjustment.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportAdjustment.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportAdjustment.PrepareReport();
			m_StockTreeReportAdjustment.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSales.GetReportParamsFilename(), m_StockTreeReportSales.GetReportParamsKey(), this );
		}
		break;

	case TOP100_MESSAGE:
		switch( m_nReportType )
		{
			case STOCK_TOP_100_SALES:
				{
					CStockReportTop100Dlg* pDlg = (CStockReportTop100Dlg*) lParam;

					pDlg -> GetChartInfo( m_chartinfoSales );
					m_StockTreeReportTop100Sales.SetChartInfo( m_chartinfoSales );

					m_StockTreeReportTop100Sales.PrepareArrays();
					m_StockTreeReportTop100Sales.SetConLevelText( LocSelEntity.GetEntityName() );
					m_StockTreeReportTop100Sales.SetSupplierNameFlag( m_bSuppName != 0 );
					m_StockTreeReportTop100Sales.SetSupplierRefFlag( m_bSuppRef != 0 );
					m_StockTreeReportTop100Sales.SetStockUnitFlag( m_bStockUnit != 0 );
					m_StockTreeReportTop100Sales.PrepareReport();
					m_StockTreeReportTop100Sales.CreateReport();
			
					CReportManager ReportManager;
					ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportTop100Sales.GetReportParamsFilename(), m_StockTreeReportTop100Sales.GetReportParamsKey(), this );
				}
				break;
			
			case STOCK_TOP_100_APPARENT:
				{
					CStockReportTop100Dlg* pDlg = (CStockReportTop100Dlg*) lParam;

					pDlg -> GetChartInfo( m_chartinfoApparent );
					m_StockTreeReportTop100Apparent.SetChartInfo( m_chartinfoApparent );

					m_StockTreeReportTop100Apparent.PrepareArrays();
					m_StockTreeReportTop100Apparent.SetConLevelText( LocSelEntity.GetEntityName() );
					m_StockTreeReportTop100Apparent.SetSupplierNameFlag( m_bSuppName != 0 );
					m_StockTreeReportTop100Apparent.SetSupplierRefFlag( m_bSuppRef != 0 );
					m_StockTreeReportTop100Apparent.SetStockUnitFlag( m_bStockUnit != 0 );
					m_StockTreeReportTop100Apparent.PrepareReport();
					m_StockTreeReportTop100Apparent.CreateReport();
			
					CReportManager ReportManager;
					ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportTop100Sales.GetReportParamsFilename(), m_StockTreeReportTop100Sales.GetReportParamsKey(), this );
				}
				break;
		}
		break;
		
	case STOCK_REORDER:
		{
			CStockReportReorderDateDlg* pDlg = (CStockReportReorderDateDlg*) lParam;
			
			CString strDateFrom = pDlg -> m_strDateFrom;
			CString strDateTo = pDlg -> m_strDateTo;
			bool bReorder = ( pDlg -> m_bReorderItems != 0 );

			DataManager.Stock.GenerateFromAuditedSales( LocSelEntity, strDateTo, strDateFrom, bReorder );
			
			m_StockTreeReportReorder.SetDateFrom ( strDateFrom );
			m_StockTreeReportReorder.SetDateTo ( strDateTo );
			
			DataManager.Stock.SetQuickModeFlag(TRUE);
			
			m_StockTreeReportReorder.PrepareArrays();
			m_StockTreeReportReorder.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportReorder.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportReorder.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportReorder.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportReorder.PrepareReport();
			m_StockTreeReportReorder.CreateReport();
			
			DataManager.Stock.SetQuickModeFlag(FALSE);
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReorder.GetReportParamsFilename(), m_StockTreeReportReorder.GetReportParamsKey(), this );
		}
		break;
	}
	return 0L;
}

/**********************************************************************/

void CReportStockNewDlg::PrintAdjustmentReport()
{
	int nTimeType = m_comboPeriod.GetCurSel();
	m_StockTreeReportAdjustment.SetTimeType( nTimeType );
	CStockReportAdjustmentDlg dlg ( nTimeType, this );
	dlg.DoModal();
}

/**********************************************************************/

void CReportStockNewDlg::PrintApparentQtyReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportApparent.PrepareArrays();
		m_StockTreeReportApparent.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportApparent.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportApparent.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportApparent.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportApparent.SetDrawLinesFlag( m_bDrawLines != 0 );
		m_StockTreeReportApparent.PrepareReport();
		m_StockTreeReportApparent.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportApparent.GetReportParamsFilename(), m_StockTreeReportApparent.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_QTY, this );
		dlg.m_bHideAboveMin = m_StockTreeReportApparent.GetHideAboveMinFlag();
		dlg.m_bPositive = m_StockTreeReportApparent.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportApparent.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportApparent.GetNegativeFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintApparentSummaryReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportSummary.PrepareArrays();
		m_StockTreeReportSummary.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportSummary.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportSummary.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportSummary.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportSummary.SetDrawLinesFlag( m_bDrawLines != 0 );
		m_StockTreeReportSummary.PrepareReport();
		m_StockTreeReportSummary.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSummary.GetReportParamsFilename(), m_StockTreeReportSummary.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_SUMMARY, this );
		dlg.m_bHideAboveMin = m_StockTreeReportSummary.GetHideAboveMinFlag();
		dlg.m_bPositive = m_StockTreeReportSummary.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportSummary.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportSummary.GetNegativeFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintApparentQtyDUReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportApparentDU.PrepareArrays();
		m_StockTreeReportApparentDU.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportApparentDU.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportApparentDU.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportApparentDU.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportApparentDU.SetDrawLinesFlag( m_bDrawLines != 0 );
		m_StockTreeReportApparentDU.PrepareReport();
		m_StockTreeReportApparentDU.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportApparentDU.GetReportParamsFilename(), m_StockTreeReportApparentDU.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_QTY_DU, this );
		dlg.m_bHideAboveMin = m_StockTreeReportApparentDU.GetHideAboveMinFlag();
		dlg.m_bPositive = m_StockTreeReportApparentDU.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportApparentDU.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportApparentDU.GetNegativeFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintApparentSummaryDUReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportSummaryDU.PrepareArrays();
		m_StockTreeReportSummaryDU.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportSummaryDU.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportSummaryDU.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportSummaryDU.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportSummaryDU.SetDrawLinesFlag( m_bDrawLines != 0 );
		m_StockTreeReportSummaryDU.PrepareReport();
		m_StockTreeReportSummaryDU.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSummaryDU.GetReportParamsFilename(), m_StockTreeReportSummaryDU.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_SUMMARY_DU, this );
		dlg.m_bHideAboveMin = m_StockTreeReportSummaryDU.GetHideAboveMinFlag();
		dlg.m_bPositive = m_StockTreeReportSummaryDU.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportSummaryDU.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportSummaryDU.GetNegativeFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintCostSummaryReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportCostSummary.PrepareArrays();
	m_StockTreeReportCostSummary.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportCostSummary.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportCostSummary.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportCostSummary.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportCostSummary.PrepareReport();
	m_StockTreeReportCostSummary.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportCostSummary.GetReportParamsFilename(), m_StockTreeReportCostSummary.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintDeliveryReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportDelivery.PrepareArrays();
	m_StockTreeReportDelivery.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportDelivery.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportDelivery.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportDelivery.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportDelivery.PrepareReport();
	m_StockTreeReportDelivery.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportDelivery.GetReportParamsFilename(), m_StockTreeReportDelivery.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintEstimatedProfitReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportEstimatedProfit.PrepareArrays();
	m_StockTreeReportEstimatedProfit.SetTimeType( m_comboPeriod.GetCurSel() );
	m_StockTreeReportEstimatedProfit.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportEstimatedProfit.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportEstimatedProfit.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportEstimatedProfit.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportEstimatedProfit.PrepareReport();
	m_StockTreeReportEstimatedProfit.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportEstimatedProfit.GetReportParamsFilename(), m_StockTreeReportEstimatedProfit.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintStockPriceList()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportPriceList.PrepareArrays();
	m_StockTreeReportPriceList.SetConLevelText( dbDatabase.GetName( LocSelEntity.GetDbIdx() ) );
	m_StockTreeReportPriceList.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportPriceList.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportPriceList.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportPriceList.SetDrawLinesFlag( m_bDrawLines != 0 );
	m_StockTreeReportPriceList.PrepareReport();
	m_StockTreeReportPriceList.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportPriceList.GetReportParamsFilename(), m_StockTreeReportPriceList.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintReorderReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
	{
		m_StockTreeReportReorder.PrepareArrays();
		m_StockTreeReportReorder.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportReorder.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportReorder.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportReorder.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportReorder.PrepareReport();
		
		OpenStockLevels();
		
		m_StockTreeReportReorder.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReorder.GetReportParamsFilename(), m_StockTreeReportReorder.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportReorderDateDlg dlg ( this );
		dlg.DoModal();
	}
}

/**********************************************************************/
		
void CReportStockNewDlg::PrintRetailSummaryReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportRetailSummary.PrepareArrays();
	m_StockTreeReportRetailSummary.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportRetailSummary.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportRetailSummary.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportRetailSummary.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportRetailSummary.PrepareReport();
	m_StockTreeReportRetailSummary.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportRetailSummary.GetReportParamsFilename(), m_StockTreeReportRetailSummary.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintReturnsReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportReturns.PrepareArrays();
	m_StockTreeReportReturns.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportReturns.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportReturns.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportReturns.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportReturns.PrepareReport();
	m_StockTreeReportReturns.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReturns.GetReportParamsFilename(), m_StockTreeReportReturns.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintSalesReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	int nTimeType = m_comboPeriod.GetCurSel();

	if ( nTimeType == REPORT_PERIOD )
	{	
		CStockReportSalesDlg dlg( this );
		dlg.DoModal();
	}
	else
	{
		m_StockTreeReportYtdSales.PrepareArrays();
		m_StockTreeReportYtdSales.SetPeriodSalesFlag( nTimeType == REPORT_YTD_PLUS_PERIOD );
		m_StockTreeReportYtdSales.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportYtdSales.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportYtdSales.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportYtdSales.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportYtdSales.PrepareReport();
		m_StockTreeReportYtdSales.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportYtdSales.GetReportParamsFilename(), m_StockTreeReportYtdSales.GetReportParamsKey(), this );
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintStockSheet()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportStockSheet.PrepareArrays();
	m_StockTreeReportStockSheet.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportStockSheet.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportStockSheet.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportStockSheet.SetDrawLinesFlag( m_bDrawLines != 0 );
	m_StockTreeReportStockSheet.PrepareReport();
	m_StockTreeReportStockSheet.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportStockSheet.GetReportParamsFilename(), m_StockTreeReportStockSheet.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintSupplierReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportSupplier.PrepareArrays();
	m_StockTreeReportSupplier.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportSupplier.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportSupplier.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportSupplier.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportSupplier.SetDrawLinesFlag( m_bDrawLines != 0 );
	m_StockTreeReportSupplier.PrepareReport();
	
	OpenStockLevels();
	
	m_StockTreeReportSupplier.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportSupplier.GetReportParamsFilename(), m_StockTreeReportSupplier.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintTop100ReportApparent()
{
	CStockReportTop100Dlg dlg ( TOP100_STOCKLEVEL_APPARENT, "", m_chartinfoApparent, this );
	dlg.DoModal();
}

/**********************************************************************/

void CReportStockNewDlg::PrintTop100ReportSales()
{
	int nContext;
	int nTimeType = m_comboPeriod.GetCurSel();
	switch ( nTimeType )
	{
	case REPORT_YTD:			
		nContext = TOP100_STOCKLEVEL_SALES_YTD;				
		break;
	
	case REPORT_YTD_PLUS_PERIOD:	
		nContext = TOP100_STOCKLEVEL_SALES_YTD_PLUS_PERIOD;	
		break;
	
	case REPORT_PERIOD:
	default:					
		nContext = TOP100_STOCKLEVEL_SALES_PERIOD;		
		break;
	}

	m_StockTreeReportTop100Sales.SetTimeType( nTimeType );
	CStockReportTop100Dlg dlg ( nContext, "", m_chartinfoSales, this );
	dlg.DoModal();
}

/**********************************************************************/

void CReportStockNewDlg::PrintTransferPeriodReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportTransferPeriod.PrepareStockpointList();
	m_StockTreeReportTransferPeriod.PrepareArrays();
	m_StockTreeReportTransferPeriod.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportTransferPeriod.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportTransferPeriod.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportTransferPeriod.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportTransferPeriod.PrepareReport();
	m_StockTreeReportTransferPeriod.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportTransferPeriod.GetReportParamsFilename(), m_StockTreeReportTransferPeriod.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStockNewDlg::PrintApparentValuationReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportValuationApparent.PrepareArrays();
		m_StockTreeReportValuationApparent.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportValuationApparent.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportValuationApparent.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportValuationApparent.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportValuationApparent.PrepareReport();
		m_StockTreeReportValuationApparent.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationApparent.GetReportParamsFilename(), m_StockTreeReportValuationApparent.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_VALUE, this );
		dlg.m_bHideAboveMin = m_StockTreeReportValuationApparent.GetHideAboveMinFlag();
		dlg.m_bPositive = m_StockTreeReportValuationApparent.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportValuationApparent.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportValuationApparent.GetNegativeFlag();
		dlg.m_bZeroCost = m_StockTreeReportValuationApparent.GetZeroCostFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintOverstockValuationReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportValuationOverStock.PrepareArrays();
		m_StockTreeReportValuationOverStock.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportValuationOverStock.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportValuationOverStock.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportValuationOverStock.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportValuationOverStock.PrepareReport();
		m_StockTreeReportValuationOverStock.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationOverStock.GetReportParamsFilename(), m_StockTreeReportValuationOverStock.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_APPARENT_VALUE, this );
		dlg.m_bPositive = m_StockTreeReportValuationOverStock.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportValuationOverStock.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportValuationOverStock.GetNegativeFlag();
		dlg.m_bZeroCost = m_StockTreeReportValuationOverStock.GetZeroCostFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintOpeningValuationReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportValuationOpening.PrepareArrays();
		m_StockTreeReportValuationOpening.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportValuationOpening.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportValuationOpening.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportValuationOpening.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportValuationOpening.PrepareReport();
		m_StockTreeReportValuationOpening.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationOpening.GetReportParamsFilename(), m_StockTreeReportValuationOpening.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_OPENING_VALUE, this );
		dlg.m_bPositive = m_StockTreeReportValuationOpening.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportValuationOpening.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportValuationOpening.GetNegativeFlag();
		dlg.m_bZeroCost = m_StockTreeReportValuationOpening.GetZeroCostFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintTransferValuationReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportValuationTransfer.PrepareArrays();
		m_StockTreeReportValuationTransfer.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportValuationTransfer.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportValuationTransfer.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportValuationTransfer.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportValuationTransfer.PrepareReport();
		m_StockTreeReportValuationTransfer.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationTransfer.GetReportParamsFilename(), m_StockTreeReportValuationTransfer.GetReportParamsKey(), this );	
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_TRANSFER_VALUE, this );
		dlg.m_bPositive = m_StockTreeReportValuationTransfer.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportValuationTransfer.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportValuationTransfer.GetNegativeFlag();
		dlg.m_bZeroCost = m_StockTreeReportValuationTransfer.GetZeroCostFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintOnOrderReportSupplier()
{
	CWordArray wSuppSelectArray;
	if ( GetSupplierIdxArray ( wSuppSelectArray ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CProcessedOrderArray dummy;
	CProcessedOrderReportOnOrderSupplier OnOrderReport( wSuppSelectArray, dummy );
			
	OnOrderReport.SetDateFrom ( "" );
	OnOrderReport.SetDateTo ( "" );
	OnOrderReport.SetCheckAllOrderFoldersFlag( TRUE );
	OnOrderReport.SetReportType ( m_nReportType );

	OnOrderReport.SetConLevel ( LocSelEntity.GetConType() );		
	OnOrderReport.SetTableNo ( LocSelEntity.GetEntityNo() );
	OnOrderReport.SetConLevelText ( LocSelEntity.GetEntityName() );

	int nCatFilter = m_CategorySelector.GetCategoryFilter();
	OnOrderReport.SetCategoryFilter( nCatFilter );
	
	if ( m_bStockCodes == TRUE )
	{
		OnOrderReport.SetStockCodeFrom ( m_strStockCodeFrom );
		OnOrderReport.SetStockCodeTo ( m_strStockCodeTo );
	}
	else
	{
		OnOrderReport.SetStockCodeFrom ( "" );
		OnOrderReport.SetStockCodeTo ( "" );
	}

	//CONVERT OLD ORDERS
	COrderCSVArray OldOrders;
	if ( OldOrders.UpdateDatabaseOrders( LocSelEntity.GetDbIdx() ) == FALSE )
	{
		OldOrders.ShowUpgradeError();
		return;
	}

	int nSpIdxStart, nSpIdxEnd;
	dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nSpIdxStart, nSpIdxEnd );

	for ( int nSpIdx = nSpIdxStart; nSpIdx <= nSpIdxEnd; nSpIdx++ )
	{
		COrderCSVArray OldOrders;
		if ( OldOrders.UpdateStockpointOrders( nSpIdx ) == FALSE )
		{
			OldOrders.ShowUpgradeError();
			return;
		}
	}
			
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = ( OnOrderReport.CreateReport( LocSelEntity.GetDbIdx() ) );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), OnOrderReport.GetReportParamsFilename ( m_nReportType ), "", this );
	}
}

/**********************************************************************/

void CReportStockNewDlg::PrintOnOrderReportCategory()
{
	CWordArray wSuppSelectArray;
	if ( GetSupplierIdxArray ( wSuppSelectArray ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	CProcessedOrderReportOnOrderCategory OnOrderReport( wSuppSelectArray );
			
	OnOrderReport.SetDateFrom ( "" );
	OnOrderReport.SetDateTo ( "" );
	OnOrderReport.SetReportType ( m_nReportType );

	OnOrderReport.SetConLevel ( LocSelEntity.GetConType() );		
	OnOrderReport.SetTableNo ( LocSelEntity.GetEntityNo() );
	OnOrderReport.SetConLevelText ( LocSelEntity.GetEntityName() );
	
	int nCatFilter = m_CategorySelector.GetCategoryFilter();
	OnOrderReport.SetCategoryFilter( nCatFilter );
	
	if ( m_bStockCodes == TRUE )
	{
		OnOrderReport.SetStockCodeFrom ( m_strStockCodeFrom );
		OnOrderReport.SetStockCodeTo ( m_strStockCodeTo );
	}
	else
	{
		OnOrderReport.SetStockCodeFrom ( "" );
		OnOrderReport.SetStockCodeTo ( "" );
	}

	//CONVERT OLD ORDERS
	COrderCSVArray OldOrders;
	if ( OldOrders.UpdateDatabaseOrders( LocSelEntity.GetDbIdx() ) == FALSE )
	{
		OldOrders.ShowUpgradeError();
		return;
	}
	
	int nSpIdxStart, nSpIdxEnd;
	dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nSpIdxStart, nSpIdxEnd );

	for ( int nSpIdx = nSpIdxStart; nSpIdx <= nSpIdxEnd; nSpIdx++ )
	{
		COrderCSVArray OldOrders;
		if ( OldOrders.UpdateStockpointOrders( nSpIdx ) == FALSE )
		{
			OldOrders.ShowUpgradeError();
			return;
		}
	}
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = ( OnOrderReport.CreateReport( LocSelEntity.GetDbIdx() ) );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), OnOrderReport.GetReportParamsFilename ( m_nReportType ), "", this );
	}
}

/**********************************************************************/

