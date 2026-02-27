/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRepositoryPebbleExportTransactionSent.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleExportTransactionSent : public CSQLRepositoryPebbleExportTransactionSent
{
public:
	CProjectSQLRepositoryPebbleExportTransactionSent();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransactionSent, CSQLRowPebbleExportTransactionSent&>& arrayPebbleExportTransactionSent, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

