/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
/**********************************************************************/

class CProjectSQLRepositoryStaffCard : public CSQLRepositoryStaffCard
{
public:
	CProjectSQLRepositoryStaffCard();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

