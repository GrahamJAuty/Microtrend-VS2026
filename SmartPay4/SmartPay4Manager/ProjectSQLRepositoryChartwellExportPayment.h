/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ChartwellExportPayment\SQLRepositoryChartwellExportPayment.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryChartwellExportPayment : public CSQLRepositoryChartwellExportPayment
{
public:
	CProjectSQLRepositoryChartwellExportPayment();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

