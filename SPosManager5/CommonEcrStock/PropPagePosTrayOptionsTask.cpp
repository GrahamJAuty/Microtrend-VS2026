/**********************************************************************/
#include "DateRangeHelpers.h"
#include "EposReportSelect.h"
#include "FileRemover.h"
#include "PosTrayTask.h"
#include "PosTrayFilenameEposDlg.h"
#include "PosTrayFilenameHistoryDlg.h"
#include "PosTrayFilenameTimeAttendDlg.h"
#include "PosTrayExportFieldNamesDlg.h"
#include "PosTrayManualExportDlg.h"
#include "PosTrayOptions.h"
#include "PropertySheetPosTrayTask.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsTask.h"
/**********************************************************************/

CPropPagePosTrayOptionsTask::CPropPagePosTrayOptionsTask() : CSSPropertyPage(CPropPagePosTrayOptionsTask::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsTask)
	//}}AFX_DATA_INIT
	m_ExportOptions.m_strComboSelect = szRANGE_TODAY;
	m_ExportOptions.m_oleDateFrom = COleDateTime::GetCurrentTime();
	m_ExportOptions.m_oleDateTo = m_ExportOptions.m_oleDateFrom;
	m_ExportOptions.m_nInterval = 0;

	m_pArrayTaskInfo = NULL;
	m_pArrayBatchInfo = NULL;
	m_nSelectedType = 0;
	m_nSelectedTask = 0;
}

/**********************************************************************/

CPropPagePosTrayOptionsTask::~CPropPagePosTrayOptionsTask()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsTask)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_BUTTON_FILENAME, m_buttonFilename);
	DDX_Control( pDX, IDC_BUTTON_EXPORT, m_buttonExport);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsTask, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsTask)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FILENAME, OnButtonFilename)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TYPE, OnSelectType)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK, OnDoubleClickListTask)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TYPE, OnGetDispInfoListType)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TASK, OnGetDispInfoListTask)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsTask::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listType.SubclassDlgItem(IDC_LIST_TYPE, this);
	m_listType.InsertColumn(1, "Type", LVCFMT_LEFT, 160);
	m_listType.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listType.SetCurSel(0);

	m_listTask.SubclassDlgItem(IDC_LIST_TASK, this);
	m_listTask.InsertColumn(1, "Name", LVCFMT_LEFT, 360);
	m_listTask.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listTask.SetCurSel(0);

	return TRUE;
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_TASK:
		m_listTask.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_TYPE:
		m_listType.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::UpdateSelectedTaskArray()
{
	m_arraySelected.RemoveAll();

	POSITION pos = m_listTask.GetFirstSelectedItemPosition();

	while (pos)
	{
		CSortedIntItem indexItem;
		indexItem.m_nItem = m_listTask.GetNextSelectedItem(pos);
		m_arraySelected.Consolidate(indexItem);
	}
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsTask::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	m_nSelectedTask = m_listTask.GetCurSel();
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsTask::OnSetActive()
{
	GetRecordData();
	m_arrayTaskTypes.RemoveAll();

	if (PosTrayOptions.GetPosTrayProcessTranFlag() == TRUE)
	{
		m_arrayTaskTypes.Add(POSTRAY_TASKFILE_PROCESSTRAN_TASK);
	}

	if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
	{
		if (SysInfo.IsPosTrayTimeAndAttendanceSystem() == TRUE)
		{
			if (PosTrayOptions.GetReportTaskTimeAttendFlag() == TRUE)
			{
				m_arrayTaskTypes.Add(POSTRAY_TASKFILE_TIMEATTEND_TASK);
			}
		}

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

	m_listType.SetItemCountEx(m_arrayTaskTypes.GetSize());

	int nSel = -1;
	for (int n = 0; n < m_arrayTaskTypes.GetSize(); n++)
	{
		if (m_arrayTaskTypes.GetAt(n) == m_nSelectedType)
		{
			nSel = n;
			break;
		}
	}

	if (-1 == nSel)
	{
		nSel = 0;
		m_nSelectedType = m_arrayTaskTypes.GetAt(0);
		m_nSelectedTask = 0;
	}

	SelectLineType(nSel);
	SelectTaskType();
	SelectLineTask(m_nSelectedTask);

	return (CPropertyPage::OnSetActive() != 0);
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::SaveRecord()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnSelectType(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectTaskType();
		SelectLineTask(0);
    }
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::SelectTaskType()
{
	int nSel = GetSafeSelectionType();
	
	if ( nSel != -1 )
	{
		{
			int nOldType = m_nSelectedType;
			m_nSelectedType = m_arrayTaskTypes.GetAt( nSel );

			if (nOldType != m_nSelectedType)
			{
				m_nSelectedTask = 0;
			}
		}

		m_PosTrayFileSet.GetPosTrayFileSet( m_nSelectedType );

		switch( m_nSelectedType )
		{
		case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskProcessTran;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchProcessTran;
			break;

		case POSTRAY_TASKFILE_EPOS_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskReportEpos;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchReportEpos;
			break;

		case POSTRAY_TASKFILE_HIST_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskReportHist;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchReportHist;
			break;

		case POSTRAY_TASKFILE_STKLEVEL_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskReportStkLevel;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchReportStkLevel;
			break;

		case POSTRAY_TASKFILE_TIMEATTEND_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskReportTimeAttend;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchReportTimeAttend;
			break;

		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskExportSales;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchExportSales;
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskExportPayment;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchExportPayment;
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskExportVoid;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchExportVoid;
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_pArrayTaskInfo = m_PosTrayTaskArrays.m_taskExportDiscount;
			m_pArrayBatchInfo = m_PosTrayTaskArrays.m_batchExportDiscount;
			break;
		}

		m_listTask.SetItemCountEx( m_pArrayTaskInfo -> GetSize() );
		m_listTask.Invalidate();

		switch (m_nSelectedType)
		{
		case POSTRAY_TASKFILE_EPOS_TASK:
		case POSTRAY_TASKFILE_HIST_TASK:
		case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		case POSTRAY_TASKFILE_STKLEVEL_TASK:
		{
			bool bFilenames = PosTrayOptions.GetEditReportFilenamesFlag();
			m_buttonFilename.SetWindowText("&Export\nFilenames");
			m_buttonExport.SetWindowText("Make Task\nDue &Now");
			ShowAndEnableWindow(&m_buttonFilename, bFilenames);
			ShowAndEnableWindow(&m_buttonExport, TRUE);
			MoveControl(&m_buttonExport, 423, (bFilenames) ? 216 : 180);
		}
		break;

		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		{
			bool bFieldnames = PosTrayOptions.GetEditExportFieldnamesFlag();
			m_buttonFilename.SetWindowText("&Field\nNames");
			m_buttonExport.SetWindowText("&Manual\nExport");
			ShowAndEnableWindow(&m_buttonFilename, bFieldnames);
			ShowAndEnableWindow(&m_buttonExport, PosTrayOptions.GetManualExportFlag());
			MoveControl(&m_buttonExport, 423, (bFieldnames) ? 216 : 180);
		}
		break;

		default:
			ShowAndEnableWindow(&m_buttonFilename, FALSE);
			ShowAndEnableWindow(&m_buttonExport, FALSE);
			break;
		}
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		CString strTaskType = "";
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTaskTypes.GetSize() ) )
		{
			switch( m_arrayTaskTypes.GetAt( nIndex ) )
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
		
		lstrcpy ( pDispInfo->item.pszText, strTaskType );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnGetDispInfoListTask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLineTask( nIndex );
		CCSV csv ( m_strDisplayLineTask, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPropPagePosTrayOptionsTask::PrepareLineTask( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_pArrayTaskInfo -> GetSize() )
		return FALSE;

	CPosTrayTaskInfo info;
	m_pArrayTaskInfo -> GetAt( nIndex, info );
	m_strDisplayLineTask = info.m_strTaskName;
	
	return TRUE;
}

/**********************************************************************/

int CPropPagePosTrayOptionsTask::GetSafeSelectionTask()
{
	return GetSafeSelectionTask( 0, m_pArrayTaskInfo -> GetSize() - 1 );
}

/**********************************************************************/

int CPropPagePosTrayOptionsTask::GetSafeSelectionTask( int nMin, int nMax )
{
	int nSel = m_listTask.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listTask.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

int CPropPagePosTrayOptionsTask::GetSafeSelectionType()
{
	return GetSafeSelectionType( 0, m_arrayTaskTypes.GetSize() - 1 );
}

/**********************************************************************/

int CPropPagePosTrayOptionsTask::GetSafeSelectionType( int nMin, int nMax )
{
	int nSel = m_listType.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listType.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::SelectLineType( int nIndex )
{
	int nSize = m_listType.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listType.SetCurSel ( nIndex );
	else 
		m_listType.SetCurSel ( nSize - 1 );
	
	m_listType.Invalidate();
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::SelectLineTask(int nIndex)
{
	m_listTask.Invalidate();

	for (int n = 0; n < m_listTask.GetItemCount(); n++)
	{
		if (n != nIndex)
			m_listTask.SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_listTask.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_listTask.SetSelectionMark(n);
			m_listTask.SetCurSel(n);
		}
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnDoubleClickListTask(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

bool CPropPagePosTrayOptionsTask::GetNextFreeTaskNo( int& nTaskNo )
{
	nTaskNo = 1;

	for ( int n = 0; n < m_pArrayTaskInfo -> GetSize(); n++ )
	{
		CPosTrayTaskInfo info;
		m_pArrayTaskInfo -> GetAt( n, info );

		if ( info.m_nTaskNo > nTaskNo )
			break;

		nTaskNo++;
	}

	return ( nTaskNo <= POSTRAY_MAX_TASK );
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::AddTask( int nTaskNo, CPosTrayTask& TaskToCopy )
{
	CPosTrayTaskInfo info;
	info.m_nTaskNo = nTaskNo;
	info.m_strTaskName.Format( "Task %d", nTaskNo );
	info.m_bModified = TRUE;

	int nPos;
	if ( m_pArrayTaskInfo -> Find( info, nPos ) == FALSE )
	{
		m_pArrayTaskInfo -> InsertAt( nPos, info );
		m_listTask.SetItemCountEx ( m_pArrayTaskInfo -> GetSize() );
		m_listTask.Invalidate();
		SelectLineTask( nPos );

		TaskToCopy.SetTaskNo( info.m_nTaskNo );
		TaskToCopy.SetTaskName( info.m_strTaskName );
		TaskToCopy.Write( TRUE );

		EditItem( nPos, TRUE );
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonAdd()
{
	int nTaskNo = 0;
	if ( GetNextFreeTaskNo( nTaskNo ) == FALSE )
	{
		Prompter.Error( "You have already created the maximum\nnumber of tasks of this type." );
		return;
	}

	CPosTrayTask Task( m_PosTrayFileSet.m_nTaskFileType );
	AddTask( nTaskNo, Task );
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonCopy()
{
	int nTaskNo = 0;
	if ( GetNextFreeTaskNo( nTaskNo ) == FALSE )
	{
		Prompter.Error( "You have already created the maximum\nnumber of tasks of this type." );
		return;
	}

	CPosTrayTask TaskToCopy( m_PosTrayFileSet.m_nTaskFileType );
	
	{
		UpdateSelectedTaskArray();

		if (m_arraySelected.GetSize() != 1)
		{
			Prompter.Error("You must select a single task to copy.");
			return;
		}

		int nIndex = 0;
		{
			CSortedIntItem item;
			m_arraySelected.GetAt(0, item);
			nIndex = item.m_nItem;
		}
		
		CPosTrayTaskInfo info;
		m_pArrayTaskInfo -> GetAt( nIndex, info );
		TaskToCopy.SetTaskNo( info.m_nTaskNo );
		TaskToCopy.Read( info.m_bModified );
	}

	AddTask( nTaskNo, TaskToCopy );
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonEdit()
{
	UpdateSelectedTaskArray();

	if (m_arraySelected.GetSize() != 1)
	{
		Prompter.Error("You must select a single task to edit.");
		return;
	}

	int nIndex = 0;
	{
		CSortedIntItem item;
		m_arraySelected.GetAt(0, item);
		nIndex = item.m_nItem;
	}

	EditItem( nIndex, FALSE );
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::EditItem( int nIndex, bool bNewTask )
{
	CPosTrayTaskInfo info;
	m_pArrayTaskInfo -> GetAt( nIndex, info );

	CPosTrayTask task( m_PosTrayFileSet.m_nTaskFileType );
	task.SetTaskNo( info.m_nTaskNo );
	task.Read( info.m_bModified );

	if ( TRUE == bNewTask )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		
		COleDateTime timeLastRun = COleDateTime( 
			timeNow.GetYear(), 
			timeNow.GetMonth(), 
			timeNow.GetDay(), 
			timeNow.GetHour(), 
			timeNow.GetMinute(), 
			0 );

		timeLastRun -= COleDateTimeSpan( 0, 0, 1, 0 );
		
		CDateRangeHelpers DateRangeHelpers;
		task.SetLastRunTime( timeLastRun, TRUE );
	}

	CPropertySheetPosTrayTask propSheet( m_PosTrayFileSet, task, this );
	propSheet.SetBatchInfoArray( m_pArrayBatchInfo );

	if ( propSheet.DoModal() == IDOK )
	{
		info.m_strTaskName = task.GetTaskName();
		info.m_bModified = TRUE;
		m_pArrayTaskInfo -> SetAt( nIndex, info );
		task.Write( TRUE );

		COleDateTime oleLastTaskTime;
		if ( propSheet.GetNotBeforeTime( oleLastTaskTime ) == TRUE )
		{
			oleLastTaskTime -= COleDateTimeSpan( 0, 0, 1, 0 );
			task.SetLastRunTime( oleLastTaskTime, TRUE );
		}

		m_listTask.Invalidate();
		SelectLineTask( nIndex );
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonDelete()
{
	UpdateSelectedTaskArray();

	int nSize = m_arraySelected.GetSize();
	if (0 == nSize)
	{
		return;
	}

	CString strMsg = "";
	if (1 == nSize)
	{
		strMsg = "Are you sure you wish to delete this task";
	}
	else
	{
		strMsg.Format("Are you sure you wish to delete these tasks\n\n(%d tasks selected)", nSize);
	}

	if (Prompter.YesNo(strMsg) != IDYES)
	{
		return;
	}

	int nNewSel = 0;
	for (int n = m_arraySelected.GetSize() - 1; n >= 0; n--)
	{
		CSortedIntItem item;
		m_arraySelected.GetAt(n, item);

		CPosTrayTaskInfo infoTask;
		m_pArrayTaskInfo->GetAt(item.m_nItem, infoTask);

		CFilenameUpdater FnUp(m_PosTrayFileSet.m_infoTaskTemp, 0, infoTask.m_nTaskNo);
		CFileRemover FileRemover(FnUp.GetFilenameToUse());

		m_pArrayTaskInfo->RemoveAt(item.m_nItem);
		nNewSel = item.m_nItem;
	}

	m_listTask.SetItemCountEx(m_pArrayTaskInfo->GetSize());
	m_listTask.Invalidate();
	SelectLineTask(nNewSel);
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonFilename()
{
	switch( m_PosTrayFileSet.m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	{
		CDataManagerInfo info;
		if (DataManagerNonDb.OpenTimeAttendReportNames(DB_READWRITE, info) == FALSE)
		{
			Prompter.ShareError(info);
			return;
		}

		CPosTrayFilenameTimeAttendDlg dlg(this);

		if (dlg.DoModal() == IDOK)
		{
			if (DataManagerNonDb.WriteTimeAttendReportNames(info) == FALSE)
				Prompter.WriteError(info);
		}

		DataManagerNonDb.CloseTimeAttendReportNames(info);
	}
	break;

	case POSTRAY_TASKFILE_HIST_TASK:
		{
			CDataManagerInfo info;
			if ( DataManagerNonDb.OpenSalesHistoryPlu( DB_READWRITE, info ) == FALSE )
			{
				Prompter.ShareError( info );
				return;
			}
		
			CPosTrayFilenameHistoryDlg dlg ( this );
		
			if ( dlg.DoModal() == IDOK )
			{
				if ( DataManagerNonDb.WriteSalesHistoryPlu( info ) == FALSE )
					Prompter.WriteError( info );
			}

			DataManagerNonDb.CloseSalesHistoryPlu( info);
		}
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
		{
			CEposReportSelect EposReportSelect;
			EposReportSelect.LoadReportSelection();

			CPosTrayFilenameEposDlg dlg( EposReportSelect, this );
			
			if ( dlg.DoModal() == IDOK )
				EposReportSelect.SaveReportSelection();
		}
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		if ( SysInfo.IsStockSystem() == TRUE )	
			Prompter.Diagnostic( "Stock Level Report Stock Manager Filenames" );
		else
			Prompter.Diagnostic( "Stock Level Report Ecr Manager Filenames" );
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		{
			CDataManagerInfo info;
			DataManagerNonDb.PosTrayExportFieldNames.FixTaxNames();
			if ( DataManagerNonDb.OpenPosTrayExportFieldNames( DB_READWRITE, info ) == FALSE )
				Prompter.ShareError( info );
			else
			{
				CPosTrayExportFieldNamesDlg dlg( m_PosTrayFileSet.m_nTaskFileType, this );
				
				if ( dlg.DoModal() == IDOK )
				{
					DataManagerNonDb.PosTrayExportFieldNames.PrepareForWrite();
					DataManagerNonDb.WritePosTrayExportFieldNames( info );
				}

				DataManagerNonDb.ClosePosTrayExportFieldNames( info);
			}
		}
		break;
	}
}

/**********************************************************************/

void CPropPagePosTrayOptionsTask::OnButtonExport()
{
	int nIndex = GetSafeSelectionTask();
	if (nIndex == -1)
	{
		return;
	}

	switch (m_nSelectedType)
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
	{

		CPosTrayTaskInfo info;
		m_pArrayTaskInfo->GetAt(nIndex, info);

		CPosTrayManualExportDlg dlg(info, m_PosTrayFileSet, m_ExportOptions, this);
		dlg.DoModal();
	}
	break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	case POSTRAY_TASKFILE_HIST_TASK:
	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
	case POSTRAY_TASKFILE_STKLEVEL_TASK:
	{
		UpdateSelectedTaskArray();

		int nSize = m_arraySelected.GetSize();
		if (0 == nSize)
		{
			return;
		}

		CString strMsg = "";
		if (1 == nSize)
		{
			strMsg = "Are you sure you wish to flag this task as due.";
		}
		else
		{
			strMsg.Format("Are you sure you wish to flag these tasks as due\n\n(%d tasks selected)", nSize);
		}

		strMsg += "\n\nPosTray will not run ";

		if (1 == nSize)
		{
			strMsg += "this task";
		}
		else
		{
			strMsg += "these tasks";
		}

		strMsg += " until the setup dialog is closed via the OK button.";

		if (Prompter.YesNo(strMsg) == IDYES)
		{
			int nEmailTaskType = 0;
			switch (m_nSelectedType)
			{
			case POSTRAY_TASKFILE_EPOS_TASK:
				nEmailTaskType = EMAILTASK_TYPE_EPOS;
				break;

			case POSTRAY_TASKFILE_HIST_TASK:
				nEmailTaskType = EMAILTASK_TYPE_HIST;
				break;

			case POSTRAY_TASKFILE_TIMEATTEND_TASK:
				nEmailTaskType = EMAILTASK_TYPE_TIMEATTEND;
				break;

			case POSTRAY_TASKFILE_STKLEVEL_TASK:
				nEmailTaskType = EMAILTASK_TYPE_STKLEVEL;
				break;

			}

			for (int n = m_arraySelected.GetSize() - 1; n >= 0; n--)
			{
				CSortedIntItem item;
				m_arraySelected.GetAt(n, item);

				CPosTrayTaskInfo infoTask;
				m_pArrayTaskInfo->GetAt(item.m_nItem, infoTask);

				PosTrayOptions.AddToRunNowList(nEmailTaskType, infoTask.m_nTaskNo);
			}
		}
	}
	break;
	}
}

/**********************************************************************/
