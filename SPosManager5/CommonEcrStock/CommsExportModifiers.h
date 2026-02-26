#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportModifiers : public CCommsExportBase
{
public:
	CCommsExportModifiers( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

