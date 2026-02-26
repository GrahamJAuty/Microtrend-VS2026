#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSInvoiceArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSInvoiceDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSInvoiceDlg)

public:
	CPMSInvoiceDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bCheckout, bool bAllowCancel, bool bPendingBooking, CWnd* pWndParent = NULL);   
	virtual ~CPMSInvoiceDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

protected:
	//{{AFX_MSG(CPMSInvoiceDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonSpend();
	afx_msg void OnButtonPayment();
	afx_msg void OnButtonAuto1();
	afx_msg void OnButtonAuto2();
	afx_msg void OnButtonAuto3();
	afx_msg void OnButtonAuto4();
	afx_msg void OnButtonAuto5();
	afx_msg void OnButtonAuto6();
	afx_msg void OnButtonAuto7();
	afx_msg void OnButtonAuto8();
	afx_msg void OnButtonAuto9();
	afx_msg void OnButtonAuto10();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonConfirm();
	afx_msg void OnSelectInvoiceType0();
	afx_msg void OnSelectInvoiceType1();
	afx_msg void OnSelectInvoiceType2();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	//{{AFX_DATA(CPMSInvoiceDlg)
	enum { IDD = IDD_PMS_INVOICE };
	//}}AFX_DATA
	
	CStatic m_staticBalanceBox;
	CEdit m_editSpend;
	CButton m_buttonSpend;
	CEdit m_editAllowance;
	CEdit m_editPaid;
	CButton m_buttonPaid;
	CStatic m_staticBalance;
	CEdit m_editBalance;
	
	CStatic m_staticPaymentBox;
	CStatic m_staticPayment[10];
	CButton m_buttonAuto[10];
	CButton m_buttonConfirm;
	
	CStatic m_staticInvoiceBox;
	CButton m_radioInvoice1;
	CButton m_radioInvoice2;
	CButton m_radioInvoice3;
	CButton m_buttonDisplay;
	CButton m_buttonPrint;
	CButton m_buttonOK;
	CButton m_buttonCancel;

public:
	double m_dPayment[10];

private:
	CEdit* GetEditPayment( int n );

private:
	void SelectInvoiceType( int n );
	void AutoBalance( int n );
	void CreatePaymentArray( CPMSInvoiceArray& arrayPayment );
	bool CheckPendingPayments();
	void DisplayBalance();

private:
	int m_nEditID[10];
	int m_nButtonID[10];
	int m_nPaymentNo[10];
	CString m_strPaymentName[10];

private:
	bool m_bPaymentOwed;
	int m_nInvoiceType;
	int m_nPaymentTypeCount;
	double m_dPaid;
	double m_dSpend;
	double m_dAllowance;
	double m_dBalance;

private:
	int m_nRoomGridRoomIdx;
	int m_nRoomGridAccountIdx;
	bool m_bCheckout;
	bool m_bAllowCancel;
	bool m_bPendingBooking;

private:
	CPMSRoomStatusInfo m_infoRoom;
	CPMSRoomAccountInfo m_infoAccount;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/