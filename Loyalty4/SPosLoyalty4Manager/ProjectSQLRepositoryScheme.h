/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRepositoryScheme.h"
/**********************************************************************/

class CProjectSQLRepositoryScheme : public CSQLRepositoryScheme
{
public:
	CProjectSQLRepositoryScheme();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

