/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryGroup : public CSQLRepositoryGroup
{
public:
	CProjectSQLRepositoryGroup();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

