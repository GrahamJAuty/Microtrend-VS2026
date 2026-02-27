/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ImportFilenameCache\SQLRepositoryImportFilenameCache.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryImportFilenameCache : public CSQLRepositoryImportFilenameCache
{
public:
	CProjectSQLRepositoryImportFilenameCache();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

