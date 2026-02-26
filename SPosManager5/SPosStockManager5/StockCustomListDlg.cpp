/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "Password.h"
#include "RepSpawn.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockReportSupplierDlg.h"
/**********************************************************************/
#include "StockCustomListDlg.h"
/**********************************************************************/

CStockCustomListDlg::CStockCustomListDlg( CStockFilterArray& StockFilter, int nRepNum, CWnd* pParent )
	: CSSDialog(CStockCustomListDlg::IDD, pParent),
	m_StockFilter( StockFilter ),
	m_StockTreeReportCustomList( m_StockLevelTree )
{
	//{{AFX_DATA_INIT(CStockCustomListDlg)
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	m_bExcludeInactive = FALSE;
	m_bDrawLines = FALSE;
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_bSpecifyStockCodes = FALSE;

	m_nCategorySelect = FIRST_CATEGORYSET_FILTER - 1;

	m_nRepNum = nRepNum;
	m_bFirstTime = TRUE;

	CFilenameUpdater FnUp( SysFiles::StockCustomDef, 0, m_nRepNum );
	m_strFilenameSettings = FnUp.GetFilenameToUse();

	m_StockTreeReportCustomList.GetCSVReportName( m_nRepNum, m_strName );
}

/**********************************************************************/

void CStockCustomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockCustomListDlg)
	DDX_Control(pDX, IDC_CHECK_DRAWLINES, m_checkDrawLines);
	DDX_Control(pDX, IDC_COMBO_SORTORDER, m_comboSortOrder);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkExcludeInactive);
	DDX_Control(pDX, IDC_BUTTON_BROWSETO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSEFROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE_TO, m_editStockCodeTo);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE_FROM, m_editStockCodeFrom);
	DDX_Control(pDX, IDC_STATIC_CATEGORY, m_staticCategory);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_CONLEVEL, m_comboConLevel);
	DDX_Check(pDX, IDC_CHECK_ALLSTOCK, m_bSpecifyStockCodes);
	DDX_Check(pDX, IDC_CHECK_INACTIVE, m_bExcludeInactive);
	DDX_Check(pDX, IDC_CHECK_DRAWLINES, m_bDrawLines);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 25);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_FROM, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_TO, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockCustomListDlg, CDialog)
	//{{AFX_MSG_MAP(CStockCustomListDlg)
	ON_WM_MEASUREITEM()
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_CHECK_ALLSTOCK, OnToggleAllStock)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEFROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSETO, OnButtonBrowseTo)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_RESETORDER, OnButtonResetOrder)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockCustomListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strTitle;
	strTitle.Format( "Print Stock Custom List %d (%s)",
		m_nRepNum,
		(const char*) m_strName );

	SetWindowText( strTitle );

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
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboConLevel );

	if (PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC)
	{
		m_LocationSelector.ForceSelection(NODE_DATABASE, DataManager.GetActiveDbIdx());
	}

	if (m_comboConLevel.GetCount() < 3)
	{
		m_comboConLevel.EnableWindow(FALSE);
	}

	m_nCategoryDbIdx = -1;
	FillCategoryCombo();
	FillSupplierCombo();
	
	m_editStockCodeFrom.EnableWindow ( FALSE );
	m_editStockCodeTo.EnableWindow ( FALSE );
	m_buttonBrowseFrom.EnableWindow ( FALSE );
	m_buttonBrowseTo.EnableWindow ( FALSE );

	FillSortOrderCombo();

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 200 );
	m_listFields.SetLockItemZeroFlag( TRUE );
	CreateAvailableFieldList();
		
	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer = "";
	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strBuffer );
		fileFields.Close();
	}
	else
	{
		strBuffer = "";
	}

	//STOCK CODE IS ALREADY SELECTED
	AddFieldToList( CustomStockList::StockCode.ListLabel, CustomStockList::StockCode.Type, 1 );

	//TICK THE OPTIONS THAT WERE FOUND IN THE FILE
	CCSV csvFields ( strBuffer ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
	{
		int nField = csvFields.GetInt(nPos);
		
		switch ( nField )
		{
		case 1000: m_checkDrawLines.SetCheck( TRUE );		break;
		case 1001: m_checkExcludeInactive.SetCheck( TRUE );	break;
		case 9999: nPos++; /*SKIP NAME*/					break;
		default:
			{
				CSortedStringByInt item;
				item.m_nItem = nField;

				int nFindPos;
				if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
				{
					m_arrayAvailableFields.GetAt( nFindPos, item );
					AddFieldToList( item.m_strItem, item.m_nItem, 1 );
					m_arrayAvailableFields.RemoveAt(nFindPos);
				}
			}	
			break;
		}
	}

	//ADD THE REMAINING UNTICKED ITEMS
	for ( int n = 0; n < m_arrayDefaultFields.GetSize(); n++ )
	{
		CSortedStringByInt item;
		item.m_nItem = m_arrayDefaultFields.GetAt(n);

		int nFindPos;
		if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFindPos, item );
			AddFieldToList( item.m_strItem, item.m_nItem, 0 );
			m_arrayAvailableFields.RemoveAt(nFindPos);
		}
	}

	if ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_NONE )
	{
		m_checkExcludeInactive.SetCheck( FALSE );
		m_checkExcludeInactive.EnableWindow( FALSE );
		m_checkExcludeInactive.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CStockCustomListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockCustomListDlg::CreateAvailableFieldList()
{
	m_arrayAvailableFields.RemoveAll();
	m_arrayDefaultFields.RemoveAll();

	AddAvailableField ( CustomStockList::Description );
	AddAvailableField ( CustomStockList::CatName );
	AddAvailableField ( CustomStockList::ImportSheetCode );
	
	if (DataManagerNonDb.CustomFieldNames.CheckName(1) == TRUE)
	{
		if (DataManager.CustomFieldValue[1].GetSize() > 0)
			AddAvailableField(CustomStockList::CustomFieldValue1, DataManagerNonDb.CustomFieldNames.GetName(1));
	}

	if (DataManagerNonDb.CustomFieldNames.CheckName(2) == TRUE)
	{
		if (DataManager.CustomFieldValue[2].GetSize() > 0)
		{
			AddAvailableField(CustomStockList::CustomFieldValue2, DataManagerNonDb.CustomFieldNames.GetName(2));
		}
	}

	if (DataManagerNonDb.CustomFieldNames.CheckName(3) == TRUE)
	{
		if (DataManager.CustomFieldValue[3].GetSize() > 0)
		{
			AddAvailableField(CustomStockList::CustomFieldValue3, DataManagerNonDb.CustomFieldNames.GetName(3));
		}
	}

	if (DataManagerNonDb.CustomFieldNames.CheckName(4) == TRUE)
	{
		if (DataManager.CustomFieldValue[4].GetSize() > 0)
		{
			AddAvailableField(CustomStockList::CustomFieldValue4, DataManagerNonDb.CustomFieldNames.GetName(4));
		}
	}

	if (DataManagerNonDb.CustomFieldNames.CheckName(5) == TRUE)
	{
		if (DataManager.CustomFieldValue[5].GetSize() > 0)
		{
			AddAvailableField(CustomStockList::CustomFieldValue5, DataManagerNonDb.CustomFieldNames.GetName(5));
		}
	}

	AddAvailableField ( CustomStockList::StockUnitName );

	if (StockOptions.GetStockmateLinkFlag() == TRUE)
	{
		AddAvailableField(CustomStockList::StockmateText);
	}

	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
	{
		AddAvailableField ( CustomStockList::StockUnitDetail );
		AddAvailableField ( CustomStockList::RetailUnitName );
		AddAvailableField ( CustomStockList::Wastage );		
	}

	AddAvailableField ( CustomStockList::LabelsFlag );
	AddAvailableField ( CustomStockList::OrderFlag );
	AddAvailableField ( CustomStockList::PluRangeFlag );

	AddAvailableField ( CustomStockList::SupplierName1 );
	AddAvailableField ( CustomStockList::SupplierRef1 );
	AddAvailableField ( CustomStockList::WrapperCode1 );
	AddAvailableField ( CustomStockList::DelUnitName1 );
	AddAvailableField ( CustomStockList::DelUnitDetail1 );
	AddAvailableField ( CustomStockList::DelUnitYield1 );
	AddAvailableField ( CustomStockList::DelUnitCost1 );
	AddAvailableField ( CustomStockList::StockUnitCost1 );

	if (StockOptions.GetStockSalesEntryType() != 0)
	{
		AddAvailableField(CustomStockList::DelUnitRetail1);
	}

	if ( StockOptions.GetStockMaxSuppliersPerItem() >= 2 )
	{
		AddAvailableField ( CustomStockList::SupplierName2 );
		AddAvailableField ( CustomStockList::SupplierRef2 );
		AddAvailableField ( CustomStockList::WrapperCode2 );
		AddAvailableField ( CustomStockList::DelUnitName2 );
		AddAvailableField ( CustomStockList::DelUnitDetail2 );
		AddAvailableField ( CustomStockList::DelUnitYield2 );
		AddAvailableField ( CustomStockList::DelUnitCost2 );
		AddAvailableField ( CustomStockList::StockUnitCost2 );

		if (StockOptions.GetStockSalesEntryType() != 0)
		{
			AddAvailableField(CustomStockList::DelUnitRetail2);
		}
	}

	if ( StockOptions.GetStockMaxSuppliersPerItem() >= 3 )
	{
		AddAvailableField ( CustomStockList::SupplierName3 );
		AddAvailableField ( CustomStockList::SupplierRef3 );
		AddAvailableField ( CustomStockList::WrapperCode3 );
		AddAvailableField ( CustomStockList::DelUnitName3 );
		AddAvailableField ( CustomStockList::DelUnitDetail3 );
		AddAvailableField ( CustomStockList::DelUnitYield3 );
		AddAvailableField ( CustomStockList::DelUnitCost3 );
		AddAvailableField ( CustomStockList::StockUnitCost3 );

		if (StockOptions.GetStockSalesEntryType() != 0)
		{
			AddAvailableField(CustomStockList::DelUnitRetail3);
		}
	}

	if (StockOptions.GetStockMaxSuppliersPerItem() >= 4)
	{
		AddAvailableField(CustomStockList::SupplierName4);
		AddAvailableField(CustomStockList::SupplierRef4);
		AddAvailableField(CustomStockList::WrapperCode4);
		AddAvailableField(CustomStockList::DelUnitName4);
		AddAvailableField(CustomStockList::DelUnitDetail4);
		AddAvailableField(CustomStockList::DelUnitYield4);
		AddAvailableField(CustomStockList::DelUnitCost4);
		AddAvailableField(CustomStockList::StockUnitCost4);

		if (StockOptions.GetStockSalesEntryType() != 0)
		{
			AddAvailableField(CustomStockList::DelUnitRetail4);
		}
	}

	if (StockOptions.GetStockMaxSuppliersPerItem() >= 5)
	{
		AddAvailableField(CustomStockList::SupplierName5);
		AddAvailableField(CustomStockList::SupplierRef5);
		AddAvailableField(CustomStockList::WrapperCode5);
		AddAvailableField(CustomStockList::DelUnitName5);
		AddAvailableField(CustomStockList::DelUnitDetail5);
		AddAvailableField(CustomStockList::DelUnitYield5);
		AddAvailableField(CustomStockList::DelUnitCost5);
		AddAvailableField(CustomStockList::StockUnitCost5);

		if (StockOptions.GetStockSalesEntryType() != 0)
		{
			AddAvailableField(CustomStockList::DelUnitRetail5);
		}
	}

	if (DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE)
	{
		AddAvailableField(CustomStockList::Apparent);
	}
}

/**********************************************************************/

void CStockCustomListDlg::AddFieldToList( const char* szField, int nData, int nTagState  )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szField;
	item.m_nData = nData;
	item.SetTagState(nTagState);
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CStockCustomListDlg::AddAvailableField( CustomStockField Field, const char* szCustomText )
{
	CString strCustomText = szCustomText;

	CSortedStringByInt item;
	
	item.m_nItem = Field.Type;

	if (strCustomText != "")
	{
		item.m_strItem = strCustomText;
	}
	else
	{
		item.m_strItem = Field.ListLabel;
	}
	
	m_arrayDefaultFields.Add( item.m_nItem );
	m_arrayAvailableFields.Consolidate( item );
}

/**********************************************************************/

void CStockCustomListDlg::FillCategoryCombo()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetDbIdx() != m_nCategoryDbIdx )
	{
		m_nCategoryDbIdx = LocSelEntity.GetDbIdx();
		m_CategorySelector.SelectCategorySet ( -1 );
		m_CategorySelector.IncludeReportEnabled();
		m_CategorySelector.IncludeCategorySets();
		m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
		m_CategorySelector.ForceSelection ( m_nCategorySelect );
	}
}

/**********************************************************************/

void CStockCustomListDlg::FillSupplierCombo()
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

bool CStockCustomListDlg::GetSupplierIdxArray( CWordArray& wArray )
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

void CStockCustomListDlg::FillSortOrderCombo()
{
	WORD wOldSortOrder = 999;
	
	int nSel = m_comboSortOrder.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboSortOrder.GetCount() )
		wOldSortOrder = ( (WORD) m_comboSortOrder.GetItemData( nSel ) ); //FIDDLE - AVOID WARNING

	CWordArray arraySortOrders;
	
	if ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE )
	{
		arraySortOrders.Add( SH_STKLVL_NONE );
		arraySortOrders.Add( SH_STKLVL_SUPP );
		arraySortOrders.Add( SH_STKLVL_SUPP_SP );
		arraySortOrders.Add( SH_STKLVL_SUPP_SP_CAT );
		arraySortOrders.Add( SH_STKLVL_SUPP_CAT );
		arraySortOrders.Add( SH_STKLVL_SUPP_CAT_SP );
		arraySortOrders.Add( SH_STKLVL_SP );
		arraySortOrders.Add( SH_STKLVL_SP_SUPP );
		arraySortOrders.Add( SH_STKLVL_SP_SUPP_CAT );
		arraySortOrders.Add( SH_STKLVL_SP_CAT );
		arraySortOrders.Add( SH_STKLVL_SP_CAT_SUPP );
		arraySortOrders.Add( SH_STKLVL_CAT );
		arraySortOrders.Add( SH_STKLVL_CAT_SUPP );
		arraySortOrders.Add( SH_STKLVL_CAT_SUPP_SP );
		arraySortOrders.Add( SH_STKLVL_CAT_SP );
		arraySortOrders.Add( SH_STKLVL_CAT_SP_SUPP );
	}
	else
	{
		arraySortOrders.Add( SH_STKLVL_NONE );
		arraySortOrders.Add( SH_STKLVL_SUPP );
		arraySortOrders.Add( SH_STKLVL_SUPP_CAT );
		arraySortOrders.Add( SH_STKLVL_CAT );
		arraySortOrders.Add( SH_STKLVL_CAT_SUPP );
	}

	m_comboSortOrder.ResetContent();

	for ( int n = 0; n < arraySortOrders.GetSize(); n++ )
	{
		CString strLine = "";

		WORD wSortType = arraySortOrders.GetAt(n);

		switch( wSortType )
		{
		case SH_STKLVL_NONE:		strLine = "Stock Code";						break;
		case SH_STKLVL_SUPP:		strLine = "Supplier";						break;
		case SH_STKLVL_SUPP_SP:		strLine = "Supplier Stockpoint";			break;
		case SH_STKLVL_SUPP_SP_CAT:	strLine = "Supplier Stockpoint Category";	break;
		case SH_STKLVL_SUPP_CAT:	strLine = "Supplier Category";				break;
		case SH_STKLVL_SUPP_CAT_SP:	strLine = "Supplier Category Stockpoint";	break;
		case SH_STKLVL_SP:			strLine = "Stockpoint";						break;
		case SH_STKLVL_SP_SUPP:		strLine = "Stockpoint Supplier";			break;
		case SH_STKLVL_SP_SUPP_CAT:	strLine = "Stockpoint Supplier Category";	break;
		case SH_STKLVL_SP_CAT:		strLine = "Stockpoint Category";			break;
		case SH_STKLVL_SP_CAT_SUPP:	strLine = "Stockpoint Category Supplier";	break;
		case SH_STKLVL_CAT:			strLine = "Category";						break;
		case SH_STKLVL_CAT_SUPP:	strLine = "Category Supplier";				break;
		case SH_STKLVL_CAT_SUPP_SP:	strLine = "Category Supplier Stockpoint";	break;
		case SH_STKLVL_CAT_SP:		strLine = "Category Stockpoint";			break;
		case SH_STKLVL_CAT_SP_SUPP:	strLine = "Category Stockpoint Supplier";	break;
		}

		int nPos = m_comboSortOrder.AddString( strLine );
		m_comboSortOrder.SetItemData( nPos, wSortType );
	}

	m_comboSortOrder.SetCurSel( 0 );

	int nCount = m_comboSortOrder.GetCount();
	for ( int n = 1; n < nCount; n++ )
	{
		if ( m_comboSortOrder.GetItemData( n ) == wOldSortOrder )
		{
			m_comboSortOrder.SetCurSel(n);
			break;
		}
	}

	m_comboSortOrder.EnableWindow( nCount > 1 );
}

/**********************************************************************/

void CStockCustomListDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockCustomListDlg::OnToggleAllStock() 
{
	UpdateData( TRUE, FALSE );
	m_editStockCodeFrom.EnableWindow ( m_bSpecifyStockCodes );
	m_editStockCodeTo.EnableWindow ( m_bSpecifyStockCodes );
	m_buttonBrowseFrom.EnableWindow ( m_bSpecifyStockCodes );
	m_buttonBrowseTo.EnableWindow ( m_bSpecifyStockCodes );
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonBrowseFrom() 
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
	StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetSelection() );
		
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

void CStockCustomListDlg::OnButtonBrowseTo() 
{
	UpdateData( TRUE, FALSE );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
	
	CWaitCursor wait;
	CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
	StockFilterArray.SetSupplierFilter ( 0 );
	StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetSelection() );
	
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

void CStockCustomListDlg::OnButtonUp() 
{
	
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 1 && nSel < m_listFields.GetItemCount() )
		m_listFields.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 0 && nSel < m_listFields.GetItemCount() - 1 )
		m_listFields.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonAll() 
{
	m_listFields.SelectAll();
	m_StockFilter.GetDefaultRange ( 0, m_strStockCodeFrom, m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonNone() 
{
	m_listFields.SelectNone();	
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonResetOrder() 
{
	int nSelectedField = -1;

	int nSel = m_listFields.GetCurSel();
	if ( nSel >= 0 && nSel < m_listFields.GetItemCount() )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nSel, ListItem );
		nSelectedField = ListItem.m_nData;
	}

	CReportConsolidationArray<CSortedIntItem> arraySelected;
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );

		if ( ListItem.GetTagState() == 1 )
		{
			CSortedIntItem item;
			item.m_nItem = ListItem.m_nData;
			arraySelected.Consolidate( item );
		}
	}

	CreateAvailableFieldList();

	m_listFields.ClearList();
	AddFieldToList( CustomStockList::StockCode.ListLabel, CustomStockList::StockCode.Type, 1 );
		
	int nNewSel = 0;
	for ( int c = 0; c < m_arrayDefaultFields.GetSize(); c++ )
	{
		CSortedStringByInt item;
		item.m_nItem = m_arrayDefaultFields.GetAt(c);

		int nFieldFindPos;
		if ( m_arrayAvailableFields.Find( item, nFieldFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFieldFindPos, item );

			int nTickFindPos;
			CSortedIntItem itemTickCheck;
			itemTickCheck.m_nItem = item.m_nItem;
			bool bSelected = arraySelected.Find( itemTickCheck, nTickFindPos );
			
			AddFieldToList( item.m_strItem, item.m_nItem, bSelected ? 1 : 0 );
			m_arrayAvailableFields.RemoveAt(nFieldFindPos);
		
			if ( item.m_nItem == nSelectedField )
			{
				nNewSel = m_listFields.GetItemCount() - 1;
			}
		}
	}
	
	m_listFields.SetCurSel(nNewSel);
}

/**********************************************************************/

void CStockCustomListDlg::AppendFieldTicks ( CCSV& csv )
{
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
			csv.Add( ListItem.m_nData );
	}
}

/**********************************************************************/

void CStockCustomListDlg::OnButtonSave() 
{
	UpdateData ( TRUE, FALSE );

	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "wb" ) == TRUE )
	{
		CreateSavedSettingsLine();
		fileFields.WriteLine ( m_strSavedSettingsLine );
	}
}

/**********************************************************************/

void CStockCustomListDlg::CreateSavedSettingsLine()
{
	CCSV csv;
	csv.Add( 9999 );
	csv.Add( m_strName );

	AppendFieldTicks ( csv );

	if ( m_bDrawLines )			csv.Add ( 1000 );
	if ( m_bExcludeInactive )	csv.Add ( 1001 );
	
	m_strSavedSettingsLine = csv.GetLine();
}

/**********************************************************************/

void CStockCustomListDlg::OnOK()
{
	UpdateData(TRUE, FALSE);

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity(LocSelEntity);

	OnKillFocusStockCode();

	int nCatFilter = m_CategorySelector.GetSelection();

	CString strFrom = "";
	CString strTo = "";

	if (TRUE == m_bSpecifyStockCodes)
	{
		strFrom = m_strStockCodeFrom;
		strTo = m_strStockCodeTo;
	}

	bool bExcludeInactive = (m_bExcludeInactive != 0);

	CWordArray arraySuppNo;
	if (GetSupplierIdxArray(arraySuppNo) == FALSE)
	{
		return;
	}

	bool bAllSupp = TRUE;
	bool bShowItems = TRUE;
	int nSortOrder = m_comboSortOrder.GetItemData(m_comboSortOrder.GetCurSel());

	CWordArray arraySortOrder;
	CStockReportSortOrderCSVRecord SortOrder;
	SortOrder.SetSortOrderType(nSortOrder);
	SortOrder.GetSortOrderFieldsExternal(arraySortOrder);

	if (TRUE == m_bFirstTime)
	{
		m_StockLevelTree.Configure(2, &m_StockFilter, LocSelEntity, 0, arraySortOrder, bShowItems, nCatFilter, strFrom, strTo, bExcludeInactive, FALSE, arraySuppNo, bAllSupp, FALSE, FALSE);
		m_bFirstTime = FALSE;
	}
	else
	{
		m_StockLevelTree.Configure(2, NULL, LocSelEntity, 0, arraySortOrder, bShowItems, nCatFilter, strFrom, strTo, bExcludeInactive, FALSE, arraySuppNo, bAllSupp, FALSE, FALSE);
	}

	m_StockLevelTree.Consolidate();

	CCSV csv;
	AppendFieldTicks(csv);
	m_StockTreeReportCustomList.SetKey(csv.GetLine());
	m_StockTreeReportCustomList.SetDrawLinesFlag(m_bDrawLines != 0);
	m_StockTreeReportCustomList.PrepareArrays();
	m_StockTreeReportCustomList.SetConLevelText(LocSelEntity.GetEntityName());
	m_StockTreeReportCustomList.SetCustomName(m_strName);
	m_StockTreeReportCustomList.PrepareReport();
	m_StockTreeReportCustomList.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport(Super.ReportFilename(), m_StockTreeReportCustomList.GetReportParamsFilename(), m_StockTreeReportCustomList.GetReportParamsKey(), this);
}

/**********************************************************************/
