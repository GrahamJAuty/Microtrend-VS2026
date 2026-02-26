/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "IDraughtOptions.h"
#include "IDraughtExportManager.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "PropPageIDraughtOptionsGeneral.h"
/**********************************************************************/

CPropPageIDraughtOptionsGeneral::CPropPageIDraughtOptionsGeneral() : CSSPropertyPage(CPropPageIDraughtOptionsGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageIDraughtOptionsGeneral)
	//}}AFX_DATA_INIT
	m_strServer = "";
	m_strUserName = "";
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
	m_nExportDelayTime = 1;
	m_nConnectRetryTime = 1;
}

/**********************************************************************/

CPropPageIDraughtOptionsGeneral::~CPropPageIDraughtOptionsGeneral()
{
}

/**********************************************************************/

void CPropPageIDraughtOptionsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageIDraughtOptionsGeneral)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Text( pDX, IDC_EDIT_SERVER, m_strServer );
	DDV_MaxChars(pDX, m_strUserName, MAX_LENGTH_IDRAUGHT_SERVER);
	DDX_Text( pDX, IDC_EDIT_USERNAME, m_strUserName );
	DDV_MaxChars(pDX, m_strUserName, MAX_LENGTH_IDRAUGHT_USERNAME);
	DDX_Text( pDX, IDC_EDIT_PAUSE, m_nExportDelayTime );
	DDV_MinMaxInt( pDX, m_nExportDelayTime, 1, 6 );
	DDX_Text( pDX, IDC_EDIT_RETRY, m_nConnectRetryTime );
	DDV_MinMaxInt( pDX, m_nConnectRetryTime, 1, 30 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageIDraughtOptionsGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageIDraughtOptionsGeneral)
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageIDraughtOptionsGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit( IDC_EDIT_PAUSE, SS_NUM, 1, "%d" );
	SubclassEdit( IDC_EDIT_RETRY, SS_NUM, 2, "%d" );

	CString strPassword = IDraughtOptions.GetFTPPassword();
	
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

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsGeneral::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPageIDraughtOptionsGeneral::GetRecordData()
{
	m_strServer = IDraughtOptions.GetFTPServer();
	m_strUserName = IDraughtOptions.GetFTPUserName();
	m_nExportDelayTime = IDraughtOptions.GetExportDelayTime();
	m_nConnectRetryTime = IDraughtOptions.GetConnectRetryTime();
}

/**********************************************************************/

void CPropPageIDraughtOptionsGeneral::OnButtonPassword()
{
	CDefEditDlg dlg( "Set IDraught FTP Password", "", MAX_LENGTH_IDRAUGHT_PASSWORD, "Password", this );
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

void CPropPageIDraughtOptionsGeneral::SaveRecord()
{
	IDraughtOptions.SetFTPServer( m_strServer );
	IDraughtOptions.SetFTPUserName( m_strUserName );
	IDraughtOptions.SetExportDelayTime( m_nExportDelayTime );
	IDraughtOptions.SetConnectRetryTime( m_nConnectRetryTime );

	if ( m_strNewPassword != "" )
		IDraughtOptions.SetFTPPassword( m_strNewPassword );
}

/**********************************************************************/
