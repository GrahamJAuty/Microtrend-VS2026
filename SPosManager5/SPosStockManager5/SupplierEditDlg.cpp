/**********************************************************************/
#include "SupplierKeyDlg.h"
#include "SupplierEditDlg.h"
/**********************************************************************/

CSupplierEditDlg::CSupplierEditDlg( const char* szTitle, int nSuppIdx, CWnd* pParent)
	: CSSDialog(CSupplierEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSupplierEditDlg)
	m_strAccountNo = _T("");
	m_strKey = _T("");
	//}}AFX_DATA_INIT
	m_nSuppIdx = nSuppIdx;
	DataManager.Supplier.GetAt ( m_nSuppIdx, m_SupplierRecord );

	m_strTitle = szTitle;
	m_strKey = m_SupplierRecord.GetKey();
	m_strOldKey = m_strKey;
	m_strName = m_SupplierRecord.GetName();
	m_strAddress1 = m_SupplierRecord.GetAddress1();
	m_strAddress2 = m_SupplierRecord.GetAddress2();
	m_strAddress3 = m_SupplierRecord.GetAddress3();
	m_strAddress4 = m_SupplierRecord.GetAddress4();
	m_strAddress5 = m_SupplierRecord.GetAddress5();
	m_strPostcode = m_SupplierRecord.GetPostcode();
	m_strPhone = m_SupplierRecord.GetPhone();
	m_strFax = m_SupplierRecord.GetFax();
	m_strEmail = m_SupplierRecord.GetEmail();
	m_strAccountNo = m_SupplierRecord.GetAccountNo();
	m_strFAO = m_SupplierRecord.GetFAO();
}

/**********************************************************************/

void CSupplierEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSupplierEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, Supplier::Name.Max);
	DDX_Text(pDX, IDC_EDIT_ADDRESS1, m_strAddress1);
	DDV_MaxChars(pDX, m_strAddress1, Supplier::Address1.Max);
	DDX_Text(pDX, IDC_EDIT_ADDRESS2, m_strAddress2);
	DDV_MaxChars(pDX, m_strAddress2, Supplier::Address2.Max);
	DDX_Text(pDX, IDC_EDIT_ADDRESS3, m_strAddress3);
	DDV_MaxChars(pDX, m_strAddress3, Supplier::Address3.Max);
	DDX_Text(pDX, IDC_EDIT_ADDRESS4, m_strAddress4);
	DDV_MaxChars(pDX, m_strAddress4, Supplier::Address4.Max);
	DDX_Text(pDX, IDC_EDIT_ADDRESS5, m_strAddress5);
	DDV_MaxChars(pDX, m_strAddress5, Supplier::Address5.Max);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, Supplier::Phone.Max);
	DDX_Text(pDX, IDC_EDIT_POSTCODE, m_strPostcode);
	DDV_MaxChars(pDX, m_strPostcode, Supplier::Postcode.Max);
	DDX_Text(pDX, IDC_EDIT_FAO, m_strFAO);
	DDV_MaxChars(pDX, m_strFAO, Supplier::FAO.Max);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, Supplier::Email.Max);
	DDX_Text(pDX, IDC_EDIT_FAX, m_strFax);
	DDV_MaxChars(pDX, m_strFax, Supplier::Fax.Max);
	DDX_Text(pDX, IDC_EDIT_ACCOUNTNO, m_strAccountNo);
	DDV_MaxChars(pDX, m_strAccountNo, Supplier::AccountNo.Max);
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_strKey);
	DDV_MaxChars(pDX, m_strKey, Supplier::SupplierKey.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSupplierEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSupplierEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSupplierEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );
	return TRUE;  
}

/**********************************************************************/

void CSupplierEditDlg::OnButtonSet() 
{
	UpdateData( TRUE, FALSE );

	CSupplierKeyDlg dlgSupplierKey ( FALSE, FALSE, this );
	dlgSupplierKey.m_strKey = m_strKey;
	if ( dlgSupplierKey.DoModal() != IDOK )
		return;

	CString strKey = dlgSupplierKey.m_strKey;
	if ( strKey == "" )
		return;

	if ( strKey == m_strKey )
		return;

	int nPos;
	if ( DataManager.Supplier.FindSupplierByKey ( strKey, nPos ) == TRUE )
	{
		CString strMsg = "You have already created a supplier\nwith key ";	
		strMsg += strKey;
		Prompter.Error ( strMsg );
		return;
	}

	m_strKey = strKey;
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSupplierEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_SupplierRecord.SetKey ( m_strKey );
		m_SupplierRecord.SetName ( m_strName );
		m_SupplierRecord.SetAddress1 ( m_strAddress1 );
		m_SupplierRecord.SetAddress2 ( m_strAddress2 );
		m_SupplierRecord.SetAddress3 ( m_strAddress3 );
		m_SupplierRecord.SetAddress4 ( m_strAddress4 );
		m_SupplierRecord.SetAddress5 ( m_strAddress5 );
		m_SupplierRecord.SetPostcode ( m_strPostcode );
		m_SupplierRecord.SetFAO ( m_strFAO );
		m_SupplierRecord.SetPhone ( m_strPhone );
		m_SupplierRecord.SetFax ( m_strFax );
		m_SupplierRecord.SetEmail ( m_strEmail );
		m_SupplierRecord.SetAccountNo ( m_strAccountNo );

		if ( m_strKey == m_strOldKey )
			DataManager.Supplier.SetAt ( m_nSuppIdx, m_SupplierRecord );
		else
		{
			DataManager.Supplier.DeleteSupplier ( m_nSuppIdx );
			m_nSuppIdx = DataManager.Supplier.AddSupplier ( m_SupplierRecord );
		}
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

