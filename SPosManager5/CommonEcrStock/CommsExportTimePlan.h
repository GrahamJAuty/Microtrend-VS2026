#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

class CCommsExportTimePlan : public CCommsExportBase
{
public:
	CCommsExportTimePlan( CJobListCSVRecord& JobInfo, bool bFromMemory );
	int PrepareFile();

private:
	int PrepareSPOSFile( int nSPOSVersion );

private:
	void AddPromotionTemplatePlans(int nSPOVersion, CSQLOut& SQLOut, std::array<int, TIMEPLAN_MAX_PLANS + 1>& nMaxEntry);
	void AddMixMatchTemplatePlans(int nSPOVersion, CSQLOut& SQLOut, std::array<int, TIMEPLAN_MAX_PLANS + 1>& nMaxEntry);
	void AddSQLTimeplanEntry(bool bTemplate, CSQLOut& SQLOut, CTimePlanEntriesCSVRecord& Entry);
};

/**********************************************************************/
#endif
/**********************************************************************/
