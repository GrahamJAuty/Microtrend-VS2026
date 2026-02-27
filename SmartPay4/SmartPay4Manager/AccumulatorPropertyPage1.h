#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*******************************************************************
#include "AccumulatorReportInfo.h"
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "ReportLocationControls.h"
//*******************************************************************

class CAccumulatorPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CAccumulatorPropertyPage1();
	virtual ~CAccumulatorPropertyPage1();
	
public:
	void SetReportInfo(CAccumulatorReportInfo* pInfo) { m_pReportInfo = pInfo; }

	// Dialog Data
	enum { IDD = IDD_PROPPAGE_ACCUMULATOR1 };
	//criteria
	CStatic m_staticType;
	CStatic m_staticTop100;
	CSSComboBox m_comboRankBy;
	CButton	m_checkNoZeroSkip;
	CStatic	m_staticReportOn;
	CStatic m_staticInfo;
	CSSComboBox m_comboInfo;
	//date
	CStatic m_staticDate;
	CSSComboBox m_comboDateRange;
	CStatic m_staticDateFrom;
	CStatic m_staticDateTo;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	CStatic m_staticTimeStart;
	CStatic m_staticTimeEnd;
	CButton m_checkContinuous;
	//terminals
	CStatic m_staticEPOS;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//altkey
	CButton m_buttonAltKey1;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectTimeRange();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditThreshold();
	CEdit* GetEditTop100();

private:
	CAccumulatorReportInfo* m_pReportInfo;
	
private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
};

//*******************************************************************
