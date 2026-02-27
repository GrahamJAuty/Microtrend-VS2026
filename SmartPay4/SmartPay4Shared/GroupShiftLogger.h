#pragma once
//$$******************************************************************
#include "SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "Consolidation.h"
#include "GroupShiftValidator.h"
#include "ReportConsolidationArray.h"
//$$******************************************************************

class CGroupShiftLogger
{
public:
	CGroupShiftLogger();

public:
	void LogManualChange(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, int nManualType);
	void LogSystemChange(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator);

	
private:
	void LogChangeInternal(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, int nManualType);

private:
	CReportConsolidationArray<CSortedStringByInt64> m_logCache;
};

//$$******************************************************************
extern CGroupShiftLogger GroupShiftLogger;
//$$******************************************************************
