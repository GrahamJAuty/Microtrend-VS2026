#pragma once
//*********************************************************************
#include "UserIDRecord.h"
//*********************************************************************

class CGetUserDetails
{
public:
	CGetUserDetails(const char* szFilename, CUserIDRecord* pRecord, CWnd* pParentWnd);
	bool Extract(bool bServerValidate);

private:
	bool AddToLoyReqFile(const char* szPrefix, const char* szUserID);
	bool WaitReply(const char* szReplyFilename);

	bool SendLoyReqFile();
	bool MultipleChoice();

public:
	int m_nMessageNo = nMESSAGE_NONE;
	CString m_strErrorMessage = "";

private:
	CString m_strFilename = "";
	CUserIDRecord* m_pRecord = nullptr;
	CStringArray m_arrayUserLines;
	CWnd* m_pParentWnd = nullptr;
};

//*********************************************************************