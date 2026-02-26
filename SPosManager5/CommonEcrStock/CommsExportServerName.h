#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportServerName : public CCommsExportBase
{
public:
	CCommsExportServerName( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

