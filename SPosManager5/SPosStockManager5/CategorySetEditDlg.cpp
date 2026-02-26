/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "CategorySetEditDlg.h"
/**********************************************************************/

CCategorySetEditDlg::CCategorySetEditDlg( bool bSystem, CCategorySetCSVRecord& CategorySetRecord, CWnd* pParent)
	: CDialog(CCategorySetEditDlg::IDD, pParent), m_CategorySetRecord ( CategorySetRecord )
{
	//{{AFX_DATA_INIT(CCategorySetEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bSystem = bSystem;
	m_pCatArray = bSystem ? &DataManagerNonDb.SystemCategory : &DataManager.Category;
}

/**********************************************************************/

void CCategorySetEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCategorySetEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCategorySetEditDlg, CDialog)
	//{{AFX_MSG_MAP(CCategorySetEditDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCategorySetEditDlg::OnInitDialog() 
{
	m_strName = m_CategorySetRecord.GetName();
	CDialog::OnInitDialog();	
	
	m_listCategory.SubclassDlgItem ( IDC_LIST, this );
	m_listCategory.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listCategory.InsertColumn ( 0, "Categories", LVCFMT_LEFT, 220 );
	m_listCategory.SetLockItemZeroFlag( FALSE );
	
	AddList();
	return TRUE;  
}

/**********************************************************************/

void CCategorySetEditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listCategory.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CCategorySetEditDlg::OnButtonAll() 
{	
	m_listCategory.SelectAll();
}

/**********************************************************************/

void CCategorySetEditDlg::OnButtonNone() 
{	
	m_listCategory.SelectNone();
}

/**********************************************************************/

void CCategorySetEditDlg::AddList()
{
	m_listCategory.ClearList();

	CArray<int,int> arrayCatWithName;
	CArray<int,int> arrayCatNoName;

	for ( int nCategory = 1; nCategory <= MAX_CATEGORY; nCategory++ )
	{
		CString strText = m_pCatArray -> GetText ( nCategory );

		if ( ( strText != "" ) && ( strText != m_pCatArray -> GetDefaultText( nCategory ) ) )
		{
			arrayCatWithName.Add(nCategory);
		}
		else
		{
			arrayCatNoName.Add(nCategory);
		}
	}

	for ( int n = 0; n < arrayCatWithName.GetSize(); n++ )
	{
		int nCategory = arrayCatWithName.GetAt(n);
		
		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = m_pCatArray -> GetText(nCategory);
		ListItem.m_nData = nCategory;
		ListItem.SetTagState( m_CategorySetRecord.GetCategoryStatus( nCategory ) ? 1 : 0 );
		m_listCategory.AddSortedItemToList( ListItem );		
	}

	for ( int n = 0; n < arrayCatNoName.GetSize(); n++ )
	{
		int nCategory = arrayCatNoName.GetAt(n);
		
		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = m_pCatArray -> GetDefaultText(nCategory);
		ListItem.m_nData = nCategory;
		ListItem.SetTagState( m_CategorySetRecord.GetCategoryStatus( nCategory ) ? 1 : 0 );
		m_listCategory.AddItemToList( ListItem );
	}	
}

/**********************************************************************/

void CCategorySetEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strName, FALSE );
		
		if ( m_strName == "" )
			m_strName = "CATEGORY SET";

		m_CategorySetRecord.SetName ( m_strName );
		
		for ( int nIndex = 0; nIndex < m_listCategory.GetItemCount(); nIndex++ )
		{
			CSSListTaggedSelectItem ListItem;
			m_listCategory.GetListItem( nIndex, ListItem );
			m_CategorySetRecord.IncludeCategory ( ListItem.m_nData, ListItem.GetTagState() != 0 );
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
