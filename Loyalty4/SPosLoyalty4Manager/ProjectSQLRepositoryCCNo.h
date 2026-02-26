/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
/**********************************************************************/

class CProjectSQLRepositoryCCNo : public CSQLRepositoryCCNo
{
public:
	CProjectSQLRepositoryCCNo();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

