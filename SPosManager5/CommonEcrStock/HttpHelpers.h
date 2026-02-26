#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CHttpHelpers
{
public:
	const char* Http ( const char* szVerb, bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, int nTimeOut );
	const char* HttpGet ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, int nTimeOut );
	const char* HttpPost ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, int nTimeOut );

private:
	CString m_strResult;
};

/**********************************************************************/
#endif
/**********************************************************************/
