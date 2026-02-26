#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportBarcode : public CCommsExportBase
{
public:
	CCommsExportBarcode( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

