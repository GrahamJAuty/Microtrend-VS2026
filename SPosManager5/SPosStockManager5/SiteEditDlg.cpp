/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "SiteEditDlg.h"
/**********************************************************************/

CSiteEditDlg::CSiteEditDlg( const char* szTitle, CWnd* pParent )
	: CDialog(CSiteEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSiteEditDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CSiteEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSiteEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SITE_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_ADDRESS_NAME); 
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS1, m_strAddress1);
	DDV_MaxChars(pDX, m_strAddress1, MAX_LENGTH_ADDRESS_STREET); 
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS2, m_strAddress2);
	DDV_MaxChars(pDX, m_strAddress2, MAX_LENGTH_ADDRESS_STREET); 
	DDX_Text(pDX, IDC_EDIT_SITE_ADDRESS3, m_strAddress3);
	DDV_MaxChars(pDX, m_strAddress3, MAX_LENGTH_ADDRESS_STREET); 
	DDX_Text(pDX, IDC_EDIT_SITE_PHONE, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, MAX_LENGTH_ADDRESS_PHONE);
	DDX_Text(pDX, IDC_EDIT_SITE_POSTCODE, m_strPostcode);
	DDV_MaxChars(pDX, m_strPostcode, MAX_LENGTH_ADDRESS_POSTCODE); 
	DDX_Text(pDX, IDC_EDIT_SITE_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, MAX_LENGTH_ADDRESS_EMAIL);
	DDX_Text(pDX, IDC_EDIT_SITE_FAX, m_strFax);
	DDV_MaxChars(pDX, m_strFax, MAX_LENGTH_ADDRESS_FAX);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSiteEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSiteEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSiteEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );
	return TRUE;  
}

/**********************************************************************/

