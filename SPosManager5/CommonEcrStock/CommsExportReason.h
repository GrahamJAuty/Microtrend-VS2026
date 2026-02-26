#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportReason : public CCommsExportBase
{
public:
	CCommsExportReason( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nDbIdx );

private:
	int m_nParam1;
};

/**********************************************************************/
#endif
/**********************************************************************/

