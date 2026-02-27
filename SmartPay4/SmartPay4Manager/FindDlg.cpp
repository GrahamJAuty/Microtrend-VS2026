//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\FilenameHandler.h"
#include "..\SmartPay4Shared\GroupShiftValidator.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "AuditReportWrapper.h"
#include "AutoShutdownStacker.h"
#include "ColumnHandler.h"
#include "EditRecord.h"
#include "GroupComboHelpers.h"
#include "GroupShiftDlg.h"
#include "PhotoIDDlg.h"
#include "ReportDefaultHandler.h"
#include "ReportPurchaseHistory.h"
#include "ReportStatement.h" 
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "FindDlg.h"
//$$******************************************************************

CFindDlg::CFindDlg( const char* szLeaversYear, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CFindDlg::IDD, pParent)
{
	m_strLeaversYear = szLeaversYear;
	m_strListFilename = "";
	m_strFindLabel = "FIND1";
	
	m_nRequestedUserID = 0;
	m_nSingleGroupNo = 0;

	m_bIncludeAll = FALSE;
	m_bSearchAll = FALSE;

	m_strTitle = "Find User ID";
	if (m_strLeaversYear != "")
	{
		m_strTitle.Format("Find %s User", 
			(const char*) m_strLeaversYear);
	}
}

//$$******************************************************************

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FINDSTRING, m_editFindString);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	/*****/
	DDX_Control(pDX, IDC_CHECK_USERNAME, m_checkUserName);
	DDX_Control(pDX, IDC_CHECK_MEMBERID, m_checkMemberID);
	DDX_Control(pDX, IDC_CHECK_FORENAME, m_checkForename);
	DDX_Control(pDX, IDC_CHECK_SURNAME, m_checkSurname);
	DDX_Control(pDX, IDC_CHECK_INFO1, m_checkInfo1);
	DDX_Control(pDX, IDC_CHECK_INFO2, m_checkInfo2);
	DDX_Control(pDX, IDC_CHECK_INFO3, m_checkInfo3);
	DDX_Control(pDX, IDC_CHECK_INFO4, m_checkInfo4);
	DDX_Control(pDX, IDC_CHECK_INFO5, m_checkInfo5);
	DDX_Control(pDX, IDC_CHECK_INFO6, m_checkInfo6);
	DDX_Control(pDX, IDC_CHECK_INFO7, m_checkInfo7);
	DDX_Control(pDX, IDC_CHECK_INFO8, m_checkInfo8);
	DDX_Control(pDX, IDC_BUTTON_INCLUDEALL, m_buttonIncludeAll);
	/*****/
	DDX_Control(pDX, IDC_CHECK_VALID, m_checkValid);
	DDX_Control(pDX, IDC_CHECK_EXPIRED, m_checkExpired);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_BUTTON_SEARCHALL, m_buttonSearchAll);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CFindDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	/*****/
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_INCLUDEALL, OnButtonIncludeAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHALL, OnButtonSearchAll)
	/*****/
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_POPUP_EDIT, OnPopupEdit)
	ON_COMMAND(IDM_POPUP_PURCHASEHISTORY, OnPopupPurchasehistory)
	ON_COMMAND(IDM_POPUP_STATEMENT, OnPopupStatement)
	ON_COMMAND(IDM_POPUP_AUDIT, OnPopupAudit)
	ON_COMMAND(IDM_POPUP_AUDIT_SUMMARY, OnPopupAuditSummary)
	ON_COMMAND(IDM_POPUP_GROUPSHIFT, OnPopupGroupShift)
END_MESSAGE_MAP()

//$$******************************************************************
static const int nColumnCount = 14;
//$$******************************************************************

BOOL CFindDlg::CSSAutoShutdownPostInitDialog()
{
	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strFindLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		m_editFindString.SetWindowText(csv.GetString(0));
		m_checkUserName.SetCheck( csv.GetBool(1));
		m_checkForename.SetCheck(csv.GetBool(2));
		m_checkSurname.SetCheck(csv.GetBool(3));
		m_checkMemberID.SetCheck(csv.GetBool(4));
		m_checkInfo1.SetCheck(csv.GetBool(5));
		m_checkInfo1.SetCheck(csv.GetBool(6));
		m_checkValid.SetCheck(csv.GetBool(7));
		m_checkExpired.SetCheck(csv.GetBool(8));
		m_checkInactive.SetCheck(csv.GetBool(9));
		m_checkAlert.SetCheck(csv.GetBool(10));
		m_checkInfo3.SetCheck(csv.GetBool(11));
		m_checkInfo4.SetCheck(csv.GetBool(12));
		m_checkInfo5.SetCheck(csv.GetBool(13));
		m_checkInfo6.SetCheck(csv.GetBool(14));
		m_checkInfo7.SetCheck(csv.GetBool(15));
		m_checkInfo8.SetCheck(csv.GetBool(16));
	}
	else
	{
		m_editFindString.SetWindowText("");
		m_checkUserName.SetCheck(TRUE);
		m_checkForename.SetCheck(FALSE);
		m_checkSurname.SetCheck(FALSE);
		m_checkMemberID.SetCheck(FALSE);
		m_checkInfo1.SetCheck(FALSE);
		m_checkInfo2.SetCheck(FALSE);
		m_checkInfo3.SetCheck(FALSE);
		m_checkInfo4.SetCheck(FALSE);
		m_checkInfo5.SetCheck(FALSE);
		m_checkInfo6.SetCheck(FALSE);
		m_checkInfo7.SetCheck(FALSE);
		m_checkInfo8.SetCheck(FALSE);
		m_checkValid.SetCheck(TRUE);
		m_checkExpired.SetCheck(FALSE);
		m_checkInactive.SetCheck(FALSE);
		m_checkAlert.SetCheck(FALSE);
	}

	SetWindowText ( m_strTitle );

	CColumnHandler column;
	if ( column.Read (  m_strFindLabel, nColumnCount ) == FALSE )	// "FIND"
	{
		column.SetWidth ( 0, 72 );						// default values
		column.SetWidth ( 1, 100 );
		column.SetWidth ( 2, 30 );
		column.SetWidth ( 3, 80 );
		column.SetWidth ( 4, 80 );
		column.SetWidth ( 5, 80 );
		column.SetWidth ( 6, 80 );
		column.SetWidth ( 7, 80 );
		column.SetWidth ( 8, 80 );
		column.SetWidth ( 9, 80 );
		column.SetWidth ( 10, 80 );
		column.SetWidth ( 11, 80 );
		column.SetWidth ( 12, 80 );
		column.SetWidth ( 13, 80 );
	}

	CSQLRepositoryUserText RepoText;
	CString strMemberIDLabel = RepoText.GetMemberIDText(Account::MemberID.Label);
	CString strInfo1Label = RepoText.GetInfo1Text(Account::Info1.Label);
	CString strInfo2Label = RepoText.GetInfo2Text(Account::Info2.Label);
	CString strInfo3Label = RepoText.GetInfo3Text(Account::Info3.Label);
	CString strInfo4Label = RepoText.GetInfo4Text(Account::Info4.Label);
	CString strInfo5Label = RepoText.GetInfo5Text(Account::Info5.Label);
	CString strInfo6Label = RepoText.GetInfo6Text(Account::Info6.Label);
	CString strInfo7Label = RepoText.GetInfo7Text(Account::Info7.Label);
	CString strInfo8Label = RepoText.GetInfo8Text(Account::Info8.Label);

	m_list.SubclassDlgItem(IDC_LIST, this);
	m_list.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn ( 0, Account::UserID.Label, LVCFMT_LEFT, column.GetWidth(0) );
	m_list.InsertColumn ( 1, Account::Username.Label, LVCFMT_LEFT, column.GetWidth(1) );
	m_list.InsertColumn ( 2, "Grp", LVCFMT_LEFT, column.GetWidth(2) );
	m_list.InsertColumn ( 3, Account::Forename.Label, LVCFMT_LEFT, column.GetWidth(3) );
	m_list.InsertColumn ( 4, Account::Surname.Label, LVCFMT_LEFT, column.GetWidth(4) );
	m_list.InsertColumn ( 5, strMemberIDLabel, LVCFMT_LEFT, column.GetWidth(5) );
	m_list.InsertColumn ( 6, strInfo1Label, LVCFMT_LEFT, column.GetWidth(6) );
	m_list.InsertColumn ( 7, strInfo2Label, LVCFMT_LEFT, column.GetWidth(7) );
	m_list.InsertColumn ( 8, strInfo3Label, LVCFMT_LEFT, column.GetWidth(8) );
	m_list.InsertColumn ( 9, strInfo4Label, LVCFMT_LEFT, column.GetWidth(9) );
	m_list.InsertColumn (10, strInfo5Label, LVCFMT_LEFT, column.GetWidth(10) );
	m_list.InsertColumn (11, strInfo6Label, LVCFMT_LEFT, column.GetWidth(11) );
	m_list.InsertColumn (12, strInfo7Label, LVCFMT_LEFT, column.GetWidth(12) );
	m_list.InsertColumn (13, strInfo8Label, LVCFMT_LEFT, column.GetWidth(13) );
	m_list.EnableColour();

	//GROUP
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, 0, TRUE);

// user texts

	m_checkMemberID.SetWindowText(strMemberIDLabel);
	m_checkInfo1.SetWindowText(strInfo1Label);
	m_checkInfo2.SetWindowText(strInfo2Label);
	m_checkInfo3.SetWindowText(strInfo3Label);
	m_checkInfo4.SetWindowText(strInfo4Label);
	m_checkInfo5.SetWindowText(strInfo5Label);
	m_checkInfo6.SetWindowText(strInfo6Label);
	m_checkInfo7.SetWindowText(strInfo7Label);
	m_checkInfo8.SetWindowText(strInfo8Label);

// alerts

	CAlertText alert;
	alert.Open ( Filenames.GetAlertFilename() );
	if (alert.GetAlertCount() == 0)
	{
		m_checkAlert.ShowWindow(SW_HIDE);
	}
	else
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_checkAlert.SetWindowText("Hotlist");
		}
	}

	if (m_strListFilename != "")
	{
		DisplayListFile();
	}

	return TRUE;
}

//$$******************************************************************

void CFindDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_list.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

//$$******************************************************************

void CFindDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		if ( (nIndex >= 0) && ( nIndex < m_RowSetFind.GetSize() ) )
		{
			CSQLRowAccountFind RowAccount;
			m_RowSetFind.GetRow(nIndex, RowAccount);

			CString strText = "";
			switch (pDispInfo->item.iSubItem)
			{
			case 0:
				strText = System.FormatCardNo(RowAccount.GetUserID(), TRUE);
				break;

			case 1:
				strText = RowAccount.GetUsername();;
				break;

			case 2:
				strText.Format("%d", RowAccount.GetGroupNo());
				break;

			case 3:
				strText = RowAccount.GetForename();
				break;

			case 4:
				strText = RowAccount.GetSurname();
				break;

			case 5:
				strText = RowAccount.GetMemberID();
				break;

			case 6:
				strText = RowAccount.GetInfo1();
				break;

			case 7:
				strText = RowAccount.GetInfo2();
				break;

			case 8:
				strText = RowAccount.GetInfo3();
				break;

			case 9:
				strText = RowAccount.GetInfo4();
				break;

			case 10:
				strText = RowAccount.GetInfo5();
				break;

			case 11:
				strText = RowAccount.GetInfo6();
				break;

			case 12:
				strText = RowAccount.GetInfo7();
				break;

			case 13:
				strText = RowAccount.GetInfo8();
				break;
			}

			lstrcpy(pDispInfo->item.pszText, strText);
			m_list.SetColour(nIndex, RowAccount.GetRecordStatusColour());
		}
	}
	*pResult = 0;
}

//$$******************************************************************

bool CFindDlg::HaveSearchField()
{
	if (IsTicked(m_checkUserName)) return TRUE;
	if (IsTicked(m_checkForename)) return TRUE;
	if (IsTicked(m_checkSurname)) return TRUE;
	if (IsTicked(m_checkMemberID)) return TRUE;
	if (IsTicked(m_checkInfo1)) return TRUE;
	if (IsTicked(m_checkInfo2)) return TRUE;
	if (IsTicked(m_checkInfo3)) return TRUE;
	if (IsTicked(m_checkInfo4)) return TRUE;
	if (IsTicked(m_checkInfo5)) return TRUE;
	if (IsTicked(m_checkInfo6)) return TRUE;
	if (IsTicked(m_checkInfo7)) return TRUE;
	if (IsTicked(m_checkInfo8)) return TRUE;
	return FALSE;
}

//$$******************************************************************

bool CFindDlg::HaveIncludeField()
{
	if (IsTicked(m_checkValid)) return TRUE;
	if (IsTicked(m_checkExpired)) return TRUE;
	if (IsTicked(m_checkInactive)) return TRUE;
	if (IsTicked(m_checkAlert)) return TRUE;
	return FALSE;
}

//$$******************************************************************

void CFindDlg::OnButtonFind()
{
	if (HaveIncludeField() == FALSE)
	{
		MessageBeep(MB_ICONINFORMATION);
		Prompter.Info("No include types specified", "Input required");
		m_checkValid.SetFocus();
		return;
	}

	if (HaveSearchField() == FALSE)
	{
		MessageBeep(MB_ICONINFORMATION);
		Prompter.Info("No search fields specified", "Input required");
		m_checkSurname.SetFocus();
		return;
	}

	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);

	CWaitCursor cursor;

	m_strMatchString = GetEditBoxText(m_editFindString);
	m_strMatchString.MakeUpper();

	m_RowSetFind.Reset();
	m_list.SetItemCountEx(0);
	m_list.Invalidate();

	CSQLRowAccountFindInfo infoSearch;
	infoSearch.m_nGroupSel = m_tabcomboGroup.GetCurSel();
	infoSearch.m_nGroupNo = m_nSingleGroupNo;
	infoSearch.m_strMatchString = m_strMatchString;
	infoSearch.m_bMatchUsername = IsTicked(m_checkUserName);
	infoSearch.m_bMatchForename = IsTicked(m_checkForename);
	infoSearch.m_bMatchSurname = IsTicked(m_checkSurname);
	infoSearch.m_bMatchMemberID = IsTicked(m_checkMemberID);
	infoSearch.m_bMatchInfo1 = IsTicked(m_checkInfo1);
	infoSearch.m_bMatchInfo2 = IsTicked(m_checkInfo2);
	infoSearch.m_bMatchInfo3 = IsTicked(m_checkInfo3);
	infoSearch.m_bMatchInfo4 = IsTicked(m_checkInfo4);
	infoSearch.m_bMatchInfo5 = IsTicked(m_checkInfo5);
	infoSearch.m_bMatchInfo6 = IsTicked(m_checkInfo6);
	infoSearch.m_bMatchInfo7 = IsTicked(m_checkInfo7);
	infoSearch.m_bMatchInfo8 = IsTicked(m_checkInfo8);
	infoSearch.m_bWantValidAccounts = IsTicked(m_checkValid);
	infoSearch.m_bWantInactiveAccounts = IsTicked(m_checkInactive);
	infoSearch.m_bWantExpiredAccounts = IsTicked(m_checkExpired);
	infoSearch.m_bWantAccountsWithAlert = IsTicked(m_checkAlert);

	m_RowSetFind.BuildFromSearch(infoSearch);

	int nMatched = m_RowSetFind.GetSize();
	m_list.SetItemCountEx(nMatched);
	m_list.Invalidate();

	if (0 == nMatched)
	{
		MessageBeep(MB_ICONINFORMATION);
		Prompter.Info("No matching IDs found", "Find UserID");
	}
	else
	{
		SetTitle();
	}
}

//$$******************************************************************

void CFindDlg::SetTitle()
{
	CSQLRepositoryAccount RepoAccount;
	__int64 nTotalRecords = RepoAccount.CountRows(NULL).GetSQLResult();

	int nSelected = m_RowSetFind.GetSize();

	CString strTitle = "";
	
	if (nSelected <= nTotalRecords)
	{
		strTitle.Format("Find %s ( %d found out of %I64d )", 
			(const char*) m_strLeaversYear, 
			nSelected, 
			nTotalRecords);
	}
	else
	{
		strTitle.Format("Find %s ( %d found )", 
			(const char*) m_strLeaversYear, 
			nSelected);

	}
	
	SetWindowText ( strTitle );			
	m_list.SetCurSel(0);
}

//$$******************************************************************

void CFindDlg::DisplayListFile()
{
	CWaitCursor cursor;										// display a wait cursor
	
	m_RowSetFind.Reset();
	m_list.SetItemCountEx(0);
	m_list.Invalidate();

	int nIndexCardNo = -1;

	CSSFile file;
	if ( file.Open ( m_strListFilename, "rb" ) == TRUE )
	{
		CCSV csvHeader;
		if ( file.Read ( csvHeader ) == TRUE )					// read header line 
		{
			for ( int n = 0 ; n < csvHeader.GetSize() ; n++ )
			{
				CString strField = csvHeader.GetString(n);

				if (strField == Account::UserID.Label)
				{
					nIndexCardNo = n;
				}
				else if (strField == "CardNo" && nIndexCardNo == -1)	// allow a field header of CardNo
				{
					nIndexCardNo = n;
				}
			}
		}

		if ( nIndexCardNo != -1 )
		{
			CCSV csvLine;
			while ( file.Read ( csvLine ) == TRUE )
			{
				m_RowSetFind.InsertRowByUserID(csvLine.GetInt64(nIndexCardNo));
			}
		}
		file.Close();
	}

	m_list.SetItemCountEx(m_RowSetFind.GetSize());
	m_list.Invalidate();
	m_list.SetCurSel(0);

	SetTitle();
}

//$$******************************************************************

void CFindDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnPopupEdit();
	*pResult = 0;
}

//$$******************************************************************

void CFindDlg::OnPopupEdit()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID >= 0)
	{
		CString strCardNo = System.FormatCardNo(nUserID,TRUE);

		CEditRecord record(m_strLeaversYear, this);
		record.Edit(_atoi64(strCardNo));

		ReloadSelectedRow();
		m_list.Invalidate();
	}
}

//$$******************************************************************

void CFindDlg::OnPopupGroupShift()
{
	if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
	{
		__int64 nUserID = GetSelectedUserID();

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

				if (dlg.DoModal() == IDOK)
				{
					ReloadSelectedRow();
					m_list.Invalidate();
				}
			}
		}
	}
}

//$$******************************************************************

void CFindDlg::OnButtonClear()
{
	m_list.SetItemCountEx(0);
	m_RowSetFind.Reset();
	m_list.Invalidate();
	m_editFindString.SetWindowText("");
	m_editFindString.SetFocus();
}

//$$******************************************************************

void CFindDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID < 0)
	{
		return;
	}

	if (m_strLeaversYear != "")						// no popup if looking at leavers database
	{
		return;
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
	VERIFY(menu.LoadMenu(IDR_MENU3));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CMenuChanger menuChanger;
	menuChanger.EatMenuPopup(pPopup);

	menuChanger.EnableItem(IDM_POPUP_EDIT, Passwords.IsEditRecordAllowed());

	menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, FALSE);
	if (Passwords.IsReportsAllowed() == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());
		if (Server.GetActiveEnablePurchaseHistoryFlag() == TRUE)
		{
			menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, TRUE);

			CString strFilename = Filenames.GetPurchaseHistoryFilename(FormatInt64Value(nUserID));

			bool bEnableHistory;
			if ((bEnableHistory = ::FileExists(strFilename)) == FALSE)
			{
				strFilename = Filenames.GetPurchaseHistoryUpdateFilename(FormatInt64Value(nUserID));
				bEnableHistory = ::FileExists(strFilename);
			}

			menuChanger.EnableItem(IDM_POPUP_PURCHASEHISTORY, bEnableHistory);
		}
	}

	CImageHandler image(FALSE);
	bool bEnable = (image.GetImageFilepath(FormatInt64Value(nUserID)) == "") ? FALSE : TRUE;
	menuChanger.EnableItem(IDM_POPUP_PHOTOID, bEnable);

	if (System.GetAllowGroupShiftDateRangeFlag() == FALSE)
	{
		menuChanger.ShowItem(IDM_POPUP_GROUPSHIFT, FALSE);
	}

	menuChanger.BuildMenu(pPopup);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

//$$******************************************************************

__int64 CFindDlg::GetSelectedUserID()
{
	__int64 nResult = -1;

	int nSel = m_list.GetCurSel();
	if ((nSel >= 0) && ( nSel < m_RowSetFind.GetSize()))
	{
		CSQLRowAccountFind RowFind;
		m_RowSetFind.GetRow(nSel, RowFind);
		nResult = RowFind.GetUserID();
	}

	return nResult;
}

//$$******************************************************************

void CFindDlg::ReloadSelectedRow()
{
	int nSel = m_list.GetCurSel();
	if ((nSel >= 0) && (nSel < m_RowSetFind.GetSize()))
	{
		CSQLRowAccountFind RowFind;
		m_RowSetFind.GetRow(nSel, RowFind);
		
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRowFind(RowFind, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			m_RowSetFind.SetRow(nSel, RowFind);
		}
	}
}

//$$******************************************************************

void CFindDlg::OnPopupPurchasehistory()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0 )
	{
		CPurchaseReport report ( "R105", nUserID, this );
		report.Show();
	}
}

//$$******************************************************************

void CFindDlg::OnPopupStatement()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		CReportStatement report ( Filenames.GetAuditFilename(), "R122", nUserID, this );
		report.Show();
	}
}

//$$******************************************************************

void CFindDlg::OnPopupAudit()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		CAuditReportWrapper Wrapper ( Filenames.GetAuditFilename(), "R111", nUserID, this );
		Wrapper.ShowReport();
	}
}

//$$******************************************************************

void CFindDlg::OnPopupAuditSummary()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		CAuditReportWrapper Wrapper(Filenames.GetAuditFilename(), "R141", nUserID, this);
		Wrapper.ShowReport();
	}
}

//$$******************************************************************

void CFindDlg::OnButtonSearchAll()
{
	m_bSearchAll = (FALSE == m_bSearchAll);
	m_buttonSearchAll.SetWindowText(m_bSearchAll ? "None" : "All");

	m_checkUserName.SetCheck(m_bSearchAll);
	m_checkForename.SetCheck(m_bSearchAll);
	m_checkSurname.SetCheck(m_bSearchAll);
	m_checkMemberID.SetCheck(m_bSearchAll);
	m_checkInfo1.SetCheck(m_bSearchAll);
	m_checkInfo2.SetCheck(m_bSearchAll);
	m_checkInfo3.SetCheck(m_bSearchAll);
	m_checkInfo4.SetCheck(m_bSearchAll);
	m_checkInfo5.SetCheck(m_bSearchAll);
	m_checkInfo6.SetCheck(m_bSearchAll);
	m_checkInfo7.SetCheck(m_bSearchAll);
	m_checkInfo8.SetCheck(m_bSearchAll);
}

//$$******************************************************************

void CFindDlg::OnButtonIncludeAll()
{
	m_bIncludeAll = (FALSE == m_bIncludeAll);
	m_buttonIncludeAll.SetWindowText(m_bIncludeAll ? "None" : "All");

	m_checkValid.SetCheck(m_bIncludeAll);
	m_checkExpired.SetCheck(m_bIncludeAll);
	m_checkInactive.SetCheck(m_bIncludeAll);
	m_checkAlert.SetCheck(m_bIncludeAll);
}

//$$******************************************************************

void CFindDlg::HandleClose()
{
	CCSV csv;
	csv.Add(GetEditBoxText(m_editFindString));
	csv.Add(IsTicked(m_checkUserName));
	csv.Add(IsTicked(m_checkForename));
	csv.Add(IsTicked(m_checkSurname));
	csv.Add(IsTicked(m_checkMemberID));
	csv.Add(IsTicked(m_checkInfo1));
	csv.Add(IsTicked(m_checkInfo2));
	csv.Add(IsTicked(m_checkValid));
	csv.Add(IsTicked(m_checkExpired));
	csv.Add(IsTicked(m_checkInactive));
	csv.Add(IsTicked(m_checkAlert));
	csv.Add(IsTicked(m_checkInfo3));
	csv.Add(IsTicked(m_checkInfo4));
	csv.Add(IsTicked(m_checkInfo5));
	csv.Add(IsTicked(m_checkInfo6));
	csv.Add(IsTicked(m_checkInfo7));
	csv.Add(IsTicked(m_checkInfo8));

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strFindLabel, &csv);
}

//$$******************************************************************

void CFindDlg::OnCancel()
{
	HandleClose();
	EndDialog(IDCANCEL);
}

//$$******************************************************************

void CFindDlg::OnOK()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		m_nRequestedUserID = nUserID;

		CColumnHandler column;
		column.Save(m_strFindLabel, 0, nColumnCount, m_list);

		EndDialog(IDOK);
	}
	else
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************
