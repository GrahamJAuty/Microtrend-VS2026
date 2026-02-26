#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
#include "JobListCSVArray.h"
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
#include "..\SPosStockManager5\PluMonitorStockman.h"
/**********************************************************************/

class CCommsExportPlu : public CCommsExportBase
{
public:
	CCommsExportPlu( CJobListCSVArray& JobArray, CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile( int nParam1 );
	int GetKPImage( __int64 nSSPluNo, CString& strImageData, CString& strImageFilename );

public:
	void RenameTempFile();

private:
	int PrepareFileInternal( int nParam1, int nDbIdx );

#ifdef STOCKMAN_SYSTEM
	int PrepareSPOSFile( int nSPOSVersion, CPluMonitorStockman& PluMonitor, int nParam1 );
#else
	int PrepareSPOSFile( int nSPOSVersion, CPluMonitorEcrman& PluMonitor, int nParam1 );
#endif
	
private:
	CString m_strEffectiveDate;
	int m_nPriceSet;
};

/**********************************************************************/
#endif
/**********************************************************************/

