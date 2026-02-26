/**********************************************************************/
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "NetworkEposTypeDlg.h"
/**********************************************************************/
#include "NetworkEditDlg.h"
/**********************************************************************/

CNetworkEditDlg::CNetworkEditDlg( int nNetworkIdx, CWnd* pParent)
	: CSSDialog(CNetworkEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkEditDlg)
	//}}AFX_DATA_INIT	
	m_nNetworkIdx = nNetworkIdx;
	m_nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );
	m_nSPOSVersion = dbNetwork.GetSPOSVersion( nNetworkIdx );
	m_strName = dbNetwork.GetName ( nNetworkIdx );
	m_strFilePath = dbNetwork.GetFileNetworkPath( nNetworkIdx );
}

/**********************************************************************/

void CNetworkEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CNetworkEditDlg)
	DDX_Control(pDX, IDC_EDIT_SPOSVERSION, m_editSPOSVersion);
	DDX_Control(pDX, IDC_COMBO_CONNECTION, m_comboConnection);
	DDX_Control(pDX, IDC_STATIC_LINE1, m_staticLine1);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_editFilePath);
	DDX_Control(pDX, IDC_BUTTON_FILE_PATH, m_buttonFilePath);
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_strFilePath);
	DDV_MaxChars(pDX, m_strFilePath, MAX_LENGTH_NETWORK_REMPATH);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CNetworkEditDlg, CDialog)
	//{{AFX_MSG_MAP(CNetworkEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECTION, OnSelectConnection)
	ON_BN_CLICKED(IDC_BUTTON_FILE_PATH, OnButtonFilePath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CNetworkEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format ( "Edit %s", dbNetwork.GetSystemName( m_nNetworkIdx ) );
	SetWindowText ( strTitle );

	CString strSPOSVersion = m_SPOSVersionArray.GetSPOSVersionText( m_nSPOSVersion );
	m_editSPOSVersion.SetWindowText( strSPOSVersion );

	m_comboConnection.AddString( "None" );
	m_comboConnection.AddString( "Real Time" );
	m_comboConnection.AddString( "Batch" );

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_REALTIME:
		m_comboConnection.SetCurSel(1);
		break;

	case CONNECTION_TYPE_STANDARD_BATCH:
		m_comboConnection.SetCurSel(2);
		break;

	case CONNECTION_TYPE_STANDARD_NONE:
	default:
		m_comboConnection.SetCurSel(0);
		break;
	}
	OnSelectConnection();
	
	return TRUE;  	
}

/**********************************************************************/

void CNetworkEditDlg::OnButtonSet()
{
	CNetworkEposTypeDlg dlg( m_nSPOSVersion, m_SPOSVersionArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_nSPOSVersion != -1 )
		{
			m_nSPOSVersion = dlg.m_nSPOSVersion;
			m_editSPOSVersion.SetWindowText( m_SPOSVersionArray.GetSPOSVersionText( m_nSPOSVersion ) );
		}
	}		
}

/**********************************************************************/

void CNetworkEditDlg::OnSelectConnection()
{
	int nSel = m_comboConnection.GetCurSel();
	ShowAndEnableWindow( &m_staticLine1, ( nSel != 0 ) );
	ShowAndEnableWindow( &m_editFilePath, ( nSel != 0 ) );
	ShowAndEnableWindow( &m_buttonFilePath, ( nSel != 0 ) );
}

/**********************************************************************/

void CNetworkEditDlg::OnButtonFilePath()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Select Network Path", NULL, this) == TRUE)
	{
		m_editFilePath.SetWindowText(strPathname);
	}
}

/**********************************************************************/

void CNetworkEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		switch( m_comboConnection.GetCurSel() )
		{
		case 1:
			m_nConnectionType = CONNECTION_TYPE_STANDARD_REALTIME;
			break;

		case 2:
			m_nConnectionType = CONNECTION_TYPE_STANDARD_BATCH;
			break;

		case 0:
		default:
			m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
			break;
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
