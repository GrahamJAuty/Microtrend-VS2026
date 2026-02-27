/**********************************************************************/
#include "BackupTypeDlg.h"
/**********************************************************************/

CBackupTypeDlg::CBackupTypeDlg(CWnd* pParent) : CSSAutoShutdownDialog(AutoShutdownHelper,CBackupTypeDlg::IDD, pParent)
{
	m_nType = 0;
}

/**********************************************************************/

CBackupTypeDlg::~CBackupTypeDlg()
{
}

/**********************************************************************/

void CBackupTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_RADIO_TYPE, m_nType);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBackupTypeDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

/**********************************************************************/
