/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPINNumber : public CSQLRepositoryPINNumber
{
public:
	CProjectSQLRepositoryPINNumber();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

