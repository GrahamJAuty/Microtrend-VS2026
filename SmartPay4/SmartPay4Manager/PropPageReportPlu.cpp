//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRecordSetPluInfo.h"
//$$******************************************************************
#include "ColumnHandler.h"
#include "ReportDefaultHandler.h"
#include "ReportPluRanking.h"
//$$******************************************************************
#include "PropPageReportPlu.h"
//$$******************************************************************

CPropPageReportPlu::CPropPageReportPlu() : CSSPropertyPage(CPropPageReportPlu::IDD)
{
	m_ReportSettings.m_bIncludeUnknownItems = FALSE;
	m_ReportSettings.m_bNewPage = FALSE;
	m_ReportSettings.m_bAlwaysAll = FALSE;
	m_ReportSettings.m_strFindText = "";
	/*****/
	m_bPluRankingMode = FALSE;
	m_strPluDataDefaults = "";
	m_bAdjustedRowHeight = FALSE;
}

//$$******************************************************************

CPropPageReportPlu::~CPropPageReportPlu()
{
}

//$$******************************************************************

void CPropPageReportPlu::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_UNKNOWN, m_checkUnknown);
	DDX_Control(pDX, IDC_CHECK_ALWAYSALL, m_checkAlwaysAll);
	DDX_Control(pDX, IDC_EDIT_FIND, m_editFind);
	DDX_Control(pDX, IDC_SPIN_FIND, m_spinFind);
	DDX_Control(pDX, IDC_STATIC_COUNT, m_staticCount);
}

//$$******************************************************************

void CPropPageReportPlu::LoadAccumulatorReportInfo(CAccumulatorReportInfo* pInfo)
{
	m_bPluRankingMode = TRUE;
	m_strPluDataDefaults = Filenames.GetPluRankingListFilename();
	
	m_ReportSettings.m_bNewPage = pInfo->GetNewPageFlag();
	m_ReportSettings.m_bIncludeUnknownItems = pInfo->GetIncludeUnknownItemsFlag();
	
	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)
		{
			m_ReportSettings.m_bNewPage = csv.GetBool(0);
			m_ReportSettings.m_bIncludeUnknownItems = csv.GetBool(1);
		}
	}
}

//$$******************************************************************

void CPropPageReportPlu::LoadReportPurchaseHelpers(CReportPurchaseHelpers& ReportPurchaseHelpers)
{
	m_bPluRankingMode = FALSE;
	m_strPluDataDefaults = Filenames.GetPluPurchaseListFilename(ReportPurchaseHelpers.GetReportLabelInternal());
	m_ReportSettings.m_strFindText = ReportPurchaseHelpers.GetSelectivePluText();

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(ReportPurchaseHelpers.GetReportLabelExternal(), strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)
		{
			m_ReportSettings.m_bAlwaysAll = (csv.GetBool(0) == FALSE);
		}
	}
}

//$$******************************************************************

void CPropPageReportPlu::SaveAccumulatorReportInfo(CAccumulatorReportInfo* pInfo)
{
	pInfo->SetIncludeUnknownItemsFlag(m_ReportSettings.m_bIncludeUnknownItems);
	pInfo->SetNewPageFlag(m_ReportSettings.m_bNewPage);

	CCSV csv;
	csv.Add(m_ReportSettings.m_bNewPage);
	csv.Add(m_ReportSettings.m_bIncludeUnknownItems);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strReportLabel, &csv);
}

//$$******************************************************************

void CPropPageReportPlu::SaveReportPurchaseHelpers(CReportPurchaseHelpers& ReportPurchaseHelpers)
{
	ReportPurchaseHelpers.SetSelectivePluFlag(m_ReportSettings.m_bAlwaysAll == FALSE);
	ReportPurchaseHelpers.SetSelectivePluText(m_ReportSettings.m_strFindText);

	CCSV csv;
	csv.Add(ReportPurchaseHelpers.GetSelectivePluFlag());

	CReportDefaultHandler repdefs;
	repdefs.Save(ReportPurchaseHelpers.GetReportLabelExternal(), &csv);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CPropPageReportPlu, CSSPropertyPage)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL,OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FIND, OnDeltaposSpinFind)
	ON_MESSAGE(WM_APP + 2, OnUpdateCount)
END_MESSAGE_MAP()

//$$******************************************************************

void CPropPageReportPlu::SetReportLabel(const char* szLabel)
{
	m_strReportLabel = szLabel;
}

//$$******************************************************************

BOOL CPropPageReportPlu::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_listboxPlu.ClearRowHeightFlag();
	m_listboxPlu.SetMultiColumnsFlag(TRUE);
	m_listboxPlu.SubclassDlgItem(IDC_LIST_PLU, this);
	m_listboxPlu.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listboxPlu.InsertColumn(0, "PluNo", LVCFMT_LEFT, 100);
	m_listboxPlu.InsertColumn(1, "Description", LVCFMT_LEFT, 230);
	m_listboxPlu.SetLockItemZeroFlag(FALSE);
	m_listboxPlu.SetTagStateNotifyMessage(this, WM_APP + 2);

	if (TRUE == m_bPluRankingMode)
	{
		m_checkNewPage.SetCheck(m_ReportSettings.m_bNewPage);
		m_checkUnknown.SetCheck(m_ReportSettings.m_bIncludeUnknownItems);
		ShowAndEnableWindow(&m_checkAlwaysAll, FALSE);
	}
	else
	{
		m_checkAlwaysAll.SetCheck(m_ReportSettings.m_bAlwaysAll);
		m_editFind.SetWindowText(m_ReportSettings.m_strFindText);
		ShowAndEnableWindow(&m_checkNewPage, FALSE);
		ShowAndEnableWindow(&m_checkUnknown, FALSE);
		MoveControl(&m_listboxPlu, 7, 7, 249, 288);
		MoveControl(&m_checkAlwaysAll, 19, 310);
	}

	CWaitCursor cursor;

	Server.Read(Filenames.GetServerDataFilename());

	int nTotalWidth = 0;

	CColumnHandler column;
	if (column.Read("PLULIST", 3) == TRUE)
	{
		nTotalWidth += column.GetWidth(0);
		nTotalWidth += column.GetWidth(1);
		nTotalWidth += column.GetWidth(2);
	}

	if ((nTotalWidth < 300) || (nTotalWidth > 330))
	{
		column.SetWidth(0, 90);
		column.SetWidth(1, 190);
		column.SetWidth(2, 50);
	}

	FillPluList();

	return TRUE;
}

//$$******************************************************************

long CPropPageReportPlu::OnUpdateCount(WPARAM wIndex, LPARAM lParam)
{
	DisplayCount();
	return 0l;
}

//*****************************************************************************

void CPropPageReportPlu::DisplayCount()
{
	CString strCount = "";
	strCount.Format("Selected: %d", m_listboxPlu.GetTaggedItemCount());
	m_staticCount.SetWindowText(strCount);
}

//$$******************************************************************

void CPropPageReportPlu::FillPluList()
{
	m_listboxPlu.DeleteAllItems();
	
	CCSV csvPlu('|');
	csvPlu.Add(szDISCOUNT_PLUNO);
	csvPlu.Add(szDISCOUNT_ECRTEXT);

	CSSListTaggedSelectItem itemPlu;
	itemPlu.m_strText = csvPlu.GetLine();
	itemPlu.SetTagState(0);
	m_listboxPlu.AddItemToList(itemPlu);

	m_arrayPluNo.RemoveAll();

	CReportSelectPlu PluInfo;
	PluInfo.m_nPluNo = 0;
	PluInfo.m_bDefaultSelect = FALSE;
	m_arrayPluNo.Add(PluInfo);

	CSQLRecordSetPluInfo RecordSet(NULL, RSParams_PluInfo_NormalNoParams());
	
	CSQLRowPluInfo RowPluInfo;
	while (RecordSet.StepSelectAll(RowPluInfo) == TRUE)
	{
		if (RowPluInfo.IsBasePlu() == TRUE)
		{
			CString strBasePluNo = RowPluInfo.GetMicrotrendBasePluNo(TRUE);

			CString strPoints = "";
			strPoints.Format("%d", RowPluInfo.GetPoints());

			CCSV csvPlu('|');
			csvPlu.Add(strBasePluNo);
			csvPlu.Add(RowPluInfo.GetDescription());

			CSSListTaggedSelectItem itemPlu;
			itemPlu.m_strText = csvPlu.GetLine();
			itemPlu.SetTagState(0);
			m_listboxPlu.AddItemToList(itemPlu);

			CReportSelectPlu PluInfo;
			PluInfo.m_nPluNo = _atoi64(strBasePluNo);
			PluInfo.m_strText = RowPluInfo.GetDescription();
			PluInfo.m_bDefaultSelect = FALSE;
			m_arrayPluNo.Add(PluInfo);
		}
	}

	CReportConsolidationArray<CSortedInt64Item> arrayDefaults;
	CSortedInt64Item itemDefault;

	{
		CSSFile file;
		if (file.Open(m_strPluDataDefaults, "rb") == TRUE)
		{
			CString strBuf = "";
			__int64 nLast = -1;

			while (file.ReadString(strBuf) == TRUE)
			{
				itemDefault.m_nItem = _atoi64(strBuf);

				if (itemDefault.m_nItem > nLast)
				{
					arrayDefaults.DirectAdd(itemDefault);
					nLast = itemDefault.m_nItem;	
				}
				else
				{
					arrayDefaults.Consolidate(itemDefault);
				}
			}
			file.Close();
		}
	}

	int nCurSel = 0;
	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		CReportSelectPlu infoPlu = m_arrayPluNo.GetAt(i);

		itemDefault.m_nItem = infoPlu.m_nPluNo;

		int nPos = 0;
		if (arrayDefaults.Find(itemDefault, nPos) == TRUE)
		{
			m_listboxPlu.SetItemTagState(i, 1);

			if (m_listboxPlu.GetTaggedItemCount() == 1)
			{
				nCurSel = i;
			}

			infoPlu.m_bDefaultSelect = TRUE;
			m_arrayPluNo.SetAt(i, infoPlu);
		}
	}

	DisplayCount();
}

//$$******************************************************************

void CPropPageReportPlu::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_PLU)
	{
		if (FALSE == m_bAdjustedRowHeight)
		{
			m_listboxPlu.AdjustRowHeight(lpMeasureItemStruct, 10);
			m_bAdjustedRowHeight = TRUE;
		}
	}
}

//$$******************************************************************

void CPropPageReportPlu::OnButtonAll()
{
	m_listboxPlu.SelectAll();
	DisplayCount();
}

//$$******************************************************************

void CPropPageReportPlu::OnButtonNone()
{
	m_listboxPlu.SelectNone();
	DisplayCount();
}

//$$******************************************************************

void CPropPageReportPlu::OnButtonReset()
{
	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		CReportSelectPlu infoPlu = m_arrayPluNo.GetAt(i);
		m_listboxPlu.SetItemTagState(i, infoPlu.m_bDefaultSelect);
	}
	m_listboxPlu.RedrawWindow();
}

//$$******************************************************************

void CPropPageReportPlu::OnDeltaposSpinFind(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nIndex = -1;
	int nCurSel = m_listboxPlu.GetCurSel();

	CString strFindString = GetEditBoxText(m_editFind);
	
	if (m_listboxPlu.GetCurSel() >= 0)
	{
		m_listboxPlu.SetItemState(nCurSel, 0, LVIS_SELECTED);
	}

	if (strFindString == "")
	{
		nIndex = nCurSel;
		bool bFound = FALSE;

		if (pNMUpDown->iDelta > 0)			// Down
		{
			for (++nIndex; nIndex < m_listboxPlu.GetItemCount(); nIndex++)
			{
				CSSListTaggedSelectItem itemPlu;
				m_listboxPlu.GetListItem(nIndex, itemPlu);

				if (itemPlu.GetTagState() == 1)
				{
					bFound = TRUE;
					break;
				}
			}
		}
		else									// up
		{
			for (--nIndex; nIndex >= 0; nIndex--)
			{
				CSSListTaggedSelectItem itemPlu;
				m_listboxPlu.GetListItem(nIndex, itemPlu);

				if (itemPlu.GetTagState() == 1)
				{
					bFound = TRUE;
					break;
				}
			}
		}

		if (bFound == FALSE)
		{
			nIndex = -1;
		}
	}
	else
	{
		if (pNMUpDown->iDelta > 0)
		{
			nIndex = FindNext(nCurSel, TRUE);	// previous / Down
		}
		else
		{
			nIndex = FindNext(nCurSel, FALSE);		// next / Up
		}
	}

	if (nIndex != -1)
	{
		m_listboxPlu.SetCurSel(nIndex);
	}

	*pResult = 0;
}

//*****************************************************************************

int CPropPageReportPlu::FindNext(int nStartPos, bool bAscending)
{
	int nIndex = -1;

	CString strFindString = GetEditBoxText(m_editFind);
	strFindString.MakeUpper();

	if (bAscending == TRUE)
	{
		for (int i = nStartPos + 1; i < m_listboxPlu.GetItemCount(); i++)
		{
			CSSListTaggedSelectItem itemPlu;
			m_listboxPlu.GetListItem(i, itemPlu);
			CString strText = itemPlu.m_strText;

			strText.MakeUpper();

			if (strText.Find(strFindString) > -1)
			{
				nIndex = i;
				break;
			}
		}
	}
	else
	{
		for (int i = nStartPos - 1; i >= 0; i--)
		{
			CSSListTaggedSelectItem itemPlu;
			m_listboxPlu.GetListItem(i, itemPlu);
			CString strText = itemPlu.m_strText;

			strText.MakeUpper();

			if (strText.Find(strFindString) > -1)
			{
				nIndex = i;
				break;
			}
		}
	}

	return nIndex;
}

//*****************************************************************************

BOOL CPropPageReportPlu::OnSetActive()
{
	return CSSPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CPropPageReportPlu::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_ReportSettings.m_bNewPage = IsTicked(m_checkNewPage);
		m_ReportSettings.m_bIncludeUnknownItems = IsTicked(m_checkUnknown);
		m_ReportSettings.m_bAlwaysAll = IsTicked(m_checkAlwaysAll);
		m_ReportSettings.m_strFindText = GetEditBoxText(m_editFind);

		CSSFile file;
		if (file.Open(m_strPluDataDefaults, "wb") == TRUE)
		{
			for (int n = 0; n < m_listboxPlu.GetItemCount(); n++)
			{
				CSSListTaggedSelectItem itemPlu;
				m_listboxPlu.GetListItem(n, itemPlu);

				if (itemPlu.GetTagState() == 1)
				{
					CReportSelectPlu infoPlu = m_arrayPluNo.GetAt(n);

					CString strPluNo = "";
					strPluNo.Format("%I64d", infoPlu.m_nPluNo);
					file.WriteLine(strPluNo);
				}
			}

			file.Close();
		}

		CColumnHandler column;
		column.Save("PLULIST", 0, 3, m_listboxPlu);
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
