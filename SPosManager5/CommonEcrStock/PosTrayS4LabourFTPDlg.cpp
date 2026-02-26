/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayS4LabourFTPDlg.h"
/**********************************************************************/

CPosTrayS4LabourFTPDlg::CPosTrayS4LabourFTPDlg( CWnd* pParent )
	: CSSDialog(CPosTrayS4LabourFTPDlg::IDD, pParent)
{
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
}

/**********************************************************************/

CPosTrayS4LabourFTPDlg::~CPosTrayS4LabourFTPDlg()
{
}

/**********************************************************************/

void CPosTrayS4LabourFTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_EDIT_SERVER, m_editServer );
	DDX_Control( pDX, IDC_EDIT_USERNAME, m_editUserName );
	DDX_Control( pDX, IDC_EDIT_PASSWORD, m_editPassword );
	DDX_Control( pDX, IDC_BUTTON_PASSWORD, m_buttonPassword );
	DDX_Control(pDX, IDC_COMBO_HOUR, m_comboHour);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPosTrayS4LabourFTPDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPosTrayS4LabourFTPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editServer.LimitText( MAX_LENGTH_FTP_SERVER );
	m_editUserName.LimitText( MAX_LENGTH_FTP_USERNAME );
	
	m_editServer.SetWindowText( PosTrayOptions.GetS4LabourFTPServer() );
	m_editUserName.SetWindowText( PosTrayOptions.GetS4LabourFTPUserName() );
	
	CString strPassword = PosTrayOptions.GetS4LabourFTPPassword();
	
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

	for ( int n = 0; n <= 23; n++ )
	{
		CString strHour;
		strHour.Format( "%2.2d:00", n );
		m_comboHour.AddString( strHour );
	}

	{
		int nSel = PosTrayOptions.GetS4LabourExportHour();
		if ( ( nSel >= 0 ) && ( nSel <= 23 ) )
			m_comboHour.SetCurSel(nSel);
		else
			m_comboHour.SetCurSel(0);
	}

	return TRUE;
}

/**********************************************************************/

void CPosTrayS4LabourFTPDlg::OnButtonPassword()
{
	CDefEditDlg dlg( "Set S4 Labour FTP Password", "", MAX_LENGTH_FTP_PASSWORD, "Password", this );
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

void CPosTrayS4LabourFTPDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		CString str;
		m_editServer.GetWindowText( str );
		PosTrayOptions.SetS4LabourFTPServer( str );

		m_editUserName.GetWindowText( str );
		PosTrayOptions.SetS4LabourFTPUserName( str );

		if ( m_strNewPassword != "" )
			PosTrayOptions.SetS4LabourFTPPassword( m_strNewPassword );

		PosTrayOptions.SetS4LabourExportHour( m_comboHour.GetCurSel() );

		EndDialog( IDOK );
	}
}

/**********************************************************************/

