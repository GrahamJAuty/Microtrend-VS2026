/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRecordSetOfferStamp.h"
/**********************************************************************/
#include "SQLOut.h"
#include "WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRecordSetOfferStamp : public CSQLRecordSetOfferStamp	
{
public:
	CProjectSQLRecordSetOfferStamp();

public:
	void ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg);
};

/**********************************************************************/

