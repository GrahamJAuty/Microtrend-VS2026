#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayReportCreatorHistory
{
public:
	CPosTrayReportCreatorHistory( CPosTrayTask& Task );
	~CPosTrayReportCreatorHistory(void);

public:
	bool ProcessTask( COleDateTime& dateTask );
	int GetSkippedReportCountEmpty() { return m_nSkippedReportsEmpty; }
	int GetSkippedReportCountNoChange() { return m_nSkippedReportsNoChange; }

private:
	void RoundDates();
	
private:
	bool ProcessTaskInternal();

private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CEposSelectArray m_SelectArray;

private:
	COleDateTime m_dateTask;
	CPosTrayTask& m_Task;

private:
	CString m_strEmailFolder;
	CString m_strExportFolderPending;
	CString m_strExportFolderTarget;
	CString m_strPrintFolder;
	CString m_strPrintFilenamePrefix;
	CString m_strMD5CacheFolder;
	int m_nNextPrintFileNumber;
	int m_nSkippedReportsEmpty;
	int m_nSkippedReportsNoChange;

};

/**********************************************************************/
#endif
/**********************************************************************/
