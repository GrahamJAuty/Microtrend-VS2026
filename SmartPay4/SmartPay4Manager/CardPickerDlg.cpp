//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "ColumnHandler.h"
#include "GroupComboHelpers.h"
#include "SelectImportTypeDlg.h"
#include "SQLRecordSetAccountList.h"
//$$******************************************************************
#include "CardPickerDlg.h"
//$$******************************************************************
#define nIMPORT_USERID 0
#define nIMPORT_MEMBERID 1
//$$******************************************************************

CCardPickerDlg::CCardPickerDlg( const char* szTitle, const char* szDestFilename, CArray<CSQLRowAccountList, CSQLRowAccountList>& SourceItems, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CCardPickerDlg::IDD, pParent), m_arraySourceItems(SourceItems)
{
	m_strTitle = szTitle;
	m_strDestFilename = szDestFilename;

	m_strTitle = szTitle;
	
	m_bValidAllowed = TRUE;
	m_bInactiveAllowed = TRUE;
	m_bExpiredAllowed = TRUE;
	m_bAlertAllowed = TRUE;
	m_bGroupsAllowed = TRUE;
	m_nSingleGroupNo = 0;
	m_nColumnCount = 6;
	m_strBoxText = "Select cards to report on";

	m_nTaggedCount = 0;
	
	m_nMaxCardNoLength = 10;
	m_bNoPreviousSelection = FALSE;
	m_bMembershipList = FALSE;
}

//$$******************************************************************

CCardPickerDlg::~CCardPickerDlg()
{
}

//$$******************************************************************

void CCardPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CARDNOTEXT, m_staticCardNoText);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_STATIC_REPORTONBOXTEXT, m_staticReportOnBoxText);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_SPIN_CARDPICKER, m_spinCardPicker);
	DDX_Control(pDX, IDC_EDIT_TOTAL, m_editTotal);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_editCount);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_buttonPrevious);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCardPickerDlg, CSSAutoShutdownDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CARDPICKER, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_BUTTON_CARDNO, OnButtonCardNo)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CARDPICKER, OnSpinCardPicker)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnButtonPrevious)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CARDPICKER, OnColumnClickList)
	ON_MESSAGE(WM_APP + 2, OnToggleTickBox)
	ON_MESSAGE(WM_APP + 3, OnColumnClick)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CCardPickerDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strTitle);
	m_staticReportOnBoxText.SetWindowText(m_strBoxText);
	m_editCardNo.LimitText(m_nMaxCardNoLength);

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, 0, TRUE);

	if (FALSE == m_bGroupsAllowed)
	{
		m_tabcomboGroup.EnableWindow(FALSE);
	}

	CColumnHandler column;
	{
		int nTotalWidth = 0;
		if (column.Read("CLISTG2", m_nColumnCount) == TRUE)
		{
			for (int n = 0; n < 6; n++)
			{
				nTotalWidth += column.GetWidth(n);
			}
		}

		if ((nTotalWidth > 560) || (nTotalWidth < 300))
		{
			column.SetWidth(0, 20);
			column.SetWidth(1, 80);			// card no		
			column.SetWidth(2, 30);			// group
			column.SetWidth(3, 130);		// card name
			column.SetWidth(4, 150);		// add1
			column.SetWidth(5, 150);		// add2
		}
	}

	CString strText = "";

	m_listCardPicker.SubclassDlgItem(IDC_LIST_CARDPICKER, this);
	m_listCardPicker.SetUseExternalArray(TRUE);
	m_listCardPicker.SetExternalArray(&m_arrayListItems);

	m_listCardPicker.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCardPicker.SetLockItemZeroFlag(FALSE);
	m_listCardPicker.SetMultiColumnsFlag(TRUE);
	m_listCardPicker.SetTagStateNotifyMessage(this, WM_APP + 2);

	m_listCardPicker.InsertColumn(0, "", LVCFMT_LEFT, 20);
	m_listCardPicker.InsertColumn(1, "UserID", LVCFMT_LEFT, column.GetWidth(1));
	m_listCardPicker.InsertColumn(2, "Grp", LVCFMT_LEFT, column.GetWidth(2));
	m_listCardPicker.InsertColumn(3, "UserName", LVCFMT_LEFT, column.GetWidth(3));
	m_listCardPicker.InsertColumn(4, "Forename", LVCFMT_LEFT, column.GetWidth(4));

	if (TRUE == m_bMembershipList )
	{
		m_listCardPicker.InsertColumn(5, System.GetWebPaymentDisplayName(), LVCFMT_LEFT, column.GetWidth(5));
	}
	else
	{
		CSQLRepositoryUserText RepoText;
		m_listCardPicker.InsertColumn(5, RepoText.GetInfo1Text(Account::Info1.Label), LVCFMT_LEFT, column.GetWidth(5));
	}

	if (TRUE == m_bNoPreviousSelection)
	{
		ShowAndEnableWindow(&m_buttonPrevious, FALSE);
	}

	FillList();

	if (FALSE == m_bNoPreviousSelection)
	{
		ReloadPrevious();
	}

	m_listCardPicker.SetCurSel(0);
	m_listCardPicker.SetFocus();
	return FALSE;
}

//$$******************************************************************

void CCardPickerDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		CString str = "";

		if ((nIndex >= 0) && (nIndex < m_arrayListItems.GetSize()))
		{
			if (m_arraySourceItems.GetSize() == 0)
			{
				CCSV csv(m_arrayListItems[nIndex].m_strText, '|');
				str = csv.GetString(pDispInfo->item.iSubItem);
			}
			else
			{
				int nSourceIdx = atoi(m_arrayListItems[nIndex].m_strText);
				if ((nSourceIdx >= 1) && (nSourceIdx < m_arraySourceItems.GetSize()))
				{
					CSQLRowAccountList RowAccount = m_arraySourceItems.GetAt(nSourceIdx);

					switch (pDispInfo->item.iSubItem)
					{
					case 1:
						str.Format("%I64d", RowAccount.GetUserID());
						System.FormatCardNo(str);
						break;

					case 2:
						str.Format("%d", RowAccount.GetGroupNo());
						break;

					case 3:
						str = RowAccount.GetUsername();
						break;

					case 4:
						str = RowAccount.GetForename();
						break;

					case 5:
						if (TRUE == m_bMembershipList)
						{
							str = RowAccount.GetMemberID();
						}
						else
						{
							str = RowAccount.GetInfo1();
						}
						break;
					}
				}
			}
		}

		lstrcpy(pDispInfo->item.pszText, str);
	}
	*pResult = 0;
}

/**********************************************************************/

void CCardPickerDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_CARDPICKER)
	{
		m_listCardPicker.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

//$$******************************************************************

void CCardPickerDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage(WM_APP + 3, nIndex);
	*pResult = 0;
}

//$$******************************************************************

LONG CCardPickerDlg::OnColumnClick(WPARAM wParam, LONG lParam)
{
	int nIndex = (int)wParam;

	if (0 == nIndex)
	{
		if (Prompter.YesNo("Restore Default Columns") == IDYES)
		{
			m_listCardPicker.SetColumnWidth(0, 20);
			m_listCardPicker.SetColumnWidth(1, 80);
			m_listCardPicker.SetColumnWidth(2, 30);
			m_listCardPicker.SetColumnWidth(3, 130);
			m_listCardPicker.SetColumnWidth(4, 150);
			m_listCardPicker.SetColumnWidth(5, 150);
			m_listCardPicker.Invalidate();
		}
	}

	return 0L;
}

//$$******************************************************************

LONG CCardPickerDlg::OnToggleTickBox(WPARAM wParam, LONG lParam)
{
	UpdateCount();
	return 0L;
}

//$$******************************************************************

void CCardPickerDlg::UpdateCount()
{
	int nCount = m_listCardPicker.GetTaggedItemCount();
	EnableSpinFind(nCount);
	SetEditBoxInt(m_editCount, nCount);
}

//$$******************************************************************

void CCardPickerDlg::UpdateTotal()
{
	SetEditBoxInt(m_editTotal, m_listCardPicker.GetItemCount());
}

//$$******************************************************************

void CCardPickerDlg::FillList()
{
	CWaitCursor cursor;

	if (m_arraySourceItems.GetSize() == 0)
	{
		CSQLRecordSetAccountList RecordSet(NULL, RSParams_AccountList_CardPickerNoParams{});

		CSQLRowAccountList RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			ShowLine(RowAccount, 0);
		}
	}
	else
	{
		for (int n = 1; n < m_arraySourceItems.GetSize(); n++)
		{
			CSQLRowAccountList RowAccount = m_arraySourceItems.GetAt(n);
			ShowLine(RowAccount, n);
		}
	}

	m_listCardPicker.SetItemCount(m_arrayListItems.GetSize());

	EnableSpinFind(0);
	UpdateCount();
	UpdateTotal();
}

//$$******************************************************************

void CCardPickerDlg::ShowLine(CSQLRowAccountList& RowAccount, int nIndex )
{
	if ( IsValidRecord(RowAccount) == TRUE )
	{
		CSSListTaggedSelectItem item;

		if (m_arraySourceItems.GetSize() == 0)
		{
			CCSV csv('|');

			CString strCardNo = "";
			strCardNo.Format("%I64d", RowAccount.GetUserID());

			csv.Add("");
			csv.Add(System.FormatCardNo(strCardNo));
			csv.Add(RowAccount.GetGroupNo());
			csv.Add(RowAccount.GetUsername());
			csv.Add(RowAccount.GetForename());

			if (TRUE == m_bMembershipList)
			{
				csv.Add(RowAccount.GetMemberID());
			}
			else
			{
				csv.Add(RowAccount.GetInfo1());
			}

			item.m_strText = csv.GetLine();
		}
		else
		{
			CString strText = "";
			strText.Format("%d", nIndex);
			item.m_strText = strText;
		}

		item.m_nData = 0;
		item.SetTagState(0);

		m_listCardPicker.AddItemToList(item);
	}
}

//$$******************************************************************

int CCardPickerDlg::GetSelectedCount()
{
	int nCount = m_listCardPicker.GetTaggedItemCount();
	EnableSpinFind ( nCount );
	return nCount;
}

//$$******************************************************************

void CCardPickerDlg::EnableSpinFind ( int nCount )
{
	m_spinCardPicker.EnableWindow( nCount != 0 );
}

//$$******************************************************************

void CCardPickerDlg::OnSelectGroup()
{
	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup,0);
	m_listCardPicker.ClearList();
	FillList();
}

//$$******************************************************************

bool CCardPickerDlg::IsValidRecord(CSQLRowAccountList& RowAccount)
{
	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);

	if ((m_nSingleGroupNo > 0) && (RowAccount.GetGroupNo() != m_nSingleGroupNo))
	{
		return FALSE;
	}

	if ( TRUE == m_bMembershipList )
	{
		if (RowAccount.GetMemberID() == "")			// see if membershipNo field is set
		{
			return FALSE;
		}
	}

	bool bIsAlert = RowAccount.IsAlertSet();
	bool bIsInactive = RowAccount.GetInactive();
	bool bIsExpired = RowAccount.HasUserExpired();

	if (m_bValidAllowed && ((bIsInactive == FALSE) && (bIsExpired == FALSE) && (bIsAlert == FALSE)))
	{
		return TRUE;
	}

	if (m_bInactiveAllowed && bIsInactive)
	{
		return TRUE;
	}

	if (m_bAlertAllowed && bIsAlert)
	{
		return TRUE;
	}

	if (m_bExpiredAllowed && bIsExpired)
	{
		return TRUE;
	}

	return FALSE;
}

//$$******************************************************************

void CCardPickerDlg::OnButtonCardNo()
{
	CString strCardNo = System.FormatCardNo(GetEditBoxText(m_editCardNo));
	if (strCardNo != System.GetBlankCardNo())
	{
		int nIndex;
		if (FindCardNo(strCardNo, nIndex) == TRUE)
		{
			m_listCardPicker.SetItemTagState(nIndex, 1);
			m_listCardPicker.SetCurSel(nIndex);
			m_listCardPicker.Invalidate();
			UpdateCount();
			MessageBeep(MB_ICONINFORMATION);
			m_editCardNo.SetWindowText("");
		}
	}

	m_editCardNo.SetFocus();
}

//$$******************************************************************
// return true if cardno if found in list - index updated with pointer to record

bool CCardPickerDlg::FindCardNo(const char* szCardNo, int& nIndex)
{
	int nLow = 0;
	int nLowest = m_listCardPicker.GetItemCount();
	int nHigh = nLowest - 1;

	__int64 nCardNo = _atoi64(szCardNo);

	while (nLow <= nHigh)
	{
		int nMid = (nLow + nHigh) / 2;

		CSSListTaggedSelectItem item{};
		m_listCardPicker.GetListItem(nMid, item);

		__int64 nListCardNo = 0;

		if (m_arraySourceItems.GetSize() == 0)
		{
			CCSV csvItem(item.m_strText, '|');
			nListCardNo = _atoi64(csvItem.GetString(1));
		}
		else
		{
			int nIndex = atoi(item.m_strText);
			if ((nIndex >= 0) && (nIndex < m_arraySourceItems.GetSize()))
			{
				CSQLRowAccountList RowList = m_arraySourceItems.GetAt(nIndex);
				nListCardNo = RowList.GetUserID();
			}
			else
			{
				nIndex = nLow;
				return FALSE;
			}
		}

		if (nCardNo < nListCardNo)
		{
			nHigh = nMid - 1;
		}
		else
		{
			if (nCardNo > nListCardNo)
			{
				nLow = nMid + 1;
			}
			else									// strings compare/
			{
				nHigh = nMid - 1;					// assume comp < 0/
				if (nMid < nLowest)
				{
					nLowest = nMid;					// save lowest value
				}
			}
		}
	}

	if (nLowest < m_listCardPicker.GetItemCount())// lowest has been modified
	{
		nIndex = nLowest;
		return TRUE;								// found
	}

	nIndex = nLow;
	return FALSE;									// not found
}

//$$******************************************************************

void CCardPickerDlg::OnSpinCardPicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nIndex = m_listCardPicker.GetCurSel();
	
	bool bNextFound = FALSE;

	if ( pNMUpDown->iDelta > 0 )			// previous / Down
	{	
		for ( --nIndex ; nIndex >= 0 ; nIndex-- )
		{
			if ( m_listCardPicker.GetItemTagState(nIndex) == 1 )
			{
				bNextFound = TRUE;
				break;
			}
		}
	}
	else									// next / Up
	{
		for ( ++nIndex ; nIndex < m_listCardPicker.GetItemCount() ; nIndex++ )
		{
			if ( m_listCardPicker.GetItemTagState(nIndex) == 1 )
			{
				bNextFound = TRUE;
				break;
			}
		}
	}

	if ( bNextFound == TRUE )
	{
		m_listCardPicker.SetCurSel(nIndex);
		m_listCardPicker.SetFocus();
	}

	*pResult = 0;
}

//$$******************************************************************

void CCardPickerDlg::OnButtonInvert()
{
	CWaitCursor cursor;
	for (int nIndex = 0; nIndex < m_listCardPicker.GetItemCount(); nIndex++)
	{
		int nCurrentState = m_listCardPicker.GetItemTagState(nIndex);
		m_listCardPicker.SetItemTagState(nIndex, ( 0 == nCurrentState ) ? 1 : 0 ); 
	}

	m_listCardPicker.Invalidate();
	UpdateCount();
}

//$$******************************************************************

void CCardPickerDlg::OnButtonAll()	
{
	CWaitCursor cursor;
	m_listCardPicker.SelectAll();
	UpdateCount();
}

//$$******************************************************************

void CCardPickerDlg::OnButtonNone()	
{ 
	CWaitCursor cursor;
	m_listCardPicker.SelectNone();
	UpdateCount();
}

//$$******************************************************************

void CCardPickerDlg::ReloadPrevious()
{
	CSSFile file;
	if (file.Open(m_strDestFilename, "rb") == TRUE)
	{
		int nIndex;
		CString strCardNo;
		while (file.ReadString(strCardNo) == TRUE)
		{
			if (FindCardNo(strCardNo, nIndex) == TRUE)
			{
				m_listCardPicker.SetItemTagState(nIndex, 1);
			}
		}
		file.Close();
	}
	UpdateCount();
}

//$$******************************************************************

void CCardPickerDlg::OnButtonPrevious()
{
	CWaitCursor cursor;
	
	int nOldCount = m_listCardPicker.GetTaggedItemCount();
	ReloadPrevious();
	int nNewCount = m_listCardPicker.GetTaggedItemCount();
	
	m_listCardPicker.Invalidate();
	UpdateCount();
	Prompter.Info( GetAddedMessage(nNewCount-nOldCount), "Previous selection" );
}

//$$******************************************************************

void CCardPickerDlg::OnButtonImport()
{
	CSelectImportTypeDlg dlg(this);

	if (dlg.DoModal() == IDOK)
	{
		switch (dlg.m_nSel)
		{
		case 0:
			ImportViaUserID();
			break;

		case 1:
			ImportViaMemberID();
			break;
		}
	}
}

//$$******************************************************************

void CCardPickerDlg::ImportViaMemberID()
{
	ImportFromFile(nIMPORT_MEMBERID);
}

//$$******************************************************************

void CCardPickerDlg::ImportViaUserID()
{
	ImportFromFile(nIMPORT_USERID);
}

//$$******************************************************************

void CCardPickerDlg::ImportFromFile(int nImportType)
{
	CString strTitle = "Import selection from CSV file via ";

	CString strLabel1;
	CString strLabel2 = "";
	switch (nImportType)
	{
	case nIMPORT_USERID:	
		strLabel1 = Account::UserID.Label;
		break;

	case nIMPORT_MEMBERID:
		{
			CSQLRepositoryUserText RepoText;
			strLabel1 = Account::MemberID.Label;
			strLabel2 = RepoText.GetMemberIDText();
		}
		break;
	}

	strTitle += strLabel1;

	CFileDialog dlg(TRUE, "csv", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Data Files (*.csv)|*.csv|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = Sysset.GetDataPath();

	if (DoFileDialog(dlg) == IDCANCEL)
	{
		return;
	}

	CSSFile file;
	if (file.Open(dlg.GetPathName(), "rb") == FALSE)
	{
		return;
	}

	int nCount = 0;
	int nIndexCardNo = -1;
	CString strEndMsg = "";
	int nOldCount = m_listCardPicker.GetTaggedItemCount();

	CCSV csvLine;
	while (file.Read(csvLine) == TRUE)
	{
		if (nIndexCardNo == -1)							// see if set yet
		{
			for (int i = 0; i < csvLine.GetSize(); i++)	// scan first line - could be header line
			{
				CString strData = csvLine.GetString(i);		// get field text
				if (strData == strLabel1)					// see if import label
				{
					nIndexCardNo = i;						// index to CardNo field
					break;
				}
				else if (strLabel2 != "")					// see if second label set
				{
					if (strData == strLabel2)
					{
						nIndexCardNo = i;					// index to user defined MemberId text column
						break;
					}
				}
			}

			if (nIndexCardNo == -1)						// see if anything set
			{
				if (csvLine.GetSize() == 1)
				{
					nIndexCardNo = 0;						// assume file just contains label
				}
				else
				{
					CString strMsg = strLabel1;
					if (strLabel2 != "")
					{
						strMsg += " ( or ";
						strMsg += strLabel2;
						strMsg += " )";
					}
					strEndMsg.Format("No %s field found in CSV header line", 
						(const char*) strMsg);
					break;
				}
			}
		}
		else
		{
			CString strData = csvLine.GetString(nIndexCardNo);		// data from csv file
			if (strData != "")
			{
				CString strCardNo = "";

				if (nImportType == nIMPORT_USERID)					// via UserID
				{
					strCardNo = strData;
				}
				else													// via MemberID
				{
					CSQLRepositoryAccount RepoAccount;
					__int64 nUserID = RepoAccount.LookupMemberID(strData, NULL).GetSQLResult();

					if (nUserID != 0 )
					{
						strCardNo = FormatInt64Value(nUserID);
					}
				}

				strCardNo = System.FormatCardNo(strCardNo);
				if (strCardNo != System.GetBlankCardNo())	
				{
					int nIndex;
					if (FindCardNo(strCardNo, nIndex) == TRUE)
					{
						m_listCardPicker.SetItemTagState(nIndex, 1);
					}
				}
			}
		}
	}

	int nNewCount = m_listCardPicker.GetTaggedItemCount();

	UpdateCount();

	if (strEndMsg == "")
	{
		strEndMsg = GetAddedMessage(nNewCount - nOldCount);
	}

	Prompter.Info(strEndMsg, strTitle);
}

//$$******************************************************************

CString CCardPickerDlg::GetAddedMessage ( int nCount )
{
	CString strEndMsg = "";
	if (nCount == 0)
	{
		strEndMsg = "No new selections added";
	}
	else
	{
		strEndMsg.Format("Number of new selections added = %d", nCount);
	}
	return strEndMsg;
}

//$$******************************************************************

void CCardPickerDlg::SaveColumns()
{
	CColumnHandler column;
	column.Save("CLISTG2", 0, m_nColumnCount, m_listCardPicker);

}

//$$******************************************************************

void CCardPickerDlg::OnCancel()
{
	SaveColumns();
	EndDialog(IDCANCEL);
}

//$$******************************************************************

void CCardPickerDlg::OnOK()
{
	SaveColumns();

	m_nTaggedCount = m_listCardPicker.GetTaggedItemCount();

	if (0 == m_nTaggedCount)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		Prompter.Error("No cards have been selected", m_strTitle);
		return;
	}

	bool bCanContinue = TRUE;

	CSSFile file;
	if (m_strDestFilename != "")
	{
		bCanContinue = file.Open(m_strDestFilename, "wb");
	}

	if (TRUE == bCanContinue)
	{
		for (int nIndex = 0; nIndex < m_listCardPicker.GetItemCount(); nIndex++)
		{
			CSSListTaggedSelectItem item{};
			m_listCardPicker.GetListItem(nIndex, item);

			if (item.GetTagState() != 0)
			{
				if (m_arraySourceItems.GetSize() == 0)
				{
					if (m_strDestFilename != "")
					{
						CCSV csvItem(item.m_strText, '|');
						file.WriteLine(csvItem.GetString(1));
					}
				}
				else
				{
					int nIndex = atoi(item.m_strText);
					if ((nIndex >= 0) && (nIndex < m_arraySourceItems.GetSize()))
					{
						m_arraySourceItems[nIndex].SetCardPickerFlag(TRUE);

						if (m_strDestFilename != "")
						{
							CString strCardNo = "";
							strCardNo.Format("%I64d", m_arraySourceItems[nIndex].GetUserID());
							file.WriteLine(System.FormatCardNo(strCardNo));
						}
					}
				}
			}
		}

		if (m_strDestFilename != "")
		{
			file.Close();
		}
	}

	EndDialog(IDOK);
}

//$$******************************************************************
