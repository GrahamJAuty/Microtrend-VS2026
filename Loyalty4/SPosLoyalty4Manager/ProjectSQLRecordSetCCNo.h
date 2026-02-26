/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRecordSetCCNo.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetCCNo : public CSQLRecordSetCCNo	
{
public:
	CProjectSQLRecordSetCCNo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

