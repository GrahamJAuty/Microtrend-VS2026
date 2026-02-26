/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRecordSetExternalAccount.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetExternalAccount : public CSQLRecordSetExternalAccount
{
public:
	CProjectSQLRecordSetExternalAccount();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

