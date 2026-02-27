/**********************************************************************/
#include "..\SmartPay4Shared\EmailOptions.h"
/**********************************************************************/
#include "EmailHandler.h"
/**********************************************************************/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/**********************************************************************/

CEmailHandler::CEmailHandler()
{
	m_strError = "";
	m_nCount = 0;
}

/**********************************************************************/
// Send a html report file
// Must specify full path NOT RELATIVE PATH
/**********************************************************************/

bool CEmailHandler::SendEmail ( const char* szEmailAddress, const char* szSubject, const char* szEmailFullSourcePath  )
{
	bool bReply = FALSE;

	if ( EmailOptions.UseSMTPForDirectEmail() == FALSE )
	{
		CSSMapi32 mapi;
		bReply = mapi.SendMessage ( szEmailAddress, szSubject, szEmailFullSourcePath, TRUE );
		m_strError = mapi.GetError();
	}
	else
	{
		bReply = CreateSMTPMessage( szEmailAddress, szSubject, szEmailFullSourcePath );
	}

	return bReply;
}

/**********************************************************************/

bool CEmailHandler::IsLongFilename ( const char* szFilename )
{
	CString strFilename	= szFilename;
	int nLength			= strFilename.GetLength();

	if ( nLength > 12 )
		return TRUE;

	int nPtr = strFilename.Find ( '.' );			// look for start of extension
	if ( nPtr == -1 )								// see if have extension
	{
		if ( nLength > 8 )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CEmailHandler::CreateSMTPMessage( const char* szEmailAddress, const char* szSubject, const char* szEmailFullSourcePath )
{
	CString strEmailFolder = "Email";
	GetSyssetProgramPath( strEmailFolder );
	CreateSubdirectory( strEmailFolder );
	strEmailFolder += "\\Reports";
	CreateSubdirectory( strEmailFolder );

	CString strEmailFullSourcePath = szEmailFullSourcePath;
	
	{
		CString strTestExt = strEmailFullSourcePath.Right(4);
		strTestExt.MakeUpper();

		if ( strTestExt != ".HTM" )
		{
			m_strError = "Unexpected extension type for SMTP email";
			return FALSE;
		}
	}

	CString strDestFilename = "";

	do
	{
		strDestFilename.Format( "%s\\%2.2X%8.8X",
			(const char*) strEmailFolder,
			Sysset.GetInstanceNo(),
			GetTickCount() );

		if ( ::FileExists( strDestFilename + ".HTM" ) == FALSE )
		{
			break;
		}
	}
	while( TRUE );

	CSSFile fileNKN;
	if ( fileNKN.Open( strDestFilename + ".NKN", "wb" ) == TRUE )
	{
		fileNKN.WriteLine( szEmailAddress );
		fileNKN.WriteLine( szSubject );
	}
	else
	{
		m_strError = "Unable to create NKN file";
		return FALSE;
	}

	if ( CopyFile( strEmailFullSourcePath, strDestFilename + ".HTM", FALSE ) == 0 )
	{
		m_strError = "Unable to copy email file";
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/
