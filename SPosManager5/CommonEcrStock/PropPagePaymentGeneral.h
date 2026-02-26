#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "GroupSelector.h"
#include "PaymentCSVArray.h"
/**********************************************************************/

class CPropPagePaymentGeneral : public CSSPropertyPage
{
public:
	CPropPagePaymentGeneral();
	~CPropPagePaymentGeneral();

	//{{AFX_DATA(CPropPagePaymentGeneral)
	enum { IDD = IDD_PROPPAGE_PAYMENT_GENERAL };
	
	CButton	m_checkTender;
	CButton	m_checkOverTender;
	CButton	m_checkChange;
	CButton	m_checkGratuity;
	CButton	m_checkDrawer;
	CButton	m_checkNonBank;
	CButton	m_checkReceipt;
	CButton	m_checkAccount;
	CButton	m_checkCashDecl;
	CButton	m_checkChangeAsCashback;
	
	CEdit	m_editEPOSName;
	CEdit	m_editReportName;
	CSSComboBox	m_comboType;
	CEdit	m_editGroup;
	CSSComboBox	m_comboGroup;
	CSSComboBox	m_comboFunction;
	CStatic m_staticOption1;
	CEdit m_editParams;
	CSSComboBox m_comboDepositChangeType;
	CStatic m_staticOption2;
	CButton	m_checkCashback;
	CButton	m_checkHandheld;
	CButton m_checkCashLimit;
	
	CString	m_strPayment;
	int		m_nForexNo;
	int		m_nDepositChangeVoucherMin;
	double	m_dLimit;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePaymentGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePaymentGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPaymentType();
	afx_msg void OnSelectGroup();
	afx_msg void OnKillFocusGroupNo();
	afx_msg void OnButtonSetGroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPaymentRecord ( CPaymentCSVRecord* pPaymentRecord ) { m_pPaymentRecord = pPaymentRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	
	void FillTypeCombo();
	void FillFunctionCombo();

private:
	CEdit* GetEditLimit() { return GetEdit( IDC_EDIT_LIMIT ); }
	CEdit* GetEditForex() { return GetEdit( IDC_EDIT_FOREX ); }
	CEdit* GetEditDepositVoucherMin() { return GetEdit( IDC_EDIT_DEPOSIT_VOUCHER_MIN ); }

private:
	CGroupSelector m_GroupSelector;
	
private:
	CPaymentCSVRecord* m_pPaymentRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/

