/**********************************************************************/
#include "DefEditFunctionDlg.h"
/**********************************************************************/

CDefEditFunctionDlg::CDefEditFunctionDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditFunctionDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_nIndex = nIndex;
	m_pListDataDlg = NULL;
	//{{AFX_DATA_INIT(CDefEditFunctionDlg)
	//}}AFX_DATA_INIT
	
}

/**********************************************************************/

void CDefEditFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditFunctionDlg)
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_checkPassword);
	DDX_Control(pDX, IDC_CHECK_AUDIT, m_checkAudit);
	DDX_Control(pDX, IDC_CHECK_LOG, m_checkLog);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditFunctionDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditFunctionDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditFunctionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit( IDC_EDIT_LEVEL, SS_NUM, 1, "%d" );
	GetData();
	return FALSE;  
}

/**********************************************************************/
CEdit* CDefEditFunctionDlg::GetEditLevel(){ return GetEdit( IDC_EDIT_LEVEL ); }
/**********************************************************************/

void CDefEditFunctionDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditFunctionDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditFunctionDlg::SpinPrevious()
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

void CDefEditFunctionDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < DataManagerNonDb.FunctionSystem.GetFunctionCount() - 1 )
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

void CDefEditFunctionDlg::GetData()
{
	CString strTitle;

	CFunctionAuthMiniInfo infoMini;
	DataManagerNonDb.FunctionSystem.GetFunction( m_nIndex, infoMini );
	
	strTitle.Format ( "F%4.4d %s", infoMini.m_nFuncNo, (const char*) infoMini.m_strName );
	SetWindowText ( strTitle );
	
	CString strLevel;
	strLevel.Format( "%d", infoMini.m_nAuthLevel );
	GetEditLevel() -> SetWindowText( strLevel );
	
	m_checkPassword.SetCheck( infoMini.m_bPassword );
	m_checkAudit.SetCheck( infoMini.m_bAudit );
	m_checkLog.SetCheck( infoMini.m_bLog );

	GetEditLevel() -> SetSel ( 0, -1 );
	GetEditLevel() -> SetFocus();
}

/**********************************************************************/

void CDefEditFunctionDlg::SaveData()
{
	CFunctionAuthMiniInfo infoMini;
	DataManagerNonDb.FunctionSystem.GetFunction( m_nIndex, infoMini );

	CString strLevel;
	GetEditLevel() -> GetWindowText( strLevel );

	infoMini.m_nAuthLevel = atoi( strLevel );
	infoMini.m_bPassword = ( m_checkPassword.GetCheck() != 0 );
	infoMini.m_bAudit = ( m_checkAudit.GetCheck() != 0 );
	infoMini.m_bLog = ( m_checkLog.GetCheck() != 0 );
	DataManagerNonDb.FunctionSystem.UpdateFunction( m_nIndex, infoMini );

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditFunctionDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
