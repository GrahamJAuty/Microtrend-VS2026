#pragma once
//$$******************************************************************

class CGroupShiftReportWrapper
{
public:
	CGroupShiftReportWrapper( CString strLabel, __int64 nSingleUserID, CWnd* pParent);
	
public:
	void ShowReport();
	
private:
	CString m_strLabel;
	__int64 m_nSingleUserID;
	CWnd* m_pParent;
};

//$$******************************************************************

