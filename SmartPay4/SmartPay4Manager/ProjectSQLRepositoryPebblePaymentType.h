/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRepositoryPebblePaymentType.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebblePaymentType : public CSQLRepositoryPebblePaymentType
{
public:
	CProjectSQLRepositoryPebblePaymentType();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

