/**********************************************************************/
#include "ListDataEmailAddressDlg.h"
/**********************************************************************/
#include "PosTrayEmailJobsDlg.h"
/**********************************************************************/

CPosTrayEmailJobsDlg::CPosTrayEmailJobsDlg( CPosTrayTask& batch, CStringArray& arrayEmailAddresses, CString& strEmailByReport, CWnd* pParent /*=NULL*/)
	: CDialog(CPosTrayEmailJobsDlg::IDD, pParent), 
	m_batch(batch), 
	m_arrayEmailAddresses(arrayEmailAddresses),
	m_strEmailByReport(strEmailByReport)
{
	//{{AFX_DATA_INIT(CPosTrayEmailJobsDlg)
	//}}AFX_DATA_INIT

	for (int n = 1; n <= POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		CString str = "";
		str.Format("%d", n);
		m_arrayComboTexts.AddText(str);
	}

	m_arrayComboTexts.AddText("None");
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayEmailJobsDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_EMAIL1, m_comboEmail[0]);
	DDX_Control(pDX, IDC_COMBO_EMAIL2, m_comboEmail[1]);
	DDX_Control(pDX, IDC_COMBO_EMAIL3, m_comboEmail[2]);
	DDX_Control(pDX, IDC_COMBO_EMAIL4, m_comboEmail[3]);
	DDX_Control(pDX, IDC_COMBO_EMAIL5, m_comboEmail[4]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayEmailJobsDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayEmailJobsDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_LIST,OnButtonList)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayEmailJobsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	FillEmailCombos();
	UpdateSelEmailCombos();

	m_listReports.SubclassDlgItem(IDC_LIST, this);
	m_listReports.InsertColumn(1, "Report", LVCFMT_LEFT, 320);
	m_listReports.InsertColumn(2, "Email", LVCFMT_LEFT, 100);
	m_listReports.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listReports.EnableVScroll(TRUE);

	m_batch.GetSelectedReportList(m_arrayReports);

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	CPosTrayTask::CreateEmailReportMap(m_strEmailByReport, arrayEmailByReport);
	
	for (int n = 0; n < m_arrayReports.GetSize(); n++)
	{
		int nEmailIdx = 1;

		int nPos = 0;
		CSortedIntByString info{};
		info.m_strItem = m_arrayReports[n].m_strReportID;
		if (arrayEmailByReport.Find(info, nPos) == TRUE)
		{
			arrayEmailByReport.GetAt(nPos, info);
			nEmailIdx = info.m_nItem;

			if ((nEmailIdx < 0) || (nEmailIdx > 5))
			{
				nEmailIdx = 1;
			}
		}

		AddReportToList(m_arrayReports[n].m_strReportName, nEmailIdx);
	}

	return TRUE;
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::AddReportToList(CString strReportName, int nEmailIdx)
{
	int nComboSelect = 0;
	CString strCurrent = "";

	switch (nEmailIdx)
	{
	case 0:
		nComboSelect = 5;
		strCurrent = "None";
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		nComboSelect = nEmailIdx - 1;
		strCurrent.Format("%d", nEmailIdx);
		break;

	default:
		return;
	}
	
	CellTypeInfo info;
	info.m_strComboTabs = "5,50";
	info.ResetForCombo(m_arrayComboTexts);
	info.m_nComboSelect = nComboSelect;
	m_arrayCellTypes.Add(info);

	CCSV csv;
	csv.Add(1); 
	csv.Add(strReportName);
	csv.Add(strCurrent);
	int nPos = m_listReports.GetItemCount();
	m_listReports.InsertItem(nPos, "");
	m_listReports.SetItem(nPos, 0, LVIF_TEXT, csv.GetString(1), 0, 0, 0, NULL);
	m_listReports.SetItem(nPos, 1, LVIF_TEXT, csv.GetString(2), 0, 0, 0, NULL);
	m_listReports.SetItemData(nPos, csv.GetInt(0));
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listReports.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

BOOL CPosTrayEmailJobsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == &m_listReports)
			{
				m_listReports.HandleReturn(1);
				pMsg->wParam = NULL;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

LRESULT CPosTrayEmailJobsDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
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
	m_listReports.GetWindowRect(&obWindowRect);

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

	return true;
}

/**********************************************************************/

LRESULT CPosTrayEmailJobsDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);

	int nCol = pCellType->m_nCol;
	int nRow = pCellType->m_nRow;

	if (nCol != 1)
	{
		pCellType->m_nType = 0;
		return 1;
	}

	if (nRow < 0 || nRow >= m_arrayCellTypes.GetSize())
	{
		pCellType->m_nType = 0;
		return 1;
	}

	pCellType->m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType->m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType->m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType->m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	pCellType->m_nComboSelect = m_arrayCellTypes[nRow].m_nComboSelect;

	return 1;
}

/**********************************************************************/

LRESULT CPosTrayEmailJobsDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_listReports.AllowListScroll(TRUE);
	m_listReports.SetEditCell(-1, -1);

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
			/*
			case CELL_TYPE_EDIT:
				switch (info.m_nSSEditType)
				{
				case SS_NUM:
				case SS_NUM_S:
				case SS_BLANK_NUM:
				case SS_BLANK_NUM_S:
				{
					int n = (strText == "") ? -1 : atoi(strText);

					if (n >= info.m_nMinInt && n <= info.m_nMaxInt)
					{
						m_arrayCellTypes[nRow].m_strCurrent = strText;
					}
					else
					{
						m_listReports.SetItem(nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL);
					}
				}
				break;

				case SS_NUM_DP:
				case SS_NUM_SDP:
				case SS_BLANK_NUM_DP:
				case SS_BLANK_NUM_SDP:
				{
					double d = (strText == "") ? -1.00 : atof(strText);

					if (d >= info.m_dMinDouble && d <= info.m_dMaxDouble)
					{
						m_arrayCellTypes[nRow].m_strCurrent = strText;
					}
					else
					{
						m_listReports.SetItem(nRow, 1, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL);
					}
				}
				break;
				}
				*/

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
						m_listReports.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nNewSel), 0, 0, 0, NULL);
					}
					else if ((nOldSel >= 0) && (nOldSel < csv.GetSize()))
					{
						m_arrayCellTypes[nRow].m_nComboSelect = nOldSel;
						m_listReports.SetItem(nRow, 1, LVIF_TEXT, csv.GetString(nOldSel), 0, 0, 0, NULL);
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

void CPosTrayEmailJobsDlg::UpdateSelEmailCombos()
{
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		CString strEmail = m_arrayEmailAddresses.GetAt(n);
		int nPos = DataManagerNonDb.EmailAddress.FindNameOrFirstAddress(strEmail);

		if (nPos != -1)
		{
			m_comboEmail[n].SetCurSel(nPos);
		}
		else
		{
			m_comboEmail[n].SetWindowText(strEmail);
		}
	}
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::FillEmailCombos()
{
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		m_comboEmail[n].ResetContent();
	}

	for (int nIndex = 0; nIndex < DataManagerNonDb.EmailAddress.GetSize(); nIndex++)
	{
		for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
		{
			m_comboEmail[n].AddString(DataManagerNonDb.EmailAddress.GetComboText(nIndex));
		}
	}
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::OnButtonList()
{
	CDataManagerInfo info;
	if (DataManagerNonDb.OpenEmailAddressBuffer(info) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CString strDummy = "";
	CListDataEmailAddressDlg dlg(strDummy, this);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManagerNonDb.WriteEmailAddressBuffer(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManagerNonDb.CloseEmailAddressBuffer(info);

	FillEmailCombos();
	UpdateSelEmailCombos();
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::OnButtonReset()
{
	if (Prompter.YesNo("Do you wish to use the task default email address for all reports") == IDYES)
	{
		for ( int n = 0; n < m_arrayCellTypes.GetSize(); n++ )
		{ 
			{
				m_arrayCellTypes[n].m_nComboSelect = 0;
				m_listReports.SetItem(n, 1, LVIF_TEXT, "1", 0, 0, 0, NULL);
			}
		}
	}
}

/**********************************************************************/

void CPosTrayEmailJobsDlg::OnOK()
{
	for (int n = 0; n < POSTRAY_MAX_EMAIL_NICKNAMES; n++)
	{
		CString strEmail = "";
		m_comboEmail[n].GetWindowText(strEmail);
		strEmail = strEmail.SpanExcluding("(");
		::TrimSpaces(strEmail, FALSE);
		m_arrayEmailAddresses.SetAt(n, strEmail);
	}

	CCSV csvEmailByReport{};

	for (int n = 0; n < m_listReports.GetItemCount(); n++)
	{
		CellTypeInfo info = m_arrayCellTypes[n];
		
		int nSel = info.m_nComboSelect;

		if ((nSel > 0) && (nSel <= POSTRAY_MAX_EMAIL_NICKNAMES))
		{
			int nEmailType = (nSel == POSTRAY_MAX_EMAIL_NICKNAMES) ? 0 : nSel + 1;
			csvEmailByReport.Add(m_arrayReports[n].m_strReportID);
			csvEmailByReport.Add(nEmailType);
		}
	}

	m_strEmailByReport = csvEmailByReport.GetLine();

	EndDialog(IDOK);
}

/**********************************************************************/
