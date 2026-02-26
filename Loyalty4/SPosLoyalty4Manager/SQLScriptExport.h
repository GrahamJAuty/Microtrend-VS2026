#pragma once
//**********************************************************************
#include "WorkingDlg.h"
//**********************************************************************

class CSQLScriptExport
{
public:
	CSQLScriptExport();

public:
	void DoExport();

private:
	void WriteFileBegin(CSSFile& fileOut);
	void ExportDbVersion(CSSFile& fileOut);
	void ExportAccounts(CSSFile& fileOut);
	void ExportGroups(CSSFile& fileOut);
	void ExportGroupTopupBonuses(CSSFile& fileOut);
	void ExportExternalAccounts(CSSFile& fileOut);
	void ExportDeptInfo(CSSFile& fileOut);
	void ExportPluInfo(CSSFile& fileOut);
	void ExportStaffCards(CSSFile& fileOut);
	void ExportStaffGiftExceptions(CSSFile& fileOut);
	void ExportCCNo(CSSFile& fileOut);
	void ExportAuditPeriods(CSSFile& fileOut);
	void ExportOfferStamps(CSSFile& fileOut);
	void ExportStampOfferInfo(CSSFile& fileOut);
	void ExportEcrmanDatabases(CSSFile& fileOut);
	void ExportSchemes(CSSFile& fileOut);
	void WriteFileEnd(CSSFile& fileOut);

private:
	CWorkingDlg m_WorkingDlg;
};

//**********************************************************************
