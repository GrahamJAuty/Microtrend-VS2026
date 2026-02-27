#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditReportConfig.h"
#include "DateSelectorRange.h"
#include "CommentHandler.h"
//$$******************************************************************

class CPCAuditReportDlg : public CSSAutoShutdownDialog
{
public:
	CPCAuditReportDlg( CAuditReportConfig& Config, CWnd* pParent = NULL);
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
public:
// Dialog Data
	enum { IDD = IDD_REPORT_PCAUDIT };

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
	CButton m_checkShowDeleted;
	
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
	//altkey
	DECLARE_MESSAGE_MAP()

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);

private:
	CAuditReportConfig& m_ReportConfig;
};

//$$******************************************************************
