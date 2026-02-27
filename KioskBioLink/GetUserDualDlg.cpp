//**********************************************************************
#include "SystemData.h"
#include "GlobalFunctions.h"
#include "ImageHandler.h"
#include "GetUserDualDlg.h"
//**********************************************************************
IMPLEMENT_DYNAMIC(CGetUserDualDlg, CDialog)
//**********************************************************************

CGetUserDualDlg::CGetUserDualDlg(CWnd* pParent)
	: CSSColourDialog(CGetUserDualDlg::IDD, pParent)
{
}

//**********************************************************************

CGetUserDualDlg::~CGetUserDualDlg()
{
}

//**********************************************************************

void CGetUserDualDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*********************************************************************

BEGIN_MESSAGE_MAP(CGetUserDualDlg, CSSColourDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BIOMETRIC, OnButtonFinger)
	ON_BN_CLICKED(IDC_BUTTON_READCARD, OnButtonReadcard)
END_MESSAGE_MAP()

//*********************************************************************
// CGetUserDualDlg message handlers

BOOL CGetUserDualDlg::OnInitDialog()
{
	CString strFilename = "";

	{
		CImageHandler image(System.m_strSysFilesImagePath, "DualScreen");
		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}

		bool bRounded = (strFilename == "") ? TRUE : FALSE;

		CSSColourDialog::OnInitDialog(BLACK, bRounded);
		CSSColourDialog::SetImage(strFilename, TRANSPARENT_COLOUR);
	}

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, WHITE, -1, "");
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

		m_buttonBiometric.Init(IDC_BUTTON_BIOMETRIC, this, BLACK, MYCOLOUR_BUTTON, "", 20);
		m_buttonBiometric.SetImage(strFilename, TRANSPARENT_COLOUR);

		m_buttonReadCard.Init(IDC_BUTTON_READCARD, this, BLACK, MYCOLOUR_BUTTON, "", 20);
		m_buttonReadCard.SetImage(strFilename, TRANSPARENT_COLOUR);

		CString strText = System.m_strCardNoText.Left(1);				// text = "card" or "fob"
		strText.MakeUpper();											// becomes
		strText += System.m_strCardNoText.Mid(1);						// "Card" or "Fob"

		m_buttonReadCard.SetWindowText(strText);
	}

	LONGLONG nCurrentTime = GetTickCount64();
	m_nLastMessageTime = nCurrentTime;

	ShowDateTime();
	StartTimer();
	return TRUE;
}

//*********************************************************************

void CGetUserDualDlg::OnButtonFinger()
{
	StopTimer();
	m_nInterfaceReqd = nINTERFACE_BIOMETRIC;
	CDialog::OnOK();
}

//**********************************************************************

void CGetUserDualDlg::OnButtonReadcard()
{
	StopTimer();
	m_nInterfaceReqd = nINTERFACE_MIFAREv1;
	CDialog::OnOK();
}

//**********************************************************************

void CGetUserDualDlg::StartTimer()
{
	m_hTimer = SetTimer( ID_TIMER_GETUSERDUAL, 500, NULL );
}

//**********************************************************************

void CGetUserDualDlg::StopTimer()
{
	if (m_hTimer != 0)
	{
		KillTimer(m_hTimer);
	}

	m_hTimer = 0;
}

//*********************************************************************

void CGetUserDualDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		if (FALSE == m_bInTimer)
		{
			m_bInTimer = TRUE;
			HandleTimer();

			ResetEventIfFired(m_nLastMessageTime);

			m_bInTimer = FALSE;
		}
	}
}

//*********************************************************************

void CGetUserDualDlg::HandleTimer()
{
	// see if have input file appearing
	if (::FileExists(System.m_strUserIDFilename) == TRUE)
	{
		m_bHaveInputFile = TRUE;
		OnButtonFinger();
		return;
	}

	// see if have command file 
	if (CheckEventDue(m_nLastMessageTime, m_nMessageInterval) == TRUE)
	{
		::PlayMessage(nMESSAGE_WAITING);
	}

	ShowDateTime();
}

//**********************************************************************

void CGetUserDualDlg::ShowDateTime()
{
	if (System.GetShowDateTimeFlag() == TRUE)
	{
		CString strDate = CSSDate::GetCurrentDate().GetDate();
		CString strTime = CSSTime::GetCurrentTime().GetTime();

		m_strCurrentDateTimeText.Format("%s  %s",
			(const char*)strTime,
			(const char*)strDate);

		if (m_strCurrentDateTimeText != m_strStaticDateTimeText)
		{
			m_strStaticDateTimeText = m_strCurrentDateTimeText;
			m_staticDateTime.SetWindowText(m_strStaticDateTimeText);
		}
	}
}

//**********************************************************************
