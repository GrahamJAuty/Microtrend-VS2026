#pragma once
/******************************************************************************/

class CLeaversHandler
{
public:
	CLeaversHandler ( const char* szYear );
	~CLeaversHandler(void);

	CString GetYear() { return m_strLeaversYear; }
	CString GetPurchaseHistoryFilename ( const char* szCardNo );
	CString GetPhotoIDFilename ( const char* szFilename );

public:
	CString m_strLeaverDatabase;
	CString m_strLeaversFindListFile;
	CString m_strAuditFilename;
	CString m_strSessionAuditFilename;


private:
	CString m_strPurchaseFolder;
	CString m_strPhotoIDFolder;
	CString m_strLeaversYear;
};

/******************************************************************************/
