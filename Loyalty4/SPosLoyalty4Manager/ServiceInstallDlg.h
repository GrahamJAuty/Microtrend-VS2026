#pragma once
//$$******************************************************************
#include "../SPosLoyalty4Shared/ServiceOptions.h"
#include "../SPosLoyalty4Shared/TrackedFile.h"
//$$******************************************************************
#include "LoyaltyManager.h"
#include "WinService.h"
//$$******************************************************************

class CControlDisplayStatus
{
public:
	CControlDisplayStatus( CWnd* pControl );

public:
	void UpdateDisplay( CSSDialog* pParent );
	void SetShowAndEnable( bool bShow, bool bEnable = TRUE );
	void SetText( const char* sz ){ m_strWantText = sz; }

private:
	CWnd* m_pControl;
	bool m_bCurrentEnable;
	bool m_bCurrentShow;
	bool m_bWantEnable;
	bool m_bWantShow;
	CString m_strCurrentText;
	CString m_strWantText;
};

//$$******************************************************************

class CServiceInstallDlg : public CSSDialog
{
public:
	CServiceInstallDlg( CWnd* pParent = NULL); 
	
	enum { IDD = IDD_SERVICE_INSTALL };

public:
	CButton m_checkThisPC;
	CEdit m_editPCId;
	CButton m_checkStop;
	CStatic m_staticInstall;
	CButton m_buttonInstall;
	CStatic m_staticRunning;
	CButton m_buttonRunning;
	CStatic m_staticActivity;
	CEdit m_editActiveID;
	CEdit m_editActiveTime;
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnToggleThisPC();
	afx_msg void OnToggleStop();
	afx_msg void OnButtonInstall();
	afx_msg void OnButtonRunning();
	afx_msg void OnToggleTime();
	afx_msg void OnToggleActivity();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	void DisplayPCId( __int64 nId );
	void UpdateInstallOptions();
	void UpdateRunningOptions( bool bStopRequest );
	void DisplayError( CString strAction, CString strError );
	void UpdateActiveTime( bool bForce = FALSE );

private:
	CWinService m_Service;
	__int64 m_nPreviousPCId;
	bool m_bServiceInstalled;
	bool m_bServiceRunning;
	UINT m_hTimer;
	bool m_bInTimer;
	bool m_bFirstCheckActive;
	bool m_bFirstCheckError;
	bool m_bShowError;

private:
	CControlDisplayStatus m_DisplayStaticInstall;
	CControlDisplayStatus m_DisplayButtonInstall;
	CControlDisplayStatus m_DisplayStaticRunning;
	CControlDisplayStatus m_DisplayButtonRunning;

private:
	CString m_strServiceStopFilename;
	CServiceOptions* m_pServiceOptions;

private:
	CTrackedFile m_TrackedFileActive;
	CTrackedFile m_TrackedFileError;
	CString m_strTimeActive;
	CString m_strInfoActive;
	CString m_strTimeError;
	CString m_strInfoError;
};

//$$******************************************************************
