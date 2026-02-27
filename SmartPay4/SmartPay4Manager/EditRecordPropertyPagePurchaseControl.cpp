//$$******************************************************************
#include "..\SmartPay4Shared\DateRangeHelpers.h"
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRecordSetPurchaseControlRule.h"
//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "EditRecordPropertySheet.h"
#include "EditStampsDlg.h"
#include "SQLTranStampManualAdd.h"
#include "SQLTranStampManualEdit.h"
#include "PurchaseControlStampDeleteDlg.h"
#include "PurchaseControlStampModifyDlg.h"
//$$******************************************************************
#include "EditRecordPropertyPagePurchaseControl.h"
//$$******************************************************************

CEditRecordPropertyPagePurchaseControl::CEditRecordPropertyPagePurchaseControl(CSQLRowAccountFull& RowAccount)
	: CSSPropertyPage(CEditRecordPropertyPagePurchaseControl::IDD)
	, m_SQLRowAccount(RowAccount)
{
	m_PurchaseControlCache.Reset();
}

//$$******************************************************************

CEditRecordPropertyPagePurchaseControl::~CEditRecordPropertyPagePurchaseControl()
{
}

//$$******************************************************************

void CEditRecordPropertyPagePurchaseControl::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RULE, m_comboRule);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_DELETE_EXPIRED, m_buttonDeleteExpired);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_EXPIRED, m_buttonModifyExpired);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditRecordPropertyPagePurchaseControl, CSSPropertyPage)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_CBN_SELCHANGE(IDC_COMBO_RULE,OnSelectRule)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_EXPIRED, OnButtonDeleteExpired)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_EXPIRED, OnButtonModifyExpired)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//$$******************************************************************

CString CEditRecordPropertyPagePurchaseControl::FormatDate(CString strDate, bool bIsExpiryDate)
{
	CString strResult = "";

	if ((strDate.GetLength() == 8) && (::TestNumeric(strDate) == TRUE))
	{
		if ((strDate == CSQLRowPurchaseControlStamp::GetNeverExpireDate()) && (TRUE == bIsExpiryDate))
		{
			strResult = "Never";
		}
		else
		{
			CString strSlash = "/";
			strResult = strDate.Right(2) + strSlash + strDate.Mid(4, 2) + strSlash + strDate.Left(4);
		}
	}

	return strResult;
}

//$$******************************************************************

BOOL CEditRecordPropertyPagePurchaseControl::OnInitDialog()
{
	m_list.ClearRowHeightFlag();

	CSSPropertyPage::OnInitDialog();

	m_list.SubclassDlgItem(IDC_LIST, this);
	m_list.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.EnableColour();
	m_list.InsertColumn(0, "Control ID", LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, "Control Name", LVCFMT_LEFT, 280);
	m_list.InsertColumn(2, "Enabled", LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, "Awarded", LVCFMT_LEFT, 110);
	m_list.InsertColumn(4, "Expires", LVCFMT_LEFT, 110);
	m_list.InsertColumn(5, "Stamps", LVCFMT_RIGHT, 60);

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(50);
	m_comboRule.SetTabStops(arrayStops);

	m_comboRule.AddItem(",All Controls");
	m_comboRule.SetItemData(0, 0);

	{
		CSQLRecordSetPurchaseControlRule RecordSet(NULL, RSParams_PurchaseControlRule_NormalNoParams());

		int nPos = 1;
		CSQLRowPurchaseControlRule RowSO;
		while (RecordSet.StepSelectAll(RowSO) == TRUE)
		{
			int nRuleID = RowSO.GetRuleID();

			CString str = "";
			str.Format("%d,%s",
				nRuleID,
				(const char*) RowSO.GetDisplayDescription());

			m_comboRule.AddItem(str);
			m_comboRule.SetItemData(nPos++, nRuleID);

			m_PurchaseControlCache.InsertRow(RowSO);
		}
	}

	m_comboRule.SetCurSel(0);
	OnSelectRule();

	return TRUE;
}

//$$******************************************************************

void CEditRecordPropertyPagePurchaseControl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_list.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	CString strDateToday = "";
	strDateToday.Format("%4.4d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay());

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		
		CCSV csvLine('\t');

		bool bExpired = FALSE;
		if ((nIndex >= 0) && (nIndex < m_arrayStamps.GetSize()))
		{
			CSQLRowPurchaseControlStamp Row = m_arrayStamps.GetAt(nIndex);

			CString strRuleName = "";
			CString strEnabled = "";
			
			{
				CSQLRowPurchaseControlRule RowInfo;
				if (m_PurchaseControlCache.GetPurchaseControlRow(Row.GetRuleID(), RowInfo) == TRUE)
				{
					strRuleName = RowInfo.GetDisplayDescription();
					strEnabled = RowInfo.GetEnableFlag() ? "Yes" : "No";
				}
			}

			CString strExpireDate = Row.GetExpireDate();
			bExpired = (strDateToday > strExpireDate);

			csvLine.Add(Row.GetRuleID());
			csvLine.Add(strRuleName);
			csvLine.Add(strEnabled);
			csvLine.Add(FormatDate(Row.GetAwardDate(), FALSE));
			csvLine.Add(FormatDate(strExpireDate, TRUE));
			csvLine.Add(Row.GetStampCount());
		}
		
		lstrcpy(pDispInfo->item.pszText, csvLine.GetString(pDispInfo->item.iSubItem));

		if (TRUE == bExpired)
		{
			m_list.SetColour(nIndex, 0xD);
		}
		else
		{
			m_list.SetColour(nIndex, 0x0);
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnSelectRule()
{
	int nSel = m_comboRule.GetCurSel();
	if ((nSel >= 0) && (nSel < m_comboRule.GetCount()))
	{
		SelectRule(m_comboRule.GetItemData(nSel));
		m_buttonAdd.EnableWindow(nSel != 0);
		m_buttonEdit.EnableWindow(TRUE);
		m_buttonDelete.EnableWindow(TRUE);
		m_buttonDeleteExpired.EnableWindow(nSel != 0);
		m_buttonModifyExpired.EnableWindow(nSel != 0);
	}
	else
	{
		m_buttonAdd.EnableWindow(FALSE);
		m_buttonEdit.EnableWindow(FALSE);
		m_buttonDelete.EnableWindow(FALSE);
		m_buttonDeleteExpired.EnableWindow(FALSE);
		m_buttonModifyExpired.EnableWindow(FALSE);
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::SelectRule(int nRuleID)
{
	CSQLRepositoryPurchaseControlStamp Repo;
	if (0 == nRuleID)
	{
		Repo.GetPurchaseControlStampsByUserID(m_SQLRowAccount.GetUserID(), m_arrayStamps);
	}
	else
	{
		Repo.GetPurchaseControlStampsByUserIDAndRuleID(m_SQLRowAccount.GetUserID(), nRuleID, m_arrayStamps);
	}

	m_list.SetItemCountEx(m_arrayStamps.GetSize());
	SelectLine(0);
	m_list.Invalidate();
}

/**********************************************************************/

int CEditRecordPropertyPagePurchaseControl::GetSafeSelection()
{
	int nSel = m_list.GetCurSel();
	if ((nSel < 0) || (nSel >= m_list.GetItemCount()) || ( nSel >= m_arrayStamps.GetSize() ) )
	{
		nSel = -1;
	}

	return nSel;
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::SelectLine(int nSel)
{
	if (nSel < 0)
	{
		nSel = 0;
	}

	{
		int nMax = m_list.GetItemCount() - 1;
		if (nSel > nMax)
		{
			nSel = nMax;
		}
	}

	m_list.Invalidate();

	for (int n = 0; n < m_list.GetItemCount(); n++)
	{
		if (n != nSel)
		{
			m_list.SetItemState(n, 0, LVIS_SELECTED);
		}
		else
		{
			m_list.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_list.SetSelectionMark(n);
			m_list.SetCurSel(n);
		}
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::SelectRow(CSQLRowPurchaseControlStamp& Row)
{
	int nSel = 0;
	for (int n = 0; (n < m_arrayStamps.GetSize()) && (n < m_list.GetItemCount()); n++)
	{
		CSQLRowPurchaseControlStamp arrayRow = m_arrayStamps.GetAt(n);

		if (arrayRow.Compare(Row) == TRUE )
		{
			nSel = n;
			break;
		}
	}

	SelectLine(nSel);
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnButtonAdd()
{
	int nRuleSel = m_comboRule.GetCurSel();
	if ((nRuleSel < 1) || (nRuleSel >= m_comboRule.GetCount()))
	{
		return;
	}

	CSQLRepositoryPurchaseControlRule RepoRule;
	CSQLRowPurchaseControlRule RowRule;
	RowRule.SetRuleID(m_comboRule.GetItemData(nRuleSel));
	if (RepoRule.SelectRow(RowRule, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CSQLRowPurchaseControlStamp RowStampsOld;
	RowStampsOld.SetRuleID(RowRule.GetRuleID());
	RowStampsOld.SetUserID(m_SQLRowAccount.GetUserID());

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	{
		CString strAwardDate = "";
		strAwardDate.Format("%4.4d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay());

		RowStampsOld.SetAwardDate(strAwardDate);
	}

	if ((RowRule.GetExpiryDays() <= 0) || (RowRule.GetExpiryDays() > PurchaseControlRule::Expiry.Max))
	{
		RowStampsOld.SetNeverExpire();
	}
	else
	{
		CDateRangeHelpers DateRangeHelpers;
		COleDateTime dateExpire = timeNow;

		if (RowRule.GetExpiryDays() > 1)
		{
			DateRangeHelpers.AddDaysToTime(dateExpire, RowRule.GetExpiryDays() - 1);
		}

		CString strExpireDate = "";
		strExpireDate.Format("%4.4d%4.4d%4.4d",
			dateExpire.GetYear(),
			dateExpire.GetMonth(),
			dateExpire.GetDay());

		RowStampsOld.SetExpireDate(strExpireDate);
	}

	RowStampsOld.SetStampCount(1);
	CSQLRowPurchaseControlStamp RowStampsNew = RowStampsOld;

	CAutoShutdownPropSheetStacker AutoShutdownStacker((CEditRecordPropertySheet*)GetParent());

	CEditStampsDlg dlg(RowStampsOld, RowStampsNew, RowRule, TRUE, this);
	if (dlg.DoModal() == IDOK)
	{
		if (RowStampsNew.GetStampCount() > 0)
		{
			SQLPurchaseControlBuffer.Reset();

			CSQLTranStampManualAdd Tran;
			Tran.BeginTrans();
			Tran.DoWork(m_SQLRowAccount, RowStampsNew, RepoRule.GetRuleName(RowStampsOld.GetRuleID(), NULL));

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				break;

			default:
				Tran.ShowErrorMessage("Add Purchase Control Stamps");
				break;
			}

			OnSelectRule();
			SelectRow(RowStampsNew);
		}
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnButtonEdit()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CSQLRowPurchaseControlStamp RowStampsOld = m_arrayStamps.GetAt(nSel);
		CSQLRowPurchaseControlStamp RowStampsNew = RowStampsOld;

		CSQLRepositoryPurchaseControlStamp RepoStamp;
		CSQLRepositoryPurchaseControlRule RepoRule;
		
		CSQLRowPurchaseControlRule RowRule;
		RowRule.SetRuleID(RowStampsOld.GetRuleID());
		RepoRule.SelectRow(RowRule, NULL);

		CAutoShutdownPropSheetStacker AutoShutdownStacker((CEditRecordPropertySheet*)GetParent());
			
		CEditStampsDlg dlg(RowStampsOld, RowStampsNew, RowRule, FALSE, this);
		if (dlg.DoModal() == IDOK)
		{
			if ((RowStampsOld.GetExpireDate() == RowStampsNew.GetExpireDate()))
			{
				if (RowStampsOld.GetStampCount() != RowStampsNew.GetStampCount())
				{
					if (RepoStamp.UpdateRow(RowStampsNew, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						SQLPurchaseControlBuffer.Reset();
						
						SQLPurchaseControlBuffer.BufferPCAction(
							APPNO_SO_MEDIT,
							m_SQLRowAccount,
							RowStampsOld,
							RowRule.GetDisplayDescription(),
							RowStampsNew.GetStampCount() - RowStampsOld.GetStampCount(),
							RowStampsNew.GetStampCount()
						);

						SQLPurchaseControlBuffer.WritePCAuditLines(NULL);

						OnSelectRule();
						SelectRow(RowStampsNew);
					}
				}
			}
			else
			{
				SQLPurchaseControlBuffer.Reset();

				CSQLTranStampManualEdit Tran(APPNO_SO_MEDIT);
				Tran.BeginTrans();
				Tran.DoWork(m_SQLRowAccount,RowStampsOld,RowStampsNew, 
					RepoRule.GetRuleName(RowStampsOld.GetRuleID(), NULL));

				switch (Tran.EndTrans())
				{
				case SQLTRAN_STATE_COMMITOK:
					Tran.AfterTransaction();
					break;

				default:
					Tran.ShowErrorMessage("Edit Purchase Control Stamps");
					break;
				}

				OnSelectRule();
				SelectRow(RowStampsNew);
			}
		}
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnButtonDelete()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		if (Prompter.YesNo("Are you sure you wish to delete this row of stamps ?") == IDYES)
		{
			CSQLRowPurchaseControlStamp RowStamps = m_arrayStamps.GetAt(nSel);

			CSQLRepositoryPurchaseControlStamp RepoStamp;
			CSQLRepositoryPurchaseControlRule RepoRule;
			if (RepoStamp.DeleteRow(RowStamps, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				SQLPurchaseControlBuffer.Reset();

				SQLPurchaseControlBuffer.BufferPCAction(
					APPNO_SO_MDEL,
					m_SQLRowAccount,
					RowStamps,
					RepoRule.GetRuleName(RowStamps.GetRuleID(),NULL),
					RowStamps.GetStampCount() * (-1),
					0
				);

				SQLPurchaseControlBuffer.WritePCAuditLines(NULL);

				CSQLRowPurchaseControlStamp RowToSelect;
				if (nSel < m_arrayStamps.GetSize() - 1)
				{
					RowToSelect = m_arrayStamps.GetAt(nSel + 1);
				}
				else if ((nSel > 0) && (nSel < m_arrayStamps.GetSize() + 1))
				{
					RowToSelect = m_arrayStamps.GetAt(nSel - 1);
				}

				OnSelectRule();
				SelectRow(RowToSelect);
			}
			else
			{
				Prompter.Error("Unable to delete row.");
			}
		}
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnButtonDeleteExpired()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CAutoShutdownPropSheetStacker AutoShutdownStacker((CEditRecordPropertySheet*)GetParent());

		CSQLRowPurchaseControlStamp RowStamps = m_arrayStamps.GetAt(nSel);
		CPurchaseControlStampDeleteDlg dlgAdmin(FALSE, m_SQLRowAccount.GetUserID(), RowStamps.GetRuleID());
		
		AutoShutdownHelper.ClearShutdownRequestMarker();

		if ( (dlgAdmin.DoModal() == IDOK) && (AutoShutdownHelper.GetShutdownRequestMarker() == FALSE))
		{
			OnSelectRule();
		}
	}
}

/**********************************************************************/

void CEditRecordPropertyPagePurchaseControl::OnButtonModifyExpired()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CAutoShutdownPropSheetStacker AutoShutdownStacker((CEditRecordPropertySheet*)GetParent());

		CSQLRowPurchaseControlStamp RowStamps = m_arrayStamps.GetAt(nSel);
		CPurchaseControlStampModifyDlg dlgAdmin(FALSE, m_SQLRowAccount.GetUserID(), RowStamps.GetRuleID());
		
		AutoShutdownHelper.ClearShutdownRequestMarker();

		if ((dlgAdmin.DoModal() == IDOK) && (AutoShutdownHelper.GetShutdownRequestMarker() == FALSE))
		{
			OnSelectRule();
		}
	}
}

/**********************************************************************/

BOOL CEditRecordPropertyPagePurchaseControl::OnKillActive()
{
	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEditRecordPropertyPagePurchaseControl::OnOK()
{
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

BOOL CEditRecordPropertyPagePurchaseControl::OnQueryCancel()
{
	return CEditRecordPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditRecordPropertyPagePurchaseControl::OnCancel()
{
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************
