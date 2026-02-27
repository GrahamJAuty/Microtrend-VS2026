/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRecordSetExternalAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetExternalAccount : public CSQLRecordSetExternalAccount
{
public:
	CProjectSQLRecordSetExternalAccount();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

