#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportGroup : public CCommsExportBase
{
public:
	CCommsExportGroup( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();
	
private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

