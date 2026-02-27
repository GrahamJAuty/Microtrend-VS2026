/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRecordSetPebblePaymentType.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebblePaymentType : public CSQLRecordSetPebblePaymentType
{
public:
	CProjectSQLRecordSetPebblePaymentType();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

