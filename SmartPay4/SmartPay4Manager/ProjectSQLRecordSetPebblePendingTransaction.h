/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRecordSetPebblePendingTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebblePendingTransaction : public CSQLRecordSetPebblePendingTransaction
{
public:
	CProjectSQLRecordSetPebblePendingTransaction(bool bUnmatched);

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

