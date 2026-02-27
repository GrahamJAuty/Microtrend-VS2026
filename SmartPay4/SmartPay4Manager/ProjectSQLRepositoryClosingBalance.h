/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryClosingBalance : public CSQLRepositoryClosingBalance
{
public:
	CProjectSQLRepositoryClosingBalance();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

