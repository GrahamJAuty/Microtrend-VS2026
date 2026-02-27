#pragma once
//****************************************************************************
#include "UserIDRecord.h"
//****************************************************************************

class CGetEnrolmentDetails
{
public:
	CGetEnrolmentDetails ( const char* szMiFareNo, const char* szAccountID, CUserIDRecord* pRecord, CWnd* pParent);
	bool Extract();

private:
	bool CreateLoyReqFile();
	bool WaitReply ( const char* szReplyFilename );
	bool SendLoyReqFile();

public:
	int m_nMessageNo = nMESSAGE_NONE;
	CString m_strErrorMessage = "";

private:
	CString m_strAccountID = "";
	CString m_strMiFareNo = "";
	CUserIDRecord* m_pRecord = nullptr;
	CWnd* m_pParent = nullptr;
};

//****************************************************************************
