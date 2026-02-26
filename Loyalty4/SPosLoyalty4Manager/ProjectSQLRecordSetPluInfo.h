/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRecordSetPluInfo.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetPluInfo : public CSQLRecordSetPluInfo
{
public:
	CProjectSQLRecordSetPluInfo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

