#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DateSelectorRange.h"
#include "ReportInfoCashless.h"
//$$******************************************************************

class CCashlessAuditDlg : public CSSDialog
{
public:
	CCashlessAuditDlg( void* pReport, CWnd* pParent = NULL); 

public:
	void SetReportInfo( CReportInfoCashless* pInfo ){ m_pReportInfoCashless = pInfo; }

public:
	enum { IDD = IDD_CASHLESSAUDIT };
	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CDateTimeCtrl m_TimePickerCutoff;
	CSSComboBox m_comboNextDay;
	//lookback
	CStatic m_staticPrevious1;
	CStatic m_staticPrevious2;
	CDateTimeCtrl m_DatePickerPrevious;
	CSSComboBox m_comboRefund;

private:
	CDateSelectorRange m_DateSelectorRange;
	
public:
	int GetFinalError(){ return m_nFinalError; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
	
private:
	void* m_pReport;
	int m_nFinalError;

private:
	CReportInfoCashless* m_pReportInfoCashless;
};

//$$******************************************************************
