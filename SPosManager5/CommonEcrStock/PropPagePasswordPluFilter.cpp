/**********************************************************************/
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "PluDefineFilterDlg.h"
#include "PresetPluFilter.h"
/**********************************************************************/
#include "PropPagePasswordPluFilter.h"
/**********************************************************************/

CPropPagePasswordPluFilter::CPropPagePasswordPluFilter() : CPropertyPage(CPropPagePasswordPluFilter::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePasswordPluFilter)
	//}}AFX_DATA_INIT

	m_arrayTexts.AddText("No Preset Filter");

	for (int n = 1; n <= 20; n++)
	{
		CString strPreset = "";
		strPreset.Format("Preset Filter %2.2d", n);
		m_arrayTexts.AddText(strPreset);
	}

	m_bBlockSpinButton = FALSE;
}

/**********************************************************************/

CPropPagePasswordPluFilter::~CPropPagePasswordPluFilter()
{
}

/**********************************************************************/

void CPropPagePasswordPluFilter::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePasswordPluFilter)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePasswordPluFilter, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePasswordPluFilter)
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
	ON_MESSAGE(WM_CELL_BUTTON, OnCellButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePasswordPluFilter::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listDatabases.SubclassDlgItem(IDC_LIST, this);
	m_listDatabases.InsertColumn(1, "Database", LVCFMT_LEFT, 280);
	m_listDatabases.InsertColumn(2, "Preset", LVCFMT_LEFT, 200);
	m_listDatabases.InsertColumn(3, "", LVCFMT_LEFT, 60);
	m_listDatabases.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listDatabases.EnableVScroll(TRUE);

	return TRUE;  
}

/**********************************************************************/

void CPropPagePasswordPluFilter::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listDatabases.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

BOOL CPropPagePasswordPluFilter::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == &m_listDatabases)
			{
				m_listDatabases.HandleReturn(1);
				pMsg->wParam = NULL;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPropPagePasswordPluFilter::AddList()
{
	m_listDatabases.DeleteAllItems();
	m_arrayCellTypes.RemoveAll();

	CReportConsolidationArray<CSortedIntByInt> arrayPluFilter;
	m_pPasswordRecord->GetPluFilterArray(arrayPluFilter);

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		CString strDatabase = "";
		strDatabase.Format("%3.3d %s",
			dbDatabase.GetDbNo(nDbIdx),
			dbDatabase.GetName(nDbIdx));

		int nPreset = 0;
		{
			CSortedIntByInt item;
			item.m_nKey = dbDatabase.GetDbNo(nDbIdx);

			int nPos;
			if (arrayPluFilter.Find(item, nPos))
			{
				arrayPluFilter.GetAt(nPos, item);

				nPreset = item.m_nVal;
				if ((nPreset < 1) || (nPreset > 20))
				{
					nPreset = 0;
				}
			}
		}

		CellTypeInfoTextItem item;
		m_arrayTexts.GetItem(nPreset, item);
		CString strPreset = item.m_strCellText;

		m_listDatabases.InsertItem(nDbIdx, "");
		m_listDatabases.SetItem(nDbIdx, 0, LVIF_TEXT, strDatabase, 0, 0, 0, NULL);
		m_listDatabases.SetItem(nDbIdx, 1, LVIF_TEXT, strPreset, 0, 0, 0, NULL);
		m_listDatabases.SetItem(nDbIdx, 2, LVIF_TEXT, "Edit", 0, 0, 0, NULL);

		CellTypeInfo info;
		info.m_strComboTabs = "";
		info.ResetForCombo(m_arrayTexts);
		info.m_nComboSelect = nPreset;
		m_arrayCellTypes.Add(info);
	}
}

/**********************************************************************/

LRESULT CPropPagePasswordPluFilter::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	m_bBlockSpinButton = TRUE;

	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft());

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_listDatabases.GetWindowRect(&obWindowRect);

	int iRowIndex = wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft))
	{
		if (iRowIndex >= 0 && iRowIndex < m_arrayCellTypes.GetSize())
		{
			CCSV csv(m_arrayCellTypes[iRowIndex].m_strComboListText);

			if (m_arrayCellTypes[iRowIndex].m_nType == CELL_TYPE_COMBO_TABBED)
				pComboList->AddTail(m_arrayCellTypes[iRowIndex].m_strComboTabs);

			for (int n = 0; n < csv.GetSize(); n++)
				pComboList->AddTail(csv.GetString(n));
		}
	}

	return true;
}

/**********************************************************************/

LRESULT CPropPagePasswordPluFilter::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_listDatabases.AllowListScroll(TRUE);
	m_listDatabases.SetEditCell(-1, -1);

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
				m_bBlockSpinButton = FALSE;

				int nOldSel = m_arrayCellTypes[nRow].m_nComboSelect;
				int nNewSel = atoi(strText) - 1;
				{
					CCSV csv(info.m_strComboCellText);
					if ((nNewSel >= 0) && (nNewSel < csv.GetSize()))
					{
						m_arrayCellTypes[nRow].m_nComboSelect = nNewSel;
						m_listDatabases.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nNewSel), 0, 0, 0, NULL);
					}
					else if ((nOldSel >= 0) && (nOldSel < csv.GetSize()))
					{
						m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
						m_listDatabases.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nOldSel), 0, 0, 0, NULL);
					}
				}
			}
			break;
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPropPagePasswordPluFilter::OnCellButton(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);

	int nDbIdx = pCellType->m_nRow;
	if ((nDbIdx) >= 0 && (nDbIdx < m_arrayCellTypes.GetSize()) && (nDbIdx < dbDatabase.GetSize()))
	{
		int nPreset = m_arrayCellTypes[nDbIdx].m_nComboSelect;

		if (0 == nPreset)
		{
			Prompter.Error("No Preset Selected for Editing");
		}
		else
		{
			CPresetPluFilter Preset;
			Preset.SetFilename(nDbIdx, nPreset);
			Preset.Read();

			CPluFilterArray FilterArray(FILTERTYPE_DATABASE);
			FilterArray.CopyFromPreset(Preset);

			CString strTitle = "";
			strTitle.Format("Plu Filter Preset (D%3.3d %s, Preset %d)",
				dbDatabase.GetDbNo(nDbIdx),
				dbDatabase.GetName(nDbIdx),
				nPreset);

			CDataManagerInfo info;
			DataManager.OpenDatabaseQuickNoFiles(nDbIdx, TRUE);
			DataManager.ReopenDepartment(info);

			CPluDefineFilterDlg dlgFilter(FilterArray, this);
			dlgFilter.SetExternalTitle(strTitle);

			if (dlgFilter.DoModal() == IDOK)
			{
				FilterArray.CopyToPreset(Preset);
				Preset.Write();
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPropPagePasswordPluFilter::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);

	int nCol = pCellType->m_nCol;
	int nRow = pCellType->m_nRow;

	pCellType->m_nType = 0;
	pCellType->m_nComboSelect = 0;

	if ((nRow >= 0) && (nRow < m_arrayCellTypes.GetSize()))
	{
		switch (nCol)
		{
		case 1:
			pCellType->m_nType = CELL_TYPE_COMBO_NORMAL;
			pCellType->m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;
			break;

		case 2:
			pCellType->m_nType = CELL_TYPE_BUTTON;
			break;
		}
	}

	return 1;
}

/**********************************************************************/

BOOL CPropPagePasswordPluFilter::OnKillActive()
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePasswordPluFilter::OnSetActive()
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePasswordPluFilter::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePasswordPluFilter::SetRecordControls()
{
	AddList();
}

/**********************************************************************/

bool CPropPagePasswordPluFilter::UpdateRecord()
{
	if (UpdateData(TRUE) == TRUE)
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePasswordPluFilter::SaveRecord()
{
	CReportConsolidationArray<CSortedIntByInt> arrayPluFilter;

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		CellTypeInfo info = m_arrayCellTypes[nDbIdx];

		int nPreset = info.m_nComboSelect;
		if ((nPreset >= 1) && (nPreset <= 20))
		{
			CSortedIntByInt item;
			item.m_nKey = dbDatabase.GetDbNo(nDbIdx);
			item.m_nVal = nPreset;
			arrayPluFilter.Consolidate(item);
		}
	}

	m_pPasswordRecord->SetPluFilterArray(arrayPluFilter);
}

/**********************************************************************/

void CPropPagePasswordPluFilter::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
