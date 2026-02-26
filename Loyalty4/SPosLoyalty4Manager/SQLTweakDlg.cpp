//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
//*******************************************************************
#include "SQLTweakDlg.h"
//*******************************************************************

CSQLTweakDlg::CSQLTweakDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog( CSQLTweakDlg::IDD, pParent)
{
}

//*******************************************************************

void CSQLTweakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FILESIZE, m_buttonFileSize);
	DDX_Control(pDX, IDC_BUTTON_SHRINKLOG, m_buttonShrinkLog);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSQLTweakDlg, CSSDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILESIZE, OnButtonFileSize)
	ON_BN_CLICKED(IDC_BUTTON_SHRINKLOG, OnButtonShrinkLog)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSQLTweakDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (System.GetDoneSQLFileSizeFlag() == TRUE)
	{
		m_buttonFileSize.EnableWindow(FALSE);
	}

	if (System.GetDoneSQLShrinkLogFlag() == TRUE)
	{
		m_buttonShrinkLog.EnableWindow(FALSE);
	}

	return TRUE;
}

//*******************************************************************

void CSQLTweakDlg::OnButtonFileSize()
{
	int nCommandFail = 0;
	int nResult = CSQLHelpers::SetLogFileSize(nCommandFail);

	if ( SQLCRUD_ERR_NONE == nResult )
	{
		m_buttonFileSize.EnableWindow(FALSE);
		System.SetDoneSQLFileSizeFlag(TRUE);
		Prompter.Info("SQL Command Completed OK");
	}
	else
	{
		CString strMsg = "";
		strMsg.Format("SQL Command Fail\n\nError code(%d:%d)",
			nResult, nCommandFail);

		Prompter.Error(strMsg);
	}
}

//*******************************************************************

void CSQLTweakDlg::OnButtonShrinkLog()
{
	int nResult = CSQLHelpers::ShrinkLogFile();

	if (SQLCRUD_ERR_NONE == nResult)
	{
		m_buttonShrinkLog.EnableWindow(FALSE);
		System.SetDoneSQLShrinkLogFlag(TRUE);
		Prompter.Info("SQL Command Completed OK");
	}
	else
	{
		CString strMsg = "";
		strMsg.Format("SQL Command Fail\n\nError code(%d)",
			nResult);

		Prompter.Error(strMsg);
	}
}

//*******************************************************************

void CSQLTweakDlg::OnOK()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CSQLTweakDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************
