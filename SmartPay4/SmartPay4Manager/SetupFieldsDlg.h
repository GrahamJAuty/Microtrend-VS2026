#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSetupFieldsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupFieldsDlg( const char* szFilename, const char* szCaption, const char* szDefault, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SETUP_FIELDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	void SetData();

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();
	
public:
	bool m_bOpenFileError;

private:
	CString m_strFilename;
	CString m_strCaption;
	CString m_strDefault;

	CString m_strField1;
	CString m_strField2;
	CString m_strField3;
	CString m_strField4;
	CString m_strField5;
	CString m_strField6;
	CString m_strField7;
	CString m_strField8;
	CString m_strField9;

private:
	CSharedStringArray m_csvarray;	
};

//$$******************************************************************

