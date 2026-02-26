#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayReportCreatorStkLevelEcr
{
public:
	CPosTrayReportCreatorStkLevelEcr( CPosTrayTask& Task );
	~CPosTrayReportCreatorStkLevelEcr(void);

public:
	bool ProcessTask( COleDateTime& dateTask );

private:
	bool ProcessTaskInternal();
	bool ProcessNormalReports();
	bool ProcessQuickReports();

private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CEposSelectArray m_SelectArray;

private:
	COleDateTime m_dateTask;
	CPosTrayTask& m_Task;

private:
	CString m_strEmailFolder;
	CString m_strExportFolderTarget;
	CString m_strExportFolderPending;
	CString m_strPrintFolder;
	CString m_strPrintFilenamePrefix;
	int m_nNextPrintFileNumber;
};

/**********************************************************************/
#endif
/**********************************************************************/
