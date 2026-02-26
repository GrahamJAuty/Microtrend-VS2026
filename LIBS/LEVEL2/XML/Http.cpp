/**********************************************************************/

static CString Http ( const char* szVerb, bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, const char* szAgent )
{
	CString strError;

	CSSFile file;
	if ( file.Open ( szResponseFilename, "wb" ) == FALSE )
	{
		strError = "Http: Cannot create file";
		return strError;
	}

	CHttpConnection* pConnection = NULL;
	CHttpFile* pFile1 = NULL;

	try
	{
		CInternetSession session(szAgent);

		CString strIPAddress = szIPAddress;
		int nPortNo = ( bUseHttps == FALSE ) ? 80 : INTERNET_DEFAULT_HTTPS_PORT;

		CCSV csv ( szIPAddress, ':' );	// separate ip address and port number
		if ( csv.GetSize() >= 2 )
		{
			strIPAddress = csv.GetString(0);
			nPortNo = csv.GetInt(1);
		}

		if ( bUseHttps == FALSE )
		{
			pConnection = session.GetHttpConnection ( strIPAddress, (INTERNET_PORT) nPortNo );
			pFile1 = pConnection->OpenRequest ( szVerb, szObjectName, NULL, 1, NULL, NULL, INTERNET_FLAG_DONT_CACHE );
		}
		else
		{
			pConnection = session.GetHttpConnection ( strIPAddress, (INTERNET_PORT) nPortNo );
			pFile1 = pConnection->OpenRequest ( szVerb, szObjectName, NULL, 1, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE );
		}

		for ( int i = 0 ; i < arHeaders.GetSize() ; i++ )
			pFile1->AddRequestHeaders ( arHeaders.GetAt(i), HTTP_ADDREQ_FLAG_ADD );

		pFile1->SendRequest ( NULL, 0, (LPVOID)szBody, strlen(szBody) );

		CString strBuf;
		while ( pFile1->ReadString ( strBuf ) == TRUE )
			file.WriteString ( strBuf );
	}
	catch ( CInternetException* e )
	{
		strError.Format ( "Http: Error %d", e->m_dwError );
		e->Delete();
	}

	file.Close();

	if ( pFile1 != NULL )
	{
		pFile1->Close();
		delete pFile1;
	}
	if ( pConnection != NULL )
	{
		pConnection->Close();
		delete pConnection;
	}

	return strError;
}

/**********************************************************************/

CString HttpGet ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, const char* szAgent )
{
	return Http ( "GET", bUseHttps, szIPAddress, szObjectName, arHeaders, szBody, szResponseFilename, szAgent );
}

/**********************************************************************/

CString HttpPost ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, const char* szAgent)
{
	return Http ( "POST", bUseHttps, szIPAddress, szObjectName, arHeaders, szBody, szResponseFilename, szAgent );
}

/**********************************************************************/
