/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BromComItem\SQLRepositoryBromComItem.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryBromComItem : public CSQLRepositoryBromComItem
{
public:
	CProjectSQLRepositoryBromComItem();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

