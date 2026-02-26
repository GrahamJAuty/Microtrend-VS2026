/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRepositoryAuditPeriod.h"
/**********************************************************************/

class CProjectSQLRepositoryAuditPeriod : public CSQLRepositoryAuditPeriod
{
public:
	CProjectSQLRepositoryAuditPeriod();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

