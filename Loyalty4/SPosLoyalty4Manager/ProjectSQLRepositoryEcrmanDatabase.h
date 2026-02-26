/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
/**********************************************************************/

class CProjectSQLRepositoryEcrmanDatabase : public CSQLRepositoryEcrmanDatabase
{
public:
	CProjectSQLRepositoryEcrmanDatabase();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

