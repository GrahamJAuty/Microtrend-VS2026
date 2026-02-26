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
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "..\CommonEcrStock\SelectPresetActionDlg.h"
#include "SiteCSVArray.h"
#include "StatusProgress.h"
#include "StockFilterArray.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "StockReportAllowanceDlg.h"
#include "StockReportApparentDlg.h"
#include "StockReportNames.h"
#include "StockReportSortOrderEditDlg.h"
#include "StockReportSupplierDlg.h"
#include "StockReportVarianceDlg.h"
#include "StocktakeCSVArray.h"
#include "SupplierSetCSVArray.h"
/**********************************************************************/
#include "ReportStocktakeNewDlg.h"
/**********************************************************************/

CReportStocktakeNewDlg::CReportStocktakeNewDlg( CWnd* pParent )
	: CSSDialog("IDD_STOCK_REPORT_STOCKTAKE", pParent),
	m_StockTreeReportEstimatedAllowance( m_StockLevelTree ),
	m_StockTreeReportReconcile( m_StockLevelTree ),
	m_StockTreeReportReconcileCash( m_StockLevelTree ),
	m_StockTreeReportReconcileCategory( m_StockLevelTree ),
	m_StockTreeReportValuationClosing( m_StockLevelTree ),
	m_StockTreeReportVariance( m_StockLevelTree ),
	m_StockTreeReportTransferClosing( m_StockLevelTree ),
	m_StockTreeReportStockSheet( m_StockLevelTree ),
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

	//{{AFX_DATA_INIT(CReportStocktakeNewDlg)
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	m_bStockCodes = FALSE;
	m_bSuppRef = FALSE;
	m_bSuppName = FALSE;
	m_bStockUnit = FALSE;
	m_bExcludeInactive = FALSE;
	m_bShowItems = TRUE;
	m_bDrawLines = FALSE;
	//}}AFX_DATA_INIT
	m_bSelectShowItem = TRUE;
	m_bSelectDrawLines = FALSE;
	m_bSelectSuppRef = TRUE;
	m_bSelectStockUnit = TRUE;
	m_bSelectExcludeInactive = TRUE;
	m_nReportType = STOCK_CLOSING_VALUE;
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_bStockCodes = FALSE;
	m_nDefSelectIdx = 0;
	m_nDefSelectType = NODE_SYSTEM;
}

/**********************************************************************/

void CReportStocktakeNewDlg::SetDefaultSelection( int nDefSelectIdx, int nDefSelectType, int nDefStocktake, bool bFixStockpoint )
{
	m_nDefSelectIdx = nDefSelectIdx;
	m_nDefSelectType = nDefSelectType;
	m_nDefStocktake = nDefStocktake;
	m_bFixStockpoint = bFixStockpoint;
}

/**********************************************************************/

void CReportStocktakeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportStocktakeNewDlg)

	DDX_Control(pDX, IDC2_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC2_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC2_COMBO_PRESET, m_comboPreset);
	
	DDX_Control(pDX, IDC2_BUTTON_SORTORDER, m_buttonSortOrder);
	DDX_Control(pDX, IDC2_COMBO_SORTORDER, m_comboSortOrder);
	DDX_Control(pDX, IDC2_CHECK_SHOWITEMS, m_checkShowItems);
	
	DDX_Control(pDX, IDC2_COMBO_STOCKTAKE, m_comboStocktake);
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

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportStocktakeNewDlg, CDialog)
	//{{AFX_MSG_MAP(CReportStocktakeNewDlg)

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

BOOL CReportStocktakeNewDlg::OnInitDialog() 
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
	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( DataManager.GetActiveDbIdx() );
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
	
	m_comboConLevel.EnableWindow( FALSE == m_bFixStockpoint );
	
	m_nCategoryDbIdx = -1;
	m_nCustomFieldDbIdx = -1;
	
	PresetReportManager.StockClosing.SetPasswordAccess();
	FillPresetCombo(-1);
	
	FillSortOrderCombo( SORTORDER_SELECT_INDEX, 0 );
	m_CategoryComboHelpers.UpdateCombos();
	FillSupplierCombo();
	InitialiseCustomFieldCombos();
	FillCustomFieldCombos();
	
	CWordArray arrayReportTypes;
	arrayReportTypes.Add( STOCK_CLOSING_VALUE );
	arrayReportTypes.Add( STOCK_VARIANCE );
	arrayReportTypes.Add( STOCK_ESTIMATE_ALLOWANCE );
	arrayReportTypes.Add( STOCK_RECONCILE );
	arrayReportTypes.Add( STOCK_RECONCILE_CATEGORY );
	arrayReportTypes.Add( STOCK_RECONCILE_CASH );
	arrayReportTypes.Add( STOCK_TRANSFER_CLOSING );
	arrayReportTypes.Add( STOCK_SHEET );

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

	if ( m_nDefStocktake >= 0 && m_nDefStocktake < m_comboStocktake.GetCount() - 1 )
		m_comboStocktake.SetCurSel( m_nDefStocktake + 1 );

	m_editStockCodeFrom.EnableWindow ( FALSE );
	m_editStockCodeTo.EnableWindow ( FALSE );
	m_buttonBrowseFrom.EnableWindow ( FALSE );
	m_buttonBrowseTo.EnableWindow ( FALSE );

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );

	return TRUE;  
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC2_LIST )	
	{
		m_listReports.AdjustRowHeight( lpMeasureItemStruct, 8 );
	}
}

/**********************************************************************/

void CReportStocktakeNewDlg::InitialiseCustomFieldCombos()
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

	DataManager.CustomFieldManager.SetIncludeAllFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeNAFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeSetsFlag( TRUE );

	for ( int nSlot = 0; nSlot < 5; nSlot++ )
		DataManager.CustomFieldManager.SetComboSelector( nSlot, &m_comboCustomField[ nSlot ] );
}

/**********************************************************************/

void CReportStocktakeNewDlg::SetEnables()
{	
	//we can only select categories if we have two or more of them
	m_comboCategory.EnableWindow ( m_comboCategory.GetCount() > 1 );
	m_checkSuppRef.EnableWindow ( m_bSelectSuppRef );
	m_checkSuppName.EnableWindow ( m_bSelectSuppRef );
	m_checkStockUnit.EnableWindow( m_bSelectStockUnit );
	m_checkShowItems.EnableWindow ( m_bSelectShowItem );
	m_checkExcludeInactive.EnableWindow ( m_bSelectExcludeInactive );
	m_checkDrawLines.EnableWindow( m_bSelectDrawLines );
}

/**********************************************************************/

void CReportStocktakeNewDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_STOCK_CLOSING, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

int CReportStocktakeNewDlg::GetSortOrderTypeFromReportType( int nReportType )
{
	int nSortOrderType = STOCKTAKE_SORTORDER_CLOSING_VALUE;
	switch( nReportType )
	{
	case STOCK_RECONCILE_CASH:			nSortOrderType = -1;										break;
	case STOCK_CLOSING_VALUE:			nSortOrderType = STOCKTAKE_SORTORDER_CLOSING_VALUE;			break;
	case STOCK_VARIANCE:				nSortOrderType = STOCKTAKE_SORTORDER_VARIANCE;				break;
	case STOCK_RECONCILE:				nSortOrderType = STOCKTAKE_SORTORDER_RECONCILE;				break;
	case STOCK_RECONCILE_CATEGORY:		nSortOrderType = STOCKTAKE_SORTORDER_RECONCILE_CATEGORY;	break;
	case STOCK_ESTIMATE_ALLOWANCE:		nSortOrderType = STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE;	break;
	case STOCK_TRANSFER_CLOSING:		nSortOrderType = STOCKTAKE_SORTORDER_TRANSFER_CLOSING;		break;
	case STOCK_SHEET:					nSortOrderType = STOCKTAKE_SORTORDER_SHEET;					break;
	}

	return nSortOrderType;
}

/**********************************************************************/

void CReportStocktakeNewDlg::FillSortOrderCombo( int nType, int nSelection )
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

	switch( m_nReportType )
	{
	case STOCK_RECONCILE_CASH:
		m_arraySortOrder.AddSortOrder( SH_STKLVL_SUPP );
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

void CReportStocktakeNewDlg::FillSupplierCombo()
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

void CReportStocktakeNewDlg::FillCustomFieldCombos()
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

bool CReportStocktakeNewDlg::GetSupplierIdxArray( CWordArray& wArray )
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

void CReportStocktakeNewDlg::OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectReport();
	}
}

/**********************************************************************/

void CReportStocktakeNewDlg::SelectReport() 
{
	UpdateData( TRUE, FALSE );

	//default settings
	m_bSelectShowItem = TRUE;
	m_bSelectDrawLines = FALSE;
	m_bSelectSuppRef = TRUE;
	m_bSelectStockUnit = TRUE;
	m_bSelectExcludeInactive = TRUE;

	int nSelected = m_listReports.GetCurSel();
	if ( nSelected >= 0 && nSelected < m_listReports.GetItemCount() )
		m_nReportType = m_listReports.GetItemData ( nSelected );

	//special settings for categories
	switch ( m_nReportType )
	{
	case STOCK_RECONCILE_CATEGORY:
	case STOCK_RECONCILE_CASH:
	case STOCK_SHEET:
		m_bSelectShowItem = FALSE;
		break;
	}
	
	//special settings for supplier reference
	switch ( m_nReportType )
	{
	case STOCK_RECONCILE_CATEGORY:
	case STOCK_RECONCILE_CASH:
		m_bSelectSuppRef = FALSE;
		break;
	}

	//special settings for stock unit
	switch ( m_nReportType )
	{
	case STOCK_RECONCILE_CATEGORY:
	case STOCK_RECONCILE_CASH:
	case STOCK_SHEET:
		m_bSelectStockUnit = FALSE;
		break;
	}

	//special settings for exclude inactive items
	switch ( m_nReportType )
	{
	case STOCK_RECONCILE_CATEGORY:
	case STOCK_RECONCILE_CASH:
	case STOCK_TRANSFER_CLOSING:
		m_bSelectExcludeInactive = FALSE;
		break;
	} 
	
	//special settings for lines between items
	switch( m_nReportType )
	{
	case STOCK_SHEET:
		m_bSelectDrawLines = TRUE;
		break;
	}

	SetEnables();
	FillSortOrderCombo( SORTORDER_SELECT_CURRENT, 0 );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_STOCK_CLOSING, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportStocktakeNewDlg::CreatePreset( CPresetReportInfo& Base ) 
{
	CPresetReportInfoStockLevel Preset;

	int nSel = m_listReports.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listReports.GetItemCount() ) )
		Preset.SetReportType( m_listReports.GetItemData( nSel ) );

	nSel = m_comboSortOrder.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboSortOrder.GetCount() ) )
		Preset.SetSortOrder( m_comboSortOrder.GetItemData( nSel ) );

	Preset.SetShowItemsFlag( m_checkShowItems.GetCheck() != 0 );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );
	
	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	nSel = m_comboStocktake.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listReports.GetItemCount() ) )
		Preset.SetStocktake( m_comboStocktake.GetItemData( nSel ) );

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

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.StockClosing.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.StockClosing.GetAt( nSel, Base );

	CPresetReportInfoStockLevel Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

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

	if ( FALSE == m_bFixStockpoint )
	{
		int nConLevel = Base.GetPasswordConLevel();
		int nEntity1 = Base.GetPasswordEntityNo1();
		int nEntity2 = Base.GetPasswordEntityNo2();
	
		int nEntityIdx = 0;
		CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
		m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );
	}

	//MUST DO THIS AFTER CONSOLIDATION LEVEL
	int nStocktake = Preset.GetStocktake();
	for ( int n = 0; n < m_comboStocktake.GetCount(); n++ )
	{
		if ( m_comboStocktake.GetItemData(n) == nStocktake )
		{
			m_comboStocktake.SetCurSel(n);
			break;
		}
	}

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
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnButtonSortOrder() 
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
			case STOCK_RECONCILE_CASH:
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

void CReportStocktakeNewDlg::OnSelectConLevel() 
{
	UpdateData( TRUE, FALSE );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if (( LocSelEntity.GetConType() == m_LocSelEntityOld.GetConType() ) && 
		( LocSelEntity.GetDbIdx() == m_LocSelEntityOld.GetDbIdx() ) && 
		( LocSelEntity.GetEntityNo() == m_LocSelEntityOld.GetEntityNo() ) )
		return;

	m_LocationSelector.GetSelectedEntity( m_LocSelEntityOld );

	m_CategoryComboHelpers.UpdateCombos();

	if ( StockOptions.GetGlobalCustomFieldFlag() == FALSE )
		FillCustomFieldCombos();
	
	m_comboStocktake.ResetContent();
	m_comboStocktake.AddString( "All Stock Records" );
	m_comboStocktake.SetItemData( 0, 0 );

	if ( LocSelEntity.GetConType() == NODE_STOCKPOINT )
	{
		int nSpIdx;
		int nDbNo = dbDatabase.GetDbNo( LocSelEntity.GetDbIdx() );
		if ( dbStockpoint.FindStockpointByNumber( nDbNo, LocSelEntity.GetEntityNo(), nSpIdx ) == TRUE )
		{
			if ( nSpIdx == m_nDefSelectIdx )
			{
				CStocktakeCSVArray StocktakeArray;
				StocktakeArray.Read( nSpIdx );

				for ( int nIndex = 0; nIndex < StocktakeArray.GetSize(); nIndex++ )
				{
					CStocktakeCSVRecord Stocktake;
					StocktakeArray.GetAt( nIndex, Stocktake );
					m_comboStocktake.AddString( Stocktake.GetComment() );
					m_comboStocktake.SetItemData( nIndex + 1, Stocktake.GetStocktakeNo() );
				}
			}
		}
	}

	if ( m_comboStocktake.GetCount() >= 2 )
		m_comboStocktake.SetCurSel(1);
	else
		m_comboStocktake.SetCurSel(0);

	m_comboStocktake.EnableWindow( m_comboStocktake.GetCount() > 1 );

	SetEnables();
	
	UpdateData( FALSE );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnSelectCategory() 
{
	UpdateData( TRUE, FALSE );
	SetEnables();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnToggleAllStock() 
{
	UpdateData( TRUE, FALSE );
	m_editStockCodeFrom.EnableWindow ( m_bStockCodes );
	m_editStockCodeTo.EnableWindow ( m_bStockCodes );
	m_buttonBrowseFrom.EnableWindow ( m_bStockCodes );
	m_buttonBrowseTo.EnableWindow ( m_bStockCodes );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnButtonBrowseFrom() 
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

void CReportStocktakeNewDlg::OnButtonBrowseTo() 
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

void CReportStocktakeNewDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonPrint();
	*pResult = 0;
}

/**********************************************************************/

void CReportStocktakeNewDlg::OnButtonPrint() 
{
	UpdateData( TRUE, FALSE );
	OnKillFocusStockCode();
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
	
	if ( STOCK_TRANSFER_CLOSING == m_nReportType )
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

	bool bShowItems = ( m_bShowItems != 0 );

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

	int nStocktakeNo = m_comboStocktake.GetItemData( m_comboStocktake.GetCurSel() );

	switch( m_nReportType )
	{
	case STOCK_RECONCILE_CASH:
		if ( arraySuppNo.GetSize() > DataManager.Supplier.GetSize() )
			nSortOrder = SH_STKLVL_NONE;
		break;

	case STOCK_RECONCILE_CATEGORY:
		bShowItems = FALSE;
		break;

	case STOCK_SHEET:
		bShowItems = TRUE;
		break;
	}

	bool bWantTransferDetail = ( STOCK_TRANSFER_CLOSING == m_nReportType );
	m_StockLevelTree.Configure( ( m_bAlphaSort ? 1 : 0 ), NULL, LocSelEntity, nStocktakeNo, arraySortOrder, bShowItems, nCatFilter, strFrom, strTo, bExcludeInactive, FALSE, arraySuppNo, FALSE, bWantTransferDetail, FALSE );
	
	m_StockLevelTree.Consolidate();
	
	switch ( m_nReportType )
	{
	case STOCK_CLOSING_VALUE:		PrintClosingValuationReport();	break;
	case STOCK_VARIANCE:			PrintVarianceReport();			break;
	case STOCK_RECONCILE:			PrintReconcileReport();			break;
	case STOCK_ESTIMATE_ALLOWANCE:	PrintEstimateAllowanceReport();	break;
	case STOCK_RECONCILE_CASH:		PrintReconcileCashReport();		break;
	case STOCK_RECONCILE_CATEGORY:	PrintReconcileCategoryReport();	break;
	case STOCK_TRANSFER_CLOSING:	PrintTransferDetailReport();	break;
	case STOCK_SHEET:				PrintStockSheet();				break;							
	}
}		

/**********************************************************************/

long CReportStocktakeNewDlg::OnReportMessage( WPARAM wIndex, LPARAM lParam )
{ 
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	switch ( wIndex )
	{
	case STOCK_RECONCILE_CASH:
		{
			CStockReportAllowanceDlg* pDlg = (CStockReportAllowanceDlg*) lParam;
			
			for ( int n = 0; n < 9; n++ )
				m_StockTreeReportReconcileCash.SetAllowanceValue ( n + 1, pDlg -> m_dAllowance[n] );
			
			m_StockTreeReportReconcileCash.SetAllowanceTax ( pDlg -> m_dAllowanceTax );
			m_StockTreeReportReconcileCash.SetCashTakings ( pDlg -> m_dCash );
			m_StockTreeReportReconcileCash.SetCashTakingsTax ( pDlg -> m_dCashTax );
			m_StockTreeReportReconcileCash.PrepareArrays();
			m_StockTreeReportReconcileCash.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportReconcileCash.PrepareReport();
			m_StockTreeReportReconcileCash.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReconcileCash.GetReportParamsFilename(), m_StockTreeReportReconcileCash.GetReportParamsKey(), this );
		}
		break;

	case STOCK_VARIANCE:
		{
			CStockReportVarianceDlg* pDlg = (CStockReportVarianceDlg*) lParam;
			
			m_StockTreeReportVariance.PrepareArrays();
			m_StockTreeReportVariance.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportVariance.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportVariance.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportVariance.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportVariance.SetZeroSkipFlag( pDlg -> m_bZeroSkip != 0 );
			m_StockTreeReportVariance.PrepareReport();
			m_StockTreeReportVariance.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportVariance.GetReportParamsFilename(), m_StockTreeReportVariance.GetReportParamsKey(), this );
		}
		break;

	case STOCK_CLOSING_VALUE:
		{
			CStockReportApparentDlg* pDlg = (CStockReportApparentDlg*) lParam;

			m_StockTreeReportValuationClosing.PrepareArrays();
			m_StockTreeReportValuationClosing.SetConLevelText( LocSelEntity.GetEntityName() );
			m_StockTreeReportValuationClosing.SetSupplierNameFlag( m_bSuppName != 0 );
			m_StockTreeReportValuationClosing.SetSupplierRefFlag( m_bSuppRef != 0 );
			m_StockTreeReportValuationClosing.SetStockUnitFlag( m_bStockUnit != 0 );
			m_StockTreeReportValuationClosing.SetPositiveFlag( pDlg -> m_bPositive != 0 );
			m_StockTreeReportValuationClosing.SetZeroFlag( pDlg -> m_bZero != 0 );
			m_StockTreeReportValuationClosing.SetNegativeFlag( pDlg -> m_bNegative != 0 );
			m_StockTreeReportValuationClosing.SetZeroCostFlag( pDlg -> m_bZeroCost != 0 );
			m_StockTreeReportValuationClosing.PrepareReport();
			m_StockTreeReportValuationClosing.CreateReport();

			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationClosing.GetReportParamsFilename(), m_StockTreeReportValuationClosing.GetReportParamsKey(), this );
		}
		break;

	}
	return 0L;
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintClosingValuationReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( DealerFlags.GetStockLevelFilterFlag() == FALSE )
	{
		m_StockTreeReportValuationClosing.PrepareArrays();
		m_StockTreeReportValuationClosing.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportValuationClosing.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportValuationClosing.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportValuationClosing.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportValuationClosing.PrepareReport();
		m_StockTreeReportValuationClosing.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportValuationClosing.GetReportParamsFilename(), m_StockTreeReportValuationClosing.GetReportParamsKey(), this );
	}
	else
	{
		CStockReportApparentDlg dlg( STOCK_CLOSING_VALUE, this );
		dlg.m_bPositive = m_StockTreeReportValuationClosing.GetPositiveFlag();
		dlg.m_bZero = m_StockTreeReportValuationClosing.GetZeroFlag();
		dlg.m_bNegative = m_StockTreeReportValuationClosing.GetNegativeFlag();
		dlg.m_bZeroCost = m_StockTreeReportValuationClosing.GetZeroCostFlag();
		dlg.DoModal();
	}
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintVarianceReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( m_bShowItems == TRUE )
	{
		CStockReportVarianceDlg dlg ( this );
		dlg.DoModal();
	}
	else
	{
		m_StockTreeReportVariance.PrepareArrays();
		m_StockTreeReportVariance.SetConLevelText( LocSelEntity.GetEntityName() );
		m_StockTreeReportVariance.SetSupplierNameFlag( m_bSuppName != 0 );
		m_StockTreeReportVariance.SetSupplierRefFlag( m_bSuppRef != 0 );
		m_StockTreeReportVariance.SetStockUnitFlag( m_bStockUnit != 0 );
		m_StockTreeReportVariance.SetZeroSkipFlag( FALSE );
		m_StockTreeReportVariance.PrepareReport();
		m_StockTreeReportVariance.CreateReport();

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportVariance.GetReportParamsFilename(), m_StockTreeReportVariance.GetReportParamsKey(), this );
	}
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintReconcileReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportReconcile.PrepareArrays();
	m_StockTreeReportReconcile.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportReconcile.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportReconcile.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportReconcile.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportReconcile.PrepareReport();
	m_StockTreeReportReconcile.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReconcile.GetReportParamsFilename(), m_StockTreeReportReconcile.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintEstimateAllowanceReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportEstimatedAllowance.PrepareArrays();
	m_StockTreeReportEstimatedAllowance.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportEstimatedAllowance.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportEstimatedAllowance.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportEstimatedAllowance.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportEstimatedAllowance.PrepareReport();
	m_StockTreeReportEstimatedAllowance.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportEstimatedAllowance.GetReportParamsFilename(), m_StockTreeReportEstimatedAllowance.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintReconcileCategoryReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportReconcileCategory.PrepareArrays();
	m_StockTreeReportReconcileCategory.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportReconcileCategory.PrepareReport();
	m_StockTreeReportReconcileCategory.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportReconcileCategory.GetReportParamsFilename(), m_StockTreeReportReconcileCategory.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintReconcileCashReport()
{
	CStockReportAllowanceDlg dlg ( this );
	dlg.DoModal();
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintTransferDetailReport()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportTransferClosing.PrepareStockpointList();
	m_StockTreeReportTransferClosing.PrepareArrays();
	m_StockTreeReportTransferClosing.SetConLevelText( LocSelEntity.GetEntityName() );
	m_StockTreeReportTransferClosing.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportTransferClosing.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportTransferClosing.SetStockUnitFlag( m_bStockUnit != 0 );
	m_StockTreeReportTransferClosing.PrepareReport();
	m_StockTreeReportTransferClosing.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportTransferClosing.GetReportParamsFilename(), m_StockTreeReportTransferClosing.GetReportParamsKey(), this );
}

/**********************************************************************/

void CReportStocktakeNewDlg::PrintStockSheet()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_StockTreeReportStockSheet.PrepareArrays();
	m_StockTreeReportStockSheet.SetConLevelText( dbDatabase.GetName( LocSelEntity.GetDbIdx() ) );
	m_StockTreeReportStockSheet.SetSupplierNameFlag( m_bSuppName != 0 );
	m_StockTreeReportStockSheet.SetSupplierRefFlag( m_bSuppRef != 0 );
	m_StockTreeReportStockSheet.SetDrawLinesFlag( m_bDrawLines != 0 );
	m_StockTreeReportStockSheet.PrepareReport();
	m_StockTreeReportStockSheet.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), m_StockTreeReportStockSheet.GetReportParamsFilename(), m_StockTreeReportStockSheet.GetReportParamsKey(), this );
}

/**********************************************************************/
