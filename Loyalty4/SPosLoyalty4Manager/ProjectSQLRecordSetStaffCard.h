/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRecordSetStaffCard.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetStaffCard : public CSQLRecordSetStaffCard
{
public:
	CProjectSQLRecordSetStaffCard();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

