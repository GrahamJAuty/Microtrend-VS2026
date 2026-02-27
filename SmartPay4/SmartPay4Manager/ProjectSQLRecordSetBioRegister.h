/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRecordSetBioRegister.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"	
/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/

class CProjectSQLRecordSetBioRegister : public CSQLRecordSetBioRegister
{
public:
	CProjectSQLRecordSetBioRegister();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

