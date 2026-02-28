/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRepositoryScheme.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "DefEditDlg.h"
#include "GroupNoDlg.h"
#include "ReportCSVFile.h"
/**********************************************************************/
#include "ListDataSchemeNamesDlg.h"
/**********************************************************************/

CSingleListContentHelpersSchemeNames::CSingleListContentHelpersSchemeNames(CReportConsolidationArray<CSortedStringByInt>& arrayNames) : CSingleListContentHelpers(),
	m_arrayNames(arrayNames)
{
}

/**********************************************************************/

bool CSingleListContentHelpersSchemeNames::PrepareLine(int nIndex, CString& strLine)
{
	if ((nIndex >= 0) && (nIndex < m_arrayNames.GetSize()))
	{ 
		CSortedStringByInt item;
		m_arrayNames.GetAt(nIndex, item);

		strLine.Format( "S%3.3d\t%s",
			item.m_nItem, 
			(const char*) item.m_strItem);
	}

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersSchemeNames::DeleteDatabaseEntry( int nIndex )
{
	if ((nIndex >= 0) && (nIndex < m_arrayNames.GetSize()))
	{
		m_arrayNames.RemoveAt(nIndex);
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSchemeNames::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersSchemeNames::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn(0, "Scheme", LVCFMT_LEFT, 80);
	list.InsertColumn(1, "Name", LVCFMT_LEFT, 400);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSchemeNamesDlg::CListDataSchemeNamesDlg( CWnd* pParent) : CListDataDlg(pParent)
{
	m_strTitle = "Edit Loyalty Scheme Names";

	CSQLRepositoryScheme PrepStatScheme;
	PrepStatScheme.GetSchemeNameList(m_arrayNames, -1, FALSE, NULL);

	m_pContentHelpers = new CSingleListContentHelpersSchemeNames(m_arrayNames);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);

	m_pDisplayHelpers->SetTopSingleText("scheme name");
	m_pDisplayHelpers->SetTopPluralText("scheme names");
	m_pDisplayHelpers->SetBottomSingleText("scheme name");
	m_pDisplayHelpers->SetBottomPluralText("scheme names");
}

/**********************************************************************/

CListDataSchemeNamesDlg::~CListDataSchemeNamesDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSchemeNamesDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataSchemeNamesDlg::SetButtonTexts()
{
	m_button1.SetWindowText("Add");
	m_button2.SetWindowText("Edit");
	m_button3.SetWindowText("Delete");
	m_button4.ShowWindow(SW_HIDE);
	m_button5.ShowWindow(SW_HIDE);
	m_button6.ShowWindow(SW_HIDE);

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 47);
	MoveControl(&m_button3, 368, 87);
	MoveControl(&m_button4, 368, 127);
	MoveControl(&m_button5, 368, 187);
	MoveControl(&m_button6, 368, 187);
}

/**********************************************************************/

void CListDataSchemeNamesDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSchemeNamesDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataSchemeNamesDlg::HandleButton1()
{
	int nNewSchemeNo = 0;
	for (int n = 0; n < m_arrayNames.GetSize(); n++)
	{
		CSortedStringByInt item;
		m_arrayNames.GetAt(n, item);

		if (item.m_nItem > n + 1)
		{
			nNewSchemeNo = n + 1;
			break;
		}
	}

	if (0 == nNewSchemeNo)
	{
		nNewSchemeNo = m_arrayNames.GetSize() + 1;
	}

	if ((nNewSchemeNo < 1) || (nNewSchemeNo > Scheme::SchemeNo.Max))
	{
		Prompter.Error("No further scheme numbers are available.");
		return;
	}

	CGroupNoDlg dlg(TRUE, nNewSchemeNo, this);

	if (dlg.DoModal() == IDOK)
	{
		CSortedStringByInt item;
		item.m_nItem = dlg.m_nGroupNo;
		item.m_strItem.Format("Scheme %d", dlg.m_nGroupNo);

		int nPos = 0;
		if (m_arrayNames.Find(item, nPos) == FALSE)
		{
			m_arrayNames.InsertAt(nPos, item);
			m_listData.SetItemCountEx(GetArraySize());
			SelectLine(nPos);
			m_WarnCancelAfterChange.SetFlag();
			HandleButton2();
		}
		else
		{
			SelectLine(nPos);

			CString strMsg = "";
			strMsg.Format("Scheme %d already exists.",
				dlg.m_nGroupNo);	

			Prompter.Error(strMsg);
		}
	}
}

/**********************************************************************/

void CListDataSchemeNamesDlg::HandleButton2()
{
	int nSel = GetSafeSelection();

	if ((nSel >= 0) && (nSel < m_arrayNames.GetSize()))
	{
		CSortedStringByInt item;
		m_arrayNames.GetAt(nSel, item);

		CDefEditDlg dlg("Edit Scheme Name", item.m_strItem, Scheme::SchemeName.Max, "Scheme name", this);

		if (dlg.DoModal() == IDOK)
		{
			CString strName = dlg.m_strName;
			SolutionGlobalFunctions::TrimSpacesFromString(strName, FALSE);

			if (strName != item.m_strItem)
			{
				item.m_strItem = strName;
				m_arrayNames.SetAt(nSel, item);
				SelectLine(nSel);
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

void CListDataSchemeNamesDlg::HandleButton3()
{
	m_pDisplayHelpers->HandleDelete(this);
	m_WarnCancelAfterChange.SetFlag(m_pContentHelpers->GetDeletionEffectFlag());
}

/**********************************************************************/

bool CListDataSchemeNamesDlg::HandleOK()
{
	CSQLRepositoryScheme PrepStatScheme;
	PrepStatScheme.SaveSchemeNameList(m_arrayNames);
	return TRUE;
}

/**********************************************************************/

bool CListDataSchemeNamesDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

