/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleRecentTransaction\SQLRepositoryPebbleRecentTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleRecentTransaction : public CSQLRepositoryPebbleRecentTransaction
{
public:
	CProjectSQLRepositoryPebbleRecentTransaction();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

