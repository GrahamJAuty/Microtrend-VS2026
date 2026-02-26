/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "CustomFieldSetEditDlg.h"
/**********************************************************************/

CCustomFieldSetEditDlg::CCustomFieldSetEditDlg(CCustomFieldSetCSVRecord& CustomFieldSetRecord, CWnd* pParent)
	: CDialog(CCustomFieldSetEditDlg::IDD, pParent), m_CustomFieldSetRecord ( CustomFieldSetRecord )
{
	//{{AFX_DATA_INIT(CCustomFieldSetEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CCustomFieldSetEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomFieldSetEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomFieldSetEditDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomFieldSetEditDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomFieldSetEditDlg::OnInitDialog() 
{
	m_strName = m_CustomFieldSetRecord.GetName();
	CDialog::OnInitDialog();	
	
	m_listCustomFields.SubclassDlgItem ( IDC_LIST, this );
	m_listCustomFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listCustomFields.InsertColumn ( 0, "", LVCFMT_LEFT, 220 );
	m_listCustomFields.SetLockItemZeroFlag( FALSE );

	AddList();
	return TRUE;  
}

/**********************************************************************/

void CCustomFieldSetEditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listCustomFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CCustomFieldSetEditDlg::OnButtonAll() 
{	
	m_listCustomFields.SelectAll();
}

/**********************************************************************/

void CCustomFieldSetEditDlg::OnButtonNone() 
{	
	m_listCustomFields.SelectNone();
}

/**********************************************************************/

void CCustomFieldSetEditDlg::AddList()
{
	int nBit = m_CustomFieldSetRecord.GetNumber();

	m_listCustomFields.ClearList();

	for ( int n = 0; n < DataManager.GetActiveCustomField() -> GetSize(); n++ )
	{
		CCustomFieldValueCSVRecord record;
		DataManager.GetActiveCustomField() -> GetAt( n, record );
		CString strFlags = record.GetFieldSetFlags();

		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = record.GetName();
		ListItem.m_nData = 0;
		ListItem.SetTagState( ::GetHexStringFlag( strFlags, nBit ) ? 1 : 0 );
		m_listCustomFields.AddSortedItemToList( ListItem );
	}

	CSSListTaggedSelectItem ListItem;
	ListItem.m_strText = "N/A";
	ListItem.m_nData = 0;
	ListItem.SetTagState( m_CustomFieldSetRecord.GetIncludeNAFlag() ? 1 : 0 );
	m_listCustomFields.InsertItemIntoList( 0, ListItem );
}

/**********************************************************************/

void CCustomFieldSetEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strName, FALSE );
		
		if ( m_strName == "" )
			m_strName = "CUSTOM FIELD SET";

		m_CustomFieldSetRecord.SetName ( m_strName );
		m_CustomFieldSetRecord.SetIncludeNAFlag( m_listCustomFields.GetItemTagState(0) == 1 );
	
		int nBit = m_CustomFieldSetRecord.GetNumber();
	
		for ( int nIndex = 1; nIndex < m_listCustomFields.GetItemCount(); nIndex++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.GetActiveCustomField() -> GetAt( nIndex - 1, record );
			CString strFlags = record.GetFieldSetFlags();
		
			bool bFlag = ( m_listCustomFields.GetItemTagState( nIndex ) == 1 );
			::SetHexStringFlag( strFlags, nBit, bFlag );

			record.SetFieldSetFlags( strFlags );
			DataManager.GetActiveCustomField() -> SetAt( nIndex - 1, record );
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
