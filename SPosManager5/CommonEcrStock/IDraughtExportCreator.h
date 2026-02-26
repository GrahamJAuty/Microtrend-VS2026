#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define IDRAUGHT_EXPORT_ERROR_NONE 0
#define IDRAUGHT_EXPORT_ERROR_LOCK 1
#define IDRAUGHT_EXPORT_ERROR_CONFIG 2
#define IDRAUGHT_EXPORT_ERROR_PAUSE 3
#define IDRAUGHT_EXPORT_ERROR_NETWORK_NOPATH 4
#define IDRAUGHT_EXPORT_ERROR_NETWORK_LOCK 5
#define IDRAUGHT_EXPORT_ERROR_ALREADY_SENT 6
#define IDRAUGHT_EXPORT_ERROR_READ_SALES 7
#define IDRAUGHT_EXPORT_ERROR_COPY_SALES 8
#define IDRAUGHT_EXPORT_ERROR_DATABASE_OPEN 9
#define IDRAUGHT_EXPORT_ERROR_DEPT_ENABLE 10
#define IDRAUGHT_EXPORT_ERROR_SOURCEFILE_OPEN 11
#define IDRAUGHT_EXPORT_ERROR_DESTFILE_DELETE 12
#define IDRAUGHT_EXPORT_ERROR_DESTFILE_APPEND 13
#define IDRAUGHT_EXPORT_ERROR_DESTFILE_RENAME 14
#define IDRAUGHT_EXPORT_ERROR_FTP_CONNECT 15
#define IDRAUGHT_EXPORT_ERROR_FTP_PUTFILE 16
#define IDRAUGHT_EXPORT_ERROR_FTP_CONFIG 17
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CIDraughtTermFileInfo
{
	int m_nDbIdx;
	int m_nNetworkIdx;
	int m_nLocIdx;
	bool m_bStartDate;
	bool m_bLegacySales;
	CString m_strFilename;
};

/**********************************************************************/

struct CIDraughtExportLineInfo
{
public:
	CIDraughtExportLineInfo();

public:
	void Reset();
	int Compare ( CIDraughtExportLineInfo& source, int nHint = 0 );
	void Add	( CIDraughtExportLineInfo& source );

public:
	int m_nHour;
	__int64 m_nPluNo;

public:
	double m_dQty;
};

/**********************************************************************/

class CIDraughtExportCreator
{
public:
	CIDraughtExportCreator( COleDateTime& date );

public:
	int CreateExport();

private:
	int GetFileList();
	int ConsolidateSales( CIDraughtTermFileInfo& infoFile );
	int WriteSales( int nLocIdx );

private:
	int CreateExportInternal();

private:
	COleDateTime m_dateFrom;
	COleDateTime m_dateTo;
	CString m_strCheckDateFrom;
	CString m_strCheckDateTo;
	CString m_strReportFilename;

private:
	__int64 m_nTotalFileSize;
	__int64 m_nProgress;
	CArray<CIDraughtTermFileInfo,CIDraughtTermFileInfo> m_arrayFiles;
	CReportConsolidationArray<CIDraughtExportLineInfo> m_arraySales;
};

/**********************************************************************/
#endif
/**********************************************************************/
