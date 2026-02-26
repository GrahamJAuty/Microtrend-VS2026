//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRecordSetPluInfo.h"
//$$******************************************************************
#include "AccumulatorPropertySheet.h"
#include "ColumnHandler.h"
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "AccumulatorPropertyPage2.h"
//$$******************************************************************

CAccumulatorPropertyPage2::CAccumulatorPropertyPage2() : CSSPropertyPage(CAccumulatorPropertyPage2::IDD)
{
	m_pReportInfo = NULL;
	m_nLastDbNo = 0;
	m_nColumnCount = 2;
}

//$$******************************************************************

CAccumulatorPropertyPage2::~CAccumulatorPropertyPage2()
{
}

//$$******************************************************************

void CAccumulatorPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_UNKNOWN, m_checkUnknown);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAccumulatorPropertyPage2, CSSPropertyPage)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL,OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
END_MESSAGE_MAP()

//$$******************************************************************

void CAccumulatorPropertyPage2::SetPointer(const char* szLabel)
{
	m_strReportLabel = szLabel;
}

//$$******************************************************************

void CAccumulatorPropertyPage2::SetReportInfo(CReportInfoAccumulator* pInfo)
{
	m_pReportInfo = pInfo;

	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)
		{
			m_pReportInfo->SetNewPageFlag(csv.GetBool(0));
			m_pReportInfo->SetIncludeUnknownItemsFlag(csv.GetBool(1));
		}
	}
}

//$$******************************************************************

BOOL CAccumulatorPropertyPage2::OnInitDialog()
{
	m_listboxPlu.ClearRowHeightFlag();
	bool bNewPage = m_pReportInfo -> GetNewPageFlag();
	bool bUnknown = m_pReportInfo->GetIncludeUnknownItemsFlag();	

	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)
		{
			bNewPage = csv.GetBool(0);
			bUnknown = csv.GetBool(1);
		}
	}

	CSSPropertyPage::OnInitDialog();

	m_checkNewPage.SetCheck(bNewPage);
	m_checkUnknown.SetCheck(bUnknown);

	CWaitCursor cursor;

	Server.Read(Filenames.GetServerDataFilename());
	if (Server.GetEnablePluBonusFlag() == TRUE)
	{
		m_nColumnCount = 3;
	}

	int nTotalWidth = 0;

	CColumnHandler column;
	if (column.Read("PLULIST", m_nColumnCount) == TRUE)
	{
		nTotalWidth += column.GetWidth(0);
		nTotalWidth += column.GetWidth(1);
		nTotalWidth += column.GetWidth(2);
	}

	if ( (nTotalWidth < 300) || ( nTotalWidth > 330 ) )
	{
		column.SetWidth(0, 90);
		column.SetWidth(1, 190);
		column.SetWidth(2, 50);
	}

	m_listboxPlu.SubclassDlgItem(IDC_LIST_PLU, this);
	m_listboxPlu.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listboxPlu.InsertColumn(0, "PluNo", LVCFMT_LEFT, column.GetWidth(0));
	m_listboxPlu.InsertColumn(1, "Description", LVCFMT_LEFT, column.GetWidth(1));

	if (m_nColumnCount == 3)
	{
		m_listboxPlu.InsertColumn(2, "Points", LVCFMT_RIGHT, column.GetWidth(2));
	}

	FillPluList(m_pReportInfo->GetDbNo());
	m_nLastDbNo = m_pReportInfo->GetDbNo();
	
	return TRUE;
}

//$$******************************************************************

void CAccumulatorPropertyPage2::FillPluList(int nDbNo)
{
	m_listboxPlu.DeleteAllItems();
	m_listboxPlu.InsertItem(0, "");
	m_listboxPlu.SetItem(0, 0, LVIF_TEXT, szDISCOUNT_PLUNO, 0, 0, 0, NULL);
	m_listboxPlu.SetItem(0, 1, LVIF_TEXT, szDISCOUNT_ECRTEXT, 0, 0, 0, NULL);

	m_arrayPluNo.RemoveAll();

	CAccumulatorPlu PluInfo;
	PluInfo.m_nPluNo = 0;
	PluInfo.m_bDefaultSelect = FALSE;
	m_arrayPluNo.Add(PluInfo);

	CSQLRecordSetPluInfo RecordSet(NULL,
		RSParams_PluInfo_NormalByDbNo{ nDbNo });

	int nListPos = 1;

	CSQLRowPluInfo RowPluInfo;
	while (RecordSet.StepSelectAll(RowPluInfo) == TRUE)
	{
		if (RowPluInfo.IsBasePlu() == TRUE)
		{
			CString strBasePluNo = RowPluInfo.GetMicrotrendBasePluNo(TRUE);

			m_listboxPlu.InsertItem(nListPos, "");
			m_listboxPlu.SetItem(nListPos, 0, LVIF_TEXT, strBasePluNo, 0, 0, 0, NULL);
			m_listboxPlu.SetItem(nListPos, 1, LVIF_TEXT, RowPluInfo.GetDescription(), 0, 0, 0, NULL);

			if (m_nColumnCount == 3)
			{
				CString strPoints = "";
				strPoints.Format("%d", RowPluInfo.GetPoints());
				m_listboxPlu.SetItem(nListPos, 2, LVIF_TEXT, strPoints, 0, 0, 0, NULL);
			}

			nListPos++;

			CAccumulatorPlu PluInfo;
			PluInfo.m_nPluNo = _atoi64(strBasePluNo);
			PluInfo.m_bDefaultSelect = FALSE;
			m_arrayPluNo.Add(PluInfo);
		}
	}

	CReportConsolidationArray<CSortedInt64Item> arrayDefaults;
	CSortedInt64Item itemDefault;

	{
		CSSFile file;
		if (file.Open(Filenames.GetPluDatabaseListFilename(nDbNo), "rb") == TRUE)
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

	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		CAccumulatorPlu infoPlu = m_arrayPluNo.GetAt(i);

		itemDefault.m_nItem = infoPlu.m_nPluNo;

		int nPos = 0;
		if (arrayDefaults.Find(itemDefault, nPos) == TRUE)
		{
			m_listboxPlu.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_listboxPlu.SetSelectionMark(i);

			infoPlu.m_bDefaultSelect = TRUE;
			m_arrayPluNo.SetAt(i, infoPlu);
		}
	}
}

//$$******************************************************************

void CAccumulatorPropertyPage2::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_PLU)
	{
		m_listboxPlu.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

//$$******************************************************************

void CAccumulatorPropertyPage2::OnButtonAll()
{
	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		m_listboxPlu.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_listboxPlu.SetSelectionMark(i);
	}
}

//$$******************************************************************

void CAccumulatorPropertyPage2::OnButtonNone()
{
	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		m_listboxPlu.SetItemState(i, 0, LVIS_SELECTED);
	}
}

//$$******************************************************************

void CAccumulatorPropertyPage2::OnButtonReset()
{
	for (int i = 0; i < m_listboxPlu.GetItemCount(); i++)
	{
		CAccumulatorPlu infoPlu = m_arrayPluNo.GetAt(i);

		if (TRUE == infoPlu.m_bDefaultSelect)
		{
			m_listboxPlu.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_listboxPlu.SetSelectionMark(i);
		}
		else
		{
			m_listboxPlu.SetItemState(i, 0, LVIS_SELECTED);
		}
	}
}

//$$******************************************************************

BOOL CAccumulatorPropertyPage2::OnSetActive()
{
	int nNewDbNo = m_pReportInfo->GetDbNo();
	if ( nNewDbNo != m_nLastDbNo)
	{
		FillPluList(nNewDbNo);
		m_nLastDbNo = nNewDbNo;
	}

	return CSSPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CAccumulatorPropertyPage2::OnKillActive()
{
	bool bNewPage = IsTicked(m_checkNewPage);
	bool bUnknown = IsTicked(m_checkUnknown);

	if (UpdateData() == TRUE)
	{
		m_pReportInfo->SetNewPageFlag(bNewPage);
		m_pReportInfo->SetIncludeUnknownItemsFlag(bUnknown);

		CSSFile file;
		if (file.Open(Filenames.GetPluDatabaseListFilename(m_nLastDbNo), "wb") == TRUE)
		{
			for (int n = 0; n < m_listboxPlu.GetItemCount(); n++)
			{
				if (m_listboxPlu.GetItemState(n, LVIS_SELECTED) != 0)
				{
					CAccumulatorPlu infoPlu = m_arrayPluNo.GetAt(n);

					CString strPluNo = "";
					strPluNo.Format("%I64d", infoPlu.m_nPluNo);
					file.WriteLine(strPluNo);
				}
			}

			file.Close();
		}

		CCSV csv;
		csv.Add(bNewPage);
		csv.Add(bUnknown);

		CReportDefaultHandler repdefs;
		repdefs.Save(m_strReportLabel, &csv);

		CColumnHandler column;
		column.Save("PLULIST", 0, m_nColumnCount, m_listboxPlu);
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
