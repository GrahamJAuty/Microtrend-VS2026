/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRepositoryPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRepositoryPebbleExportTransactionSent.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleExportTransaction : public CSQLRepositoryPebbleExportTransaction
{
public:
	CProjectSQLRepositoryPebbleExportTransaction();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransaction, CSQLRowPebbleExportTransaction&>& arrayPebbleExportTransaction, CWorkingDlg& WorkingDlg);
	bool ProcessImportedTransactions(CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransaction, CSQLRowPebbleExportTransaction&>& arrayPebbleExportTransaction, CArray<CSQLRowPebbleExportTransactionSent, CSQLRowPebbleExportTransactionSent&>& arrayPebbleExportTransactionSent);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

