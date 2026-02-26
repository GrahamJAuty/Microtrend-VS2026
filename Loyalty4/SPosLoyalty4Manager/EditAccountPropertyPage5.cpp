//$$******************************************************************
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\DateRangeHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRecordSetStampOfferInfo.h"
//$$******************************************************************
#include "EditAccountPropertySheet.h"
#include "EditStampsDlg.h"
#include "SQLTranStampManualAdd.h"
#include "SQLTranStampManualEdit.h"
#include "StampOfferDeleteDlg.h"
#include "StampOfferModifyDlg.h"
//$$******************************************************************
#include "EditAccountPropertyPage5.h"
//$$******************************************************************

CEditAccountPropertyPage5::CEditAccountPropertyPage5(CSQLRowAccountFull& RowAccount)
	: CSSPropertyPage(CEditAccountPropertyPage5::IDD)
	, m_SQLRowAccount(RowAccount)
{
	m_StampOfferCache.Reset();
}

//$$******************************************************************

CEditAccountPropertyPage5::~CEditAccountPropertyPage5()
{
}

//$$******************************************************************

void CEditAccountPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OFFER, m_comboOffer);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_DELETE_EXPIRED, m_buttonDeleteExpired);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_EXPIRED, m_buttonModifyExpired);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditAccountPropertyPage5, CSSPropertyPage)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_CBN_SELCHANGE(IDC_COMBO_OFFER,OnSelectOffer)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_EXPIRED, OnButtonDeleteExpired)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_EXPIRED, OnButtonModifyExpired)
END_MESSAGE_MAP()

//$$******************************************************************

CString CEditAccountPropertyPage5::FormatDate(CString strDate, bool bIsExpiryDate)
{
	CString strResult = "";

	if ((strDate.GetLength() == 8) && (::TestNumeric(strDate) == TRUE))
	{
		if ((strDate == CSQLRowOfferStamp::GetNeverExpireDate()) && (TRUE == bIsExpiryDate))
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

BOOL CEditAccountPropertyPage5::OnInitDialog()
{
	m_list.ClearRowHeightFlag();

	CSSPropertyPage::OnInitDialog();

	m_list.SubclassDlgItem(IDC_LIST, this);
	m_list.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.EnableColour();
	m_list.InsertColumn(0, "Offer ID", LVCFMT_LEFT, 60);
	m_list.InsertColumn(1, "Edition", LVCFMT_LEFT, 60);
	m_list.InsertColumn(2, "Offer Name", LVCFMT_LEFT, 250);
	m_list.InsertColumn(3, "Enabled", LVCFMT_LEFT, 80);
	m_list.InsertColumn(4, "Awarded", LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, "Expires", LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, "Stamps", LVCFMT_RIGHT, 60);

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(50);
	m_comboOffer.SetTabStops(arrayStops);

	m_comboOffer.AddItem(",All Offers");
	m_comboOffer.SetItemData(0, 0);

	{
		CSQLRecordSetStampOfferInfo RecordSet(NULL, RSParams_StampOffer_NormalNoParams{});

		int nPos = 1;
		CSQLRowStampOfferInfo RowSO;
		while (RecordSet.StepSelectAll(RowSO) == TRUE)
		{
			int nOfferID = RowSO.GetStampOfferID();

			CString str = "";
			str.Format("%d,%s",
				nOfferID,
				(const char*) RowSO.GetDisplayDescription());

			m_comboOffer.AddItem(str);
			m_comboOffer.SetItemData(nPos++, nOfferID);

			m_StampOfferCache.InsertRow(RowSO);
		}
	}

	m_comboOffer.SetCurSel(0);
	OnSelectOffer();

	return TRUE;
}

//$$******************************************************************

void CEditAccountPropertyPage5::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_list.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
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
			CSQLRowOfferStamp Row = m_arrayStamps.GetAt(nIndex);

			CString strOfferName = "";
			CString strEnabled = "";
			
			{
				CSQLRowStampOfferInfo RowInfo;
				if (m_StampOfferCache.GetStampOfferRow(Row.GetStampOfferID(), RowInfo) == TRUE)
				{
					strOfferName = RowInfo.GetDisplayDescription();
					strEnabled = RowInfo.GetEnableFlag() ? "Yes" : "No";
				}
			}

			CString strExpireDate = Row.GetExpireDate();
			bExpired = (strDateToday > strExpireDate);

			csvLine.Add(Row.GetStampOfferID());
			csvLine.Add(Row.GetEdition());
			csvLine.Add(strOfferName);
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

void CEditAccountPropertyPage5::OnSelectOffer()
{
	int nSel = m_comboOffer.GetCurSel();
	if ((nSel >= 0) && (nSel < m_comboOffer.GetCount()))
	{
		SelectOffer(m_comboOffer.GetItemData(nSel));
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

void CEditAccountPropertyPage5::SelectOffer(int nOfferID)
{
	CSQLRepositoryOfferStamp repoStamp;

	if (0 == nOfferID)
	{
		repoStamp.GetOfferStampsByUserID(m_SQLRowAccount.GetUserID(), m_arrayStamps);
	}
	else
	{
		repoStamp.GetOfferStampsByUserIDAndOfferID(m_SQLRowAccount.GetUserID(), nOfferID, m_arrayStamps);
	}

	m_list.SetItemCountEx(m_arrayStamps.GetSize());
	SelectLine(0);
	m_list.Invalidate();
}

/**********************************************************************/

int CEditAccountPropertyPage5::GetSafeSelection()
{
	int nSel = m_list.GetCurSel();
	if ((nSel < 0) || (nSel >= m_list.GetItemCount()) || ( nSel >= m_arrayStamps.GetSize() ) )
	{
		nSel = -1;
	}

	return nSel;
}

/**********************************************************************/

void CEditAccountPropertyPage5::SelectLine(int nSel)
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

void CEditAccountPropertyPage5::SelectRow(CSQLRowOfferStamp& Row)
{
	int nSel = 0;
	for (int n = 0; (n < m_arrayStamps.GetSize()) && (n < m_list.GetItemCount()); n++)
	{
		CSQLRowOfferStamp arrayRow = m_arrayStamps.GetAt(n);

		if (arrayRow.Compare(Row) == TRUE )
		{
			nSel = n;
			break;
		}
	}

	SelectLine(nSel);
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnButtonAdd()
{
	int nOfferSel = m_comboOffer.GetCurSel();
	if ((nOfferSel < 1) || (nOfferSel >= m_comboOffer.GetCount()))
	{
		return;
	}

	CSQLRepositoryStampOfferInfo repoInfo;

	CSQLRowStampOfferInfo RowOffer;
	RowOffer.SetStampOfferID(m_comboOffer.GetItemData(nOfferSel));
	if (repoInfo.SelectRow(RowOffer, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CSQLRowOfferStamp RowStampsOld;
	RowStampsOld.SetStampOfferID(RowOffer.GetStampOfferID());
	RowStampsOld.SetEdition(RowOffer.GetEdition());
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

	if ((RowOffer.GetExpiry() <= 0) || (RowOffer.GetExpiry() > StampOfferInfo::Expiry.Max))
	{
		RowStampsOld.SetNeverExpire();
	}
	else
	{
		CDateRangeHelpers DateRangeHelpers;
		COleDateTime dateExpire = timeNow;
		DateRangeHelpers.AddDaysToTime(dateExpire, RowOffer.GetExpiry());

		CString strExpireDate = "";
		strExpireDate.Format("%4.4d%4.4d%4.4d",
			dateExpire.GetYear(),
			dateExpire.GetMonth(),
			dateExpire.GetDay());

		RowStampsOld.SetExpireDate(strExpireDate);
	}

	RowStampsOld.SetStampCount(1);
	CSQLRowOfferStamp RowStampsNew = RowStampsOld;

	CEditStampsDlg dlg(RowStampsOld, RowStampsNew, RowOffer, TRUE, this);
	if (dlg.DoModal() == IDOK)
	{
		if (RowStampsNew.GetStampCount() > 0)
		{
			SQLStampOfferBuffer.Reset();

			CSQLTranStampManualAdd Tran;
			Tran.BeginTrans();
			Tran.DoWork(m_SQLRowAccount, RowStampsNew, repoInfo.GetOfferName(RowStampsOld.GetStampOfferID(), NULL));

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();
				break;

			default:
				Tran.ShowErrorMessage("Add Offer Stamps");
				break;
			}

			OnSelectOffer();
			SelectRow(RowStampsNew);
		}
	}
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnButtonEdit()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CSQLRowOfferStamp RowStampsOld = m_arrayStamps.GetAt(nSel);
		CSQLRowOfferStamp RowStampsNew = RowStampsOld;

		CSQLRepositoryStampOfferInfo repoInfo;
		CSQLRepositoryOfferStamp repoStamp;

		CSQLRowStampOfferInfo RowOffer;
		RowOffer.SetStampOfferID(RowStampsOld.GetStampOfferID());
		repoInfo.SelectRow(RowOffer, NULL);
			
		CEditStampsDlg dlg(RowStampsOld, RowStampsNew, RowOffer, FALSE, this);
		if (dlg.DoModal() == IDOK)
		{
			if ((RowStampsOld.GetExpireDate() == RowStampsNew.GetExpireDate()))
			{
				if (RowStampsOld.GetStampCount() != RowStampsNew.GetStampCount())
				{
					if (repoStamp.UpdateRow(RowStampsNew, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						SQLStampOfferBuffer.Reset();
						
						SQLStampOfferBuffer.BufferPCAction(
							APPNO_SO_MEDIT,
							m_SQLRowAccount,
							RowStampsOld,
							RowOffer.GetDisplayDescription(),
							RowStampsNew.GetStampCount() - RowStampsOld.GetStampCount(),
							RowStampsNew.GetStampCount()
						);

						SQLStampOfferBuffer.WritePCAuditLines(NULL);

						OnSelectOffer();
						SelectRow(RowStampsNew);
					}
				}
			}
			else
			{
				SQLStampOfferBuffer.Reset();

				CSQLTranStampManualEdit Tran(APPNO_SO_MEDIT);
				Tran.BeginTrans();
				Tran.DoWork(m_SQLRowAccount,RowStampsOld,RowStampsNew, 
					repoInfo.GetOfferName(RowStampsOld.GetStampOfferID(), NULL));

				switch (Tran.EndTrans())
				{
				case SQLTRAN_STATE_COMMITOK:
					Tran.AfterTransaction();
					break;

				default:
					Tran.ShowErrorMessage("Edit Offer Stamps");
					break;
				}

				OnSelectOffer();
				SelectRow(RowStampsNew);
			}
		}
	}
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnButtonDelete()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		if (Prompter.YesNo("Are you sure you wish to delete this row of stamps ?") == IDYES)
		{
			CSQLRowOfferStamp RowStamps = m_arrayStamps.GetAt(nSel);

			CSQLRepositoryStampOfferInfo repoInfo;
			CSQLRepositoryOfferStamp repoStamp;

			if (repoStamp.DeleteRow(RowStamps, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				SQLStampOfferBuffer.Reset();

				SQLStampOfferBuffer.BufferPCAction(
					APPNO_SO_MDEL,
					m_SQLRowAccount,
					RowStamps,
					repoInfo.GetOfferName(RowStamps.GetStampOfferID(),NULL),
					RowStamps.GetStampCount() * (-1),
					0
				);

				SQLStampOfferBuffer.WritePCAuditLines(NULL);

				CSQLRowOfferStamp RowToSelect;
				if (nSel < m_arrayStamps.GetSize() - 1)
				{
					RowToSelect = m_arrayStamps.GetAt(nSel + 1);
				}
				else if ((nSel > 0) && (nSel < m_arrayStamps.GetSize() + 1))
				{
					RowToSelect = m_arrayStamps.GetAt(nSel - 1);
				}

				OnSelectOffer();
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

void CEditAccountPropertyPage5::OnButtonDeleteExpired()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CSQLRowOfferStamp RowStamps = m_arrayStamps.GetAt(nSel);
		CStampOfferDeleteDlg dlgAdmin(FALSE, m_SQLRowAccount.GetUserID(), RowStamps.GetStampOfferID());
		
		if (dlgAdmin.DoModal() == IDOK)
		{
			OnSelectOffer();
		}
	}
}

/**********************************************************************/

void CEditAccountPropertyPage5::OnButtonModifyExpired()
{
	int nSel = GetSafeSelection();
	if (nSel != -1)
	{
		CSQLRowOfferStamp RowStamps = m_arrayStamps.GetAt(nSel);
		CStampOfferModifyDlg dlgAdmin(FALSE, m_SQLRowAccount.GetUserID(), RowStamps.GetStampOfferID());
		
		if (dlgAdmin.DoModal() == IDOK)
		{
			OnSelectOffer();
		}
	}
}

/**********************************************************************/

BOOL CEditAccountPropertyPage5::OnKillActive()
{
	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEditAccountPropertyPage5::OnOK()
{
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

BOOL CEditAccountPropertyPage5::OnQueryCancel()
{
	return CEditAccountPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditAccountPropertyPage5::OnCancel()
{
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************
