#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "../CommonEcrStock/BackupResultDlg.h"
#include "../CommonEcrStock/EposTaskDatabase.h"
/**********************************************************************/
#include "../CommonEcrStockTray/MyMessageBoxDlg.h"
/**********************************************************************/
#include "LockManager.h"
/**********************************************************************/

class CMainFrameEcrman : public CFrameWnd
{
protected: // create from serialization only
	CMainFrameEcrman();
	DECLARE_DYNCREATE(CMainFrameEcrman)
	
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrameEcrman)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	//}}AFX_VIRTUAL

public:
	virtual ~CMainFrameEcrman();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

protected:
	//{{AFX_MSG(CMainFrameEcrman)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnAppExit();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnAdminPasswords();
	afx_msg void OnSetupSystem();
	afx_msg void OnSetupPMSOptions();
	afx_msg void OnSetupPosTrayOptions();
	afx_msg void OnSetupIDraughtOptions();
	afx_msg void OnSetupSageOptions();
	afx_msg void OnSetupNetworkResources();
	afx_msg void OnSetupOptions();
	afx_msg void OnDatabaseEditData();
	afx_msg void OnSetupBarcodeNumbers();
	afx_msg void OnSetupTexts();
	afx_msg void OnFileBackup();
	afx_msg void OnFileBackupLog();
	afx_msg void OnFileBackupStatus();
	afx_msg void OnFileRestoreNormal();
	afx_msg void OnFileRestoreFolder();
	afx_msg void OnFileSageLog();
	afx_msg void OnFileWriteLog();
	afx_msg long OnIDraughtExport( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnIDraughtTransfer( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnSageExport( WPARAM wIndex, LPARAM lParam );
	afx_msg void OnReportsEpos();
	afx_msg void OnReportsWorking();
	afx_msg void OnReportsSalesHistoryPlu();
	afx_msg void OnReportsSalesHistoryPluRegion();
	afx_msg void OnReportsSalesHistoryPluDateRange();
	afx_msg void OnReportsCustomer();
	afx_msg void OnReportsCustomerStatement();
	afx_msg void OnReportsPluStock();
	afx_msg void OnReportsBritannia();
	afx_msg void OnReportsIDraught();
	afx_msg void OnAdminDeleteReports();
	afx_msg void OnAdminDeletePluUnsold();
	afx_msg void OnAdminDeleteBarcodes();
	afx_msg void OnSetupBarcode();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileCommsLogSystem();
	afx_msg void OnFileEmailLog();
	afx_msg void OnFilePosTrayLogAction();
	afx_msg void OnFilePosTrayLogExport();
	afx_msg void OnFileFNBLog();
	afx_msg void OnFileS4LabourLog();
	afx_msg void OnFileFTPLog();
	afx_msg void OnSetupTasks();
	afx_msg void OnTasksAdhoc();
	
	afx_msg void OnJobPreset1();
	afx_msg void OnUpdateJobPreset1(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset2();
	afx_msg void OnUpdateJobPreset2(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset3();
	afx_msg void OnUpdateJobPreset3(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset4();
	afx_msg void OnUpdateJobPreset4(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset5();
	afx_msg void OnUpdateJobPreset5(CCmdUI* pCmdUI);

	afx_msg void OnJobPreset6();
	afx_msg void OnUpdateJobPreset6(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset7();
	afx_msg void OnUpdateJobPreset7(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset8();
	afx_msg void OnUpdateJobPreset8(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset9();
	afx_msg void OnUpdateJobPreset9(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset10();
	afx_msg void OnUpdateJobPreset10(CCmdUI* pCmdUI);

	afx_msg void OnJobPreset11();
	afx_msg void OnUpdateJobPreset11(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset12();
	afx_msg void OnUpdateJobPreset12(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset13();
	afx_msg void OnUpdateJobPreset13(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset14();
	afx_msg void OnUpdateJobPreset14(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset15();
	afx_msg void OnUpdateJobPreset15(CCmdUI* pCmdUI);

	afx_msg void OnJobPreset16();
	afx_msg void OnUpdateJobPreset16(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset17();
	afx_msg void OnUpdateJobPreset17(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset18();
	afx_msg void OnUpdateJobPreset18(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset19();
	afx_msg void OnUpdateJobPreset19(CCmdUI* pCmdUI);
	afx_msg void OnJobPreset20();
	afx_msg void OnUpdateJobPreset20(CCmdUI* pCmdUI);

	afx_msg void OnUpdateTasksAdhoc(CCmdUI* pCmdUI);
	afx_msg void OnSetupCommsSettings();
	afx_msg void OnFileCommsLogNetwork();
	afx_msg void OnImportSQL();
	afx_msg void OnHelpAbout(); 
	afx_msg void OnHelpLicenceDetails();
	afx_msg long OnAutoBackup( WPARAM, LPARAM );
	afx_msg long OnStartupMsg(WPARAM, LPARAM);
	
	afx_msg void OnLoyalty();
	afx_msg void OnSmartPay();
	afx_msg void OnPMSCalendar();

	afx_msg void OnSpecialKeyA();
	afx_msg void OnSpecialKeyB();
	afx_msg void OnSpecialKeyC();
	afx_msg void OnSpecialKeyD();
	afx_msg void OnSpecialKeyE();
	afx_msg void OnSpecialKeyF();
	afx_msg void OnSpecialKeyG();
	afx_msg void OnSpecialKeyH();
	afx_msg void OnSpecialKeyI();
	afx_msg void OnSpecialKeyJ();
	afx_msg void OnSpecialKeyK();
	afx_msg void OnSpecialKeyL();
	afx_msg void OnSpecialKeyM();
	afx_msg void OnSpecialKeyN();
	afx_msg void OnSpecialKeyO();
	afx_msg void OnSpecialKeyP();
	afx_msg void OnSpecialKeyQ();
	afx_msg void OnSpecialKeyR();
	afx_msg void OnSpecialKeyS();
	afx_msg void OnSpecialKeyT();
	afx_msg void OnSpecialKeyU();
	afx_msg void OnSpecialKeyV();
	afx_msg void OnSpecialKeyW();
	afx_msg void OnSpecialKeyX();
	afx_msg void OnSpecialKeyY();
	afx_msg void OnSpecialKeyZ();
	afx_msg void OnSpecialKey0();
	afx_msg void OnSpecialKey1();
	afx_msg void OnSpecialKey2();
	afx_msg void OnSpecialKey3();
	afx_msg void OnSpecialKey4();
	afx_msg void OnSpecialKey5();
	afx_msg void OnSpecialKey6();
	afx_msg void OnSpecialKey7();
	afx_msg void OnSpecialKey8();
	afx_msg void OnSpecialKey9();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL DestroyWindow();

public:
	bool CheckSystem();
	void CheckPMSSystem();
	void DisableMenus();
	void SetPasswordMenuAccess();
	bool CheckForLegacySystem();
	void SetTitleBarText();
	void ShowBackupResult( CWnd* pParent, bool bDestroyOld = FALSE );
	CBackupResultDlg* GetBackupResultDlg(){ return m_pBackupResultDlg; }
	CMyMessageBoxDlg* GetStartupMsgDlg() { return m_pStartupMsgDlg; }

	void Housekeeping();
	void CheckNetworks();
	void RefreshLayout();

	void BuildJobMenu();
	
	void ProcessSpecialKey( const char* sz );
	void ProcessHiddenOptions();
	void ProcessRebuildHistory();
	void ProcessSetPluLength();
	void ProcessCustomerAutoDelete();
	void ProcessSetupLayout();
	void ProcessSetupLists();
	void HandleFileRestore( bool bFolder );
	
private:
	void EditPosTrayOptions();
	void EditCommsSettings();
	void UpdateTaskMenuAccess( CCmdUI* pCmdUI, int nTaskNo );
	void DoPresetTask( int nType );
	void ShowNetworkCommsLog( int nNetworkIdx );
	
private:
	void SetupBarcodeInternal();
	int RestoreInternal( CString& strError, bool& bComplete, bool& bTerminatedLoyalty, CLockManagerInfo& infoLock, bool bRestoreToFolder );
	int ImportSQLInternal();
	void CheckForLegacySystemInternal( bool& bAttempted, DWORD& dwResult );
	bool SpecialKeyValidationCheck();

	int ManagePMSInternal();

private:
	UINT m_hTimerIdle;
	CMenuChanger m_MenuChanger;
		
private:
	int m_arrayPresetTaskIdx[10];

private:
	CBackupResultDlg* m_pBackupResultDlg;
	CMyMessageBoxDlg* m_pStartupMsgDlg;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
#endif 
/**********************************************************************/
