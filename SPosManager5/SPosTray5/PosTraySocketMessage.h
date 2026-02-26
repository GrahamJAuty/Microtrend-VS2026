#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/

class CPosTraySocketMessage
{
public:
	CPosTraySocketMessage();
	void Reset();
	void ParseData();

public:
	void CreateParamsFilename();
	void CreatePendingFolder();
	void SetParamsFilename( const char* sz );

public:
	void WriteParamsFile();
	bool AwaitReply( int nTimeOut, bool& bTerminate );
	void CreateHTMLResponseFile( const char* szMessage );
	const char* CreateHTMLResponseString( const char* szMessage );
	void DeletePendingFiles();

public:
	void GetPosTraySocketFilePath( CString& strFilename );
	const char* GetHTMLFilePath();
	const char* GetParamsFilePath();
	const char* GetParamsFileName(){ return m_strParamsFilename; }

private:
	void ParseDataInternal( CString& strData, CReportConsolidationArray<CSortedStringByString>& arrayProperties );
	void ParseChunk( CString& strChunk, CReportConsolidationArray<CSortedStringByString>& arrayProperties );

public:
	CString m_strIntro;
	CString m_strHeader;
	CString m_strBody;

private:
	CReportConsolidationArray<CSortedStringByString> m_arrayHeaderProperties;
	CReportConsolidationArray<CSortedStringByString> m_arrayBodyProperties;

private:
	CString m_strHTMLFilePath;
	CString m_strParamsFilePath;
	CString m_strParamsFilename;
	CString m_strHTMLResponse;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
