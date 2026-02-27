/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleConfig : public CSQLRepositoryPebbleConfig
{
public:
	CProjectSQLRepositoryPebbleConfig();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

