/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_DeptInfo\SQLRecordSetDeptInfo.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetDeptInfo : public CSQLRecordSetDeptInfo
{
public:
	CProjectSQLRecordSetDeptInfo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

