#pragma once
//$$******************************************************************

class CPCAuditReportWrapper
{
public:
	CPCAuditReportWrapper(CString strAuditFilename, CString strLabel, CWnd* pParent);

public:
	void ShowReport();
	
private:
	CString m_strAuditFilename;
	CString m_strLabel;
	CWnd* m_pParent;
};

//$$******************************************************************

