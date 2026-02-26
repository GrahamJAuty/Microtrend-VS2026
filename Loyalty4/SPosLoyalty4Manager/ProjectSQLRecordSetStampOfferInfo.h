/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRecordSetStampOfferInfo.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetStampOfferInfo : public CSQLRecordSetStampOfferInfo	
{
public:
	CProjectSQLRecordSetStampOfferInfo();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

