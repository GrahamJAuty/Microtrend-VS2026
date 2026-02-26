#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportMacro : public CCommsExportBase
{
public:
	CCommsExportMacro( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nDbIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/

