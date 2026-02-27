/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryExternalAccount : public CSQLRepositoryExternalAccount
{
public:
	CProjectSQLRepositoryExternalAccount();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

