#pragma once
//$$******************************************************************

class CAuditReportWrapper
{
public:
	CAuditReportWrapper( CString strAuditFilename, CString strLabel, __int64 nUserID, CWnd* pParent);

public:
	void ShowReport();
	void ShowArchive();

private:
	CString m_strAuditFilename;
	CString m_strLabel;
	__int64 m_nUserID;
	CWnd* m_pParent;
};

//$$******************************************************************

