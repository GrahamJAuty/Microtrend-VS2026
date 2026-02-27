//**********************************************************************
#include "RepmanHandler.h"
//**********************************************************************
extern CSysset Sysset;
//**********************************************************************
const char* szMSG_EXCEPTIONSDETECTED	= "Exceptions were detected with the import data!\n\nPlease see end of report for details.";
const char* szMSG_CREATINGREPORT		= "Creating report";
const char* szMSG_SCANNINGAUDIT			= "Scanning audit file";
const char* szMSG_SCANNINGDATABASE		= "Scanning database";
const char* szMSG_SCANCOMPLETE			= "Database scan complete";
const char* szMSG_SCANCANCELLED			= "Database scan terminated!";
//**********************************************************************

//*******************************************************************/
// szText can override standard error text if passed

CString CRepmanHandler::GetRepmanHandlerError ( int nErrorNo, const char* szText )
{
	CString strMsg = szText;
	if ( strMsg == "" )
	{
		switch ( nErrorNo )
		{
		case nREPORT_NOERROR:		
		case nREPORT_ERRORDISPLAYED:
		case nREPORT_CANCEL:
		case nREPORT_EXIT:				break;

		case nREPORT_NOSOURCE:			strMsg = "No source report data found!";				break;
		case nREPORT_INVALIDSOURCE:		strMsg = "Invalid source data!";						break;
		case nREPORT_CREATEFAIL:		strMsg = "Unable to create output report file!";		break;
		case nREPORT_INVALIDCARDRANGE:	strMsg = "No accounts found in range!";					break;
		case nREPORT_NOMATCH:			strMsg = "No matching account data found!";				break;
		case nREPORT_NOTRANSACTIONS:	strMsg = "No matching transaction data found!";			break;
		case nREPORT_NOREPMAN:			strMsg = "Report Manager has not been installed!";		break;
		case nREPORT_NOBARMAN:			strMsg = "Barcode Manager has not been installed!";		break;
		case nREPORT_INVALIDMEMBERSHIP:	strMsg = "Unable to open membership database!";			break;
		case nREPORT_OPENERROR:			strMsg = "Unable to open source report data";			break;
		case nREPORT_INVALIDDATA:		strMsg = "Invalid data encountered!";					break;
		case nREPORT_PAUSEERROR:		strMsg = "Unable to pause system!";						break;
		case nREPORT_NOEODSTART:		strMsg = "Unable to find previous EOD in audit!";		break;
		case nREPORT_NOEODEND:			strMsg = "Unable to find requested EOD in audit!";		break;

		default:						strMsg.Format ( "Unexpected error ( %d )", nErrorNo );
										break;
		}
	}

	return strMsg;
}

//**********************************************************************
// Convert report file to csv \ html

void CRepmanHandler::SetupSaveAs ( int nSaveAsType, const char* szSaveAsFilename )
{
	m_nSaveAsType		= nSaveAsType;
	m_strSaveAsFilename = szSaveAsFilename;
}

//**********************************************************************

void CRepmanHandler::SaveReportAs ( const char* szReport )
{
	CReportFileDoc report ( szReport );

	switch ( m_nSaveAsType )
	{
	case nSAVEAS_NONE:	break;
	case nSAVEAS_CSV:	report.SaveAsCSV ( m_strSaveAsFilename );	break;
	case nSAVEAS_HTML:	report.SaveAsHTML( m_strSaveAsFilename );	break;
	}
}

//**********************************************************************
