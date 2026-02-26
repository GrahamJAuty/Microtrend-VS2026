/**********************************************************************/
#include "FilterTypes.h"
#include "MaxLengths.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockFilterDlg.h"
/**********************************************************************/

CStockFilterDlg::CStockFilterDlg( CStockFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CStockFilterDlg::IDD, pParent), m_StockFilterArray ( FilterArray ),
	m_CategorySelector( m_comboCategory )
{
	m_nStockIdx = -1;
	m_pArrayStockIdx = NULL;
	m_nSpSelect = 0;
	m_bSimpleSupplier = FALSE;
	m_strSearchText = m_StockFilterArray.GetDescription();
	m_nCategoryFilter = m_StockFilterArray.GetCategoryFilter();
	m_nSupplierFilter = m_StockFilterArray.GetSupplierFilter();
	RunSearch ( TRUE, FALSE );
}

/**********************************************************************/

CStockFilterDlg::CStockFilterDlg( CStockFilterArray& FilterArray, CArray<int,int>* pArrayStockIdx, CWnd* pParent )
	: CSSDialog(CStockFilterDlg::IDD, pParent), m_StockFilterArray ( FilterArray ),
	m_CategorySelector( m_comboCategory )
{
	m_nStockIdx = -1;
	m_pArrayStockIdx = pArrayStockIdx;
	m_pArrayStockIdx -> RemoveAll();
	m_nSpSelect = 0;
	m_bSimpleSupplier = FALSE;
	m_strSearchText = m_StockFilterArray.GetDescription();
	m_nCategoryFilter = m_StockFilterArray.GetCategoryFilter();
	m_nSupplierFilter = m_StockFilterArray.GetSupplierFilter();
	RunSearch ( TRUE, FALSE );
}

/**********************************************************************/

void CStockFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockFilterDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CStockFilterDlg)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNAME, OnSelectSearchText)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelectCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listStock.SubclassDlgItem ( IDC_LIST, this );
	m_listStock.EnableColour();
	m_listStock.InsertColumn ( 0, "Stock Code", LVCFMT_LEFT, 115 );
	m_listStock.InsertColumn ( 1, "Description", LVCFMT_LEFT, 200 );

	if ( m_pArrayStockIdx != NULL )
		m_listStock.ModifyStyle( LVS_SINGLESEL, 0);
	
	switch ( m_StockFilterArray.GetFilterType() )
	{
	case FILTERTYPE_DATABASE:
	case FILTERTYPE_BROWSE_STOCK:
		if ( m_StockFilterArray.GetCheckApparentFlag() == TRUE )
		{
			m_listStock.InsertColumn ( 2, "Category", LVCFMT_LEFT, 120 );
			m_listStock.InsertColumn ( 3, "Apparent", LVCFMT_RIGHT, 80 );
		}
		else
		{
			m_listStock.InsertColumn ( 2, "Category", LVCFMT_LEFT, 200 );
		}
		break;

	case FILTERTYPE_ORDERBROWSE_PREVIEW:
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
		{
			m_listStock.InsertColumn ( 2, "Apparent", LVCFMT_RIGHT, 100 );
			m_listStock.InsertColumn ( 3, "Minimum", LVCFMT_RIGHT, 100 );
		}
		else
		{
			m_listStock.InsertColumn ( 2, "Category", LVCFMT_LEFT, 120 );
			m_listStock.InsertColumn ( 3, "Apparent", LVCFMT_RIGHT, 80 );
		}
		break;

	case FILTERTYPE_ORDERBROWSE_NEWITEM:
	case FILTERTYPE_ORDERBROWSE_NEWORDER:
		m_listStock.InsertColumn ( 2, "Expected", LVCFMT_RIGHT, 100 );
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			m_listStock.InsertColumn ( 3, "Minimum", LVCFMT_RIGHT, 100 );
		else
			m_listStock.InsertColumn ( 3, "Apparent", LVCFMT_RIGHT, 100 );
		break;

	case FILTERTYPE_BROWSE_TRANSFER:
		m_listStock.InsertColumn ( 2, "App. From", LVCFMT_RIGHT, 100 );
		m_listStock.InsertColumn ( 3, "App. To", LVCFMT_RIGHT, 100 );
		break;

	case FILTERTYPE_BROWSE_OPENING:
		m_listStock.InsertColumn ( 2, "Opening", LVCFMT_RIGHT, 100 );
		m_listStock.InsertColumn ( 3, "Apparent", LVCFMT_RIGHT, 100 );
		break;
	
	case FILTERTYPE_BROWSE_STOCKTAKE:
		m_listStock.InsertColumn ( 2, "Actual", LVCFMT_RIGHT, 100 );
		m_listStock.InsertColumn ( 3, "Apparent", LVCFMT_RIGHT, 100 );
		break;
	}

	switch ( m_StockFilterArray.GetFilterType() )
	{
	case FILTERTYPE_DATABASE:				m_strTitle = "Filter Stock Database";	break;
	case FILTERTYPE_ORDERBROWSE_PREVIEW:	m_strTitle = "Add to Pending Order";	break;
	case FILTERTYPE_ORDERBROWSE_NEWITEM:	m_strTitle = "Add Item to Order";		break;
	case FILTERTYPE_ORDERBROWSE_NEWORDER:	m_strTitle = "Create New Order";		break;
	case FILTERTYPE_BROWSE_STOCK:			m_strTitle = "Browse Stock Database";	break;
	default:								m_strTitle = "Filter Stock Database";	break;
	}

	m_nCategoryFilter = m_StockFilterArray.GetCategoryFilter();
	m_nSupplierFilter = m_StockFilterArray.GetSupplierFilter();
	FillCategoryCombo();
	SelectCategoryFromNumber();
	FillSupplierCombo();
	SelectSupplierFromNumber();
	RunSearch( FALSE, TRUE );
	return TRUE;  
}

/**********************************************************************/

void CStockFilterDlg::RunSearch( bool bDoSearch, bool bUpdateDisplay )
{
	if ( bDoSearch == TRUE )
	{
		m_StockFilterArray.SetDescriptionFilter ( m_strSearchText, ( STOCK_FILTER_DESCRIPTION | STOCK_FILTER_STOCKCODE | STOCK_FILTER_SUPPREF ), FALSE );
		m_StockFilterArray.SetCategoryFilter ( m_nCategoryFilter );
		m_StockFilterArray.SetSupplierFilter ( m_nSupplierFilter );
		m_StockFilterArray.BuildRecordList();
	}

	if ( bUpdateDisplay == TRUE )
	{
		m_listStock.SetItemCountEx ( m_StockFilterArray.GetSize() );
		m_listStock.Invalidate();
		m_listStock.SetCurSel(0);
		UpdateTitle();
	}
}

/**********************************************************************/

void CStockFilterDlg::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "%s ( %d of %d records )",
		( const char* ) m_strTitle,
		m_StockFilterArray.GetSize(),
		DataManager.Stock.GetSize() );
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CStockFilterDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		CCSV csv ( m_StockFilterArray.GetDisplayLine ( nIndex ), '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );

		if ( m_StockFilterArray.GetNegativeStockFlag() )
			m_listStock.SetColour( nIndex, 0x0C );
		else if ( m_StockFilterArray.GetBelowMinStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockBelowMinimumColour() ) );
		else if ( m_StockFilterArray.GetAboveMaxStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockAboveMaximumColour() ) );
		else
			m_listStock.SetColour( nIndex, 0x00 );
	}
	*pResult = 0;
}

/**********************************************************************/

void CStockFilterDlg::OnSelectSearchText() 
{
	CString strSearchTextOld = m_strSearchText;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strSearchText, FALSE );
		
		if ( m_strSearchText != strSearchTextOld )
		{
			RunSearch( TRUE, TRUE );
			UpdateData( FALSE );
		}
	}	
}

/**********************************************************************/

void CStockFilterDlg::OnSelectCategory() 
{	
	UpdateData ( TRUE, FALSE );

	int nOldCategoryFilter = m_nCategoryFilter;
	m_nCategoryFilter = m_CategorySelector.GetCategoryFilter();			
	if ( m_nCategoryFilter != nOldCategoryFilter )
		RunSearch ( TRUE, TRUE );
			
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockFilterDlg::FillCategoryCombo()
{
	m_CategorySelector.FillCategoryComboDatabase( 0, NEWCATEGORY_SELECTOR_NORMAL );
}

/**********************************************************************/

void CStockFilterDlg::SelectCategoryFromNumber()
{
	m_CategorySelector.UpdateCategoryCombo( m_nCategoryFilter );
}

/**********************************************************************/

void CStockFilterDlg::OnSelectSupplier() 
{
	UpdateData( TRUE, FALSE );

	int nIndex = m_comboSupplier.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nOldSupplierFilter = m_nSupplierFilter;
		m_nSupplierFilter = m_comboSupplier.GetItemData ( nIndex ) - 1;
			
		if ( m_nSupplierFilter != nOldSupplierFilter )
			RunSearch ( TRUE, TRUE );
		
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CStockFilterDlg::FillSupplierCombo()
{
	m_comboSupplier.ResetContent();

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nSuppNo = DataManager.Supplier.GetSuppNo ( nIndex );
		if ( ( m_bSimpleSupplier == FALSE ) || ( nSuppNo == m_nSupplierFilter ) )
		{
			int nPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
			m_comboSupplier.SetItemData ( nPos, nSuppNo + 1 );
		}
	}
	
	if ( m_bSimpleSupplier == FALSE )
	{
		m_comboSupplier.InsertString ( 0, "All" );
		m_comboSupplier.SetItemData ( 0, 1 );
		m_comboSupplier.InsertString ( 1, "Unspecified" );
		m_comboSupplier.SetItemData ( 1, 0 );
	}
	else if ( m_nSupplierFilter == -1 )
	{
		m_comboSupplier.InsertString ( 0, "Unspecified" );
		m_comboSupplier.SetItemData ( 0, 0 );
	}
	
	int nInsertPos = ( m_bSimpleSupplier == FALSE ) ? 2 : 0;
	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
	{
		if ( ( m_bSimpleSupplier == FALSE ) || ( m_nSupplierFilter == nIndex + FIRST_SUPPLIERSET_FILTER ) )
		{
			m_comboSupplier.InsertString ( nInsertPos, DataManager.SupplierSet.GetName ( nIndex ) );
			m_comboSupplier.SetItemData ( nInsertPos, nIndex + FIRST_SUPPLIERSET_FILTER + 1 );
			nInsertPos++;
		}
	}

	if ( m_bSimpleSupplier == TRUE )
	{
		int nPos = m_comboSupplier.InsertString ( -1, "All" );
		m_comboSupplier.SetItemData ( nPos, 1 );
	}
}

/**********************************************************************/

void CStockFilterDlg::SelectSupplierFromNumber()
{
	for ( int nIndex = 0; nIndex <= m_comboSupplier.GetCount(); nIndex++ )
	{
		if ( m_comboSupplier.GetItemData ( nIndex ) == WORD ( m_nSupplierFilter + 1 ) )
		{
			m_comboSupplier.SetCurSel ( nIndex );
			break;
		}
	}
}	

/**********************************************************************/

void CStockFilterDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	int nIndex = m_listStock.GetCurSel();
	if ( ( nIndex >= 0 ) && ( nIndex < m_listStock.GetItemCount() ) )
		OnOK();
}

/**********************************************************************/

void CStockFilterDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nStockIdx = -1;

		if ( NULL == m_pArrayStockIdx )
		{
			int nIndex = m_listStock.GetCurSel();
	
			if ( nIndex >= 0 && nIndex < m_listStock.GetItemCount() )
				m_nStockIdx = m_StockFilterArray.GetStockItemIndexAt ( nIndex );
		}
		else
		{
			m_pArrayStockIdx -> RemoveAll();

			POSITION pos = m_listStock.GetFirstSelectedItemPosition();
	
			while (pos)
			{
				int nIndex = m_listStock.GetNextSelectedItem(pos);
				int nStockIdx = m_StockFilterArray.GetStockItemIndexAt ( nIndex );
				m_pArrayStockIdx -> Add( nStockIdx );
			}
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
