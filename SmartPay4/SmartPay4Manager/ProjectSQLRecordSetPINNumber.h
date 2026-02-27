/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRecordSetPINNumber.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPINNumber : public CSQLRecordSetPINNumber
{
public:
	CProjectSQLRecordSetPINNumber();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

