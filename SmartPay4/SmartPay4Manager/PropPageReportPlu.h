//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AccumulatorReportInfo.h"
#include "ReportPurchaseHelpers.h"
//$$******************************************************************

struct CReportSelectPlu
{
public:
	CReportSelectPlu() 
	{ 
		m_nPluNo = 0;
		m_strText = "";
		m_bDefaultSelect = FALSE;
	}

public:
	__int64 m_nPluNo;
	CString m_strText;
	bool m_bDefaultSelect;
};

//$$******************************************************************

struct CReportSelectPluSettings
{
public:
	bool m_bNewPage;
	bool m_bIncludeUnknownItems;
	bool m_bAlwaysAll;
	CString m_strFindText;
};

//$$******************************************************************

class CPropPageReportPlu : public CSSPropertyPage
{
public:
	CPropPageReportPlu();
	virtual ~CPropPageReportPlu();

public:
	void SetReportLabel ( const char* szLabel );
	void LoadAccumulatorReportInfo(CAccumulatorReportInfo* pInfo);
	void SaveAccumulatorReportInfo(CAccumulatorReportInfo* pInfo);
	void LoadReportPurchaseHelpers(CReportPurchaseHelpers& ReportPurchaseHelpers);
	void SaveReportPurchaseHelpers(CReportPurchaseHelpers& ReportPurchaseHelpers);

	enum { IDD = IDD_PROPPAGE_REPORTPLU };
	CEdit m_editFind;
	CSpinButtonCtrl m_spinFind;
	CButton m_checkNewPage;
	CButton m_checkUnknown;
	CButton m_checkAlwaysAll;
	CStatic m_staticCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonReset();
	afx_msg void OnDeltaposSpinFind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg long OnUpdateCount(WPARAM wIndex, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void FillPluList();
	int FindNext(int nStartPos, bool bAscending);
	void DisplayCount();

public:
	CArray<CReportSelectPlu, CReportSelectPlu> m_arrayPluNo;

private:
	CSSListTaggedSelectCtrl m_listboxPlu;
	CString m_strReportLabel;
	
private:
	CReportSelectPluSettings m_ReportSettings;
	bool m_bPluRankingMode;
	bool m_bAdjustedRowHeight;
	CString m_strPluDataDefaults;
};

//$$******************************************************************

