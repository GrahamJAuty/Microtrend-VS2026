/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRepositoryBromComPendingOrder.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryBromComPendingOrder : public CSQLRepositoryBromComPendingOrder
{
public:
	CProjectSQLRepositoryBromComPendingOrder();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

