/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleLinkTime\SQLRecordSetPebbleLinkTime.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleLinkTime : public CSQLRecordSetPebbleLinkTime
{
public:
	CProjectSQLRecordSetPebbleLinkTime();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

