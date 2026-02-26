/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/

class CProjectSQLRepositoryGroup : public CSQLRepositoryGroup
{
public:
	CProjectSQLRepositoryGroup();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, bool bGotTopupBonuses, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

