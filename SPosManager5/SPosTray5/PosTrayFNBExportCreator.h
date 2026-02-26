#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "EposSelectArray.h"
#include "FNBDatabaseOptions.h"
#include "PosTrayTaskInfo.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CPosTrayFNBExportCreator
{
public:
	CPosTrayFNBExportCreator( CPosTrayFNBScheduleInfo& infoExport, CFNBDatabaseOptions& Options );
	~CPosTrayFNBExportCreator(void);

public:
	bool ProcessExport( COleDateTime& dateReport );

private:
	void GetFileList ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	bool CheckTime( CString& strTime, CString& strDate );

private:
	CPosTrayFNBScheduleInfo& m_infoExport;
	CFNBDatabaseOptions& m_Options;

private:
	CEposSelectArray m_SelectArray;
	CString m_strCheckDateFrom;
	CString m_strCheckDateTo;
	CString m_strSODTime;
};

/**********************************************************************/
#endif
/**********************************************************************/
