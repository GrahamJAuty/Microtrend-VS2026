#pragma once
//****************************************************************************
#include "cardlink.h"
//****************************************************************************

class CErrorDlg : public CSSColourDialog
{
public:
	CErrorDlg ( const char* szTitle, const char* szPrompt, CWnd* pParent); 
	virtual ~CErrorDlg();

// Dialog Data
	enum { IDD = IDD_ERRORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strTitle = "";
	CString m_strPrompt = "";
	CSSColourButton m_buttonClose;
	CSSColourStatic m_staticTitle;
	CSSColourStatic m_staticPrompt;
};

//****************************************************************************
