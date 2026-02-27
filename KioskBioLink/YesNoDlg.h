#pragma once
//*******************************************************************
#include "Cardlink.h"
//*******************************************************************

class CYesNoDlg : public CSSColourDialog
{
public:
	CYesNoDlg( const char* szPrompt, CWnd* pParent);  

	enum { IDD = IDD_YESNODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonYes();
	afx_msg void OnButtonNo();

	DECLARE_MESSAGE_MAP()

public:
	int m_nReply = IDNO;

private:
	CString m_strPrompt = "";
	CSSColourButton m_buttonYes;
	CSSColourButton m_buttonNo;
	CSSColourButton m_buttonCancel;
	CSSColourStatic m_staticPrompt;
};

//*******************************************************************