#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsReports2 : public CSSPropertyPage
{
public:
	CPropPageOptionsReports2();
	~CPropPageOptionsReports2();

	//{{AFX_DATA(CPropPageOptionsReports2)
	enum { IDD = IDD_PROPPAGE_OPTIONS_REPORTS2 };
	CSSComboBox m_comboReportCash;
	CButton m_checkRAPayment1;
	CButton m_checkRAPayment2;
	CButton m_checkRABalance1;
	CButton m_checkRABalance2;
	CButton m_checkPaymentChange;
	CButton m_checkPaymentQuantity;
	/*****/
	CStatic m_staticVIP;
	CSSComboBox m_comboVIPType;
	CSSComboBox m_comboVIPLevel;
	CButton m_checkVIPRetail;
	/*****/
	CStatic m_staticCustomer1;
	CStatic m_staticCustomer2;
	CStatic m_staticCustomer3;
	CStatic m_staticCustomer4;
	CButton m_checkTransaction;
	CButton m_checkStatement;
	CButton m_checkContact;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsReports2)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageOptionsReports2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTogglePaymentChange();
	afx_msg void OnToggleRAPayment1();
	afx_msg void OnToggleRABalance1();
	afx_msg void OnToggleCustomerContact();
	afx_msg void OnSelectVIPType();

private:
	CEdit* GetEditBefore(){ return GetEdit( IDC_EDIT_BEFORE ); }
	CEdit* GetEditAfter(){ return GetEdit( IDC_EDIT_AFTER ); }
	CEdit* GetEditVisit(){ return GetEdit( IDC_EDIT_VISIT ); }

public:
	void SetRecordControls();
	void ShowOrHideCustomerArea();
	
private:
	void SaveRecord();
	void GetRecordData();
};

/**********************************************************************/
#endif
/**********************************************************************/
