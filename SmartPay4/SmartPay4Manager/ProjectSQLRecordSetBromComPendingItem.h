/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComPendingItem\SQLRecordSetBromComPendingItem.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBromComPendingItem : public CSQLRecordSetBromComPendingItem
{
public:
	CProjectSQLRecordSetBromComPendingItem();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

