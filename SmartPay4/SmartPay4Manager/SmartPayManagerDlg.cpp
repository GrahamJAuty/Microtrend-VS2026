//$$******************************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\GroupShiftValidator.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\PresentationOptions.h"
#include "..\SmartPay4Shared\PaymentAuditReport.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SystemData.h"
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\AutoImportHandler.h"
#include "..\SmartPay4ManagerBgnd\BackgroundTimeTable.h"
#include "..\SmartPay4ManagerBgnd\DatabaseExport.h"
#include "..\SmartPay4ManagerBgnd\DatabaseImport.h"
#include "..\SmartPay4ManagerBgnd\EmailLowBalances.h"
#include "..\SmartPay4ManagerBgnd\EmailPointsAchievement.h"
#include "..\SmartPay4ManagerBgnd\EmailPurchases.h"
#include "..\SmartPay4ManagerBgnd\EmailWeeklyPoints.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportBromCom.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportPebble.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentImport.h"
//$$******************************************************************
#include "AboutDlg.h"
#include "AddCashDlg.h"
#include "AlertTextSelectDlg.h"
#include "AutoBackupOptions.h"
#include "AutoBackupProcessor.h"
#include "AutoShutdownStacker.h"
#include "AuditReport.h"
#include "AuditReportWrapper.h"
#include "BatchUpdate.h"
#include "BirthdayReport.h"
#include "CardNoDlg.h"
#include "ColumnHandler.h"
#include "ColumnListDlg.h"
#include "CommentDlg.h"
#include "CreditAdjustDlg.h" 
#include "DatabaseReport.h"
#include "DatabaseAuditImport.h"
#include "DeleteAccountRecord.h"
#include "DemoWarning.h"
#include "EditRecord.h"
#include "EposTerminalsDlg.h"
#include "FindDlg.h"
#include "FontTable.h"
#include "GroupShiftDlg.h"
#include "GroupShiftReportWrapper.h"
#include "HotlistDlg.h"
#include "HotlistTraceDlg.h"
#include "ImportDataHandler.h"
#include "ImportLeaversList.h"
#include "ImportSimsDlg.h"
#include "ImportWondeDlg.h"
#include "ListDataDatabaseReportDlg.h"
#include "ListDataGroupDlg.h"
#include "ListDataLeaversDlg.h"
#include "ListDataPurchaseControlInfoDlg.h"
#include "MyMessageBoxDlg.h"
#include "OfficeMealDlg.h" 
#include "PasswordDlg.h"
#include "PasswordLogger.h"
#include "PCAuditReportWrapper.h"
#include "PCOptions.h"
#include "PhotoIDDlg.h"
#include "RecordXferPropertySheet.h"
#include "ReportAccumulator.h"
#include "ReportKioskAudit.h"
#include "ReportGroupPeriodSales.h"
#include "ReportPebbleBalances.h"
#include "ReportPebbleLinkTimes.h"
#include "ReportPhotoIDList.h"
#include "ReportPluRanking.h"
#include "ReportPurchaseBreakdown.h"
#include "ReportStartingBalance.h"
#include "ReportStatement.h"
#include "ReportWeeklySpend.h"
#include "ServiceInstallDlg.h"
#include "ServerPropertySheet.h"
#include "SmartPay4Manager.h"
#include "SQLTranBatchAdd.h"
#include "SQLTranCreatePINNumbers.h"
#include "SQLTranXferAccount.h"
#include "SQLTranUnhotlistAccount.h"
#include "PurchaseControlReportWrapper.h"
#include "SurnameListDlg.h"
#include "SystemBackup.h"
#include "SystemPropertySheet.h"
#include "UpgradeHandler.h"
#include "WebPaymentImportLogDlg.h"
#include "WebPaymentLinkAccounts.h"
//$$******************************************************************
#include "SmartPayManagerDlg.h"
//$$******************************************************************
#define	WM_USER_COLUMN		( WM_USER + 200 )
#define	WM_DEMO_WARNING		( WM_USER + 201 )
#define Y_ADJUSTMENT		16
#define X_ADJUSTMENT		105
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************
static int nColumnCount = 8;
//$$******************************************************************

CSmartPayManagerDlg::CSmartPayManagerDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSmartPayManagerDlg::IDD, pParent),
	m_ColumnManager(110)
{
	m_hTimer = NULL;

	m_nPendingEmailCount = 0;
	m_strCurrentInfo1Text = "";
	m_strCurrentInfo2Text = "";

	m_pBackupResultDlg = NULL;
	m_bDoingAutoBackupCheck = FALSE;
	m_bShowRestoreFromScript = FALSE;

	m_LastFullTimerCheck = 0;

	m_oleTimeNextEmailCheck = COleDateTime(2020, 1, 1, 0, 0, 0);
	m_oleTimeNextGroupShiftCheck = COleDateTime(2020, 1, 1, 0, 0, 0);
	m_oleLastAutoBackupCheck = COleDateTime(2011, 1, 1, 0, 0, 0);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitDialog = FALSE;

	m_nNextDefaultUserID = 0;
	m_bRestartServerLater = FALSE;

	Passwords.GetPassword(0);
}

//$$******************************************************************

CSmartPayManagerDlg::~CSmartPayManagerDlg()
{
	if (m_pBackupResultDlg != NULL)
	{
		delete m_pBackupResultDlg;
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_TRANSFER, m_buttonTransfer);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSmartPayManagerDlg, CSSAutoShutdownDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE() 
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE ( WM_USER_COLUMN, OnColumnClick )
	ON_MESSAGE ( WM_DEMO_WARNING, OnDemoWarning )

	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, OnGetDispInfoList)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFER, OnButtonTransfer)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)

	ON_COMMAND(IDM_POPUP_ADD, OnButtonAdd)
	ON_COMMAND(IDM_POPUP_EDIT, OnButtonEdit)
	ON_COMMAND(IDM_POPUP_DELETE, OnButtonDelete)
	ON_COMMAND(IDM_POPUP_TRANSFER, OnButtonTransfer)
	ON_COMMAND(IDM_POPUP_ALERT, OnPopupAlert)
	ON_COMMAND(IDM_POPUP_HOTLIST, OnPopupHotlist)
	ON_COMMAND(IDM_POPUP_UNHOTLIST, OnPopupUnhotlist)
	ON_COMMAND(IDM_POPUP_HOTLISTREPLACE, OnPopupHotlistReplace)
	ON_COMMAND(IDM_POPUP_REPLACE, OnPopupReplace)
	ON_COMMAND(IDM_POPUP_TRACE, OnPopupTrace)
	ON_COMMAND(IDM_POPUP_PURCHASEHISTORY, OnPopupPurchaseHistory)
	ON_COMMAND(IDM_POPUP_STATEMENT, OnPopupStatement)
	ON_COMMAND(IDM_POPUP_PHOTOID, OnPopupPhotoID)
	ON_COMMAND(IDM_POPUP_TOPUP, OnPopupTopup)
	ON_COMMAND(IDM_POPUP_TOPUP2, OnPopupTopup2)
	ON_COMMAND(IDM_POPUP_TOPUP3, OnPopupTopup3)
	ON_COMMAND(IDM_POPUP_PEBBLELINK, OnPopupPebbleLink)
	ON_COMMAND(IDM_POPUP_AUDIT, OnPopupAudit)
	ON_COMMAND(IDM_POPUP_AUDIT_SUMMARY, OnPopupAuditSummary)
	ON_COMMAND(IDM_POPUP_PURCHASECONTROLS, OnPopupPurchaseControls)
	ON_COMMAND(IDM_FILE_REFRESH, OnFileRefresh)
	ON_COMMAND(IDM_FILE_SIGNOFF, OnFileSignoff)
	ON_COMMAND(IDM_FILE_EXIT, OnFileExit)
	ON_COMMAND(IDM_FILE_TERMINALLIST, OnFileTerminallist)

	ON_COMMAND(IDM_BACKUP_SYSTEM, OnBackupSystem)
	ON_COMMAND(IDM_RESTORE_EXISTING, OnRestoreExisting)
	ON_COMMAND(IDM_RESTORE_SEPARATE, OnRestoreSeparate)
	ON_COMMAND(IDM_RESTORE_SCRIPT, OnRestoreScript)
	ON_COMMAND(IDM_BACKUP_LOG, OnBackupLog)
	ON_COMMAND(IDM_BACKUP_STATUS, OnBackupStatus)
	ON_MESSAGE(WM_AUTOBACKUP, OnAutoBackup)

	ON_COMMAND(IDM_SETUP_SERVER, OnSetupServer)
	ON_COMMAND(IDM_SETUP_SYSTEM, OnSetupSystem)
	ON_COMMAND(IDM_SETUP_GROUPS, OnSetupGroups)
	ON_COMMAND(IDM_SETUP_PURCHASECONTROL, OnSetupPurchaseControl)

	ON_COMMAND(IDM_HELP_SMARTPAYMANAGER, OnHelpSmartPayManager)
	ON_COMMAND(IDM_HELP_LICENCEDETAILS, OnHelpLicencedetails)

	ON_COMMAND(IDM_REPORTS_DATABASE, OnReportsDatabase)
	ON_COMMAND(IDM_REPORTS_LEAVERS, OnReportsLeavers)
	ON_COMMAND(IDM_REPORTS_AUDIT, OnReportsAudit)
	ON_COMMAND(IDM_REPORTS_AUDIT_SUMMARY, OnReportsAuditSummary)
	ON_COMMAND(IDM_REPORTS_REVALUEBONUS, OnReportsBonusRevalue)
	ON_COMMAND(IDM_REPORTS_SPENDBONUS, OnReportsBonusSpend)
	ON_COMMAND(IDM_REPORTS_BIRTHDAYS, OnReportsBirthdays)
	ON_COMMAND(IDM_REPORTS_PURCHASEHISTORY, OnReportsPurchaseHistory)
	ON_COMMAND(IDM_REPORTS_GROUPSHIFTLOG, OnReportsGroupShiftLog)
	ON_COMMAND(IDM_REPORTS_PURCHASECONTROLS, OnReportsPurchaseControls)

	ON_COMMAND(IDM_FILE_UPGRADE, OnFileUpgrade)
	ON_COMMAND(IDM_FILE_WEBPAYMENTIMPORT, OnFileWebPaymentImport)
	ON_COMMAND(IDM_FILE_WEBPAYMENTENDOFDAY, OnFileWebPaymentEndofDay)
	ON_COMMAND(IDM_FILE_WEBPAYMENTLOG, OnWebPaymentLog)

	ON_COMMAND(IDM_FILE_IMPORT, OnFileImport)
	ON_COMMAND(IDM_IMPORT_SIMSFILE, OnImportSimsfile)
	ON_COMMAND(IDM_IMPORT_TOPUPFILE, OnImportTopupfile)
	ON_COMMAND(IDM_IMPORT_CUSTOMFILE, OnImportCustomfile)
	ON_COMMAND(IDM_IMPORT_AUTOFILE, OnImportAutofile)
	ON_COMMAND(IDM_IMPORT_LEAVERS, OnImportLeavers)
	ON_COMMAND(IDM_IMPORT_WONDEFILE, OnImportWondefile)
	ON_COMMAND(IDM_IMPORT_AUDITDATA, OnImportAuditdata)
	ON_COMMAND(IDM_FILE_EXPORT, OnFileExport)

	ON_COMMAND(IDM_BATCH_UPDATE, OnBatchUpdate)
	ON_COMMAND(IDM_BATCH_ADDCARDS, OnBatchAdd)
	ON_COMMAND(IDM_BATCH_DELETE, OnBatchDelete)
	ON_COMMAND(IDM_BATCH_REDEEM, OnBatchRedeem)

	ON_COMMAND(IDM_REPORTS_PHOTOIDLIST, OnReportsPhotoIDList)
	ON_COMMAND(IDM_REPORTS_STARTINGBALANCE, OnReportsStartingBalance)
	ON_COMMAND(IDM_REPORTS_CASHTAKEN, OnReportsCashTaken)
	ON_COMMAND(IDM_REPORTS_GROUPPERIODSALES, OnReportGroupPeriodSales)
	ON_COMMAND(IDM_AUDIT_POINTSADDED, OnAuditPointsAdded)
	ON_COMMAND(IDM_AUDIT_POINTSAUDIT, OnAuditPointsAudit)
	ON_COMMAND(IDM_AUDIT_POINTSDEDUCTED, OnAuditPointsDeducted)
	ON_COMMAND(IDM_AUDIT_SALES, OnAuditSales)
	ON_COMMAND(IDM_AUDIT_CREDIT, OnAuditCredit)
	ON_COMMAND(IDM_AUDIT_PURSEADDITIONS, OnAuditPurseAdditions)
	ON_COMMAND(IDM_AUDIT_REFUNDS, OnAuditRefunds)
	ON_COMMAND(IDM_AUDIT_KIOSKTOPUPS, OnAuditKioskTopups)
	ON_COMMAND(IDM_AUDIT_REDEEMBONUS, OnAuditRedeemBonus)

	ON_COMMAND(IDM_CARDUSAGE_ACTIVECARDS, OnCardUsageActiveCards)
	ON_COMMAND(IDM_CARDUSAGE_INACTIVECARDS, OnCardUsageInactiveCards)
	ON_COMMAND(IDM_CARDUSAGE_LOWPOINTS, OnCardUsageLowPoints)
	ON_COMMAND(IDM_CARDUSAGE_LOWSPEND, OnCardUsageLowSpend)
	ON_COMMAND(IDM_CARDUSAGE_RANKINGPOINTS, OnCardUsageRankingPoints)
	ON_COMMAND(IDM_CARDUSAGE_RANKINGSPEND, OnCardUsageRankingSpend)
	ON_COMMAND(IDM_CARDUSAGE_NEVERUSED, OnCardUsageNeverUsed)
	ON_COMMAND(IDM_CARDUSAGE_RANKINGPURCHASES, OnCardUsageRankingPurchases)
	ON_COMMAND(IDM_FILE_WEBPAYMENTLINKACCOUNTS, OnFileWebPaymentLinkAccounts)
	ON_COMMAND(IDM_PEBBLE_BALANCES, OnPebbleBalances)
	ON_COMMAND(IDM_PEBBLE_LINKTIMES, OnPebbleLinkTimes)
	ON_COMMAND(IDM_FILE_ARCHIVE, OnFileArchive)
	ON_COMMAND(IDM_AUDIT_ARCHIVE, OnAuditArchive)
	ON_COMMAND(IDM_FILE_TIMETABLE, OnFileTimetable)
	ON_COMMAND(IDM_FILE_BGND_SERVICE, OnFileBgndService)
	ON_COMMAND(IDM_FILE_BGND_SERVICELOG, OnFileBgndServiceLog)
	ON_COMMAND(IDM_FILE_SERVER_SERVICE, OnFileServerService)
	ON_COMMAND(IDM_FILE_SERVER_SERVICELOG, OnFileServerServiceLog)
	ON_COMMAND(IDM_REPORTS_ENDOFDAY, OnReportsEndofday)
	ON_COMMAND(IDM_EXPORT_SALESTOPUPS, OnExportSalestopups)
	ON_COMMAND(IDM_PURCHASES_STATEMENTS, OnPurchasesStatements)
	ON_COMMAND(IDM_EMAIL_LOWBALANCE, OnEmailLowbalance)
	ON_COMMAND(IDM_EMAIL_POINTSACHIEVEMENT, OnEmailPointsachievement)
	ON_COMMAND(IDM_EMAIL_PURCHASES, OnEmailPurchases)
	ON_COMMAND(IDM_EMAIL_WEEKLYPOINTS, OnEmailWeeklypoints)
	ON_COMMAND(IDM_EMAIL_LOG, OnEmailLog)
	ON_COMMAND(IDM_FILE_WEBPAYMENTBALANCES, OnFileWebPaymentBalances)
	ON_COMMAND(IDM_USAGE_WEEKLYSPEND, OnUsageWeeklySpend)
	ON_COMMAND(IDM_AUDIT_EPOSPAYMENTS, OnAuditEposPayments)
	ON_COMMAND(IDM_PURCHASES_BREAKDOWN, OnPurchasesBreakdown)
	ON_COMMAND(IDM_POPUP_MEAL, OnPopupMeal)
	ON_COMMAND(IDM_POPUP_GROUPSHIFT, OnPopupGroupShift)
	ON_COMMAND(IDM_POPUP_CREDIT, OnPopupCredit)
	
	ON_COMMAND( IDM_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND( IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND( IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND( IDM_COLUMN_DEFAULT, OnColumnDefault)

	END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSmartPayManagerDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(::GetTidyAppName(FALSE));

	CDC* pDC = GetDC();
	FontTable.BuildFontTable(pDC);

	m_bInitDialog = TRUE;
	SetIcon(m_hIcon, TRUE);			// Set big icon

	m_list.SubclassDlgItem(IDC_LIST1, this);
	m_list.EnableColour();

	m_ColumnManager.Reset(TableNames::AccountList);
	m_ColumnManager.AddColumn(ColumnNames::UserID, "UserID", LVCFMT_LEFT, 54, 54, COLUMN_STATUS_LOCKED);
	m_ColumnManager.AddColumn(ColumnNames::UserName, "UserName", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Forename, "Forename", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Surname, "Surname", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Group, "Grp", LVCFMT_LEFT, 30, 30, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Purse1, "Purse1", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Purse2, "Purse2", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Purse3, "Purse3", LVCFMT_RIGHT, 100, 100, System.GetEnablePurse3Flag() ? COLUMN_STATUS_SHOWN : COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Points, "Points", LVCFMT_RIGHT, 100, 100, System.GetActiveEnablePointsFlag() ? COLUMN_STATUS_SHOWN : COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Balance, "Balance", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Status, "Status", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.LoadSettings();

	m_ColumnManager.RedrawList(&m_list, this);

	// setup menu changer
	m_MenuChanger.EatMenuTopLevel(GetMenu());					

	// load basic account information
	m_SQLRowSetAccountList.SelectRows(0);

	if ((System.GetUsePasswordsFlag() == TRUE) && (Passwords.InUse() == TRUE))
	{
		GetPassword(TRUE);
	}
	else
	{
		Sysset.CreateProgUserFile("Master");
		PasswordLogger.LogEvent(0, 1);
	}

	RefreshAccountList(0, FALSE);							// do first screen & set menu options

	CWaitCursor cursor;

	CImportDataHandler import(this);
	import.ScanAll();								// see if any data is waiting to be imported
	
	if (m_SQLRowSetAccountList.GetSize() == 0 )		// see if have any records
	{
		AutoShutdownHelper.SetTimerBlock1(TRUE);

		CUpgradeHandler version(this);
		if (version.DefaultUpgradePossible() == TRUE)	// no records - see if have a previous version to upgrade
		{
			if (version.Upgrade() == TRUE)			// upgrade from default folder
			{
				RefreshAccountList(TRUE);
			}
		}

		AutoShutdownHelper.SetTimerBlock1(FALSE);
	}

	WebPaymentExceptionsCheck();							// check for any web payment exceptions lurking

	m_hTimer = SetTimer(1, 1000, 0);
	
	PostMessage(WM_DEMO_WARNING);

	return TRUE;
}

//$$******************************************************************
	
void CSmartPayManagerDlg::OnColumnEdit()
{
	m_ColumnManager.RememberCurrentLine( &m_list, this );
	CArray<CColumnStatusInfo,CColumnStatusInfo> arrayPos;
	m_ColumnManager.GetListColumnDetails( arrayPos );

	CColumnListDlg dlg( "Account List", arrayPos, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_ColumnManager.SetListColumnDetails( arrayPos );
		m_ColumnManager.RedrawList( &m_list, this );
		m_ColumnManager.SaveCurrentLine( &m_list, this );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnColumnSave()
{
	m_ColumnManager.SaveCurrentLine( &m_list, this );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnColumnRestore()
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_list, this );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnColumnDefault()
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_list, this );
}

//$$******************************************************************

void CSmartPayManagerDlg::CSSAutoShutdownDestroy()
{
	if (m_hTimer != NULL)
	{
		KillTimer(m_hTimer);
	}

	if (m_pBackupResultDlg != NULL)
	{
		delete m_pBackupResultDlg;
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::WebPaymentExceptionsCheck()
{
	if (System.GetWebPaymentType() != nWEBPAYMENT_NONE)
	{
		CString strTitle = "Web Payment Validation";

		bool bIsSuspended = ::FileExists(Filenames.GetWebPaymentStopIndicator());
		bool bHaveExceptions = ::FileExists(Filenames.GetWebPaymentExceptionIndicator());

		CString strMsg = "ATTENTION:\n\n";

		if (bIsSuspended == TRUE)												// see if background webpayment has been stopped
		{
			strMsg += "Web Payments have been suspended due to an internet error!";

			if (bHaveExceptions == TRUE)
			{
				strMsg += "\n\nWeb payment exceptions have also been detected!";
			}

			Prompter.Error(strMsg, strTitle);
			OnWebPaymentLog();													// show web payment logs
		}

		else if (bHaveExceptions == TRUE)										// see if web payment excection indicator file found
		{
			strMsg += "Web payment exceptions have been detected!\n\nDo you to wish to view the exception logs?";

			if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDYES)
			{
				OnWebPaymentLog();
			}
		}

		if (bHaveExceptions == TRUE)
		{
			CFileRecycler::SendToRecycleBin(Filenames.GetWebPaymentExceptionIndicator());		// delete file if not already deleted
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	{
		__int64 nUserID = GetSelectedListUserID();
		if (nUserID < 0)
		{
			return;
		}

		{
			m_SQLRowContextMenu.SetUserID(nUserID);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}
	}

	if ((point.x == -1) && (point.y == -1))		//keystroke invocation
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU2));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CMenuChanger menuChanger;
	menuChanger.EatMenuPopup(pPopup);

	// topup buttons

	bool bAllowed = (System.GetShowTopupButtonFlag() == FALSE) ? FALSE : Passwords.IsTopupButtonAllowed();
	menuChanger.ShowParent(IDM_POPUP_TOPUP, bAllowed);

	if (bAllowed == TRUE)
	{
		bAllowed = FALSE;												// Purse2 only if manual refresh allowed
		
		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(m_SQLRowContextMenu.GetGroupNo());
		
		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroup.GetRefreshType() == nREFRESHTYPE_MANUAL)
			{
				bAllowed = TRUE;
			}
		}

		menuChanger.ShowItem(IDM_POPUP_TOPUP2, bAllowed);
		menuChanger.ShowItem(IDM_POPUP_TOPUP3, System.GetEnablePurse3Flag()); // only if purse3 allowed
	}

	//

	menuChanger.EnableItem(IDM_POPUP_ADD, Passwords.IsAddRecordAllowed());
	menuChanger.EnableItem(IDM_POPUP_DELETE, Passwords.IsDeleteRecordAllowed());
	menuChanger.EnableItem(IDM_POPUP_TRANSFER, Passwords.IsTransferRecordAllowed());
	menuChanger.EnableItem(IDM_POPUP_ALERT, Passwords.IsEditRecordAllowed());

	//
	if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)			// UserID = Mifare number
	{
		menuChanger.ShowItem(IDM_POPUP_ALERT, FALSE);

		bool bHotlisted = m_SQLRowContextMenu.IsAlertSet();
		bool bShowTrace = m_SQLRowContextMenu.HavePreviousUserID();		// has been replaced
		bool bShowReplace = TRUE;
		bool bShowUnhotlist = TRUE;

		if (bHotlisted == TRUE)
		{
			bShowTrace = TRUE;
			menuChanger.ShowItem(IDM_POPUP_HOTLIST, FALSE);
			menuChanger.ShowItem(IDM_POPUP_HOTLISTREPLACE, FALSE);
			menuChanger.EnableItem(IDM_POPUP_UNHOTLIST, Passwords.IsTransferRecordAllowed());

			if (m_SQLRowContextMenu.HaveNextUserID() == TRUE)
			{
				bShowReplace = FALSE;
			}
			else
			{
				menuChanger.EnableItem(IDM_POPUP_REPLACE, Passwords.IsTransferRecordAllowed());
			}
		}
		else
		{
			menuChanger.EnableItem(IDM_POPUP_HOTLIST, Passwords.IsTransferRecordAllowed());
			menuChanger.EnableItem(IDM_POPUP_HOTLISTREPLACE, Passwords.IsTransferRecordAllowed());
			bShowReplace = FALSE;
			bShowUnhotlist = FALSE;
		}

		menuChanger.ShowItem(IDM_POPUP_REPLACE, bShowReplace);
		menuChanger.ShowItem(IDM_POPUP_UNHOTLIST, bShowUnhotlist);
		menuChanger.ShowItem(IDM_POPUP_TRACE, bShowTrace);
	}
	else														// Biometeric or ExtRef = Mifare number
	{
		CAlertText alert;
		alert.Open(Filenames.GetAlertFilename());			// readonly
		if (alert.GetAlertCount() == 0)
		{
			menuChanger.ShowItem(IDM_POPUP_ALERT, FALSE);
		}

		menuChanger.ShowItem(IDM_POPUP_HOTLIST, FALSE);
		menuChanger.ShowItem(IDM_POPUP_HOTLISTREPLACE, FALSE);
		menuChanger.ShowItem(IDM_POPUP_REPLACE, FALSE);
		menuChanger.ShowItem(IDM_POPUP_UNHOTLIST, FALSE);
		menuChanger.ShowItem(IDM_POPUP_TRACE, FALSE);
	}

	menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, FALSE);
	
	if (Passwords.IsReportsAllowed() == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());

		if (Server.GetActiveEnablePurchaseHistoryFlag() == TRUE)
		{
			menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, TRUE);
	
			bool bEnableHistory = FALSE;
			CString strFilename = Filenames.GetPurchaseHistoryFilename(m_SQLRowContextMenu.GetUserIDString());
			if ((bEnableHistory = ::FileExists(strFilename)) == FALSE)
			{
				strFilename = Filenames.GetPurchaseHistoryUpdateFilename(m_SQLRowContextMenu.GetUserIDString());
				bEnableHistory = ::FileExists(strFilename);
			}

			menuChanger.EnableItem(IDM_POPUP_PURCHASEHISTORY, bEnableHistory);
		}
	}

	CImageHandler image(FALSE);
	bool bEnable = (image.GetImageFilepath(m_SQLRowContextMenu.GetUserIDString()) == "") ? FALSE : TRUE;
	menuChanger.EnableItem(IDM_POPUP_PHOTOID, bEnable);

	if (System.GetAllowGroupShiftDateRangeFlag() == FALSE)
	{
		menuChanger.ShowItem(IDM_POPUP_GROUPSHIFT, FALSE);
	}

	if ((System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE) || (System.GetCreditOptionsFlag() == FALSE))
	{
		menuChanger.ShowItem(IDM_POPUP_CREDIT, FALSE);
	}

	menuChanger.ShowItem(IDM_POPUP_PEBBLELINK, System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE);

	menuChanger.BuildMenu(pPopup);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CSSAutoShutdownDialog::OnPaint();
	}
}

//$$******************************************************************

HCURSOR CSmartPayManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize = CPoint(500, 250); // minimum tracking width and height
	CSSAutoShutdownDialog::OnGetMinMaxInfo(lpMMI);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CSSAutoShutdownDialog::OnSize(nType, cx, cy);
		
		CRect rect1( 71, 10, 180, 24 );
		MapDialogRect( &rect1 );

		CRect rect2( 182, 10, 173, 24 );
		MapDialogRect( &rect2 );
		
		m_list.MoveWindow ( rect1.left, Y_ADJUSTMENT, cx-(rect1.left+10), cy-(Y_ADJUSTMENT+50) );
		
		m_staticInfo1.SetWindowPos( &wndTop, 
			rect1.left, 
			cy - ( rect1.Height() + Y_ADJUSTMENT), 
			rect1.Width(), 
			rect1.Height(), 
			SWP_NOZORDER );
		
		m_staticInfo2.SetWindowPos( &wndTop, 
			rect2.left, 
			cy - ( rect1.Height() + Y_ADJUSTMENT), 
			cx - (rect2.left + 10), 
			rect1.Height(), 
			SWP_NOZORDER );
		
		m_staticInfo2.SetWindowText( m_strCurrentInfo2Text );
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSmartPayManagerDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nRow = pDispInfo->item.iItem;

		if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
		{
			CSQLRowAccountList ListRow;
			m_SQLRowSetAccountList.GetRow(nRow, ListRow);

			CString strText = "";
			switch (m_ColumnManager.GetListColumnType(pDispInfo->item.iSubItem))
			{
			case COLUMN_USERID:
				strText = System.FormatCardNo(ListRow.GetUserID(), TRUE);
				m_list.SetColour(nRow, ListRow.GetRecordStatusColour());
				break;

			case COLUMN_USERNAME:		
				strText = ListRow.GetUsername();		
				break;

			case COLUMN_FORENAME:		
				strText = ListRow.GetForename();		
				break;

			case COLUMN_SURNAME:		
				strText = ListRow.GetSurname();			
				break;

			case COLUMN_GROUP:			
				strText = ListRow.GetGroupNoString();			
				break;

			case COLUMN_PURSE1:			
				strText = ListRow.GetPurseBalanceString(1);	
				break;

			case COLUMN_PURSE2:			
				strText = ListRow.GetPurseBalanceString(2);	
				break;

			case COLUMN_PURSE3:			
				strText = ListRow.GetPurseBalanceString(3);	
				break;

			case COLUMN_POINTS:			
				strText = ListRow.GetPointsString();			
				break;

			case COLUMN_BALANCE:
				strText = ListRow.GetTotalBalanceString();
				break;

			case COLUMN_STATUS:
				strText = ListRow.GetACListRecordStatusText();
				break;

			default:
				strText = "";
				break;
			}

			lstrcpy(pDispInfo->item.pszText, strText);
		}
	}
	*pResult = 0;
}

//$$******************************************************************

LONG CSmartPayManagerDlg::OnColumnClick ( WPARAM wParam, LONG lParam )
{
	int nIndex = (int)wParam;

	switch ( m_ColumnManager.GetListColumnType( nIndex ) )
	{
	case COLUMN_USERID:		OnButtonCardNo();	break;		// find card no
	case COLUMN_USERNAME:	OnButtonFindCard();	break;		// UserName
	case COLUMN_FORENAME:
	case COLUMN_SURNAME:	SurnameList();		break;		// Forename
	}

	return 0L;
}

//$$******************************************************************

LONG CSmartPayManagerDlg::OnDemoWarning(WPARAM wParam, LONG lParam)
{
	CDemoWarning DemoWarning;
	return 0L;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_COLUMN, nIndex );
	*pResult = 0;
}

//$$******************************************************************

void CSmartPayManagerDlg::UpdateTitle()
{
	CString strInfo1Text = "";
	{
		int nCount = m_SQLRowSetAccountList.GetSize();
		strInfo1Text.Format( "%d Account%s", nCount, ( nCount != 1 ) ? "s" : ""   );
	}

	CString strInfo2Text = "";

	if ( ( EmailOptions.UseSMTPForDirectEmail() == TRUE ) && ( m_nPendingEmailCount != 0 ) )
	{
		strInfo2Text.Format( "%d Pending Email%s", m_nPendingEmailCount, ( m_nPendingEmailCount > 1 ) ? "s" : "" );
	}
	else if (AutoShutdownHelper.IsEnabled() == TRUE)
	{
		strInfo2Text = GetAutoShutdownSecondsText();
	}

	if ( strInfo1Text != m_strCurrentInfo1Text )
	{
		m_staticInfo1.SetWindowText ( strInfo1Text );
		m_strCurrentInfo1Text = strInfo1Text;
	}

	if ( strInfo2Text != m_strCurrentInfo2Text )
	{
		m_staticInfo2.SetWindowText ( strInfo2Text );
		m_strCurrentInfo2Text = strInfo2Text;
	}
}

//$$******************************************************************

CString CSmartPayManagerDlg::GetAutoShutdownSecondsText()
{
	CString strResult = "";

	if ( (AutoShutdownHelper.IsTriggered() == TRUE) || ( AutoShutdownHelper.IsFinalShutdownMode() == TRUE ) )
	{
		strResult = "Auto Logoff due now";
	}
	else
	{
		__int64 nSeconds = AutoShutdownHelper.GetSecondsToShutdown();

		if (nSeconds > 120)
		{
			strResult.Format("Auto Logoff in %I64d minutes", nSeconds / 60);
		}
		else if (nSeconds > 1)
		{
			strResult.Format("Auto Logoff in %I64d seconds", nSeconds);
		}
		else if (nSeconds == 1)
		{
			strResult.Format("Auto Logoff in 1 second" );
		}
		else if (nSeconds <= 0)
		{
			strResult = "Auto Logoff due now";
		}
	}
	
	return strResult;
}

//$$******************************************************************

bool CSmartPayManagerDlg::CheckContextRowExists()
{
	CSQLRepositoryAccount RepoAccount;
	__int64 nSelectError = RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError();
	
	bool bResult = FALSE;

	if ( SQLCRUD_ERR_NOMATCH == nSelectError)
	{
		CString strError = "";
		strError.Format("Unable to find card no %s in database",
			(const char*)m_SQLRowContextMenu.GetUserIDString());

		Prompter.Error(strError);
	}
	else if (SQLCRUD_ERR_DUPLICATE == nSelectError)
	{
		CString strError = "";
		strError.Format("Duplicate card no %s in database",
			(const char*)m_SQLRowContextMenu.GetUserIDString());

		Prompter.Error(strError);
	}
	else if (SQLCRUD_ERR_NONE != nSelectError)
	{
		CString strError = "";
		strError.Format("Unable to select card no %s in database",
			(const char*)m_SQLRowContextMenu.GetUserIDString());

		Prompter.Error(strError);
	}
	else
	{
		bResult = TRUE;
	}

	return bResult;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupPhotoID()
{
	if (CheckContextRowExists() == TRUE)
	{
		CImageHandler image(FALSE);
		CString strPhotoFile = image.GetImageFilepath(m_SQLRowContextMenu.GetUserIDString());
		if (strPhotoFile != "")
		{
			CPhotoIDDlg dlg( m_SQLRowContextMenu, strPhotoFile, m_SQLRowContextMenu.GetForename(), m_SQLRowContextMenu.GetSurname(), this);
			dlg.DoModal();
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonAdd()
{
	//SETTINGS FOR CARD NUMBER OR RANGE DIALOG
	bool bReaderButtonAllowed = (System.GetInterfaceType() == nINTERFACE_MIFAREv1);

	CString strTitle = "Add Database Record";

	//CHECK THAT GROUPS HAVE BEEN SETUP
	CString strMsg = "";
	if ( CheckForGroupsSetup ( strMsg ) == FALSE )		
	{
		Prompter.FatalError( strMsg, strTitle );
		return;
	}

	__int64 nNewCardNo = 1;

	if ( 0 == m_nNextDefaultUserID )	
	{	
		CSQLRepositoryAccount RepoAccount;
		nNewCardNo = RepoAccount.GetHighestCurrentUserID(NULL).GetSQLResult() + 1;
	}
	else								
	{
		nNewCardNo = m_nNextDefaultUserID + 1;
	}

	if ((nNewCardNo < 1) || (nNewCardNo > Account::UserID.Max))
	{
		nNewCardNo = 1;
	}

	{
		CCardNoDlg dlg ( strTitle, FormatInt64Value(nNewCardNo), this );
		dlg.m_bBrowseButtonAllowed = FALSE;
		dlg.m_bReaderButtonAllowed = bReaderButtonAllowed;	
		dlg.m_bCountButtonAllowed  = FALSE;

		if ( dlg.DoModal() == IDCANCEL )
		{
			return;
		}

		nNewCardNo = _atoi64(dlg.GetUserID());
	}

	CSQLRowAccountFull AccountRow;
	AccountRow.SetUserID(nNewCardNo);
	
	CSQLRepositoryAccount RepoAccount;
	switch (RepoAccount.SelectRow(AccountRow, NULL).GetSQLError())
	{
	case SQLCRUD_ERR_NONE:
	case SQLCRUD_ERR_DUPLICATE:
	{
		CString strMsg = "";
		strMsg.Format("Database record %s already exists.",
			(const char*) System.FormatCardNo(nNewCardNo,TRUE));

		Prompter.Error(strMsg, "Add Database Record");
	}
	break;

	default:
	{
		bool bSuccess = FALSE;
		int nNewRecordIdx = 0;
		AddRecord(nNewCardNo, APPNO_INSERT, "", bSuccess, nNewRecordIdx);

		if (FALSE == bSuccess)
		{
			CString strMsg = "";
			strMsg.Format("Unable to add Database record %s.",
				(const char*) System.FormatCardNo(nNewCardNo,TRUE));

			Prompter.Error(strMsg, "Add Database Record");
		}
		else
		{
			EditRecord(nNewCardNo);
		}
	}
	break;
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnBatchAdd()
{
	CString strTitle = "Batch Add Accounts";

	//CHECK THAT GROUPS HAVE BEEN SETUP
	CString strMsg;
	if (CheckForGroupsSetup(strMsg) == FALSE)
	{
		Prompter.FatalError(strMsg, strTitle);
		return;
	}

	do
	{
		int nNewRecordsRequested = 1;
		__int64 nNewCardNo = 1;
		
		if (0 == m_nNextDefaultUserID)
		{
			CSQLRepositoryAccount RepoAccount;
			nNewCardNo = RepoAccount.GetHighestCurrentUserID(NULL).GetSQLResult() + 1;
		}
		else
		{
			nNewCardNo = m_nNextDefaultUserID + 1;
		}

		if ((nNewCardNo < 1) || (nNewCardNo > Account::UserID.Max))
		{
			nNewCardNo = 1;
		}

		__int64 nFirstNewCardNo = nNewCardNo;

		{
			CCardNoDlg dlg(strTitle, FormatInt64Value(nFirstNewCardNo), this);
			dlg.m_bBrowseButtonAllowed = FALSE;
			dlg.m_bReaderButtonAllowed = FALSE;
			dlg.m_bCountButtonAllowed = TRUE;

			if (dlg.DoModal() == IDCANCEL)
			{
				break;
			}

			nFirstNewCardNo = _atoi64(dlg.GetUserID());
			nNewRecordsRequested = dlg.m_nRecordCount;
			nNewRecordsRequested = FastMax(nNewRecordsRequested, 1);
			nNewRecordsRequested = FastMin(nNewRecordsRequested, 999);
		}

		if (nNewRecordsRequested <= 1)
		{
			CSQLRepositoryAccount RepoAccount;
			CSQLResultInfo SQLResult = RepoAccount.RowExists(nFirstNewCardNo, NULL);

			if (SQLCRUD_ERR_NONE != SQLResult.GetSQLError())
			{
				CString strMsg = "";
				strMsg.Format("Unable to check for existing record %s.",
					(const char*) System.FormatCardNo(nFirstNewCardNo, TRUE));

				Prompter.Error(strMsg, "Add Database Record");
			}
			else if (0 != SQLResult.GetSQLResult())
			{
				CString strMsg = "";
				strMsg.Format("Database record %s already exists.", 
					(const char*) System.FormatCardNo(nFirstNewCardNo, TRUE));

				Prompter.Error(strMsg, "Add Database Record");
			}
			else
			{
				bool bSuccess = FALSE;
				int nNewRecordIdx = 0;
				AddRecord(nFirstNewCardNo, APPNO_INSERT, "", bSuccess, nNewRecordIdx);

				if (FALSE == bSuccess)
				{
					CString strMsg = "";
					strMsg.Format("Unable to add Database record %s.", 
						(const char*) System.FormatCardNo(nFirstNewCardNo, TRUE));

					Prompter.Error(strMsg, "Add Database Record");
				}
				else
				{
					EditRecord(nFirstNewCardNo);
				}
			}		
				
		}
		else
		{
			__int64 nCannotAdd = 0;
			__int64 nSysMaxCardNo = _atoi64(System.GetHighCardNo());
			__int64 nLastNewCardNo = nFirstNewCardNo + nNewRecordsRequested - 1;
			
			if (nLastNewCardNo > nSysMaxCardNo)
			{
				nCannotAdd = nLastNewCardNo - nSysMaxCardNo;
				nLastNewCardNo = nSysMaxCardNo;
			}

			CArray<__int64, __int64> arrayInUse;

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.GetUserIDListInRange(arrayInUse, nFirstNewCardNo, nLastNewCardNo, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				Prompter.Error("Unable to check existing rows");
				return;
			}
		
			nCannotAdd += arrayInUse.GetSize();

			if (nCannotAdd >= (__int64) nNewRecordsRequested )
			{
				Prompter.Error("All of the cards in your batch already exist or are out of range.");
				continue;
			}
			else if (nCannotAdd > 0)
			{
				CString strWarn = "";
				strWarn.Format("%I64d of %I64d cards in your batch already exist or are out of range.\n\nDo you wish to add the remaining cards ?", 
					nCannotAdd, 
					(__int64)nNewRecordsRequested);

				if (Prompter.YesNo(strWarn) != IDYES)
				{
					continue;
				}
			}

			__int64 nRecordsToAdd = nNewRecordsRequested - nCannotAdd;

			CCommentDlg dlgConfirm("Add Account Records");

			dlgConfirm.m_strMessage.Format("Add %I64d new account%s to the database",
				nRecordsToAdd,
				nRecordsToAdd > 1 ? "s" : "");

			if (dlgConfirm.DoModal() == IDOK)
			{
				bool bDoneAdd = FALSE;

				CSQLTranBatchAdd Tran;
				Tran.BeginTrans();
				Tran.DoWork(nFirstNewCardNo, nLastNewCardNo, nRecordsToAdd, arrayInUse, dlgConfirm.m_strComment);

				switch (Tran.EndTrans())
				{
				case SQLTRAN_STATE_COMMITOK:
					bDoneAdd = TRUE;
					Tran.AfterTransaction();
					break;

				default:
					Tran.ShowErrorMessage("Batch Add");
					break;
				}

				if (TRUE == bDoneAdd)
				{
					m_nNextDefaultUserID = Tran.GetUserIDToSelect();
					RefreshAccountList(m_nNextDefaultUserID, TRUE);
				}
			}
		}
	} 
	while (TRUE);
}

//$$******************************************************************

void CSmartPayManagerDlg::AddRecord(__int64 nCardNo, int nAppNo, const char* szComment, bool& bSuccess, int& nNewRecordIdx)
{
	CSQLRowAccountFull NewRow;
	NewRow.SetUserID(nCardNo);

	bSuccess = FALSE;
	nNewRecordIdx = m_list.GetCurSel();
	
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.InsertRow(NewRow, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		m_SQLRowSetAccountList.SelectRows(NewRow.GetUserID());
		nNewRecordIdx = m_SQLRowSetAccountList.GetUserIDPos();
		bSuccess = TRUE;

		CSQLAuditRecord atc;
		atc.Set(&NewRow);
		atc.SetApplicationNo(nAppNo);
		atc.SetOperatorID(Passwords.GetPasswordInUse());
		atc.SetLineComment(szComment);
		atc.SaveNewLine();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::SurnameList()
{
	CSurnameListDlg dlg ( this );
	
	if (dlg.DoModal() == IDOK)
	{
		RefreshAccountList(dlg.m_nRequestedUserID, TRUE);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonFindCard()
{
	CFindDlg dlg ( "", this );

	if (dlg.DoModal() == IDOK)
	{
		RefreshAccountList(dlg.m_nRequestedUserID, TRUE);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonCardNo()
{
	CCardNoDlg dlg ( "Find UserID", "", this );

	while ( dlg.DoModal() != IDCANCEL )
	{
		__int64 nCardNo	= _atoi64(dlg.GetUserID());						// UserID
		CString strMiFareNo	= dlg.GetMiFareNo();					// MiFare number if MiFare-2 format

		if ( nCardNo >= 0 )										// may be blank if MiFare-2 & no entry in MiFare database
		{
			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.RowExists(nCardNo, NULL).GetSQLResult() != 0 )
			{
				EditRecord ( nCardNo );
				break;
			}
		}

		CString strNotFoundMsg = "";
		if (strMiFareNo != "")
		{
			strNotFoundMsg.Format("Account card %s", 
				(const char*) strMiFareNo);
		}
		else
		{
			strNotFoundMsg.Format("UserID %s",
				(const char*) System.FormatCardNo(nCardNo, TRUE));
		}

		CString strError = "";
		strError.Format ( "No database record found for %s", 
			(const char*) strNotFoundMsg);

		Prompter.Error( strError, "Find Database Record"  );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnButtonEdit();
	*pResult = 0;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonEdit()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();								

	if ( nUserID >= 0 )	
	{
		EditRecord ( nUserID );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::EditRecord ( __int64 nCardNo )
{
	CEditRecord record ( "", this );
	record.Edit ( nCardNo );
	RefreshAccountList ( nCardNo, TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonDelete()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CDeleteAccountRecord RecordDelete(this);
		RecordDelete.DeleteRecord(nUserID);
		RefreshAccountList(nUserID, TRUE);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnSetupSystem()
{
	if ( Sysset.LockAllUsers() == FALSE )
	{
		Prompter.LockError( "change system settings" );
	}
	else
	{
		SetupSystemInternal();
		Sysset.UnlockAllUsers();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnSetupPurchaseControl()
{
	CAutoShutdownDlgStacker Stacker(this);
	AutoShutdownHelper.ClearShutdownRequestMarker();

	CWaitUpdatePluDlg dlgWait("Checking for Purchase Control Updates", this);
	dlgWait.LockLoyalty();

	if (AutoShutdownHelper.GetShutdownRequestMarker() == FALSE)
	{
		CListDataPurchaseControlInfoDlg dlgShow;
		dlgShow.DoModal();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::SetupSystemInternal()
{		
	CSystemPropertySheet dlg ( this );

	AutoBackupOptions.RememberSettings();

	CPresentationOptions OldOptions;
	OldOptions.CopyFrom( PresentationOptions );

	CEmailOptions OldEmailOptions;
	OldEmailOptions.CopyFrom( EmailOptions );

	CPCOptionsClient OldPCOptionsClient;
	OldPCOptionsClient.CopyFrom( PCOptionsClient );

	CPCOptionsHost OldPCOptionsHost;
	OldPCOptionsHost.CopyFrom( PCOptionsHost );

	CAutoShutdownDlgStacker Stacker(this);
	
	int nReply = dlg.DoModal();
	
	if ( nReply == IDOK )
	{
		System.Write();							// save new settings

		if ( dlg.HasDisplayTab() == TRUE )
		{
			PresentationOptions.Write();
		}

		EmailOptions.Write();
		PCOptionsClient.Write();
		PCOptionsHost.Write();

		if ( System.GetAllowPINNumbersFlag() == TRUE )
		{
			CSQLTranCreatePINNumbers Tran;
			Tran.BeginTrans();
			Tran.DoWork(FALSE,FALSE);
			Tran.EndTrans();
		}

		if (AutoBackupOptions.HasScheduleChanged())
		{
			AutoBackupProcessor.ScheduleNextBackup(FALSE);

			if (m_pBackupResultDlg != NULL)
			{
				m_pBackupResultDlg->UpdateDisplay();
			}
		}
		AutoBackupOptions.Write();

		if ( EmailOptions.UseSMTPForDirectEmail() == TRUE )
		{
			CSSFile fileToken;
			if ( fileToken.Open( Filenames.GetEmailLogRetryTokenFilename(), "wb" ) == TRUE )
			{
				fileToken.WriteLine( "XX" );
				fileToken.Close();

				SSspawnl ( this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL );
			}
		}
	}
	else
	{
		if ( dlg.HasDisplayTab() == TRUE )
		{
			PresentationOptions.CopyFrom( OldOptions );
		}

		EmailOptions.CopyFrom( OldEmailOptions );
		PCOptionsClient .CopyFrom(OldPCOptionsClient);
		PCOptionsHost.CopyFrom(OldPCOptionsHost);
	}

	System.Rewind();							// reload either new settings or old settings if cancel used

	if ( nReply == IDOK )						// if new settings, set menu options etc
	{
		SetMenuOptions();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnSetupGroups()
{
	CListDataGroupDlg dlg( this );
	CAutoShutdownDlgStacker Stacker(this);
	dlg.DoModal();
}

//$$******************************************************************

bool CSmartPayManagerDlg::CheckForGroupsSetup ( CString& strErrorMsg )
{
	bool bReply = TRUE;

	CSQLRepositoryGroup RepoGroup;
	if ( RepoGroup.CountRows(NULL).GetSQLResult() == 0 )
	{
		strErrorMsg = "No SmartPay Groups have been setup";
		bReply = FALSE;
	}
	
	return bReply;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnSetupServer()
{
	if ( Sysset.LockAllUsers() == FALSE )
	{
		Prompter.LockError( "change server settings" );
	}
	else
	{
		SetupServerInternal();
		Sysset.UnlockAllUsers();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::SetupServerInternal()
{
	CAutoShutdownDlgStacker Stacker(this);

	ServerCopy.CopyFrom(Server);

	CServerPropertySheet dlg(this);

	if ( dlg.DoModal() == IDOK )
	{
		dlg.Save();
		SetMenuOptions();								// may change menu options
	}
	else
	{
		Server.CopyFrom(ServerCopy);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnHelpSmartPayManager()
{
	CDemoWarning DemoWarning;
	CAboutDlg dlg;
	dlg.DoModal();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnHelpLicencedetails()
{
	Sysset.DisplayLicenceDetails ( "", "", 0, this );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileRefresh()
{
	CImportDataHandler import( this );
	import.ScanAll();									// see if any data is waiting to be imported
	RefreshAccountList(TRUE);							// reopen data & set menu options
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileUpgrade()
{
	RefreshAccountList(TRUE);

	if (m_SQLRowSetAccountList.GetSize() != 0)
	{
		Prompter.Info("You cannot upgrade data from a previous version because\nthe smartpay database already contains new records.");
		return;
	}

	AutoShutdownHelper.SetTimerBlock1(TRUE);

	CUpgradeHandler version(this);
	if (version.Upgrade() == TRUE)					// upgrade from user specified folder
	{
		RefreshAccountList(TRUE);
		SetMenuOptions();							// remove menu options not required for given system
	}

	AutoShutdownHelper.SetTimerBlock1(FALSE);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileSignoff()
{
	GetPassword(FALSE);
	SetMenuOptions();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSmartPayManagerDlg::SetMenuOptions()
{
	Server.Read(Filenames.GetServerDataFilename());

	// buttons

	m_buttonAdd.EnableWindow(Passwords.IsAddRecordAllowed());
	m_buttonDelete.EnableWindow(Passwords.IsDeleteRecordAllowed());
	m_buttonTransfer.EnableWindow(Passwords.IsTransferRecordAllowed());

	CAlertText alert{};
	alert.Open(Filenames.GetAlertFilename());				// readonly
	if (alert.GetAlertCount() == 0)
	{
		m_buttonAlert.ShowWindow(SW_HIDE);
	}
	else
	{
		m_buttonAlert.ShowWindow(SW_SHOW);
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_buttonAlert.EnableWindow(Passwords.IsTransferRecordAllowed());
			m_buttonAlert.SetWindowText("Hotlist\nAccount");
		}
		else
		{
			m_buttonAlert.EnableWindow(Passwords.IsEditRecordAllowed());
			m_buttonAlert.SetWindowText("Alert");
		}
	}

	// File options - SmartPay Server

	m_MenuChanger.ShowItem(IDM_FILE_UPGRADE, (m_SQLRowSetAccountList.GetSize() == 0));

	// File options - SmartPay Server

	m_MenuChanger.ShowParent(IDM_FILE_SERVER_SERVICE, Passwords.IsInstallServerAllowed());

	// File options - Background Processor

	{
		bool bAllowed = (System.GetEnableBackgroundFlag() && Passwords.IsInstallBgndAllowed());

		/*
		if (TRUE == bAllowed)
		{
			CBackgroundOptions options;
			options.Read();
			bAllowed = options.IsSet();
		}
		*/

		m_MenuChanger.ShowParent(IDM_FILE_BGND_SERVICE, bAllowed);
	}

	//file options - backup
	{
		int nAccessType = PCOptionsHost.GetBackupAccessType();
		m_MenuChanger.ShowItem(IDM_BACKUP_SYSTEM, nAccessType < 2);
		m_MenuChanger.ShowItem(IDM_RESTORE_SCRIPT, m_bShowRestoreFromScript);
		m_MenuChanger.ShowParent(IDM_RESTORE_EXISTING, nAccessType < 3);
	}

	// File options - other

	m_MenuChanger.ShowItem(IDM_FILE_SIGNOFF, System.GetUsePasswordsFlag() && Passwords.InUse());

	// Utlity menu
	{
		bool bAllowed = Passwords.IsImportAllowed();
		m_MenuChanger.ShowParent(IDM_FILE_IMPORT, bAllowed);

		if (bAllowed == TRUE)										// import options;
		{
			CCsvImportData import;
			import.Read();

			bool bShowImportMenu = FALSE;

			bShowImportMenu |= import.m_bEnableImport;
			m_MenuChanger.ShowItem(IDM_FILE_IMPORT, import.m_bEnableImport);

			bShowImportMenu |= System.GetEnableSimsFlag();
			m_MenuChanger.ShowItem(IDM_IMPORT_SIMSFILE, System.GetEnableSimsFlag());

			bShowImportMenu |= System.GetEnableWondeFlag();
			m_MenuChanger.ShowItem(IDM_IMPORT_WONDEFILE, System.GetEnableWondeFlag());

			bShowImportMenu |= import.m_bEnableTopupImport;
			m_MenuChanger.ShowItem(IDM_IMPORT_TOPUPFILE, import.m_bEnableTopupImport);

			bShowImportMenu |= import.m_bEnableCustomImport;
			if (TRUE == import.m_bEnableCustomImport)
			{
				CString strText = import.m_strCustomMenuText;
				m_MenuChanger.SetItemText(IDM_IMPORT_CUSTOMFILE, strText);
			}
			m_MenuChanger.ShowItem(IDM_IMPORT_CUSTOMFILE, import.m_bEnableCustomImport);

			bShowImportMenu |= import.m_bEnableDbaseAuditImport;
			m_MenuChanger.ShowItem(IDM_IMPORT_AUDITDATA, import.m_bEnableDbaseAuditImport);

			bShowImportMenu |= System.GetEnableAutoImportFlag();
			m_MenuChanger.ShowItem(IDM_IMPORT_AUTOFILE, System.GetEnableAutoImportFlag());

			bShowImportMenu |= System.GetKeepLeaversDatabaseFlag();
			m_MenuChanger.ShowItem(IDM_IMPORT_LEAVERS, bAllowed);

			m_MenuChanger.ShowParent(IDM_FILE_IMPORT, System.GetKeepLeaversDatabaseFlag());
		}
	}

	m_MenuChanger.ShowParent(IDM_FILE_EXPORT, Passwords.IsExportAllowed());
	m_MenuChanger.ShowItem(IDM_EXPORT_SALESTOPUPS, System.GetEnableExportSalesFlag());

	//	Utility \ Email

	CEmailAlertData email;
	email.Read();

	bool bAllowed = email.AlertsRequired();
	m_MenuChanger.ShowParent(IDM_EMAIL_LOWBALANCE, bAllowed);

	if (TRUE == bAllowed)
	{
		bool bAllowExport = Passwords.IsExportAllowed();
		m_MenuChanger.ShowItem(IDM_EMAIL_LOWBALANCE, email.SendLowBalancesReqd() && bAllowExport);
		m_MenuChanger.ShowItem(IDM_EMAIL_POINTSACHIEVEMENT, email.SendPointsAchievementReqd() && bAllowExport);
		m_MenuChanger.ShowItem(IDM_EMAIL_PURCHASES, email.SendPurchasesReqd() && bAllowExport);
		m_MenuChanger.ShowItem(IDM_EMAIL_WEEKLYPOINTS, email.SendWeeklyPointsReqd() && bAllowExport);
		m_MenuChanger.ShowItem(IDM_EMAIL_LOG, EmailOptions.UseSMTPForDirectEmail());
	}

	// Utility \ Web Payments
	m_MenuChanger.ShowParent(IDM_FILE_WEBPAYMENTIMPORT, FALSE);

	if ( System.GetWebPaymentType() != nWEBPAYMENT_NONE)
	{
		CSQLRepositoryWebPaymentOptions webOptions;
		webOptions.LoadRow(NULL);

		bool bAllowImport = Passwords.IsImportAllowed();
		bool bAllowExport = Passwords.IsExportAllowed();
		bool bAllowReport = Passwords.IsReportsAllowed();
		bool bAllowPebbleBalanceReport = FALSE;
		bool bAllowWebPaymentBalances = FALSE;

		bool bAllowWebPaymentEOD = bAllowExport;
		bAllowWebPaymentEOD &= webOptions.m_Row.EODAllowed();
		bAllowWebPaymentEOD &= (FALSE == webOptions.m_Row.GetEODAfterPaymentFlag());

		bool bAllowLinkAccounts = bAllowImport;
		bAllowLinkAccounts &= webOptions.m_Row.GetImportAccountsFlag();
		bAllowLinkAccounts &= webOptions.m_Row.ImportAccountsService();

		CString strParentText = "&Web Payments";
		CString strWebPaymentImportText = "Import Web Payments";
	
		switch (System.GetWebPaymentType())
		{
		case nWEBPAYMENT_PEBBLE:
			strParentText = "&Pebble";
			strWebPaymentImportText = "Synchronise Transactions";
			bAllowPebbleBalanceReport = bAllowReport;
			break;

		case nWEBPAYMENT_BROMCOM:
			strParentText = "&BromCom";
			bAllowWebPaymentBalances = bAllowExport;
			break;

		case nWEBPAYMENT_SCHOOLCOMMS:
			bAllowWebPaymentBalances = bAllowExport;
			break;
		}

		if (TRUE == (bAllowImport || bAllowWebPaymentEOD || bAllowWebPaymentBalances || bAllowReport))
		{
			m_MenuChanger.ShowParent(IDM_FILE_WEBPAYMENTIMPORT, TRUE);
		
			m_MenuChanger.SetParentText(IDM_FILE_WEBPAYMENTIMPORT, strParentText);
			m_MenuChanger.SetItemText(IDM_FILE_WEBPAYMENTIMPORT, strWebPaymentImportText);
			m_MenuChanger.SetItemText(IDM_FILE_WEBPAYMENTLINKACCOUNTS, webOptions.m_Row.GetImportMenuCaption());

			m_MenuChanger.ShowItem(IDM_FILE_WEBPAYMENTIMPORT, bAllowImport);
			m_MenuChanger.ShowItem(IDM_FILE_WEBPAYMENTENDOFDAY, bAllowWebPaymentEOD);
			m_MenuChanger.ShowItem(IDM_FILE_WEBPAYMENTBALANCES, bAllowWebPaymentBalances);
			m_MenuChanger.ShowItem(IDM_FILE_WEBPAYMENTLOG, bAllowReport);
			m_MenuChanger.ShowItem(IDM_FILE_WEBPAYMENTLINKACCOUNTS, bAllowLinkAccounts);
			m_MenuChanger.ShowItem(IDM_PEBBLE_BALANCES, bAllowPebbleBalanceReport);
			m_MenuChanger.ShowItem(IDM_PEBBLE_LINKTIMES, bAllowPebbleBalanceReport);
		}
	}

	//batch menu
	if (FALSE == (System.GetEnableBatchUpdatesFlag() && Passwords.IsBatchUpdatesAllowed()))
	{
		m_MenuChanger.ShowParent(IDM_BATCH_UPDATE, FALSE);
	}
	else
	{
		m_MenuChanger.ShowParent(IDM_BATCH_UPDATE, TRUE);
		m_MenuChanger.ShowItem(IDM_BATCH_ADDCARDS, Passwords.IsAddRecordAllowed());
		m_MenuChanger.ShowItem(IDM_BATCH_UPDATE, TRUE);
		m_MenuChanger.ShowItem(IDM_BATCH_REDEEM, Server.RedeemPointsEnabled());
		m_MenuChanger.ShowItem(IDM_BATCH_DELETE, Passwords.IsDeleteRecordAllowed());
	}

	// reports

	m_MenuChanger.ShowParent(IDM_REPORTS_DATABASE, Passwords.IsReportsAllowed());
	m_MenuChanger.ShowItem(IDM_REPORTS_LEAVERS, System.GetKeepLeaversDatabaseFlag() && Passwords.IsReportsAllowed());
	m_MenuChanger.ShowParent(IDM_REPORTS_PURCHASEHISTORY, Server.GetActiveEnablePurchaseHistoryFlag());
	m_MenuChanger.ShowItem(IDM_REPORTS_REVALUEBONUS, Server.RevaluationBonusEnabled());
	m_MenuChanger.ShowItem(IDM_REPORTS_SPENDBONUS, Server.SpendBonusEnabled());
	m_MenuChanger.ShowItem(IDM_AUDIT_REDEEMBONUS, Server.RedeemPointsEnabled());
	m_MenuChanger.ShowItem(IDM_REPORTS_PHOTOIDLIST, System.GetEnablePhotoIDListFlag());
	m_MenuChanger.ShowItem(IDM_AUDIT_POINTSADDED, System.GetActiveEnablePointsFlag());
	m_MenuChanger.ShowItem(IDM_AUDIT_POINTSAUDIT, System.GetActiveEnablePointsFlag());
	m_MenuChanger.ShowItem(IDM_AUDIT_POINTSDEDUCTED, System.GetActiveEnablePointsFlag());
	m_MenuChanger.ShowItem(IDM_CARDUSAGE_LOWPOINTS, System.GetActiveEnablePointsFlag());
	m_MenuChanger.ShowItem(IDM_CARDUSAGE_RANKINGPOINTS, System.GetActiveEnablePointsFlag());
	m_MenuChanger.ShowItem(IDM_AUDIT_CREDIT, System.GetCreditOptionsFlag());

	// setup options

	{
		bool bSystem = Passwords.IsSetupSystemAllowed();
		bool bServer = Passwords.IsSetupServerAllowed();

		if ((FALSE == bSystem) && (FALSE == bServer))
		{
			m_MenuChanger.ShowParent(IDM_SETUP_SYSTEM, FALSE);
		}
		else
		{
			m_MenuChanger.ShowParent(IDM_SETUP_SYSTEM, TRUE);
			m_MenuChanger.ShowItem(IDM_SETUP_SYSTEM, bSystem);
			m_MenuChanger.ShowItem(IDM_SETUP_SERVER, bServer);
			m_MenuChanger.ShowItem(IDM_SETUP_GROUPS, bSystem);
			m_MenuChanger.ShowItem(IDM_FILE_TERMINALLIST, bSystem);
			m_MenuChanger.ShowItem(IDM_SETUP_PURCHASECONTROL, TRUE);
		}
	}

	m_MenuChanger.ShowItem(IDM_REPORTS_GROUPSHIFTLOG, System.GetAllowGroupShiftDateRangeFlag());
	
	m_MenuChanger.BuildMenu(GetMenu());
	DrawMenuBar();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonTransfer()
{
	__int64 nUserID = GetSelectedListUserID();
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
	{
		TransferRecord(nUserID);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::TransferRecord ( __int64 nUserIDFrom )
{
	CString strTitle = "Transfer";
	__int64 nUserIDTo = nUserIDFrom;
	__int64 nUserIDExit = nUserIDFrom;

	do
	{
		nUserIDExit = nUserIDFrom;

		CString strMsg = "";
		strMsg.Format("Enter destination for UserID %s's data", 
			(const char*) FormatInt64Value(nUserIDFrom));

		CCardNoDlg dlg(strMsg, "", this);

		if (dlg.DoModal() == IDCANCEL)
		{
			break;
		}

		nUserIDTo = _atoi64(dlg.GetUserID());
		if (nUserIDFrom == nUserIDTo)
		{
			continue;
		}

		bool bNeedCreate = FALSE;
		CSQLRowAccountFull RowAccountTo;
		RowAccountTo.SetUserID(nUserIDTo);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(RowAccountTo.GetUserID(), NULL).GetSQLResult() == 0)
		{
			CString strMsg = "";
			strMsg.Format("No record exists for destination UserID %s.  Do you wish to create one?",
				(const char*) FormatInt64Value(nUserIDTo));

			MessageBeep(MB_ICONQUESTION);

			if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDNO)
			{
				continue;
			}

			bNeedCreate = TRUE;
		}
		else
		{
			RowAccountTo.SetUserID(nUserIDTo);
			RepoAccount.SelectRow(RowAccountTo, NULL);

			if (RowAccountTo.IsAlertSet() == TRUE)
			{
				CString strMsg = "";
				strMsg.Format("The recipient UserID %s has an Alert.  Do you still wish to continue with the transfer?", 
					(const char*) FormatInt64Value(nUserIDTo));

				MessageBeep(MB_ICONQUESTION);

				if (Prompter.YesNo(strMsg, strTitle) == IDNO)
				{
					continue;
				}
			}
		}

		CRecordXferPropertySheet dlgXfer(FormatInt64Value(nUserIDFrom), FormatInt64Value(nUserIDTo), FALSE, this);

		if (dlgXfer.DoModal() == IDOK)
		{
			CRecordXferData RecordXferData;
			dlgXfer.GetRecordXferData(RecordXferData);

			CSQLTranXFerAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RecordXferData, bNeedCreate, FALSE, FALSE, 0, "", nUserIDFrom, nUserIDTo);;

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				RefreshAccountList(nUserIDTo, TRUE);
				MessageBeep(MB_ICONINFORMATION);
				Prompter.Info("Transfer complete", strTitle);
				nUserIDExit = nUserIDTo;
				break;

			default:
				Tran.ShowErrorMessage("Transfer Account");
				break;
			}
			break;
		}
	}
	while ( TRUE );

	RefreshAccountList ( nUserIDExit, TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileImport()
{
	CAutoShutdownDlgStacker Stacker(this);

	__int64 nUserID = GetSelectedListUserID();

	CSQLImportCounters Counters;
	CImportDatabase import(this);

	if (import.UserImport(Counters) == TRUE)
	{
		RefreshAccountList(nUserID, TRUE);
		import.ShowStats(Counters);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportSimsfile()
{
	__int64 nUserID = GetSelectedListUserID();

	CImportSimsDlg dlg(this);
	dlg.DoModal();

	RefreshAccountList(nUserID, TRUE);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportWondefile()
{
	__int64 nUserID = GetSelectedListUserID();

	CImportWondeDlg dlg ( this );
	dlg.DoModal();

	RefreshAccountList( nUserID, TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportTopupfile()
{
	__int64 nUserID = GetSelectedListUserID();

	CAutoShutdownDlgStacker Stacker(this);
	
	CSQLImportCounters Counters;

	CImportDatabase import ( this );
	if (import.ImportTopups(Counters) == TRUE)
	{
		RefreshAccountList(nUserID, TRUE);
		import.ShowStats(Counters);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportCustomfile()
{
	__int64 nUserID = GetSelectedListUserID();

	CSQLImportCounters Counters;
	CImportDatabase import ( this );
	if (import.CustomImport(Counters) == TRUE)					
	{
		RefreshAccountList(nUserID, TRUE);					
		import.ShowStats(Counters);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportAutofile()
{
	__int64 nUserID = GetSelectedListUserID();

	CAutoImportHandler autoImport ( this );	
	autoImport.ImportAccounts();							// import new accounts via csv file or SIMS file
	autoImport.ImportPhotoIDs();							// new photo files

	RefreshAccountList( nUserID, TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportLeavers()
{
	__int64 nUserID = GetSelectedListUserID();

	CAutoShutdownDlgStacker Stacker(this);
	CImportLeaversList list ( this );	
	list.Import();
	
	RefreshAccountList( nUserID, TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnImportAuditdata()
{
	__int64 nUserID = GetSelectedListUserID();

	CDatabaseAuditImport import ( this );
	import.ProcessAudit();;

	RefreshAccountList( nUserID, TRUE );						// no force re-open datbase
}

//$$******************************************************************
void CSmartPayManagerDlg::OnFileExport()		{ ExportData ( "CSV" ); }
//$$******************************************************************

void CSmartPayManagerDlg::ExportData ( const char* szLabel )
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.CountRows(NULL).GetSQLResult() == 0)
	{
		return;
	}

	CDbExportEditor editor ( this );
	if ( editor.EditExportLabel ( szLabel ) == TRUE )
	{
		CExportDatabase exp ( szLabel, this );
		exp.Execute( TRUE );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::ExportDatabase()
{
	if ( TRUE == System.GetExportOnExitFlag() )
	{
		CExportDatabase exp1 ( "DB1", this );	
		exp1.Execute ( FALSE );

		CExportDatabase exp2 ( "DB2", this );
		exp2.Execute ( FALSE );

		CExportDatabase exp3 ( "DB3", this );
		exp3.Execute ( FALSE );
	}
}

//$$******************************************************************
void CSmartPayManagerDlg::OnBatchUpdate()	{ BatchUpdate ( nBATCHUPDATE_UPDATE ); }
void CSmartPayManagerDlg::OnBatchRedeem()	{ BatchUpdate ( nBATCHUPDATE_REDEEM ); }
void CSmartPayManagerDlg::OnBatchDelete()	{ BatchUpdate ( nBATCHUPDATE_DELETE ); }
//$$******************************************************************

void CSmartPayManagerDlg::BatchUpdate ( int nBatchType )
{
	__int64 nUserID = GetSelectedListUserID();

	RefreshAccountList(nUserID, TRUE);

	if ( m_SQLRowSetAccountList.GetSize() > 1 )
	{
		CBatchUpdate update ( nBatchType, this );	// Batch update
		CString strFinalMessage = update.DoUpdate();

		RefreshAccountList( nUserID, TRUE );

		if (strFinalMessage != "")
		{
			Prompter.Info(strFinalMessage, update.GetTitle());
		}
	}
}

//$$******************************************************************
//$$*** reports ******************************************************
//$$******************************************************************

void CSmartPayManagerDlg::OnReportsDatabase()
{
	if (CheckHaveAccountRows() == FALSE )
	{
		return;
	}

	CListDataDatabaseReportDlg dlg(this);
	dlg.DoModal();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsLeavers()
{
	CListDataLeaversDlg dlg ( this );
	dlg.DoModal();
}

//$$******************************************************************
void CSmartPayManagerDlg::OnCardUsageNeverUsed()	{	DatabaseReport ( "R010" );	}
//$$******************************************************************

void CSmartPayManagerDlg::DatabaseReport ( const char* szLabel  )
{
	CDatabaseReport report ( szLabel, this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsBirthdays()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CBirthdayReport report ( this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsPhotoIDList()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CReportPhotoIDList report( this );
	while ( report.Display() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupPebbleLink()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CReportPebbleLinkTimes Report(this);
		CString strLabel = "R145";
		Report.m_ReportInfo.m_strReportLabel = strLabel;
		Report.m_ReportInfo.m_strReportFilename = Filenames.GetReportFilename(strLabel);
		Report.m_ReportInfo.m_strParamsFilename = Filenames.GetReportParamsFilename(strLabel);
		Report.m_ReportInfo.m_strReportKey = "A";
		Report.m_ReportInfo.m_strReportTitle = "Pebble Link Time Report";
		Report.m_ReportInfo.m_dateFrom = CSSDate("01/01/2025");
		Report.m_ReportInfo.m_dateTo = CSSDate::GetCurrentDate();
		Report.m_ReportInfo.m_bContinuous = TRUE;
		Report.m_ReportInfo.m_timeStart = CSSTime("00:00");
		Report.m_ReportInfo.m_timeEnd = CSSTime("23:59");
		Report.m_ReportInfo.m_nReportRangeType = 1;
		Report.m_ReportInfo.m_strCardNoFrom = FormatInt64Value(nUserID);
		Report.m_ReportInfo.m_strCardNoTo = Report.m_ReportInfo.m_strCardNoFrom;
		Report.m_ReportInfo.m_nSingleGroupNo = 0;
		Report.m_ReportInfo.m_bSortByDate = FALSE;
		Report.m_ReportInfo.m_bSortByGroup = FALSE;
		Report.m_ReportInfo.m_bSortByID = FALSE;
		Report.ShowDirect();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupAudit()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{ 
			ShowAuditReport("R111", nUserID);
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupAuditSummary()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{
			ShowAuditReport("R141", nUserID);
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupPurchaseControls()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{
			CPurchaseControlReportWrapper Wrapper("R143", m_SQLRowContextMenu.GetUserID(), this);
			Wrapper.ShowReport();
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsAudit()		
{
	ShowAuditReport ( "R101", 0 );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsAuditSummary()
{ 
	ShowAuditReport("R140", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsBonusRevalue()
{ 
	ShowAuditReport ( "R102", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsBonusSpend()	
{ 
	ShowAuditReport ( "R103", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsCashTaken()	
{ 
	ShowAuditReport ( "R109", 0); 
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditPointsAudit()
{ 
	ShowAuditReport ( "R112", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditPointsAdded()	
{ 
	ShowAuditReport ( "R113", 0); 
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditPointsDeducted()
{
	ShowAuditReport ( "R114", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditSales()	
{
	ShowAuditReport ( "R115", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditPurseAdditions()
{ 
	ShowAuditReport ( "R116", 0); 
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditRefunds()	
{ 
	ShowAuditReport ( "R117", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditRedeemBonus()	
{
	ShowAuditReport ( "R137", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditCredit()		
{
	ShowAuditReport ( "R139", 0);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsGroupShiftLog()
{
	if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
	{
		ShowGroupShiftReport("R140", 0);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::ShowAuditReport(const char* szLabel, __int64 nSingleUserID )
{
	CAutoShutdownDlgStacker Stacker(this);
	CAuditReportWrapper wrapper(Filenames.GetAuditFilename(), szLabel, nSingleUserID, this);
	wrapper.ShowReport();
}

//$$******************************************************************

void CSmartPayManagerDlg::ShowPCAuditReport(const char* szLabel)
{
	CAutoShutdownDlgStacker Stacker(this);
	CPCAuditReportWrapper wrapper(Filenames.GetAuditFilename(), szLabel, this);
	wrapper.ShowReport();
}

//$$******************************************************************

void CSmartPayManagerDlg::ShowGroupShiftReport(const char* szLabel, __int64 nSingleUserID)
{
	CAutoShutdownDlgStacker Stacker(this);
	CGroupShiftReportWrapper wrapper(szLabel, nSingleUserID, this);
	wrapper.ShowReport();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditArchive()
{
	CAutoShutdownDlgStacker Stacker(this);
	CAuditReportWrapper Wrapper( Filenames.GetAuditFilename(), "R120", 0, this);
	Wrapper.ShowArchive();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditKioskTopups()
{
	CReportKioskAudit report ( "R118", this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnUsageWeeklySpend()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CReportWeeklySpend report ( "R128", this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupPurchaseHistory()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{
			CPurchaseReport report("R105", nUserID, this);
			report.Show();
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsPurchaseHistory()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CPurchaseReport report ( "R104", 0, this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsPurchaseControls()
{
	CPurchaseControlReportWrapper Wrapper("R142", 0, this);
	Wrapper.ShowReport();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPurchasesBreakdown()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CPurchaseBreakdownReport report ( "R136", this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupStatement()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{
			CReportStatement report(Filenames.GetAuditFilename(), "R122", nUserID, this);
			report.Show();
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPurchasesStatements()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CReportStatement report ( Filenames.GetAuditFilename(), "R121", 0, this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageLowPoints()
{
	AccumulatorReport("R003");
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageLowSpend()
{
	AccumulatorReport("R004");
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageRankingPoints()
{
	AccumulatorReport("R005");

}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageRankingSpend()
{
	AccumulatorReport("R006");
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageActiveCards()
{
	AccumulatorReport("R007");
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageInactiveCards()
{
	AccumulatorReport("R008");
}

//$$******************************************************************

void CSmartPayManagerDlg::AccumulatorReport ( const char* szLabel ) 
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CAccumulatorReportInfo ReportInfo;
	ReportInfo.SetReportLabel(szLabel);

	CReportAccumulator report ( ReportInfo, Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsStartingBalance()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CReportStartingBalance report ( Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCardUsageRankingPurchases()
{
	CReportPluRanking report ( this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportGroupPeriodSales()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CGroupPeriodSalesReport report ( Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnReportsEndofday()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	CReportEndOfDay report ( this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupUnhotlist()
{ 
	HandlePopupToggleHotlistUser();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupHotlist()	
{ 
	HandlePopupToggleHotlistUser();
}

//$$******************************************************************

void CSmartPayManagerDlg::HandlePopupToggleHotlistUser()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();
	
	if (nUserID > 0)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);
		
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (RowAccount.IsAlertSet() == FALSE)
		{
			ProcessHotlist(RowAccount);
		}
		else
		{
			ProcessUnhotlist(RowAccount);
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::ProcessHotlist(CSQLRowAccountFull& RowAccount)
{
	CHotlistDlg dlg(RowAccount, this);

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	int nHotlistCode = dlg.GetReason();
	
	RowAccount.SetUserAlert(nHotlistCode);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.UpdateRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CSQLAuditRecord atc;
		atc.Set(&RowAccount);
		atc.SetApplicationNo(APPNO_HOTLIST);
		atc.SetOperatorID(Passwords.GetPasswordInUse());
		atc.SetAuditLineExtraText(nHotlistCode);
		atc.SetLineComment(dlg.m_strComment);
		atc.SaveNewLine();

		RefreshAccountList(RowAccount.GetUserID(), TRUE);
	}
	else
	{
		Prompter.Error("Unable to hotlist account");
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::ProcessUnhotlist(CSQLRowAccountFull& RowAccount)
{
	CString strComment = "";

	CString strName = RowAccount.GetAccountDisplayName();

	if (strName == "")
	{
		strName = "account";
	}

	CString strCaption = "";
	strCaption.Format("Unhotlist UserID %s", 
		(const char*) System.FormatCardNo(RowAccount.GetUserIDString()));

	CCommentDlg dlg(strCaption);
	dlg.m_strMessage.Format("Remove %s from hotlist", 
		(const char*) strName);

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CSQLTranUnhotlistAccount Tran;
	Tran.BeginTrans();
	Tran.DoWork(RowAccount.GetUserID(), dlg.m_strComment);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		Tran.AfterTransaction();
		RefreshAccountList(RowAccount.GetUserID(), TRUE);
		break;

	default:
		Tran.ShowErrorMessage("Unhotlist account");
		break;
	}
}

//$$******************************************************************
void CSmartPayManagerDlg::OnPopupReplace()			{ HotlistReplace(); }
void CSmartPayManagerDlg::OnPopupHotlistReplace()	{ HotlistReplace(); }
//$$******************************************************************

void CSmartPayManagerDlg::HotlistReplace()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID > 0)
	{
		ReplaceRecord(nUserID);
	}
}

//$$******************************************************************
//$$ Record already exists & read onoly use by MIFAREv1 interface

void CSmartPayManagerDlg::ReplaceRecord ( __int64 nUserIDFrom )
{
	CString strTitle = "Hotlist / Replace";
	
	CSQLRowAccountFull RowAccountFrom;
	RowAccountFrom.SetUserID(nUserIDFrom);
	
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccountFrom, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		Prompter.DatabaseAccessError();
		return;
	}

	bool bNeedHotlist = FALSE;
	int nHotlistCode = 0;
	CString strHotlistComment = "";
	
	if (RowAccountFrom.IsAlertSet() == FALSE)		
	{
		CHotlistDlg dlg(RowAccountFrom, this);

		if (dlg.DoModal() == IDCANCEL)
		{
			return;
		}

		bNeedHotlist = TRUE;
		strHotlistComment = dlg.m_strComment;
		nHotlistCode = dlg.GetReason();
	}

	do
	{
		CString strMsg = "";
		strMsg.Format("Specify the replacement for UserID %s", 
			(const char*) System.FormatCardNo(nUserIDFrom, TRUE));

		CCardNoDlg dlg(strMsg, "", this);
		dlg.m_bBrowseButtonAllowed = FALSE;

		if (dlg.DoModal() == IDCANCEL)
		{
			break;
		}

		__int64 nUserIDTo = _atoi64(dlg.GetUserID());
		if (nUserIDFrom == nUserIDTo)
		{
			continue;
		}

		bool bNeedNewAccount = FALSE;
		bool bDestAccountOK = TRUE;

		CSQLRowAccountFull RowAccountTo;
		RowAccountTo.SetUserID(nUserIDTo);

		CSQLRepositoryAccount RepoAccount;
		switch (RepoAccount.SelectRow(RowAccountTo, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_NOMATCH:
			bNeedNewAccount = TRUE;
			break;

		case SQLCRUD_ERR_NONE:
			if (RowAccountTo.IsAlertSet() == TRUE)
			{
				strMsg.Format("Cannot replace with UserID %s as it is already hotlisted", 
					(const char*) System.FormatCardNo(nUserIDTo, TRUE));

				MessageBeep(MB_ICONEXCLAMATION);
				Prompter.Error(strMsg, strTitle);
				bDestAccountOK = FALSE;
			}
			else if (RowAccountTo.IsRecordEmpty() == FALSE)
			{
				CString strCardNo = System.FormatCardNo(nUserIDTo, FALSE);
				strMsg.Format("The record for destination UserID %s is not empty.\n\nDo you still wish to use UserID %s?",
					(const char*)strCardNo, (const char*)strCardNo);

				MessageBeep(MB_ICONQUESTION);
				if (Prompter.YesNo(strMsg, strTitle) == IDNO)
				{
					bDestAccountOK = FALSE;
				}
			}
			break;

		default:
			bDestAccountOK = FALSE;
			Prompter.DatabaseAccessError();
			break;
		}

		if (FALSE == bDestAccountOK)
		{
			continue;
		}

		CRecordXferPropertySheet dlgXfer(FormatInt64Value(nUserIDFrom), FormatInt64Value(nUserIDTo), TRUE, this);
		if (dlgXfer.DoModal() == IDOK)
		{
			CRecordXferData RecordXferData;
			dlgXfer.GetRecordXferData(RecordXferData);

			CSQLTranXFerAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RecordXferData, bNeedNewAccount, TRUE, bNeedHotlist, nHotlistCode, strHotlistComment, nUserIDFrom, nUserIDTo);;

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				RefreshAccountList(nUserIDTo, TRUE);
				MessageBeep(MB_ICONINFORMATION);
				Prompter.Info("Replacement complete", strTitle);
				break;

			default:
				Tran.ShowErrorMessage("Transfer Account");
				break;
			}

			break;
		}
	}
	while (TRUE);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupTrace()
{
	CHotlistTraceDlg dlg ( m_SQLRowContextMenu );
	dlg.DoModal();
	RefreshAccountList ( _atoi64(dlg.m_strRequestedCardNo), TRUE );
}

//$$******************************************************************
//$$ Popup alert not used for MiFare-v1

void CSmartPayManagerDlg::OnPopupAlert()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID > 0)
	{
		AlertRecord(nUserID);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::AlertRecord ( __int64 nUserID )
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);
	
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		Prompter.DatabaseAccessError();
		return;
	}
	
	CAlertTextSelectDlg dlg ( this );
	dlg.m_nAlertCode = RowAccount.GetAlertCode();

	if (dlg.DoModal() == IDOK)
	{
		//RELOAD ROW IN CASE IT HAS CHANGED WHILE IN DIALOG
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			Prompter.DatabaseAccessError();
			return;
		}

		RowAccount.SetUserAlert(dlg.m_nAlertCode);

		if (RepoAccount.UpdateRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			Prompter.Error("Unable to update Alert Code", "Database Access");
		}

		RefreshAccountList(nUserID, TRUE);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnButtonAlert()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowAccount.IsAlertSet() == FALSE)
			{
				ReplaceRecord(nUserID);
			}
			else
			{
				if (RowAccount.HaveNextUserID() == FALSE)
				{
					ReplaceRecord(nUserID);	// allow replace
				}
				else
				{
					ProcessUnhotlist(RowAccount);	// allow unhotlist
				}
			}
		}
	}
	else
	{
		AlertRecord(nUserID);
	}
}

//$$******************************************************************
//$$******************************************************************

__int64 CSmartPayManagerDlg::RefreshAccountList(bool bReloadRows)
{
	__int64 nUserID = -1;

	int nRow = m_list.GetCurSel();

	if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
	{
		CSQLRowAccountList ListRow;
		m_SQLRowSetAccountList.GetRow(nRow, ListRow);
		nUserID = ListRow.GetUserID();
	}

	return RefreshAccountList(nUserID, TRUE);
}

//$$******************************************************************

__int64 CSmartPayManagerDlg::RefreshAccountList(__int64 nUserID, bool bReloadRows)
{
	if (TRUE == bReloadRows)
	{
		m_SQLRowSetAccountList.SelectRows(nUserID);
	}

	int nListRow = m_SQLRowSetAccountList.GetUserIDPos();

	UpdateTitle();
	m_list.SetItemCountEx(m_SQLRowSetAccountList.GetSize());
	m_list.SetCurSel(nListRow >= 0 ? nListRow : 0);
	m_list.Invalidate();
	SetMenuOptions();

	return GetSelectedListUserID();
}

//$$******************************************************************

__int64 CSmartPayManagerDlg::GetSelectedListUserID()
{
	__int64 nResult = -1;

	int nRow = m_list.GetCurSel();
	if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
	{
		CSQLRowAccountList RowList;
		m_SQLRowSetAccountList.GetRow(nRow, RowList);
		nResult = RowList.GetUserID();
	}

	return nResult;
}

//$$******************************************************************

bool CSmartPayManagerDlg::CheckHaveAccountRows()
{
	bool bResult = TRUE;

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.CountRows(NULL).GetSQLResult() == 0)
	{
		Prompter.Error("You must create some accounts before\nyou can use this option");
		bResult = FALSE;
	}

	return bResult;
}

//$$******************************************************************

void CSmartPayManagerDlg::GetPassword ( bool bStartup )
{
	int nOldPassword = Passwords.GetPasswordInUse();

	CPasswordDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		if (bStartup == TRUE)
		{
			CSSAutoShutdownDialog::OnCancel();
		}
	}
	else if (Passwords.IsValid(dlg.m_strPassword) == -1)
	{
		CSSAutoShutdownDialog::OnCancel();
	}
	else
	{
		if ((FALSE == bStartup) && (nOldPassword >= 0))
		{
			PasswordLogger.LogEvent(nOldPassword, PASSWORD_EVENT_LOGOFF_MENU);
		}
		
		if (TRUE == bStartup)
		{
			PasswordLogger.LogEvent(Passwords.GetPasswordInUse(), PASSWORD_EVENT_LOGON_SYSTEM);
		}
		else
		{
			PasswordLogger.LogEvent(Passwords.GetPasswordInUse(), PASSWORD_EVENT_LOGON_MENU);
		}
	}

	CString strUserName = Passwords.GetUserName(Passwords.GetPasswordInUse());

	if (strUserName == "")
	{
		if (Passwords.GetPasswordInUse() > 0)
		{
			strUserName.Format("User %d", Passwords.GetPasswordInUse());
		}
		else
		{
			strUserName = "None";
		}
	}

	Sysset.CreateProgUserFile(strUserName);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileTerminallist()
{
	CAutoShutdownDlgStacker Stacker(this);

	CEposTerminalsDlg dlg ( this );
	dlg.DoModal();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileWebPaymentLinkAccounts()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		CWebPaymentLinkAccounts web ( this );
		web.LinkAccounts();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileWebPaymentImport()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CSmartPayLockManager LockManager;
	int nLockError = LockManager.AcquireLocks( Request, this );

	if ( LOCKMANAGER_OK == nLockError )						
	{
		// new imports
		CWebPaymentImport web ( this );
		bool bTriedSendUpdates = FALSE;
		web.ImportPayments ( TRUE, bTriedSendUpdates );						
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileWebPaymentEndofDay()				// do end of day transactions\balances
{
	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PEBBLE:
		return;
	}

	CString strMsg = "";
	CString strTitle = "End of Day Balances";
	int nMessageType = MB_ICONINFORMATION;						// assume all ok

	{
		CSmartPayLockRequest Request;
		Request.m_bWantLockBgnd = TRUE;
		Request.m_bWantLockServerInner = TRUE;
		
		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks( Request, this );

		if (LOCKMANAGER_OK == nLockError)
		{
			bool bReply = FALSE;
			int nBalanceCount = 0;

			switch (System.GetWebPaymentType())
			{
			case nWEBPAYMENT_BROMCOM:
			{
				CWebPaymentExportBromCom web(this);
				bReply = web.SendAccountUpdates(TRUE);				// Export end of dat, false reply = no export attempted
				strMsg = web.GetError();							// check for an error message
				nBalanceCount = web.GetBalanceCount();
			}
			break;

			default:
			{
				CWebPaymentExport web(this);
				bReply = web.SendAccountUpdates(TRUE);				// Export end of dat, false reply = no export attempted
				strMsg = web.GetError();							// check for an error message
				nBalanceCount = web.GetBalanceCount();
			}
			break;
			}

			if ((TRUE == bReply) && (strMsg == ""))
			{
				strMsg.Format("%d Balance lines sent", nBalanceCount);
			}
			else
			{
				nMessageType = MB_ICONSTOP;	// error reported
			}
		}
	}

	if ( strMsg != "" )
	{
		Prompter.DisplayMessage( strMsg, strTitle, nMessageType );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileWebPaymentBalances()				// Send balances for all accounts
{
	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PEBBLE:
		return;
	}

	CString strMsg = "";
	CString strTitle = "Export Balances";
	int nMessageType = MB_ICONINFORMATION;						// assume all ok

	{
		CSmartPayLockRequest Request;
		Request.m_bWantLockBgnd = TRUE;
		Request.m_bWantLockServerInner = TRUE;
		
		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks( Request, this );

		if ( LOCKMANAGER_OK == nLockError )							
		{
			int nBalanceCount = 0;

			switch (System.GetWebPaymentType())
			{
			case nWEBPAYMENT_BROMCOM:
			{
				CWebPaymentExportBromCom web(this);
				web.SendAccountBalances();								// Balances
				strMsg = web.GetError();								// check for an error message
				nBalanceCount = web.GetBalanceCount();
			}
			break;

			default:
			{
				CWebPaymentExport web(this);
				web.SendAccountBalances();								// Balances
				strMsg = web.GetError();								// check for an error message
				nBalanceCount = web.GetBalanceCount();
			}
			break;
			}

			if (strMsg == "")
			{
				strMsg.Format("%d Balance lines sent", nBalanceCount);
			}
			else
			{
				nMessageType = MB_ICONEXCLAMATION;	// error reported
			}
		}
	}

	if ( strMsg != "" )
	{
		Prompter.DisplayMessage( strMsg, strTitle, nMessageType );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnWebPaymentLog()
{
	CWebPaymentImportLogDlg dlg ( System.GetWebPaymentType(), this );
	dlg.DoModal();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPebbleBalances()
{
	CReportPebbleBalances Report(this);
	Report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPebbleLinkTimes()
{
	CReportPebbleLinkTimes Report(this);
	Report.ShowFromDialog();
}

//$$******************************************************************

void CSmartPayManagerDlg::OnExportSalestopups()
{
	CString strMsg = "";
	CString strTitle = "Export Sales & Topups";
	int nMessageType = MB_ICONINFORMATION;						// assume all ok

	{
		CSmartPayLockRequest Request;
		Request.m_bWantLockBgnd = TRUE;
		Request.m_bWantLockServerInner = TRUE;
		
		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks(Request, this);
		
		if ( LOCKMANAGER_OK == nLockError )							
		{
			CWebPaymentExport web ( this );
			web.SpecialSalesExport();

			int nCount = web.GetTransactionCount();					// get number of lines sent
			if ( nCount == - 1 )
			{
				strMsg = web.GetError();							// check for an error message
				nMessageType = MB_ICONSTOP;							// error reported
			}
			else
			{
				strMsg.Format ( "%d new lines exported", nCount ); 
			}
		}
	}

	Prompter.DisplayMessage( strMsg, strTitle, nMessageType );
}

//$$******************************************************************
void CSmartPayManagerDlg::OnPopupTopup()		{ TopupPurse ( 1 );	}
void CSmartPayManagerDlg::OnPopupTopup2()	{ TopupPurse ( 2 );	}
void CSmartPayManagerDlg::OnPopupTopup3()	{ TopupPurse ( 3 );	}
//$$******************************************************************

void CSmartPayManagerDlg::TopupPurse(int nPurse)
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID > 0)
	{
		//if (::IsAccountInuse(strCardNo) == FALSE)
		//{

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount RepoAccount;
		RepoAccount.SelectRow(RowAccount,NULL);

		CAddCashDlg dlg(nPurse);
		switch (nPurse)
		{
		case 3:
			dlg.m_dPurseBalance = RowAccount.GetPurse3Liability();
			break;	// current Purse3

		case 2:
			dlg.m_dPurseBalance = RowAccount.GetPurse2Balance();
			break;	// current Purse2

		default:
			dlg.m_dPurseBalance = RowAccount.GetPurse1Liability();
			break;	// current Purse1
		}

		if (dlg.DoModal() == TRUE)
		{
			double dValue = dlg.m_dValue;
			if (dValue != 0.0)
			{
				//if (::IsAccountInuse(strCardNo) == FALSE)
				{
					if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						switch (nPurse)
						{
						case 3:
							RowAccount.AddToPurse3Liability(dValue);
							break;	// update record's purse3

						case 2:
							RowAccount.AddToPurse2Balance(dValue);
							break;	// update record's purse2

						default:
							RowAccount.AddToPurse1Liability(dValue);
							break;	// update record's purse1
						}

						RepoAccount.UpdateRow(RowAccount,NULL);
						
						CSQLAuditRecord atc;
						atc.Set(&RowAccount);

						switch (nPurse)
						{
						case 3:
							atc.SetTransactionPurse3Liability(dValue);
							break;

						case 2:
							atc.SetTransactionPurse2Balance(dValue);
							break;

						default:
							atc.SetTransactionPurse1Liability(dValue);
							break;
						}

						atc.SetOperatorID(Passwords.GetPasswordInUse());
						atc.SetApplicationNo(APPNO_ADDCASH);
						atc.SetLineComment(dlg.m_strComment);
						atc.SaveNewLine();
					}

					RefreshAccountList(nUserID, TRUE);
				}
				//else
				//{
				//	DisplayAccountInUse(strCardNo);
				//}
			}
		}
	}
	//else
	//{
	//DisplayAccountInUse(strCardNo);
	//}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupMeal()
{
	__int64 nUserID = GetSelectedListUserID();

	if (nUserID > 0)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			COfficeMealDlg dlg(RowAccount, this);
			if (dlg.DoModal() == TRUE)
			{
				if (dlg.UpdateAccount() == TRUE)
				{
					RefreshAccountList(RowAccount.GetUserID(), TRUE);
				}
			}
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupGroupShift()
{
	if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
	{
		__int64 nUserID = GetSelectedListUserID();

		if (nUserID > 0)
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(nUserID);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				COleDateTime oleDateToday = COleDateTime::GetCurrentTime();

				CGroupShiftValidator GroupShiftValidator(oleDateToday, RowAccount);
				if (GroupShiftValidator.UpdateAccount(RowAccount) == TRUE)
				{
					CString strError = "";
					if (CSQLHelpers::UpdateAccountWithVersionCheck(RowAccount, strError) == FALSE)
					{
						Prompter.Error(strError);
						return;
					}
				}

				GroupShiftValidator.CopyEditedToOriginal();

				CAutoShutdownDlgStacker Stacker(this);

				CGroupShiftDlg dlg(RowAccount, GroupShiftValidator, this);
				if (dlg.DoModal() == TRUE)
				{
					RefreshAccountList(RowAccount.GetUserID(), TRUE);
				}
			}
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnPopupCredit()
{
	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		__int64 nUserID = GetSelectedListUserID();

		if (nUserID > 0)
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(nUserID);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				CCreditAdjustDlg dlg(RowAccount, this);
				if (dlg.DoModal() == TRUE)
				{
					if (dlg.UpdateAccount(RowAccount) == TRUE)
					{
						RefreshAccountList(nUserID, TRUE);
					}
				}
			}
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileArchive()
{
	CSmartPayAuditFile auditFile( Filenames.GetAuditFilename() );
	auditFile.WriteArchive ( this );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileTimetable()
{
	CBackgroundTimeTable timetable;
	timetable.Show ( this );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileBgndService()
{
	CAutoShutdownDlgStacker Stacker(this);

	CSSFile fileLock;
	if ( fileLock.Open( Filenames.GetBgndServiceOptionsLockFilename(), "wb" ) == FALSE )
	{
		Prompter.Error( "These options are being modified by another user.\n\nPlease try again later." );
	}
	else
	{
		BgndServiceOptions.Read();
		CServiceInstallDlg dlg( TRUE, this );
		dlg.DoModal();
		BgndServiceOptions.Write();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileBgndServiceLog()
{
	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.DisplayReport( Filenames.GetBackgroundLogFilename(), Filenames.GetBackgroundLogParamsFilename(), "" );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileServerService()
{
	CAutoShutdownDlgStacker Stacker(this);

	CSSFile fileLock;
	if ( fileLock.Open( Filenames.GetServerServiceOptionsLockFilename(), "wb" ) == FALSE )
	{
		Prompter.Error( "These options are being modified by another user.\n\nPlease try again later." );
	}
	else
	{
		ServerServiceOptions.Read();
		CServiceInstallDlg dlg( FALSE, this );
		dlg.DoModal();
		ServerServiceOptions.Write();
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileServerServiceLog()
{
	CRepmanHandler RepmanHandler( "", this );
	RepmanHandler.DisplayReport( Filenames.GetServerLogFilename(), Filenames.GetServerLogParamsFilename(), "" );
}

//$$******************************************************************

void CSmartPayManagerDlg::OnEmailLowbalance()
{
	CEmailLowBalances email (this );
	email.Execute();						

	if ( EmailOptions.UseSMTPForDirectEmail() == TRUE )
	{
		SSspawnl ( this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnEmailPurchases()
{
	CEmailPurchases email ( this );
	email.Execute();						

	if ( EmailOptions.UseSMTPForDirectEmail() == TRUE )
	{
		SSspawnl ( this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnEmailPointsachievement()
{
	CEmailPointsAchievement email ( this );
	email.Execute ();						

	if ( EmailOptions.UseSMTPForDirectEmail() == TRUE )
	{
		SSspawnl ( this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnEmailWeeklypoints()
{
	CEmailWeeklyPoints email ( this );
	email.Execute();						

	if ( EmailOptions.UseSMTPForDirectEmail() == TRUE )
	{
		SSspawnl ( this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL );
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnEmailLog()
{
	if ( ::FileExists ( Filenames.GetEmailLogAllFilename() ) == TRUE )
	{
		CRepmanHandler RepmanHandler( "", this );
		RepmanHandler.DisplayReport( Filenames.GetEmailLogAllFilename(), Filenames.GetEmailLogParamsFilename(), "" );
	}
	else
	{
		Prompter.Error("The Email Log file was not found.");
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnAuditEposPayments()
{
	CPaymentAuditReport report ( Filenames.GetPaymentAuditFilename(), this );
	report.Show();
}

//$$******************************************************************

void CSmartPayManagerDlg::DisplayAccountInUse ( const char* szUserID )
{
	CString strMsg = "";
	strMsg.Format ( "Account %s is already in use - request ignored!", szUserID );
	Prompter.Error( strMsg, "Database Access" );
}

//$$******************************************************************

void CSmartPayManagerDlg::CSSAutoShutdownTimer( UINT nIDEvent )
{
	if (1 == nIDEvent)
	{
		if (AutoShutdownHelper.IsEnabled() == TRUE)
		{
			CString strInfo2Text = GetAutoShutdownSecondsText();

			if (strInfo2Text != m_strCurrentInfo2Text)
			{
				m_staticInfo2.SetWindowText(strInfo2Text);
				m_strCurrentInfo2Text = strInfo2Text;
			}
		}
		
		if (GetTickCount64() >= m_LastFullTimerCheck + 5000)
		{
			m_LastFullTimerCheck = GetTickCount64();

			if (EmailOptions.UseSMTPForDirectEmail() == TRUE)
			{
				if (COleDateTime::GetCurrentTime() > m_oleTimeNextEmailCheck)
				{
					SSspawnl(this, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL);
					m_oleTimeNextEmailCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 30);
				}

				m_nPendingEmailCount = 0;

				CSSFile fileEmailCount;
				if (fileEmailCount.Open(Filenames.GetEmailPendingCountFilename(), "rb") == TRUE)
				{
					CString strBuffer;
					if (fileEmailCount.ReadLine(strBuffer) == TRUE)
					{
						m_nPendingEmailCount = atoi(strBuffer);
					}
				}
			}
			else
			{
				m_nPendingEmailCount = 0;
			}

			if (COleDateTime::GetCurrentTime() > m_oleTimeNextGroupShiftCheck)
			{
				SQLGroupShiftChecker.CheckGroupShifts();
				m_oleTimeNextGroupShiftCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 1, 0);
			}

			UpdateTitle();
		}
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnFileExit()
{
	ExitProgram(FALSE);
}

//$$******************************************************************

void CSmartPayManagerDlg::OnCancel()
{
	ExitProgram(FALSE);
}

//$$******************************************************************

void CSmartPayManagerDlg::CSSAutoShutdownLogOff()
{
	ExitProgram(TRUE);
}

//$$******************************************************************

void CSmartPayManagerDlg::ExitProgram(bool bAutoShutdown)
{
	if (ExitAllowed() == TRUE)
	{
		ExportDatabase();
		
		CRect rect;
		GetWindowRect(rect);
		CColumnHandler column;
		column.Save("MAIN1", rect.Height() - Y_ADJUSTMENT, nColumnCount, m_list);

		if (m_pBackupResultDlg != NULL)
		{
			m_pBackupResultDlg->DestroyWindow();
		}

		if (TRUE == bAutoShutdown)
		{
			ShowWindow(SW_HIDE);
			
			__int64 nMinutes = AutoShutdownHelper.GetIdleMinutesAllowed();
			AutoShutdownHelper.SetFinalShutdownSeconds(5);

			CString strMsg;
			strMsg.Format("SmartPay Manager automatic shutdown\nafter %I64d minutes inactivity.", nMinutes);
			
			CMyMessageBoxInfo infoMessage;
			infoMessage.SetIconType(MB_ICONINFORMATION);
			infoMessage.SetButtonMode(MB_OK);
			infoMessage.SetLineGap(PresentationOptions.GetCustomMessageBoxLineGap());
			infoMessage.SetFontSize(PresentationOptions.GetCustomMessageBoxFont());
			infoMessage.SetBackColour(PresentationOptions.GetCustomMessageBoxColourBackInt());
			infoMessage.SetTextColour(PresentationOptions.GetCustomMessageBoxColourTextInt());
			infoMessage.SetTitle(::GetTidyAppName(FALSE) );
			infoMessage.SetTexts(strMsg);

			CMyMessageBoxDlg dlg(infoMessage, this);
			dlg.DoModal();

			AutoShutdownHelper.Terminate();
		}

		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************

bool CSmartPayManagerDlg::ExitAllowed()
{
	return TRUE;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnBackupSystem()
{
	m_bDoingAutoBackupCheck = TRUE; 
	AutoBackupProcessor.ManualBackup(this);
	m_bDoingAutoBackupCheck = FALSE;

	if (TRUE == AutoBackupProcessor.GetWantQuitFlag())
	{
		OnClose();
	}

	else if (TRUE == AutoBackupProcessor.GetLoggedResultFlag())
	{
		ShowBackupResult(this);
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnBackupLog()
{
	if (::FileExists(Filenames.GetBackupLogToReadFilename()) == TRUE)
	{
		CRepmanHandler RepmanHandler("", this);
		RepmanHandler.DisplayReport(Filenames.GetBackupLogToReadFilename(), Filenames.GetBackupLogParamsFilename(), "", FALSE);
	}
	else
	{
		Prompter.Error("The Backup Log file was not found.");
	}
}

//$$******************************************************************

void CSmartPayManagerDlg::OnRestoreExisting()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(FALSE,FALSE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnRestoreScript()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(FALSE, TRUE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CSmartPayManagerDlg::OnRestoreSeparate()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(TRUE,FALSE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CSmartPayManagerDlg::HandleFileRestore(bool bFolder,bool bFromScript)
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "restore the system";
	infoLock.m_strStopCommsPrompt = "Restore system";
	infoLock.m_bStopSmartPay = TRUE;
	infoLock.m_pParentWnd = this;

	CString strError = "";
	bool bAttempted = FALSE;
	bool bComplete = FALSE;
	int nResult = 0;

	{
		CLockManager LockManager(infoLock);

		if (LockManager.GetLock() == TRUE)
		{
			nResult = RestoreInternal(strError, bComplete, infoLock, bFolder, bFromScript);
			bAttempted = TRUE;
		}
	}

	if (TRUE == bAttempted)
	{
		switch (nResult)
		{
		case 0:
		{
			CString strMsg = "";
			strMsg += "The restore was completed successfully.";
			strMsg += "\n\nPlease restart SmartPay Manager after closing this message.";
			Prompter.Info(strMsg);
		}
		break;

		case 1:
			Prompter.Error("Unable to find current working directory");
			break;

		case 2:
			Prompter.Error("Unable to change the working directory");
			break;

		case 3:
			Prompter.Error(strError);
			break;

		case 4:
			Prompter.Error("The restore folder already exists");
			break;
		}

		if (TRUE == bComplete)
		{
			//SysInfo.Read();
			PostMessage(WM_CLOSE);
		}
	}
}

/**********************************************************************/

int CSmartPayManagerDlg::RestoreInternal(CString& strError, bool& bComplete, CLockManagerInfo& infoLock, bool bRestoreToFolder, bool bFromScript)
{
	strError = "";
	bComplete = FALSE;

	CString strCwd = "";
	CString strDataPath = Sysset.GetDataPath();

	if (TRUE == bRestoreToFolder)
	{
		CString strDateTime;
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		strDateTime.Format("RESTORED_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());

		strDataPath += strDateTime;

		if (::ExistSubdirectory(strDataPath) == TRUE)
		{
			return 4;
		}

		::CreateSubdirectory(strDataPath);
	}

	if (strDataPath != "")
	{
		char cBuffer[_MAX_PATH];
		if (_getcwd(cBuffer, _MAX_PATH) == NULL)
		{
			return 1;
		}

		strCwd = cBuffer;

		if (_chdir(strDataPath) != 0)
		{
			return 2;
		}
	}

	System.SystemLock();
	Prompter.SetPreventParentEnableFlag(TRUE);
	AutoShutdownHelper.SetTimerBlock1(TRUE);
	CSystemBackup MyBackup(FALSE, bRestoreToFolder, bFromScript, infoLock);
	AutoShutdownHelper.SetTimerBlock1(FALSE);
	Prompter.SetPreventParentEnableFlag(FALSE);
	System.SystemUnlock();
	strError = MyBackup.GetOnScreenError();
	bComplete = MyBackup.IsComplete();

	if (strDataPath != "")
	{
		_chdir(strCwd);
	}

	if (strError != "")
	{
		return 3;
	}

	return 0;
}

/**********************************************************************/

void CSmartPayManagerDlg::OnBackupStatus()
{
	if (NULL == m_pBackupResultDlg)
	{
		m_pBackupResultDlg = new CBackupResultDlg(&m_pBackupResultDlg, this);
		m_pBackupResultDlg->Create(IDD_BACKUP_RESULT, this);
	}

	m_pBackupResultDlg->SetFocus();
	m_pBackupResultDlg->UpdateDisplay();
}

//$$******************************************************************

void CSmartPayManagerDlg::ShowBackupResult(CWnd* pParent, bool bDestroyOld)
{
	if (TRUE == bDestroyOld)
	{
		if (m_pBackupResultDlg != NULL)
		{
			m_pBackupResultDlg->DestroyWindow();
		}
	}

	if (m_pBackupResultDlg == NULL)
	{
		m_pBackupResultDlg = new CBackupResultDlg(&m_pBackupResultDlg, pParent);
		m_pBackupResultDlg->Create(IDD_BACKUP_RESULT, pParent);
	}

	m_pBackupResultDlg->SetFocus();
	m_pBackupResultDlg->UpdateDisplay();
}

/***********************************************************************/

long CSmartPayManagerDlg::OnAutoBackup(WPARAM, LPARAM)
{
	if (PCOptionsHost.GetBackupAccessType() == 0)
	{
		if (FALSE == m_bDoingAutoBackupCheck)
		{
			m_bDoingAutoBackupCheck = TRUE;	
			AutoBackupProcessor.CheckAutoBackup(this);
			m_bDoingAutoBackupCheck = FALSE;

			if (AutoBackupProcessor.GetWantQuitFlag() == TRUE)
			{
				OnClose();
			}
			else if (AutoBackupProcessor.GetLoggedResultFlag() == TRUE)
			{
				ShowBackupResult(this);
			}
		}
	}

	return 0;
}

/***********************************************************************/

BOOL CSmartPayManagerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (FALSE == m_bDoingAutoBackupCheck)
	{
		return CDialog::OnCommand(wParam, lParam);
	}
	else
	{
		return FALSE;
	}
}

/***********************************************************************/

LRESULT CSmartPayManagerDlg::OnKickIdle(WPARAM, LPARAM)
{
	if (AutoShutdownHelper.IsTriggered() == FALSE)
	{
		if (COleDateTime::GetCurrentTime() >= m_oleLastAutoBackupCheck + COleDateTimeSpan(0, 0, 0, 10))
		{
			if (PCOptionsHost.GetBackupAccessType() == 0)
			{
				SendMessage(WM_AUTOBACKUP, 0, 0);
			}

			m_oleLastAutoBackupCheck = COleDateTime::GetCurrentTime();
		}
	}

	return FALSE;
}

/***********************************************************************/

void CSmartPayManagerDlg::OnButtonRestore()
{
	if (FALSE == m_bShowRestoreFromScript)
	{
		m_bShowRestoreFromScript = TRUE;
		SetMenuOptions();
	}
}

//$$******************************************************************