//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//$$******************************************************************
#include "ColumnHandler.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "CardPickerDlg.h"
//$$******************************************************************

CCardPickerDlg::CCardPickerDlg(const char* szTitle, const char* szDestFilename, CArray<CSQLRowAccountList, CSQLRowAccountList>& SourceItems, CWnd* pParent)
	: CSSDialog(CCardPickerDlg::IDD, pParent), m_arraySourceItems(SourceItems)
{
	m_strTitle = szTitle;
	m_strDestFilename = szDestFilename;
	
	m_bValidAllowed = TRUE;
	m_bHotlistedAllowed = TRUE;
	m_bInactiveAllowed = TRUE;
	m_bExpiredAllowed = TRUE;
	m_nSingleGroupNo = 0;
	m_nColumnCount = 6;
	m_strBoxText = "Select cards to report on";

	m_nTaggedCount = 0;

	m_nMaxCardNoLength = System.GetMaxCardNoLength();
	m_bNoPreviousSelection = FALSE;
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

BEGIN_MESSAGE_MAP(CCardPickerDlg, CDialog)
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

BOOL CCardPickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	m_staticReportOnBoxText.SetWindowText(m_strBoxText);
	m_editCardNo.LimitText(m_nMaxCardNoLength);

	//groups
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, 0, TRUE );

	//cards  Account::ExCardNo.Label
	m_staticCardNoText.SetWindowText(System.GetCardNoText());

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

		if ((nTotalWidth < 300) || (nTotalWidth > 560))
		{
			column.SetWidth(0, 20);
			column.SetWidth(1, 80);			// card no
			column.SetWidth(2, 30);			// group
			column.SetWidth(3, 130);		// card name
			column.SetWidth(4, 150);		// add1
			column.SetWidth(5, 150);		// add2
		}
	}

	CUserTexts texts{};
	texts.Read();
	CString strText = "";

	m_listCardPicker.SubclassDlgItem(IDC_LIST_CARDPICKER, this);
	m_listCardPicker.SetUseExternalArray(TRUE);
	m_listCardPicker.SetExternalArray(&m_arrayListItems);

	m_listCardPicker.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCardPicker.SetLockItemZeroFlag(FALSE);
	m_listCardPicker.SetMultiColumnsFlag(TRUE);
	m_listCardPicker.SetTagStateNotifyMessage(this, WM_APP + 2);

	m_listCardPicker.InsertColumn(0, "", LVCFMT_LEFT, 20);
	m_listCardPicker.InsertColumn(1, System.GetCardNoText(), LVCFMT_LEFT, column.GetWidth(1));
	m_listCardPicker.InsertColumn(2, "Grp", LVCFMT_LEFT, column.GetWidth(2));
	m_listCardPicker.InsertColumn(3, "Name", LVCFMT_LEFT, column.GetWidth(3));
	m_listCardPicker.InsertColumn(4, texts.GetText(LOYALTYFIELD_ADDRESS1, "Add1"), LVCFMT_LEFT, column.GetWidth(4));
	m_listCardPicker.InsertColumn(5, texts.GetText(LOYALTYFIELD_INFO1, "Info1"), LVCFMT_LEFT, column.GetWidth(5));

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
						str.Format("%I64d", RowAccount.GetACListUserID());
						System.FormatCardNo(str);
						break;

					case 2:
						str.Format("%d", RowAccount.GetACListGroupNo());
						break;

					case 3:
						str = RowAccount.GetACListFullname();
						break;

					case 4:
						str = RowAccount.GetACListAddress1();
						break;

					case 5:
						str = RowAccount.GetACListInfo1();
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
	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);

	CWaitCursor cursor;

	if ( m_arraySourceItems.GetSize() == 0 )
	{
		CSQLRecordSetAccountList RecordSet(NULL, RSParams_AccountList_CardPickerNoParams{});

		CSQLRowAccountList RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (CanAddLineToList(RowAccount) == TRUE)
			{
				AddLineToList(RowAccount, 0);
			}
		}
	}
	else
	{
		for (int n = 1; n < m_arraySourceItems.GetSize(); n++)
		{
			CSQLRowAccountList RowAccount = m_arraySourceItems.GetAt(n);

			if (CanAddLineToList(RowAccount) == TRUE)
			{
				AddLineToList(RowAccount, n);
			}
		}
	}

	m_listCardPicker.SetItemCount(m_arrayListItems.GetSize());

	EnableSpinFind(0);
	UpdateCount();
	UpdateTotal();
}

//$$******************************************************************

void CCardPickerDlg::AddLineToList(CSQLRowAccountList& RowAccount, int nIndex)
{
	CSSListTaggedSelectItem item;

	if (m_arraySourceItems.GetSize() == 0)
	{
		CString strCardNo = "";
		strCardNo.Format("%I64d", RowAccount.GetACListUserID());

		CCSV csv('|');
		csv.Add("");
		csv.Add(System.FormatCardNo(strCardNo));
		csv.Add(RowAccount.GetACListGroupNo());
		csv.Add(RowAccount.GetACListFullname());
		csv.Add(RowAccount.GetACListAddress1());
		csv.Add(RowAccount.GetACListInfo1());
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

//$$******************************************************************

bool CCardPickerDlg::CanAddLineToList(CSQLRowAccountList& RowAccount)
{
	if ((m_nSingleGroupNo > 0) && (RowAccount.GetACListGroupNo() != m_nSingleGroupNo))
	{
		return FALSE;
	}

	bool bIsHotlisted = RowAccount.IsACListCardHotlisted();
	bool bIsInactive = RowAccount.GetACListInactive();
	bool bIsExpired = RowAccount.HasACListCardExpired();

	if (m_bValidAllowed && ((bIsHotlisted || bIsExpired || bIsInactive) == FALSE))
	{
		return TRUE;
	}

	if (m_bHotlistedAllowed && bIsHotlisted)
	{
		return TRUE;
	}

	if (m_bExpiredAllowed && bIsExpired)
	{
		return TRUE;
	}

	if (m_bInactiveAllowed && bIsInactive)
	{
		return TRUE;
	}

	return FALSE;
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
				nListCardNo = RowList.GetACListUserID();
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
		int nIndex = 0;
		CString strCardNo = "";
		while (file.ReadString(strCardNo) == TRUE)
		{
			if (FindCardNo(strCardNo, nIndex) == TRUE)
			{
				m_listCardPicker.SetItemTagState(nIndex, 1);
			}
		}
		file.Close();
	}
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
	CString strTitle = "Select CardNo entries from CSV file";

	CFileDialog dlg(TRUE, "csv", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Data Files (*.csv)|*.csv|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = strTitle;
	dlg.m_ofn.lpstrInitialDir = Sysset.GetDataPath();

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CSSFile file;
	if (file.Open(dlg.GetPathName(), "rb") == FALSE)
	{
		return;
	}

	int nIndexCardNo = -1;
	CString strEndMsg = "";
	int nOldCount = m_listCardPicker.GetTaggedItemCount();

	CCSV csvLine;
	while (file.Read(csvLine) == TRUE)
	{
		if (nIndexCardNo == -1)							// see if set yet
		{
			int i = 0;
			for (i = 0; i < csvLine.GetSize(); i++)	// scan first lin e- could be header line
			{
				CString strData = csvLine.GetString(i);		// get field text
				if (strData == Account::UserID.Label)		// see if CardNo
				{
					nIndexCardNo = i;						// index to CardNo field
					break;
				}
			}
			if (nIndexCardNo == -1)						// see if anything set
			{
				if (csvLine.GetSize() == 1)
				{
					nIndexCardNo = 0;						// assume xxx.lst file
				}
				else
				{
					strEndMsg.Format("No %s field found in CSV header line", Account::UserID.Label);
					break;
				}
			}
		}
		else
		{
			CString strCardNo = System.FormatCardNo(csvLine.GetString(nIndexCardNo));
			if (strCardNo != System.GetBlankCardNo())		// SMARTDB_BLANK_CARDNO )
			{
				int nIndex;
				if (FindCardNo(strCardNo, nIndex) == TRUE)
				{
					m_listCardPicker.SetItemTagState(nIndex, 1);
				}
			}
		}
	}

	int nNewCount = m_listCardPicker.GetTaggedItemCount();

	UpdateCount();
	
	if (strEndMsg == "")
	{
		strEndMsg = GetAddedMessage(nNewCount-nOldCount);
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
						m_arraySourceItems[nIndex].SetACListCardPickerFlag(TRUE);

						if (m_strDestFilename != "")
						{
							CString strCardNo = "";
							strCardNo.Format("%I64d", m_arraySourceItems[nIndex].GetACListUserID());
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
