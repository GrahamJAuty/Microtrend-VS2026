/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComItem\SQLRecordSetBromComItem.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBromComItem : public CSQLRecordSetBromComItem
{
public:
	CProjectSQLRecordSetBromComItem();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

