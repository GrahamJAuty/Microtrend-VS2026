//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\TrackedFile.h"
#include "TrafficLightTrayicon.h"
//$$******************************************************************

class CSPosLoyalty4ServerIconDlg : public CDialogEx
{
public:
	CSPosLoyalty4ServerIconDlg(CWnd* pParent = nullptr);

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
	void HandleTimer1();

public:
	bool m_bGotPCId;
	CString m_strRunOnceTimeStamp;
	bool m_bWantRunOnceQuit;

private:
	CTrackedFile m_TrackedFileActive;
	CTrackedFile m_TrackedFileRunOnce;
	CTrafficLightTrayIcon m_trayIcon;
	UINT m_hTimer;
	bool m_bInTimer;
	bool m_bModalBusy;
	
private:
	CTime m_timeActiveFileModified;
	bool m_bGotActiveFileTime;
	bool m_bYellowGreenFlash;
	bool m_bGreenTime;
};

//$$******************************************************************
