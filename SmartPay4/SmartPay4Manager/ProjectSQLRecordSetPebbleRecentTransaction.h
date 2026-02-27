/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleRecentTransaction\SQLRecordSetPebbleRecentTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleRecentTransaction : public CSQLRecordSetPebbleRecentTransaction
{
public:
	CProjectSQLRecordSetPebbleRecentTransaction();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

