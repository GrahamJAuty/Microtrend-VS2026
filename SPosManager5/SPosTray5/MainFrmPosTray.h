#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "PosTrayCashRSPBuffer.h"
#include "PosTrayTaskInfo.h"
#include "PosTrayNetworkInfo.h"
#include "PosTraySocketServer.h"
#include "PosTrayStatusDlg.h"
#include "TrayIcon.h"
/**********************************************************************/

class CMainFramePosTray : public CFrameWnd
{
public:
	CMainFramePosTray();
	virtual ~CMainFramePosTray();

protected:
	DECLARE_DYNAMIC(CMainFramePosTray)

	//{{AFX_MSG(CMainFramePosTray)
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTrayClose();
	afx_msg void OnTrayHelp();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTrayStatus();
	//}}AFX_MSG
	afx_msg long OnStartPosTray ( WPARAM wIndex, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

private:
	void InitialiseSystem();
	void InitialiseSystemInternal();
	void CloseTray( bool bWarn );
	
private:
	void UpdateStatus();
	bool UpdateStatusEmailTask();
	bool UpdateStatusFNBExport();
	bool UpdateStatusChartwellsExport();
	bool UpdateStatusS4LabourExport();
	bool UpdateStatusLIPAExport();
	bool UpdateStatusBackupTask();
	void PerformEmailTask();
	void PerformFNBExport();
	void PerformChartwellsExport();
	void PerformS4LabourExport();
	void PerformLIPAExport();
	void PerformBackupTask();
	void MarkS4LabourLocIDDuplicates();

private:
	void SetStatus( int nNewStatus );

private:
	void UpdateTime( COleDateTime& time, int nMinute, int nSecond, bool bMod5 );
	void SetNextCheckNotAfter(COleDateTime timeNow, int nMinutes, int nSeconds, COleDateTime& timeNextCheck);

private:
	void UpdateNetworkList();
	void UpdateTaskList();
	void UpdateFNBList();
	void UpdateChartwellsList();
	void UpdateBackupList();
	void UpdateS4LabourList();
	void UpdateLIPAList();
	void UpdateGeneralList();
	void GetStatusLine( CString& strStatus );
	
private:
	const char* GetFormattedTimeString( COleDateTime& timeDue );

public:
	void DisplayCashRSPStatus( const char* sz );

private:
	CTrayIcon m_trayIcon;
	CSSFile fileRunOnce;
	CString m_strMiscFolder;
	CString m_strFilePathRunOnce;
	CString m_strFilePathReport;
	CString m_strFilePathParams;

private:
	UINT m_hTimer;
	HICON m_hIcon;
	int m_nPosTrayStatus;
	
	bool m_bUIBusy;

	bool m_bReadyToAct;
	bool m_bReadyToActSalesImporter;
	bool m_bReadyToActEmail;
	bool m_bReadyToActPrint;
	bool m_bReadyToActExport;
	bool m_bReadyToActFNBExport;
	bool m_bReadyToActChartwellsExport;
	bool m_bReadyToActS4LabourExport;
	bool m_bReadyToActFTPExport;
	bool m_bReadyToActBackup;
	bool m_bReadyToActStatus;
	bool m_bWantQuit;
	bool m_bBlockHibernation;

	bool m_bTryFNBExportFirst;

private:
	COleDateTime m_NextCheckTime;
	COleDateTime m_NextEmailTime;
	COleDateTime m_NextPrintTime;
	COleDateTime m_NextExportTime;
	COleDateTime m_NextFNBExportTime;
	COleDateTime m_NextChartwellsExportTime;
	COleDateTime m_NextS4LabourExportTime;
	COleDateTime m_NextFTPExportTime;
	COleDateTime m_NextBackupTime;
	COleDateTime m_NextStatusTime;
	COleDateTime m_CurrentStatusTime;
	CString m_strFormattedTime;

private:
	CPosTrayCashRSPBuffer m_CashRSPBuffer;
	CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo> m_arrayNetworkInfo;
	CArray<CPosTrayEmailScheduleInfo,CPosTrayEmailScheduleInfo> m_arrayEmailTasks;
	CArray<CPosTrayFNBScheduleInfo,CPosTrayFNBScheduleInfo> m_arrayFNBExports;
	CArray<CPosTrayChartwellsScheduleInfo, CPosTrayChartwellsScheduleInfo> m_arrayChartwellsExports;
	CArray<CPosTrayS4LabourScheduleInfo,CPosTrayS4LabourScheduleInfo> m_arrayS4LabourExports;
	CPosTrayLIPAScheduleInfo m_LIPAExport;
	CPosTrayBackupScheduleInfo m_BackupTask;
	int m_nEmailTaskToCheck;
	int m_nEmailTaskToPerform;
	int m_nFNBExportToCheck;
	int m_nFNBExportToPerform;
	int m_nS4LabourExportToCheck;
	int m_nS4LabourExportToPerform;
	int m_nChartwellsExportToPerform;

private:
	CString m_strProgramName;
	CPosTrayStatusDlg* m_pStatusDlg;

private:
#ifdef POSTRAY_SOCKET
	CPosTraySocketServer m_SocketServer;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
