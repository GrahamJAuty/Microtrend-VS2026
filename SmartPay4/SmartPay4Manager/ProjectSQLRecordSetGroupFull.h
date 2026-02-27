/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Group\SQLRecordSetGroupFull.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetGroupFull : public CSQLRecordSetGroupFull
{
public:
	CProjectSQLRecordSetGroupFull();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

