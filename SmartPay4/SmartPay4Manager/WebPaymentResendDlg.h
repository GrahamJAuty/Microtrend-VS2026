#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CWebpaymentResendDlg : public CSSAutoShutdownDialog
{
public:
	CWebpaymentResendDlg( const char* szTitle, const char* szDbFilename, const char* szDestination, const char* szDisplayHeader, CWnd* pParent = NULL);   // standard constructor
	
	void SetSortField ( const char* szSortField, int nSortType ) { m_strSortField = szSortField; m_nSortType = nSortType; }

	void SetConnectingPOSTxnID ( int nConnectingPOSFieldNo, __int64 nConnectingPOSTxnID );
	__int64 GetNextConnectingPOSTxnID() { return m_nConnectingPOSTxnID; }			// return next ID to use

	bool GetSendConfirmation();

// Dialog Data
	enum { IDD = IDD_WEBPAYMENTRESEND };
	CSpinButtonCtrl	m_spinCardPicker;
	int m_nTotal;
	int m_nCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnItemChangingList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinCardPicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonInvert();
	afx_msg void OnButtonClear();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void RemoveMultiples ( const char* szSortLabel );
	void FillList();
	void SetAll ( bool bReqd );
	int GetSelectedCount();
	void EnableSpinFind ( int nCount );

private:
	CString m_strTitle;
	CString m_strDbFilename;
	CString m_strDestination;
	CCSV m_csvDisplayHeader;
	CWnd* m_pParent;

	CSSUnsortedDatabase m_db;
	CString m_strSortField;
	int m_nSortType;

	CSSListCtrl m_listctrlPicker;
	__int64 m_nConnectingPOSTxnID;
	int m_nConnectingPOSFieldNo;
};

//$$******************************************************************
