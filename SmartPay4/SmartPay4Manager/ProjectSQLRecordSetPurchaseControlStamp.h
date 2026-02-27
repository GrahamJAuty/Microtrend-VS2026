/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRecordSetPurchaseControlStamp.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPurchaseControlStamp : public CSQLRecordSetPurchaseControlStamp
{
public:
	CProjectSQLRecordSetPurchaseControlStamp();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

