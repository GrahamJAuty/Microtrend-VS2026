//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "PasswordDlg.h"
//$$******************************************************************

CPasswordDlg::CPasswordDlg(CWnd* pParent )
	: CSSDialog(CPasswordDlg::IDD, pParent)
{
	m_bTestMode = FALSE;
	m_bReadCardAllowed = FALSE;
	m_strCardNo = "";
	m_strPassword = "";
	m_terminal.Read();
}

//$$******************************************************************

CPasswordDlg::~CPasswordDlg()
{
}

//$$******************************************************************

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_READCARD, m_buttonCard);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CPasswordDlg, CSSDialog)
	ON_BN_CLICKED(IDC_BUTTON_READCARD, OnButtonReadCard)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (FALSE == m_bReadCardAllowed)
	{
		ShowAndEnableWindow(&m_buttonCard, FALSE);
		ShowAndEnableWindow(&m_staticPrompt, FALSE);
		ResizeDialog(301, 81);
	}

	return TRUE;  
}

//$$******************************************************************

void CPasswordDlg::OnButtonReadCard()
{
	m_bTestMode = TRUE;
	m_buttonOK.EnableWindow(FALSE);
	m_editPassword.EnableWindow(FALSE);

	if ( m_reader.SelectReader ( m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud ) == TRUE )
	{
		m_reader.SetUIDFormat ( m_terminal.m_nUIDFormat1 );
		m_reader.SetDataFormat( m_terminal.GetDataFormat1() );
		m_reader.m_pWnd = &m_staticPrompt;
		m_reader.m_bWaitForCard = TRUE;

		while ( TRUE )
		{
			int nReply = m_reader.GetUID ( TRUE, Server.GetLogCardReadsFlag());
		
			if ( nReply == 0 )		// 0=no error, 1=error, -1 = terminate
			{
				m_strCardNo = m_reader.GetCardNo();
				break;
			}
			else
			{
				m_staticPrompt.SetWindowText ( "" );

				if ( nReply == 1 )
					Prompter.Error( m_reader.GetError() );

				else if ( nReply == -1 )
					break;
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
	m_editPassword.EnableWindow(TRUE);
	m_editPassword.SetFocus();

	if (m_strCardNo != "")
	{
		OnOK();
	}
}

//$$******************************************************************

void CPasswordDlg::OnCancel()
{
	if (m_bTestMode == TRUE)
	{
		m_reader.Terminate();
	}
	else
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************

void CPasswordDlg::OnOK()
{
	m_strPassword = GetEditBoxText(m_editPassword);
	EndDialog(IDOK);
}

//$$******************************************************************
