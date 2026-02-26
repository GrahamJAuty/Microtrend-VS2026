/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRecordSetGroup.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetGroup : public CSQLRecordSetGroup
{
public:
	CProjectSQLRecordSetGroup();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

