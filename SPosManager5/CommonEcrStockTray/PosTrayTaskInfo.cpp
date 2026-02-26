/**********************************************************************/
#include "FileRemover.h"
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayTaskInfo.h"
/**********************************************************************/

CPosTrayEmailScheduleInfo::CPosTrayEmailScheduleInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayEmailScheduleInfo::Reset()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_nTaskType = EMAILTASK_TYPE_EPOS;
	m_nTaskNo = 0;
	m_strTaskName = "";
	m_timeTaskDue = timeNow;
	m_timeTaskChecked = timeNow;
	m_timeTaskRetryFolder = timeNow;
	m_timeTaskFile = CTime( 1980, 1, 1, 0, 0, 0, -1 );
	m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
	m_nTaskError = 0;
	m_bWantEmail = FALSE;
	m_bWantExport = FALSE;
	m_bWantPrint = FALSE;
	m_nRunNowStatus = 0;
}

/**********************************************************************/

void CPosTrayEmailScheduleInfo::Add ( CPosTrayEmailScheduleInfo& source )
{
}

/**********************************************************************/

int CPosTrayEmailScheduleInfo::Compare( CPosTrayEmailScheduleInfo& source, int nHint )
{
	if ( m_nTaskType != source.m_nTaskType )
		return ( m_nTaskType > source.m_nTaskType ? 1 : -1 );

	if ( m_nTaskNo != source.m_nTaskNo )
		return ( m_nTaskNo > source.m_nTaskNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

int CPosTrayEmailScheduleInfo::GetFileType()
{
	switch( m_nTaskType )
	{
	case EMAILTASK_TYPE_PROCESSTRAN:
		return POSTRAY_TASKFILE_PROCESSTRAN_ALL;

	case EMAILTASK_TYPE_TIMEATTEND:
		return POSTRAY_TASKFILE_TIMEATTEND_ALL;

	case EMAILTASK_TYPE_HIST:
		return POSTRAY_TASKFILE_HIST_ALL;

	case EMAILTASK_TYPE_STKLEVEL:
		return POSTRAY_TASKFILE_STKLEVEL_ALL;

	case EMAILTASK_TYPE_EXPORT_SALES:
		return POSTRAY_TASKFILE_EXPORT_SALES_ALL;

	case EMAILTASK_TYPE_EXPORT_PAYMENT:
		return POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL;

	case EMAILTASK_TYPE_EXPORT_VOID:
		return POSTRAY_TASKFILE_EXPORT_VOID_ALL;

	case EMAILTASK_TYPE_EXPORT_DISCOUNT:
		return POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL;

	case EMAILTASK_TYPE_EPOS:
	default:
		return POSTRAY_TASKFILE_EPOS_ALL;
	}
}

/**********************************************************************/

void CPosTrayEmailScheduleInfo::GetTaskTypeStrings( CString& strAction, CString& strType )
{
	switch( m_nTaskType )
	{
	case EMAILTASK_TYPE_PROCESSTRAN:
		strAction = "PROCESS";
		strType = "TRAN";
		break;

	case EMAILTASK_TYPE_EXPORT_SALES:
		strAction = "CSV EXPORT";
		strType = "SALES";
		break;
		
	case EMAILTASK_TYPE_EXPORT_PAYMENT:
		strAction = "CSV EXPORT";
		strType = "PAYMENTS";
		break;

	case EMAILTASK_TYPE_EXPORT_VOID:
		strAction = "CSV EXPORT";
		strType = "VOIDS";
		break;

	case EMAILTASK_TYPE_EXPORT_DISCOUNT:
		strAction = "CSV EXPORT";
		strType = "DISCOUNTS";
		break;

	case EMAILTASK_TYPE_TIMEATTEND:
		strAction = "REPORTS";
		strType = "TIME & ATTENDANCE";
		break;

	case EMAILTASK_TYPE_HIST:
		strAction = "REPORTS";
		strType = "HISTORY";
		break;

	case EMAILTASK_TYPE_STKLEVEL:
		strAction = "REPORTS";
		strType = "STOCK LEVELS";
		break;
	
	case EMAILTASK_TYPE_EPOS:
	default:
		strAction = "REPORTS";
		strType = "EPOS";
		break;
	}	
}

/**********************************************************************/

void CPosTrayEmailScheduleInfo::InitRunNowFlag()
{
	CString strFilename = GetRunTaskNowTokenFilename();
	m_nRunNowStatus = ::FileExists(strFilename) ? 1 : 0;
	CFileRemover FileRemover(strFilename);
}

/**********************************************************************/

CString CPosTrayEmailScheduleInfo::GetRunTaskNowTokenFilename()
{
	CString strFilename;
	strFilename.Format("POSTRAY\\RUNTASKNOW\\T%3.3dN%3.3d.dat",
		m_nTaskType,
		m_nTaskNo);

	::GetSyssetProgramPath(strFilename);
	return strFilename;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayTaskInfo::CPosTrayTaskInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayTaskInfo::Reset()
{
	m_nTaskNo = 0;
	m_strTaskName = "";
	m_bModified = 0;
}

/**********************************************************************/

void CPosTrayTaskInfo::Add ( CPosTrayTaskInfo& source )
{
}

/**********************************************************************/

int CPosTrayTaskInfo::Compare(CPosTrayTaskInfo& source, int nHint)
{
	if (m_nTaskNo != source.m_nTaskNo)
	{
		return (m_nTaskNo > source.m_nTaskNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayBatchInfo::CPosTrayBatchInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayBatchInfo::Reset()
{
	m_nBatchNo = 0;
	m_strBatchName = "";
	m_bModified = 0;
}

/**********************************************************************/

void CPosTrayBatchInfo::Add ( CPosTrayBatchInfo& source )
{
}

/**********************************************************************/

int CPosTrayBatchInfo::Compare( CPosTrayBatchInfo& source, int nHint )
{
	if ( m_nBatchNo != source.m_nBatchNo )
		return ( m_nBatchNo > source.m_nBatchNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayFNBInfo::CPosTrayFNBInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayFNBInfo::Reset()
{
	m_nDbIdx = 0;
	m_strDbName = "";
	m_bEnable = FALSE;
	m_bModified = 0;
	m_strFolder = "";
}

/**********************************************************************/

void CPosTrayFNBInfo::Add ( CPosTrayFNBInfo& source )
{
}

/**********************************************************************/

int CPosTrayFNBInfo::Compare( CPosTrayFNBInfo& source, int nHint )
{
	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayFNBScheduleInfo::CPosTrayFNBScheduleInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayFNBScheduleInfo::Reset()
{
	m_nDbIdx = 0;
	m_strDbName = "";
	m_timeExportDue = COleDateTime::GetCurrentTime();
	m_timeExportFile = CTime( 1980, 1, 1, 0, 0, 0, -1 );
	m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
	m_nExportError = 0;
}

/**********************************************************************/

void CPosTrayFNBScheduleInfo::Add ( CPosTrayFNBScheduleInfo& source )
{
}

/**********************************************************************/

int CPosTrayFNBScheduleInfo::Compare( CPosTrayFNBScheduleInfo& source, int nHint )
{
	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayChartwellsInfo::CPosTrayChartwellsInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayChartwellsInfo::Reset()
{
	m_bEnable = FALSE;
	m_bModified = 0;
	m_strServer = "";
}

/**********************************************************************/

void CPosTrayChartwellsInfo::Add(CPosTrayChartwellsInfo& source)
{
}

/**********************************************************************/

int CPosTrayChartwellsInfo::Compare(CPosTrayChartwellsInfo& source, int nHint)
{
	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayChartwellsScheduleInfo::CPosTrayChartwellsScheduleInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayChartwellsScheduleInfo::Reset()
{
	m_timeExportDue = COleDateTime::GetCurrentTime();
	m_timeExportFile = CTime(1980, 1, 1, 0, 0, 0, -1);
	m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
	m_strLastDbCheckTime = "";
	m_nLastDbCheckMinute = -1;
	m_nExportError = 0;
	m_nDatabaseError = -1;
}

/**********************************************************************/

void CPosTrayChartwellsScheduleInfo::Add(CPosTrayChartwellsScheduleInfo& source)
{
}

/**********************************************************************/

int CPosTrayChartwellsScheduleInfo::Compare(CPosTrayChartwellsScheduleInfo& source, int nHint)
{
	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayBackupScheduleInfo::CPosTrayBackupScheduleInfo	()
{
	Reset();
}

/**********************************************************************/

void CPosTrayBackupScheduleInfo::Reset()
{
}

/**********************************************************************/

void CPosTrayBackupScheduleInfo::Add(CPosTrayBackupScheduleInfo& source)
{
}

/**********************************************************************/

int CPosTrayBackupScheduleInfo::Compare(CPosTrayBackupScheduleInfo& source, int nHint)
{
	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayS4LabourInfo::CPosTrayS4LabourInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayS4LabourInfo::Reset()
{
	m_nLocIdx = 0;
	m_strLocName = "";
	m_bEnable = FALSE;
	m_bModified = 0;
	m_strLocID = "";
}

/**********************************************************************/

void CPosTrayS4LabourInfo::Add ( CPosTrayS4LabourInfo& source )
{
}

/**********************************************************************/

int CPosTrayS4LabourInfo::Compare( CPosTrayS4LabourInfo& source, int nHint )
{
	if (m_nLocIdx != source.m_nLocIdx)
	{
		return (m_nLocIdx > source.m_nLocIdx ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayS4LabourScheduleInfo::CPosTrayS4LabourScheduleInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayS4LabourScheduleInfo::Reset()
{
	m_nLocIdx = 0;
	m_strLocName = "";
	m_strLocID = "";
	m_bDuplicateID = FALSE;
	m_timeExportDue = COleDateTime::GetCurrentTime();
	m_timeExportFile = CTime( 1980, 1, 1, 0, 0, 0, -1 );
	m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
	m_nExportError = 0;
}

/**********************************************************************/

void CPosTrayS4LabourScheduleInfo::Add ( CPosTrayS4LabourScheduleInfo& source )
{
}

/**********************************************************************/

int CPosTrayS4LabourScheduleInfo::Compare( CPosTrayS4LabourScheduleInfo& source, int nHint )
{
	if ( m_nLocIdx != source.m_nLocIdx )
		return ( m_nLocIdx > source.m_nLocIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayLIPAScheduleInfo::CPosTrayLIPAScheduleInfo()
{
	Reset();
}

/**********************************************************************/

void CPosTrayLIPAScheduleInfo::Reset()
{
	m_timeExportDue = COleDateTime::GetCurrentTime();
	m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
	m_nExportError = 0;
}

/**********************************************************************/
