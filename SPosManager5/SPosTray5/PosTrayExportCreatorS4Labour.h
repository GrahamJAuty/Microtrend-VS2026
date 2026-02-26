#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "S4LabourLocationOptions.h"
#include "PosTrayTaskInfo.h"
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

class CPosTrayExportCreatorS4Labour
{
public:
	CPosTrayExportCreatorS4Labour( CPosTrayS4LabourScheduleInfo& infoExport, CS4LabourLocationOptions& Options, bool& bQuitNow );
	~CPosTrayExportCreatorS4Labour(void);

public:
	void SetExportDate( COleDateTime& date );
	__int64 ProcessTask();
	
private:
	__int64 ProcessTaskInternal();

private:
	CPosTrayS4LabourScheduleInfo& m_infoExport;
	CS4LabourLocationOptions& m_Options;

private:
	COleDateTime m_dateExport;
	CSessionCSVRecord m_ReportSession;
	CEposSelectArray m_SelectArray;

private:
	CString m_strExportFolder;

private:
	bool& m_bQuitNow;
};

/**********************************************************************/
#endif
/**********************************************************************/
