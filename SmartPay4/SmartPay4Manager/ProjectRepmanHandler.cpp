//**********************************************************************
#include "PCOptions.h"
//**********************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//**********************************************************************

CRepmanHandler::CRepmanHandler ( const char* szTitle, CWnd* pParent )
{
	m_strTitle = szTitle;
	m_pParent = pParent;
	m_nMode = P_NOWAIT;
}

//**********************************************************************

void CRepmanHandler::SetParentWindow(CWnd* pParent)
{
	m_pParent = pParent;
}

//**********************************************************************

int CRepmanHandler::DisplayReport(const char* szReport, const char* szParams, const char* szParamsKey, CWnd* pReportParentWnd)
{
	if (NULL == pReportParentWnd)
	{
		pReportParentWnd = m_pParent;
	}

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
	strUserPath += "/";									// forward slash to avoid escaping other params

	CString strDataPath = "";
	GetDataProgramPath( strDataPath );
			
	if (strDataPath != "")
	{
		strDataPath = strDataPath.Left(strDataPath.GetLength() - 1);
	}

	CString strDevmodePath = "PRINTB_";
	GetPCPathClient( strDevmodePath );

	SaveReportAs ( szReport );							// as any additional report files

	if (P_WAIT == m_nMode)
	{
		SSspawnl(pReportParentWnd, m_nMode, strProgName, strProgName, szReport, szParams, szParamsKey, strUserPath, "2", strDataPath, "59", "60", PCOptionsClient.GetDefaultReportPrinter(), strDevmodePath, NULL);
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
		CommandLineBuilder.AddToken( "59" );
		CommandLineBuilder.AddToken( "60" );
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

int CRepmanHandler::ShowBarcode ( const char* szReport, const char* szLabel, bool bPrint )
{
	CString strProgName = "barman.pgm";
	if ( ::FileExists ( strProgName ) == FALSE )
		return nREPORT_NOBARMAN;

	CProtect1 protect;

	CString strProgramPath = Sysset.GetProgramPath();
	strProgramPath += "\\";									// needs trialing '\'

	if ( bPrint == TRUE )	SSspawnl ( m_pParent, P_WAIT, strProgName, strProgName, szReport, protect.GetCode(), szLabel, "1", strProgramPath, "/p", NULL );
	else					SSspawnl ( m_pParent, P_WAIT, strProgName, strProgName, szReport, protect.GetCode(), szLabel, "1", strProgramPath, NULL );
	return nREPORT_NOERROR;
}

//********************************************************************
// szText can override standard error text if passed
// Reply = TRUE if none fatal error

bool CRepmanHandler::DisplayError ( int nErrorNo, const char* szText )
{
	CString strMsg = GetRepmanHandlerError ( nErrorNo, szText );

	if ( strMsg != "" )
		DisplayMessage ( strMsg, MB_ICONEXCLAMATION );

	bool bNonFatal;
	switch ( nErrorNo )
	{
	case nREPORT_NOERROR:		
	case nREPORT_CREATEFAIL:
	case nREPORT_INVALIDCARDRANGE:
	case nREPORT_NOMATCH:
	case nREPORT_NOTRANSACTIONS:
	case nREPORT_OPENERROR:
	case nREPORT_PAUSEERROR:
	case nREPORT_ERRORDISPLAYED:
	case nREPORT_CANCEL:	bNonFatal = TRUE;		// allow retry of report
							break;

	case nREPORT_NOSOURCE:							// no point retrying
	case nREPORT_INVALIDSOURCE:
	case nREPORT_INVALIDMEMBERSHIP:
	case nREPORT_INVALIDDATA:
	case nREPORT_EXIT:
	case nREPORT_NOEODSTART:
	case nREPORT_NOEODEND:
	default:				bNonFatal = FALSE;
							break;
	}
	return bNonFatal;
}

//********************************************************************

void CRepmanHandler::DisplayMessage ( const char* szMsg, int nType )
{
	Prompter.DisplayMessage( szMsg, m_strTitle, nType );
}

//********************************************************************
