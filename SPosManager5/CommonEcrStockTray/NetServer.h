#pragma once
/**********************************************************************/

class CNetCommsServer : public CWnd
{
public:
	CNetCommsServer();

	bool InitialiseComms( const char* szNetworkPath, CWnd* pParent );
	void TerminateComms();

	void EnableComms();
	void DisableComms();

	const char* GetError() { return m_strError; }

protected:
	virtual void Timer(){}

private:	
	void LogCommsMessage( bool bLineBefore, bool bLineAfter, const char* szMessage, const char* szError );

private:
	CString m_strNetworkPath;
	CString m_strError;
	UINT m_hTimer;

private:
	bool m_bEnableComms;
	bool m_bCreatedWindow;
	bool m_bPauseComms;
	static bool m_bTimerRunning;

protected:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
