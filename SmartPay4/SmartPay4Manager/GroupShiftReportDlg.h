#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "GroupShiftReportConfig.h"
#include "DateSelectorRange.h"
//$$******************************************************************

class CGroupShiftReportDlg : public CSSAutoShutdownDialog
{
public:
	CGroupShiftReportDlg( CGroupShiftReportConfig& Config, CWnd* pParent = NULL);
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
public:
// Dialog Data
	enum { IDD = IDD_REPORT_GROUPSHIFT };

	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	CButton m_checkContinuous;
	//accounts
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CButton m_checkNewPage;
	CButton m_checkSortByCardNo;
	CSSTabbedComboBox m_tabcomboGroup;
	CButton m_checkShowRemoved;
	CButton m_checkSourceManager;
	CButton m_checkSourceServer;
	CButton m_checkSourceBackground;
	CButton m_checkSourceManual;
	CButton m_checkShowPrevious;

private:
	CDateSelectorRange m_DateSelectorRange;
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void OnOK();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//time
	afx_msg void OnSelectTimeRange();
	//account selection
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnToggleSortByCardNo();
	DECLARE_MESSAGE_MAP()

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);

private:
	CGroupShiftReportConfig& m_ReportConfig;
};

//$$******************************************************************
