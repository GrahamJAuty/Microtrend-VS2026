#pragma once
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CWebPaymentImportLogDlg : public CSSAutoShutdownDialog
{
public:
	CWebPaymentImportLogDlg ( int nWebPaymentTYpe, CWnd* pParent = NULL); 
	
	enum { IDD = IDD_WEBPAYMENTIMPORTLOG };
	CButton m_buttonDeleteView1;
	CButton m_buttonDeleteView2;
	CButton m_buttonDeleteView3;
	CButton m_buttonOther;
	CButton m_buttonReport1;
	CButton m_buttonReport2;
	CButton m_buttonResend;
	CButton m_buttonRetry;
	CButton m_buttonRetryReport;
	CButton m_buttonRetryReport2;
	CButton m_buttonView1;
	CButton m_buttonView2;
	CButton m_buttonView3;
	CButton m_buttonView4;
	CStatic m_staticExportBox;
	CListBox m_listboxView1;
	CListBox m_listboxView2;
	CListBox m_listboxView3;
	CString	m_strView1File;
	CString	m_strView2File;
	CString	m_strView3File;
	CStatic m_staticPayments;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnButtonView1();
	afx_msg void OnButtonView2();
	afx_msg void OnButtonView3();
	afx_msg void OnButtonDeleteView1();
	afx_msg void OnButtonDeleteView2();
	afx_msg void OnButtonDeleteView3();
	afx_msg void OnButtonReport1();
	afx_msg void OnButtonReport2();
	afx_msg void OnDoubleClickListView1();
	afx_msg void OnDoubleClickListView2();
	afx_msg void OnDoubleClickListView3();
	afx_msg void OnButtonView4();
	afx_msg void OnButtonRetryReport();
	afx_msg void OnButtonRetryReport2();
	afx_msg void OnButtonRetry();
	afx_msg void OnButtonOther();
	afx_msg void OnButtonResend();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private: 
	int AddLogFiles (int nType );
	bool AddToList ( int nType, const char* szFileTitle );
	void DisplayRxReport ( int nType, const char* szFile );
	bool DeleteLog ( int nType, const char* szMask );
	CString GetFilePrefix ( int nType );
	CString GetFilename ( int nType, const char* szMask );
	void SetExceptionButtons();
	void SetExportsMoreButton();
	CString GetExportsBoxTitle();

private:
	int m_nWebPaymentType;

	CString m_strWebPaymentListFilename1;
	CString m_strWebPaymentListFilename2;
	CString m_strWebPaymentExceptionFilename1;
	CString m_strWebPaymentExceptionFilename2;

	int m_nView3Type;
	CString m_strView3TransTitle;

	bool m_bResendAllowed;
	CString m_strReportButtonList1;
	CString m_strReportButtonList2;
	CString m_strRetryButtonList1;
};

//$$******************************************************************
