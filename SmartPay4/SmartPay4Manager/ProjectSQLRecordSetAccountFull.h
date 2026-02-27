/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetAccountFull : public CSQLRecordSetAccountFull
{
public:
	CProjectSQLRecordSetAccountFull();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg, int nTableType);
};

/**********************************************************************/

