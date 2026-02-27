/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ChartwellExportPayment\SQLRecordSetChartwellExportPayment.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetChartwellExportPayment : public CSQLRecordSetChartwellExportPayment
{
public:
	CProjectSQLRecordSetChartwellExportPayment();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

