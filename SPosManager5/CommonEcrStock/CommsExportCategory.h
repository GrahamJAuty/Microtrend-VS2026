#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportCategory : public CCommsExportBase
{
public:
	CCommsExportCategory( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

