/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
/**********************************************************************/

class CProjectSQLRepositoryStampOfferInfo : public CSQLRepositoryStampOfferInfo
{
public:
	CProjectSQLRepositoryStampOfferInfo();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

