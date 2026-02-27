#pragma once
//*******************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "Status.h"
//*******************************************************************
#include "PostTransactionThreadManager.h"
//*******************************************************************

class CWaitPostTransactionDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CWaitPostTransactionDlg)

public:
	CWaitPostTransactionDlg(int nAction, CWnd* pParent);
	virtual ~CWaitPostTransactionDlg();

	//INTENTIONALLY USING SAME ID AS WAITSTATUS DLG
	enum { IDD = IDD_WAITSTATUS };

public:
	bool GetBoolResult() { return m_bBoolResult; }
	int GetIntResult() { return m_nIntResult; }
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	void HandleTimer();

private:
	CSSColourStatic m_staticPrompt;

private:
	CPostTransactionThreadManager m_PostTransactionThreadManager;

private:
	bool m_bBoolResult = FALSE;
	int m_nIntResult = 0;

private:
	UINT m_hTimer = 0;
	bool m_bInTimer = FALSE;
};

//*******************************************************************