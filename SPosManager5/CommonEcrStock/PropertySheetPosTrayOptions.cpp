/**********************************************************************/
#include "..\CommonEcrStockTray\ChartwellsOptions.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "PosTrayTask.h"
#include "PosTrayOptions.h"
#include "S4LabourLocationOptions.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPosTrayOptions.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPosTrayOptions, CPropertySheet)
/**********************************************************************/

CPropertySheetPosTrayOptions::CPropertySheetPosTrayOptions( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	BuildTaskIndex();

	m_TaskArrays.m_taskProcessTran = &m_arrayTaskInfoProcessTran;
	m_TaskArrays.m_taskReportEpos = &m_arrayTaskInfoEpos;
	m_TaskArrays.m_taskReportHist = &m_arrayTaskInfoHist;
	m_TaskArrays.m_taskReportStkLevel = &m_arrayTaskInfoStkLevel;
	m_TaskArrays.m_taskReportTimeAttend = &m_arrayTaskInfoTimeAttend;
	m_TaskArrays.m_taskExportSales = &m_arrayTaskInfoExportSales;
	m_TaskArrays.m_taskExportPayment = &m_arrayTaskInfoExportPayment;
	m_TaskArrays.m_taskExportVoid = &m_arrayTaskInfoExportVoid;
	m_TaskArrays.m_taskExportDiscount = &m_arrayTaskInfoExportDiscount;

	m_TaskArrays.m_batchProcessTran = &m_arrayBatchInfoProcessTran;
	m_TaskArrays.m_batchReportEpos = &m_arrayBatchInfoEpos;
	m_TaskArrays.m_batchReportHist = &m_arrayBatchInfoHist;
	m_TaskArrays.m_batchReportStkLevel = &m_arrayBatchInfoStkLevel;
	m_TaskArrays.m_batchReportTimeAttend = &m_arrayBatchInfoTimeAttend;
	m_TaskArrays.m_batchExportSales = &m_arrayBatchInfoExportSales;
	m_TaskArrays.m_batchExportPayment = &m_arrayBatchInfoExportPayment;
	m_TaskArrays.m_batchExportVoid = &m_arrayBatchInfoExportVoid;
	m_TaskArrays.m_batchExportDiscount = &m_arrayBatchInfoExportDiscount;

	m_Page1.SetPropertySheet( this );
	m_Page2.SetTaskArrays( m_TaskArrays );
	m_Page3.SetFNBInfoArray( &m_arrayFNBInfo );
	m_Page5.SetS4LabourInfoArray( &m_arrayS4LabourInfo );
	m_Page7.SetChartwellsInfoArray(&m_arrayChartwellsInfo);	

	m_Page2.m_psp.dwFlags |= PSP_USETITLE;
	m_Page2.m_psp.pszTitle = "Tasks";

	m_Page3.m_psp.dwFlags |= PSP_USETITLE;
	m_Page3.m_psp.pszTitle = "FNB Export";

	m_Page4.m_psp.dwFlags |= PSP_USETITLE;
	m_Page4.m_psp.pszTitle = "FTP Settings";

	m_Page5.m_psp.dwFlags |= PSP_USETITLE;
	m_Page5.m_psp.pszTitle = "S4 Labour Export";

	m_Page6.m_psp.dwFlags |= PSP_USETITLE;
	m_Page6.m_psp.pszTitle = "LIPA Export";

	m_Page7.m_psp.dwFlags |= PSP_USETITLE;
	m_Page7.m_psp.pszTitle = "Chartwells Export";

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	AddPage( &m_Page1 );
	AddTaskPages();

	SetTitle ( "Setup PosTray Options" );
}

/**********************************************************************/

CPropertySheetPosTrayOptions::~CPropertySheetPosTrayOptions()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPosTrayOptions, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPosTrayOptions)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPosTrayOptions::GetPosTrayFileSet( int nType )
{
	m_PosTrayFileSet.GetPosTrayFileSet( nType );

	switch( nType )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoProcessTran;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoProcessTran;
		break;

	case POSTRAY_TASKFILE_HIST_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoHist;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoHist;
		break;

	case POSTRAY_TASKFILE_STKLEVEL_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoStkLevel;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoStkLevel;
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoTimeAttend;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoTimeAttend;
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoExportSales;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoExportSales;
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoExportPayment;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoExportPayment;
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoExportVoid;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoExportVoid;
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoExportDiscount;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoExportDiscount;
		break;

	case POSTRAY_TASKFILE_EPOS_ALL:
	default:
		m_PosTrayFileSet.m_pArrayTaskInfo = &m_arrayTaskInfoEpos;
		m_PosTrayFileSet.m_pArrayBatchInfo = &m_arrayBatchInfoEpos;
		break;
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::AddTaskPages()
{
	for (int n = GetPageCount() - 1; n > 0; n--)
	{
		RemovePage(n);
	}

	m_Page2.ClearAdjustedListTypeFlag();
	m_Page2.ClearAdjustedListTaskFlag();
	m_Page3.ClearAdjustedListFlag();
	m_Page5.ClearAdjustedListFlag();
	m_Page7.ClearAdjustedListFlag();

	if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
	{
		if (PosTrayOptions.AreTasksEnabled() == TRUE)
		{
			AddPage(&m_Page2);
		}

		if (PosTrayOptions.AreExportsEnabled() == TRUE)
		{
			AddPage(&m_Page4);
		}

		if (PosTrayOptions.GetFNBTaskFlag() == TRUE)
		{
			AddPage(&m_Page3);
		}

		if (PosTrayOptions.GetS4LabourTaskFlag() == TRUE)
		{
			AddPage(&m_Page5);
		}

		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
		{
			if (PosTrayOptions.GetChartwellsTaskFlag() == TRUE)
			{
				AddPage(&m_Page7);
			}
		}

		if (DealerFlags.Get_LIPA_LJMU_ExportType() != 0)
		{
			AddPage(&m_Page6);
		}
	}
	else
	{
		if (PosTrayOptions.GetPosTrayProcessTranFlag() == TRUE)
		{
			AddPage(&m_Page2);
		}
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::BuildTaskIndex()
{
	for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
	{
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_PROCESSTRAN_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_EPOS_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_HIST_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_STKLEVEL_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_TIMEATTEND_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_EXPORT_SALES_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_EXPORT_VOID_ALL);
		ReadPosTrayTaskFile(nTaskNo, POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL);
	}

	for (int nBatchNo = 1; nBatchNo <= POSTRAY_MAX_BATCH; nBatchNo++)
	{
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_PROCESSTRAN_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_EPOS_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_HIST_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_STKLEVEL_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_TIMEATTEND_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_EXPORT_SALES_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_EXPORT_VOID_ALL);
		ReadPosTrayBatchFile(nBatchNo, POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL);
	}

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		ReadFNBFile(nDbIdx);
	}

	for (int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++)
	{
		ReadS4LabourFile(nLocIdx);
	}

	ReadChartwellsFile();
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::ReadPosTrayTaskFile( int nTaskNo, int nType )
{
	GetPosTrayFileSet( nType );

	//TASK FILE
	{
		CFilenameUpdater FnUp( m_PosTrayFileSet.m_infoTaskFile, 0, nTaskNo );

		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			CPosTrayTask task( m_PosTrayFileSet.m_nTaskFileType );
			task.SetTaskNo(nTaskNo);
			task.Read( FALSE );

			CPosTrayTaskInfo info;
			info.m_nTaskNo = nTaskNo;
			info.m_strTaskName = task.GetTaskName();
			info.m_bModified = FALSE;

			m_PosTrayFileSet.m_pArrayTaskInfo -> Consolidate( info );
		}
	}

	//TASK TEMP FILE
	{
		CFilenameUpdater FnUp( m_PosTrayFileSet.m_infoTaskTemp, 0, nTaskNo );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}

	//TASK DATE TEMP FILE
	{
		CFilenameUpdater FnUp( m_PosTrayFileSet.m_infoDateTemp, 0, nTaskNo );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::ReadPosTrayBatchFile( int nBatchNo, int nType )
{
	GetPosTrayFileSet( nType );

	//BATCH FILE
	{
		CFilenameUpdater FnUp( m_PosTrayFileSet.m_infoBatchFile, 0, nBatchNo );

		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			CPosTrayTask batch( m_PosTrayFileSet.m_nBatchFileType );
			batch.SetBatchNo(nBatchNo);
			batch.Read( FALSE );

			CPosTrayBatchInfo info;
			info.m_nBatchNo = nBatchNo;
			info.m_strBatchName = batch.GetBatchName();
			info.m_bModified = FALSE;

			m_PosTrayFileSet.m_pArrayBatchInfo -> Consolidate( info );
		}
	}

	//BATCH TEMP
	{
		CFilenameUpdater FnUp( m_PosTrayFileSet.m_infoBatchTemp, 0, nBatchNo );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}

	if ( POSTRAY_TASKFILE_EPOS_ALL != nType )
	{
		CFilenameUpdater FnUp( SysFiles::PosTrayConsolTemp, 0, nBatchNo );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::ReadFNBFile( int nDbIdx )
{
	//DATABASE FILE
	{
		CFilenameUpdater FnUp( SysFiles::FNBOptions, nDbIdx );

		CPosTrayFNBInfo info;
		info.m_bEnable = FALSE;
		info.m_nDbIdx = nDbIdx;
		info.m_strDbName = dbDatabase.GetName( nDbIdx );
		info.m_bModified = FALSE;

		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			CFNBDatabaseOptions FNBOptions;
			FNBOptions.Read( nDbIdx, FALSE );
			info.m_bEnable = FNBOptions.GetEnable();
			info.m_strFolder = FNBOptions.GetFTPFolder();
		}

		m_arrayFNBInfo.Consolidate( info );
	}

	//TEMP FILE
	{
		CFilenameUpdater FnUp( SysFiles::FNBOptionsTemp, nDbIdx );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}

	//DATE TEMP FILE
	{
		CFilenameUpdater FnUp( SysFiles::FNBExportDateTemp, nDbIdx );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::ReadS4LabourFile( int nLocIdx )
{
	//DATABASE FILE
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourOptions, nLocIdx );

		CPosTrayS4LabourInfo info;
		info.m_bEnable = FALSE;
		info.m_nLocIdx = nLocIdx;
		info.m_strLocName = dbLocation.GetName( nLocIdx );
		info.m_bModified = FALSE;

		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			CS4LabourLocationOptions S4LabourOptions;
			S4LabourOptions.Read( nLocIdx, FALSE );
			info.m_bEnable = S4LabourOptions.GetEnable();
			info.m_strLocID = S4LabourOptions.GetLocID();
		}

		m_arrayS4LabourInfo.Consolidate( info );
	}

	//TEMP FILE
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourOptionsTemp, nLocIdx );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}

	//DATE TEMP FILE
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourExportDateTemp, nLocIdx );
		CFileRemover FileRemover( FnUp.GetFilenameToUse() );
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::ReadChartwellsFile()
{
	//DATABASE FILE
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptions);

		CPosTrayChartwellsInfo info;
		info.m_bEnable = FALSE;
		info.m_bModified = FALSE;

		if (::FileExists(FnUp.GetFilenameToUse()) == TRUE)
		{
			ChartwellsOptions.Read(FALSE);
			info.m_bEnable = ChartwellsOptions.GetEnable();
			info.m_strServer = ChartwellsOptions.GetFTPServer();
		}

		m_arrayChartwellsInfo.Consolidate(info);
	}

	//TEMP FILE
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptionsTemp);
		CFileRemover FileRemover(FnUp.GetFilenameToUse());
	}

	//DATE TEMP FILE
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsExportDateTemp);
		CFileRemover FileRemover(FnUp.GetFilenameToUse());
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveTaskChanges(bool bMergedOnly)
{
	CArray<int, int> arrayTaskFileTypes;
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_PROCESSTRAN_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_EPOS_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_HIST_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_STKLEVEL_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_TIMEATTEND_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_EXPORT_SALES_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_EXPORT_VOID_ALL);
	arrayTaskFileTypes.Add(POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL);

	__int64 nTarget = POSTRAY_MAX_TASK * arrayTaskFileTypes.GetSize();
	nTarget += dbDatabase.GetSize();
	nTarget += dbLocation.GetSize();
	nTarget += 1;

	if (FALSE == bMergedOnly)
	{
		nTarget += POSTRAY_MAX_TASK * arrayTaskFileTypes.GetSize();
		nTarget += POSTRAY_MAX_BATCH * (arrayTaskFileTypes.GetSize() + 1);
	}

	if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
	{
		nTarget++;
	}

	__int64 nProgress = 0;
	StatusProgress.Lock(TRUE, "Saving PosTray Task Settings");

	if (FALSE == bMergedOnly)
	{
		for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
		{
			for (int n = 0; n < arrayTaskFileTypes.GetSize(); n++)
			{
				SavePosTrayTaskFile(nTaskNo, arrayTaskFileTypes.GetAt(n));
				StatusProgress.SetPos(++nProgress, nTarget);
			}
		}

		for (int nBatchNo = 1; nBatchNo <= POSTRAY_MAX_BATCH; nBatchNo++)
		{
			for (int n = 0; n < arrayTaskFileTypes.GetSize(); n++)
			{
				SavePosTrayBatchFile(nBatchNo, arrayTaskFileTypes.GetAt(n));
				StatusProgress.SetPos(++nProgress, nTarget);
			}

			SaveBatchEposConsolFile(nBatchNo);
			StatusProgress.SetPos(++nProgress, nTarget);
		}
	}

	CString strIndexFile = "PosTray";
	::GetSyssetProgramPath(strIndexFile);
	::CreateSubdirectory(strIndexFile);
	strIndexFile += "\\File9998.dat";

	{
		CSSFile fileIndex;
		fileIndex.Open(strIndexFile, "wb");

		for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
		{
			for (int n = 0; n < arrayTaskFileTypes.GetSize(); n++)
			{
				SavePosTrayMergedTaskFile(nTaskNo, arrayTaskFileTypes.GetAt(n), fileIndex);
				StatusProgress.SetPos(++nProgress, nTarget);
			}
		}
	}

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		SaveFNBFile(nDbIdx);
		StatusProgress.SetPos(++nProgress, nTarget);
	}

	if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
	{
		SaveChartwellsFile();
		StatusProgress.SetPos(++nProgress, nTarget);
	}
	
	for (int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++)
	{
		SaveS4LabourFile(nLocIdx);
		StatusProgress.SetPos(++nProgress, nTarget);
	}

	SaveLIPADate();
	StatusProgress.SetPos(++nProgress, nTarget);

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SavePosTrayBatchFile(int nBatchNo, int nType)
{
	GetPosTrayFileSet(nType);

	CString strFilenameBatch;
	{
		CFilenameUpdater FnUp(m_PosTrayFileSet.m_infoBatchFile, 0, nBatchNo);
		strFilenameBatch = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp(m_PosTrayFileSet.m_infoBatchTemp, 0, nBatchNo);
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CPosTrayBatchInfo info;
	info.m_nBatchNo = nBatchNo;

	int nPos;
	if (m_PosTrayFileSet.m_pArrayBatchInfo->Find(info, nPos) == TRUE)
	{
		m_PosTrayFileSet.m_pArrayBatchInfo->GetAt(nPos, info);

		if (TRUE == info.m_bModified)
		{
			::CopyFile(strFilenameTemp, strFilenameBatch, FALSE);
		}
	}
	else
	{
		CFileRemover FileRemover(strFilenameBatch);
	}

	CFileRemover FileRemover(strFilenameTemp);
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveBatchEposConsolFile( int nBatchNo )
{
	CString strFilenameBatch;
	{
		CFilenameUpdater FnUp ( SysFiles::PosTrayConsolEpos, 0, nBatchNo );
		strFilenameBatch = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::PosTrayConsolTemp, 0, nBatchNo );
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CReportConsolidationArray<CPosTrayBatchInfo>* pArray = &m_arrayBatchInfoEpos;

	CPosTrayBatchInfo info;
	info.m_nBatchNo = nBatchNo;

	int nPos;
	if ( pArray -> Find( info, nPos ) == TRUE )
	{
		pArray -> GetAt( nPos, info );

		if (TRUE == info.m_bModified)
		{
			::CopyFile(strFilenameTemp, strFilenameBatch, FALSE);
		}
	}
	else
	{
		CFileRemover FileRemover(strFilenameBatch);
	}

	CFileRemover FileRemover( strFilenameTemp );
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SavePosTrayTaskFile( int nTaskNo, int nType )
{
	GetPosTrayFileSet( nType );

	CString strFilenameTask;
	{
		CFilenameUpdater FnUp ( m_PosTrayFileSet.m_infoTaskFile, 0, nTaskNo );
		strFilenameTask = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp ( m_PosTrayFileSet.m_infoTaskTemp, 0, nTaskNo );
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CPosTrayTaskInfo info;
	info.m_nTaskNo = nTaskNo;

	int nPos;
	if (m_PosTrayFileSet.m_pArrayTaskInfo->Find(info, nPos) == TRUE)
	{
		m_PosTrayFileSet.m_pArrayTaskInfo->GetAt(nPos, info);

		if (TRUE == info.m_bModified)
		{
			::CopyFile(strFilenameTemp, strFilenameTask, FALSE);
		}
	}
	else
	{
		CFileRemover FileRemover(strFilenameTask);
	}

	CFileRemover FileRemover( strFilenameTemp );
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SavePosTrayMergedTaskFile( int nTaskNo, int nType, CSSFile& fileIndex )
{
	GetPosTrayFileSet( nType );

	CPosTrayTask task	( m_PosTrayFileSet.m_nTaskFileType );
	CPosTrayTask batch	( m_PosTrayFileSet.m_nBatchFileType );
	CPosTrayTask merge	( nType );

	bool bGotTask = TRUE;

	task.SetTaskNo( nTaskNo );
	if (::FileExists(task.GetFilename(FALSE)) == FALSE)
	{
		bGotTask = FALSE;
	}
	else if (task.Read(FALSE) == FALSE)
	{
		bGotTask = FALSE;
	}
	else
	{
		int nBatchNo = task.GetTaskBatchNo();

		if (nBatchNo != 0)
		{
			batch.SetBatchNo(task.GetTaskBatchNo());
			if (::FileExists(batch.GetFilename(FALSE)) == FALSE)
			{
				bGotTask = FALSE;
			}
			else if (batch.Read(FALSE) == FALSE)
			{
				bGotTask = FALSE;
			}
		}
	}
	
	if ( TRUE == bGotTask )
	{
		merge.SetTaskNo( nTaskNo );
		merge.MergeTask( task );
		merge.MergeBatch( batch );
		merge.SetMergeDeleteFlag( FALSE );
		merge.Write( TRUE );

		CString strDateFilenameTemp = task.GetDateFilename( TRUE );
		CString strDateFilenameLive = task.GetDateFilename( FALSE );

		CopyFile( strDateFilenameTemp, strDateFilenameLive, FALSE );
		CFileRemover FileRemover( strDateFilenameTemp );

		{
			CString strIndex = "";
			CPosTrayTask TaskAll( m_PosTrayFileSet.m_nAllFileType );
			strIndex = TaskAll.GetFilename( FALSE );
			fileIndex.WriteLine( strIndex.Right(26).Left(13) );
		}
	}
	else
	{
		merge.SetTaskNo( nTaskNo );
		merge.SetMergeDeleteFlag( TRUE );
		merge.Write( TRUE );
		merge.SetLastRunTime( "", FALSE );
	}
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveFNBFile( int nDbIdx )
{
	CString strFilenameDb;
	{
		CFilenameUpdater FnUp ( SysFiles::FNBOptions, nDbIdx );
		strFilenameDb = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::FNBOptionsTemp, nDbIdx );
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CString strFilenameDate;
	{
		CFilenameUpdater FnUp ( SysFiles::FNBExportDate, nDbIdx );
		strFilenameDate = FnUp.GetFilenameToUse();
	}

	CString strFilenameDateTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::FNBExportDateTemp, nDbIdx );
		strFilenameDateTemp = FnUp.GetFilenameToUse();
	}

	CPosTrayFNBInfo info;
	info.m_nDbIdx = nDbIdx;

	int nPos;
	if ( m_arrayFNBInfo.Find( info, nPos ) == TRUE )
	{
		m_arrayFNBInfo.GetAt( nPos, info );

		if ( TRUE == info.m_bModified )
		{
			::CopyFile( strFilenameTemp, strFilenameDb, FALSE );
			::CopyFile( strFilenameDateTemp, strFilenameDate, FALSE );
		}
	}

	CFileRemover FileRemover;
	FileRemover.RemoveFile( strFilenameTemp );
	FileRemover.RemoveFile( strFilenameDateTemp );
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveChartwellsFile()
{
	CString strFilenameDb;
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptions);
		strFilenameDb = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptionsTemp);
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CString strFilenameDate;
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsExportDate);
		strFilenameDate = FnUp.GetFilenameToUse();
	}

	CString strFilenameDateTemp;
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsExportDateTemp);
		strFilenameDateTemp = FnUp.GetFilenameToUse();
	}

	CPosTrayChartwellsInfo info;
	
	int nPos = 0;
	if (m_arrayChartwellsInfo.Find(info, nPos) == TRUE)
	{
		m_arrayChartwellsInfo.GetAt(nPos, info);

		if (TRUE == info.m_bModified)
		{
			::CopyFile(strFilenameTemp, strFilenameDb, FALSE);
			::CopyFile(strFilenameDateTemp, strFilenameDate, FALSE);
		}
	}

	CFileRemover FileRemover;
	FileRemover.RemoveFile(strFilenameTemp);
	FileRemover.RemoveFile(strFilenameDateTemp);
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveLIPADate()
{
	CString strFilenameDate;
	{
		CFilenameUpdater FnUp ( SysFiles::LIPAExportDate );
		strFilenameDate = FnUp.GetFilenameToUse();
	}

	CString strFilenameDateTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::LIPAExportDateTemp );
		strFilenameDateTemp = FnUp.GetFilenameToUse();
	}

	::CopyFile( strFilenameDateTemp, strFilenameDate, FALSE );
	CFileRemover FileRemover( strFilenameDateTemp );
}

/**********************************************************************/

void CPropertySheetPosTrayOptions::SaveS4LabourFile( int nLocIdx )
{
	CString strFilenameLoc;
	{
		CFilenameUpdater FnUp ( SysFiles::S4LabourOptions, nLocIdx );
		strFilenameLoc = FnUp.GetFilenameToUse();
	}

	CString strFilenameTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::S4LabourOptionsTemp, nLocIdx );
		strFilenameTemp = FnUp.GetFilenameToUse();
	}

	CString strFilenameDate;
	{
		CFilenameUpdater FnUp ( SysFiles::S4LabourExportDate, nLocIdx );
		strFilenameDate = FnUp.GetFilenameToUse();
	}

	CString strFilenameDateTemp;
	{
		CFilenameUpdater FnUp ( SysFiles::S4LabourExportDateTemp, nLocIdx );
		strFilenameDateTemp = FnUp.GetFilenameToUse();
	}

	CPosTrayS4LabourInfo info;
	info.m_nLocIdx = nLocIdx;

	int nPos;
	if ( m_arrayS4LabourInfo.Find( info, nPos ) == TRUE )
	{
		m_arrayS4LabourInfo.GetAt( nPos, info );

		if ( TRUE == info.m_bModified )
		{
			::CopyFile( strFilenameTemp, strFilenameLoc, FALSE );
			::CopyFile( strFilenameDateTemp, strFilenameDate, FALSE );
		}
	}

	CFileRemover FileRemover;
	FileRemover.RemoveFile( strFilenameTemp );
	FileRemover.RemoveFile( strFilenameDateTemp );
}

/**********************************************************************/

bool CPropertySheetPosTrayOptions::RunPosTrayAtStartup()
{	
	char cBuffer[ _MAX_PATH ];
	if ( _getcwd( cBuffer, _MAX_PATH ) == NULL )
		return FALSE;

	CString strFilePathPosTray = cBuffer;
	strFilePathPosTray += "\\spostray.exe";

	if ( ::FileExists( strFilePathPosTray ) == FALSE )
	{
		CString strMsg;
		strMsg = "You must install SPosTray.exe in the same folder as ";
		strMsg += AfxGetAppName();
		strMsg += "\nbefore you can run the PosTray utility.";

		Prompter.Error( strMsg );
		return FALSE;
	}

	CString strParam = "";
	CString strDataPath = "";
	CString strLink = "spostray.lnk";

	int nDataPath = SysInfo.GetDataPathNo();
	
	if ( 0 != nDataPath )
	{
		strParam.Format( "%d", nDataPath );
		strLink.Format ( "spostray_%d.lnk", nDataPath );
		strDataPath = strParam;
	}
	else
		strDataPath = "0";
		
	CString strFolderPath = strFilePathPosTray.Left( strFilePathPosTray.GetLength() - 13 );	
	CreateStartupLink( strLink, strFilePathPosTray, strFolderPath, strParam );
	
	CCommandLineBuilder CommandLineBuilder;
	CommandLineBuilder.AddToken( strFilePathPosTray );
	CommandLineBuilder.AddToken( "1" );

#ifdef STOCKMAN_SYSTEM
	CommandLineBuilder.AddToken( "1" );
#else
	CommandLineBuilder.AddToken( "0" );
#endif
		
	CommandLineBuilder.AddToken( strDataPath );
	CString strCommandLine = CommandLineBuilder.GetLine();
	LPTSTR cmd = strCommandLine.GetBuffer(strCommandLine.GetLength() + 1);
 
	STARTUPINFO startup;
     PROCESS_INFORMATION procinfo;

     ::ZeroMemory(&startup, sizeof(startup));
	::ZeroMemory(&procinfo, sizeof(procinfo));

	if ( CreateProcess( NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &procinfo ) == TRUE )
	{
		CloseHandle(procinfo.hProcess);
		CloseHandle(procinfo.hThread);
	}

	return TRUE;
}

/**********************************************************************/

bool CPropertySheetPosTrayOptions::RemovePosTrayAtStartup()
{
	CString strPath = ::GetStartupPath();
	
	if ( strPath == "" )
		return FALSE;

	int nDataPath = SysInfo.GetDataPathNo();

	if ( 0 == nDataPath )
		strPath += "\\spostray.lnk";
	else
	{
		CString strExtra;
		strExtra.Format( "\\spostray_%d.lnk", nDataPath );
		strPath += strExtra;
	}

	CFileRemover FileRemover( strPath );

	return ( ::FileExists( strPath ) == FALSE );
}

/**********************************************************************/
