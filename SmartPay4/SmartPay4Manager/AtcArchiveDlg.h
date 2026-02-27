#pragma once

#include "SmartPay4Manager.h"
#include "AtcDateList.h"

// CAtcArchiveDlg dialog

class CAtcArchiveDlg : public CSSAutoShutdownDialog
{
public:
	CAtcArchiveDlg ( const char* szFilename, const char* szTitle, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAtcArchiveDlg();

// Dialog Data
	enum { IDD = IDD_ATCARCHIVE };
	int m_nDayCount;
	int m_nLinesOut;
	int m_nDaysOut;
	int m_nEntries;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	const char* GetDate() { return m_strSelDate; }
	const char* GetFirstDate() { return m_strFirstDate; }
	void RemoveEntries ( CSSDate& date );

private:
	void ClearTotals();

private:
	CSSListCtrl m_listboxDate;
	CAtcDateList m_datelist;
	CString m_strSelDate;
	CString m_strFirstDate;
	CString m_strTitle;
};
