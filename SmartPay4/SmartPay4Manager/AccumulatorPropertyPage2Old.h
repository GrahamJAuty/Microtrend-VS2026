#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CAccumulatorPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CAccumulatorPropertyPage2();
	virtual ~CAccumulatorPropertyPage2();
	void SetReportLabel ( CString strLabel );

public:
	void SetReportInfo(CAccumulatorReportInfo* pInfo) { m_pReportInfo = pInfo; }

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ACCUMULATOR2 };
	CSSListTaggedSelectCtrl m_listPlu;
	CButton m_checkNewPage;
	CString m_strFindString;
	CSpinButtonCtrl m_spinFind;
	CStatic m_staticCount;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSpinFind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDoubleClickStaticCount();
	afx_msg long OnUpdateCount(WPARAM wIndex, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CStringArray m_arrayList;

private:
	int FindNext ( int nStartPos, bool bAscending );
	void DisplayCount();

private:
	CString m_strReportLabel;

private:
	CAccumulatorReportInfo* m_pReportInfo;
};

//*******************************************************************
