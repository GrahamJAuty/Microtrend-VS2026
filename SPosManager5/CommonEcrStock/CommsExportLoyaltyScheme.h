#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportLoyaltyScheme : public CCommsExportBase
{
public:
	CCommsExportLoyaltyScheme( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/

