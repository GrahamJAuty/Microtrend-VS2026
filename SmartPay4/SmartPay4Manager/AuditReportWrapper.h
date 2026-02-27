#pragma once
//$$******************************************************************

class CAuditReportWrapper
{
public:
	CAuditReportWrapper( CString strAuditFilename, CString strLabel, __int64 nSingleUserID, CWnd* pParent);
	
public:
	void SetAuditArray(CStringArray* pArray);
	void ShowReport();
	void ShowEditSessionAudit();
	void ShowArchive();

private:
	CString m_strAuditFilename;
	CString m_strLabel;
	__int64 m_nSingleUserID;
	CWnd* m_pParent;

private:
	//USED FOR INCONTEXT AUDIT WHILE EDITING RECORD
	CStringArray* m_pAuditArray;
};

//$$******************************************************************

