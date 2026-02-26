#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "DbExportHandler.h"
#include "ReportInfoAccumulator.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CAccumulatorPropertyPage1 : public CSSPropertyPage
{
public:
	CAccumulatorPropertyPage1();
	virtual ~CAccumulatorPropertyPage1();

public:
	void SetReportInfo( CReportInfoAccumulator* pInfo ){ m_pReportInfo = pInfo; }

public:
	enum { IDD = IDD_PROPPAGE_ACCUMULATOR1 };
	//criteria
	CStatic m_staticType;
	CStatic m_staticTop100;
	CEdit m_editTop100;
	CSSComboBox m_comboRankBy;
	CButton	m_checkNoZeroSkip;
	CStatic	m_staticReportOn;
	CButton m_checkUnsold;
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
	//terminals
	CStatic m_staticEPOS;
	CStatic m_staticDatabase;
	CSSTabbedComboBox m_comboDatabase;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//altkey
	CButton m_buttonAltKey1;

private:
	double m_dThreshold;
	int m_nTop100;

private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//time
	afx_msg void OnSelectTimeRange();
	//terminals
	afx_msg void OnSelectDatabase();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();

	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditThreshold();
	
private:
	CReportInfoAccumulator* m_pReportInfo;
};

//$$******************************************************************

