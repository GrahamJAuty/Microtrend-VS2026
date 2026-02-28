//$$******************************************************************
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "ColumnHandler.h"
#include "EditLoyaltyRecord.h"
#include "EditAccountPropertySheet.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "PhotoIDDlg.h"
#include "PurchaseReport.h"
//$$******************************************************************
#include "FindDlg.h"
//$$******************************************************************

CFindDlg::CFindDlg(CWnd* pParent) : CSSDialog(CFindDlg::IDD, pParent)
{
	m_strFindLabel = "FIND";

	m_nRequestedCardNo = 0;
	m_nSingleGroupNo = 0;

	m_bSearchAll = FALSE;
	m_bIncludeAll = FALSE;

	m_strTitle = "Find Accounts";
}

//$$******************************************************************

CFindDlg::~CFindDlg()
{
}

//$$******************************************************************

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FINDSTRING, m_editFindString);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	/*****/
	DDX_Control(pDX, IDC_CHECK_NAME, m_checkCardName);
	DDX_Control(pDX, IDC_CHECK_FORENAME, m_checkForename);
	DDX_Control(pDX, IDC_CHECK_SURNAME, m_checkSurname);
	DDX_Control(pDX, IDC_CHECK_ADDRESS, m_checkAddress);
	DDX_Control(pDX, IDC_CHECK_POSTCODE, m_checkPostcode);
	DDX_Control(pDX, IDC_CHECK_INFORMATION, m_checkInformation);
	DDX_Control(pDX, IDC_CHECK_TELEPHONE, m_checkTelephone);
	DDX_Control(pDX, IDC_BUTTON_SEARCHALL, m_buttonSearchAll);
	/*****/	
	DDX_Control(pDX, IDC_CHECK_VALID, m_checkValid);
	DDX_Control(pDX, IDC_CHECK_EXPIRED, m_checkExpired);
	DDX_Control(pDX, IDC_CHECK_HOTLISTED, m_checkHotlisted);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlertSet);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_BUTTON_INCLUDEALL, m_buttonIncludeAll);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
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
	ON_COMMAND(IDM_POPUP_PURCHASEHISTORY, OnPopupPurchaseHistory)
	ON_COMMAND(IDM_POPUP_PHOTOID, OnPopupPhotoID)
END_MESSAGE_MAP()

//$$******************************************************************
static const int nColumnCount = 6;
//$$******************************************************************

BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_strTitle != "")
	{
		SetWindowText(m_strTitle);
	}

	m_checkValid.SetCheck( System.GetAccountFindValidFlag());
	m_checkExpired.SetCheck( System.GetAccountFindExpiredFlag());
	m_checkHotlisted.SetCheck( System.GetAccountFindHotlistedFlag());
	m_checkAlertSet.SetCheck(System.GetAccountFindAlertSetFlag());
	m_checkInactive.SetCheck( System.GetAccountFindInactiveFlag());
	m_checkCardName.SetCheck(System.GetAccountFindUsernameFlag());
	m_checkForename.SetCheck(System.GetAccountFindForenameFlag());
	m_checkSurname.SetCheck(System.GetAccountFindSurnameFlag());
	m_checkAddress.SetCheck(System.GetAccountFindAddressFlag());
	m_checkPostcode.SetCheck(System.GetAccountFindPostcodeFlag());
	m_checkTelephone.SetCheck(System.GetAccountFindPhoneFlag());
	m_checkInformation.SetCheck(System.GetAccountFindInfoFlag());

	int nTotalWidth = 0;
	int nColumnCount = 7;

	CColumnHandler column;
	if (column.Read(m_strFindLabel, nColumnCount) == FALSE)
	{
		for (int n = 0; n < nColumnCount; n++)
		{
			nTotalWidth += column.GetWidth(n);
		}
	}

	if ( ( nTotalWidth < 400 ) || ( nTotalWidth > 650 ) )
	{ 
		column.SetWidth ( 0, 60 );						// default values
		column.SetWidth ( 1, 120 );
		column.SetWidth ( 2, 120 );
		column.SetWidth ( 3, 40 );
		column.SetWidth ( 4, 120 );
		column.SetWidth ( 5, 80 );
		column.SetWidth ( 6, 110);
	}

	CUserTexts texts;
	texts.Read();
	CString strText = "";;

	m_list.SubclassDlgItem ( IDC_LIST, this );
	m_list.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn ( 0, "Account", LVCFMT_LEFT, column.GetWidth(0) );
	m_list.InsertColumn ( 1, "User Name", LVCFMT_LEFT, column.GetWidth(1) );
	m_list.InsertColumn ( 2, "Forename", LVCFMT_LEFT, column.GetWidth(2) );
	m_list.InsertColumn ( 3, "Grp", LVCFMT_LEFT, column.GetWidth(3) );

	strText = texts.GetText( LOYALTYFIELD_ADDRESS1, "Address");
	m_list.InsertColumn ( 4, strText, LVCFMT_LEFT, column.GetWidth(4) );
	m_checkAddress.SetWindowText(strText);

	strText = texts.GetText( LOYALTYFIELD_ADDRESS5, "Postcode");
	m_list.InsertColumn(5, strText, LVCFMT_LEFT, column.GetWidth(5));
	m_checkPostcode.SetWindowText(strText);

	strText = texts.GetText( LOYALTYFIELD_INFO1, "Info1");
	m_list.InsertColumn(6, strText, LVCFMT_LEFT, column.GetWidth(6));
	m_checkInformation.SetWindowText("Info");

	strText = texts.GetText( LOYALTYFIELD_PHONE1, "Telephone");
	m_checkTelephone.SetWindowText(strText);

	m_list.EnableColour();

//groups
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, 0, TRUE);

	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
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

		if ((nIndex >= 0) && (nIndex < m_RowSetFind.GetSize()))
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
				strText = RowAccount.GetFullname();;
				break;

			case 2:
				strText = RowAccount.GetForename();
				break;

			case 3:
				strText.Format("%d", RowAccount.GetGroupNo());
				break;

			case 4:
				strText = RowAccount.GetAddress1();
				break;

			case 5:
				strText = RowAccount.GetAddress5();
				break;

			case 6:
				strText = RowAccount.GetInfo1();
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
	if (IsTicked(m_checkCardName)) return TRUE;
	if (IsTicked(m_checkForename)) return TRUE;
	if (IsTicked(m_checkSurname)) return TRUE;
	if (IsTicked(m_checkAddress)) return TRUE;
	if (IsTicked(m_checkPostcode)) return TRUE;
	if (IsTicked(m_checkTelephone)) return TRUE;
	if (IsTicked(m_checkInformation)) return TRUE;
	return FALSE;
}

//$$******************************************************************

bool CFindDlg::HaveIncludeField()
{
	if (IsTicked(m_checkValid)) return TRUE;
	if (IsTicked(m_checkExpired)) return TRUE;
	if (IsTicked(m_checkInactive)) return TRUE;
	if (IsTicked(m_checkHotlisted)) return TRUE;
	if (IsTicked(m_checkAlertSet)) return TRUE;
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

	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, 0);

	CWaitCursor cursor;

	m_strMatchString = GetEditBoxText(m_editFindString);
	m_strMatchString.MakeUpper();

	m_RowSetFind.Reset();
	m_list.SetItemCountEx(0);
	m_list.Invalidate();

	CSQLRowAccountFindInfo infoSearch;
	infoSearch.m_nGroupSel = m_comboGroup.GetCurSel();
	infoSearch.m_nGroupNo = m_nSingleGroupNo;
	infoSearch.m_strMatchString = m_strMatchString;
	infoSearch.m_bMatchFullname = IsTicked(m_checkCardName);
	infoSearch.m_bMatchForename = IsTicked(m_checkForename);
	infoSearch.m_bMatchSurname = IsTicked(m_checkSurname);
	infoSearch.m_bMatchInfo = IsTicked(m_checkInformation);
	infoSearch.m_bMatchAddress = IsTicked(m_checkAddress);
	infoSearch.m_bMatchPostcode = IsTicked(m_checkPostcode);
	infoSearch.m_bMatchTelephone = IsTicked(m_checkTelephone);
	infoSearch.m_bWantValidAccounts = IsTicked(m_checkValid);
	infoSearch.m_bWantInactiveAccounts = IsTicked(m_checkInactive);
	infoSearch.m_bWantExpiredAccounts = IsTicked(m_checkExpired);
	infoSearch.m_bWantAlertSetAccounts = IsTicked(m_checkAlertSet);
	infoSearch.m_bWantHotlistedAccounts = IsTicked(m_checkHotlisted);

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
	CSQLRepositoryAccount repoAccount;
	__int64 nTotalRecords = repoAccount.CountRows(NULL).GetSQLResult();
	
	int nSelected = m_RowSetFind.GetSize();

	CString strTitle = "";

	if (nSelected <= nTotalRecords)
	{
		strTitle.Format("%s ( %d found out of %I64d )", 
			(const char*) m_strTitle,
			nSelected, 
			nTotalRecords);
	}
	else
	{
		strTitle.Format("%s ( %d found )",
			(const char*)m_strTitle,
			nSelected);
	}

	SetWindowText(strTitle);
	m_list.SetCurSel(0);
}

//$$******************************************************************

__int64 CFindDlg::GetSelectedUserID()
{
	__int64 nResult = -1;

	int nSel = m_list.GetCurSel();
	if ((nSel >= 0) && (nSel < m_RowSetFind.GetSize()))
	{
		CSQLRowAccountFind RowFind;
		m_RowSetFind.GetRow(nSel, RowFind);
		nResult = RowFind.GetUserID();
	}

	return nResult;
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
		CEditLoyaltyRecord EditRecord;
		EditRecord.EditRecord(ACCOUNTEDIT_FIND, nUserID, this);
		m_list.Invalidate();
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

	CString strUserID = SimpleFormatInt64Value(nUserID);

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
		if (Server.GetPurchaseHistoryFlag() == TRUE)
		{
			menuChanger.ShowItem(IDM_POPUP_PURCHASEHISTORY, TRUE);

			bool bEnableHistory;
			CString strFilename = Filenames.GetPurchaseHistoryFilename(strUserID);
			if ((bEnableHistory = SolutionGlobalFunctions::FileExists(strFilename)) == FALSE)
			{
				strFilename = Filenames.GetPurchaseHistoryUpdateFilename(strUserID);
				bEnableHistory = SolutionGlobalFunctions::FileExists(strFilename);
			}

			menuChanger.EnableItem(IDM_POPUP_PURCHASEHISTORY, bEnableHistory);
		}
	}

	CImageHandler photo{};
	CString strFilename = photo.GetFirst(strUserID);
	menuChanger.EnableItem(IDM_POPUP_PHOTOID, strFilename != "");
	menuChanger.BuildMenu(pPopup);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

//$$******************************************************************

void CFindDlg::OnPopupPurchaseHistory()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		CPurchaseReport report("R105", this);
		report.Show(nUserID);
	}
}

//$$******************************************************************

void CFindDlg::OnPopupPhotoID()
{
	__int64 nUserID = GetSelectedUserID();
	
	if (nUserID > 0)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			CImageHandler photo{};
			CString strFilename = photo.GetFirst(SimpleFormatInt64Value(nUserID));
			if (strFilename != "")
			{
				CPhotoIDDlg dlg( RowAccount, strFilename, this);
				dlg.DoModal();
			}
		}
	}
}

//$$******************************************************************

void CFindDlg::OnButtonSearchAll()
{
	m_bSearchAll = (FALSE == m_bSearchAll);
	m_checkCardName.SetCheck(m_bSearchAll);
	m_checkForename.SetCheck(m_bSearchAll);
	m_checkSurname.SetCheck(m_bSearchAll);
	m_checkInformation.SetCheck(m_bSearchAll);
	m_checkAddress.SetCheck(m_bSearchAll);
	m_checkPostcode.SetCheck(m_bSearchAll);
	m_checkTelephone.SetCheck(m_bSearchAll);
	m_buttonSearchAll.SetWindowText(m_bSearchAll ? "&None" : "&All");
}

//$$******************************************************************

void CFindDlg::OnButtonIncludeAll()
{
	m_bIncludeAll = (FALSE == m_bIncludeAll);
	m_checkValid.SetCheck(m_bIncludeAll);
	m_checkExpired.SetCheck(m_bIncludeAll);
	m_checkHotlisted.SetCheck(m_bIncludeAll);
	m_checkInactive.SetCheck(m_bIncludeAll);
	m_buttonIncludeAll.SetWindowText(m_bIncludeAll ? "&None" : "&All");
}

//$$******************************************************************

void CFindDlg::HandleClose()
{
	CColumnHandler column;
	column.Save(m_strFindLabel, 0, nColumnCount, m_list);
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
	HandleClose();
	bool bOK = FALSE;

	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		m_nRequestedCardNo = nUserID;
		bOK = TRUE;
	}
	
	EndDialog(bOK ? IDOK : IDCANCEL);
}

//$$******************************************************************
