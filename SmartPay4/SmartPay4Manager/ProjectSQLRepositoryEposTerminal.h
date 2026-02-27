/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryEposTerminal : public CSQLRepositoryEposTerminal
{
public:
	CProjectSQLRepositoryEposTerminal();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

