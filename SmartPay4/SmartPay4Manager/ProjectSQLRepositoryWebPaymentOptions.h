/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryWebPaymentOptions : public CSQLRepositoryWebPaymentOptions
{
public:
	CProjectSQLRepositoryWebPaymentOptions();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

