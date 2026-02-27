/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRecordSetEposTerminal.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetEposTerminal : public CSQLRecordSetEposTerminal
{
public:
	CProjectSQLRecordSetEposTerminal();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

