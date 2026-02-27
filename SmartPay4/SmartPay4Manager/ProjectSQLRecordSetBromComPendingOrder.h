/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRecordSetBromComPendingOrder.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBromComPendingOrder : public CSQLRecordSetBromComPendingOrder
{
public:
	CProjectSQLRecordSetBromComPendingOrder();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

