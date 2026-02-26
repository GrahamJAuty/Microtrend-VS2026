/**********************************************************************/
 
/**********************************************************************/
#include "CustomerActivityChecker.h"
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "CustomerMoveDlg.h"
/**********************************************************************/

CCustomerMoveDlg::CCustomerMoveDlg( int nLocIdx, bool bOneOff, CArray<int,int>& arrayCustomerIdx, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomerMoveDlg::IDD, pParent), m_arrayCustomerIdx( arrayCustomerIdx )
{
	//{{AFX_DATA_INIT(CCustomerMoveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bOneOff = bOneOff;
}

/**********************************************************************/

void CCustomerMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerMoveDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerMoveDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

CCustomerCSVArray* CCustomerMoveDlg::GetCustomerArray()
{
	if ( TRUE == m_bOneOff )
		return &DataManagerNonDb.CustomerOneOff;
	else
		return &DataManagerNonDb.CustomerAccount;
}

/**********************************************************************/

BOOL CCustomerMoveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strText = "Move Customers to ";

	if ( TRUE == m_bOneOff )
		strText += "Account File (";
	else
		strText += "One-Off File (";

	strText += "after customer file is saved)";
	SetWindowText( strText );

	m_listCustomers.SubclassDlgItem ( IDC_LIST, this );
	m_listCustomers.InsertColumn ( 0, "", LVCFMT_LEFT, 25 );
	
	if ( FALSE == m_bOneOff )
	{
		m_listCustomers.InsertColumn ( 1, "Name", LVCFMT_LEFT, 235 );
		m_listCustomers.InsertColumn ( 2, "Recent Activity", LVCFMT_LEFT, 100 );
	}
	else
	{
		m_listCustomers.InsertColumn ( 1, "Name", LVCFMT_LEFT, 335 );
	}
	
	m_listCustomers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	AddList();

	return TRUE;
}

/**********************************************************************/

void CCustomerMoveDlg::AddList()
{
	int nListPos = 0;
	for ( int nListIdx = 0; nListIdx < m_arrayCustomerIdx.GetSize(); nListIdx++ )
	{
		int nCustomerIdx = m_arrayCustomerIdx.GetAt(nListIdx);

		CCustomerCSVRecord Customer;
		GetCustomerArray() -> GetAt( nCustomerIdx, Customer );
		
		m_listCustomers.InsertItem ( nListPos, "" );
		m_listCustomers.SetItem ( nListPos, 1, LVIF_TEXT, Customer.GetDisplayNameFull(), 0, 0, 0, NULL );
		
		if ( FALSE == m_bOneOff )
		{
			CString strDate = "none";

			CString strCustomerDate = Customer.GetTempDate();
			if ( strCustomerDate.GetLength() == 8 )
			{
				strDate = "";
				strDate += strCustomerDate.Right(2);
				strDate += "/";
				strDate += strCustomerDate.Mid(4,2);
				strDate += "/";
				strDate += strCustomerDate.Left(4);
			}

			m_listCustomers.SetItem ( nListPos, 2, LVIF_TEXT, strDate, 0, 0, 0, NULL );
		}
		
		m_listCustomers.SetCheck ( nListPos, Customer.GetMoveToOtherArrayFlag() );
		
		m_listCustomers.SetItemData( nListPos, nCustomerIdx );
		nListPos++;	
	}
}

/**********************************************************************/

void CCustomerMoveDlg::OnButtonSelectAll() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, TRUE );
}

/**********************************************************************/

void CCustomerMoveDlg::OnButtonSelectNone() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, FALSE );
}

/**********************************************************************/

void CCustomerMoveDlg::OnOK() 
{
	UpdateData( TRUE );

	m_arrayCustomerIdx.RemoveAll();

	for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
	{
		int nCustomerIdx = m_listCustomers.GetItemData(n);	
		if ( ( nCustomerIdx >= 0 ) && ( nCustomerIdx < GetCustomerArray() -> GetSize() ) )
		{
			CCustomerCSVRecord CustomerRecord;
			GetCustomerArray() -> GetAt( nCustomerIdx, CustomerRecord );
			CustomerRecord.SetMoveToOtherArrayFlag( m_listCustomers.GetCheck(n) != 0 );
			GetCustomerArray() -> SetAt( nCustomerIdx, CustomerRecord );
		}
	}

	EndDialog( IDOK );
}

/**********************************************************************/




