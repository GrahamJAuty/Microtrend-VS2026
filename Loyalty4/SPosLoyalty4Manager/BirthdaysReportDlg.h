#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DateSelectorRange.h"
//$$******************************************************************
#include "ReportInfoBirthday.h"
//$$******************************************************************

class CBirthdaysReportDlg : public CDialog
{
public:
	CBirthdaysReportDlg( CWnd* pParent = NULL);   
	virtual ~CBirthdaysReportDlg();

public:
	void SetReportInfo( CReportInfoBirthday* pInfo ){ m_pReportInfo = pInfo; }

public:
	enum { IDD = IDD_BIRTHDAYS };
	//expiry
	CButton m_checkExpiry;
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CButton m_checkExpiryMonth;
	//dob
	CStatic m_staticDOB;
	CButton m_checkDOB;
	CButton m_checkMonth[12];
	CButton m_buttonAll;
	CButton m_buttonNext;

private:
	CDateSelectorRange m_DateSelectorRange;

private:
	int m_nCheckMonthID[12];

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//expiry
	afx_msg void OnToggleExpiry();
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//dob
	afx_msg void OnToggleDOB();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonAll();

	DECLARE_MESSAGE_MAP()
	
private:
	int m_nSelectAll;
	int m_nCurrentMonth;
	int m_nNextMonth;

private:
	int MonthsSelected();

private:
	CReportInfoBirthday* m_pReportInfo;
};

//$$******************************************************************
