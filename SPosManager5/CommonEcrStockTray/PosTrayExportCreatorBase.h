#pragma once
/**********************************************************************/
#include "EposSelectArray.h"
#include "PosTrayTask.h"
#include "SessionCSVArray.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/

class CPosTrayExportCreatorBase
{
protected:
	CPosTrayExportCreatorBase( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog, int nManualExportNo );

public:
	void CreateExportFile();
	void FixDates( COleDateTime& dateFrom, COleDateTime& dateTo );
	
public:
	__int64 ProcessTask( COleDateTime& dateTask );

protected:
	virtual __int64 ProcessTaskInternal() = 0;	
	void PrepareDatesAndFolders();
	void PrepareSessionAndTimeSlice( CTimeSliceMapCSVRecord& TimeSliceRecord );
	bool ProcessExport();

protected:
	void RoundDates();

protected:
	COleDateTime m_dateTask;
	CPosTrayTask& m_Task;

protected:
	CString m_strExportFileType;	
	CString m_strFixedDateSuffix;
	CString m_strTempFilepath;
	
protected:
	CString m_strEmailFolder;
	CString m_strEmailSubjectType;
	CString m_strExportFolderTarget;
	CString m_strExportFolderPending;
	CString m_strInternalFolderName;
	
protected:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CEposSelectArray m_SelectArray;

protected:
	int m_nManualExportNo;
	bool m_bWantCustomerFile;
	bool& m_bQuitNow;
	
protected:	
	void* m_pHostDialog;
};

/**********************************************************************/
