/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "SupplierSetEditDlg.h"
/**********************************************************************/

CSupplierSetEditDlg::CSupplierSetEditDlg(CSupplierSetCSVRecord& SupplierSetRecord, CWnd* pParent)
	: CDialog(CSupplierSetEditDlg::IDD, pParent), m_SupplierSetRecord ( SupplierSetRecord )
{
	//{{AFX_DATA_INIT(CSupplierSetEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSupplierSetEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSupplierSetEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSupplierSetEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSupplierSetEditDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSupplierSetEditDlg::OnInitDialog() 
{
	m_strName = m_SupplierSetRecord.GetName();
	CDialog::OnInitDialog();	
	
	m_listSuppliers.SubclassDlgItem ( IDC_LIST, this );
	m_listSuppliers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSuppliers.InsertColumn ( 0, "Suppliers", LVCFMT_LEFT, 220 );
	m_listSuppliers.SetLockItemZeroFlag( FALSE );
	
	AddList();
	return TRUE;  
}

/**********************************************************************/

void CSupplierSetEditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSuppliers.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSupplierSetEditDlg::OnButtonAll() 
{	
	m_listSuppliers.SelectAll();
}

/**********************************************************************/

void CSupplierSetEditDlg::OnButtonNone() 
{	
	m_listSuppliers.SelectNone();
}

/**********************************************************************/

void CSupplierSetEditDlg::AddList()
{
	CByteArray SuppSelectArray;
	m_SupplierSetRecord.GetSupplierFlags ( SuppSelectArray );
	
	m_listSuppliers.ClearList();
	
	for ( int nSupplier = 1; nSupplier <= DataManager.Supplier.GetSize(); nSupplier++ )
	{
		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = DataManager.Supplier.GetName ( nSupplier - 1 );
		ListItem.m_nData = DataManager.Supplier.GetSuppNo ( nSupplier - 1 );
		ListItem.SetTagState( ( SuppSelectArray.GetAt ( nSupplier ) == 1 ) ? 1 : 0 );
		m_listSuppliers.AddSortedItemToList( ListItem );
	}

	CSSListTaggedSelectItem ListItem;
	ListItem.m_strText = "Unspecified";
	ListItem.m_nData = 0;
	ListItem.SetTagState( ( SuppSelectArray.GetAt ( 0 ) == 1 ) ? 1 : 0 );
	m_listSuppliers.InsertItemIntoList( 0, ListItem );
}

/**********************************************************************/

void CSupplierSetEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strName, FALSE );
		
		if ( m_strName == "" )
			m_strName = "SUPPLIER SET";

		m_SupplierSetRecord.SetName ( m_strName );
		m_SupplierSetRecord.RemoveAll();

		for ( int nIndex = 0; nIndex < m_listSuppliers.GetItemCount(); nIndex++ )
		{
			CSSListTaggedSelectItem ListItem;
			m_listSuppliers.GetListItem( nIndex, ListItem );
		
			if ( ListItem.GetTagState() == 1 )
			{
				m_SupplierSetRecord.AddSuppNo ( ListItem.m_nData );
			}
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
