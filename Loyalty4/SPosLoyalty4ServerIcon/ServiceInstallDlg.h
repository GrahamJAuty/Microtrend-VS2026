#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "../SPosLoyalty4Shared/ServiceOptions.h"
#include "../SPosLoyalty4Shared/TrackedFile.h"
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
	CButton m_buttonLaunch;
	CStatic m_staticActivity;
	CButton m_checkDesktopApp;
	CEdit m_editActiveID;
	CEdit m_editActiveTime;
	CComboBox m_comboIconBorder;
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnToggleThisPC();
	afx_msg void OnToggleStop();
	afx_msg void OnButtonRunning();
	afx_msg void OnToggleTime();
	afx_msg void OnToggleActivity();
	afx_msg void OnToggleDesktopApp();
	afx_msg void OnButtonLaunch();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSelectIconBorder();
	DECLARE_MESSAGE_MAP()

private:
	void DisplayPCId( __int64 nId );
	void DisplayError( CString strAction, CString strError );
	void UpdateActiveTime( bool bForce = FALSE );
	void UpdateLaunchOptions();
	
private:
	__int64 m_nPreviousPCId;
	bool m_bServiceInstalled;
	bool m_bServiceRunning;
	UINT m_hTimer;
	bool m_bInTimer;
	bool m_bFirstCheckActive;
	bool m_bFirstCheckError;
	bool m_bShowError;

private:
	CServiceOptions* m_pServiceOptions;
	CString m_strServiceStopFilename;
	
private:
	CTrackedFile m_TrackedFileActive;
	CTrackedFile m_TrackedFileError;
	CString m_strTimeActive;
	CString m_strInfoActive;
	CString m_strTimeError;
	CString m_strInfoError;
};

//$$******************************************************************
