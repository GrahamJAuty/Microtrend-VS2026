/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPurchaseControlStamp : public CSQLRepositoryPurchaseControlStamp
{
public:
	CProjectSQLRepositoryPurchaseControlStamp();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

