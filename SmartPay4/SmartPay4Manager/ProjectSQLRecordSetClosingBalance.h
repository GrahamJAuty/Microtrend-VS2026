/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRecordSetClosingBalance.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetClosingBalance : public CSQLRecordSetClosingBalance
{
public:
	CProjectSQLRecordSetClosingBalance();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

