//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "SetupReaderDlg.h"
//$$******************************************************************

CSetupReaderDlg::CSetupReaderDlg(CWnd* pParent) : CSSDialog(CSetupReaderDlg::IDD, pParent)
{
	m_bTestMode = FALSE;
	m_nOriginalDataFormat = 0;

	m_terminal.Read();
	m_userTexts.Read();

	m_nChangedPaxtonMode = 0;
}

//$$******************************************************************

CSetupReaderDlg::~CSetupReaderDlg()
{
}

//$$******************************************************************

void CSetupReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_READER, m_comboReader);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_Control(pDX, IDC_COMBO_EXTREF1, m_comboExtRef1);
	DDX_Control(pDX, IDC_COMBO_FORMAT1, m_comboFormat1);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
	DDX_Control(pDX, IDC_STATIC_UIDTYPE, m_staticUIDType);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupReaderDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSetupReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_staticPrompt.SetWindowText("");

	int nCurSel = 0;
	m_comboReader.AddString ( szTERMINAL_NONE );

	CStringArray arReaders;
	if ( m_reader.GetReaders ( arReaders ) > 0 )
	{
		for ( int i = 0 ; i < arReaders.GetSize() ; i++ )
		{
			CString strTerminal = arReaders.GetAt ( i );

			m_comboReader.AddString ( strTerminal );

			if (strTerminal == m_terminal.m_strTerminal)
			{
				nCurSel = 1 + i;
			}
		}
	}
	m_comboReader.SetCurSel(nCurSel);

	CStandardCombos::FillPortCombo( m_comboPort, m_terminal.m_strPort );
	CStandardCombos::FillBaudCombo( m_comboBaud, m_terminal.m_strBaud );
	CStandardCombos::FillExtRefCombo( m_comboExtRef1, m_terminal.GetDataFormat1() );
	CStandardCombos::FillFormatCombo( m_comboFormat1, m_terminal.m_nUIDFormat1 );

	m_nOriginalDataFormat = m_terminal.GetDataFormat1();
	
	return TRUE;
}

//$$******************************************************************

void CSetupReaderDlg::OnButtonTest()
{
	TestReader(m_comboFormat1.GetCurSel(), m_comboExtRef1.GetCurSel());
}

//$$******************************************************************

void CSetupReaderDlg::TestReader(int nFormat, int nDataFormat)
{
	CString strDataFormat = "";

	m_staticUIDType.SetWindowText(strDataFormat);

	m_bTestMode = TRUE;
	m_buttonTest.EnableWindow(FALSE);
	m_buttonOK.EnableWindow(FALSE);

	CString strPort, strBaud;
	m_comboPort.GetWindowText(strPort);
	m_comboBaud.GetWindowText(strBaud);

	if (m_reader.SelectReader(GetComboBoxText(m_comboReader), strPort, strBaud) == TRUE)
	{
		m_reader.SetUIDFormat(nFormat);
		m_reader.SetDataFormat(nDataFormat);
		m_reader.m_pWnd = &m_staticPrompt;
		m_reader.m_bWaitForCard = TRUE;

		while (TRUE)
		{
			int nReply = m_reader.GetUID(TRUE, Server.GetLogCardReadsFlag());

			if (nReply == 0)		// 0=no error, 1=error, -1 = terminate
			{
				CString strMsg = "";
				strMsg.Format("UID = %s", (const char*) m_reader.GetCardNo());
				m_staticPrompt.SetWindowText(strMsg);

				switch (nDataFormat)
				{
				case nMIFARE_DATAFORMAT_STANDARD:		strDataFormat = "MiFare";		break;
				case nMIFARE_DATAFORMAT_PAXTON:			strDataFormat = "Paxton";		break;
				case nMIFARE_DATAFORMAT_INNERRANGE:		strDataFormat = "Innerrange";	break;
				}
				m_staticUIDType.SetWindowText(strDataFormat);
				break;
			}
			else
			{
				m_staticPrompt.SetWindowText("");

				if (nReply == 1)
				{
					Prompter.Error(m_reader.GetError());
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
		Prompter.Error(strError, "Hardware Error Reported");
	}

	m_bTestMode = FALSE;
	m_buttonTest.EnableWindow(TRUE);
	m_buttonOK.EnableWindow(TRUE);
}

//$$******************************************************************

void CSetupReaderDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		m_terminal.m_strTerminal = GetComboBoxText(m_comboReader);
		m_comboPort.GetWindowText(m_terminal.m_strPort);
		m_comboBaud.GetWindowText(m_terminal.m_strBaud);
		m_terminal.m_nUIDFormat1 = m_comboFormat1.GetCurSel();
		m_terminal.SetDataFormat1(m_comboExtRef1.GetCurSel());
		m_terminal.Write();

		switch (m_nOriginalDataFormat)
		{
		case 1:
			m_nChangedPaxtonMode = (m_terminal.GetDataFormat1() != 1) ? 1 : 0;
			break;

		default:
			m_nChangedPaxtonMode = (m_terminal.GetDataFormat1() == 1) ? 2 : 0;
			break;
		}

		EndDialog(IDOK);
	}
}

//$$******************************************************************

void CSetupReaderDlg::OnCancel()
{
	if ( m_bTestMode == TRUE )
	{
		m_reader.Terminate();
	}
	else
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************
