#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
#include "PosTrayEposConsolParamsArray.h"
/**********************************************************************/

class CPosTrayReportCreatorEpos
{
public:
	CPosTrayReportCreatorEpos( COleDateTime& dateTask, CPosTrayTask& Task );
	~CPosTrayReportCreatorEpos(void);

public:
	bool ProcessTask();
	int GetSkippedReportCountEmpty() { return m_nSkippedReportsEmpty; }
	int GetSkippedReportCountNoChange() { return m_nSkippedReportsNoChange; }

private:
	bool ProcessNormalReports();
	bool ProcessQuickReports();

private:
	bool CreateReport( int nReportType, int nConLevel, CString& strTitle, CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport, CPosTrayTaskConsolInfo& infoQuick );
	bool CreateReportInternal( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle, bool bFixStartDate, CPosTrayTaskConsolInfo& infoQuick );
	bool ProcessTaskInternal();

private:
	int m_nDeptFilter;
	int m_nPluFilter;
	bool m_bSaleTime;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	COleDateTime m_oleFutureDateTo;
	CSessionCSVRecord m_ReportSession;
	CEposSelectArray m_SelectArray;

private:
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
	
private:
	COleDateTime m_dateTask;
	CPosTrayEposConsolParamsArray m_ParamsArray;
	CEposReportSelect m_EposReportSelect;
};

/**********************************************************************/
#endif
/**********************************************************************/
