/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRepositoryPebbleExportPayment.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleExportPayment : public CSQLRepositoryPebbleExportPayment
{
public:
	CProjectSQLRepositoryPebbleExportPayment();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

