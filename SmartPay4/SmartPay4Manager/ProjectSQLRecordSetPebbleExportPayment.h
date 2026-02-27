/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRecordSetPebbleExportPayment.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleExportPayment : public CSQLRecordSetPebbleExportPayment
{
public:
	CProjectSQLRecordSetPebbleExportPayment();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

