#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "DateSelectorSingle.h"
#include "DbReportHandler.h"
#include "ReportInfoStartingBalance.h"
//$$******************************************************************

class CReportStartingBalanceDlg : public CSSDialog
{
public:
	CReportStartingBalanceDlg( CWnd* pParent = NULL);   
	
public:
	void SetReportInfo( CReportInfoStartingBalance* pInfo ){ m_pReportInfo = pInfo; }

public:
	enum { IDD = IDD_REPORT_STARTINGBALANCE };
	CSSComboBox m_comboDate;
	CDateTimeCtrl m_DatePicker;
	CDateTimeCtrl m_DatePickerPrevious;
	
private:
	CDateSelectorSingle m_DateSelector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//date
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	CReportInfoStartingBalance* m_pReportInfo;
};

//$$******************************************************************
