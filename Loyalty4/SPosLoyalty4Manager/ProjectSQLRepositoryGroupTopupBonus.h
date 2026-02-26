/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
/**********************************************************************/

class CProjectSQLRepositoryGroupTopupBonus : public CSQLRepositoryGroupTopupBonus
{
public:
	CProjectSQLRepositoryGroupTopupBonus();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, bool& bGotTopupBonuses, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

