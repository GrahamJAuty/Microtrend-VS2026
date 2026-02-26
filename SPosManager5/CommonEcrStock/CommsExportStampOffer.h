#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportStampOffer : public CCommsExportBase
{
public:
	CCommsExportStampOffer( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

