#pragma once
//*******************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "Status.h"
//*******************************************************************
#include "GetUserThreadManager.h"
//*******************************************************************

class CWaitGetUserDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CWaitGetUserDlg)

public:
	CWaitGetUserDlg(CString strFilename, bool bServerValidate, CStringArray& arrayUserLines, CUserIDRecord& UserIDRecord, CWnd* pParent);
	virtual ~CWaitGetUserDlg();

	//INTENTIONALLY USING SAME ID AS WAITSTATUS DLG
	enum { IDD = IDD_WAITSTATUS };

public:
	bool GetBoolResult() { return m_bBoolResult; }
	int GetMessageNo() { return m_nMessageNo; }
	
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
	CGetUserThreadManager m_GetUserThreadManager;

private:
	UINT m_hTimer = 0;
	bool m_bInTimer = FALSE;

private:
	bool m_bBoolResult = FALSE;
	int m_nMessageNo = nMESSAGE_NONE;
};

//*******************************************************************