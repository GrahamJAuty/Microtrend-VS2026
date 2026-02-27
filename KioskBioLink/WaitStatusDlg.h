#pragma once
//*******************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "Status.h"
//*******************************************************************
#include "StatusCheckThreadManager.h"
//*******************************************************************

class CWaitStatusDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CWaitStatusDlg)

public:
	CWaitStatusDlg(CStatus& status, int nAction, bool bMakeVisible, CWnd* pParent);
	virtual ~CWaitStatusDlg();

	enum { IDD = IDD_WAITSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	void HandleTimer();

public:
	int GetIntResult() { return m_nIntResult; }
	bool GetBoolResult() { return m_bBoolResult; }

private:
	CSSColourStatic m_staticPrompt;

private:
	CStatusCheckThreadManager m_StatusCheckThreadManager;

private:
	UINT m_hTimer = 0;
	bool m_bInTimer = FALSE;
	bool m_bMakeVisible = TRUE;

private:
	int m_nIntResult = 0;
	bool m_bBoolResult = FALSE;
};

//*******************************************************************