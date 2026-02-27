/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRecordSetPebbleConfig.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPebbleConfig : public CSQLRecordSetPebbleConfig
{
public:
	CProjectSQLRecordSetPebbleConfig();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

