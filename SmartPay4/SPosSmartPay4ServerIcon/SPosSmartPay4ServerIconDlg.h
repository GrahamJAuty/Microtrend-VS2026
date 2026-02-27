//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\TrackedFile.h"
#include "TrafficLightTrayicon.h"
//$$******************************************************************

class CSPosSmartPay4ServerIconDlg : public CDialogEx
{
public:
	CSPosSmartPay4ServerIconDlg(CWnd* pParent = nullptr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAbout();
	afx_msg void OnStatus();
	afx_msg void OnViewLog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	CTrackedFile m_TrackedFileActive;
	CTrackedFile m_TrackedFileRunOnce;
	CTrafficLightTrayIcon m_trayIcon;
	UINT m_hTimer = NULL;
	bool m_bInTimer = FALSE;
	bool m_bModalBusy = FALSE;

#ifdef BGNDICON
	CTrackedFile m_TrackedFileBusy;
#endif

public:
	bool m_bGotPCId = FALSE;
	CString m_strRunOnceTimeStamp = "";
	bool m_bWantRunOnceQuit = FALSE;

private:
	CTime m_timeActiveFileModified;
	bool m_bGotActiveFileTime = FALSE;
	bool m_bYellowGreenFlash = FALSE;
	bool m_bRedGreenFlash = FALSE;
	bool m_bGreenTime = FALSE;

#ifdef BGNDICON
	CTime m_timeBusyFileModified;
	bool m_bGotBusyFileTime = FALSE;
#endif
};

//$$******************************************************************
