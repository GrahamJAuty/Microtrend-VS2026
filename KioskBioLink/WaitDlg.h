#pragma once
//*******************************************************************
#include "cardlink.h"
//*******************************************************************

class CWaitDlg : public CSSColourDialog
{
public:
	CWaitDlg( const char* szTitle, const char* szPrompt, CWnd* pParent);  
	virtual ~CWaitDlg();
	void PumpMessages();

	enum { IDD = IDD_WAITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strTitle = "";
	CString m_strPrompt = "";
	CSSColourStatic m_staticTitle;
	CSSColourStatic m_staticPrompt;
};

//*******************************************************************
