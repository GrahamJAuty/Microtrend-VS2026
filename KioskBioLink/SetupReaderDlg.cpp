//*******************************************************************
#include "SetupReaderDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CSetupReaderDlg, CDialog)
//*******************************************************************

CSetupReaderDlg::CSetupReaderDlg(CWnd* pParent)
	: CDialog(CSetupReaderDlg::IDD, pParent)
{
	m_terminal.Read();
	m_strReader = m_terminal.m_strTerminal;
	m_strPort = m_terminal.m_strPort;
	m_strBaud = m_terminal.m_strBaud;
	m_nUIDFormat = m_terminal.m_nUIDFormat;
	m_nDataFormat = m_terminal.m_nDataFormat;
	m_nLocateOn = m_terminal.m_nLocateOn;
}

//*******************************************************************

CSetupReaderDlg::~CSetupReaderDlg()
{
}

//*******************************************************************

void CSetupReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_READER, m_strReader);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_strPort);
	DDX_CBString(pDX, IDC_COMBO_BAUD, m_strBaud);
	DDX_Control(pDX, IDC_COMBO_READER, m_comboReader);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_CBIndex(pDX, IDC_COMBO_FORMAT, m_nUIDFormat);
	DDX_CBIndex(pDX, IDC_COMBO_DATAFORMAT, m_nDataFormat);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_strPrompt);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATEON, m_nLocateOn);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupReaderDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSetupReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nCurSel = 0;
	CStringArray arReaders;
	if (m_reader.GetReaders(arReaders) > 0)
	{
		for (int i = 0; i < arReaders.GetSize(); i++)
		{
			CString strTerminal = arReaders.GetAt(i);

			m_comboReader.AddString(strTerminal);

			if (strTerminal == m_strReader)
			{
				nCurSel = i;
			}
		}
	}

	m_comboReader.SetCurSel(nCurSel);
	return TRUE;
}

//*******************************************************

void CSetupReaderDlg::OnButtonTest()
{
	CString strDataFormat = "";

	if (UpdateData() == TRUE)
	{
		m_bTestMode = TRUE;
		m_buttonOK.EnableWindow(FALSE);

		if (m_reader.SelectReader(m_strReader, m_strPort, m_strBaud) == TRUE)
		{
			m_reader.SetUIDFormat(m_nUIDFormat);
			m_reader.SetDataFormat(m_nDataFormat);
			m_reader.m_pWnd = &m_staticPrompt;
			m_reader.m_bWaitForCard = TRUE;
			m_reader.SetCardReferenceText("item");

			while (TRUE)
			{
				int nReply = m_reader.GetUID(TRUE);

				if (nReply == 0)		// 0=no error, 1=error, -1 = terminate
				{
					CString strMsg = "";
					strMsg.Format("UID = %s",
						(const char*)m_reader.GetCardNo());

					m_staticPrompt.SetWindowText(strMsg);
					break;
				}
				else
				{
					m_staticPrompt.SetWindowText("");

					if (nReply == 1)
					{
						AfxMessageBox(m_reader.GetError());
					}
					else if (nReply == -1)
					{
						break;
					}
				}
			}

			m_reader.DeselectReader();
		}
		else
		{
			CString strError = m_reader.GetError();
			MessageBox(strError, "Hardware Error Reported");
		}

		m_bTestMode = FALSE;
		m_buttonOK.EnableWindow(TRUE);
	}
}

//**********************************************************************

void CSetupReaderDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		m_terminal.m_strTerminal = m_strReader;
		m_terminal.m_strPort = m_strPort;
		m_terminal.m_strBaud = m_strBaud;
		m_terminal.m_nUIDFormat = m_nUIDFormat;
		m_terminal.m_nDataFormat = m_nDataFormat;
		m_terminal.m_nLocateOn = m_nLocateOn;
		m_terminal.Write();
		CDialog::OnOK();
	}
}

//**********************************************************************

void CSetupReaderDlg::OnCancel()
{
	if (TRUE == m_bTestMode)
	{
		m_reader.Terminate();
	}
	else
	{
		CDialog::OnCancel();
	}
}

//*******************************************************