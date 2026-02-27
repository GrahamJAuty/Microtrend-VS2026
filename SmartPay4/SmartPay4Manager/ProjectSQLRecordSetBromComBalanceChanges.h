/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComBalanceChanges\SQLRecordSetBromComBalanceChanges.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBromComBalanceChanges : public CSQLRecordSetBromComBalanceChanges
{
public:
	CProjectSQLRecordSetBromComBalanceChanges();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

