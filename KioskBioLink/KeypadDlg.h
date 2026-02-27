#pragma once
//*******************************************************************
#include "Cardlink.h"
//*******************************************************************

class CKeypadDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CKeypadDlg)

public:
	CKeypadDlg(bool bAdminPassword, CWnd* pParent);
	virtual ~CKeypadDlg();

	enum { IDD = IDD_KEYPADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton0();
	afx_msg void OnButtonClear();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	void AddDigit ( CString strNo );

public:
	CString m_strPrompt = "";
	CString m_strInput = "";
	bool m_bSecretInput = TRUE;
	CString m_strSecretText = "";

private:
	CString m_strDisplay = "";
	bool m_bForAccountPINNumber = FALSE;

	CSSColourButton m_buttonOK;
	CSSColourButton m_buttonCancel;
	CSSColourButton m_buttonClear;
	CSSColourButton m_button1;
	CSSColourButton m_button2;
	CSSColourButton m_button3;
	CSSColourButton m_button4;
	CSSColourButton m_button5;
	CSSColourButton m_button6;
	CSSColourButton m_button7;
	CSSColourButton m_button8;
	CSSColourButton m_button9;
	CSSColourButton m_button0;
	CSSColourStatic m_staticDisplay;
	CSSColourStatic m_staticPrompt;
};

//*******************************************************************