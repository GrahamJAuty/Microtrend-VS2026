#pragma once
//$$******************************************************************
#include "afxcmn.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "BackupResultDlg.h"
#include "ColumnManager.h"
#include "SQLRowSetAccountList.h"
//$$******************************************************************

class CLoyaltyManagerDlg : public CSSDialog
{
public:
	CLoyaltyManagerDlg(CWnd* pParent = NULL);
	~CLoyaltyManagerDlg();

	enum { IDD = IDD_LOYALTYMANAGER_DIALOG };

public:
	CSSListCtrlVirtual m_listAccounts;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	CButton m_buttonAdd;
	CButton m_buttonEdit;
	CButton m_buttonDelete;
	CButton m_buttonHotlist;
	CButton m_buttonTransfer;

private:
	HICON m_hIcon;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	LONG OnColumnClick ( WPARAM wParam, LONG lParam );
	LONG OnDemoWarning(WPARAM wParam, LONG lParam);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer( UINT nIDEvent );
	/*****/
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonHotlist();
	afx_msg void OnButtonTransfer();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRestore();
	/*****/
	afx_msg void OnFileUpgrade();
	afx_msg void OnFileRefresh();
	afx_msg void OnFileSignoff();
	afx_msg void OnFileServerService();
	afx_msg void OnFileServerServiceLog();
	afx_msg void OnFileExit();
	/*****/
	afx_msg void OnBackupSystem();
	afx_msg void OnRestoreExisting();
	afx_msg void OnRestoreSeparate();
	afx_msg void OnRestoreScript();
	afx_msg void OnBackupLog();
	afx_msg void OnBackupStatus();
	/*****/
	afx_msg void OnColumnEdit();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	/*****/
	afx_msg void OnReportsDatabase();
	afx_msg void OnReportsPhotoIDList();
	afx_msg void OnReportsHotlist();
	afx_msg void OnReportsBirthdays();
	afx_msg void OnReportsAuditAudit();
	afx_msg void OnReportsAuditCashTaken();
	afx_msg void OnReportsAuditPurseAdditions();
	afx_msg void OnReportsAuditCardSales();
	afx_msg void OnReportsAuditPointsAudit();
	afx_msg void OnReportsAuditPointsAdded();
	afx_msg void OnReportsAuditPointsDeducted();
	afx_msg void OnReportsAuditBonusRevalue();
	afx_msg void OnReportsAuditBonusSpend();
	afx_msg void OnReportsAuditBonusRedeem();
	afx_msg void OnReportsAuditRefunds();
	afx_msg void OnReportsAuditSummary();
	afx_msg void OnReportsAuditArchiveSearch();
	afx_msg void OnReportsPurchaseHistory();
	afx_msg void OnReportsPurchaseRanking();
	afx_msg void OnReportsCardUsageActive();
	afx_msg void OnReportsCardUsageInactive();
	afx_msg void OnReportsCardUsageNeverUsed();
	afx_msg void OnReportsCardUsageLowPoints();
	afx_msg void OnReportsCardUsageRankingPoints();
	afx_msg void OnReportsCardUsageLowSpend();
	afx_msg void OnReportsCardUsageRankingSpend();
	afx_msg void OnReportsCardUsageDiffer();
	afx_msg void OnReportsCardUsageTotaliser();
	afx_msg void OnReportsStartingBalance();
	afx_msg void OnReportsGroupPeriodSales();
	afx_msg void OnReportsCashlessPosting();
	afx_msg void OnReportsCashlessLiability();
	afx_msg void OnReportsStampOffer();
	/*****/
	afx_msg void OnBatchAdd();
	afx_msg void OnBatchUpdate();
	afx_msg void OnBatchRedeem();
	afx_msg void OnBatchDelete();
	/*****/
	afx_msg void OnFileImport();
	afx_msg void OnFileExport();
	afx_msg void OnFileArchive();
	afx_msg void OnFileStaffGift();
	/*****/
	afx_msg void OnSetupSystem();
	afx_msg void OnSetupServer();
	afx_msg void OnSetupGroups();
	afx_msg void OnSetupStaffCards();
	afx_msg void OnSetupStampOffers();
	/*****/
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpLicence();
	
	/*****/
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);	
	afx_msg void OnPopupTopupPurse1();
	afx_msg void OnPopupTopupPurse2();
	afx_msg void OnPopupAlert();
	afx_msg void OnPopupHotlist();
	afx_msg void OnPopupHotlistReplace();
	afx_msg void OnPopupReplace();
	afx_msg void OnPopupUnhotlist();
	afx_msg void OnPopupTrace();
	afx_msg void OnPopupPhotoID();
	afx_msg void OnPopupPurchaseHistory();
	afx_msg void OnPopupAudit();
	afx_msg void OnPopupAuditSummary();
	afx_msg void OnPopupStampOffer();
	/*****/
	afx_msg long OnAutoBackup(WPARAM, LPARAM);

private:
	void ColumnClickCardNo();
	void ColumnClickUserName();
	void ExitProgram();
	bool ExitAllowed();
	/*****/
	void AccumulatorReport ( const char* szLabel );
	void DatabaseReport ( const char* szLabel );
	void ShowAuditReport ( const char* szLabel, __int64 nUserID );
	/*****/
	void AddRecord(__int64 nCardNo, int nAppNo, const char* szComment, bool& bSuccess, int& nNewRecordIndex);
	void EditRecord (__int64 nCardNo );
	void DeleteRecord ( CSQLRowAccountFull& RowAccount, const char* szComment = "" );
	/*****/
	void BatchRecordUpdate ( int nUpdateType );
	void HandlePopupTopup(bool bPurse2);
	/*****/
	void SetupSystemInternal();
	void SetupServerInternal();
	/*****/
	void HotlistReplaceCard(CSQLRowAccountFull& RowAccount);
	void HotlistCard(CSQLRowAccountFull& RowAccount);
	void ProcessHotlistCard(CSQLRowAccountFull& RowAccount);
	void ProcessUnhotlistCard(CSQLRowAccountFull& RowAccount);
	
private:
	void SetMenuOptions();
	void UpdateTitle();

	void GetPassword ( bool bStartUp );
	
	void ShowBackupResult(CWnd* pParent, bool bDestroyOld = FALSE);
	
	void ExportData ( const char* szLabel );
	void ExportDatabase();
	
	void AutoRefresh();
	__int64 RefreshDatabase();

private:
	bool CheckHaveAccountRows();
	__int64 RefreshAccountList(bool bReloadRows);
	__int64 RefreshAccountList(__int64 nUserID, bool bReloadRows);
	__int64 GetSelectedListUserID();

private:
	void HandleFileRestore(bool bFolder, bool bFromScript);
	int RestoreInternal(CString& strError, bool& bComplete, CLockManagerInfo& infoLock, bool bRestoreToFolder, bool bFromScript);
		
private:
	CMenuChanger m_MenuChanger;
	CColumnManager m_ColumnManager;

private:
	UINT m_hTimer;
	bool m_bDoingAutoRefresh;
	bool m_bDoingAutoBackupCheck;
	bool m_bBlockAutoRefresh;
	COleDateTime m_oleLastAutoRefreshTime;
	bool m_bTitleShowsCannotAccess;
	
private:
	bool m_bInitDialog;
	CString m_strNextDefaultCardNo;
	__int64 m_nNextDefaultUserID;
	CString m_strCurrentInfo1Text;
	CString m_strCurrentInfo2Text;
	bool m_bShowRestoreFromScript;

private:
	CSQLRowSetAccountList m_SQLRowSetAccountList;
	CSQLRowAccountFull m_SQLRowContextMenu;

private:
	CBackupResultDlg* m_pBackupResultDlg;
	COleDateTime m_oleLastAutoBackupCheck;
};

//$$******************************************************************
