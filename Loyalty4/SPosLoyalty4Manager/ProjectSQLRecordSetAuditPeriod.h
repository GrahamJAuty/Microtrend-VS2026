/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRecordSetAuditPeriod.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetAuditPeriod : public CSQLRecordSetAuditPeriod	
{
public:
	CProjectSQLRecordSetAuditPeriod();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

