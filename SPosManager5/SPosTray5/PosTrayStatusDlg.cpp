/**********************************************************************/
#include "PosTrayOptions.h"
#include "PosTrayTask.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayStatusDlg.h"
/**********************************************************************/

CPosTrayStatusDlg::CPosTrayStatusDlg( CPosTrayStatusDlg** pParentToThis, CWnd* pParent) : CDialog(CPosTrayStatusDlg::IDD, pParent)
{
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	m_nSelectedTaskType = POSTRAY_TASKFILE_EPOS_TASK;
	m_nSelectedTaskIdx = 0;
	m_nTabPage = 0;
}

/**********************************************************************/

CPosTrayStatusDlg::~CPosTrayStatusDlg()
{
}

/**********************************************************************/

void CPosTrayStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_STATUS, m_TabStatus );
	DDX_Control(pDX, IDC_STATIC_TIME, m_staticTime );
	DDX_Control(pDX, IDC_STATIC_STATUS1, m_staticStatus1 );
	DDX_Control(pDX, IDC_STATIC_STATUS2, m_staticStatus2 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayStatusDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_STATUS, OnTcnSelchangeTabs)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_NETWORK, OnGetDispInfoListNetwork)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TYPE, OnGetDispInfoListType)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TASK, OnGetDispInfoListTask)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FNB, OnGetDispInfoListFNB)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CHARTWELLS, OnGetDispInfoListChartwells)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_S4LABOUR, OnGetDispInfoListS4Labour)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LIPA, OnGetDispInfoListLIPA)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_GENERAL, OnGetDispInfoListGeneral)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_BACKUP, OnGetDispInfoListBackup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TYPE, OnSelectType)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TASK, OnSelectTask)
END_MESSAGE_MAP()

/**********************************************************************/

void CPosTrayStatusDlg::PrepareTabPagesAndTaskTypes()
{
	m_arrayTabPages.RemoveAll();
	m_arrayTabPages.Add(POSTRAY_TABTYPE_NETWORK);

	if (PosTrayOptions.AreTasksEnabled() == TRUE)
	{
		m_arrayTabPages.Add(POSTRAY_TABTYPE_TASK);
	}

	if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
	{
		if (PosTrayOptions.GetFNBTaskFlag() == TRUE)
		{
			m_arrayTabPages.Add(POSTRAY_TABTYPE_FNB);
		}

		if (PosTrayOptions.GetChartwellsTaskFlag() == TRUE)
		{
			m_arrayTabPages.Add(POSTRAY_TABTYPE_CHARTWELLS);
		}

		if (PosTrayOptions.GetS4LabourTaskFlag() == TRUE)
		{
			m_arrayTabPages.Add(POSTRAY_TABTYPE_S4LABOUR);
		}

		if (DealerFlags.Get_LIPA_LJMU_ExportType() != 0)
		{
			m_arrayTabPages.Add(POSTRAY_TABTYPE_LIPA);
		}

		if (PosTrayOptions.GetManageBackupFlag() == TRUE)
		{
			m_arrayTabPages.Add(POSTRAY_TABTYPE_BACKUP);
		}
	}
	m_arrayTabPages.Add(POSTRAY_TABTYPE_GENERAL);

	m_arrayTaskTypes.RemoveAll();

	if (PosTrayOptions.GetPosTrayProcessTranFlag() == TRUE)
	{
		m_arrayTaskTypes.Add(POSTRAY_TASKFILE_PROCESSTRAN_TASK);
	}

	if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
	{
		if (PosTrayOptions.GetReportTaskEposFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_EPOS_TASK);
		}

		if (PosTrayOptions.GetReportTaskHistoryFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_HIST_TASK);
		}

		if (PosTrayOptions.GetReportTaskStockLevelFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_STKLEVEL_TASK);
		}

		if (PosTrayOptions.GetReportTaskTimeAttendFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_TIMEATTEND_TASK);
		}

		if (PosTrayOptions.GetExportTaskSalesFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_EXPORT_SALES_TASK);
		}

		if (PosTrayOptions.GetExportTaskPaymentFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK);
		}

		if (PosTrayOptions.GetExportTaskVoidFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_EXPORT_VOID_TASK);
		}

		if (PosTrayOptions.GetExportTaskDiscountFlag() == TRUE)
		{
			m_arrayTaskTypes.Add(POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK);
		}
	}
}
	
/**********************************************************************/

BOOL CPosTrayStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listNetwork.SubclassDlgItem ( IDC_LIST_NETWORK, this );
	m_listNetwork.InsertColumn ( 1, "Name", LVCFMT_LEFT, 200 );
	m_listNetwork.InsertColumn ( 2, "Version", LVCFMT_LEFT, 200 );
	m_listNetwork.InsertColumn ( 3, "Path", LVCFMT_LEFT, 350 );
	m_listNetwork.InsertColumn ( 4, "Trans", LVCFMT_RIGHT, 60 );

	m_listType.SubclassDlgItem ( IDC_LIST_TYPE, this );
	m_listType.InsertColumn ( 1, "Type", LVCFMT_LEFT, 160 );
	m_listType.SetItemCountEx( m_arrayTaskTypes.GetSize() );
	
	m_listTask.SubclassDlgItem ( IDC_LIST_TASK, this );
	m_listTask.InsertColumn ( 1, "Name", LVCFMT_LEFT, 200 );
	m_listTask.InsertColumn ( 2, "Status", LVCFMT_LEFT, 410 );

	m_listFNB.SubclassDlgItem ( IDC_LIST_FNB, this );
	m_listFNB.InsertColumn ( 1, "Db No", LVCFMT_LEFT, 50 );
	m_listFNB.InsertColumn ( 2, "Name", LVCFMT_LEFT, 280 );
	m_listFNB.InsertColumn ( 3, "Status", LVCFMT_LEFT, 480 );

	m_listChartwells.SubclassDlgItem(IDC_LIST_CHARTWELLS, this);
	m_listChartwells.InsertColumn(1, "Database Access", LVCFMT_LEFT, 330);
	m_listChartwells.InsertColumn(2, "Status", LVCFMT_LEFT, 480);

	m_listS4Labour.SubclassDlgItem ( IDC_LIST_S4LABOUR, this );
	m_listS4Labour.InsertColumn ( 1, "Loc No", LVCFMT_LEFT, 50 );
	m_listS4Labour.InsertColumn ( 2, "Name", LVCFMT_LEFT, 280 );
	m_listS4Labour.InsertColumn ( 3, "Status", LVCFMT_LEFT, 480 );

	m_listLIPA.SubclassDlgItem ( IDC_LIST_LIPA, this );
	m_listLIPA.InsertColumn ( 1, "Context", LVCFMT_LEFT, 100 );
	m_listLIPA.InsertColumn ( 2, "Status", LVCFMT_LEFT, 710 );

	m_listBackup.SubclassDlgItem(IDC_LIST_BACKUP, this);
	m_listBackup.InsertColumn(0, "Last Update", LVCFMT_LEFT, 200);
	m_listBackup.InsertColumn(1, "Status", LVCFMT_LEFT, 610);

	m_listGeneral.SubclassDlgItem ( IDC_LIST_GENERAL, this );
	m_listGeneral.InsertColumn ( 0, "", LVCFMT_LEFT, 20 );
	m_listGeneral.InsertColumn ( 1, "", LVCFMT_LEFT, 790 );

	m_listNetwork.SetItemCountEx(0);
	m_listType.SetItemCountEx(0);
	m_listTask.SetItemCountEx(0);
	m_listFNB.SetItemCountEx(0);
	m_listChartwells.SetItemCountEx(0);
	m_listS4Labour.SetItemCountEx(0);
	m_listLIPA.SetItemCountEx(0);
	m_listGeneral.SetItemCountEx(0);
	m_listBackup.SetItemCountEx(1);

	PrepareTabPagesAndTaskTypes();
	ResetTabPages();
	CenterWindow();

	m_staticTime.SetWindowText( "" );
	m_staticStatus1.SetWindowText( "" );
	m_staticStatus2.SetWindowText( "" );

	m_nTabPage = 0;
	SelectList();

	return FALSE;
}

/**********************************************************************/

void CPosTrayStatusDlg::ResetTabPages()
{
	m_TabStatus.DeleteAllItems();

	for (int n = 0; n < m_arrayTabPages.GetSize(); n++)
	{
		switch (m_arrayTabPages.GetAt(n))
		{
		case POSTRAY_TABTYPE_NETWORK:			m_TabStatus.InsertItem(n, "Networks");				break;
		case POSTRAY_TABTYPE_TASK:				m_TabStatus.InsertItem(n, "Tasks");					break;
		case POSTRAY_TABTYPE_FNB:				m_TabStatus.InsertItem(n, "FNB Export");			break;
		case POSTRAY_TABTYPE_CHARTWELLS:		m_TabStatus.InsertItem(n, "Chartwells Export");		break;
		case POSTRAY_TABTYPE_S4LABOUR:			m_TabStatus.InsertItem(n, "S4 Labour Export");		break;
		case POSTRAY_TABTYPE_LIPA:				m_TabStatus.InsertItem(n, "LIPA Export");			break;
		case POSTRAY_TABTYPE_BACKUP:			m_TabStatus.InsertItem(n, "System Backup");			break;
		case POSTRAY_TABTYPE_GENERAL:			m_TabStatus.InsertItem(n, "General");				break;
		}
	}

	m_TabStatus.SetCurSel(0);
	m_nTabPage = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	m_nTabPage = m_TabStatus.GetCurSel();
	if ((m_nTabPage < 0) || (m_nTabPage >= m_arrayTabPages.GetSize()))
	{
		m_nTabPage = 0;
	}

	SelectList();
}

/**********************************************************************/

void CPosTrayStatusDlg::ShowList(CSSListCtrlVirtual& list, int nListType, int nVisibleType)
{
	list.ShowWindow((nListType == nVisibleType) ? SW_SHOW : SW_HIDE);
}

/**********************************************************************/

void CPosTrayStatusDlg::SelectList()
{
	int nType = m_arrayTabPages.GetAt(m_nTabPage);
	ShowList(m_listNetwork, POSTRAY_TABTYPE_NETWORK, nType);
	ShowList(m_listType, POSTRAY_TABTYPE_TASK, nType);
	ShowList(m_listTask, POSTRAY_TABTYPE_TASK, nType);
	ShowList(m_listFNB, POSTRAY_TABTYPE_FNB, nType);
	ShowList(m_listChartwells, POSTRAY_TABTYPE_CHARTWELLS, nType);
	ShowList(m_listS4Labour, POSTRAY_TABTYPE_S4LABOUR, nType);
	ShowList(m_listLIPA, POSTRAY_TABTYPE_LIPA, nType);
	ShowList(m_listBackup, POSTRAY_TABTYPE_BACKUP, nType);
	ShowList(m_listGeneral, POSTRAY_TABTYPE_GENERAL, nType);

	if (POSTRAY_TABTYPE_TASK == nType)
	{
		int nSel = 0;
		bool bFoundType = FALSE;
		for (int n = 0; n < m_arrayTaskTypes.GetSize(); n++)
		{
			if (m_arrayTaskTypes.GetAt(n) == m_nSelectedTaskType)
			{
				nSel = n;
				bFoundType = TRUE;
				break;
			}
		}

		if (FALSE == bFoundType)
		{
			if (m_arrayTaskTypes.GetSize() > 0)
			{
				m_nSelectedTaskType = m_arrayTaskTypes.GetAt(0);
			}
			else
			{
				m_nSelectedTaskType = POSTRAY_TASKFILE_EPOS_TASK;
			}
		}

		SelectLineTaskType(nSel);
		SelectTaskType();

		if (TRUE == bFoundType)
		{
			SelectLineTask(m_nSelectedTaskIdx);
		}
		else
		{
			m_nSelectedTaskIdx = 0;
			SelectLineTask(0);
		}
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListNetwork(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineNetwork(nIndex);
		CCSV csv(m_strDisplayLineNetwork, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		CString strTaskType = "";
		if ((nIndex >= 0) && (nIndex < m_arrayTaskTypes.GetSize()))
		{
			switch (m_arrayTaskTypes.GetAt(nIndex))
			{
			case POSTRAY_TASKFILE_PROCESSTRAN_TASK:		strTaskType = "Process Transactions";		break;
			case POSTRAY_TASKFILE_TIMEATTEND_TASK:		strTaskType = "Time & Attendance Reports";	break;
			case POSTRAY_TASKFILE_EPOS_TASK:			strTaskType = "Epos Reports";				break;
			case POSTRAY_TASKFILE_HIST_TASK:			strTaskType = "Sales History Reports";		break;
			case POSTRAY_TASKFILE_STKLEVEL_TASK:		strTaskType = "Stock Level Reports";		break;
			case POSTRAY_TASKFILE_EXPORT_SALES_TASK:	strTaskType = "Export Sales";				break;
			case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:	strTaskType = "Export Payments";			break;
			case POSTRAY_TASKFILE_EXPORT_VOID_TASK:		strTaskType = "Export Voids";				break;
			case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:	strTaskType = "Export Discounts";			break;
			}
		}

		lstrcpy(pDispInfo->item.pszText, strTaskType);
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListTask(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineTask(nIndex);
		CCSV csv(m_strDisplayLineTask, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListFNB(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineFNB(nIndex);
		CCSV csv(m_strDisplayLineFNB, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListChartwells(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineChartwells(nIndex);
		CCSV csv(m_strDisplayLineChartwells, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListS4Labour(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineS4Labour(nIndex);
		CCSV csv(m_strDisplayLineS4Labour, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListLIPA(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineLIPA(nIndex);
		CCSV csv(m_strDisplayLineLIPA, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListBackup(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineBackup(nIndex);
		CCSV csv(m_strDisplayLineBackup, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayStatusDlg::OnGetDispInfoListGeneral(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineGeneral(nIndex);
		CCSV csv(m_strDisplayLineGeneral, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

CStringArray* CPosTrayStatusDlg::GetVisibleTaskArray()
{
	switch( m_nSelectedTaskType )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
		return &m_strArrayLinesTaskProcessTran;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		return &m_strArrayLinesTaskReportTimeAttend;

	case POSTRAY_TASKFILE_HIST_TASK:
		return &m_strArrayLinesTaskReportHist;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		return &m_strArrayLinesTaskReportStkLevel;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		return &m_strArrayLinesTaskExportSales;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		return &m_strArrayLinesTaskExportPayment;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		return &m_strArrayLinesTaskExportVoid;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		return &m_strArrayLinesTaskExportDiscount;

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		return &m_strArrayLinesTaskReportEpos;
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::OnSelectType(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectTaskType();
		m_nSelectedTaskIdx = 0;
		SelectLineTask(0);
    }
}

/**********************************************************************/

void CPosTrayStatusDlg::OnSelectTask(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		m_nSelectedTaskIdx = m_listTask.GetCurSel();
    }
}

/**********************************************************************/

void CPosTrayStatusDlg::SelectTaskType()
{
	int nSel = GetSafeSelectionTaskType();
	
	if ( nSel != -1 )
	{
		m_nSelectedTaskType = m_arrayTaskTypes.GetAt( nSel );
		m_listTask.SetItemCountEx( GetVisibleTaskArray() -> GetSize() );
		m_listTask.Invalidate();
	}
}

/**********************************************************************/

int CPosTrayStatusDlg::GetSafeSelectionTaskType()
{
	return GetSafeSelectionTaskType( 0, m_arrayTaskTypes.GetSize() - 1 );
}

/**********************************************************************/

int CPosTrayStatusDlg::GetSafeSelectionTaskType( int nMin, int nMax )
{
	int nSel = m_listType.GetCurSel();
	if ((nSel < nMin) || (nSel > nMax) || (nSel >= m_listType.GetItemCount()))
	{
		nSel = -1;
	}
	
	return nSel;
}

/**********************************************************************/

void CPosTrayStatusDlg::SelectLineTask( int nIndex )
{
	int nSize = m_listTask.GetItemCount();

	int nSel = 0;
	if ((nIndex >= 0) && (nIndex < nSize))
	{
		nSel = nIndex;
	}
	else
	{
		nSel = nSize - 1;
	}

	m_listTask.SetCurSel( nSel );
	m_listTask.Invalidate();
}

/**********************************************************************/

void CPosTrayStatusDlg::SelectLineTaskType(int nIndex)
{
	int nSize = m_listType.GetItemCount();

	int nSel = 0;
	if ((nIndex >= 0) && (nIndex < nSize))
	{
		nSel = nIndex;
	}
	else
	{
		nSel = nSize - 1;
	}

	m_listType.SetCurSel(nSel);
	m_listType.Invalidate();
}

/**********************************************************************/

int CPosTrayStatusDlg::GetActiveTab()
{
	return m_nTabPage;
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineNetwork(int nIndex)
{
	m_strDisplayLineNetwork = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesNetwork.GetSize()))
	{
		m_strDisplayLineNetwork = m_strArrayLinesNetwork.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineTask(int nIndex)
{
	m_strDisplayLineTask = "";
	CStringArray* pArray = GetVisibleTaskArray();
	if ((nIndex >= 0) && (nIndex < pArray->GetSize()))
	{
		m_strDisplayLineTask = pArray->GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineFNB(int nIndex)
{
	m_strDisplayLineFNB = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesFNB.GetSize()))
	{
		m_strDisplayLineFNB = m_strArrayLinesFNB.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineChartwells(int nIndex)
{
	m_strDisplayLineChartwells = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesChartwells.GetSize()))
	{
		m_strDisplayLineChartwells = m_strArrayLinesChartwells.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineS4Labour(int nIndex)
{
	m_strDisplayLineS4Labour = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesS4Labour.GetSize()))
	{
		m_strDisplayLineS4Labour = m_strArrayLinesS4Labour.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineLIPA(int nIndex)
{
	m_strDisplayLineLIPA = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesLIPA.GetSize()))
	{
		m_strDisplayLineLIPA = m_strArrayLinesLIPA.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineBackup(int nIndex)
{
	m_strDisplayLineBackup = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesBackup.GetSize()))
	{
		m_strDisplayLineBackup = m_strArrayLinesBackup.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::PrepareLineGeneral(int nIndex)
{
	m_strDisplayLineGeneral = "";
	if ((nIndex >= 0) && (nIndex < m_strArrayLinesGeneral.GetSize()))
	{
		m_strDisplayLineGeneral = m_strArrayLinesGeneral.GetAt(nIndex);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateNetworkList(CString& strStatus, CStringArray& arrayList, COleDateTime& time)
{
	m_strArrayLinesNetwork.RemoveAll();
	for (int n = 0; n < arrayList.GetSize(); n++)
	{
		m_strArrayLinesNetwork.Add(arrayList.GetAt(n));
	}

	m_listNetwork.SetItemCountEx(m_strArrayLinesNetwork.GetSize());
	m_listNetwork.Invalidate();

	UpdateStatusTime(time);
	m_staticStatus1.SetWindowText(strStatus);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateStatusLine(CString& strStatus, COleDateTime& time)
{
	UpdateStatusTime(time);
	m_staticStatus1.SetWindowText(strStatus);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskList(int nTaskType, CStringArray& arrayList, CStringArray& arrayDest)
{
	arrayDest.RemoveAll();
	for (int n = 0; n < arrayList.GetSize(); n++)
	{
		arrayDest.Add(arrayList.GetAt(n));
	}

	if (nTaskType == m_nSelectedTaskType)
	{
		m_listTask.SetItemCountEx(arrayDest.GetSize());
		m_listTask.Invalidate();
		SelectLineTask(m_nSelectedTaskIdx);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListProcessTran(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_PROCESSTRAN_TASK, arrayList, m_strArrayLinesTaskProcessTran);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListReportTimeAttend(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_TIMEATTEND_TASK, arrayList, m_strArrayLinesTaskReportTimeAttend);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListReportEpos(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_EPOS_TASK, arrayList, m_strArrayLinesTaskReportEpos);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListReportHist(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_HIST_TASK, arrayList, m_strArrayLinesTaskReportHist);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListReportStkLevel(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_STKLEVEL_TASK, arrayList, m_strArrayLinesTaskReportStkLevel);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListExportSales(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_EXPORT_SALES_TASK, arrayList, m_strArrayLinesTaskExportSales);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListExportPayment(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK, arrayList, m_strArrayLinesTaskExportPayment);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListExportVoid(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_EXPORT_VOID_TASK, arrayList, m_strArrayLinesTaskExportVoid);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateTaskListExportDiscount(CStringArray& arrayList)
{
	UpdateTaskList(POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK, arrayList, m_strArrayLinesTaskExportDiscount);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateNonTaskList(CString& strStatus, CStringArray& arrayList, COleDateTime& time, CSSListCtrlVirtual& DestList, CStringArray& DestArray, bool bMergeExistingList)
{
	if (arrayList.GetSize() > 0)
	{
		if (FALSE == bMergeExistingList)
		{
			DestArray.RemoveAll();
			for (int n = 0; n < arrayList.GetSize(); n++)
			{
				DestArray.Add(arrayList.GetAt(n));
			}
		}
		else
		{
			for ( int n = 0; n < arrayList.GetSize(); n++)
			{
				CString strLine = arrayList.GetAt(n);

				if ( n >= DestArray.GetSize() )
				{
					DestArray.Add(strLine);
				}
				else
				{
					if ( strLine != "" )
					{
						DestArray.SetAt(n, strLine);
					}
				}
			}
		}

		DestList.SetItemCountEx(DestArray.GetSize());
		DestList.Invalidate();
	}

	UpdateStatusTime(time);
	m_staticStatus1.SetWindowText(strStatus);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateFNBList(CString& strStatus, CStringArray& arrayList, COleDateTime& time)
{
	UpdateNonTaskList(strStatus, arrayList, time, m_listFNB, m_strArrayLinesFNB);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateChartwellsList(CString& strStatus, CStringArray& arrayList, COleDateTime& time)
{
	UpdateNonTaskList(strStatus, arrayList, time, m_listChartwells, m_strArrayLinesChartwells);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateS4LabourList( CString& strStatus, CStringArray& arrayList, COleDateTime& time )
{
	UpdateNonTaskList(strStatus, arrayList, time, m_listS4Labour, m_strArrayLinesS4Labour);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateLIPAList( CString& strStatus, CStringArray& arrayList, COleDateTime& time )
{
	UpdateNonTaskList(strStatus, arrayList, time, m_listLIPA, m_strArrayLinesLIPA);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateBackupList(CString& strStatus, CStringArray& arrayList, COleDateTime& time)
{
	UpdateNonTaskList(strStatus, arrayList, time, m_listBackup, m_strArrayLinesBackup, TRUE);
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateGeneralList(CString& strStatus, CStringArray& arrayList, COleDateTime& time)
{
	m_strArrayLinesGeneral.RemoveAll();
	for (int n = 0; n < arrayList.GetSize(); n++)
	{
		m_strArrayLinesGeneral.Add(arrayList.GetAt(n));
	}

	m_listGeneral.SetItemCountEx(m_strArrayLinesGeneral.GetSize());
	m_listGeneral.Invalidate();

	UpdateStatusTime(time);
	m_staticStatus1.SetWindowText(strStatus);

	CArray<int, int> arrayTabPages;
	for (int n = 0; n < m_arrayTabPages.GetSize(); n++)
	{
		arrayTabPages.Add(m_arrayTabPages.GetAt(n));
	}

	CArray<int, int> arrayTaskTypes;
	for (int n = 0; n < m_arrayTaskTypes.GetSize(); n++)
	{
		arrayTaskTypes.Add(m_arrayTaskTypes.GetAt(n));
	}

	PrepareTabPagesAndTaskTypes();

	m_listType.SetItemCountEx(m_arrayTaskTypes.GetSize());

	bool bChangedSettings = FALSE;

	if (arrayTabPages.GetSize() != m_arrayTabPages.GetSize())
	{
		bChangedSettings = TRUE;
	}

	if (arrayTaskTypes.GetSize() != m_arrayTaskTypes.GetSize())
	{
		bChangedSettings = TRUE;
	}

	if ( FALSE == bChangedSettings )
	{
		for ( int n = 0; n < arrayTabPages.GetSize(); n++ )
		{
			if ( arrayTabPages.GetAt(n) != m_arrayTabPages.GetAt(n) )
			{
				bChangedSettings = TRUE;
				break;
			}
		}
	}

	if ( FALSE == bChangedSettings )
	{
		for ( int n = 0; n < arrayTaskTypes.GetSize(); n++ )
		{
			if ( arrayTaskTypes.GetAt(n) != m_arrayTaskTypes.GetAt(n) )
			{
				bChangedSettings = TRUE;
				break;
			}
		}
	}

	if ( TRUE == bChangedSettings )
	{	
		ResetTabPages();
		SelectList();
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::UpdateStatusTime(COleDateTime& time)
{
	CString strTime = "";
	strTime.Format("Status update at %2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d",
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond(),
		time.GetDay(),
		time.GetMonth(),
		time.GetYear());

	m_staticTime.SetWindowText(strTime);
}

/**********************************************************************/

void CPosTrayStatusDlg::DisplayCashRSPStatus(const char* sz)
{
	if (::IsWindow(m_staticStatus2.m_hWnd))
	{
		m_staticStatus2.SetWindowText(sz);
	}
}

/**********************************************************************/

void CPosTrayStatusDlg::OnOK()
{
	DestroyWindow();
}

/**********************************************************************/

void CPosTrayStatusDlg::OnCancel()
{
	DestroyWindow();
}

/**********************************************************************/

void CPosTrayStatusDlg::PostNcDestroy() 
{
	*m_pParentToThis = NULL;
	delete this;
}

/**********************************************************************/
