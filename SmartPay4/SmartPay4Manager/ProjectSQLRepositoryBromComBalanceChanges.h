/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComBalanceChanges\SQLRepositoryBromComBalanceChanges.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryBromComBalanceChanges : public CSQLRepositoryBromComBalanceChanges
{
public:
	CProjectSQLRepositoryBromComBalanceChanges();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

