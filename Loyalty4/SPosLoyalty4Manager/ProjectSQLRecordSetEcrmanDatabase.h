/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRecordSetEcrmanDatabase.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetEcrmanDatabase : public CSQLRecordSetEcrmanDatabase	
{
public:
	CProjectSQLRecordSetEcrmanDatabase();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

