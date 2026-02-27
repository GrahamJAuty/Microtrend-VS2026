/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRecordSetWebPaymentOptions.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetWebPaymentOptions : public CSQLRecordSetWebPaymentOptions
{
public:
	CProjectSQLRecordSetWebPaymentOptions();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

