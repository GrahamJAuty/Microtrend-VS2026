/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRecordSetPebbleExportTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleExportTransaction : public CSQLRecordSetPebbleExportTransaction
{
public:
	CProjectSQLRecordSetPebbleExportTransaction();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

