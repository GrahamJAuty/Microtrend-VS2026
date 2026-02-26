#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersReports : public CDataAccessHelpers
{
public:
	CDataAccessHelpersReports();

public:
	void SalesHistoryPlu( int nPreset, bool bAutoRunPreset );
	void EposReports( int nPreset, bool bPresetPrint, bool bAutoRunPreset );
	void CustomerTransactions( int nPreset, bool bAutoRunPreset );
	void TimeAttendance( int nPreset, bool bAutoRunPreset );

#ifdef STOCKMAN_SYSTEM
	void SalesHistoryStock( int nPreset, bool bAutoRunPreset );
#else
	void EcrmanStockLevel( int nPreset, bool bAutoRunPreset );
#endif

};

/**********************************************************************/
#endif
/**********************************************************************/
