/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPurchaseControlRule : public CSQLRepositoryPurchaseControlRule
{
public:
	CProjectSQLRepositoryPurchaseControlRule();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

