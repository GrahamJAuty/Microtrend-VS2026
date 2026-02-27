//$$******************************************************************
#include "..\SmartPay4Shared\alerttext.h"
//$$******************************************************************
#include "AlertTextSelectDlg.h"
//$$******************************************************************

CAlertTextSelectDlg::CAlertTextSelectDlg( CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAlertTextSelectDlg::IDD, pParent)
{
	m_nAlertCode = 0;
	m_bClearAllowed = TRUE;
}

//$$******************************************************************

void CAlertTextSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_ALERT1, m_radioAlert[1]);
	DDX_Control(pDX, IDC_RADIO_ALERT2, m_radioAlert[2]);
	DDX_Control(pDX, IDC_RADIO_ALERT3, m_radioAlert[3]);
	DDX_Control(pDX, IDC_RADIO_ALERT4, m_radioAlert[4]);
	DDX_Control(pDX, IDC_RADIO_ALERT5, m_radioAlert[5]);
	DDX_Control(pDX, IDC_RADIO_ALERT6, m_radioAlert[6]);
	DDX_Control(pDX, IDC_RADIO_ALERT7, m_radioAlert[7]);
	DDX_Control(pDX, IDC_RADIO_ALERT8, m_radioAlert[8]);
	DDX_Control(pDX, IDC_RADIO_ALERT9, m_radioAlert[9]);
	DDX_Control(pDX, IDC_RADIO_CLEAR, m_radioClear);
	DDX_Radio(pDX, IDC_RADIO_CLEAR, m_nAlertCode);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CAlertTextSelectDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CAlertTextSelectDlg::CSSAutoShutdownPostInitDialog()
{
	CAlertText alert;
	alert.Open ( Filenames.GetAlertFilename() );				// readonly

	if ( FALSE == m_bClearAllowed )
	{
		m_radioClear.EnableWindow(FALSE);
		SetWindowText ( "Change Alert Code" );
	}

	for (int n = 1; n <= 9; n++)
	{
		CString strText = alert.GetText(n);

		if ((strText.GetLength() == 0) && (m_nAlertCode != n))
		{
			m_radioAlert[n].EnableWindow(FALSE);
		}
		else
		{
			m_radioAlert[n].SetWindowText(strText);
		}
	}

	return TRUE;
}

//$$******************************************************************
