/**********************************************************************/
#include "..\SmartPay4Shared\EmailOptions.h"
/**********************************************************************/
#include "DefEditEmailDlg.h"
/**********************************************************************/

CDefEditEmailDlg::CDefEditEmailDlg( const char* szTitle, CStringArray& arrayEmail, CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CDefEditEmailDlg::IDD, pParent), m_arrayEmail ( arrayEmail )
{
	//{{AFX_DATA_INIT(CDefEditEmailDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;

	m_strEmail = "";
	
	for ( int n = 0; n < m_arrayEmail.GetSize(); n++ )
	{
		if ( m_strEmail != "" )
			m_strEmail += "\r\n";

		m_strEmail += m_arrayEmail.GetAt( n );
	}
}

/**********************************************************************/

void CDefEditEmailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditEmailDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
	DDV_MaxChars(pDX, m_strEmail, MAX_LENGTH_ADDRESS_MULTI_EMAIL);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_ADDRESS_NAME); 
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditEmailDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CDefEditEmailDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditEmailDlg::CSSAutoShutdownPostInitDialog ()
{
	SetWindowText ( m_strTitle );
	return TRUE;  
}

/**********************************************************************/

void CDefEditEmailDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_arrayEmail.RemoveAll();
		
		CString strAddress = "";

		int nLength = m_strEmail.GetLength();
		
		for ( int nPos = 0; nPos < nLength; nPos++ )
		{
			if ( nPos == nLength - 1 )
				strAddress += m_strEmail.GetAt(nPos);
			else
			{
				if ( m_strEmail.Mid( nPos, 2 ) == "\r\n" )
				{
					if ( strAddress != "" )
						m_arrayEmail.Add( strAddress );

					strAddress = "";
					
					nPos++;
				}
				else
					strAddress += m_strEmail.GetAt(nPos);
			}
		}

		if ( strAddress != "" )
			m_arrayEmail.Add( strAddress );
	}

	EndDialog( IDOK );
}

/**********************************************************************/
