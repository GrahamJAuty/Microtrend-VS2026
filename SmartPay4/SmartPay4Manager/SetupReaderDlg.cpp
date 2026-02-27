//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "SetupReaderDlg.h"
//$$******************************************************************

CSetupReaderDlg::CSetupReaderDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupReaderDlg::IDD, pParent)
{
	m_strPrompt = "";
	m_bTestMode = FALSE;
	m_terminal.Read();
	m_strReader = m_terminal.m_strTerminal;
}

//$$******************************************************************

void CSetupReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
	DDX_Control(pDX, IDC_BUTTON_TEST2, m_buttonTest2);
	DDX_Control(pDX, IDC_STATIC_UIDTYPE, m_staticUIDType);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
	DDX_Control(pDX, IDC_COMBO_READER, m_comboReader);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_Control(pDX, IDC_COMBO_EXTREF1, m_comboExtRef1);
	DDX_Control(pDX, IDC_COMBO_EXTREF2, m_comboExtRef2);
	DDX_Control(pDX, IDC_COMBO_FORMAT1, m_comboFormat1);
	DDX_Control(pDX, IDC_COMBO_FORMAT2, m_comboFormat2);
	DDX_Control(pDX, IDC_COMBO_DEFAULTFORMAT, m_comboDefaultFormat);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_CBString(pDX, IDC_COMBO_READER, m_strReader);
	DDX_Text(pDX, IDC_STATIC_PROMPT, m_strPrompt);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupReaderDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, OnButtonTest2)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSetupReaderDlg::CSSAutoShutdownPostInitDialog()
{
	int nCurSel = 0;
	m_comboReader.AddString(szTERMINAL_NONE);

	CStringArray arReaders;
	if (m_reader.GetReaders(arReaders) > 0)
	{
		for (int i = 0; i < arReaders.GetSize(); i++)
		{
			CString strTerminal = arReaders.GetAt(i);

			m_comboReader.AddString(strTerminal);

			if (strTerminal == m_strReader)
			{
				nCurSel = 1 + i;
			}
		}
	}
	m_comboReader.SetCurSel(nCurSel);

	CStandardCombos::FillPortCombo(m_comboPort, m_terminal.m_strPort);
	CStandardCombos::FillBaudCombo(m_comboBaud, m_terminal.m_strBaud);
	CStandardCombos::FillExtRefCombo(m_comboExtRef1, m_terminal.m_nDataFormat1);
	CStandardCombos::FillExtRefCombo(m_comboExtRef2, m_terminal.m_nDataFormat2);
	CStandardCombos::FillFormatCombo(m_comboFormat1, m_terminal.m_nUIDFormat1);
	CStandardCombos::FillFormatCombo(m_comboFormat2, m_terminal.m_nUIDFormat2);
	CStandardCombos::FillDefaultExtRefCombo(m_comboDefaultFormat, m_terminal.m_nDefaultFormat);

	return TRUE;
}

//$$******************************************************************

void CSetupReaderDlg::OnButtonTest()
{
	if (UpdateData() == TRUE)
	{
		TestReader(m_comboFormat1.GetCurSel(), m_comboExtRef1.GetCurSel());
	}
}

//$$******************************************************************

void CSetupReaderDlg::OnButtonTest2()
{
	if (UpdateData() == TRUE)
	{
		TestReader(m_comboFormat2.GetCurSel(), m_comboExtRef2.GetCurSel());
	}
}

//$$******************************************************************

void CSetupReaderDlg::TestReader ( int nFormat, int nDataFormat )
{
	CString strDataFormat = "";

	if ( UpdateData() == TRUE )
	{
		m_staticUIDType.SetWindowText ( strDataFormat );

		m_bTestMode = TRUE;
		m_buttonOK.EnableWindow(FALSE);
		m_buttonTest.EnableWindow(FALSE);
		m_buttonTest2.EnableWindow(FALSE);

		CString strPort, strBaud;
		m_comboPort.GetWindowText( strPort );
		m_comboBaud.GetWindowText( strBaud );

		if ( m_reader.SelectReader ( m_strReader, strPort, strBaud ) == TRUE )
		{
			m_reader.SetUIDFormat ( nFormat );
			m_reader.SetDataFormat ( nDataFormat );
			m_reader.m_pWnd = &m_staticPrompt;
			m_reader.m_bWaitForCard = TRUE;

			while ( TRUE )
			{
				int nReply = m_reader.GetUID ( TRUE, Server.GetLogCardReadsFlag() );
		
				if ( nReply == 0 )		// 0=no error, 1=error, -1 = terminate
				{
					CString strMsg = "";
					strMsg.Format ( "UID = %s", 
						(const char*) m_reader.GetCardNo() );

					m_staticPrompt.SetWindowText ( strMsg );

					switch ( nDataFormat )
					{
					case nMIFARE_DATAFORMAT_STANDARD:	
						strDataFormat = "MiFare";	
						break;

					case nMIFARE_DATAFORMAT_PAXTON:	
						strDataFormat = "Paxton";
						break;

					case nMIFARE_DATAFORMAT_INNERRANGE:	
						strDataFormat = "Innerrange";
						break;
					}
					
					m_staticUIDType.SetWindowText ( strDataFormat );
					break;
				}

				else
				{
					m_staticPrompt.SetWindowText ( "" );

					if (nReply == 1)
					{
						Prompter.Error(m_reader.GetError());
					}
					else if ( nReply == -1 )
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
			Prompter.Error( strError, "Hardware Error Reported" );
		}

		m_bTestMode = FALSE;
		m_buttonOK.EnableWindow(TRUE);
		m_buttonTest.EnableWindow(TRUE);
		m_buttonTest2.EnableWindow(TRUE);
	}
}

//$$******************************************************************

void CSetupReaderDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		m_terminal.m_strTerminal = m_strReader;
		m_comboPort.GetWindowText(m_terminal.m_strPort);
		m_comboBaud.GetWindowText(m_terminal.m_strBaud);
		m_terminal.m_nDefaultFormat = m_comboDefaultFormat.GetCurSel();
		m_terminal.m_nUIDFormat1 = m_comboFormat1.GetCurSel();
		m_terminal.m_nUIDFormat2 = m_comboFormat2.GetCurSel();
		m_terminal.m_nDataFormat1 = m_comboExtRef1.GetCurSel();
		m_terminal.m_nDataFormat2 = m_comboExtRef2.GetCurSel();
		m_terminal.Write();

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

void CSetupReaderDlg::CSSAutoShutdownLogOff()
{
	if (m_bTestMode == TRUE)
	{
		m_reader.Terminate();
	}

	EndDialog(IDCANCEL);
}

//$$******************************************************************
