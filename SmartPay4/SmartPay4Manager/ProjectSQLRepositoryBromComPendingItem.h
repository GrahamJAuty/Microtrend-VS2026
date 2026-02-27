/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComPendingItem\SQLRepositoryBromComPendingItem.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryBromComPendingItem : public CSQLRepositoryBromComPendingItem
{
public:
	CProjectSQLRepositoryBromComPendingItem();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

