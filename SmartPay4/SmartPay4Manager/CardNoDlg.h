#pragma once
//$$******************************************************************
#include "MyMifare.h"
#include "TerminalData.h"
//$$******************************************************************

class CCardNoDlg : public CSSAutoShutdownDialog
{
public:
	CCardNoDlg(const char* szTitle, const char* szCardNo = "", CWnd* pParent = NULL);   // standard constructor
	CString GetUserID()   { return m_strOutputCardNo; }
	CString GetMiFareNo() { return m_strMiFareNo; }

// Dialog Data
	enum { IDD = IDD_CARDNO };
	CButton m_buttonBrowse;
	CButton m_buttonReader;
	CButton m_buttonOK;
	CEdit m_editCount;
	CStatic m_staticPrompt;
	CStatic m_staticRecordCount;
	int m_nRecordCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonReader();
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditCardNo() { return GetEdit(IDC_EDIT_CARDNO); }

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetReaderMode ( bool bMode );
	bool ValidateUserInput();

public:
	bool m_bBrowseButtonAllowed;
	bool m_bCountButtonAllowed;
	bool m_bReaderButtonAllowed;

private:
	CString m_strTitle;
	CWnd* m_pParent;
	CString m_strInputCardNo;
	CString m_strOutputCardNo;
	CString m_strMiFareNo;

	CMyMifare m_reader;
	CTerminalData m_terminal;
	bool m_bReaderMode;
};

//$$******************************************************************
