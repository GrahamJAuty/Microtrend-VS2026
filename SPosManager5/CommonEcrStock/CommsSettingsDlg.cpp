/**********************************************************************/
#include "CommsHandshaker.h"
#include "LockManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsSettingsDlg.h"
/**********************************************************************/

CCommsSettingsDlg::CCommsSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommsSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommsSettingsDlg)
	//}}AFX_DATA_INIT
	m_bBuffer = SysInfo.GetProcessCashRSPBufferFlag();
}

/**********************************************************************/

void CCommsSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommsSettingsDlg)
	DDX_Check(pDX, IDC_CHECK_BUFFER, m_bBuffer);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCommsSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CCommsSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCommsSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_comboSpeed.AddString( "1 (slowest)" );
	m_comboSpeed.AddString( "2" );
	m_comboSpeed.AddString( "3" );
	m_comboSpeed.AddString( "4" );
	m_comboSpeed.AddString( "5" );
	m_comboSpeed.AddString( "6" );
	m_comboSpeed.AddString( "7" );
	m_comboSpeed.AddString( "8" );
	m_comboSpeed.AddString( "9" );
	m_comboSpeed.AddString( "10 (fastest)" );

	int nSel = SysInfo.GetCommsThreadSpeed();
	if ( ( nSel >= 1 ) && ( nSel <= 10 ) )
		m_comboSpeed.SetCurSel( nSel - 1 );
	else
		m_comboSpeed.SetCurSel( 0 );

	return TRUE;  
}

/**********************************************************************/

void CCommsSettingsDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSel = m_comboSpeed.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel <= 9 ) )
			SysInfo.SetCommsThreadSpeed( nSel + 1 );
		else
			SysInfo.SetCommsThreadSpeed( 1 );

		CLockManagerInfo infoLock;
		infoLock.m_bLockAllUsers = FALSE;
		infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
		infoLock.m_bStopComms = TRUE;
		infoLock.m_bStopCommsPMSExemption = FALSE;
		infoLock.m_strStopCommsPrompt = "Save comms settings";
		infoLock.m_bStopPosTray = TRUE;
		infoLock.m_pParentWnd = this;

		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == TRUE )
		{
			SysInfo.SetProcessCashRSPBufferFlag ( m_bBuffer != 0 );
			SysInfo.Write();
			EndDialog ( IDOK );
		}
	}
}

/**********************************************************************/
