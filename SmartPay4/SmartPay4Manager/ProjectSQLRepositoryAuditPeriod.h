/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRepositoryAuditPeriod.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryAuditPeriod : public CSQLRepositoryAuditPeriod
{
public:
	CProjectSQLRepositoryAuditPeriod();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

