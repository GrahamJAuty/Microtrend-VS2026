/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRecordSetGroupTopupBonus.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetGroupTopupBonus : public CSQLRecordSetGroupTopupBonus
{
public:
	CProjectSQLRecordSetGroupTopupBonus();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

