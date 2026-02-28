//$$******************************************************************
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\PresentationOptions.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//$$******************************************************************
#include "AboutDlg.h"
#include "AddCashDlg.h"
#include "AuditReportWrapper.h"
#include "AutoBackupOptions.h"
#include "AutoBackupProcessor.h"
#include "BatchUpdate.h"
#include "CalculateBonus.h"
#include "ColumnListDlg.h"
#include "CommentDlg.h"
#include "DatabaseExport.h"
#include "DatabaseImportForManager.h"
#include "DatabaseReportsDlg.h"
#include "DemoWarning.h"
#include "EditLoyaltyRecord.h"
#include "EmailOptions.h"
#include "FindDlg.h"
#include "FindSingleDlg.h"
#include "FontTable.h"
#include "GroupComboHelpers.h"
#include "HotlistDlg.h"
#include "HotlistTraceDlg.h"
#include "ImportDataHandler.h"
#include "ListDataGroupDlg.h"
#include "ListDataStaffCardDlg.h"
#include "ListDataStaffGiftExceptionDlg.h"
#include "ListDataStampOfferInfoDlg.h"
#include "PasswordData.h"
#include "PasswordDlg.h"
#include "PasswordLogger.h"
#include "PCOptions.h"
#include "PhotoIDDlg.h"
#include "PurchaseReport.h"
#include "RecordTransferDlg.h"
#include "RepmanHandler.h"
#include "ReportAccumulator.h"
#include "ReportBirthday.h"
#include "ReportCashlessLiability.h"
#include "ReportCashlessPosting.h"
#include "ReportDailyDifference.h"
#include "ReportDatabase.h"
#include "ReportGroupPeriodSales.h"
#include "ReportPluRanking.h"
#include "ReportPhotoIDList.h"
#include "ReportStartingBalance.h"
#include "ReportVisitTotaliser.h"
#include "SelectAlertDlg.h"
#include "ServerPropertySheet.h"
#include "ServiceInstallDlg.h"
#include "SQLTranBatchAdd.h"
#include "SQLTranDeleteAccount.h"
#include "SQLTranUnhotlistAccount.h"
#include "SQLTranXferAccount.h"
#include "StampOfferReportWrapper.h"
#include "SystemBackup.h"
#include "SystemPropertySheet.h"
#include "UpgradeHandler.h"
#include "WMNumbers.h"
#include "WorkingDlg.h"
//$$******************************************************************
#include "LoyaltyManagerDlg.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************
#define	WM_USER_COLUMN		( WM_USER + 200 )
#define	WM_DEMO_WARNING		( WM_USER + 201 )
#define Y_ADJUSTMENT		16
#define X_ADJUSTMENT		105
//$$******************************************************************
static int nColumnCount = 9;
//$$******************************************************************

CLoyaltyManagerDlg::CLoyaltyManagerDlg(CWnd* pParent)
	: CSSDialog(CLoyaltyManagerDlg::IDD, pParent)
	, m_ColumnManager(110)
{
	m_bInitDialog = FALSE;
	m_bBlockAutoRefresh = FALSE;
	m_bShowRestoreFromScript = FALSE;
	m_strNextDefaultCardNo = "";
	m_nNextDefaultUserID = 0;

	m_pBackupResultDlg = NULL;

	m_strCurrentInfo1Text = "";
	m_strCurrentInfo2Text = "";

	m_oleLastAutoBackupCheck = COleDateTime(2011, 1, 1, 0, 0, 0);
	m_oleLastAutoRefreshTime = COleDateTime( 2011, 1, 1, 0, 0, 0 );
	m_bDoingAutoRefresh = FALSE;
	m_bDoingAutoBackupCheck = FALSE;

	m_bTitleShowsCannotAccess = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hTimer = NULL;

	Passwords.GetPassword(0);							// load passwords
}

//$$******************************************************************

CLoyaltyManagerDlg::~CLoyaltyManagerDlg()
{
	if (m_pBackupResultDlg != NULL)
	{
		delete m_pBackupResultDlg;
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_HOTLIST, m_buttonHotlist);
	DDX_Control(pDX, IDC_BUTTON_TRANSFER, m_buttonTransfer);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CLoyaltyManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ACCOUNTS, OnColumnClickList)
	ON_MESSAGE( WM_USER_COLUMN, OnColumnClick )
	ON_MESSAGE( WM_DEMO_WARNING, OnDemoWarning)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACCOUNTS, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ACCOUNTS, OnGetDispInfoList)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_WM_TIMER()
	/****/
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_HOTLIST, OnButtonHotlist)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFER, OnButtonTransfer)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
	ON_COMMAND(IDM_POPUP_ADD, OnButtonAdd)
	ON_COMMAND(IDM_POPUP_EDIT, OnButtonEdit)
	ON_COMMAND(IDM_POPUP_TRANSFER, OnButtonTransfer)
	ON_COMMAND(IDM_POPUP_DELETE, OnButtonDelete)
	/****/
	ON_COMMAND(IDM_FILE_UPGRADE, OnFileUpgrade)
	ON_COMMAND(IDM_FILE_SIGNOFF, OnFileSignoff)
	ON_COMMAND(IDM_FILE_SERVER_SERVICE, OnFileServerService)
	ON_COMMAND(IDM_FILE_SERVER_SERVICELOG, OnFileServerServiceLog)
	ON_COMMAND(IDM_FILE_REFRESH, OnFileRefresh)
	ON_COMMAND(IDM_FILE_EXIT, OnFileExit)
	/****/
	ON_COMMAND(IDM_BACKUP_SYSTEM, OnBackupSystem)
	ON_COMMAND(IDM_RESTORE_EXISTING, OnRestoreExisting)
	ON_COMMAND(IDM_RESTORE_SEPARATE, OnRestoreSeparate)
	ON_COMMAND(IDM_RESTORE_SCRIPT, OnRestoreScript)
	ON_COMMAND(IDM_BACKUP_LOG, OnBackupLog)
	ON_COMMAND(IDM_BACKUP_STATUS, OnBackupStatus)
	/****/
	ON_COMMAND( IDM_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND( IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND( IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND( IDM_COLUMN_DEFAULT, OnColumnDefault)
	/****/
	ON_COMMAND(IDM_REPORTS_DATABASE, OnReportsDatabase)
	ON_COMMAND(IDM_REPORTS_PHOTOIDLIST, OnReportsPhotoIDList)
	ON_COMMAND(IDM_REPORTS_HOTLIST, OnReportsHotlist)
	ON_COMMAND(IDM_REPORTS_BIRTHDAYS, OnReportsBirthdays)
	ON_COMMAND(IDM_REPORTS_AUDIT_AUDIT, OnReportsAuditAudit)
	ON_COMMAND(IDM_REPORTS_AUDIT_CASHTAKEN, OnReportsAuditCashTaken)
	ON_COMMAND(IDM_REPORTS_AUDIT_PURSEADDITIONS, OnReportsAuditPurseAdditions)
	ON_COMMAND(IDM_REPORTS_AUDIT_CARDSALES, OnReportsAuditCardSales)
	ON_COMMAND(IDM_REPORTS_AUDIT_POINTS_AUDIT, OnReportsAuditPointsAudit)
	ON_COMMAND(IDM_REPORTS_AUDIT_POINTS_ADDED, OnReportsAuditPointsAdded)
	ON_COMMAND(IDM_REPORTS_AUDIT_POINTS_DEDUCTED, OnReportsAuditPointsDeducted)
	ON_COMMAND(IDM_REPORTS_AUDIT_BONUS_REVALUE, OnReportsAuditBonusRevalue)
	ON_COMMAND(IDM_REPORTS_AUDIT_BONUS_SPEND, OnReportsAuditBonusSpend)
	ON_COMMAND(IDM_REPORTS_AUDIT_BONUS_REDEEM, OnReportsAuditBonusRedeem)
	ON_COMMAND(IDM_REPORTS_AUDIT_REFUNDS, OnReportsAuditRefunds)
	ON_COMMAND(IDM_REPORTS_AUDIT_SUMMARY, OnReportsAuditSummary)
	ON_COMMAND(IDM_REPORTS_AUDIT_ARCHIVESEARCH, OnReportsAuditArchiveSearch)
	ON_COMMAND(IDM_REPORTS_PURCHASE_HISTORY, OnReportsPurchaseHistory)
	ON_COMMAND(IDM_REPORTS_PURCHASE_RANKING, OnReportsPurchaseRanking)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_ACTIVE, OnReportsCardUsageActive)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_INACTIVE, OnReportsCardUsageInactive)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_NEVERUSED, OnReportsCardUsageNeverUsed)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_LOWPOINTS, OnReportsCardUsageLowPoints)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_RANKINGPOINTS, OnReportsCardUsageRankingPoints)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_LOWSPEND, OnReportsCardUsageLowSpend)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_RANKINGSPEND, OnReportsCardUsageRankingSpend)
	ON_COMMAND(IDM_REPORTS_STARTINGBALANCE, OnReportsStartingBalance)
	ON_COMMAND(IDM_REPORTS_GROUPPERIODSALES, OnReportsGroupPeriodSales)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_DIFFER, OnReportsCardUsageDiffer)
	ON_COMMAND(IDM_REPORTS_CARDUSAGE_TOTALISER, OnReportsCardUsageTotaliser)
	ON_COMMAND(IDM_REPORTS_CASHLESS_POSTING, OnReportsCashlessPosting)
	ON_COMMAND(IDM_REPORTS_CASHLESS_LIABILITY, OnReportsCashlessLiability)
	ON_COMMAND(IDM_REPORTS_STAMPOFFER, OnReportsStampOffer)
	/*****/
	ON_COMMAND(IDM_BATCH_ADD, OnBatchAdd)
	ON_COMMAND(IDM_BATCH_UPDATE, OnBatchUpdate)
	ON_COMMAND(IDM_BATCH_REDEEM, OnBatchRedeem)
	ON_COMMAND(IDM_BATCH_DELETE, OnBatchDelete)
	/*****/
	ON_COMMAND(IDM_FILE_IMPORT, OnFileImport)
	ON_COMMAND(IDM_FILE_EXPORT, OnFileExport)
	ON_COMMAND(IDM_FILE_ARCHIVE, OnFileArchive)
	ON_COMMAND(IDM_FILE_STAFFGIFT, OnFileStaffGift)
	/*****/
	ON_COMMAND(IDM_SETUP_SYSTEM, OnSetupSystem)
	ON_COMMAND(IDM_SETUP_SERVER, OnSetupServer)
	ON_COMMAND(IDM_SETUP_GROUPS, OnSetupGroups)
	ON_COMMAND(IDM_SETUP_STAFFCARDS, OnSetupStaffCards)
	ON_COMMAND(IDM_SETUP_STAMPOFFERS, OnSetupStampOffers)
	/*****/
	ON_COMMAND(IDM_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDM_HELP_LICENCE, OnHelpLicence)
	/*****/
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_POPUP_HOTLIST, OnPopupHotlist)
	ON_COMMAND(IDM_POPUP_UNHOTLIST, OnPopupUnhotlist)
	ON_COMMAND(IDM_POPUP_HOTLISTREPLACE, OnPopupHotlistReplace)
	ON_COMMAND(IDM_POPUP_REPLACE, OnPopupReplace)
	ON_COMMAND(IDM_POPUP_TRACE, OnPopupTrace)
	ON_COMMAND(IDM_POPUP_PURCHASEHISTORY, OnPopupPurchaseHistory)
	ON_COMMAND(IDM_POPUP_PHOTOID, OnPopupPhotoID)
	ON_COMMAND(IDM_POPUP_ALERT, OnPopupAlert)
	ON_COMMAND(IDM_POPUP_TOPUP_PURSE1, OnPopupTopupPurse1)
	ON_COMMAND(IDM_POPUP_TOPUP_PURSE2, OnPopupTopupPurse2)
	ON_COMMAND(IDM_POPUP_AUDIT, OnPopupAudit)
	ON_COMMAND(IDM_POPUP_AUDIT_SUMMARY, OnPopupAuditSummary)
	ON_COMMAND(IDM_POPUP_STAMPOFFER, OnPopupStampOffer)
	/*****/
	ON_MESSAGE(WM_AUTOBACKUP, OnAutoBackup)
	/*****/

END_MESSAGE_MAP()

//$$******************************************************************

BOOL CLoyaltyManagerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	bool bResult = FALSE;
	
	if (FALSE == m_bDoingAutoBackupCheck)
	{
		m_bBlockAutoRefresh = TRUE;
		bResult = CSSDialog::OnCommand(wParam, lParam);
		m_bBlockAutoRefresh = FALSE;
	}

	return bResult;
}

//$$******************************************************************

BOOL CLoyaltyManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetWindowText(::GetTidyAppName(FALSE));

	CDC* pDC = GetDC();
	FontTable.BuildFontTable(pDC);

	m_bInitDialog = TRUE;
	SetIcon(m_hIcon, TRUE);			// Set big icon

	m_listAccounts.SubclassDlgItem(IDC_LIST_ACCOUNTS, this);
	m_listAccounts.EnableColour();

	CUserTexts texts;
	texts.Read();
	CString strText = "";

	m_ColumnManager.Reset(TableNames::AccountList);
	m_ColumnManager.AddColumn(ColumnNames::AccountNo, "Account", LVCFMT_LEFT, 54, 54, COLUMN_STATUS_LOCKED);
	m_ColumnManager.AddColumn(ColumnNames::UserName, "User Name", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_SHOWN);

	strText = texts.GetText(LOYALTYFIELD_ADDRESS1, "Address1");
	m_ColumnManager.AddColumn(ColumnNames::Address1, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	strText = texts.GetText(LOYALTYFIELD_ADDRESS5, "Postcode");
	m_ColumnManager.AddColumn(ColumnNames::Address5, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	m_ColumnManager.AddColumn(ColumnNames::Group, "Grp", LVCFMT_LEFT, 30, 30, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::SpendTD, "SpendTD", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::PointsTD, "PointsTD", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_SHOWN);
	m_ColumnManager.AddColumn(ColumnNames::Purse1, "Purse1", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Purse2, "Purse2", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Balance, "Balance", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.AddColumn(ColumnNames::Points, "Points", LVCFMT_RIGHT, 100, 100, COLUMN_STATUS_HIDDEN);

	strText = texts.GetText(LOYALTYFIELD_INFO1, "Info1");
	m_ColumnManager.AddColumn(ColumnNames::Info1, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	strText = texts.GetText(LOYALTYFIELD_INFO2, "Info2");
	m_ColumnManager.AddColumn(ColumnNames::Info2, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	strText = texts.GetText(LOYALTYFIELD_INFO3, "Info3");
	m_ColumnManager.AddColumn(ColumnNames::Info3, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	strText = texts.GetText(LOYALTYFIELD_INFO4, "Info4");
	m_ColumnManager.AddColumn(ColumnNames::Info4, strText, LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);

	m_ColumnManager.AddColumn(ColumnNames::Status, "Status", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN);
	m_ColumnManager.LoadSettings();

	m_ColumnManager.RedrawList(&m_listAccounts, this);

	// setup menu changer
	m_MenuChanger.EatMenuTopLevel(GetMenu());

	// load basic account information
	m_SQLRowSetAccountList.SelectRows(0);

	CImportDataHandler import(this);
	import.ScanAll();									// check for auto csv imports

	if ((System.GetSignOnMethod() != 0) && (Passwords.InUse() == TRUE))
	{
		GetPassword(TRUE);
	}
	else
	{
		Sysset.CreateProgUserFile("Master");
	}

	RefreshAccountList(0, FALSE);							// do first screen & set menu options

	m_hTimer = SetTimer(1, 1000, 0);
	
	PostMessage(WM_DEMO_WARNING);

	return TRUE;
}

//$$******************************************************************

__int64 CLoyaltyManagerDlg::RefreshAccountList(bool bReloadRows)
{
	__int64 nUserID = -1;

	int nRow = m_listAccounts.GetCurSel();

	if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
	{
		CSQLRowAccountList ListRow;
		m_SQLRowSetAccountList.GetRow(nRow, ListRow);
		nUserID = ListRow.GetACListUserID();
	}

	return RefreshAccountList(nUserID, bReloadRows);
}

//$$******************************************************************

__int64 CLoyaltyManagerDlg::RefreshAccountList(__int64 nUserID, bool bReloadRows)
{
	if (TRUE == bReloadRows)
	{
		m_SQLRowSetAccountList.SelectRows(nUserID);
	}
	else
	{
		m_SQLRowSetAccountList.FindUserID(nUserID);
	}

	int nListRow = m_SQLRowSetAccountList.GetUserIDPos();

	UpdateTitle();
	m_listAccounts.SetItemCountEx(m_SQLRowSetAccountList.GetSize());
	m_listAccounts.SetCurSel(nListRow >= 0 ? nListRow : 0);
	m_listAccounts.Invalidate();
	SetMenuOptions();

	return GetSelectedListUserID();
}

//$$******************************************************************

bool CLoyaltyManagerDlg::CheckHaveAccountRows()
{
	bool bResult = TRUE;

	CSQLRepositoryAccount RepoAccount;
	CSQLResultInfo Result = RepoAccount.CountRows(NULL);

	if (Result.GetSQLError() != SQLCRUD_ERR_NONE)
	{
		Prompter.DatabaseAccessError();
		bResult = FALSE;
	}
	else if (Result.GetSQLResult() == 0)
	{
		Prompter.Error("You must create some accounts before\nyou can use this option");
		bResult = FALSE;
	}
	
	return bResult;
}

//$$******************************************************************

__int64 CLoyaltyManagerDlg::GetSelectedListUserID()
{
	__int64 nResult = -1;

	int nRow = m_listAccounts.GetCurSel();
	if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
	{
		CSQLRowAccountList RowList;
		m_SQLRowSetAccountList.GetRow(nRow, RowList);
		nResult = RowList.GetACListUserID();
	}

	return nResult;
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize = CPoint(500, 400); // minimum tracking width and height
	CDialog::OnGetMinMaxInfo(lpMMI);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		
		CRect rect1( 71, 10, 180, 24 );
		MapDialogRect( &rect1 );

		CRect rect2( 182, 10, 173, 24 );
		MapDialogRect( &rect2 );
		
		m_listAccounts.MoveWindow ( rect1.left, Y_ADJUSTMENT, cx-(rect1.left+10), cy-(Y_ADJUSTMENT+50) );
		
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

void CLoyaltyManagerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnDestroy()
{
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	CSSDialog::OnDestroy();
}

//$$******************************************************************

HCURSOR CLoyaltyManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//$$******************************************************************

LONG CLoyaltyManagerDlg::OnDemoWarning(WPARAM wParam, LONG lParam)
{
	CDemoWarning DemoWarning;
	return 0L;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_COLUMN, nIndex );	
	*pResult = 0;
}

//$$******************************************************************

LONG CLoyaltyManagerDlg::OnColumnClick ( WPARAM wParam, LONG lParam )
{
	int nIndex = (int) wParam;

	switch ( m_ColumnManager.GetListColumnType( nIndex ) )
	{
	case COLUMN_ACCOUNTNO:	ColumnClickCardNo();		break;		// find card no
	case COLUMN_USERNAME:	
	case COLUMN_ADDRESS1:
	case COLUMN_ADDRESS5:	ColumnClickUserName();		break;		// find card name
	}

	return 0L;
}

//$$******************************************************************

void CLoyaltyManagerDlg::ColumnClickUserName()
{
	CFindDlg dlg{};
	if ( dlg.DoModal() == IDOK )
	{
		RefreshAccountList ( dlg.m_nRequestedCardNo, FALSE );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::ColumnClickCardNo()
{
	CFindSingleBrowseDlg dlg ( this );
	
	while ( dlg.DoModal() != IDCANCEL )
	{
		CString strUserID = dlg.GetOutputCardNo();
		CString strMiFareNo	= dlg.GetOutputMiFareNo();			// MiFare number if MiFare-2 format

		if (strUserID != "")									// may be blank if MiFare-2 & no entry in MiFare database
		{
			__int64 nUserID = _atoi64(strUserID);

			CSQLRepositoryAccount RepoAccount;
			CSQLResultInfo Result = RepoAccount.RowExists(nUserID, NULL);

			if (Result.GetSQLError() != SQLCRUD_ERR_NONE)
			{
				Prompter.DatabaseAccessError();
				continue;
			}

			if (Result.GetSQLResult() != 0)
			{ 
				EditRecord(nUserID);
				break;
			}

			if ( (Server.GetSimpsInnsLookupFlag() == TRUE) && ( strUserID.Left(4) == "1033"))
			{
				CSQLRecordSetAccountUserID RecordSet(NULL,
					RSParams_AccountUserId_NormalByInfo2{ strUserID });
					
				bool bGotUserID = RecordSet.StepSelect(nUserID, FALSE);

				if (RecordSet.IsOpen())
				{
					RecordSet.Close();
				}

				if (TRUE == bGotUserID)
				{
					EditRecord(nUserID);
					break;
				}
			}
		}

		CString strNotFoundMsg = "";
		if (strMiFareNo != "")
		{
			strNotFoundMsg.Format("MiFare card %s", 
				(const char*) strMiFareNo);
		}
		else
		{
			strNotFoundMsg.Format("CardNo %s", 
				(const char*) System.FormatCardNo(strUserID));
		}

		CString strError = "";
		strError.Format ( "No database record found for %s", 
			(const char*) strNotFoundMsg);

		Prompter.Error( strError, "Find Account" );

		dlg.ClearInput();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnButtonEdit();
	*pResult = 0;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nRow = pDispInfo->item.iItem;

		if ((nRow >= 0) && (nRow < m_SQLRowSetAccountList.GetSize()))
		{
			CSQLRowAccountList ListRow;
			m_SQLRowSetAccountList.GetRow(nRow, ListRow);

			if (ListRow.GetACListDataLoadedFlag() == FALSE)
			{
				CSQLRecordSetAccountList RecordSet(NULL, 
					RSParams_AccountList_NormalById{SimpleFormatInt64Value(ListRow.GetACListUserID())});
			
				if (RecordSet.StepSelectAll(ListRow) == TRUE)
				{
					ListRow.SetACListDataLoadedFlag(TRUE);
					m_SQLRowSetAccountList.SetRow(nRow, ListRow);
				}
				else
				{
					m_SQLRowSetAccountList.GetRow(nRow, ListRow);
					ListRow.SetACListDataLoadedFlag(FALSE);
				}
			}

			CString strText = "";
			int nColumnType = m_ColumnManager.GetListColumnType(pDispInfo->item.iSubItem);
			
			if (COLUMN_ACCOUNTNO == nColumnType)
			{
				strText = System.FormatCardNo(ListRow.GetACListUserID(), TRUE);
				m_listAccounts.SetColour(nRow, ListRow.GetACListRecordStatusColour());
			}
			else if (ListRow.GetACListDataLoadedFlag() == TRUE)
			{
				switch (nColumnType)
				{
				case COLUMN_ACCOUNTNO:
					break;

				case COLUMN_USERNAME:
					strText = ListRow.GetACListFullname();
					break;

				case COLUMN_GROUP:
					strText = ListRow.GetACListGroupNoString();
					break;

				case COLUMN_INFO1:
					strText = ListRow.GetACListInfo1();
					break;

				case COLUMN_INFO2:
					strText = ListRow.GetACListInfo2();
					break;

				case COLUMN_INFO3:
					strText = ListRow.GetACListInfo3();
					break;

				case COLUMN_INFO4:
					strText = ListRow.GetACListInfo4();
					break;

				case COLUMN_ADDRESS1:
					strText = ListRow.GetACListAddress1();
					break;

				case COLUMN_ADDRESS5:
					strText = ListRow.GetACListAddress5();
					break;

				case COLUMN_PURSE1:
					strText = ListRow.GetACListPurseBalanceString(1);
					break;

				case COLUMN_PURSE2:
					strText = ListRow.GetACListPurseBalanceString(2);
					break;

				case COLUMN_BALANCE:
					strText = ListRow.GetACListTotalBalanceString();
					break;

				case COLUMN_POINTS:
					strText = ListRow.GetACListPointsString();
					break;

				case COLUMN_POINTSTD:
					strText = ListRow.GetACListPointsToDateString();
					break;

				case COLUMN_SPENDTD:
					strText = ListRow.GetACListSpendToDateString();
					break;

				case COLUMN_STATUS:
					strText = ListRow.GetACListRecordStatusText();
					break;

				default:
					strText = "";
					break;
				}
			}

			lstrcpy(pDispInfo->item.pszText, strText);
		}
	}
	*pResult = 0;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnTimer(UINT nIDEvent)
{
	if (1 == nIDEvent)
	{
		int nRefreshRate = System.GetListRefreshRate();
		if ((nRefreshRate > 0) && (nRefreshRate <= 60))
		{
			if (COleDateTime::GetCurrentTime() >= m_oleLastAutoRefreshTime + COleDateTimeSpan(0, 0, 0, nRefreshRate))
			{
				AutoRefresh();
			}
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::AutoRefresh()
{
	if ( ( FALSE == m_bDoingAutoRefresh ) && ( FALSE == m_bBlockAutoRefresh ) )
	{
		m_bDoingAutoRefresh = TRUE;
		RefreshDatabase();
		m_bDoingAutoRefresh = FALSE;
		m_oleLastAutoRefreshTime = COleDateTime::GetCurrentTime();
	}
}

//$$******************************************************************

__int64 CLoyaltyManagerDlg::RefreshDatabase()
{
	__int64 nUserID = GetSelectedListUserID();

	CImportDataHandler import ( this );
	import.ScanAll();									
	
	RefreshAccountList ( nUserID, TRUE );					
	return nUserID;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnCancel()		
{ 
	ExitProgram(); 
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnButtonAdd()
{
	__int64 nNewCardNo = 1;

	if (0 == m_nNextDefaultUserID)
	{
		CSQLRepositoryAccount RepoAccount;
		CSQLResultInfo Result = RepoAccount.GetHighestCurrentUserID(NULL);

		if (Result.GetSQLError() != SQLCRUD_ERR_NONE)
		{
			Prompter.DatabaseAccessError();
			return;
		}

		nNewCardNo = Result.GetSQLResult() + 1;
	}
	else
	{
		nNewCardNo = m_nNextDefaultUserID + 1;
	}

	if ((nNewCardNo < 1) || (nNewCardNo > Account::UserID.Max))
	{
		nNewCardNo = 1;
	}

	CFindSingleAddOneDlg dlg ( SimpleFormatInt64Value(nNewCardNo), this);

	if ( dlg.DoModal() == IDCANCEL )
	{
		return;
	}

	nNewCardNo = _atoi64(dlg.GetOutputCardNo());

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
			(const char*) System.FormatCardNo(nNewCardNo, TRUE));

		Prompter.Error(strMsg, "Add Database Record");
	}
	break;

	case SQLCRUD_ERR_EXCEPT_DB:
	case SQLCRUD_ERR_EXCEPT_MISC:
	{
		Prompter.DatabaseAccessError();
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
				(const char*) System.FormatCardNo(nNewCardNo, TRUE));

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

void CLoyaltyManagerDlg::OnButtonEdit()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		EditRecord(nUserID);
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::EditRecord ( __int64 nCardNo )
{
	CEditLoyaltyRecord EditRecord;
	EditRecord.EditRecord( ACCOUNTEDIT_DATABASE, nCardNo, this );
	RefreshAccountList(nCardNo, TRUE);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnButtonDelete()
{
	if (CheckHaveAccountRows() == FALSE)
	{
		return;
	}

	__int64 nUserID = GetSelectedListUserID();

	if (nUserID >= 0)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		{
			CString strMsg = "";
			CSQLRepositoryAccount RepoAccount;
			switch (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError())
			{
			case SQLCRUD_ERR_NONE:
			case SQLCRUD_ERR_DUPLICATE:
				break;

			case SQLCRUD_ERR_EXCEPT_DB:
			case SQLCRUD_ERR_EXCEPT_MISC:
				Prompter.DatabaseAccessError();
				return;

			default:
				strMsg.Format("Cannot find record for account ' %s ' in database!", 
					(const char*) SimpleFormatInt64Value(nUserID));

				Prompter.Error(strMsg, "Delete Account");
				return;
			}
		}

		CString strCardDetails = "";
		{
			CString strCardNo = SimpleFormatInt64Value(nUserID);
			CString strFullname = RowAccount.GetFullname();
			
			if (strFullname != "")
			{
				strCardDetails.Format("%s ( %s )", 
					(const char*) System.FormatCardNo(strCardNo), 
					(const char*) strFullname);
			}
			else
			{
				strCardDetails = System.FormatCardNo(strCardNo);
			}
		}

		CCommentDlg dlg("Delete Account");
		dlg.m_strMessage.Format("Delete account %s from the database?", 
			(const char*) strCardDetails);

		if (dlg.DoModal() == IDOK)
		{
			DeleteRecord(RowAccount, dlg.m_strComment);
			RefreshAccountList(nUserID, TRUE);
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::DeleteRecord(CSQLRowAccountFull& RowAccount, const char* szComment)
{
	CSQLTranDeleteAccount Tran;
	Tran.BeginTrans();
	Tran.DoWork(RowAccount,szComment);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		Tran.AfterTransaction();
		break;

	default:
		Tran.ShowErrorMessage("Delete Account");
		break;
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnButtonHotlist()
{
	__int64 nUserID = GetSelectedListUserID();

	if ( nUserID <= 0 )
	{
		return;
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		switch( System.GetInterfaceType() )
		{
		case nINTERFACE_MIFAREv2:
		case nINTERFACE_MIFAREv3:
			HotlistCard(RowAccount);							// allow hotlist / unhotlist only
			break;

		default:
			if (RowAccount.IsCardHotlisted() == FALSE)
			{
				HotlistReplaceCard(RowAccount);					// hotlist & replace
			}
			else
			{
				if (RowAccount.HaveNextUserID() == FALSE)
				{
					HotlistReplaceCard(RowAccount);	// allow replace
				}
				else
				{
					HotlistCard(RowAccount);			// allow unhotlist
				}
			}
			break;
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::HotlistCard(CSQLRowAccountFull& RowAccount)
{
	int nAppNo = 0;
	int nHotlistCode = 0;
	CString strComment = "";

	int nDlgReply = IDCANCEL;
	if (RowAccount.IsCardHotlisted() == FALSE)
	{
		nHotlistCode = 1;
		nAppNo = APPNO_HOTLIST;

		CHotlistDlg dlg(RowAccount);
		
		nDlgReply = dlg.DoModal();

		if ( IDOK == nDlgReply )
		{
			strComment = dlg.m_strComment;
		}
	}
	else
	{
		nAppNo = APPNO_UNHOTLIST;

		CString strCardDetails = "";
		if (RowAccount.GetFullname() != "")
		{
			strCardDetails.Format("%I64d ( %s )",
				RowAccount.GetUserID(), 
				(const char*) RowAccount.GetFullname());
		}
		else
		{
			strCardDetails = RowAccount.GetUserIDString();
		}

		CCommentDlg dlg("Unhotlist Account");
		dlg.m_strMessage.Format("Remove account %s from the Hotlist", 
			(const char*) strCardDetails);
		
		if ((nDlgReply = dlg.DoModal()) == IDOK)
		{
			strComment = dlg.m_strComment;
		}
	}

	if (nDlgReply == IDOK)
	{
		RowAccount.SetHotlistCard(nHotlistCode);

		CAuditRecord atc;
		atc.SetSQL(RowAccount);				// audit hotlisted details
		atc.SetApplicationNo(nAppNo);
		atc.SetComment(strComment);
		atc.SaveLineToFile();

		if (nAppNo == APPNO_UNHOTLIST)
		{
			bool bHavePrevUserID = RowAccount.HavePreviousUserID();
			bool bHaveNextUserID = RowAccount.HaveNextUserID();
			__int64 nPrevUserID = RowAccount.GetPreviousUserID();
			__int64 nNextUserID = RowAccount.GetNextUserID();

			RowAccount.SetNextUserID(0);
			RowAccount.SetPreviousUserID(0);

			if (bHavePrevUserID == TRUE)		// see if it was a replacement
			{
				CSQLRowAccountFull RowPrev;
				RowPrev.SetUserID(nPrevUserID);
				
				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowPrev, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					RowPrev.SetUserID(nNextUserID);
					RepoAccount.UpdateRow(RowPrev,NULL);
				}
			}

			if (bHaveNextUserID == TRUE)
			{
				CSQLRowAccountFull RowNext;
				RowNext.SetUserID(nNextUserID);

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowNext, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					RowNext.SetPreviousUserID(nPrevUserID);
					RepoAccount.UpdateRow(RowNext,NULL);
				}
			}
		}
	}

	{
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.UpdateRow(RowAccount,NULL);
		RefreshAccountList(RowAccount.GetUserID(), TRUE);
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::ProcessHotlistCard(CSQLRowAccountFull& RowAccount)
{
	CHotlistDlg dlg(RowAccount);

	if (dlg.DoModal() == IDOK)
	{
		RowAccount.SetHotlistCard(1);

		CAuditRecord atc;
		atc.SetSQL(RowAccount);				// audit hotlisted details
		atc.SetApplicationNo(APPNO_HOTLIST);
		atc.SetComment(dlg.m_strComment);
		atc.SaveLineToFile();

		CSQLRepositoryAccount RepoAccount;
		RepoAccount.UpdateRow(RowAccount, NULL);
		RefreshAccountList(RowAccount.GetUserID(), TRUE);
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::ProcessUnhotlistCard(CSQLRowAccountFull& RowAccount)
{
	CString strCardDetails = "";
	if (RowAccount.GetFullname() != "")
	{
		strCardDetails.Format("%I64d ( %s )", 
			RowAccount.GetUserID(), 
			(const char*) RowAccount.GetFullname());
	}
	else
	{
		strCardDetails = RowAccount.GetUserIDString();
	}

	CCommentDlg dlg("Unhotlist Account");
	dlg.m_strMessage.Format("Remove account %s from the Hotlist", 
		(const char*) strCardDetails);

	if (dlg.DoModal() != IDOK)
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
		Tran.ShowErrorMessage("Unhotlist Account");
		break;
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::HotlistReplaceCard(CSQLRowAccountFull& RowAccount)
{
	__int64 nUserIDFrom = RowAccount.GetUserID();
	__int64 nUserIDTo = nUserIDFrom;

	CString strTitle = "";
	strTitle.Format ( "Hotlist / Replace - Account %I64d", nUserIDFrom );

	bool bNeedHotlist = FALSE;
	CString strHotlistComment = "";

	if (RowAccount.IsCardHotlisted() == FALSE)		// see if already hotlisted
	{
		CHotlistDlg dlg(RowAccount);

		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		bNeedHotlist = TRUE;
		strHotlistComment = dlg.m_strComment;
	}
		
	do
	{
		CFindSingleHotReplaceDlg dlg(SimpleFormatInt64Value(nUserIDFrom), this);

		if (dlg.DoModal() == IDCANCEL)
		{
			break;
		}

		nUserIDTo = _atoi64(dlg.GetOutputCardNo());

		if (nUserIDFrom == nUserIDTo)
		{
			continue;
		}

		CString strMsg = "";
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
			if (RowAccountTo.IsCardHotlisted() == TRUE)
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

		CRecordTransferDlg dlgXfer ( RowAccount, RowAccountTo, TRUE);
		if (dlgXfer.DoModal() == IDOK)
		{
			CRecordTransferInfo RecordTransferInfo;
			dlgXfer.GetRecordTransferInfo(RecordTransferInfo);

			CSQLTranXFerAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RecordTransferInfo, bNeedNewAccount, TRUE, bNeedHotlist, strHotlistComment, nUserIDFrom, nUserIDTo);;

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				RefreshAccountList(nUserIDTo, TRUE);
				MessageBeep(MB_ICONINFORMATION);
				Prompter.Info("Replacement complete", strTitle);
				break;

			default:
				Tran.ShowErrorMessage("Hotlist Replace");
				break;
			}

			break;
		}
	}
	while ( TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnButtonTransfer()
{
	__int64 nUserIDFrom = GetSelectedListUserID();

	if (nUserIDFrom <= 0)
	{
		return;
	}

	__int64 nUserIDTo = nUserIDFrom;
	
	CString strTitle = "Card Transfer";

	CSQLRowAccountFull RowFrom;
	RowFrom.SetUserID(nUserIDFrom);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowFrom, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	do
	{
		CFindSingleTransferDlg dlg(RowFrom.GetUserIDString(), this);

		if (dlg.DoModal() == IDCANCEL)
		{
			break;
		}

		nUserIDTo = _atoi64(dlg.GetOutputCardNo());
		if (nUserIDFrom == nUserIDTo)
		{
			continue;
		}

		bool bNeedCreate = FALSE;

		CSQLRowAccountFull RowTo;
		RowTo.SetUserID(nUserIDTo);

		CString strToCardNo = System.FormatCardNo(SimpleFormatInt64Value(nUserIDTo));

		if (RepoAccount.SelectRow(RowTo, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			CString strMsg = "";
			strMsg.Format("No record exists for recipient account %s.\n\nDo you wish to create one?", (const char*)strToCardNo);
			MessageBeep(MB_ICONQUESTION);
			if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDNO)
			{
				continue;
			}

			bNeedCreate = TRUE;
		}
		else
		{
			if (RowTo.IsCardHotlisted() == TRUE)
			{
				CString strMsg;
				strMsg.Format("The recipient account %s is hotlisted.  Do you still wish to continue with the transfer?", (const char*)strToCardNo);
				MessageBeep(MB_ICONQUESTION);
				if (Prompter.YesNo(strMsg, strTitle) == IDNO)
				{
					continue;
				}
			}
		}

		CRecordTransferDlg dlgXfer(RowFrom, RowTo, FALSE);

		if (dlgXfer.DoModal() == IDOK)
		{
			CRecordTransferInfo RecordTransferInfo;
			dlgXfer.GetRecordTransferInfo(RecordTransferInfo);

			CSQLTranXFerAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RecordTransferInfo, bNeedCreate, FALSE, FALSE, "", RowFrom.GetUserID(), RowTo.GetUserID());

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				RefreshAccountList(nUserIDTo, TRUE);
				MessageBeep(MB_ICONINFORMATION);
				Prompter.Info("Transfer complete", strTitle);
				break;

			default:
				Tran.ShowErrorMessage("Transfer Acconut");
				break;
			}
		}

		break;
	}
	while ( TRUE );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnFileUpgrade()
{
	RefreshAccountList(TRUE);

	if (m_SQLRowSetAccountList.GetSize() != 0)
	{
		Prompter.Info("You cannot upgrade data from a previous version because\nthe loyalty database already contains new records.");
		return;
	}

	CUpgradeHandler version(this);
	if (version.Upgrade() == TRUE)				
	{
		RefreshAccountList(TRUE);
		SetMenuOptions();					
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileSignoff()
{
	GetPassword(FALSE);
	SetMenuOptions();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileRefresh()
{
	RefreshDatabase();
	SetMenuOptions();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileExit()	
{ 
	ExitProgram(); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::ExitProgram()
{
	if ( ExitAllowed() == TRUE )
	{
		CRect rect;
		GetWindowRect( rect );
		CSSDialog::OnCancel();
	}
}

//$$******************************************************************

bool CLoyaltyManagerDlg::ExitAllowed()
{
	ExportDatabase();
	return TRUE;
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnColumnEdit()
{
	m_ColumnManager.RememberCurrentLine( &m_listAccounts, this );
	CArray<CColumnStatusInfo,CColumnStatusInfo> arrayPos;
	m_ColumnManager.GetListColumnDetails( arrayPos );

	CColumnListDlg dlg( "Account List", arrayPos, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_ColumnManager.SetListColumnDetails( arrayPos );
		m_ColumnManager.RedrawList( &m_listAccounts, this );
		m_ColumnManager.SaveCurrentLine( &m_listAccounts, this );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnColumnSave()
{
	m_ColumnManager.SaveCurrentLine( &m_listAccounts, this );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnColumnRestore()
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listAccounts, this );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnColumnDefault()
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listAccounts, this );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsDatabase()
{
	RefreshDatabase();

	CDatabaseReportsDlg dlg;
	while ( dlg.DoModal() == IDOK )
	{
		CDatabaseReport report ( dlg.GetReportLabel(), this );
		report.Show ( dlg.m_bEditReqd );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsPhotoIDList()
{
	RefreshDatabase();

	CReportPhotoIDList report( this );
	while ( report.Display() == TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsHotlist()		
{	
	DatabaseReport ( "R001" );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsBirthdays()
{
	RefreshDatabase();

	CBirthdayReport report ( this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditAudit()			
{ 
	ShowAuditReport ( "R101", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditCashTaken()		
{ 
	ShowAuditReport ( "R109", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditPurseAdditions()	
{ 
	ShowAuditReport ( "R119", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditCardSales()			
{ 
	ShowAuditReport ( "R115", 0 );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditPointsAudit()		
{ 
	ShowAuditReport ( "R112", 0 );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditPointsAdded()		
{ 
	ShowAuditReport ( "R113", 0 );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditPointsDeducted()	
{ 
	ShowAuditReport ( "R114", 0 );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditBonusRevalue()	
{ 
	ShowAuditReport ( "R102", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditBonusSpend()		
{ 
	ShowAuditReport ( "R103", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditBonusRedeem()		
{ 
	ShowAuditReport ( "R110", 0 );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditRefunds()			
{ 
	ShowAuditReport ( "R120", 0 ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditSummary()
{
	ShowAuditReport("R132", 0);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsAuditArchiveSearch()
{
	RefreshDatabase();
	CAuditReportWrapper Wrapper( Filenames.GetAuditFilename(), "R122", 0, this);
	Wrapper.ShowArchive();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsPurchaseHistory()
{
	RefreshDatabase();
	CPurchaseReport report ( "R104", this );
	report.Show(0);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsPurchaseRanking()
{
	RefreshDatabase();
	CReportPluRanking report ( this );
	report.Show();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageActive()	
{ 
	AccumulatorReport ( "R007");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageInactive()	
{ 
	AccumulatorReport ( "R008");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageNeverUsed()	
{	
	DatabaseReport ( "R010" );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageLowPoints()		
{ 
	AccumulatorReport ( "R003");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageRankingPoints()	
{ 
	AccumulatorReport ( "R005");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageLowSpend()		
{ 
	AccumulatorReport ( "R004");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageRankingSpend()	
{ 
	AccumulatorReport ( "R006");	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageDiffer()
{
	RefreshDatabase();
	CReportDailyDifference report ( this );
	report.Show();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCardUsageTotaliser()
{
	RefreshDatabase();
	CReportVisitTotaliser report ( this );
	report.Show();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsStartingBalance()
{
	RefreshDatabase();

	CReportStartingBalance report ( Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsGroupPeriodSales()
{
	RefreshDatabase();
	CGroupPeriodSalesReport report ( "R118", Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCashlessPosting()
{
	RefreshDatabase();
	CReportCashlessPosting report ( "R125", this );
	report.Show();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsCashlessLiability()
{
	RefreshDatabase();
	CReportCashlessLiability report ( "R124", this );
	report.Show();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnReportsStampOffer()
{
	RefreshDatabase();
	CStampOfferReportWrapper Wrapper("R129", 0, this);
	Wrapper.ShowReport();
}

//$$******************************************************************

void CLoyaltyManagerDlg::DatabaseReport ( const char* szLabel  )
{
	RefreshDatabase();
	CDatabaseReport report ( szLabel, this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::ShowAuditReport ( const char* szLabel, __int64 nUserID )
{
	RefreshDatabase();
	CAuditReportWrapper Wrapper( Filenames.GetAuditFilename(), szLabel, nUserID, this);
	Wrapper.ShowReport();
}

//$$******************************************************************

void CLoyaltyManagerDlg::AccumulatorReport ( const char* szLabel ) 
{
	RefreshDatabase();
	CReportAccumulator report ( szLabel, Filenames.GetAuditFilename(), this );
	while ( report.Show() == TRUE );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnBatchAdd()
{
	CString strTitle = "Batch Add Accounts";

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
			CFindSingleAddBatchDlg dlg(SimpleFormatInt64Value(nFirstNewCardNo), this);

			if (dlg.DoModal() == IDCANCEL)
			{
				break;
			}

			nFirstNewCardNo = _atoi64(dlg.GetOutputCardNo());
			nNewRecordsRequested = dlg.m_nRecordCount;
			nNewRecordsRequested = max(nNewRecordsRequested, 1);
			nNewRecordsRequested = min(nNewRecordsRequested, 999);
		}

		if (nNewRecordsRequested <= 1)
		{
			CSQLRowAccountFull AccountRow;
			AccountRow.SetUserID(nFirstNewCardNo);

			CSQLRepositoryAccount RepoAccount;
			CSQLResultInfo Result = RepoAccount.SelectRow(AccountRow, NULL);
			__int64 nSelectError = Result.GetSQLError();

			if ((SQLCRUD_ERR_NONE == nSelectError) || (SQLCRUD_ERR_DUPLICATE == nSelectError))
			{
				CString strMsg = "";
				strMsg.Format("Database record %s already exists.", 
					(const char*) System.FormatCardNo(nFirstNewCardNo, TRUE));
				Prompter.Error(strMsg, "Add Database Record");
			}
			else if (SQLCRUD_ERR_NOMATCH != nSelectError)
			{
				CString strMsg = "";
				strMsg.Format("Unable to check for existing record %s.",
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
			__int64 nSysMaxCardNo = System.GetMaxCardNo();
			__int64 nLastNewCardNo = nFirstNewCardNo + nNewRecordsRequested - 1;

			if (nLastNewCardNo > nSysMaxCardNo)
			{
				nCannotAdd = nLastNewCardNo - nSysMaxCardNo;
				nLastNewCardNo = nSysMaxCardNo;
			}

			CArray<__int64, __int64> arrayInUse;
			{
				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.GetUserIDListInRange(arrayInUse, nFirstNewCardNo, nLastNewCardNo, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					Prompter.Error("Unable to check existing rows");
					return;
				}
			}

			nCannotAdd += arrayInUse.GetSize();

			if (nCannotAdd >= (__int64)nNewRecordsRequested)
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

			__int64 nRecordsToAdd = ((__int64)nNewRecordsRequested) - nCannotAdd;

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

void CLoyaltyManagerDlg::AddRecord(__int64 nCardNo, int nAppNo, const char* szComment, bool& bSuccess, int& nNewRecordIndex)
{
	CSQLRowAccountFull NewRow;
	NewRow.SetUserID(nCardNo);

	bSuccess = FALSE;
	nNewRecordIndex = m_listAccounts.GetCurSel();

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.InsertRow(NewRow, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_SQLRowSetAccountList.SelectRows(NewRow.GetUserID());
		nNewRecordIndex = m_SQLRowSetAccountList.GetUserIDPos();
		bSuccess = TRUE;

		CAuditRecord atc;
		atc.SetSQL(NewRow);
		atc.SetApplicationNo(nAppNo);
		atc.SetComment(szComment);
		atc.SaveLineToFile();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnBatchUpdate()	
{ 
	BatchRecordUpdate ( nBATCHUPDATE_UPDATE );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnBatchDelete()			
{ 
	BatchRecordUpdate ( nBATCHUPDATE_DELETE );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnBatchRedeem()			
{ 
	BatchRecordUpdate ( nBATCHUPDATE_REDEEM );	
}

//$$******************************************************************

void CLoyaltyManagerDlg::BatchRecordUpdate ( int nUpdateType )
{
	if ( m_SQLRowSetAccountList.GetSize() > 1 )			
	{
		__int64 nUserID = GetSelectedListUserID();

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.SelectRow(RowAccount,NULL);
		
		CBatchUpdate update ( nUpdateType, RowAccount, this );
		CString strFinalMessage = update.DoUpdate();

		RefreshAccountList ( nUserID, TRUE );
		
		if (strFinalMessage != "")
		{
			Prompter.Info(strFinalMessage, update.GetTitle());
		}
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnFileImport()
{
	__int64 nUserID = GetSelectedListUserID();

	CFileDialog dlg ( TRUE, "csv", "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlg.m_ofn.lpstrTitle = "Locate Database CSV Import file";
	dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

	if ( dlg.DoModal() == IDOK )
	{
		CImportDatabaseForManager import ( dlg.GetPathName(), this );
		if ( import.IsValidFile() == TRUE )
		{
			import.UserImport();
			RefreshAccountList ( nUserID, TRUE );				// redispay last screen
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileExport()		
{ 
	ExportData ( "CSV" ); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileStaffGift()
{
	CListDataStaffGiftExceptionDlg dlg(this);
	
	if (dlg.GetExceptionCount() == 0)
	{
		Prompter.Info("There are no outstanding staff gift exceptions to process");
	}
	else
	{
		dlg.DoModal();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::ExportData ( const char* szLabel )
{
	RefreshDatabase();

	CDbExportEditor editor ( FALSE, this );
	if ( editor.EditLabel ( szLabel ) == TRUE )
	{
		CExportDatabase exp ( szLabel, this );
		exp.Execute( TRUE );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileArchive()
{
	CLoyaltyAuditFile auditFile( Filenames.GetAuditFilename() );
	auditFile.WriteArchive ( this );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnSetupSystem()
{
	if ( System.LockAllUsers() == FALSE )
	{
		Prompter.LockError( "change system settings" );
	}
	else
	{
		SetupSystemInternal();
		System.UnlockAllUsers();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::SetupSystemInternal()
{
	CSystemPropertySheet dlg ( this );

	AutoBackupOptions.RememberSettings();

	int nReply = dlg.DoModal();
	int nChangedPaxtonMode = dlg.GetChangedPaxtonMode();
	bool bChangedServer = dlg.ChangedServerSettings();

	CPresentationOptions OldPresentationOptions;
	OldPresentationOptions.CopyFrom( PresentationOptions );

	CEmailOptions OldEmailOptions;
	OldEmailOptions.CopyFrom( EmailOptions );

	CPCOptionsClient OldPCOptionsClient;
	OldPCOptionsClient.CopyFrom( PCOptionsClient );

	CPCOptionsHost OldPCOptionsHost;
	OldPCOptionsHost.CopyFrom( PCOptionsHost );

	if ( IDOK == nReply )
	{
		if (nChangedPaxtonMode != 0)
		{
			System.SetPaxtonModeFlag(2 == nChangedPaxtonMode);
		}

		System.Write();	
		System.FixInternalSettings();

		PresentationOptions.Write();
		EmailOptions.Write();
		PCOptionsClient.Write();	
		PCOptionsHost.Write();

		if (AutoBackupOptions.HasScheduleChanged())
		{
			AutoBackupProcessor.ScheduleNextBackup(FALSE);

			if ( m_pBackupResultDlg != NULL)
			{
				m_pBackupResultDlg->UpdateDisplay();
			}
		}
		AutoBackupOptions.Write();
		
		SetMenuOptions();
	}
	else
	{
		System.RewindExternal();		

		PresentationOptions.CopyFrom( OldPresentationOptions );
		EmailOptions.CopyFrom( OldEmailOptions );
		PCOptionsClient.CopyFrom( OldPCOptionsClient );
		PCOptionsHost.CopyFrom(OldPCOptionsHost);

		AutoBackupOptions.RestoreSettings();
			
		if ( nChangedPaxtonMode != 0 )
		{
			System.SetPaxtonModeFlag( 2 == nChangedPaxtonMode );
			System.Write();
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnSetupServer()
{
	if ( System.LockAllUsers() == FALSE )
	{
		Prompter.LockError( "change server settings" );
	}
	else
	{
		SetupServerInternal();
		System.UnlockAllUsers();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::SetupServerInternal()
{
	ServerCopy.CopyFrom(Server);

	Server.UpdateEcrmanDbInfo();

	CServerPropertySheet dlg ( Filenames.GetServerDataFilename(), this );
	
	if ( dlg.DoModal() == IDOK )
	{
		dlg.Save();
		SetMenuOptions();								// may change menu options
	}
	else
	{
		Server.CopyFrom( ServerCopy );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnSetupGroups()
{
	CListDataGroupDlg dlg(this);
	dlg.DoModal();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnSetupStaffCards()
{
	CListDataStaffCardDlg dlg(this);
	dlg.DoModal();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnSetupStampOffers()
{
	CWaitUpdatePluDlg dlgWait("Checking for Plu and Stamp Offer Updates", this);
	dlgWait.LockLoyalty();
	
	CListDataStampOfferInfoDlg dlgShow;
	dlgShow.DoModal();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnHelpAbout()
{
	CDemoWarning DemoWarning;
	CAboutDlg dlg;
	dlg.DoModal();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnHelpLicence()
{
	Sysset.DisplayLicenceDetails ( "", "", 0, this );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	{
		__int64 nUserID = GetSelectedListUserID();
		if (nUserID < 0)
		{
			return;
		}

		m_SQLRowContextMenu.SetUserID(nUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}
	}

	if (point.x == -1 && point.y == -1)		//keystroke invocation
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
	menuChanger.EatMenuPopup (pPopup);

	menuChanger.EnableItem(IDM_POPUP_TOPUP_PURSE1, Passwords.IsPurse1TopupButtonAllowed() );
	menuChanger.EnableItem(IDM_POPUP_TOPUP_PURSE2, Passwords.IsPurse1TopupButtonAllowed());
	menuChanger.EnableItem(IDM_POPUP_EDIT, Passwords.IsEditRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_ADD, Passwords.IsAddRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_DELETE, Passwords.IsDeleteRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_HOTLIST, Passwords.IsHotlistRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_HOTLISTREPLACE, Passwords.IsHotlistRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_TRANSFER, Passwords.IsTransferRecordAllowed() );
	menuChanger.EnableItem(IDM_POPUP_ALERT, Passwords.IsEditGeneralAllowed() );

//
	bool bIsHotlisted		= FALSE;
	bool bShowHotlist		= FALSE;
	bool bShowHotReplace	= FALSE;
	bool bShowReplace		= FALSE;
	bool bShowUnHotlist		= FALSE;
	bool bShowTrace			= FALSE;

	bIsHotlisted = m_SQLRowContextMenu.IsCardHotlisted();
	if (m_SQLRowContextMenu.HavePreviousUserID() == TRUE)			// has been replaced
	{
		bShowTrace = TRUE;
	}
	
	if (TRUE == bIsHotlisted)
	{
		bShowTrace = TRUE;
		bShowUnHotlist = TRUE;

		if (m_SQLRowContextMenu.HaveNextUserID() == FALSE)
		{
			bShowReplace = TRUE;
		}
	}
	else
	{
		bShowHotlist = TRUE;
		bShowHotReplace = TRUE;
	}

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		bShowHotReplace = FALSE;					// no replace options
		bShowReplace = FALSE;
		bShowTrace = FALSE;
		break;
	}

	menuChanger.ShowItem(IDM_POPUP_HOTLIST, bShowHotlist );
	menuChanger.ShowItem(IDM_POPUP_HOTLISTREPLACE, bShowHotReplace );
	menuChanger.ShowItem(IDM_POPUP_REPLACE, bShowReplace );
	menuChanger.ShowItem(IDM_POPUP_UNHOTLIST, bShowUnHotlist );
	menuChanger.ShowItem(IDM_POPUP_TRACE, bShowTrace );

//
	menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, FALSE );
	if ( Passwords.IsReportsAllowed() == TRUE )
	{
		Server.Read ( Filenames.GetServerDataFilename() );
		if ( Server.GetPurchaseHistoryFlag() == TRUE )
		{
			menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, TRUE );

			CString strFilename = Filenames.GetPurchaseHistoryFilename ( m_SQLRowContextMenu.GetUserIDString() );
			
			bool bEnableHistory = SolutionGlobalFunctions::FileExists(strFilename);

			if (FALSE == bEnableHistory)
			{
				strFilename = Filenames.GetPurchaseHistoryUpdateFilename(m_SQLRowContextMenu.GetUserIDString());
				bEnableHistory = SolutionGlobalFunctions::FileExists(strFilename);
			}

			menuChanger.EnableItem(IDM_POPUP_PURCHASEHISTORY, bEnableHistory );
		}
	}

	CImageHandler photo{};
	CString strFilename = photo.GetFirst ( m_SQLRowContextMenu.GetUserIDString() );
	menuChanger.EnableItem(IDM_POPUP_PHOTOID, (strFilename != ""));

	if (System.GetContextMenuPurse1TopupFlag() == FALSE)
	{
		menuChanger.ShowItem(IDM_POPUP_TOPUP_PURSE1, FALSE);
	}

	bool bShowTopupPurse2 = FALSE;	

	if (System.GetContextMenuPurse2TopupFlag() == TRUE)
	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(m_SQLRowContextMenu.GetGroupNo());

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			bShowTopupPurse2 = (RowGroup.GetRefreshType() == nREFRESHTYPE_NORMAL);
		}
	}

	menuChanger.ShowItem(IDM_POPUP_TOPUP_PURSE2, bShowTopupPurse2);
	
	menuChanger.BuildMenu ( pPopup );

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupTopupPurse1()
{
	HandlePopupTopup(FALSE);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupTopupPurse2()
{
	HandlePopupTopup(TRUE);
}

//$$******************************************************************

void CLoyaltyManagerDlg::HandlePopupTopup(bool bPurse2)
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CAddCashDlg dlg(FALSE, bPurse2, this);

	if (FALSE == bPurse2)
	{
		dlg.m_dPurseBalance = m_SQLRowContextMenu.GetPurse1Balance();
	}
	else
	{
		dlg.m_dPurseBalance = m_SQLRowContextMenu.GetPurse2Balance();
	}

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	double dValue = dlg.m_dValue;

	if (dValue == 0.0)
	{
		return;
	}

	if (FALSE == bPurse2)
	{
		m_SQLRowContextMenu.AddToPurse1(dValue);
	}
	else
	{
		m_SQLRowContextMenu.AddToPurse2(dValue);
	}

	CAuditRecord atc;
	atc.SetSQL(m_SQLRowContextMenu);

	if (FALSE == bPurse2)
	{
		atc.SetPurse1Transaction(dValue);
		atc.SetApplicationNo(APPNO_PURSE1_MANTOPUP);
	}
	else
	{
		atc.SetPurse2Transaction(dValue);
		atc.SetApplicationNo(APPNO_PURSE2_MANTOPUP);
	}
	
	atc.SetComment(dlg.m_strComment);
	atc.SaveLineToFile();

	// atc record contains relevant account details inc GroupNo
	// and is updated with any added bonus details
	CCalculateBonus bonus(bPurse2, dValue, &atc);
	double dBonusCash = bonus.GetBonusCash();					// return bonus cash cash value ( will be 0 if bonus points given )
	if (dBonusCash != 0)										// atc will have been updated
	{
		atc.SaveLineToFile();
		
		if (FALSE == bPurse2)
		{
			m_SQLRowContextMenu.AddToPurse1(dBonusCash);
		}
		else
		{
			m_SQLRowContextMenu.AddToPurse2(dBonusCash);
		}		
	}
	else
	{
		int nBonusPoints = bonus.GetBonusPoints();				// if no cash bonus see if have points bonus
		if (nBonusPoints != 0)									// atc will have been updated
		{
			atc.SaveLineToFile();
			m_SQLRowContextMenu.AddToPoints(nBonusPoints);
			m_SQLRowContextMenu.AddToPointsToDate(nBonusPoints);
		}
	}

	RepoAccount.UpdateRow(m_SQLRowContextMenu, NULL);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupAlert()
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CSelectAlertDlg dlg{};
		dlg.m_strAlertCodes = m_SQLRowContextMenu.GetAlertCodes();

		if (dlg.DoModal() == IDOK)
		{
			CSQLRepositoryAccount RepoAccount;
			m_SQLRowContextMenu.SetAlertCodes(dlg.m_strAlertCodes);	
			RepoAccount.UpdateRow(m_SQLRowContextMenu,NULL);
			RefreshAccountList(m_SQLRowContextMenu.GetUserID(), TRUE);
		}
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupHotlist()	
{
	HotlistCard(m_SQLRowContextMenu);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupHotlistReplace()	
{ 
	HotlistReplaceCard(m_SQLRowContextMenu);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupReplace()			
{ 
	HotlistReplaceCard(m_SQLRowContextMenu);
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupUnhotlist()	
{ 
	HotlistCard(m_SQLRowContextMenu); 
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupTrace()
{
	CHotlistTraceDlg dlg(m_SQLRowContextMenu.GetUserID());
	dlg.DoModal();
	RefreshAccountList ( dlg.m_nRequestedUserID, FALSE );
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupPhotoID()
{
	CImageHandler photo{};
	CString strFilename = photo.GetFirst ( m_SQLRowContextMenu.GetUserIDString() );
	if ( strFilename != "" )
	{
		CPhotoIDDlg dlg (m_SQLRowContextMenu, strFilename, this );
		dlg.DoModal();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupPurchaseHistory()
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(m_SQLRowContextMenu, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CPurchaseReport report ( "R105", this );
		report.Show(m_SQLRowContextMenu.GetUserID());
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupAudit()
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.RowExists(m_SQLRowContextMenu.GetUserID(), NULL).GetSQLResult() != 0)
	{
		ShowAuditReport("R123", m_SQLRowContextMenu.GetUserID());
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupAuditSummary()
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.RowExists(m_SQLRowContextMenu.GetUserID(), NULL).GetSQLResult() != 0)
	{
		ShowAuditReport("R133", m_SQLRowContextMenu.GetUserID());
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnPopupStampOffer()
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.RowExists(m_SQLRowContextMenu.GetUserID(), NULL).GetSQLResult() != 0)
	{
		CStampOfferReportWrapper Wrapper("R130", m_SQLRowContextMenu.GetUserID(), this);
		Wrapper.ShowReport();
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CLoyaltyManagerDlg::SetMenuOptions()
{
	Server.Read(Filenames.GetServerDataFilename());

	// File options
	m_MenuChanger.ShowItem(IDM_FILE_UPGRADE, (m_SQLRowSetAccountList.GetSize() == 0));

	// File options - Server
	m_MenuChanger.EnableParent(IDM_FILE_SERVER_SERVICE, Passwords.IsServerInstallAllowed());

	m_MenuChanger.ShowItem(IDM_FILE_SIGNOFF, (System.GetSignOnMethod() != 0) && (Passwords.InUse() == TRUE));
	m_MenuChanger.ShowItem(IDM_FILE_IMPORT, Passwords.IsImportRecordsAllowed());
	m_MenuChanger.ShowItem(IDM_FILE_EXPORT, Passwords.IsExportRecordsAllowed());

	//file options - backup
	{
		int nAccessType = PCOptionsHost.GetBackupAccessType();
		m_MenuChanger.ShowItem(IDM_BACKUP_SYSTEM, nAccessType < 2);
		m_MenuChanger.ShowItem(IDM_RESTORE_SCRIPT, m_bShowRestoreFromScript);
		m_MenuChanger.ShowParent(IDM_RESTORE_EXISTING, nAccessType < 3);
	}

	//Utility menu
	int nOptionCount = 1;								// max number of utility options

	bool bBatchUpdatesAllowed = TRUE;
	bBatchUpdatesAllowed &= System.GetEnableBatchUpdatesFlag();
	bBatchUpdatesAllowed &= Passwords.IsBatchUpdatesAllowed();

	if (bBatchUpdatesAllowed == FALSE)
	{
		--nOptionCount;
	}

	if (nOptionCount == 0)
	{
		m_MenuChanger.ShowParent(IDM_BATCH_UPDATE, FALSE);
	}
	else
	{
		m_MenuChanger.ShowParent(IDM_BATCH_UPDATE, TRUE);
		m_MenuChanger.ShowItem(IDM_BATCH_ADD, Passwords.IsAddRecordAllowed());
		m_MenuChanger.ShowItem(IDM_BATCH_UPDATE, bBatchUpdatesAllowed);
		m_MenuChanger.ShowItem(IDM_BATCH_DELETE, Passwords.IsDeleteRecordAllowed());
		m_MenuChanger.ShowItem(IDM_BATCH_REDEEM, Server.RedeemPointsEnabled());
	}

	// reports
	m_MenuChanger.ShowParent(IDM_REPORTS_DATABASE, Passwords.IsReportsAllowed());
	m_MenuChanger.ShowItem(IDM_REPORTS_PHOTOIDLIST, System.GetEnablePhotoIDListFlag());
	m_MenuChanger.ShowItem(IDM_REPORTS_AUDIT_BONUS_REDEEM, Server.RedeemPointsEnabled());
	m_MenuChanger.ShowItem(IDM_REPORTS_AUDIT_BONUS_REVALUE, Server.RevaluationBonusEnabled());
	m_MenuChanger.ShowItem(IDM_REPORTS_AUDIT_BONUS_SPEND, Server.SpendBonusEnabled());
	m_MenuChanger.ShowParent(IDM_REPORTS_PURCHASE_HISTORY, Server.GetPurchaseHistoryFlag());

	// setup options
	{
		bool bSystem = Passwords.IsSystemSetupAllowed();
		bool bServer = Passwords.IsServerSetupAllowed();

		if ((FALSE == bSystem) && (FALSE == bServer))
		{
			m_MenuChanger.ShowParent(IDM_SETUP_SYSTEM, FALSE);
		}
		else
		{
			m_MenuChanger.ShowParent(IDM_SETUP_SYSTEM, TRUE);
			m_MenuChanger.ShowItem(IDM_SETUP_SYSTEM, bSystem);
			m_MenuChanger.ShowItem(IDM_SETUP_GROUPS, bSystem);
			m_MenuChanger.ShowItem(IDM_SETUP_SERVER, bServer);
			m_MenuChanger.ShowItem(IDM_SETUP_STAFFCARDS, bSystem);
			m_MenuChanger.ShowItem(IDM_SETUP_STAMPOFFERS, TRUE);
		}
	}

	// record buttons
	m_buttonAdd.EnableWindow(Passwords.IsAddRecordAllowed());
	m_buttonEdit.EnableWindow(Passwords.IsEditRecordAllowed());
	m_buttonDelete.EnableWindow(Passwords.IsDeleteRecordAllowed());
	m_buttonHotlist.EnableWindow(Passwords.IsHotlistRecordAllowed());
	m_buttonTransfer.EnableWindow(Passwords.IsTransferRecordAllowed());

	m_MenuChanger.BuildMenu(GetMenu());
	DrawMenuBar();
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnButtonRestore()
{
	if (FALSE == m_bShowRestoreFromScript)
	{
		m_bShowRestoreFromScript = TRUE;
		SetMenuOptions();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::GetPassword(bool bStartup)
{
	int nOldPassword = Passwords.GetPasswordInUse();

	CPasswordDlg dlg;
	if (System.GetSignOnMethod() == 2)
	{
		dlg.m_bReadCardAllowed = TRUE;
	}

	if (dlg.DoModal() == IDCANCEL)
	{
		if (bStartup == TRUE)
		{
			EndDialog(IDCANCEL);
		}
	}
	else if (Passwords.ValidatePassword(dlg.m_strPassword, dlg.m_strCardNo) == -1)
	{
		EndDialog(IDCANCEL);
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
	
	if ( strUserName == "" )
	{
		if ( Passwords.GetPasswordInUse() > 0 )
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

void CLoyaltyManagerDlg::UpdateTitle()
{
	CString strInfo1Text = "";
	{
		int nCount = m_SQLRowSetAccountList.GetSize();
		strInfo1Text.Format( "%d Account%s", nCount, ( nCount != 1 ) ? "s" : ""   );
	}

	CString strInfo2Text = "";

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

void CLoyaltyManagerDlg::ExportDatabase()
{
	if ( System.GetExportOnExitFlag() == TRUE )
	{
		CWaitCursor cursor;

		RefreshDatabase();

		CExportDatabase exp1 ( "DB1", this );
		exp1.Execute ( FALSE );

		CExportDatabase exp2 ( "DB2", this );
		exp2.Execute ( FALSE );

		CExportDatabase exp3 ( "DB3", this );
		exp3.Execute ( FALSE );
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileServerService()
{
	CSSFile fileLock;
	if (fileLock.Open(Filenames.GetServerServiceOptionsLockFilename(), "wb") == FALSE)
	{
		Prompter.Error("These options are being modified by another user.\n\nPlease try again later.");
	}
	else
	{
		ServerServiceOptions.Read();
		CServiceInstallDlg dlg( this);
		dlg.DoModal();
		ServerServiceOptions.Write();
	}
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnFileServerServiceLog()
{
	CRepmanHandler RepmanHandler("Server Service Log", this);
	RepmanHandler.DisplayReport(Filenames.GetServerLogFilename(), Filenames.GetServerLogParamsFilename(), "");
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnBackupSystem()
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

void CLoyaltyManagerDlg::OnBackupLog()
{
	if (SolutionGlobalFunctions::FileExists(Filenames.GetBackupLogToReadFilename()) == TRUE)
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

void CLoyaltyManagerDlg::OnRestoreExisting()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(FALSE,FALSE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnRestoreScript()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(FALSE,TRUE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CLoyaltyManagerDlg::OnRestoreSeparate()
{
	m_bDoingAutoBackupCheck = TRUE;
	HandleFileRestore(TRUE,FALSE);
	m_bDoingAutoBackupCheck = FALSE;
}

//$$******************************************************************

void CLoyaltyManagerDlg::HandleFileRestore(bool bFolder, bool bFromScript)
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "restore the system";
	infoLock.m_strStopCommsPrompt = "Restore system";
	infoLock.m_bStopLoyalty = TRUE;
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
				strMsg += "\n\nPlease restart Loyalty Manager after closing this message.";
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

int CLoyaltyManagerDlg::RestoreInternal(CString& strError, bool& bComplete, CLockManagerInfo& infoLock, bool bRestoreToFolder, bool bFromScript)
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

	//StatusProgress.Lock();
	Prompter.SetPreventParentEnableFlag(TRUE);
	CSystemBackup MyBackup(FALSE, bRestoreToFolder, bFromScript, infoLock);
	Prompter.SetPreventParentEnableFlag(FALSE);
	//StatusProgress.Unlock();
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

void CLoyaltyManagerDlg::OnBackupStatus()
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

void CLoyaltyManagerDlg::ShowBackupResult(CWnd* pParent, bool bDestroyOld)
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

long CLoyaltyManagerDlg::OnAutoBackup(WPARAM, LPARAM)
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

LRESULT CLoyaltyManagerDlg::OnKickIdle(WPARAM, LPARAM)
{
	if (COleDateTime::GetCurrentTime() >= m_oleLastAutoBackupCheck + COleDateTimeSpan(0, 0, 0, 10))
	{
		if (PCOptionsHost.GetBackupAccessType() == 0)
		{
			SendMessage(WM_AUTOBACKUP, 0, 0);
		}

		m_oleLastAutoBackupCheck = COleDateTime::GetCurrentTime();
	}

	return FALSE;
}

/***********************************************************************/
