/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "CustomerSelectDlg.h"
/**********************************************************************/

CCustomerSelectDlg::CCustomerSelectDlg( CString& strFilter, CEposSelectArray& select, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomerSelectDlg::IDD, pParent), m_SelectArray( select ), m_strFilter( strFilter )
{
	//{{AFX_DATA_INIT(CCustomerSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strFilter = strFilter.Left(20);
	m_bIncludeOneOff = DataManagerNonDb.CustomerNameTable.Account.GetOneOffCustomerFlag();
	m_bIncludeUnknown = DataManagerNonDb.CustomerNameTable.Account.GetUnknownCustomerFlag();
}

/**********************************************************************/

void CCustomerSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerSelectDlg)
	DDX_Control(pDX, IDC_EDIT_FILTER, m_editFilter );
	DDX_Check(pDX, IDC_CHECK_ONEOFF, m_bIncludeOneOff );
	DDX_Check(pDX, IDC_CHECK_UNKNOWN, m_bIncludeUnknown );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerSelectDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_EN_KILLFOCUS(IDC_EDIT_FILTER, OnKillFocusFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomerSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listCustomers.SubclassDlgItem ( IDC_LIST, this );

	m_listCustomers.InsertColumn ( 0, "", LVCFMT_LEFT, 25 );
	m_listCustomers.InsertColumn ( 1, "Location", LVCFMT_LEFT, 140 );
	m_listCustomers.InsertColumn ( 2, "Code", LVCFMT_LEFT, 80 );
	m_listCustomers.InsertColumn ( 3, "First Name", LVCFMT_LEFT, 130 );
	m_listCustomers.InsertColumn ( 4, "Last Name", LVCFMT_LEFT, 130 );
	m_listCustomers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	m_editFilter.LimitText( 20 );
	m_editFilter.SetWindowText( m_strFilter );
	
	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelectedTemp = info.m_bIsSelected;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}
	
	AddListLocation();

	return TRUE;
}

/**********************************************************************/

void CCustomerSelectDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nColumn = pNMListView->iSubItem;
	
	int nNewSortType = -1;
	int nOldSortType = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderType();

	switch( nColumn )
	{
	case 2:
		nNewSortType = ( CUSTOMER_SORT_ID_INC == nOldSortType ) ? CUSTOMER_SORT_ID_DEC : CUSTOMER_SORT_ID_INC;
		break;

	case 3:
		nNewSortType = ( CUSTOMER_SORT_FIRSTNAME_INC == nOldSortType ) ? CUSTOMER_SORT_FIRSTNAME_DEC : CUSTOMER_SORT_FIRSTNAME_INC;
		break;

	case 4:
		nNewSortType = ( CUSTOMER_SORT_LASTNAME_INC == nOldSortType ) ? CUSTOMER_SORT_LASTNAME_DEC : CUSTOMER_SORT_LASTNAME_INC;
		break;
	}

	if ( nNewSortType != -1 )
	{
		m_editFilter.GetWindowText( m_strFilter );
		
		for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
		{
			int nIdx = m_listCustomers.GetItemData(n);

			CCustomerSelectionListAccountInfo info;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, info );
			info.m_bIsSelectedTemp = ( m_listCustomers.GetCheck( n ) != 0 );
			DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nIdx, info );
		}

		DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder( nNewSortType );
		AddListLocation();
	}

	*pResult = 0;
}

/**********************************************************************/

void CCustomerSelectDlg::OnKillFocusFilter()
{
	CString strFilter;
	m_editFilter.GetWindowText( strFilter );

	if ( strFilter != m_strFilter )
	{
		m_strFilter = strFilter;

		for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		{
			int nArrayIdx = m_listCustomers.GetItemData(nIndex);

			CCustomerSelectionListAccountInfo info;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
			info.m_bIsSelectedTemp = ( m_listCustomers.GetCheck(nIndex) != 0 );
			DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
		}

		AddListLocation();
	}
}

/**********************************************************************/

void CCustomerSelectDlg::AddListLocation()
{
	int nFilter = 0;
	CString strFilter = m_strFilter;
	strFilter.MakeUpper();
	
	if ( ( strFilter.GetLength() <= 8 ) && ( ::TestNumeric( strFilter ) == TRUE ) )
	{
		nFilter = atoi( strFilter );
		strFilter = "";
	}

	m_listCustomers.DeleteAllItems();

	int nListPos = 0;
	for ( int nSortOrderIdx = 0; nSortOrderIdx < DataManagerNonDb.CustomerNameTable.Account.GetSortOrderSize(); nSortOrderIdx++ )
	{
		int nArrayIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem(nSortOrderIdx);

		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		
		bool bAccept = TRUE;

		if ( m_SelectArray.GotTerminals( info.m_nLocIdx ) == FALSE )
			bAccept = FALSE;
		else if ( ( nFilter != 0 ) && ( nFilter != info.m_nCustomerID ) )
			bAccept = FALSE;
		else if ( strFilter != "" )
		{
			CString strNameTest = "";
			strNameTest += info.m_strName1;
			strNameTest += " ";
			strNameTest += info.m_strName2;
			strNameTest.MakeUpper();
			bAccept = ( strNameTest.Find( strFilter ) != -1 );
		}

		if ( TRUE == bAccept )
		{
			CString strCode;
			strCode.Format( "%8.8d", info.m_nCustomerID );

			m_listCustomers.InsertItem ( nListPos, "" );
			m_listCustomers.SetItem ( nListPos, 1, LVIF_TEXT, dbLocation.GetName( info.m_nLocIdx ), 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 2, LVIF_TEXT, strCode, 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 3, LVIF_TEXT, info.m_strName1, 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 4, LVIF_TEXT, info.m_strName2, 0, 0, 0, NULL );
			m_listCustomers.SetCheck ( nListPos, info.m_bIsSelectedTemp );
		
			m_listCustomers.SetItemData( nListPos, nArrayIdx );
			nListPos++;	
		}
	}

	m_listCustomers.SetCurSel(0);
}

/**********************************************************************/

void CCustomerSelectDlg::OnButtonSelectAll() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, TRUE );

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelectedTemp = TRUE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}
}

/**********************************************************************/

void CCustomerSelectDlg::OnButtonSelectNone() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, FALSE );

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelectedTemp = FALSE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}
}

/**********************************************************************/

void CCustomerSelectDlg::OnOK() 
{
	OnKillFocusFilter();
	UpdateData( TRUE );

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelected = info.m_bIsSelectedTemp;
		info.m_bIsInFilter = FALSE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}

	for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
	{
		int nIdx = m_listCustomers.GetItemData(n);

		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, info );
		info.m_bIsSelected = ( m_listCustomers.GetCheck( n ) != 0 );
		info.m_bIsInFilter = TRUE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nIdx, info );
	}
	
	DataManagerNonDb.CustomerNameTable.Account.SetOneOffCustomerFlag( m_bIncludeOneOff != 0 );
	DataManagerNonDb.CustomerNameTable.Account.SetUnknownCustomerFlag( m_bIncludeUnknown != 0 );

	EndDialog( IDOK );
}

/**********************************************************************/




