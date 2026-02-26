#pragma once
/**********************************************************************/
#include "PosTrayTask.h"
#include "PosTrayTaskInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CPosTrayFileSet
{
public:
	void GetPosTrayFileSet( int nType );
	bool IsExportTask();
	
public:
	int m_nTaskFileType;
	int m_nBatchFileType;
	int m_nAllFileType;
	SysFileInfo m_infoTaskFile;
	SysFileInfo m_infoTaskTemp;
	SysFileInfo m_infoBatchFile;
	SysFileInfo m_infoBatchTemp;
	SysFileInfo m_infoDateFile;
	SysFileInfo m_infoDateTemp;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_pArrayTaskInfo;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_pArrayBatchInfo;
};

/**********************************************************************/
