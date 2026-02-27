/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRecordSetPluInfo.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetPluInfo : public CSQLRecordSetPluInfo
{
public:
	CProjectSQLRecordSetPluInfo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

