/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRecordSetPebbleExportTransactionSent.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleExportTransactionSent : public CSQLRecordSetPebbleExportTransactionSent
{
public:
	CProjectSQLRecordSetPebbleExportTransactionSent();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

