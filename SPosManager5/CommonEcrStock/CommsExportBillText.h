#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportBillText : public CCommsExportBase
{
public:
	CCommsExportBillText( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile();
};

/**********************************************************************/
#endif
/**********************************************************************/

