#pragma once
//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************

class CGroupPickerDlg : public CSSAutoShutdownDialog
{
public:
	CGroupPickerDlg( const char* szTitle, const char* szListFilename, CWnd* pParent = NULL );   // standard constructor
	virtual ~CGroupPickerDlg();

// Dialog Data
	enum { IDD = IDD_GROUPPICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnBnClickedButtonAll();
	afx_msg void OnBnClickedButtonInvert();
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void FillList();
	void SetAll ( bool bReqd );

private:
	CString m_strTitle;
	CString m_strListFilename;

private:
	CSSListCtrl m_listctrlPicker;
};

//*******************************************************************
