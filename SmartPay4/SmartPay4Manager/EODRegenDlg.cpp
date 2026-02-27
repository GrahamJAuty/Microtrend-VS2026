//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "EODRegenDlg.h"
//*******************************************************************

CEODRegenDlg::CEODRegenDlg( CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CEODRegenDlg::IDD, pParent)
{
	m_nRegenType = 0;
}

//*******************************************************************

void CEODRegenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_REGEN, m_nRegenType);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CEODRegenDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

//*******************************************************************
