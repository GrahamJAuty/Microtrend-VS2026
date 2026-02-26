/**********************************************************************/
#include "StocktakeCategorySetDlg.h"
/**********************************************************************/

CStocktakeCategorySetDlg::CStocktakeCategorySetDlg( CCategorySetCSVRecord& CategorySet, CWnd* pParent)
	: CDialog(CStocktakeCategorySetDlg::IDD, pParent), m_CategorySet ( CategorySet )
{
	//{{AFX_DATA_INIT(CStocktakeCategorySetDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStocktakeCategorySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStocktakeCategorySetDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStocktakeCategorySetDlg, CDialog)
	//{{AFX_MSG_MAP(CStocktakeCategorySetDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStocktakeCategorySetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listCategories.SubclassDlgItem ( IDC_LIST, this );
	m_listCategories.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listCategories.InsertColumn ( 0, "", LVCFMT_LEFT, 275 );
	m_listCategories.SetLockItemZeroFlag( FALSE );

	AddList();
	return TRUE;  
}

/**********************************************************************/

void CStocktakeCategorySetDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listCategories.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStocktakeCategorySetDlg::OnButtonAll() 
{
	m_listCategories.SelectAll();
}

/**********************************************************************/

void CStocktakeCategorySetDlg::OnButtonNone() 
{
	m_listCategories.SelectNone();
}

/**********************************************************************/

void CStocktakeCategorySetDlg::AddList()
{
	m_listCategories.ClearList();

	for ( int nCategory = 1; nCategory <= MAX_CATEGORY; nCategory++ )
	{
		CString strText = DataManager.Category.GetText ( nCategory );

		if ( strText != DataManager.Category.GetDefaultText ( nCategory ) )
		{
			CSSListTaggedSelectItem item;
			item.m_strText = strText;
			item.m_nData = nCategory;
			item.SetTagState( m_CategorySet.GetCategoryStatus ( nCategory ) ? 1 : 0 );
			m_listCategories.AddSortedItemToList( item );
		}
	}
		
	for ( int nCategory = 1; nCategory <= MAX_CATEGORY; nCategory++ )
	{
		CString strText = DataManager.Category.GetText ( nCategory );

		if ( strText == DataManager.Category.GetDefaultText ( nCategory ) )
		{
			CSSListTaggedSelectItem item;
			item.m_strText = strText;
			item.m_nData = nCategory;
			item.SetTagState( m_CategorySet.GetCategoryStatus ( nCategory ) ? 1 : 0 );
			m_listCategories.AddItemToList( item );
		}
	}
}

/**********************************************************************/

void CStocktakeCategorySetDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		for ( int nPos = 0; nPos < m_listCategories.GetItemCount(); nPos++ )
		{
			CSSListTaggedSelectItem item;
			m_listCategories.GetListItem( nPos, item );
			m_CategorySet.IncludeCategory( item.m_nData, item.GetTagState() == 1 );
		}		
		EndDialog( IDOK );
	}
}

/**********************************************************************/
