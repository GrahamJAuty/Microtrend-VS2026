/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_CCNo\SQLRecordSetCCNo.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetCCNo : public CSQLRecordSetCCNo
{
public:
	CProjectSQLRecordSetCCNo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

