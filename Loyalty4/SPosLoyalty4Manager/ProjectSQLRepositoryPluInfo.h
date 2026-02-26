/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/

class CProjectSQLRepositoryPluInfo : public CSQLRepositoryPluInfo
{
public:
	CProjectSQLRepositoryPluInfo();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

