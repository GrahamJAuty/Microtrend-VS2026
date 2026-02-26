#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SPosVersions.h"
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportTableName : public CCommsExportBase
{
public:
	CCommsExportTableName( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

