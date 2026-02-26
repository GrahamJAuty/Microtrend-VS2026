#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportFunction : public CCommsExportBase
{
public:
	CCommsExportFunction( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nDbIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/

