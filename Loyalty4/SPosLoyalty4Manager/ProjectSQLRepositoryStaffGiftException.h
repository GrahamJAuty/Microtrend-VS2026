/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
/**********************************************************************/

class CProjectSQLRepositoryStaffGiftException : public CSQLRepositoryStaffGiftException
{
public:
	CProjectSQLRepositoryStaffGiftException();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

