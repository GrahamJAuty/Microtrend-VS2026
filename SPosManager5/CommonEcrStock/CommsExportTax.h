#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportTax : public CCommsExportBase
{
public:
	CCommsExportTax( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

