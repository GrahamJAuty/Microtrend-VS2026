/**********************************************************************/
#include "CCSVReportLine.h"
#include "NodeTypes.h"
#include "DefEditTimePlanDlg.h"
#include "NewRecordNumberDlg.h"
/**********************************************************************/
#include "TimePlanTemplateListManager.h"
/**********************************************************************/

CTimePlanTemplateListManager::CTimePlanTemplateListManager(CComboListCtrl& list, CWnd* pParent)
	: m_List(list), m_pParent(pParent)
{
}

/**********************************************************************/

void CTimePlanTemplateListManager::InitList()
{
	m_List.SubclassDlgItem(IDC_LIST, m_pParent);
	m_List.InsertColumn(1, "Time Plan", LVCFMT_LEFT, 180);
	m_List.InsertColumn(2, "Template", LVCFMT_LEFT, 200);
	m_List.InsertColumn(3, "", LVCFMT_LEFT, 60);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.EnableVScroll(TRUE);
}

/**********************************************************************/

void CTimePlanTemplateListManager::CreateTemplateMap(CPromotionsCSVRecord* pPromo)
{
	m_arrayTemplates.RemoveAll();
	
	if (pPromo == nullptr)
	{
		return;
	}
	
	pPromo->GetTimeplanMap(m_arrayTemplates);
}

/**********************************************************************/

void CTimePlanTemplateListManager::CreateTemplateMap(CMixMatchCSVRecord* pMixMatch)
{
	m_arrayTemplates.RemoveAll();
	
	if (pMixMatch == nullptr)
	{
		return;
	}
	
	pMixMatch->GetTimeplanMap(m_arrayTemplates);
}

/**********************************************************************/

void CTimePlanTemplateListManager::AddList()
{
	m_List.DeleteAllItems();
	m_arrayCellTypes.RemoveAll();

	for (int n = 0; n < DataManager.TimePlan.GetSize(); n++)
	{
		CTimePlansCSVRecord TimePlan;
		DataManager.TimePlan.GetAt(n, TimePlan);

		CellTypeInfoTextBuffer arrayTexts;
		arrayTexts.AddText(",None", "None");

		int nStart = 0, nEnd = 0;
		DataManager.TimePlanTemplateEntry.GetPlanRange(TimePlan.GetPlanNo(), nStart, nEnd);

		int nEntryNoToFind = -1;

		{
			int nPos = 0;
			CSortedIntByInt item;
			item.m_nKey = TimePlan.GetPlanNo();
			if (m_arrayTemplates.Find(item, nPos) == TRUE)
			{
				m_arrayTemplates.GetAt(nPos, item);
				nEntryNoToFind = item.m_nVal;
			}
		}

		CString strCurrent = "None";
		int nCurSel = 0;

		for (int n = nStart; n <= nEnd; n++)
		{
			CTimePlanEntriesCSVRecord Entry;
			DataManager.TimePlanTemplateEntry.GetAt(n, Entry);

			CString strListText = "", strCellText = "";
			strListText.Format("T%3.3d,%s", Entry.GetEntryNo(), Entry.GetName());
			strCellText.Format("T%3.3d  %s", Entry.GetEntryNo(), Entry.GetName());
			arrayTexts.AddText(strListText, strCellText);

			if (Entry.GetEntryNo() == nEntryNoToFind)
			{
				strCurrent = strCellText;
				nCurSel = (n - nStart) + 1;
			}
		}

		CellTypeInfo info;
		info.m_strComboTabs = "5,50";
		info.ResetForCombo(arrayTexts);
		info.m_nComboSelect = nCurSel;
		m_arrayCellTypes.Add(info);

		CCSV csv;
		csv.Add(TimePlan.GetPlanNo());
		csv.Add(TimePlan.GetDisplayName());
		csv.Add(strCurrent);

		int nPos = m_List.GetItemCount();
		m_List.InsertItem(nPos, "");
		m_List.SetItem(nPos, 0, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL);
		m_List.SetItem(nPos, 1, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL);
		m_List.SetItem(nPos, 2, LVIF_TEXT, "Edit", 0, 0, 0, NULL);
		m_List.SetItemData(nPos, csv.GetInt(0));
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::HandleGetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);

	int nCol = pCellType->m_nCol;
	int nRow = pCellType->m_nRow;

	switch (nCol)
	{
	case 1:
		pCellType->m_nType = CELL_TYPE_COMBO_TABBED;
		break;

	case 2:
		pCellType->m_nType = CELL_TYPE_BUTTON;
		break;

	default:
		pCellType->m_nType = 0;
		return;
	}

	if (nRow < 0 || nRow >= m_arrayCellTypes.GetSize())
	{
		pCellType->m_nType = 0;
		return;
	}

	pCellType->m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;
}

/**********************************************************************/

void CTimePlanTemplateListManager::HandlePopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (m_pParent -> GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft());

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_List.GetWindowRect(&obWindowRect);

	int iRowIndex = wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft))
	{
		if (iRowIndex >= 0 && iRowIndex < m_arrayCellTypes.GetSize())
		{
			CCSV csv(m_arrayCellTypes[iRowIndex].m_strComboListText);

			if (m_arrayCellTypes[iRowIndex].m_nType == CELL_TYPE_COMBO_TABBED)
			{
				pComboList->AddTail(m_arrayCellTypes[iRowIndex].m_strComboTabs);
			}

			for (int n = 0; n < csv.GetSize(); n++)
			{
				pComboList->AddTail(csv.GetString(n));
			}
		}
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::HandleEndLabelEdit(WPARAM wParam, LPARAM lParam)
{
	m_List.AllowListScroll(TRUE);
	m_List.SetEditCell(-1, -1);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;

	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;

		int nRow = pDispInfo->item.iItem;
		int nCol = pDispInfo->item.iSubItem;

		if (nRow >= 0 && nRow < m_arrayCellTypes.GetSize())
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt(nRow);

			switch (info.m_nType)
			{
			case CELL_TYPE_COMBO_NORMAL:
			case CELL_TYPE_COMBO_TABBED:
			{
				int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
				int nNewSel = atoi(strText) - 1;
				{
					CCSV csv(info.m_strComboCellText);
					if ((nNewSel >= 0) && (nNewSel < csv.GetSize()))
					{
						m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
						m_List.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nNewSel), 0, 0, 0, NULL);
					}
					else if ((nOldSel >= 0) && (nOldSel < csv.GetSize()))
					{
						m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
						m_List.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nOldSel), 0, 0, 0, NULL);
					}
				}
			}
			break;
			}
		}
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::HandleCellButton(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);

	int nRow = pCellType->m_nRow;

	if ((nRow < 0) || (nRow >= DataManager.TimePlan.GetSize()) || (nRow >= m_arrayCellTypes.GetSize()))
	{
		return;
	}

	CellTypeInfo info = m_arrayCellTypes[nRow];
	if ((info.m_nComboSelect < 0) || (info.m_nComboSelect > info.m_nMaxInt))
	{
		return;
	}

	int nPlanNo = m_List.GetItemData(nRow);

	{
		int nPos = 0;
		if (DataManager.TimePlan.FindPlanByNumber(nPlanNo, nPos) == FALSE)
		{
			return;
		}
	}

	if (0 == info.m_nComboSelect)
	{
		CreateNewTimeplanTemplateEntry(nPlanNo);
	}
	else
	{
		int nStart = 0, nEnd = 0;
		DataManager.TimePlanTemplateEntry.GetPlanRange(nPlanNo, nStart, nEnd);

		int nPos = nStart + info.m_nComboSelect - 1;
		if ((nPos >= nStart) && (nPos >= 0) && (nPos <= nEnd) && (nPos < DataManager.TimePlanTemplateEntry.GetSize()))
		{
			EditTimeplanTemplateEntry(FALSE, nPlanNo, nPos);
		}
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::CreateNewTimeplanTemplateEntry(int nPlanNo)
{
	int nEntryNo = DataManager.TimePlanTemplateEntry.FindFirstFreeNumber(nPlanNo);

	if (nEntryNo < 1)
	{
		Prompter.Error("Unable to create new Template Entry.\nAll entry numbers are already in use.");
		return;
	}

	CNewRecordNumberDlg dlg(NODE_TIMEPLAN_TEMPLATE_ENTRY, m_pParent);
	dlg.m_nNumber = nEntryNo;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	nEntryNo = dlg.m_nNumber;

	CTimePlanEntriesCSVRecord Entry;
	Entry.SetPlanNo(nPlanNo);
	Entry.SetEntryNo(nEntryNo);

	int nEntryIdx = 0;
	if (DataManager.TimePlanTemplateEntry.FindKey(Entry, nEntryIdx) == TRUE)
	{
		Prompter.Error("The selected entry number is already in use.");
		return;
	}

	Entry.SetName("");
	DataManager.TimePlanTemplateEntry.InsertAt(nEntryIdx, Entry);
	EditTimeplanTemplateEntry(TRUE, nPlanNo, nEntryIdx);
}

/**********************************************************************/

void CTimePlanTemplateListManager::EditTimeplanTemplateEntry(bool bNew, int nPlanNo, int nPos)
{
	CListDataDlgChangeFlag ChangeFlag;
	CDefEditTimePlanDlg dlg(TRUE, nPos, nPos, nPos, FALSE, ChangeFlag, m_pParent);

	if ((dlg.DoModal() == IDOK) || (TRUE == bNew))
	{
		CTimePlanEntriesCSVRecord Entry;
		DataManager.TimePlanTemplateEntry.GetAt(nPos, Entry);

		CSortedIntByInt item;
		item.m_nKey = nPlanNo;
		item.m_nVal = Entry.GetEntryNo();

		int nItemPos = 0;
		if (m_arrayTemplates.Find(item, nItemPos) == TRUE)
		{
			m_arrayTemplates.SetAt(nItemPos, item);
		}
		else
		{
			m_arrayTemplates.InsertAt(nItemPos, item);
		}

		AddList();
	}
}

/**********************************************************************/

CString CTimePlanTemplateListManager::GetTimePlanTemplateList()
{
	m_strTimePlanTemplateList = "";
	
	CCSV csvTimeplans;

	for (int n = 0; n < m_List.GetItemCount(); n++)
	{
		if (n >= m_arrayCellTypes.GetSize())
		{
			break;
		}

		CellTypeInfo info = m_arrayCellTypes[n];

		if ((info.m_nComboSelect < 1) || (info.m_nComboSelect > info.m_nMaxInt))
		{
			continue;
		}

		int nCellInt = info.m_nComboSelect - 1;
		if (nCellInt < 0)
		{
			continue;
		}

		int nPos = 0;
		int nPlanNo = m_List.GetItemData(n);
		if (DataManager.TimePlan.FindPlanByNumber(nPlanNo, nPos) == FALSE)
		{
			continue;
		}

		int nStart = 0, nEnd = 0;
		DataManager.TimePlanTemplateEntry.GetPlanRange(nPlanNo, nStart, nEnd);

		nPos = nStart + nCellInt;
		if ((nPos < nStart) || (nPos < 0) || (nPos > nEnd) || (nPos >= DataManager.TimePlanTemplateEntry.GetSize()))
		{
			continue;
		}

		CTimePlanEntriesCSVRecord Entry;
		DataManager.TimePlanTemplateEntry.GetAt(nPos, Entry);

		csvTimeplans.Add(nPlanNo);
		csvTimeplans.Add(Entry.GetEntryNo());
	}
	
	m_strTimePlanTemplateList = csvTimeplans.GetLine();
	return m_strTimePlanTemplateList;
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetTimeplanReportDetails(int nType, int& nLineNo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries)
{
	nLineNo = 0;
	for (int n = 0; n < DataManager.TimePlanEntry.GetSize(); n++)
	{
		CTimePlanEntriesCSVRecord Entry;
		DataManager.TimePlanEntry.GetAt(n, Entry);

		if (Entry.GetType() == nType)
		{
			CSortedTimeplanEntry SortedEntry;
			SortedEntry.m_nPromoNo = Entry.GetSelection();
			SortedEntry.m_nPlanNo = Entry.GetPlanNo();
			SortedEntry.m_nTemplate = 0;
			SortedEntry.m_nLineNo = ++nLineNo;
			SortedEntry.m_nEntryNo = Entry.GetEntryNo();
			arrayTimeplanEntries.Consolidate(SortedEntry);
		}
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetTimeplanReportDetails(int& nLineNo, CPromotionsCSVRecord& Promo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries)
{
	CReportConsolidationArray<CSortedIntByInt> TimeplanMap;
	Promo.GetTimeplanMap(TimeplanMap);
	GetTimeplanReportDetails(nLineNo, Promo.GetPromoNo(), TimeplanMap, arrayTimeplanEntries);
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetTimeplanReportDetails(int& nLineNo, CMixMatchCSVRecord& MixMatch, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries)
{
	CReportConsolidationArray<CSortedIntByInt> TimeplanMap;
	MixMatch.GetTimeplanMap(TimeplanMap);
	GetTimeplanReportDetails(nLineNo, MixMatch.GetMixMatchNo(), TimeplanMap, arrayTimeplanEntries);
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetTimeplanReportDetails(int& nLineNo, int nPromoNo, CReportConsolidationArray<CSortedIntByInt>& TimeplanMap, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries)
{
	for (int m = 0; m < TimeplanMap.GetSize(); m++)
	{
		CSortedIntByInt item;
		TimeplanMap.GetAt(m, item);

		CSortedTimeplanEntry SortedEntry;
		SortedEntry.m_nPromoNo = nPromoNo;
		SortedEntry.m_nPlanNo = item.m_nKey;
		SortedEntry.m_nTemplate = 1;
		SortedEntry.m_nLineNo = ++nLineNo;
		SortedEntry.m_nEntryNo = item.m_nVal;
		arrayTimeplanEntries.Consolidate(SortedEntry);
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetReportDetailsEntryRange(int nPromoNo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int& nEntryStartIdx, int& nEntryEndIdx)
{
	CSortedTimeplanEntry SortedEntry;
	SortedEntry.m_nPromoNo = nPromoNo;
	SortedEntry.m_nPlanNo = 0;
	arrayTimeplanEntries.Find(SortedEntry, nEntryStartIdx);
	SortedEntry.m_nPromoNo++;
	arrayTimeplanEntries.Find(SortedEntry, nEntryEndIdx);
	nEntryEndIdx--;
}

/**********************************************************************/

void CTimePlanTemplateListManager::ReportExtraTimeplans(CReportFile& ReportFile, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int nEntryStartIdx, int nEntryEndIdx, int nBlanks)
{
	for (int x = nEntryStartIdx + 1; x <= nEntryEndIdx; x++)
	{
		CCSVReportLine csvOut;
		csvOut.AppendBlanks(nBlanks);
		CString strPlanName = "";
		CString strEntryName = "";
		GetTimeplanEntryDetails(arrayTimeplanEntries, x, strPlanName, strEntryName);
		csvOut.AppendString(strPlanName);
		csvOut.AppendString(strEntryName);
		ReportFile.WriteLine(csvOut.GetLine());
	}
}

/**********************************************************************/

void CTimePlanTemplateListManager::GetTimeplanEntryDetails(CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int nEntryIdx, CString& strPlanName, CString& strEntryName)
{
	strPlanName = "";
	strEntryName = "";

	if ((nEntryIdx < 0) || (nEntryIdx >= arrayTimeplanEntries.GetSize()))
	{
		return;
	}

	CSortedTimeplanEntry SortedEntry;
	arrayTimeplanEntries.GetAt(nEntryIdx, SortedEntry);

	strPlanName.Format("P%3.3d ", SortedEntry.m_nPlanNo);

	{
		int nPos = 0;
		if (DataManager.TimePlan.FindPlanByNumber(SortedEntry.m_nPlanNo, nPos) == TRUE)
		{
			CTimePlansCSVRecord TimePlan;
			DataManager.TimePlan.GetAt(nPos, TimePlan);
			strPlanName += TimePlan.GetDisplayName();
		}
		else
		{
			strPlanName += "Unknown";
		}
	}

	CTimePlanEntriesCSVArray* pEntryArray = nullptr;

	if (SortedEntry.m_nTemplate == 0)
	{
		pEntryArray = &DataManager.TimePlanEntry;
		strEntryName = "E: ";
	}
	else
	{
		pEntryArray = &DataManager.TimePlanTemplateEntry;
		strEntryName = "T: ";
	}

	CTimePlanEntriesCSVRecord Entry;
	Entry.SetPlanNo(SortedEntry.m_nPlanNo);
	Entry.SetEntryNo(SortedEntry.m_nEntryNo);

	int nPos = 0;
	if (pEntryArray->FindKey(Entry, nPos) == TRUE)
	{
		pEntryArray->GetAt(nPos, Entry);
		strEntryName += Entry.GetName();
	}
	else
	{
		strEntryName += "Unknown";
	}
}

/**********************************************************************/
