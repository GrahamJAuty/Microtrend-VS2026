//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "FindDlg.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "CardNoDlg.h"
//$$******************************************************************

CCardNoDlg::CCardNoDlg(const char* szTitle, const char* szCardNo, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CCardNoDlg::IDD, pParent)
{
	m_strTitle = szTitle;
	m_pParent = pParent;
	m_strInputCardNo = szCardNo;
	m_nRecordCount = 1;
	m_bReaderMode = FALSE;

	m_strMiFareNo = "";
	m_strOutputCardNo = "";
	m_bBrowseButtonAllowed = TRUE;
	m_bCountButtonAllowed = FALSE;
	m_bReaderButtonAllowed = (System.GetInterfaceType() == nINTERFACE_BIOMETRIC) ? FALSE : TRUE;

	m_terminal.Read();													// reader Mifare reader data in case needed
	if (m_terminal.m_strTerminal == szTERMINAL_NONE)
	{
		m_bReaderButtonAllowed = FALSE;
	}
}

//$$******************************************************************

void CCardNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_READER, m_buttonReader);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_editCount);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_staticPrompt);
	DDX_Control(pDX, IDC_STATIC_RECORDCOUNT, m_staticRecordCount);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCardNoDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_READER, OnButtonReader)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CCardNoDlg::CSSAutoShutdownPostInitDialog()
{
	SubclassEdit( IDC_EDIT_CARDNO, SS_NUM, 10, "%s" );
	GetEditCardNo() -> SetWindowText(m_strInputCardNo);
	SetEditBoxInt(m_editCount, m_nRecordCount);

	SetWindowText ( m_strTitle );

	if ( m_bReaderButtonAllowed == FALSE )
	{
		m_buttonReader.ShowWindow(SW_HIDE);
		m_staticPrompt.ShowWindow(SW_HIDE);
	}

	if (m_bBrowseButtonAllowed == FALSE)
	{
		m_buttonBrowse.ShowWindow(SW_HIDE);
	}

	if ( m_bCountButtonAllowed == FALSE )
	{
		m_staticRecordCount.ShowWindow(SW_HIDE);
		m_editCount.ShowWindow(SW_HIDE);
	}

	return TRUE; 
}

//$$******************************************************************

bool CCardNoDlg::ValidateUserInput()
{
	Server.Read(Filenames.GetServerDataFilename());

	bool bReply = FALSE;
	if ((bReply = Server.ValidateUserID(m_strInputCardNo, m_strOutputCardNo)) == FALSE)
	{
		Prompter.Error(Server.GetError(), "Validation Error");
		GetEditCardNo()->SetWindowText(m_strInputCardNo);
		GetEditCardNo()->SetFocus();
		GetEditCardNo()->SetSel(0, -1);
	}

	return bReply;
}

//$$******************************************************************

void CCardNoDlg::OnButtonBrowse()
{
	CFindDlg dlg ( "", this );
	if ( dlg.DoModal() == IDOK )
	{
		m_strInputCardNo = FormatInt64Value(dlg.m_nRequestedUserID);
		GetEditCardNo()->SetWindowText(m_strInputCardNo);
	}
}

//$$******************************************************************

void CCardNoDlg::SetReaderMode ( bool bMode )
{
	m_bReaderMode = bMode;

	bool bAllowed = ( m_bReaderMode == TRUE ) ? FALSE : TRUE;

	GetEditCardNo()-> EnableWindow(bAllowed);
	m_buttonBrowse.EnableWindow(bAllowed);
	m_buttonReader.EnableWindow(bAllowed);
	m_buttonOK.EnableWindow(bAllowed);

	if (m_bReaderMode == FALSE)
	{
		GetEditCardNo()->SetFocus();
	}
}

//$$******************************************************************

void CCardNoDlg::OnButtonReader()
{
	m_strMiFareNo = "";
	m_strInputCardNo = "";

	SetReaderMode(TRUE);				// set dialog in reader mode / inhibit other keys

	if (m_reader.SelectReader(m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud) == TRUE)
	{
		int nFormat = m_terminal.m_nUIDFormat1;
		int nDataFormat = m_terminal.m_nDataFormat1;
		CString strPrompt = "Present card";

		CSQLRepositoryUserText RepoText;

		if (m_terminal.m_nDefaultFormat == 1)
		{
			nFormat = m_terminal.m_nUIDFormat2;
			nDataFormat = m_terminal.m_nDataFormat2;

			if (RepoText.IsExtRef2Reqd() == TRUE)
			{
				strPrompt.Format("Present %s",
					(const char*)RepoText.GetExtRef2Text());
			}
		}
		else
		{
			if (RepoText.IsExtRef2Reqd() == TRUE)
			{
				strPrompt.Format("Present %s",
					(const char*)RepoText.GetExtRef1Text());
			}
		}

		m_reader.SetUIDFormat(nFormat);
		m_reader.SetDataFormat(nDataFormat);
		m_reader.SetPresentPrompt(strPrompt);
		m_reader.m_pWnd = &m_staticPrompt;
		m_reader.m_bWaitForCard = TRUE;

		while (TRUE)
		{
			int nReply = m_reader.GetUID(TRUE,Server.GetLogCardReadsFlag());
			m_staticPrompt.SetWindowText("");

			if (nReply == 0)		// 0=no error, 1=error, -1 = terminate
			{
				m_strMiFareNo = m_reader.GetCardNo();
				GetEditCardNo()->SetWindowText(m_strInputCardNo);
				break;
			}
			else if (nReply == -1)
			{
				break;
			}
			else
			{
				Prompter.Error(m_reader.GetError(), "Reader Error");
			}
		}

		m_reader.DeselectReader();
	}
	else
	{
		CString strError = m_reader.GetError();
		Prompter.Error(strError, "Hardware Error");
	}

	SetReaderMode(FALSE);						// endbale dialog input

	if (m_strMiFareNo != "")
	{
		OnOK();										// exit - as have UID
	}
}

//$$******************************************************************

void CCardNoDlg::OnOK()
{
	m_strInputCardNo = GetEditBoxText(*GetEditCardNo());
	m_nRecordCount = GetEditBoxInt(m_editCount);

	if ((m_nRecordCount < 1) || (m_nRecordCount > 999))
	{
		Prompter.Error("Please enter a number between 1 and 999");
		m_editCount.SetFocus();
		m_editCount.SetSel(0, -1);
		return;
	}

	if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)		// UserID = Mifare number
	{
		if (m_strMiFareNo != "")
		{
			if (m_strMiFareNo <= System.GetHighCardNo())		// see if mifare card read
			{
				m_strInputCardNo = m_strMiFareNo;				// MiFare number is UserID
			}

			m_strMiFareNo = "";									// clear MiFare number 
		}

		if (ValidateUserInput() == TRUE)
		{
			EndDialog(IDOK);
		}
	}
	else														// MIFare number is ExtRef / ExtRef2 fields
	{
		if (m_strInputCardNo != "")							// see if have user input 
		{
			if (ValidateUserInput() == TRUE)					// yes - validate
			{
				EndDialog(IDOK);
			}
		}
		else if (m_strMiFareNo != "")							// have reader input
		{
			int nMiFareDbNo = 1;								// assume default MiFare data format using Ext Ref

			if (m_terminal.m_nDefaultFormat == 1)				// USer usng Ext Ref2 / Dataformat 2
			{
				nMiFareDbNo = 2;
			}

			m_strOutputCardNo = System.TranslateMiFareNo(m_strMiFareNo, nMiFareDbNo);	// translate MifareNo to UserID
			EndDialog(IDOK);
		}
	}
}

//$$******************************************************************

void CCardNoDlg::OnCancel()
{
	if ( TRUE == m_bReaderMode)
	{
		m_reader.Terminate();
	}
	else
	{
		CSSAutoShutdownDialog::OnCancel();
	}
}

//$$******************************************************************

