/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRecordSetStaffGiftException.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetStaffGiftException : public CSQLRecordSetStaffGiftException	
{
public:
	CProjectSQLRecordSetStaffGiftException();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

