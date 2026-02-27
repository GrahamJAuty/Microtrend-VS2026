/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRecordSetUserText.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetUserText : public CSQLRecordSetUserText
{
public:
	CProjectSQLRecordSetUserText();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

