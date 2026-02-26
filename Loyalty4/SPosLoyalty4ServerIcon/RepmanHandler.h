#pragma once

#define	nREPORT_NOERROR				0
#define	nREPORT_NOSOURCE			1
#define	nREPORT_INVALIDSOURCE		2
#define	nREPORT_CREATEFAIL			3
#define	nREPORT_INVALIDCARDRANGE	4
#define	nREPORT_NOMATCH				5
#define	nREPORT_CANCEL				6
#define nREPORT_EXIT				7
#define nREPORT_OPENERROR			8
#define nREPORT_DBFILEERROR			9
#define nREPORT_NOREPMAN			101
#define nREPORT_NOBARMAN			102

#define nSAVEAS_NONE			0
#define nSAVEAS_CSV				1
#define nSAVEAS_HTML			2

class CRepmanHandler
{
public:
	CRepmanHandler ( const char* szTitle, CWnd* pParent );
	void SetMode ( int nMode ) { m_nMode = nMode; }
	void SetTitle ( const char* szTitle ) { m_strTitle = szTitle; }

	int DisplayReport ( const char* szReport, const char* szParams, const char* szParamsKey = "", bool bPrintReqd = FALSE );
	
	void SetupSaveAs ( int nSaveAsType, const char* szSaveAsFilename );
	bool DisplayError ( int nErrorNo );
	void DisplayMessage ( const char* szMsg, int nType = MB_OK );

	CString GetTitle() { return m_strTitle; }

private:
	void SaveReportAs ( const char* szReport );

private:
	CString m_strTitle;
	CWnd* m_pParent;

	int m_nMode;
	int m_nSaveAsType;
	CString m_strSaveAsFilename;
};

