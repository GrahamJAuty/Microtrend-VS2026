/**********************************************************************/
#include <errno.h>
#include <process.h>
/**********************************************************************/
#include "PCOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "repspawn.h"
/**********************************************************************/

CReportManager::CReportManager()
{
	m_bShownWarning = FALSE;
}

/**********************************************************************/

bool CReportManager::DisplayReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd )
{
	return ShowReport( szReportFilename, szReportParamsFilename, szReportFieldsKey, FALSE, TRUE, pParentWnd );
}

/**********************************************************************/

bool CReportManager::DisplaySilent ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd )
{
	return ShowReport( szReportFilename, szReportParamsFilename, szReportFieldsKey, FALSE, FALSE, pParentWnd );
}

/**********************************************************************/

bool CReportManager::PrintReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd )
{
	return ShowReport( szReportFilename, szReportParamsFilename, szReportFieldsKey, TRUE, TRUE, pParentWnd );
}

/**********************************************************************/

bool CReportManager::PrintSilent ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd )
{
	return ShowReport( szReportFilename, szReportParamsFilename, szReportFieldsKey, TRUE, FALSE, pParentWnd );
}

/**********************************************************************/

bool CReportManager::ShowReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, bool bPrint, bool bErrors, CWnd* pParentWnd )
{
	int nAction = REPMAN_ACTION_DISPLAY;

	if ( TRUE == bPrint )
	{
		switch( DealerFlags.GetPrinterRedirectMode() )
		{
		case 1:	
			nAction = REPMAN_ACTION_REDIRECT1;	
			bErrors = FALSE;
			break;

		case 2:	
			nAction = REPMAN_ACTION_REDIRECT2;	
			bErrors = FALSE;
			break;

		case 0:
		default:
			nAction = REPMAN_ACTION_PRINT;
			break;
		}
	}

	CString strReportFilename = szReportFilename;
	CString strReportParamsFilename = szReportParamsFilename;
	CString strReportFieldsKey = szReportFieldsKey;
	
	if ( ::FileExists ( "REPMAN.PGM" ) == FALSE )
	{
		if ( ( bErrors == TRUE ) && ( m_bShownWarning == FALSE ) )
		{
			ReportManagerError();
			m_bShownWarning = TRUE;
		}

		return FALSE;
	}

	CString strRepmanPath = "SysFiles/";
	GetDataProgramPath( strRepmanPath );

	CString strReportPrinter = "";

#ifdef POSTRAY_UTILITY
	strReportPrinter = PCOptionsHost.GetDefaultReportPrinter();
#else
	strReportPrinter = PCOptionsClient.GetDefaultReportPrinter();
#endif

	int nResult = 0;

	CWnd* pWnd = pParentWnd;
	if (NULL == pWnd)
	{
		pWnd = AfxGetMainWnd();
	}
	else
	{
		pWnd = pWnd->GetSafeOwner(pWnd);
	}

	if ( NULL == pWnd )
	{
		if ( ( bErrors == TRUE ) && ( m_bShownWarning == FALSE ) )
		{
			Prompter.Error( "Unable to create report.\n\nBackground window could not be identified." );
			m_bShownWarning = TRUE;
		}

		return FALSE;
	}

	CString strDevmodePath = "PRINTB_";
	GetPCPathClient( strDevmodePath );

	switch (nAction)
	{
	case REPMAN_ACTION_PRINT:
		nResult = SSspawnl(pWnd, P_WAIT, "REPMAN.PGM", "REPMAN.PGM", strReportFilename, strReportParamsFilename, strReportFieldsKey, strRepmanPath, "2", "", "", "", strReportPrinter, strDevmodePath, "/p", NULL);
		break;

	case REPMAN_ACTION_REDIRECT2:
		nResult = SSspawnl(pWnd, P_WAIT, "REPMAN.PGM", "REPMAN.PGM", strReportFilename, strReportParamsFilename, strReportFieldsKey, strRepmanPath, "2", "", "", "", strReportPrinter, strDevmodePath, NULL);
		break;

	case REPMAN_ACTION_DISPLAY:
	case REPMAN_ACTION_REDIRECT1:
	default:
	{
		CString strDataPath = "";
		GetDataProgramPath(strDataPath);

		if (strDataPath != "")
		{
			strDataPath = strDataPath.Left(strDataPath.GetLength() - 1);
		}

		CString strEmailFileNumber1 = "124";
		CString strEmailFileNumber2 = "82";

		if ( (SysInfo.GetMaxBasePluLen() == 0) && ( SysInfo.GetAltFilenamesIfNoModifiersFlag() == TRUE ) )
		{
			strEmailFileNumber1 = "80124";
			strEmailFileNumber2 = "80082";
		}

		if (DealerFlags.GetSpawnRepmanFlag() == TRUE)
		{
			nResult = SSspawnl(pWnd, P_NOWAIT, "REPMAN.PGM", "REPMAN.PGM", strReportFilename, strReportParamsFilename, strReportFieldsKey, strRepmanPath, "2", strDataPath, strEmailFileNumber1, strEmailFileNumber2, strReportPrinter, strDevmodePath, NULL);
		}
		else
		{
			CCommandLineBuilder CommandLineBuilder;
			CommandLineBuilder.AddToken(".\\REPMAN.PGM");
			CommandLineBuilder.AddToken(strReportFilename);
			CommandLineBuilder.AddToken(strReportParamsFilename);
			CommandLineBuilder.AddToken(strReportFieldsKey);
			CommandLineBuilder.AddToken(strRepmanPath);
			CommandLineBuilder.AddToken("2");
			CommandLineBuilder.AddToken(strDataPath);
			CommandLineBuilder.AddToken(strEmailFileNumber1);
			CommandLineBuilder.AddToken(strEmailFileNumber2);
			CommandLineBuilder.AddToken(strReportPrinter);
			CommandLineBuilder.AddToken(strDevmodePath);
			CString strCommandLine = CommandLineBuilder.GetLine();

			LPTSTR cmd = strCommandLine.GetBuffer(strCommandLine.GetLength() + 1);

			STARTUPINFO startup;
			PROCESS_INFORMATION procinfo;

			::ZeroMemory(&startup, sizeof(startup));
			::ZeroMemory(&procinfo, sizeof(procinfo));

			if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &procinfo) == TRUE)
			{
				CloseHandle(procinfo.hProcess);
				CloseHandle(procinfo.hThread);
				nResult = 0;
			}
			else
			{
				nResult = -1;
				errno = ENOENT;
			}
		}
	}
	break;
	}

	if ( ( nResult == -1 ) && ( errno == ENOENT ) )
	{
		if ( ( bErrors == TRUE ) && ( m_bShownWarning == FALSE ) )
		{
			ReportManagerError();
			m_bShownWarning = TRUE;
		}
	
		return FALSE;
	}

	if (REPMAN_ACTION_REDIRECT1 == nAction)
	{
		Prompter.Info("Print Request Redirected To Display");
	}
		
	return TRUE;
}

/**********************************************************************/

void CReportManager::ReportManagerError()
{
	Prompter.ContactDealer ( "You must install Report Manager before you can view or print reports." );
}

/**********************************************************************/
		
bool CReportManager::DisplayGraph( const char* szGraphFilename )
{		
	if ( ::FileExists ( "GRAPHMAN.PGM" ) == FALSE )
	{
		if ( m_bShownWarning == FALSE )
		{
			GraphManagerError();
			m_bShownWarning = TRUE;
		}

		return FALSE;
	}

	CString strGraphFilename = szGraphFilename;
	CString strGraphmanPath = "SysFiles\\";
	GetDataProgramPath( strGraphmanPath );

	CWnd* pWnd = AfxGetMainWnd();

	if ( NULL == pWnd )
	{
		if ( m_bShownWarning == FALSE )
		{
			Prompter.Error( "Unable to create graph.\n\nBackground window could not be identified." );
			m_bShownWarning = TRUE;
		}

		return FALSE;
	}
	
	int nResult = SSspawnl ( pWnd, P_NOWAIT, "GRAPHMAN.PGM", "GRAPHMAN.PGM", strGraphFilename, strGraphmanPath, NULL );
	
	if ( ( nResult == -1 ) && ( errno == ENOENT ) )
	{
		if ( m_bShownWarning == FALSE )
		{
			GraphManagerError();
			m_bShownWarning = TRUE;
		}
	
		return FALSE;
	}
	
	return TRUE;	
}

/**********************************************************************/

void CReportManager::GraphManagerError()
{
	Prompter.ContactDealer ( "You must install Graph Manager before you can display or print graphs." );
}

/**********************************************************************/

