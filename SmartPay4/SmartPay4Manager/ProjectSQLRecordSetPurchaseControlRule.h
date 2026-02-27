/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRecordSetPurchaseControlRule.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPurchaseControlRule : public CSQLRecordSetPurchaseControlRule
{
public:
	CProjectSQLRecordSetPurchaseControlRule();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

