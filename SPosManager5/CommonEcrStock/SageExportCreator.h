/**********************************************************************/
#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define SAGE_EXPORT_ERROR_NONE 0
#define SAGE_EXPORT_ERROR_LOCK 1
#define SAGE_EXPORT_ERROR_CONFIG 2
#define SAGE_EXPORT_ERROR_PAUSE 3
#define SAGE_EXPORT_ERROR_NETWORK_NOPATH 4
#define SAGE_EXPORT_ERROR_NETWORK_LOCK 5
#define SAGE_EXPORT_ERROR_READ_SALES 6
#define SAGE_EXPORT_ERROR_COPY_SALES 7
#define SAGE_EXPORT_ERROR_DATABASE_OPEN 8
#define SAGE_EXPORT_ERROR_SOURCEFILE_OPEN 9
#define SAGE_EXPORT_ERROR_DESTFILE_DELETE 10
#define SAGE_EXPORT_ERROR_DESTFILE_WRITE 11
#define SAGE_EXPORT_ERROR_DESTFILE_RENAME 12
#define SAGE_EXPORT_ERROR_NODATA 13
#define SAGE_EXPORT_ERROR_NOCODES 14
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CSageTermFileInfo
{
	int m_nDbIdx;
	int m_nNetworkIdx;
	int m_nLocIdx;
	bool m_bStartDate;
	bool m_bLegacySales;
	CString m_strDate;
	CString m_strFilename;
};

/**********************************************************************/

struct CEposReportConsolSageGroup
{
public:
	CEposReportConsolSageGroup();
	void Reset();

public:
	int Compare ( CEposReportConsolSageGroup& source, int nHint = 0 );
	void Add	( CEposReportConsolSageGroup& source );
	
public:
	bool m_bPromo;
	CString m_strLineID;
	int m_nTaxDateCode;
	int m_nTaxBand;
	
public:
	double m_dVal;
	double m_dNonTax;
};

/**********************************************************************/

class CSageExportCreator
{
public:
	CSageExportCreator( COleDateTime& date );

public:
	int CreateExport();

private:
	int GetFileList();
	int ConsolidateSales( CSageTermFileInfo& infoFile );
	int WriteSales();

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
	CArray<CSageTermFileInfo,CSageTermFileInfo> m_arrayFiles;
	CReportConsolidationArray<CEposReportConsolSageGroup> m_arraySales;
};

/**********************************************************************/
#endif
/**********************************************************************/
