/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "SportsBookerNetworkDateDlg.h"
/**********************************************************************/
#include "SportsBookerNetworkEditDlg.h"
/**********************************************************************/

CSportsBookerNetworkEditDlg::CSportsBookerNetworkEditDlg( int nNetworkIdx, CSportsBookerWebAPISalesDateTime& dates, CWnd* pParent)
	: CSSDialog(CSportsBookerNetworkEditDlg::IDD, pParent), m_dates( dates )
{
	//{{AFX_DATA_INIT(CSportsBookerNetworkEditDlg)
	//}}AFX_DATA_INIT

	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
	
	m_nNetworkIdx = nNetworkIdx;
	m_nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );
	
	m_strName = dbNetwork.GetName ( nNetworkIdx );
	m_strSite = dbNetwork.GetSportsBookerSite( nNetworkIdx );
	m_strUserName = dbNetwork.GetSportsBookerUserName( nNetworkIdx );
	m_nTimeOut = dbNetwork.GetSportsBookerTimeOut( m_nNetworkIdx );
}

/**********************************************************************/

void CSportsBookerNetworkEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSportsBookerNetworkEditDlg)
	DDX_Control(pDX, IDC_EDIT_SITE, m_editSite);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_buttonPassword);
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	DDX_Control(pDX, IDC_BUTTON_DATE, m_buttonDate);
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	
	DDX_Text(pDX, IDC_EDIT_SITE, m_strSite);
	DDV_MaxChars(pDX, m_strSite, MAX_LENGTH_SPTBOOK_SITE);

	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, MAX_LENGTH_SPTBOOK_USERNAME);

	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, MIN_SB_TIMEOUT, MAX_SB_TIMEOUT );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSportsBookerNetworkEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSportsBookerNetworkEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD, OnButtonPassword)
	ON_BN_CLICKED(IDC_BUTTON_DATE, OnButtonDateStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSportsBookerNetworkEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//TITLE
	CString strTitle;
	strTitle.Format ( "Edit %s", dbNetwork.GetSystemName( m_nNetworkIdx ) );
	SetWindowText ( strTitle );
	
	m_editSite.SetWindowText( m_strSite );
	m_editUserName.SetWindowText( m_strUserName );
		
	{
		CString strPassword = dbNetwork.GetSportsBookerPassword( m_nNetworkIdx );
		
		if ( strPassword != "" )
		{
			m_editPassword.SetWindowText( "<The password has been set>" );
			m_bGotOldPassword = TRUE;
		}
		else
		{
			m_editPassword.SetWindowText( "<No password set>" );
			m_bGotOldPassword = FALSE;
		}
	}

	ShowDate();

	if ( m_dates.GetDateTimeStatus() == SB_WEBDATE_NEXT )
	{
		ShowAndEnableWindow( &m_buttonDate, FALSE ); 
		m_staticDate.SetWindowText( "Latest Web API Sale" );
	}
	
	return TRUE;  	
}

/**********************************************************************/

void CSportsBookerNetworkEditDlg::ShowDate()
{
	m_editDate.SetWindowText( m_dates.GetDisplayDateTimeNextRead() );
}

/**********************************************************************/

void CSportsBookerNetworkEditDlg::OnButtonPassword()
{
	CChangePasswordDlg dlg( "Set Sports Booker Password", "", MAX_LENGTH_SPTBOOK_PASSWORD, "Password", this );
	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_strName != "" )
		{
			m_strNewPassword = dlg.m_strName;
			
			if ( TRUE == m_bGotOldPassword )
				m_editPassword.SetWindowText ( "<The password has been changed>" );
			else
				m_editPassword.SetWindowText ( "<The password has been set>" );
		}
	}
}

/**********************************************************************/

void CSportsBookerNetworkEditDlg::OnButtonDateStart()
{
	CSportsBookerNetworkDateDlg dlgDate( m_dates, this );
	
	if ( dlgDate.DoModal() == IDOK )
		ShowDate();
}

/**********************************************************************/

void CSportsBookerNetworkEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		EndDialog ( IDOK );
}

/**********************************************************************/
