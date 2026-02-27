/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CProjectSQLRepositoryUserText : public CSQLRepositoryUserText
{
public:
	CProjectSQLRepositoryUserText();

public:
	bool ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg);

private:
	CString m_strImportTableName = "";
};

/**********************************************************************/

