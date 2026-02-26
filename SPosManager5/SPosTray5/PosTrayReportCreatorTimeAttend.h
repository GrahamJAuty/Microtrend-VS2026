#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayReportCreatorTimeAttend
{
public:
	CPosTrayReportCreatorTimeAttend(COleDateTime& dateTask, CPosTrayTask& Task );
	~CPosTrayReportCreatorTimeAttend(void);

public:
	bool ProcessTask();

private:
	void RoundDates();
	bool ProcessNormalReports();
	bool ProcessQuickReports();

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
	CString m_strEmailDatePrefix;
	CString m_strExportFolderPending;
	CString m_strExportFolderTarget;
	CString m_strPrintFolder;
	CString m_strPrintFilenamePrefix;
	int m_nNextPrintFileNumber;
};

/**********************************************************************/
#endif
/**********************************************************************/
