/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRecordSetScheme.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetScheme : public CSQLRecordSetScheme	
{
public:
	CProjectSQLRecordSetScheme();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

