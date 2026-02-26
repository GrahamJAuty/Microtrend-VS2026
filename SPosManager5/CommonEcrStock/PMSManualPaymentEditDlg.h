#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSInvoiceLineInfo.h"
/**********************************************************************/

class CPMSManualPaymentEditDlg : public CSSDialog
{
public:
	CPMSManualPaymentEditDlg( bool bAdd, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPMSManualPaymentEditDlg)
	enum { IDD = IDD_PMS_MANUAL_PAYMENT_EDIT };
	CButton	m_buttonCancel;
	CButton	m_buttonOK;
	double	m_dPayment;
	//}}AFX_DATA
	CSSComboBox m_comboPayment;

	//{{AFX_VIRTUAL(CPMSManualPaymentEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CPMSManualPaymentEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DataInit();
	void SetEditFocus();
	void HandleAdd();
		
private:
	CEdit* GetEditPayment();
	
public:
	void SetSalesInfo( CPMSInvoiceLineInfo& sale ){ m_SaleInfo = sale; }
	void GetSalesInfo( CPMSInvoiceLineInfo& sale ){ sale = m_SaleInfo; }
	
private:
	bool m_bAdd;
	bool m_bModified;
		
private:
	CPMSInvoiceLineInfo m_SaleInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/


