/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DefEditDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "ListDataTimePlanEntryDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "ReportDataTableTimePlanDlg.h"
#include "RepSpawn.h"
#include "SelectMultipleLocationDlg.h"
#include "SelectLocInfo.h"
/**********************************************************************/
#include "ListDataTimePlanDlg.h"
/**********************************************************************/

CSingleListContentHelpersTimePlan::CSingleListContentHelpersTimePlan()
{
}

/**********************************************************************/

int CSingleListContentHelpersTimePlan::GetArraySize()
{
	return DataManager.TimePlan.GetSortArraySize();
}

/**********************************************************************/

bool CSingleListContentHelpersTimePlan::PrepareLine(int nIndex, CString& strLine)
{
	strLine = "";

	bool bResult = FALSE;
	if ((nIndex >= 0) && (nIndex < DataManager.TimePlan.GetSortArraySize()))
	{
		int nArrayIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIndex);

		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt(nArrayIdx, Plan);

		CCSV csv('\t');
		csv.Add(Plan.GetPlanNo());
		csv.Add(Plan.GetPlanName());

		strLine = csv.GetLine();

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTimePlan::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.TimePlan.GetSortArraySize() ) )
	{
		CSortedIntItem item;
		item.m_nItem = DataManager.TimePlan.GetArrayIdxFromSortIdx( nIndex );
		m_arrayDeletionList.Consolidate( item );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

int CSingleListContentHelpersTimePlan::GetDeletionListIndex( int n )
{
	int nIndex = 0;

	if ( ( n >= 0 ) && ( n <= m_arrayDeletionList.GetSize() ) )
	{
		CSortedIntItem item;
		m_arrayDeletionList.GetAt(n, item );
		nIndex = item.m_nItem;
	}

	return nIndex;
}

/**********************************************************************/

void CSingleListContentHelpersTimePlan::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "No", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Name", LVCFMT_LEFT, 410);
	list.InsertColumn(2, "", LVCFMT_LEFT, 20);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTimePlanDlg::CListDataTimePlanDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	DataManager.TimePlan.SetSortMode( TIMEPLAN_SORT_NUMBER );

	m_pContentHelpers = new CSingleListContentHelpersTimePlan();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "time plan" );
	m_pDisplayHelpers -> SetTopPluralText( "time plans" );
	m_pDisplayHelpers -> SetBottomSingleText( "plan" );
	m_pDisplayHelpers -> SetBottomPluralText( "plans" );

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Time Plan by Name";
	m_nSearchColumn = 2;
}

/**********************************************************************/

CListDataTimePlanDlg::~CListDataTimePlanDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTimePlanDlg::SetTitle()
{
	CString strTitle = "";
	strTitle += "Time Plan Database ";	
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), DealerFlags.GetGlobalPromotionFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataTimePlanDlg::SetButtonTexts()
{
	m_button1.SetWindowText("&Add");
	m_button2.SetWindowText("&Edit");
	m_button3.SetWindowText("&Copy");
	m_button4.SetWindowText("&Delete");
	m_button5.SetWindowText("&Print");
	m_button6.SetWindowText("Do&wnload");

	if (PasswordArray.GetEnable(PasswordTicks::CommsDownloads) == FALSE)
	{
		m_button6.EnableWindow(FALSE);
		m_button6.ShowWindow(SW_HIDE);
	}

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 42);
	MoveControl(&m_button3, 368, 77);
	MoveControl(&m_button4, 368, 112);
	MoveControl(&m_button5, 368, 167);
	MoveControl(&m_button6, 368, 202);
}

/**********************************************************************/

void CListDataTimePlanDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleButton1()
{
	int nPlanNo = DataManager.TimePlan.FindFirstFreeNumber();

	if (nPlanNo < 1)
	{
		Prompter.Error( "All available time plans are already in use.");
		return;
	}

	CNewRecordNumberDlg dlg(NODE_TIMEPLAN, this);
	dlg.m_nNumber = nPlanNo;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int nPlanIdx = 0;
	nPlanNo = dlg.m_nNumber;
	if (DataManager.TimePlan.FindPlanByNumber(nPlanNo, nPlanIdx))
	{
		Prompter.Error("The selected time plan is already in use.");
		return;
	}

	CTimePlansCSVRecord Plan;
	Plan.SetPlanNo(nPlanNo);
	Plan.SetPlanName("");
	DataManager.TimePlan.InsertAt(nPlanIdx, Plan);

	/*
	CListDataTimePlanEntryDlg dlgEdit(FALSE, nPlanNo, this);
	dlgEdit.DoModal();
	*/

	DataManager.TimePlan.RefreshSort();
	m_listData.SetItemCountEx(GetArraySize());
	SelectLine(DataManager.TimePlan.GetSortIdxFromArrayIdx(nPlanIdx));

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleButton2()
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	CTimePlansCSVRecord Plan;
	int nArrayIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIndex);
	DataManager.TimePlan.GetAt(nArrayIdx, Plan);
	int nPlanNo = Plan.GetPlanNo();

	bool bTemplate = FALSE;
	if (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE)
	{
		CTimePlanTypeSelectDlg dlgType(this);
		if (dlgType.DoModal() != IDOK)
		{
			return;
		}
		bTemplate = (2 == dlgType.m_nType);
	}

	CListDataTimePlanEntryDlg dlgEdit(bTemplate, nPlanNo, this);
	if (dlgEdit.DoModal() == IDOK)
	{
		m_WarnCancelAfterChange.SetFlag();
	}

	DataManager.TimePlan.RefreshSort();
	SelectLine(DataManager.TimePlan.GetSortIdxFromArrayIdx(nArrayIdx));
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleButton3()
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	int nDestPlanNo = DataManager.TimePlan.FindFirstFreeNumber();

	if (nDestPlanNo < 1)
	{
		Prompter.Error("All available time plans are already in use.");
		return;
	}

	int nSourcePlanNo = 0;
	{
		int nArrayIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIndex);

		CTimePlansCSVRecord SourcePlan;
		DataManager.TimePlan.GetAt(nArrayIdx, SourcePlan);
		nSourcePlanNo = SourcePlan.GetPlanNo();

		CNewRecordNumberDlg dlg(NODE_TIMEPLAN, this);
		dlg.m_nNumber = nDestPlanNo;
		dlg.SetCopySource(nSourcePlanNo);
		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		nDestPlanNo = dlg.m_nNumber;
	}

	int nPlanIdx = 0;
	if (DataManager.TimePlan.FindPlanByNumber(nDestPlanNo, nPlanIdx))
	{
		Prompter.Error("The selected time plan is already in use.");
		return;
	}

	{
		CString strNewPlanName = "";
		strNewPlanName.Format("Copy of Time Plan %d", 
			nSourcePlanNo);

		CTimePlansCSVRecord Plan;
		Plan.SetPlanNo(nDestPlanNo);
		Plan.SetPlanName(strNewPlanName);
		DataManager.TimePlan.InsertAt(nPlanIdx, Plan);
	}

	// Copy time plan entries
	CopyTimePlanEntries(FALSE, nSourcePlanNo, nDestPlanNo);
	CopyTimePlanEntries(TRUE, nSourcePlanNo, nDestPlanNo);

	DataManager.TimePlan.RefreshSort();
	m_listData.SetItemCountEx(GetArraySize());
	SelectLine(DataManager.TimePlan.GetSortIdxFromArrayIdx(nPlanIdx));

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTimePlanDlg::CopyTimePlanEntries(bool bTemplate, int nSourcePlanNo, int nDestPlanNo)
{
	CTimePlanEntriesCSVArray* pEntryArray = bTemplate ? &DataManager.TimePlanTemplateEntry : &DataManager.TimePlanEntry;

	pEntryArray -> PurgePlan(nDestPlanNo);

	int nSourceStart = 0, nSourceEnd = 0;
	pEntryArray->GetPlanRange(nSourcePlanNo, nSourceStart, nSourceEnd);

	int nDestPos = 0, nDummy = 0;
	pEntryArray->GetPlanRange(nDestPlanNo, nDestPos, nDummy);

	for (int nEntryIdx = nSourceStart; nEntryIdx <= nSourceEnd; nEntryIdx++)
	{
		CTimePlanEntriesCSVRecord Entry;
		pEntryArray->GetAt(nEntryIdx, Entry);

		Entry.SetPlanNo(nDestPlanNo);
		pEntryArray->InsertAt(nDestPos, Entry);

		if (nDestPos <= nEntryIdx)
		{
			nEntryIdx++;
			nSourceEnd++;
		}

		nDestPos++;
	}
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleButton4()
{
	CSingleListContentHelpersTimePlan* pContentHelpers = (CSingleListContentHelpersTimePlan*)m_pContentHelpers;

	pContentHelpers->ClearDeletionList();
	m_pDisplayHelpers->HandleDelete(this);
	m_WarnCancelAfterChange.SetFlag(m_pContentHelpers->GetDeletionEffectFlag());

	if (pContentHelpers->GetDeletionListSize() != 0)
	{
		for (int n = pContentHelpers->GetDeletionListSize() - 1; n >= 0; n--)
		{
			int nArrayIdx = pContentHelpers->GetDeletionListIndex(n);

			CTimePlansCSVRecord Plan;
			DataManager.TimePlan.GetAt(nArrayIdx, Plan);
			DataManager.TimePlanEntry.PurgePlan(Plan.GetPlanNo());
			DataManager.TimePlanTemplateEntry.PurgePlan(Plan.GetPlanNo());
			DataManager.TimePlan.RemoveAt(nArrayIdx);
		}

		DataManager.TimePlan.RefreshSort();
		m_listData.SetItemCountEx(GetArraySize());
		SelectLine(m_pDisplayHelpers->GetFirstDeletedItem());
	}
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleButton5()
{
	if (DataManager.TimePlan.GetSize() == 0)
	{
		Prompter.Error("You must create some time plans\nbefore you can print this report.");
		return;
	}
	else
	{
		CReportDataTableTimePlanDlg dlg(this);
		dlg.DoModal();
	}
}
		
/**********************************************************************/

void CListDataTimePlanDlg::HandleButton6()
{
	int	nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int, int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray(nDbIdx, arrayLocIdx, bBlocked);

	if (arrayLocIdx.GetSize() == 0)
	{
		if (FALSE == bBlocked)
		{
			Prompter.Error("There are no locations available for\ndownload on this database.");
		}
		else
		{
			Prompter.Error("You are not authorised to download files\nfrom this database.");
		}

		return;
	}

	HandleDownload(arrayLocIdx);
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleDownload(CArray<int, int>& arrayLocIdx)
{
	if (CSelectMultipleLocationDlg::FilterLocListForDownload(arrayLocIdx, "Download Time Plan", this) == TRUE)
	{
		CEposTaskDatabase TaskDatabase;

		if (EcrmanOptions.GetFeaturesTimePlanDownloadFlag() == FALSE)
		{
			TaskDatabase.CreateSimpleTask(DN_TPLAN, arrayLocIdx);
		}
		else
		{
			CWordArray arrayJobs;
			arrayJobs.Add(DN_MIX);
			arrayJobs.Add(DN_TPLAN);
			TaskDatabase.CreateSimpleTask(arrayJobs, arrayLocIdx);
		}

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader(TaskHeader);

			CString strTitle;
			TaskHeader.SetName("Download Time Plan");
			strTitle.Format("Epos Import Date (%s)", TaskHeader.GetName());

			CDownloadDateDlg dlgDate(strTitle, this);
			if (dlgDate.RunIfNeeded() == FALSE)
			{
				return;
			}

			if (dlgDate.m_bImmediate == TRUE)
			{
				TaskHeader.SetImportType(IMPORT_IMMEDIATE);
			}
			else
			{
				TaskHeader.SetImportType(IMPORT_SPECIFIED);
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader(TaskHeader);
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms(TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this);
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/

void CListDataTimePlanDlg::HandleClickHeader(int nColumn)
{
	if (m_nSearchColumn == nColumn)
	{
		if ((nColumn >= 0) && (GetArraySize() > 1))
		{
			if (m_pSearchDlg == NULL)
			{
				m_pSearchDlg = new CListDataSearchDlg(&m_pSearchDlg, m_strSearchPrompt, this);
				m_pSearchDlg->Create(IDD_DEF_EDIT, this);
			}

			m_pSearchDlg->PostMessage(WM_SETFOCUS);
			m_pSearchDlg->LoadSettings(m_bSearchPrevious, m_strSearchText);
		}
	}
	else if (0 == nColumn)
	{
		if (DataManager.TimePlan.GetSortMode() != TIMEPLAN_SORT_NUMBER)
		{
			DataManager.TimePlan.SetSortMode(TIMEPLAN_SORT_NUMBER);
			m_listData.Invalidate();
			SelectLine(0);
		}
	}
	else if (1 == nColumn)
	{
		if (DataManager.TimePlan.GetSortMode() == TIMEPLAN_SORT_NAME_UP)
		{
			DataManager.TimePlan.SetSortMode(TIMEPLAN_SORT_NAME_DOWN);
		}
		else
		{
			DataManager.TimePlan.SetSortMode(TIMEPLAN_SORT_NAME_UP);
		}

		m_listData.Invalidate();
		SelectLine(0);
	}
}

/**********************************************************************/

bool CListDataTimePlanDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CTimePlansCSVRecord TP{};
	int nArrayIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIndex);
	DataManager.TimePlan.GetAt(nArrayIdx, TP);

	CString strText = TP.GetDisplayName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
