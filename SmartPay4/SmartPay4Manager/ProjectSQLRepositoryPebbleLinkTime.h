/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleLinkTime\SQLRepositoryPebbleLinkTime.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryPebbleLinkTime : public CSQLRepositoryPebbleLinkTime
{
public:
	CProjectSQLRepositoryPebbleLinkTime();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

