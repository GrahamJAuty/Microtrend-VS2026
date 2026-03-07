/**********************************************************************/
#pragma once
/**********************************************************************/

class CMyServer : public CSSServer
{
public:
	CMyServer();
	~CMyServer();
	void Log (  const char* szIpAddress, const char * szClient, const char* szServer, int nThreadNo );

protected:
	virtual void OnConnect ( CSSSocket& sockConnect, int nThreadNo, bool& bTerminate );

private:
	void SendFile ( CSSSocket& sock, CStringArray& arCookies, const char* szFilename, CString strVerb, int nThreadNo );
	void SendString ( CSSSocket& sock, CStringArray& arCookies, CString& strSource, CString strVerb, int nThreadNo );

	void HTML_Ok ( CSSSocket& sock, CStringArray& arCookies, const char* szContentType, int nContentLength, int nThreadNo );
	void HTML_Mime_Ok ( CSSSocket& sock, int nThreadNo );
	void HTML_Unauthorized ( CSSSocket& sock, int nThreadNo );
	void HTML_Forbidden ( CSSSocket& sock, int nThreadNo );
	void HTML_NotFound ( CSSSocket& sock, int nThreadNo );
	void HTML_Redirect ( CSSSocket& sock, CStringArray& arRequest, const char* szURL, int nThreadNo );

	void DisplayWindowText ( int nConnectionCount );
	CString FindHeader ( const char* szLabel, CStringArray& arRequest );
	CString GetDefaultFilename ( const char* szPath );
	CString GetRootFilename ( const char* szFilename );
	CString GetExtension ( const char* szFilename );
	CString GetMimeContentType ( const char* szFilename );
	CString GetIpAddress ( CSSSocket& sock );
	CString HtmlEncode ( const char* szBuf );
	CString HtmlDecode ( const char* szBuf );

	bool CheckUsernamePassword ( CStringArray& arRequest, CString& strFile );
	void CreatePasswordArray ( CStringArray& ar );
	void AddPassword ( CStringArray& ar, const char* szFolder, const char* szUsername, const char* szPassword );

	void GetSessionCookies ( CStringArray& arRequest, CCSV& csvSessionCookies );

	void* LoadDll ( const char* szName );
	void UnloadDlls();

public:
	CWnd* m_pWnd;

private:
	int m_nConnectionCount;
	CStringArray m_arMime;
	CStringArray m_arDefaultFilenames;
	CStringArray m_arPasswords;

	CPtrArray m_arDll;
};

/**********************************************************************/
