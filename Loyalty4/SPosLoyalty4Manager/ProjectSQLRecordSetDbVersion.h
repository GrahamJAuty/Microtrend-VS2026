/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_DbVersion\SQLRecordSetDbVersion.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetDbVersion : public CSQLRecordSetDbVersion
{
public:
	CProjectSQLRecordSetDbVersion();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

