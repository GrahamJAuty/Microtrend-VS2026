#pragma once
/**********************************************************************/
#define EMAILTASK_TYPE_EPOS 0
#define EMAILTASK_TYPE_HIST 1
#define EMAILTASK_TYPE_EXPORT_SALES 2
#define EMAILTASK_TYPE_EXPORT_PAYMENT 3
#define EMAILTASK_TYPE_EXPORT_VOID 4
#define EMAILTASK_TYPE_EXPORT_DISCOUNT 5
#define EMAILTASK_TYPE_STKLEVEL 6
#define EMAILTASK_TYPE_PROCESSTRAN 7
#define EMAILTASK_TYPE_TIMEATTEND 8
/**********************************************************************/
#define EMAILTASK_STATUS_UNCHECKED 0
#define EMAILTASK_STATUS_NOFILE 1
#define EMAILTASK_STATUS_NOTASK 2
#define EMAILTASK_STATUS_INVALID 3
#define EMAILTASK_STATUS_WAIT_TASK 4
#define EMAILTASK_STATUS_WAIT_UPDATE 5
#define EMAILTASK_STATUS_WAIT_CASHRSP 6
#define EMAILTASK_STATUS_WAIT_RETRYFOLDER 7
/**********************************************************************/
#define BESPOKE_EXPORT_STATUS_UNCHECKED 0
#define BESPOKE_EXPORT_STATUS_NOFILE 1
#define BESPOKE_EXPORT_STATUS_INVALID 2
#define BESPOKE_EXPORT_STATUS_DISABLED 3
#define BESPOKE_EXPORT_STATUS_WAIT_EXPORT 4
#define BESPOKE_EXPORT_STATUS_WAIT_UPDATE 5
#define BESPOKE_EXPORT_STATUS_WAIT_CASHRSP 6
/**********************************************************************/
//LIPA EXPORT DOES NOT USE BESPOKE STATES 1, 2, 3
/**********************************************************************/

struct CPosTrayEmailScheduleInfo
{
public:
	CPosTrayEmailScheduleInfo();
	void Reset();

public:
	int GetFileType();
	void GetTaskTypeStrings( CString& strAction, CString& strType );
	void InitRunNowFlag();

public:
	int Compare ( CPosTrayEmailScheduleInfo& source, int nHint = 0 );
	void Add	( CPosTrayEmailScheduleInfo& source );

private:
	CString GetRunTaskNowTokenFilename();

public:
	int m_nTaskType;
	int m_nTaskNo;
	CString m_strTaskName;
	COleDateTime m_timeTaskDue;
	COleDateTime m_timeTaskChecked;
	COleDateTime m_timeTaskRetryFolder;
	CTime m_timeTaskFile;
	int m_nTaskStatus;
	int m_nTaskError;
	bool m_bWantEmail;
	bool m_bWantExport;
	bool m_bWantPrint;
	int m_nRunNowStatus;
};

/**********************************************************************/

struct CPosTrayTaskInfo
{
public:
	CPosTrayTaskInfo();
	void Reset();

public:
	int Compare ( CPosTrayTaskInfo& source, int nHint = 0 );
	void Add	( CPosTrayTaskInfo& source );
	
public:
	int m_nTaskNo;
	CString m_strTaskName;
	bool m_bModified;
};

/**********************************************************************/

struct CPosTrayBatchInfo
{
public:
	CPosTrayBatchInfo();
	void Reset();

public:
	int Compare ( CPosTrayBatchInfo& source, int nHint = 0 );
	void Add	( CPosTrayBatchInfo& source );
	
public:
	int m_nBatchNo;
	CString m_strBatchName;
	bool m_bModified;
};

/**********************************************************************/

struct CPosTrayFNBInfo
{
public:
	CPosTrayFNBInfo();
	void Reset();

public:
	int Compare ( CPosTrayFNBInfo& source, int nHint = 0 );
	void Add	( CPosTrayFNBInfo& source );
	
public:
	int m_nDbIdx;
	CString m_strDbName;
	bool m_bEnable;
	bool m_bModified;
	CString m_strFolder;
};

/**********************************************************************/

struct CPosTrayFNBScheduleInfo
{
public:
	CPosTrayFNBScheduleInfo();
	void Reset();

public:
	int Compare ( CPosTrayFNBScheduleInfo& source, int nHint = 0 );
	void Add	( CPosTrayFNBScheduleInfo& source );
	
public:
	int m_nDbIdx;
	CString m_strDbName;
	COleDateTime m_timeExportDue;
	COleDateTime m_timeExportRetry;
	CTime m_timeExportFile;
	int m_nExportStatus;
	int m_nExportError;

private:
	CString m_strTaskType;
};

/**********************************************************************/

struct CPosTrayChartwellsInfo
{
public:
	CPosTrayChartwellsInfo();
	void Reset();

public:
	int Compare(CPosTrayChartwellsInfo& source, int nHint = 0);
	void Add(CPosTrayChartwellsInfo& source);

public:
	bool m_bEnable;
	bool m_bModified;
	CString m_strServer;
};

/**********************************************************************/

struct CPosTrayChartwellsScheduleInfo
{
public:
	CPosTrayChartwellsScheduleInfo();
	void Reset();

public:
	int Compare(CPosTrayChartwellsScheduleInfo& source, int nHint = 0);
	void Add(CPosTrayChartwellsScheduleInfo& source);

public:
	COleDateTime m_timeExportDue;
	COleDateTime m_timeExportRetry;
	CTime m_timeExportFile;
	CString m_strLastDbCheckTime;
	int m_nLastDbCheckMinute;
	int m_nDatabaseError;
	int m_nExportStatus;
	int m_nExportError;

private:
	CString m_strTaskType;
};

/**********************************************************************/

struct CPosTrayBackupScheduleInfo
{
public:
	CPosTrayBackupScheduleInfo();
	void Reset();

public:
	int Compare(CPosTrayBackupScheduleInfo& source, int nHint = 0);
	void Add(CPosTrayBackupScheduleInfo	& source);

private:
	CString m_strTaskType;
};

/**********************************************************************/

struct CPosTrayS4LabourInfo
{
public:
	CPosTrayS4LabourInfo();
	void Reset();

public:
	int Compare ( CPosTrayS4LabourInfo& source, int nHint = 0 );
	void Add	( CPosTrayS4LabourInfo& source );
	
public:
	int m_nLocIdx;
	CString m_strLocName;
	bool m_bEnable;
	bool m_bModified;
	CString m_strLocID;
};

/**********************************************************************/

struct CPosTrayS4LabourScheduleInfo
{
public:
	CPosTrayS4LabourScheduleInfo();
	void Reset();

public:
	int Compare ( CPosTrayS4LabourScheduleInfo& source, int nHint = 0 );
	void Add	( CPosTrayS4LabourScheduleInfo& source );
	
public:
	int m_nLocIdx;
	CString m_strLocName;
	CString m_strLocID;
	COleDateTime m_timeExportDue;
	COleDateTime m_timeExportRetry;
	CTime m_timeExportFile;
	bool m_bDuplicateID;
	int m_nExportStatus;
	int m_nExportError;

private:
	CString m_strTaskType;
};

/**********************************************************************/

struct CPosTrayLIPAScheduleInfo
{
public:
	CPosTrayLIPAScheduleInfo();
	void Reset();
	
public:
	COleDateTime m_timeExportDue;
	COleDateTime m_timeExportRetry;
	int m_nExportStatus;
	int m_nExportError;

private:
	CString m_strTaskType;
};

/**********************************************************************/
