//*****************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\GroupShiftValidator.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*****************************************************************
#include "AuditReportWrapper.h"
#include "AutoShutdownStacker.h"
#include "ColumnHandler.h"
#include "EditRecord.h"
#include "GroupShiftDlg.h"
#include "PhotoIDDlg.h"
#include "ReportDefaultHandler.h"
#include "ReportPurchaseHistory.h"
#include "ReportStatement.h" 
//*****************************************************************
#include "SurnameListDlg.h"
//*****************************************************************
extern CSysset Sysset;
//*****************************************************************

CSurnameListDlg::CSurnameListDlg ( CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSurnameListDlg::IDD, pParent)
{
	m_nRequestedUserID = -1;
	m_nUserColumnIndex = 3;
	m_strFindLabel = "FIND2";
	m_bInitDialog = FALSE;
}

//*****************************************************************

CSurnameListDlg::~CSurnameListDlg()
{
}

//*****************************************************************

void CSurnameListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUP, m_checkSortByGroup);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//*****************************************************************

BEGIN_MESSAGE_MAP(CSurnameListDlg, CSSAutoShutdownDialog)
	ON_WM_MEASUREITEM()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_CHECK_SORTBYGROUP, OnToggleSortByGroup)
	ON_COMMAND(IDM_POPUP_PURCHASEHISTORY, OnPopupPurchasehistory)
	ON_COMMAND(IDM_POPUP_STATEMENT, OnPopupStatement)
	ON_COMMAND(IDM_POPUP_AUDIT, OnPopupAudit)
	ON_COMMAND(IDM_POPUP_AUDIT_SUMMARY, OnPopupAuditSummary)
	ON_COMMAND(IDM_POPUP_EDIT, OnPopupEdit)
	ON_COMMAND(IDM_POPUP_GROUPSHIFT, OnPopupGroupShift)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//*****************************************************************
static const int nColumnCount = 5;
//*****************************************************************

void CSurnameListDlg::CSSAutoShutdownPreInitDialog()
{
	m_bInitDialog = TRUE;
}

//*****************************************************************

BOOL CSurnameListDlg::CSSAutoShutdownPostInitDialog()
{
	bool bSortByGroup = FALSE;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strFindLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		bSortByGroup = csv.GetBool(0);
		m_nRequestedUserID = csv.GetInt64(1);
	}

	m_checkSortByGroup.SetCheck(bSortByGroup);

	CColumnHandler column;
	if ( column.Read (  m_strFindLabel, nColumnCount ) == FALSE )
	{
		column.SetWidth ( 0, 120 );						// default values
		column.SetWidth ( 1, 90 );
		column.SetWidth ( 2, 120 );
		column.SetWidth ( 3, 60 );
		column.SetWidth ( 4, 40 );
	}

	m_list.SubclassDlgItem ( IDC_LIST1, this );
	m_list.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn ( 0, Account::Surname.Label, LVCFMT_LEFT, column.GetWidth(0) );
	m_list.InsertColumn ( 1, Account::Forename.Label, LVCFMT_LEFT, column.GetWidth(1) );
	m_list.InsertColumn ( 2, Account::Username.Label, LVCFMT_LEFT, column.GetWidth(2) );
	m_list.InsertColumn ( 3, Account::UserID.Label, LVCFMT_LEFT, column.GetWidth(3) );
	m_list.InsertColumn ( 4, "Grp", LVCFMT_CENTER, column.GetWidth(4) );
	m_list.EnableColour();

	Sysset.RestoreWindowSize ( this, m_strFindLabel );

	m_RowSetList.SelectRowsForSurnameList(m_nRequestedUserID,IsTicked(m_checkSortByGroup));
	m_list.SetItemCountEx(m_RowSetList.GetSize());
	m_list.SetCurSel(m_RowSetList.GetUserIDPos());

	return TRUE;
}

//*****************************************************************

void CSurnameListDlg::OnDestroy()
{
	Sysset.SaveWindowSize ( this, m_strFindLabel );
	CSSAutoShutdownDialog::OnDestroy();
}

//*****************************************************************

void CSurnameListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST1)
	{
		m_list.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

//$$******************************************************************

void CSurnameListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		if ((nIndex >= 0) && (nIndex < m_RowSetList.GetSize()))
		{
			CSQLRowAccountList RowAccount;
			m_RowSetList.GetRow(nIndex, RowAccount);

			CString strText = "";
			switch (pDispInfo->item.iSubItem)
			{
			case 0:
				strText = RowAccount.GetSurname();
				break;

			case 1:
				strText = RowAccount.GetForename();
				break;

			case 2:
				strText = RowAccount.GetUsername();;
				break;

			case 3:
				strText = System.FormatCardNo(RowAccount.GetUserID(), TRUE);
				break;

			case 4:
				strText.Format("%d", RowAccount.GetGroupNo());
				break;
			}

			lstrcpy(pDispInfo->item.pszText, strText);
			m_list.SetColour(nIndex, RowAccount.GetRecordStatusColour());
		}
	}
	*pResult = 0;
}

//$$******************************************************************

void CSurnameListDlg::SaveSettings()
{
	CColumnHandler column;
	column.Save ( m_strFindLabel, 0, nColumnCount, m_list );

	CCSV csv;
	csv.Add ( IsTicked(m_checkSortByGroup) );
	csv.Add ( m_nRequestedUserID );

	CReportDefaultHandler repdefs;
	repdefs.Save ( m_strFindLabel, &csv );
}

//*******************************************************************

void CSurnameListDlg::OnToggleSortByGroup()
{
	__int64 nUserID = GetSelectedUserID();
	m_RowSetList.SelectRowsForSurnameList(nUserID, IsTicked(m_checkSortByGroup));
	m_list.SetItemCountEx(m_RowSetList.GetSize());
	m_list.SetCurSel(m_RowSetList.GetUserIDPos());
	m_list.Invalidate();
}

//*****************************************************************

void CSurnameListDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID < 0)				
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

			bool bEnableHistory = FALSE;
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

//*****************************************************************

__int64 CSurnameListDlg::GetSelectedUserID()
{
	__int64 nResult = -1;

	int nSel = m_list.GetCurSel();
	if ((nSel >= 0) && (nSel < m_RowSetList.GetSize()))
	{
		CSQLRowAccountList RowList;
		m_RowSetList.GetRow(nSel, RowList);
		nResult = RowList.GetUserID();
	}

	return nResult;
}

//*******************************************************************

void CSurnameListDlg::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	EditRecord();
	*pResult = 0;
}

//*******************************************************************

void CSurnameListDlg::OnPopupEdit()
{
	EditRecord();
}

//*******************************************************************

void CSurnameListDlg::EditRecord()
{
	__int64 nUserID = GetSelectedUserID();
	if ( nUserID > 0 )
	{
		CEditRecord record( "", this );
		record.Edit ( nUserID );

		m_nRequestedUserID = nUserID;
		m_RowSetList.SelectRowsForSurnameList(nUserID, IsTicked(m_checkSortByGroup));
		m_list.SetItemCountEx(m_RowSetList.GetSize());
		m_list.SetCurSel(m_RowSetList.GetUserIDPos());
		m_list.Invalidate();
	}
}

//*******************************************************************

void CSurnameListDlg::OnPopupGroupShift()
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
					m_nRequestedUserID = nUserID;
					m_RowSetList.SelectRowsForSurnameList(nUserID, IsTicked(m_checkSortByGroup));
					m_list.SetItemCountEx(m_RowSetList.GetSize());
					m_list.SetCurSel(m_RowSetList.GetUserIDPos());
					m_list.Invalidate();
				}
			}
		}
	}
}

//*******************************************************************

void CSurnameListDlg::OnPopupPurchasehistory()
{
	__int64 nUserID = GetSelectedUserID();
	if ( nUserID > 0 )							// get current hightlight record
	{
		CPurchaseReport report ( "R105", nUserID, this );
		report.Show();
	}
}

//*******************************************************************

void CSurnameListDlg::OnPopupStatement()
{
	__int64 nUserID = GetSelectedUserID();
	if ( nUserID > 0 )							
	{
		CReportStatement report ( Filenames.GetAuditFilename(), "R122", nUserID, this );
		report.Show();
	}
}

//*******************************************************************

void CSurnameListDlg::OnPopupAudit()
{
	__int64 nUserID = GetSelectedUserID();
	if ( nUserID > 0 )							
	{
		CAuditReportWrapper Wrapper ( Filenames.GetAuditFilename(), "R111", nUserID, this );
		Wrapper.ShowReport();
	}
}

//*******************************************************************

void CSurnameListDlg::OnPopupAuditSummary()
{
	__int64 nUserID = GetSelectedUserID();
	if (nUserID > 0)
	{
		CAuditReportWrapper Wrapper(Filenames.GetAuditFilename(), "R141", nUserID, this);
		Wrapper.ShowReport();
	}
}

//*******************************************************************
#define X_ADJUSTMENT		12
#define Y_ADJUSTMENT		15
//*******************************************************************

void CSurnameListDlg::OnSize(UINT nType, int cx, int cy)
{
	CSSAutoShutdownDialog::OnSize(nType, cx, cy);

	if ( m_bInitDialog == TRUE )
	{
		m_list.MoveWindow ( X_ADJUSTMENT, Y_ADJUSTMENT, cx-(X_ADJUSTMENT+10), cy-(Y_ADJUSTMENT+40) );

		int x;
		CRect rect;

		m_checkSortByGroup.GetWindowRect( rect );
		ScreenToClient(rect);
		CPoint point = rect.TopLeft();
		m_checkSortByGroup.MoveWindow ( point.x, cy-26, rect.Width(), rect.Height() );

		m_buttonOK.GetWindowRect( rect );
		ScreenToClient(rect);
		x = cx - ( 2 * ( rect.Width() + X_ADJUSTMENT ) );
		m_buttonOK.MoveWindow ( x, cy-32, rect.Width(), rect.Height() );

		m_buttonCancel.GetWindowRect( rect );
		ScreenToClient(rect);
		x = cx - ( rect.Width() + X_ADJUSTMENT );
		m_buttonCancel.MoveWindow ( x, cy-32, rect.Width(), rect.Height() );
	}
}

//*******************************************************************

void CSurnameListDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_nRequestedUserID = GetSelectedUserID();

		if ( m_nRequestedUserID >= 0)
		{
			SaveSettings();
			EndDialog(IDOK);
		}
		else
		{
			CSSAutoShutdownDialog::OnCancel();
		}
	}
}

//*******************************************************************

void CSurnameListDlg::OnCancel()
{
	SaveSettings();
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CSurnameListDlg::CSSAutoShutdownLogOff()
{
	OnCancel();
}

//*******************************************************************
