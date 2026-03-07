/**********************************************************************/
#include "GlobalFunctions.h"
#include "WebServerDlg.h"
#include "WinService.h"
/**********************************************************************/
#include "WebServer.h"
/**********************************************************************/
#define RETCODE_OK 0
#define RETCODE_BADPARAMCOUNT 1
#define RETCODE_REGISTRATIONFAIL 2
#define RETCODE_UNKNOWNACTION 3
#define RETCODE_WININITFAIL 4
#define RETCODE_BADDIRECTORY 5
#define RETCODE_UNREGISTERED 6
#define RETCODE_DBACCESS_FAIL 7
//*******************************************************************

BEGIN_MESSAGE_MAP(CWebServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/**********************************************************************/

CWebServerApp::CWebServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/**********************************************************************/

CWebServerApp theApp;

/**********************************************************************/
CWinService ServerService;
/**********************************************************************/

BOOL CWebServerApp::InitInstance()
{
	ServerService.Init("WebServer", "Microtrend Web Server", "Microtrend Web Server", SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);

	m_nRetCode = RETCODE_OK;

	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.GetSize() == 0)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		CString strCommand = cmdInfo.GetAt(0);
		strCommand.MakeUpper();

		if (strCommand == "<SERVICE>")
		{
			RunService(cmdInfo);
		}
		else if (strCommand.Left(10) == "DESKTOPAPP")
		{
			RunDesktop(cmdInfo);
		}
		else if (strCommand == "COMMAND")
		{
			RunCommand(cmdInfo);
		}
		else
		{
			m_nRetCode = RETCODE_UNKNOWNACTION;

			CString strIndent = "==> ";
			LogMessage("Unknown Web Service command");
			LogMessage(strIndent + strCommand);
		}
	}

	return FALSE;
}

//*******************************************************************

void CWebServerApp::RunService(CMyCommandLineInfo& cmdInfo)
{
	if (cmdInfo.GetSize() != 2)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		m_nRetCode = RETCODE_OK;
		ServerService.ServiceRun();
	}
}

//*******************************************************************

void CWebServerApp::RunDesktop(CMyCommandLineInfo& cmdInfo)
{
	if (cmdInfo.GetSize() != 1)
	{
		m_nRetCode = RETCODE_BADPARAMCOUNT;
	}
	else
	{
		if (!AfxSocketInit())
		{
			AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
			return;
		}

		CWebServerDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == IDCANCEL)
		{
		}
	}
}

//*******************************************************************

void CWebServerApp::RunCommand(CMyCommandLineInfo& cmdInfo)
{
	ServerService.ParseStandardArgs(cmdInfo);

	CSSFile fileResult;
	if (fileResult.Open(".\\WebSrvCmd.dat", "wb") == TRUE)
	{
		fileResult.WriteLine(ServerService.GetError());
	}
}

//*******************************************************************
