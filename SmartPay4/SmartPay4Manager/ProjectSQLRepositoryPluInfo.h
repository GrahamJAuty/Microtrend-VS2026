/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
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

