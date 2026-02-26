/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "FilterTypes.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "StockDefineFilterDlg.h"
/**********************************************************************/

CStockDefineFilterDlg::CStockDefineFilterDlg( CStockFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CStockDefineFilterDlg::IDD, pParent), m_StockFilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CStockDefineFilterDlg)
	m_bPluMulti = FALSE;
	m_bPluNone = FALSE;
	m_bPluSingle = FALSE;
	//}}AFX_DATA_INIT
	m_nSearchType = m_StockFilterArray.GetSearchType();
	m_strSearchText = m_StockFilterArray.GetDescription();
	m_bMatchCase = m_StockFilterArray.GetMatchCase();
	m_nCategoryFilter = m_StockFilterArray.GetCategoryFilter();
	m_nSupplierFilter = m_StockFilterArray.GetSupplierFilter();
	m_nSearchType = m_StockFilterArray.GetSearchType();
	m_bMatchDescription =  ( ( m_nSearchType & STOCK_FILTER_DESCRIPTION ) == 0 ) ? FALSE : TRUE;
	m_bMatchStockCode =  ( ( m_nSearchType & STOCK_FILTER_STOCKCODE ) == 0 ) ? FALSE : TRUE;
	m_bMatchSuppRef =  ( ( m_nSearchType & STOCK_FILTER_SUPPREF ) == 0 ) ? FALSE : TRUE;
	m_bPluNone = m_StockFilterArray.GetNoPluLinkFlag();
	m_bPluSingle = m_StockFilterArray.GetSinglePluLinkFlag();
	m_bPluMulti = m_StockFilterArray.GetMultiPluLinkFlag();
}

/**********************************************************************/

void CStockDefineFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDefineFilterDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_MINMAX, m_comboMinMax);
	DDX_Check(pDX, IDC_CHECK_MATCHCASE, m_bMatchCase);
	DDX_Check(pDX, IDC_CHECK_DESCRIPTION, m_bMatchDescription);
	DDX_Check(pDX, IDC_CHECK_STOCKCODE, m_bMatchStockCode);
	DDX_Check(pDX, IDC_CHECK_SUPPREF, m_bMatchSuppRef);
	DDX_Check(pDX, IDC_CHECK_PLU_MULTI, m_bPluMulti);
	DDX_Check(pDX, IDC_CHECK_PLU_NONE, m_bPluNone);
	DDX_Check(pDX, IDC_CHECK_PLU_SINGLE, m_bPluSingle);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDefineFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDefineFilterDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelectCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDefineFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	FillCategoryCombo();
	FillSupplierCombo();
	SelectSupplierFromNumber();

	m_comboMinMax.AddString( "No Stock Level Filter" );
	m_comboMinMax.AddString( "Below Minimum Only" );
	m_comboMinMax.AddString( "Above Maximum Only" );

	switch( m_StockFilterArray.GetMinMaxFilter() )
	{
	case STOCKFILTER_MINMAX_BELOW_MIN:
		m_comboMinMax.SetCurSel(1);
		break;

	case STOCKFILTER_MINMAX_ABOVE_MAX:
		m_comboMinMax.SetCurSel(2);
		break;

	case STOCKFILTER_MINMAX_ALL:
	default:
		m_comboMinMax.SetCurSel(0);
		break;
	}

	return TRUE;  
}

/**********************************************************************/

void CStockDefineFilterDlg::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet(0);
	m_CategorySelector.IncludeAll();
	m_CategorySelector.IncludeCategorySets();
	m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
	m_CategorySelector.ForceSelection( m_nCategoryFilter );
}

/**********************************************************************/

void CStockDefineFilterDlg::OnSelectCategory() 
{
	UpdateData( TRUE, FALSE );
	m_nCategoryFilter = m_CategorySelector.GetSelection();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockDefineFilterDlg::OnSelectSupplier() 
{
	UpdateData( TRUE, FALSE );

	int nIndex = m_comboSupplier.GetCurSel();
	if ( nIndex != CB_ERR )
	{
		m_nSupplierFilter = m_comboSupplier.GetItemData ( nIndex ) - 1;
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CStockDefineFilterDlg::FillSupplierCombo()
{
	m_comboSupplier.ResetContent();

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nPos, DataManager.Supplier.GetSuppNo ( nIndex ) + 1 );
	}
	m_comboSupplier.InsertString ( 0, "All" );
	m_comboSupplier.SetItemData ( 0, 1 );
	m_comboSupplier.InsertString ( 1, "Unspecified" );
	m_comboSupplier.SetItemData ( 1, 0 );

	int nInsertPos = 2;
	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
	{
		m_comboSupplier.InsertString ( nInsertPos, DataManager.SupplierSet.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nInsertPos, nIndex + FIRST_SUPPLIERSET_FILTER + 1 );
		nInsertPos++;
	}
}

/**********************************************************************/

void CStockDefineFilterDlg::SelectSupplierFromNumber()
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

void CStockDefineFilterDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strSearchText, FALSE );
		
		m_nSearchType = 0;
		if ( m_bMatchDescription == TRUE ) m_nSearchType |= STOCK_FILTER_DESCRIPTION;
		if ( m_bMatchStockCode == TRUE ) m_nSearchType |= STOCK_FILTER_STOCKCODE;
		if ( m_bMatchSuppRef == TRUE ) m_nSearchType |= STOCK_FILTER_SUPPREF;

		m_StockFilterArray.SetDescriptionFilter ( m_strSearchText, m_nSearchType, ( m_bMatchCase != 0 ) );
		m_StockFilterArray.SetCategoryFilter ( m_nCategoryFilter );
		m_StockFilterArray.SetSupplierFilter ( m_nSupplierFilter );

		m_StockFilterArray.SetNoPluLinkFlag( m_bPluNone != 0 );
		m_StockFilterArray.SetSinglePluLinkFlag( m_bPluSingle != 0 );
		m_StockFilterArray.SetMultiPluLinkFlag( m_bPluMulti != 0 );

		int nMinMax = STOCKFILTER_MINMAX_ALL;
			
		switch( m_comboMinMax.GetCurSel() )
		{
		case 1:	nMinMax = STOCKFILTER_MINMAX_BELOW_MIN;	break;
		case 2:	nMinMax = STOCKFILTER_MINMAX_ABOVE_MAX;	break;
		}

		m_StockFilterArray.SetMinMaxFilter( nMinMax );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
