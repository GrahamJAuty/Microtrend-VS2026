/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRecordSetAuditPeriod.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetAuditPeriod : public CSQLRecordSetAuditPeriod
{
public:
	CProjectSQLRecordSetAuditPeriod();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

