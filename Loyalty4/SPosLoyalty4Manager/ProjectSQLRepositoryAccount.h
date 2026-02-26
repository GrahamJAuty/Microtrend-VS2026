/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/

class CProjectSQLRepositoryAccount : public CSQLRepositoryAccount
{
public:
	CProjectSQLRepositoryAccount();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

