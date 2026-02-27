/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRepositoryBromComTransaction.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryBromComTransaction : public CSQLRepositoryBromComTransaction
{
public:
	CProjectSQLRepositoryBromComTransaction();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

