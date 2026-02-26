/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_DeptInfo\SQLRepositoryDeptInfo.h"
/**********************************************************************/

class CProjectSQLRepositoryDeptInfo : public CSQLRepositoryDeptInfo
{
public:
	CProjectSQLRepositoryDeptInfo();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

