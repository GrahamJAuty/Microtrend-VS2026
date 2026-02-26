#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportAllergen : public CCommsExportBase
{
public:
	CCommsExportAllergen( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

