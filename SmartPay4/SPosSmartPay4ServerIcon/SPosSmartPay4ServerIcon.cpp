//$$******************************************************************
#include "pch.h"
//$$******************************************************************
#include "framework.h"
//$$******************************************************************
#include "MessageLogger.h"
#include "PCOptions.h"
#include "SPosSmartPay4ServerIcon.h"
#include "SPosSmartPay4ServerIconDlg.h"
//$$******************************************************************
#include "..\SmartPay4Shared\PresentationOptions.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
//$$******************************************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//$$******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//$$******************************************************************

BEGIN_MESSAGE_MAP(CSPosSmartPay4ServerIconApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

//$$******************************************************************

CSPosSmartPay4ServerIconApp::CSPosSmartPay4ServerIconApp() : CWinApp( "SmartPay Server Icon" )
{
	// support Restart Manager
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_AppStartTime = GetTickCount64();
	m_bFailIconChange = FALSE;
}

//$$******************************************************************
CSPosSmartPay4ServerIconApp theApp;
//$$******************************************************************
CFilenameHandler Filenames;
CMessageLogger MessageLogger;
CPresentationOptions PresentationOptions;
CPCOptionsClient PCOptionsClient;
CPrompter Prompter;
CSysset Sysset;
CServiceOptions ServerServiceOptions(FALSE);
CServiceOptions BgndServiceOptions(TRUE);
//$$******************************************************************

class CMyCommandLineInfo : public CCommandLineInfo
{
public:
	virtual void ParseParam(const char* szParam, BOOL bFlag, BOOL bLast);
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt(int n) { return m_arrayParam.GetAt(n); }

private:
	CStringArray m_arrayParam;
};

void CMyCommandLineInfo::ParseParam(const char* szParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag == FALSE)			// discard /p for printing etc
		m_arrayParam.Add(szParam);

	CCommandLineInfo::ParseParam(szParam, bFlag, bLast);
}

//$$******************************************************************

BOOL CSPosSmartPay4ServerIconApp::InitInstance()
{
	CWinApp::InitInstance();

	// Initialize Common Controls early to prevent WIL exceptions
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES     // Standard controls
		| ICC_TAB_CLASSES       // Tab controls
		| ICC_LISTVIEW_CLASSES  // List view controls
		| ICC_TREEVIEW_CLASSES  // Tree view controls
		| ICC_BAR_CLASSES       // Toolbar, status bar
		| ICC_PROGRESS_CLASS    // Progress bars
		| ICC_HOTKEY_CLASS      // Hot key controls
		| ICC_ANIMATE_CLASS     // Animation controls
		| ICC_USEREX_CLASSES;   // Extended controls

	if (!InitCommonControlsEx(&icex))
	{
		TRACE("Failed to initialize Common Controls\n");
		// Continue anyway - fallback to older initialization
		InitCommonControls();
	}

	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	int nRegRetryTime = 0;
	if (cmdInfo.GetSize() == 1)
	{
		nRegRetryTime = atoi(cmdInfo.GetAt(0));
		nRegRetryTime = min(nRegRetryTime, 300);
		nRegRetryTime = max(nRegRetryTime, 0);
	}

	bool bCreatePCId = FALSE;

	ULONGLONG timeGiveUp = GetTickCount64() + (1000 * nRegRetryTime);
	bool bRegistered = FALSE;

	do
	{
		bRegistered = Sysset.TestRegistrationModule(SYSSET_SMARTPAY4, NULL, bCreatePCId, TRUE, 0);

		if (FALSE == bRegistered)
		{
			Sleep(200);
		}
	} 
	while ((FALSE == bRegistered) && (GetTickCount64() < timeGiveUp));

	if (FALSE == bRegistered)
	{
		CString strText = CSysset::DecodeMessage(",FK9XK7r-iC9rC7x-`S7X-ICz9SK7X`22FnKS"); //"Please contact your software supplier"

		CString strMess = "";
		CString strError = Sysset.GetError();
		CString strField = Sysset.GetField(4);
		strMess.Format("%s\n\n%s %s", (const char*)strError, (const char*)strText, (const char*)strField);

		Prompter.FatalError(strMess);
		return FALSE;
	}

	ServerServiceOptions.Read();
	BgndServiceOptions.Read();

	CSPosSmartPay4ServerIconDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_bGotPCId = bCreatePCId;

	if (TRUE == bCreatePCId)
	{
		dlg.m_strRunOnceTimeStamp = ::CreateRunOnceTimeStamp();
	}

	INT_PTR nResponse = dlg.DoModal();

	return TRUE;
}

//$$******************************************************************
