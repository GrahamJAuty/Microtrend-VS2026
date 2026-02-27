//*********************************************************************
#include "SystemData.h"
#include "GlobalFunctions.h"
#include "PaylinkInterface.h"
#include "CommandFile.h"
#include "ImageHandler.h"
#include "KeypadDlg.h"
#include "WaitStatusDlg.h"
//*********************************************************************
#include "GetUserDlg.h"
//*********************************************************************

CGetUserDlg::CGetUserDlg(int nGetUserType, bool bIDReaderOffline, bool bHaveInputFile, CWnd* pParent)
	: CSSColourDialog( ((nGETUSER_BIO_ONLY == nGetUserType) ||(nGETUSER_MIFARE_ONLY == nGetUserType)) ? IDD_GETUSER2DLG : IDD_GETUSERDLG, pParent)
{
	m_nGetUserType = nGetUserType;
	m_bIDReaderOffline = bIDReaderOffline;
	m_bHaveInputFile = bHaveInputFile;

	switch (m_nGetUserType)
	{
	case nGETUSER_BIO_PINPAD:
	case nGETUSER_MIFARE_PINPAD:
	case nGETUSER_PINPAD_ONLY:
		m_bShowManual = TRUE;
		break;

	default:
		m_bShowManual = FALSE;
		break;
	}

	// force an in-service check
	m_ServiceTimes.Read();
	m_bKioskInService = m_ServiceTimes.InService();
	m_bKioskLastInService = !m_bKioskInService;					
}

//*********************************************************************

CGetUserDlg::~CGetUserDlg()
{
	switch (m_nGetUserType)
	{
	case nGETUSER_MIFARE_ONLY:
	case nGETUSER_MIFARE_PINPAD:
		m_reader.DeselectReader();
		break;
	}
}

//*********************************************************************

void CGetUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*********************************************************************

BEGIN_MESSAGE_MAP(CGetUserDlg, CSSColourDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, OnButtonManual)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
END_MESSAGE_MAP()

//*********************************************************************

BOOL CGetUserDlg::OnInitDialog()
{
	CString strFilename = "";
	bool bRounded = FALSE;

	{
		CString strLabel = m_bShowManual ? "GetUser" : "GetUser2";
		CImageHandler image(System.m_strSysFilesImagePath, strLabel);

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}

		bRounded = (strFilename == "");
	}

	CSSColourDialog::OnInitDialog(BLACK, bRounded);
	CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);

	m_staticPromptInfo.Init(IDC_STATIC_PROMPT, this, WHITE, -1, "");
	
	if (TRUE == m_bShowManual)
	{
		m_staticPrompt2.Init(IDC_STATIC_PROMPT2, this, WHITE, -1, "");
	}
	
	m_staticDateTime.Init(IDC_STATIC_DATETIME, this, WHITE, -1, "");

	{
		CImageHandler image(System.m_strSysFilesImagePath, "MainButton");

		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();		// get first filename name
		}
		else
		{
			strFilename = "";
		}
	}

	if (TRUE == m_bShowManual)	
	{
		m_buttonManual.Init(IDC_BUTTON_MANUAL, this, BLACK, MYCOLOUR_BUTTON, "", 20);
		m_buttonManual.SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	m_buttonCancel.Init(IDC_BUTTON_CANCEL, this, BLACK, MYCOLOUR_BUTTON, "", 20);

	if ((TRUE == m_bTimeoutCancel) && (TRUE == System.GetShowUserIDCancelFlag()))
	{
		m_buttonCancel.SetImage(strFilename, TRANSPARENT_COLOUR);
	}
	else
	{
		m_buttonCancel.ShowWindow(SW_HIDE);
	}

	switch (m_nGetUserType)
	{
	case nGETUSER_MIFARE_ONLY:
	case nGETUSER_MIFARE_PINPAD:
		m_terminal.Read();

		if (m_reader.SelectReader(m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud, TRUE) == TRUE)
		{
			// 0 = UID format 1(default), 1 = return UID format 2
			m_reader.SetUIDFormat(m_terminal.m_nUIDFormat);
			m_reader.SetDataFormat(m_terminal.m_nDataFormat);
			m_reader.SetCardReferenceText(System.m_strCardNoText);
		}
		else
		{
			m_bIDReaderOffline = TRUE;
		}
		break;
	}

	LONGLONG nCurrentTime = GetTickCount64();
	m_nLastMessageTime = nCurrentTime;
	m_nLastServerTime = nCurrentTime;
	m_nLastCancelTime = nCurrentTime;
	m_nLastFlushTime = nCurrentTime;

	m_nServerInterval = m_nServerIntervalStandard;

	DisplayPrompt();
	SetButtonColours();
	ShowDateTime();
	StartTimer();

	return TRUE;
}

//*********************************************************************

void CGetUserDlg::StartTimer()
{
	m_hTimer = SetTimer( ID_TIMER_GETUSER, 500, NULL );
}

//*********************************************************************

void CGetUserDlg::StopTimer()
{
	if (m_hTimer != 0)
	{
		KillTimer(m_hTimer);
	}

	m_hTimer = 0;
}

//*********************************************************************

void CGetUserDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		if (FALSE == m_bInTimer)
		{
			m_bInTimer = TRUE;
			SetButtonColours();

			HandleTimer();
			m_bDoneTimer = TRUE;
			ResetEventIfFired(m_nLastMessageTime);
			ResetEventIfFired(m_nLastServerTime);
			ResetEventIfFired(m_nLastFlushTime);
			ResetEventIfFired(m_nLastCancelTime);

			m_bInTimer = FALSE;
			SetButtonColours();
			DisplayPrompt();
		}
	}
}

//*********************************************************************

void CGetUserDlg::HandleTimer()
{
	switch(m_nGetUserType)
	{
	case nGETUSER_MIFARE_ONLY:
	case nGETUSER_MIFARE_PINPAD:
		if (FALSE == m_bIDReaderOffline)
		{
			// reader library will set prompt info text directly
			// reset current prompt text so we can update it afterwards
			m_reader.m_pStaticPrompt = &m_staticPromptInfo;
			m_strCurrentPromptInfoText = "";

			// 0=no error, 1=error, -1 = terminate, 2-no card
			int nReply = m_reader.GetUID(FALSE);

			if (0 == nReply)
			{
				// add leading 0's to make length 10
				m_strMiFareNo = m_reader.GetCardNo();

				CSSFile file;
				if (file.Open(System.m_strUserIDFilename, "wb") == TRUE)
				{
					CString strLine = "";
					if (m_terminal.m_nLocateOn == 1)
					{
						strLine.Format("MF2=%s",
							(const char*)m_strMiFareNo);
					}
					else
					{
						strLine.Format("MF=%s",
							(const char*)m_strMiFareNo);
					}

					file.WriteLine(strLine);
					file.Close();
				}
			}
		}
		break;
	}

	// see if biometric input file appeared
	if (::FileExists(System.m_strUserIDFilename) == TRUE)
	{
		CSSColourDialog::OnOK();
		return;
	}

	// see if need to timeout Cancel 

	if (TRUE == m_bTimeoutCancel)
	{
		if (CheckEventDue(m_nLastCancelTime, m_nCancelInterval) == TRUE)
		{
			CSSColourDialog::OnCancel();
			return;
		}
	}

	// see if Server working
	m_bKioskPendingService = FALSE;
	
	if (IsKioskInService() == TRUE)
	{
		if ((CheckEventDue(m_nLastServerTime, m_nServerInterval) == TRUE) || (nSTATUS_UNKNOWN == m_nServerStatus))
		{
			m_nLastServerTime = 0;
			m_nServerStatus = CheckServerRunning();

			switch (m_nServerStatus)
			{
			case nSTATUS_ONLINE:
				m_nServerInterval = m_nServerIntervalStandard;			// 30 Seconds
				break;

			default:
				m_nServerInterval = m_nServerIntervalError;				// 10 seconds
				break;
			}
			
			DisplayPrompt();
		}

		if ( TRUE == m_bKioskPendingService )
		{
			System.SetBalanceOnlyFlag(m_bKioskPendingBalanceOnly);
			CStatus status(System.m_nTerminalNo);
		
			SetPromptInfoText("Updating Kiosk Server Status");
			CWaitStatusDlg waitDlg(status,
				nWAITSTATUS_ACTION_SETSTATUSINSERVICE,
				FALSE,
				this);
			waitDlg.DoModal();
			
			m_bKioskPendingService = FALSE;
			DisplayPrompt();
		}

		if (CheckEventDue(m_nLastMessageTime, m_nMessageInterval) == TRUE)
		{
			::PlayMessage(nMESSAGE_WAITING);
		}
	}

	// see if have command file 
	// may already be set by secret manual entry

	CCommandFile command;

	if (TRUE == m_bShowManual)
	{
		if ((command.Found() == TRUE) || (TRUE == m_bCommandFileFound))
		{
			m_bCommandFileFound = TRUE;
			CSSColourDialog::OnOK();
			return;
		}
	}
	else
	{
		m_bCommandFileFound = command.Found();
		if (TRUE == m_bCommandFileFound)
		{
			CSSColourDialog::OnOK();
			return;
		}
	}

	// see if biometrc database refresh time is up
	switch (m_nGetUserType)
	{
	case nGETUSER_BIO_ONLY:
	case nGETUSER_BIO_PINPAD:
		if (System.m_nRefreshInterval != 0)
		{
			// refresh biomaetric database - only 1 retry
			System.AutoRefreshBiometrics(1, this);
		}
		break;
	}

	// see if need to check Paylink StartLevels for missed coinage

	if (FALSE == m_bPaylinkFlushed)
	{
		if (CheckEventDue(m_nLastFlushTime, m_nFlushInterval) == TRUE)
		{
			bool bCheckFlush = FALSE;	
			switch (m_nServerStatus)
			{
			case nSTATUS_ONLINE:
				bCheckFlush = TRUE;
				break;

			default:
				switch (m_nGetUserType)
				{
				case nGETUSER_BIO_ONLY:
				case nGETUSER_BIO_PINPAD:
					bCheckFlush = m_bIDReaderOffline;
					break;
				}
				break;
			}

			if (TRUE == bCheckFlush)
			{
				CPaylinkInterface paylink;
				if (paylink.Enable(this) == TRUE)			// may flush out any pending values
				{
					if (paylink.IsStartupOK() == TRUE)
					{
						m_bPaylinkFlushed = TRUE;		// flush out any pending values
					}

					WaitLoop(250);
					paylink.Disable();
				}
			}
		}
	}

	// Update Date & Time

	ShowDateTime();
}

//*********************************************************************

int CGetUserDlg::CheckServerRunning()
{
	if (TRUE == m_bIDReaderOffline)
	{
		switch (m_nGetUserType)
		{	
		case nGETUSER_BIO_ONLY:
			if (System.RefreshBiometrics(1) == TRUE)
			{
				m_bIDReaderOffline = FALSE;
			}
			else
			{
				return m_nServerStatus;
			}
			break;

		case nGETUSER_BIO_PINPAD:
			if (System.RefreshBiometrics(1) == TRUE)
			{
				m_bIDReaderOffline = FALSE;
			}
			break;

		case nGETUSER_MIFARE_ONLY:
			return m_nServerStatus;

		default:
			break;
		}
	}
	
	CStatus status(System.m_nTerminalNo, System.m_strUserIDFilename);
	SetPromptInfoText("Checking Kiosk Server Status");

	int nAction = nWAITSTATUS_ACTION_CHECKSERVERONLINE;

	if (TRUE == m_bKioskPendingService)
	{
		nAction = nWAITSTATUS_ACTION_SETSTATUSINSERVICE_CHECKSERVERONLINE;
		System.SetBalanceOnlyFlag(m_bKioskPendingBalanceOnly);
		m_bKioskPendingService = FALSE;
	}

	CWaitStatusDlg waitDlg(status, nAction, FALSE, this);
	waitDlg.DoModal();

	return waitDlg.GetIntResult();
}

//*********************************************************************
// see if kiosk is In Service

bool CGetUserDlg::IsKioskInService()
{
	m_bKioskInService = m_ServiceTimes.InService();

	if ( m_bKioskInService != m_bKioskLastInService )			// see if service change since last call
	{
		m_bKioskLastInService = m_bKioskInService;
		
		if (FALSE == m_bKioskInService)
		{
			System.SetBalanceOnlyFlag(m_ServiceTimes.IsBalanceOnly());	// set system flag

			CStatus status(System.m_nTerminalNo);

			SetPromptInfoText("Checking Kiosk Server Status");

			CWaitStatusDlg waitDlg(status,
				nWAITSTATUS_ACTION_SETSTATUSOUTSERVICE,
				FALSE,
				this);

			waitDlg.DoModal();
		}
		else
		{
			m_bKioskPendingService = TRUE;
			m_bKioskPendingBalanceOnly = m_ServiceTimes.IsBalanceOnly();
		}

		DisplayPrompt();
	}

	return m_bKioskInService;
}

//*********************************************************************

void CGetUserDlg::OnButtonManual()
{
	if ((FALSE == m_bInTimer) && (TRUE == m_bShowManual))
	{
		m_bInTimer = TRUE;
		bool bShownKeypad = FALSE;
		HandleManualInternal(bShownKeypad);

		if (TRUE == bShownKeypad)
		{
			ResetEvent(m_nLastMessageTime);
			ResetEvent(m_nLastServerTime);
			ResetEvent(m_nLastFlushTime);
			ResetEvent(m_nLastCancelTime);
		}

		m_bInTimer = FALSE;
	}
}

//*********************************************************************

void CGetUserDlg::HandleManualInternal(bool& bShownKeypad)
{
	bShownKeypad = FALSE;

	bool bCanShowKeypad = FALSE;
	
	switch (m_nGetUserType)
	{
	case nGETUSER_MIFARE_PINPAD:
	case nGETUSER_PINPAD_ONLY:
	case nGETUSER_BIO_PINPAD:
		bCanShowKeypad = ( (nSTATUS_ONLINE == m_nServerStatus) && (TRUE == m_bKioskInService));
		break;

	default:
		break;
	}

	if ( TRUE == bCanShowKeypad)
	{
		CKeypadDlg dlg(FALSE,this);

		if (System.GetActualPINModeFlag() == TRUE)
		{
			dlg.m_strPrompt = "Enter Account PIN";
		}
		else
		{
			dlg.m_strPrompt = "Enter Account Number";
		}

		if (dlg.DoModal() == IDOK)
		{

#ifdef _DEBUG
			CString strReverseText = "1cc";				// "**1"
#else
			CString strReverseText = "7591c01c70cc"; 	// "**07*10*1957"
#endif

			if (dlg.m_strSecretText.MakeReverse() == strReverseText)
			{
				m_bCommandFileFound = TRUE;								// set command file found
			}
			else
			{
				CSSFile file;
				if (file.Open(System.m_strUserIDFilename, "wb") == TRUE)
				{
					CString strPrefix = "";
					if (System.GetActualPINModeFlag() == TRUE)
					{
						strPrefix = "PIN=";
					}

					m_bManualInput = TRUE;
					file.WriteLine(strPrefix + dlg.m_strInput);
					file.Close();
				}
			}
		}

		bShownKeypad = TRUE;
	}
}

//*********************************************************************

void CGetUserDlg::DisplayPrompt()
{
	CString strReaderPrompt = "";
	CString strKioskPrompt = "";

	if (TRUE == m_bIDReaderOffline)
	{
		switch (m_nGetUserType)
		{
		case nGETUSER_BIO_ONLY:
		case nGETUSER_BIO_PINPAD:
			strReaderPrompt = "Biometric reader is offline";
			break;

		case nGETUSER_MIFARE_ONLY:
		case nGETUSER_MIFARE_PINPAD:
			strReaderPrompt = "Card reader is offline";
			break;
		}
	}

	if (FALSE == m_bKioskInService)
	{
		strKioskPrompt = "Kiosk is out of service";
	}
	else if (TRUE == m_bHaveInputFile)
	{
		strKioskPrompt = "Please wait";
	}
	else
	{
		switch (m_nServerStatus)
		{
		case nSTATUS_ONLINE:
			if (strReaderPrompt == "")
			{
				switch (m_nGetUserType)
				{
				case nGETUSER_BIO_ONLY:
				case nGETUSER_BIO_PINPAD:
					strKioskPrompt = "Place finger or thumb on reader";
					break;

				case nGETUSER_MIFARE_ONLY:
				case nGETUSER_MIFARE_PINPAD:
					strKioskPrompt.Format("Present %s to reader",
						(const char*)System.m_strCardNoText);
					break;
				}
			}
			break;

		case nSTATUS_OFFLINE:
			strKioskPrompt = "Kiosk Server is offline";
			break;

		case nSTATUS_DISABLED:
			strKioskPrompt = "Kiosk Server suspended";
			break;

		case nSTATUS_UNKNOWN:
			strKioskPrompt = "Checking Kiosk Server Status";
			break;
		}
	}

	if (strReaderPrompt == "")
	{
		SetPromptInfoText(strKioskPrompt);
	}
	else if (strKioskPrompt == "")
	{
		SetPromptInfoText(strReaderPrompt);
	}
	else
	{
		LONGLONG nCurrentTime = GetTickCount64();

		if ( nCurrentTime >= m_nLastSwapMessageTime + 1000 )
		{
			m_nLastSwapMessageTime = nCurrentTime;
			m_bAlternatePromptKiosk = !m_bAlternatePromptKiosk;
		}

		if (TRUE == m_bAlternatePromptKiosk)
		{
			SetPromptInfoText(strKioskPrompt);
		}
		else
		{
			SetPromptInfoText(strReaderPrompt);
		}
	}
}

//*********************************************************************

void CGetUserDlg::SetPromptInfoText(const CString& strText)
{
	if (strText != m_strCurrentPromptInfoText)
	{
		m_strCurrentPromptInfoText = strText;
		m_staticPromptInfo.SetWindowText(strText);
	}
}

//*********************************************************************

void CGetUserDlg::ShowDateTime()
{
	if (TRUE == System.GetShowDateTimeFlag())
	{
		CString strDate = CSSDate::GetCurrentDate().GetDate();
		CString strTime = CSSTime::GetCurrentTime().GetTime();

		m_strCurrentDateTimeText.Format ( "%s  %s", 
			(const char*) strTime, 
			(const char*) strDate );

		if ( m_strCurrentDateTimeText != m_strStaticDateTimeText )
		{
			m_strStaticDateTimeText = m_strCurrentDateTimeText;
			m_staticDateTime.SetWindowText(m_strStaticDateTimeText);
		}
	}
}

//*********************************************************************

void CGetUserDlg::SetButtonColours()
{
	COLORREF manualColour = GREY;
	COLORREF cancelColour = GREY;

	if ((FALSE == m_bInTimer) && (TRUE == m_bDoneTimer))
	{
		bool bCanShowKeypad = FALSE;

		switch (m_nGetUserType)
		{
		case nGETUSER_MIFARE_PINPAD:
		case nGETUSER_PINPAD_ONLY:
		case nGETUSER_BIO_PINPAD:
			bCanShowKeypad = ((nSTATUS_ONLINE == m_nServerStatus) && (TRUE == m_bKioskInService));
			break;

		default:
			break;
		}

		if (TRUE == bCanShowKeypad)
		{
			manualColour = MYCOLOUR_BUTTON;
		}
		
		cancelColour = MYCOLOUR_BUTTON;
	}

	m_buttonCancel.SetBackgroundColour(cancelColour);
	m_buttonCancel.Invalidate();

	if (TRUE == m_bShowManual)
	{
		m_buttonManual.SetBackgroundColour(manualColour);
		m_buttonManual.Invalidate();
	}
}

//*********************************************************************

void CGetUserDlg::OnButtonCancel()
{
	StopTimer();
	CSSColourDialog::OnCancel();
}

//*********************************************************************
