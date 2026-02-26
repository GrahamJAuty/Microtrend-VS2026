//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CHeaderWarnDlg : public CSSDialog
{
public:
	CHeaderWarnDlg( CWnd* pParent = NULL);  
	virtual ~CHeaderWarnDlg();

public:
	void SetTitle(CString str) { m_strTitle = str; }
	void SetInfo1Text( CString str ) { m_strInfo1Text = str;  }
	void SetInfo2Text( CString str ) { m_strInfo2Text = str;  }
	void SetHeader1Description(CString str) { m_strHeader1Description = str; }
	void SetHeader2Description(CString str) { m_strHeader2Description = str; }
	void SetHeader1(CString str) { m_strHeader1 = str; }
	void SetHeader2(CString str) { m_strHeader2 = str; }
	void SetAllowCancelFlag(bool b) { m_bAllowCancel = b; }

// Dialog Data
	enum { IDD = IDD_HEADER_WARN };
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonView();
	DECLARE_MESSAGE_MAP()

private:
	CString m_strTitle;
	CString m_strInfo1Text;
	CString m_strInfo2Text;
	CString m_strHeader1Description;
	CString m_strHeader2Description;
	CString m_strHeader1;
	CString m_strHeader2;
	bool m_bAllowCancel;
};

//$$******************************************************************
