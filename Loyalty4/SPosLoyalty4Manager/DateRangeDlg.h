#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DateSelectorRange.h"
#include "ReportInfoDateRange.h"
//$$******************************************************************

class CDateRangeDlg : public CSSDialog
{
public:
	CDateRangeDlg( const char* szLabel, CWnd* pParent = NULL); 

public:
	void SetReportInfo( CReportInfoDateRange* pInfo ){ m_pReportInfo = pInfo; }

public:
	enum { IDD = IDD_DATERANGE };
	CStatic m_staticRange;
	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	CStatic m_staticStart;
	CStatic m_staticEnd;
	//close buttons
	CButton m_buttonOK;
	CButton m_buttonCancel;

	//options
	CButton m_checkSummary;

private:
	CDateSelectorRange m_DateSelectorRange;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//time
	afx_msg void OnSelectTimeRange();
	
	DECLARE_MESSAGE_MAP()

private:
	CString m_strReportLabel;

private:
	CReportInfoDateRange* m_pReportInfo;
};

//$$******************************************************************
