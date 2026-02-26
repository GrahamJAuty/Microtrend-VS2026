#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsReports1 : public CSSPropertyPage
{
public:
	CPropPageOptionsReports1();
	~CPropPageOptionsReports1();

	//{{AFX_DATA(CPropPageOptionsReports1)
	enum { IDD = IDD_PROPPAGE_OPTIONS_REPORTS1 };
	CDateTimeCtrl m_DatePickerFYear;
	CStatic m_staticTaxQuarter;
	CSSComboBox	m_comboTaxQuarter;
	CSSComboBox	m_comboWeek;
	CSSComboBox	m_comboWeekNumber;
	CDateTimeCtrl m_TimePickerSOD;
	CDateTimeCtrl m_TimePickerTimeAttend;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboSingleDay;
	CSSComboBox	m_comboPreviousDay;
	CButton	m_checkEODSession;
	/*****/
	CSSComboBox m_comboCustomer;
	CSSComboBox m_comboSalesTime;
	CButton m_checkDiscountDetailSales;
	CButton m_checkPremiumDetailSales;
	CButton m_checkNetOfTaxSales;
	CButton m_checkDiscountDetailCovers;
	CButton m_checkPremiumDetailCovers;
	CButton m_checkNetOfTaxCovers;
	CButton m_checkAuthServer;
	CButton m_checkReportpoint;
	CButton m_checkReason;
	CButton m_checkGeneralPage;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsReports1)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg void OnToggleDiscountDetailSales();
	afx_msg void OnToggleDiscountDetailCovers();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageOptionsReports1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRecordControls();

private:
	bool SaveRecord();
	void SetOptionTicks();
	void UpdateTaxQuarterString();

private:
	CString m_strFYear;
};

/**********************************************************************/
#endif
/**********************************************************************/
