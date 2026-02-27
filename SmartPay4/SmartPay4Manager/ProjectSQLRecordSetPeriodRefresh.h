/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRecordSetPeriodRefresh.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPeriodRefresh : public CSQLRecordSetPeriodRefresh
{
public:
	CProjectSQLRecordSetPeriodRefresh();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

