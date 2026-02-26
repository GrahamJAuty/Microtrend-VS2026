/**********************************************************************/

#include "Crypto.h"

/**********************************************************************/

static void GetHeaders ( CStringArray& arHeaders, const char* szUsername, const char* szPassword, const char* szProviderName, const char* szProviderVersion )
{
	CString strUserPass;
	strUserPass.Format ( "%s:%s", szUsername, szPassword );
	CString strBase64 = AsciiToBase64 ( strUserPass );
	arHeaders.Add ( "Authorization: Basic " + strBase64 );

	CString strProviderName = "ProviderName: ";
	strProviderName += szProviderName;
	arHeaders.Add ( strProviderName );

	CString strProviderVersion = "ProviderVersion: ";
	strProviderVersion += szProviderVersion;
	arHeaders.Add ( strProviderVersion );
}

/**********************************************************************/

CString TucasiGet ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szBody, const char* szResponseFilename, const char* szUsername, const char* szPassword, const char* szProviderName, const char* szProviderVersion, const char* szAgent )
{
	CStringArray arHeaders;

	GetHeaders ( arHeaders, szUsername, szPassword, szProviderName, szProviderVersion );

	return HttpGet ( bUseHttps, szIPAddress, szObjectName, arHeaders, szBody, szResponseFilename, szAgent );
}

/**********************************************************************/

CString TucasiPost ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szBody, const char* szResponseFilename, const char* szUsername, const char* szPassword, const char* szProviderName, const char* szProviderVersion, const char* szAgent )
{
	CStringArray arHeaders;
	arHeaders.Add ( "Content-Type: application/json" );

	GetHeaders ( arHeaders, szUsername, szPassword, szProviderName, szProviderVersion );

	return HttpPost ( bUseHttps, szIPAddress, szObjectName, arHeaders, szBody, szResponseFilename, szAgent );
}

/**********************************************************************/
