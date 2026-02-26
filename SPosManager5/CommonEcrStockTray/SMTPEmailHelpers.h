#pragma once
/**********************************************************************/
#define SMTP_ERROR_NONE 0
#define SMTP_ERROR_CONFIG 3
#define SMTP_ERROR_EXCEPTION 4
#define SMTP_ERROR_UNKNOWN 6	
/**********************************************************************/
#define SMTP_PENDING_WAIT_SEND 0
#define SMTP_PENDING_WAIT_PREPARE 1
#define SMTP_PENDING_WAIT_RETRY 2
#define SMTP_PENDING_SENT 3
#define SMTP_PENDING_REDIRECTED 4
#define SMTP_PENDING_INVALID 5
/**********************************************************************/
#define SMTP_SESSIONTYPE_CUSTOMER 1
#define SMTP_SESSIONTYPE_PMS 2
#define SMTP_SESSIONTYPE_ORDER 3
#define SMTP_SESSIONTYPE_TEST 4
/**********************************************************************/

struct CSMTPPendingEmailInfo
{
public:
	CSMTPPendingEmailInfo();

public:
	CString m_strFilename;
	CString m_strFriendlyName;
	int m_nStatus;
	int m_nContextError;
	int m_nSMTPError;
};

/**********************************************************************/

class CSMTPEmailHelpers
{
public:
	CSMTPEmailHelpers();

public:
	void OpenSession(int nType);
	void CreatePendingEmail(const char* szAddress, const char* szSubject, const char* szFriendlyName, CReportFileDoc& ReportFileDoc, const char* szFilename);
	void CreatePendingEmail(CStringArray& arrayEmailAddresses, const char* szSubject, const char* szFriendlyName, CReportFileDoc& ReportFileDoc, const char* szFilename);
	void FlagUnsendableEmail(const char* szFriendlyName, int nContextError);
	void FlagRedirectedEmail(const char* szFriendlyName);
	void CreateIndexFile();
	void DeleteSession();

public:
	int GetSessionSize() { return m_arraySession.GetSize(); }
	void GetSessionEmailInfo(int nIdx, CSMTPPendingEmailInfo& info);
	void SetSessionEmailInfo(int nIdx, CSMTPPendingEmailInfo& info);
	int SendSessionEmail(CWnd* pParent, int nIdx);
	bool CheckAllSent();

private:
	int SendEmail(CWnd* pParent, const char* szToAddress, const char* szSubject, const char* szEmailFilename);

public:
	static void GetErrorText(int nErrorCode, CString& strError);

public:
	const char* GetIndexFileName(bool bCreateFolders = FALSE);
	const char* GetSessionFolderName(bool bCreateFolders = FALSE);
	const char* GetEmailFileName(int nIdx);


private:
	CString m_strCurrentSession;
	CArray<CSMTPPendingEmailInfo, CSMTPPendingEmailInfo> m_arraySession;
	int m_nErrorCount;
	int m_nRedirectCount;

private:
	CString m_strIndexFileName;
	CString m_strEmailFileName;
	CString m_strSessionFolderName;
	int m_nSessionType;
};

/**********************************************************************/
