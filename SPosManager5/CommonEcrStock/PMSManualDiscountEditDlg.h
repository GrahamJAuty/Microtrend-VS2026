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

class CPMSManualDiscountEditDlg : public CSSDialog
{
public:
	CPMSManualDiscountEditDlg( bool bAdd, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPMSManualDiscountEditDlg)
	enum { IDD = IDD_PMS_MANUAL_DISCOUNT_EDIT };
	CButton	m_buttonCancel;
	CButton	m_buttonOK;
	double	m_dDiscount;
	//}}AFX_DATA
	CStatic m_staticDiscount;
	
	//{{AFX_VIRTUAL(CPMSManualDiscountEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CPMSManualDiscountEditDlg)
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
	CEdit* GetEditDiscount();
	
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

