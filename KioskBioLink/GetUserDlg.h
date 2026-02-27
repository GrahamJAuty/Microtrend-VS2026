#pragma once
//*********************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "MyMifare.h"
#include "ServiceTimeData.h"
#include "Status.h"
#include "TerminalData.h"
//*********************************************************************

class CGetUserDlg : public CSSColourDialog
{
public:
	CGetUserDlg(int nGetUserType, bool bIDReaderOffline, bool bHaveInputFile, CWnd* pParent);
	~CGetUserDlg();
	void AllowTimeoutCancel ( bool bAllowed ) { m_bTimeoutCancel = bAllowed; }

// Dialog Data
	enum { IDD = IDD_GETUSERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonManual();
	afx_msg void OnButtonCancel();

	DECLARE_MESSAGE_MAP()

private:
	void StartTimer();
	void StopTimer();
	void HandleTimer();
	void ShowDateTime();
	void DisplayPrompt();
	void SetPromptInfoText(const CString& strText);
	int CheckServerRunning();
	bool IsKioskInService();
	void HandleManualInternal(bool& bShownKeypad);
	void SetButtonColours();

public:
	bool m_bCommandFileFound = FALSE;
	bool m_bKioskInService = FALSE;
	bool m_bShowManual = TRUE;
	bool m_bManualInput = FALSE;
	int m_nServerStatus = nSTATUS_UNKNOWN;	
	bool m_bIDReaderOffline = FALSE;
	CString m_strMiFareNo = "";

private:
	UINT m_hTimer = 0;
	int m_nGetUserType = nGETUSER_PINPAD_ONLY;
	bool m_bHaveInputFile = FALSE;
	bool m_bTimeoutCancel = FALSE;

	CMyMifare m_reader;
	CTerminalData m_terminal;

	LONGLONG m_nLastMessageTime = 0;
	LONGLONG m_nLastServerTime = 0;
	LONGLONG m_nLastFlushTime = 0;
	LONGLONG m_nLastCancelTime = 0;
	LONGLONG m_nLastSwapMessageTime = 0;

	int m_nServerInterval = 0;
	int m_nMessageInterval = nINTERVAL_MESSAGE;
	int m_nServerIntervalStandard = nINTERVAL_SERVER_STD;
	int m_nServerIntervalError = nINTERVAL_SERVER_ERROR;
	int m_nCancelInterval = nINTERVAL_SERVER_CANCEL;
	int m_nFlushInterval = nINTERVAL_FLUSH;
	bool m_bDoneTimer = FALSE;

	bool m_bPaylinkFlushed = FALSE;
	
	CServiceTimeData m_ServiceTimes;
	bool m_bKioskLastInService = FALSE;
	bool m_bKioskPendingService = FALSE;
	bool m_bKioskPendingBalanceOnly = FALSE;
	bool m_bAlternatePromptKiosk = FALSE;
	CString m_strCurrentPromptInfoText = "";

	CSSColourStatic m_staticPromptInfo;
	CSSColourStatic m_staticPrompt2;

	CSSColourStatic m_staticDateTime;
	CString m_strStaticDateTimeText = "";
	CString m_strCurrentDateTimeText = "";

	CSSColourButton m_buttonManual;
	CSSColourButton m_buttonCancel;

	bool m_bInTimer = FALSE;
};

//*********************************************************************
