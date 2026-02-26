/**********************************************************************/
#include "DefEditServerPresetDlg.h"
/**********************************************************************/

CDefEditServerPresetDlg::CDefEditServerPresetDlg( int nIndex, const char* szDeferName, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditServerPresetDlg::IDD, pParent), m_ServerSelector( m_editServer, m_comboServer ), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_nIndex = nIndex;
	m_pListDataDlg = NULL;
	m_strDeferName = szDeferName;
	//{{AFX_DATA_INIT(CDefEditServerPresetDlg)
	//}}AFX_DATA_INIT	
}

/**********************************************************************/

void CDefEditServerPresetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditServerPresetDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_NAME, m_staticName );
	DDX_Control( pDX, IDC_EDIT_SERVER, m_editServer );
	DDX_Control( pDX, IDC_COMBO_SERVER, m_comboServer );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditServerPresetDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditServerPresetDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_EN_KILLFOCUS(IDC_EDIT_SERVER, OnKillFocusServerNo)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnSelectServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditServerPresetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( m_strDeferName != "" )
	{
		CString strServer;
		m_strDeferName.MakeLower();
		strServer.Format( "(Server names are %s)", (const char*) m_strDeferName );
		m_staticName.SetWindowText( strServer );
		m_comboServer.ShowWindow( SW_HIDE );
	}
	else
	{
		m_staticName.SetWindowText( "" );
		m_staticName.ShowWindow( SW_HIDE );
	}

	GetData();
	return FALSE;  
}

/**********************************************************************/

void CDefEditServerPresetDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditServerPresetDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditServerPresetDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex <= 0 )
			return;

		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nIndex );
	}
}

/**********************************************************************/

void CDefEditServerPresetDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.ServerPresetSystem.GetPresetCount() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditServerPresetDlg::GetData()
{
	CString strTitle;

	CServerPresetMiniInfo infoMini;
	DataManagerNonDb.ServerPresetSystem.GetPreset( m_nIndex, infoMini );
	
	strTitle.Format ( "Server Preset %d", infoMini.m_nPresetNo );
	SetWindowText ( strTitle );

	m_ServerSelector.FillServerComboSystem( infoMini.m_nServerNo );
	m_editServer.SetSel ( 0, -1 );
	m_editServer.SetFocus();
}

/**********************************************************************/

void CDefEditServerPresetDlg::SaveData()
{
	m_ServerSelector.SelectServerFromEditBox( m_strDeferName == "" );
	CServerPresetMiniInfo infoMini;
	DataManagerNonDb.ServerPresetSystem.GetPreset( m_nIndex, infoMini );
	infoMini.m_nServerNo = m_ServerSelector.GetServerNo();
	DataManagerNonDb.ServerPresetSystem.UpdatePreset( m_nIndex, infoMini );

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditServerPresetDlg::OnKillFocusServerNo()
{
	m_ServerSelector.SelectServerFromEditBox( m_strDeferName == "" );
}

/**********************************************************************/

void CDefEditServerPresetDlg::OnSelectServer()
{
	if ( m_strDeferName == "" )
		m_ServerSelector.SelectServerFromCombo();
}

/**********************************************************************/

void CDefEditServerPresetDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

