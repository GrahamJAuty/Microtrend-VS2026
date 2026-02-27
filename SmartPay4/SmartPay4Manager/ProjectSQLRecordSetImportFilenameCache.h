/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_ImportFilenameCache\SQLRecordSetImportFilenameCache.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetImportFilenameCache : public CSQLRecordSetImportFilenameCache
{
public:
	CProjectSQLRecordSetImportFilenameCache();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

