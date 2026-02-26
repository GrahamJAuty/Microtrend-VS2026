#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportDept : public CCommsExportBase
{
public:
	CCommsExportDept( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

