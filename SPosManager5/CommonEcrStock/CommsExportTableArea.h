#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SPosVersions.h"
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportTableArea : public CCommsExportBase
{
public:
	CCommsExportTableArea( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

