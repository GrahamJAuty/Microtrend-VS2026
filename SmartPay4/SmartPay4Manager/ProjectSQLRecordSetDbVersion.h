/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_DbVersion\SQLRecordSetDbVersion.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetDbVersion : public CSQLRecordSetDbVersion
{
public:
	CProjectSQLRecordSetDbVersion();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

