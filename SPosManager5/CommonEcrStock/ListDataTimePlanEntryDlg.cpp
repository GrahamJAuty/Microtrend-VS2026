/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "DefEditTimePlanDlg.h"
#include "NewRecordNumberDlg.h"
#include "NodeTypes.h"
#include "Password.h"
/**********************************************************************/
#include "ListDataTimePlanEntryDlg.h"
/**********************************************************************/

CSingleListContentHelpersTimePlanEntry::CSingleListContentHelpersTimePlanEntry(CTimePlanEntriesCSVArray* pTimePlanEntryArray, int nPlanNo)
{
	m_pTimePlanEntryArray = pTimePlanEntryArray;
	m_nPlanNo = nPlanNo;
}

/**********************************************************************/

int CSingleListContentHelpersTimePlanEntry::GetArraySize()
{
	int nStart = 0, nEnd = 0;
	m_pTimePlanEntryArray -> GetPlanRange( m_nPlanNo, nStart, nEnd );
	return ( nEnd - nStart ) + 1;
}

/**********************************************************************/

bool CSingleListContentHelpersTimePlanEntry::PrepareLine(int nIndex, CString& strLine)
{
	strLine = "";

	bool bResult = FALSE;
	if ((nIndex >= 0) && (nIndex < GetArraySize()))
	{
		int nStart, nEnd;
		m_pTimePlanEntryArray->GetPlanRange(m_nPlanNo, nStart, nEnd);

		if (nIndex < 0 || nIndex > nEnd - nStart)
		{
			return FALSE;
		}

		CTimePlanEntriesCSVRecord Entry;
		m_pTimePlanEntryArray->GetAt(nStart + nIndex, Entry);

		CCSV csv('\t');
		csv.Add(Entry.GetEntryNo());
		csv.Add(Entry.GetName());

		strLine = csv.GetLine();

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTimePlanEntry::DeleteDatabaseEntry(int nIndex)
{
	int nStart, nEnd;
	m_pTimePlanEntryArray->GetPlanRange(m_nPlanNo, nStart, nEnd);

	if ((nIndex >= 0) && (nIndex <= nEnd - nStart))
	{
		m_pTimePlanEntryArray->RemoveAt(nStart + nIndex);
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersTimePlanEntry::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "Entry", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Description", LVCFMT_LEFT, 430);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTimePlanEntryDlg::CListDataTimePlanEntryDlg(bool bTemplateMode, int nPlanNo, CWnd* pParent) : CListDataDlg(pParent)
{
	m_bTemplateMode = bTemplateMode;
	if (FALSE == m_bTemplateMode)
	{
		m_pTimePlanEntryArray = &DataManager.TimePlanEntry;
		m_strEntityTypeCaps = "Time Plan Entry";
		m_strEntityTypeNoCaps = "time plan entry";
	}
	else
	{
		m_pTimePlanEntryArray = &DataManager.TimePlanTemplateEntry;
		m_strEntityTypeCaps = "Template Entry";
		m_strEntityTypeNoCaps = "template entry";
	}

	m_nPlanNo = nPlanNo;
	m_pContentHelpers = new CSingleListContentHelpersTimePlanEntry(m_pTimePlanEntryArray, nPlanNo);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);

	m_pDisplayHelpers->SetTopSingleText(m_strEntityTypeNoCaps + " entry");
	m_pDisplayHelpers->SetTopPluralText(m_strEntityTypeNoCaps + " entries");
	m_pDisplayHelpers->SetBottomSingleText("entry");
	m_pDisplayHelpers->SetBottomPluralText("entries");
}

/**********************************************************************/

CListDataTimePlanEntryDlg::~CListDataTimePlanEntryDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::SetTitle()
{
	int nPlanIdx = 0;
	CString strPlanName = "";
	if (DataManager.TimePlan.FindPlanByNumber(m_nPlanNo, nPlanIdx) == TRUE)
	{
		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt(nPlanIdx, Plan);
		strPlanName = Plan.GetDisplayName();
	}
	else
	{
		strPlanName = "Unknown";
	}

	CString strTitle = "";
	strTitle.Format("Edit %s (%d, %s)",
		(const char*) m_strEntityTypeCaps,
		m_nPlanNo,
		(const char*)strPlanName);

	SetWindowText(strTitle);
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Plan\n&Name" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::HandleButton1()
{
	int nEntryNo = m_pTimePlanEntryArray->FindFirstFreeNumber(m_nPlanNo);

	if (nEntryNo < 1)
	{
		Prompter.Error("All available entry numbers are already in use.");
		return;
	}

	CNewRecordNumberDlg dlg(m_bTemplateMode ? NODE_TIMEPLAN_TEMPLATE_ENTRY : NODE_TIMEPLAN_ENTRY, this);
	dlg.m_nNumber = nEntryNo;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	nEntryNo = dlg.m_nNumber;

	CTimePlanEntriesCSVRecord Entry;
	Entry.SetPlanNo(m_nPlanNo);
	Entry.SetEntryNo(nEntryNo);

	int nEntryIdx = 0;
	if (m_pTimePlanEntryArray->FindKey(Entry, nEntryIdx) == TRUE)
	{
		Prompter.Error("The selected entry number is already in use.");
		return;
	}

	Entry.SetName("");
	m_pTimePlanEntryArray->InsertAt(nEntryIdx, Entry);

	m_listData.SetItemCountEx(GetArraySize());

	int nStart = 0, nEnd = 0;
	m_pTimePlanEntryArray->GetPlanRange(m_nPlanNo, nStart, nEnd);

	SelectLine(nEntryIdx - nStart);

	CDefEditTimePlanDlg dlgEdit(m_bTemplateMode, nEntryIdx, nStart, nEnd, TRUE, m_WarnCancelAfterChange, this);
	dlgEdit.SetListDataDlg(this);
	dlgEdit.DoModal();

	SelectLine(dlgEdit.GetIndex() - nStart);

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	int nStart = 0, nEnd = 0;
	m_pTimePlanEntryArray->GetPlanRange( m_nPlanNo, nStart, nEnd );
		
	CDefEditTimePlanDlg dlg( m_bTemplateMode, nStart + nIndex, nStart, nEnd, TRUE, m_WarnCancelAfterChange, this );
	dlg.SetListDataDlg( this );
	dlg.DoModal();

	SelectLine ( dlg.GetIndex() - nStart );
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataTimePlanEntryDlg::HandleButton4()
{
	int nPlanIdx = 0;
	if (DataManager.TimePlan.FindPlanByNumber(m_nPlanNo, nPlanIdx) == FALSE)
	{
		return;
	}

	CTimePlansCSVRecord Plan;
	DataManager.TimePlan.GetAt(nPlanIdx, Plan);

	CDefEditDlg dlg( "Time Plan Name", Plan.GetPlanName(), TimePlans::PlanName.Max, "Name", this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPlanName = dlg.m_strName;
		::TrimSpaces(strPlanName, FALSE);

		if (strPlanName == "")
		{
			strPlanName.Format("%s %d",
				(const char*) m_strEntityTypeCaps,
				m_nPlanNo);
		}

		Plan.SetPlanName(strPlanName);
		DataManager.TimePlan.SetAt(nPlanIdx, Plan);
		SetTitle();

		m_WarnCancelAfterChange.SetFlag();
	}
}
		
/**********************************************************************/