/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRecordSetBromComTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBromComTransaction : public CSQLRecordSetBromComTransaction
{
public:
	CProjectSQLRecordSetBromComTransaction();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

