//**********************************************************************
#include "PCOptions.h"
//**********************************************************************
#include "RepmanHandler.h"
//**********************************************************************

extern CSysset Sysset;

//**********************************************************************

CRepmanHandler::CRepmanHandler(const char* szTitle, CWnd* pParent)
	: m_strTitle(szTitle)
	, m_pParent(pParent)
{
	m_nMode = P_NOWAIT;
	m_nSaveAsType = nSAVEAS_NONE;
	m_strSaveAsFilename = "";
}

//**********************************************************************

int CRepmanHandler::DisplayReport ( const char* szReport, const char* szParams, const char* szParamsKey, bool bPrintReqd )
{
	CString strProgName = "repman.pgm";
	if (::FileExists(strProgName) == FALSE)
	{
		return nREPORT_NOREPMAN;
	}

	if (::FileExists(szReport) == FALSE)
	{
		return nREPORT_NOSOURCE;
	}

	CString strParamsKey = szParamsKey;
	if (strParamsKey == "")
	{
		strParamsKey = "\"\"";							// no parameter key passed so pass blank to repman
	}

	CString strUserPath = Sysset.GetPCPathClient();
	strUserPath += "/";									// needs trailing slash

	SaveReportAs ( szReport );							// as any additional report files

	CString strDataPath = "";
	GetDataProgramPath( strDataPath );
			
	if (strDataPath != "")
	{
		strDataPath = strDataPath.Left(strDataPath.GetLength() - 1);
	}

	CString strDevmodePath = "PRINTB_";
	GetPCPathClient( strDevmodePath );

	if (bPrintReqd == TRUE)
	{
		SSspawnl(m_pParent, P_WAIT, strProgName, strProgName, szReport, szParams, szParamsKey, strUserPath, "2", "", "", "", PCOptionsClient.GetDefaultReportPrinter(), strDevmodePath, "/p", NULL);
	}
	else if (P_WAIT == m_nMode)
	{
		SSspawnl(m_pParent, m_nMode, strProgName, strProgName, szReport, szParams, szParamsKey, strUserPath, "2", strDataPath, "26", "27", PCOptionsClient.GetDefaultReportPrinter(), strDevmodePath, NULL);
	}
	else
	{
		CCommandLineBuilder CommandLineBuilder;
		CommandLineBuilder.AddToken( ".\\REPMAN.PGM" );
		CommandLineBuilder.AddToken( szReport );
		CommandLineBuilder.AddToken( szParams );
		CommandLineBuilder.AddToken( strParamsKey );
		CommandLineBuilder.AddToken( strUserPath );
		CommandLineBuilder.AddToken( "2" );
		CommandLineBuilder.AddToken( strDataPath );
		CommandLineBuilder.AddToken( "26" );
		CommandLineBuilder.AddToken( "27" );
		CommandLineBuilder.AddToken( PCOptionsClient.GetDefaultReportPrinter() );
		CommandLineBuilder.AddToken( strDevmodePath );
		CString strCommandLine = CommandLineBuilder.GetLine();
			
		LPTSTR cmd = strCommandLine.GetBuffer(strCommandLine.GetLength() + 1);
 
		STARTUPINFO startup;
		PROCESS_INFORMATION procinfo;
	
		::ZeroMemory(&startup, sizeof(startup));
		::ZeroMemory(&procinfo, sizeof(procinfo));

		if ( CreateProcess( NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &procinfo ) == TRUE )
		{
			CloseHandle(procinfo.hProcess);
			CloseHandle(procinfo.hThread);
		}
		else
		{
			errno = ENOENT;
		}
	}

	return nREPORT_NOERROR;
}

//********************************************************************
// szReport = datafile or "SETUP"

int CRepmanHandler::ShowBarcode(const char* szReport, const char* szLabel, bool bPrint)
{
	CString strProgName = "sposbarcodemanager.pgm";
	if (::FileExists(strProgName) == FALSE)
	{
		return nREPORT_NOBARMAN;
	}

	CProtect1 protect;

	CString strProgramPath = Sysset.GetProgramPath();
	strProgramPath += "\\";									// needs trialing '\'

	CString strLabelPath = "";
	CString strFirstLabel = "1";
	CString strPrint = bPrint ? "p" : "d";

	CString strDevmodePath = "PRINTA_";
	GetPCPathClient(strDevmodePath);

	CString strReport = szReport;

	if (strReport != "SETUP")
	{
		SSspawnl(m_pParent, P_WAIT, strProgName, strProgName, "2", protect.GetCode(), strReport, szLabel, strFirstLabel, strProgramPath, strPrint, PCOptionsClient.GetDefaultBarcodePrinter(), strDevmodePath, NULL);
	}
	else
	{
		SSspawnl(m_pParent, P_WAIT, strProgName, strProgName, "102", protect.GetCode(), strProgramPath, PCOptionsClient.GetDefaultBarcodePrinter(), strDevmodePath, NULL);
	}

	return nREPORT_NOERROR;
}

//********************************************************************

void CRepmanHandler::DisplayMessage ( const char* szMsg, int nType )
{
	Prompter.DisplayMessage( szMsg, "", nType );
}

//********************************************************************
// Reply = is if none fatal error

bool CRepmanHandler::DisplayError(int nErrorNo)
{
	CString strMsg = "";
	switch (nErrorNo)
	{
	case nREPORT_NOERROR:
	case nREPORT_CANCEL:
	case nREPORT_EXIT:
		break;

	case nREPORT_NOSOURCE:
		strMsg = "No source report data found!";
		break;

	case nREPORT_INVALIDSOURCE:
		strMsg = "Invalid source data!";
		break;

	case nREPORT_CREATEFAIL:
		strMsg = "Unable to create output report file!";
		break;

	case nREPORT_INVALIDCARDRANGE:
		strMsg = "No accounts found in range!";
		break;

	case nREPORT_NOMATCH:
		strMsg = "No matching account data found!";
		break;

	case nREPORT_NOREPMAN:
		strMsg = "Report Manager has not been installed!";
		break;

	case nREPORT_NOBARMAN:
		strMsg = "Barcode Manager has not been installed!";
		break;

	case nREPORT_OPENERROR:
		strMsg = "Unable to open source report data!";
		break;

	case nREPORT_DBFILEERROR:
		strMsg = "Unable to access report generated database!";
		break;

	default:
		strMsg.Format("Unexpected error ( %d )", nErrorNo);
		break;
	}

	if (strMsg != "")
	{
		DisplayMessage(strMsg, MB_ICONEXCLAMATION);
	}

	bool bNonFatal = FALSE;
	switch (nErrorNo)
	{
		// allow retry of report
	case nREPORT_NOERROR:
	case nREPORT_CREATEFAIL:
	case nREPORT_INVALIDCARDRANGE:
	case nREPORT_NOMATCH:
	case nREPORT_OPENERROR:
	case nREPORT_DBFILEERROR:
	case nREPORT_CANCEL:
		bNonFatal = TRUE;
		break;

		// no point retrying
	case nREPORT_NOSOURCE:
	case nREPORT_INVALIDSOURCE:
	case nREPORT_EXIT:
	default:
		bNonFatal = FALSE;
		break;
	}
	return bNonFatal;
}

//********************************************************************
//********************************************************************
// Convert report file to csv \ html

void CRepmanHandler::SetupSaveAs(int nSaveAsType, const char* szSaveAsFilename)
{
	m_nSaveAsType = nSaveAsType;
	m_strSaveAsFilename = szSaveAsFilename;
}

//********************************************************************

void CRepmanHandler::SaveReportAs(const char* szReport)
{
	CReportFileDoc report(szReport);

	switch (m_nSaveAsType)
	{
	case nSAVEAS_NONE:
		break;

	case nSAVEAS_CSV:
		report.SaveAsCSV(m_strSaveAsFilename);
		break;

	case nSAVEAS_HTML:
		report.SaveAsHTML(m_strSaveAsFilename);
		break;
	}
}

//**********************************************************************
