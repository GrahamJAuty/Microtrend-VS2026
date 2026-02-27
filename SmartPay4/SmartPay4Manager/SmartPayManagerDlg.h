#pragma once
//$$******************************************************************
#include <time.h>
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//$$******************************************************************
#include "BackupResultDlg.h"
#include "ColumnManager.h"
#include "SmartPay4Manager.h"
#include "SQLRowSetAccountList.h"
//$$******************************************************************

class CSmartPayManagerDlg : public CSSAutoShutdownDialog
{
public:
	CSmartPayManagerDlg( CWnd* pParent = NULL);
	~CSmartPayManagerDlg();

	enum { IDD = IDD_SMARTPAYMANAGER_DIALOG };
	CButton m_buttonAdd;
	CButton m_buttonDelete;
	CButton m_buttonTransfer;
	CButton m_buttonAlert;

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonTransfer();
	afx_msg void OnButtonAlert();
	afx_msg void OnButtonRestore();
	afx_msg void OnSetupServer();
	afx_msg void OnHelpSmartPayManager();
	afx_msg void OnHelpLicencedetails();
	afx_msg void OnFileExit();
	afx_msg void OnFileRefresh();
	afx_msg void OnReportsDatabase();
	afx_msg void OnReportsBonusRevalue();
	afx_msg void OnReportsBonusSpend();
	afx_msg void OnFileSignoff();
	afx_msg void OnReportsAudit();
	afx_msg void OnReportsAuditSummary();
	afx_msg void OnReportsGroupShiftLog();
	afx_msg void OnSetupSystem();
	afx_msg void OnSetupGroups();
	afx_msg void OnSetupPurchaseControl();
	afx_msg void OnReportsBirthdays();
	afx_msg void OnReportsPurchaseHistory();
	afx_msg void OnReportsPurchaseControls();
	afx_msg void OnFileImport();
	afx_msg void OnFileExport();
	afx_msg void OnReportsStartingBalance();
	afx_msg void OnBatchUpdate();
	afx_msg void OnBatchAdd();
	afx_msg void OnBatchRedeem();
	afx_msg void OnBatchDelete();
	afx_msg void OnReportsPhotoIDList();
	afx_msg void OnReportsCashTaken();
	afx_msg void OnReportGroupPeriodSales();
	afx_msg void OnFileWebPaymentImport();
	afx_msg void OnWebPaymentLog();
	afx_msg void OnFileTerminallist();
	afx_msg void OnImportSimsfile();
	afx_msg void OnFileUpgrade();
	afx_msg void OnFileWebPaymentEndofDay();
	afx_msg void OnFileWebPaymentLinkAccounts();
	afx_msg void OnPebbleBalances();
	afx_msg void OnPebbleLinkTimes();
	afx_msg void OnFileArchive();
	afx_msg void OnFileTimetable();
	afx_msg void OnFileBgndService();
	afx_msg void OnFileBgndServiceLog();
	afx_msg void OnFileServerService();
	afx_msg void OnFileServerServiceLog();
	afx_msg void OnImportAutofile();
	afx_msg void OnReportsEndofday();
	afx_msg void OnImportTopupfile();
	afx_msg void OnExportSalestopups();
	afx_msg void OnPurchasesStatements();
	afx_msg void OnEmailLowbalance();
	afx_msg void OnEmailPointsachievement();
	afx_msg void OnEmailPurchases();
	afx_msg void OnEmailWeeklypoints();
	afx_msg void OnEmailLog();
	afx_msg void OnFileWebPaymentBalances();
	afx_msg void OnImportCustomfile();
	afx_msg void OnReportsLeavers();
	afx_msg void OnImportLeavers();
	afx_msg void OnImportWondefile();
	afx_msg void OnUsageWeeklySpend();
	afx_msg void OnImportAuditdata();
	afx_msg void OnPurchasesBreakdown();
	
	afx_msg void OnAuditRedeemBonus();
	afx_msg void OnAuditEposPayments();
	afx_msg void OnAuditArchive();
	afx_msg void OnAuditKioskTopups();
	afx_msg void OnAuditPointsAdded();
	afx_msg void OnAuditPointsAudit();
	afx_msg void OnAuditPointsDeducted();
	afx_msg void OnAuditSales();
	afx_msg void OnAuditCredit();
	afx_msg void OnAuditPurseAdditions();
	afx_msg void OnAuditRefunds();

	afx_msg void OnPopupMeal();
	afx_msg void OnPopupGroupShift();
	afx_msg void OnPopupCredit();
	afx_msg void OnPopupPebbleLink();
	afx_msg void OnPopupAudit();
	afx_msg void OnPopupAuditSummary();
	afx_msg void OnPopupPurchaseControls();
	afx_msg void OnPopupStatement();
	afx_msg void OnPopupTopup();
	afx_msg void OnPopupTopup2();
	afx_msg void OnPopupTopup3();
	afx_msg void OnPopupAlert();
	afx_msg void OnPopupHotlist();
	afx_msg void OnPopupUnhotlist();
	afx_msg void OnPopupHotlistReplace();
	afx_msg void OnPopupReplace();
	afx_msg void OnPopupTrace();
	afx_msg void OnPopupPhotoID();
	afx_msg void OnPopupPurchaseHistory();

	afx_msg void OnCardUsageActiveCards();
	afx_msg void OnCardUsageInactiveCards();
	afx_msg void OnCardUsageLowPoints();
	afx_msg void OnCardUsageLowSpend();
	afx_msg void OnCardUsageRankingPoints();
	afx_msg void OnCardUsageRankingSpend();
	afx_msg void OnCardUsageNeverUsed();
	afx_msg void OnCardUsageRankingPurchases();

	afx_msg void OnColumnEdit();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();

	afx_msg void OnBackupSystem();
	afx_msg void OnRestoreExisting();
	afx_msg void OnRestoreSeparate();
	afx_msg void OnRestoreScript();
	afx_msg void OnBackupLog();
	afx_msg void OnBackupStatus();
	afx_msg long OnAutoBackup(WPARAM, LPARAM);

	LONG OnColumnClick(WPARAM wParam, LONG lParam);
	LONG OnDemoWarning(WPARAM wParam, LONG lParam);

	void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	void UpdateTitle();

	void GetPassword(bool bStartUp);
	void SetMenuOptions();
	
	void ExitProgram( bool bAutoShutdown );
	bool ExitAllowed();
	bool CheckForGroupsSetup ( CString& strErrorMsg );

	void WebPaymentExceptionsCheck();

	void OnButtonCardNo();
	void OnButtonFindCard();

	__int64 RefreshAccountList(bool bReloadRows);
	__int64 RefreshAccountList(__int64 nUserID, bool bReloadRows);
	__int64 GetSelectedListUserID();
	bool CheckHaveAccountRows();

	void ShowBackupResult(CWnd* pParent, bool bDestroyOld = FALSE);
	void HandleFileRestore(bool bFolder,bool bFromScript);
	int RestoreInternal(CString& strError, bool& bComplete, CLockManagerInfo& infoLock, bool bRestoreToFolder, bool bFromScript);

	void DisplayAccountInUse ( const char* szUserID );

	void TopupPurse ( int nPurse );

	void AddRecord (__int64 nCardNo, int nAppNo, const char* szComment, bool& bSuccess, int& nNewRecordIdx);
	void EditRecord ( __int64 nCardNo );
	void TransferRecord (__int64 nUserIDFrom );
	void ReplaceRecord (__int64 nUserIDFrom );
	void AlertRecord (__int64 nUserID);
	void ShowAuditReport ( const char* szLabel, __int64 nSingleUserID );
	void ShowPCAuditReport(const char* szLabel);
	void ShowGroupShiftReport(const char* szLabel, __int64 nSingleUserID);

	void ExportData ( const char* szLabel );
	void ExportDatabase();
	void BatchUpdate ( int nType );

	void AccumulatorReport ( const char* szLabel );
	void DatabaseReport ( const char* szLabel );

	void HandlePopupToggleHotlistUser();
	void ProcessHotlist(CSQLRowAccountFull& RowAccount);
	void ProcessUnhotlist(CSQLRowAccountFull& RowAccount);
	
	void HotlistReplace();

	void SurnameList();

	bool CheckContextRowExists();

private:
	void SetupSystemInternal();
	void SetupServerInternal();

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownTimer(UINT nIDEvent);
	virtual void CSSAutoShutdownLogOff();
	virtual void CSSAutoShutdownDestroy();
	CString GetAutoShutdownSecondsText();

private:
	CMenuChanger m_MenuChanger;

	bool m_bInitDialog;

	CSSListCtrlVirtual m_list;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	CColumnManager m_ColumnManager;

	__int64 m_nNextDefaultUserID;
	bool m_bRestartServerLater;

private:
	int m_nPendingEmailCount;
	CString m_strCurrentInfo1Text;
	CString m_strCurrentInfo2Text;
	COleDateTime m_oleTimeNextEmailCheck;
	COleDateTime m_oleTimeNextGroupShiftCheck;
	UINT m_hTimer;

	ULONGLONG m_LastFullTimerCheck;

private:
	CSQLRowAccountFull m_SQLRowContextMenu;
	CSQLRowSetAccountList m_SQLRowSetAccountList;

private:
	CBackupResultDlg* m_pBackupResultDlg;
	COleDateTime m_oleLastAutoBackupCheck;
	bool m_bDoingAutoBackupCheck;
	bool m_bShowRestoreFromScript;
};

//$$******************************************************************

