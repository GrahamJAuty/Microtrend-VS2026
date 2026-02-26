#pragma once
/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranScriptImport : public CSQLTranBase
{
public:
	CSQLTranScriptImport();

public:
	void DoWork(bool bUseFallbackScript);

public:
	bool GetBadDbVersionFlag() { return m_bBadDbVersion; }

private:
	bool CheckDbVersion(CSSFile& fileIn);
	bool ImportAccounts(CSSFile& fileIn);
	bool ImportGroups(CSSFile& fileIn, bool bGotGroupTopupBonuses);
	bool ImportGroupTopupBonuses(CSSFile& fileIn, bool& bGotGroupTopupBonuses);
	bool ImportExternalAccounts(CSSFile& fileIn);
	bool ImportPluInfo(CSSFile& fileIn);
	bool ImportDeptInfo(CSSFile& fileIn);
	bool ImportStaffCards(CSSFile& fileIn);
	bool ImportStaffGiftExceptions(CSSFile& fileIn);
	bool ImportCCNo(CSSFile& fileIn);
	bool ImportAuditPeriods(CSSFile& fileIn);
	bool ImportOfferStamps(CSSFile& fileIn);
	bool ImportStampOfferInfo(CSSFile& fileIn);
	bool ImportEcrmanDatabases(CSSFile& fileIn);
	bool ImportSchemes(CSSFile& fileIn);

private:
	int m_nIncomingDbVersion;
	bool m_bBadDbVersion;

private:
	CWorkingDlg m_WorkingDlg;
};

/**********************************************************************/
