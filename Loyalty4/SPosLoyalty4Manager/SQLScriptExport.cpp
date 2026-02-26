//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRecordSetScheme.h"
//**********************************************************************
#include "ProjectSQLRecordSetAccountFull.h"
#include "ProjectSQLRecordSetAuditPeriod.h"
#include "ProjectSQLRecordSetCCNo.h"
#include "ProjectSQLRecordSetDbVersion.h"
#include "ProjectSQLRecordSetDeptInfo.h"
#include "ProjectSQLRecordSetEcrmanDatabase.h"
#include "ProjectSQLRecordSetExternalAccount.h"
#include "ProjectSQLRecordSetGroup.h"
#include "ProjectSQLRecordSetGroupTopupBonus.h"
#include "ProjectSQLRecordSetOfferStamp.h"
#include "ProjectSQLRecordSetPluInfo.h"
#include "ProjectSQLRecordSetScheme.h"
#include "ProjectSQLRecordSetStaffCard.h"
#include "ProjectSQLRecordSetStaffGiftException.h"	
#include "ProjectSQLRecordSetStampOfferInfo.h"	
#include "SQLOut.h"
//**********************************************************************
#include "SQLScriptExport.h"
//**********************************************************************

CSQLScriptExport::CSQLScriptExport() : m_WorkingDlg("Preparing SQL Script for Backup", FALSE)
{
	m_WorkingDlg.Create();
}

//**********************************************************************

void CSQLScriptExport::DoExport()
{
	CSSFile fileOut;
	fileOut.Open(Filenames.GetSQLScriptFallbackFilename(), "wb");
	WriteFileBegin(fileOut);
	ExportDbVersion(fileOut);
	ExportAccounts(fileOut);
	ExportGroups(fileOut);
	ExportGroupTopupBonuses(fileOut);
	ExportExternalAccounts(fileOut);
	ExportPluInfo(fileOut);
	ExportDeptInfo(fileOut);
	ExportStaffCards(fileOut);
	ExportStaffGiftExceptions(fileOut);
	ExportCCNo(fileOut);
	ExportAuditPeriods(fileOut);
	ExportOfferStamps(fileOut);
	ExportStampOfferInfo(fileOut);
	ExportEcrmanDatabases(fileOut);
	ExportSchemes(fileOut);
	WriteFileEnd(fileOut);
}

//**********************************************************************

void CSQLScriptExport::WriteFileBegin(CSSFile& fileOut)
{
	CStringArray arrayDummy;
	CSQLOut SQLOut(fileOut, "Dummy", arrayDummy);
	SQLOut.WriteFileBegin();
}

//**********************************************************************

void CSQLScriptExport::ExportDbVersion(CSSFile& fileOut)
{
	CProjectSQLRecordSetDbVersion RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportAccounts(CSSFile& fileOut)
{
	CProjectSQLRecordSetAccountFull RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportGroups(CSSFile& fileOut)
{
	CProjectSQLRecordSetGroup RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportGroupTopupBonuses(CSSFile& fileOut)
{
	CProjectSQLRecordSetGroupTopupBonus RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportExternalAccounts(CSSFile& fileOut)
{
	CProjectSQLRecordSetExternalAccount RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPluInfo(CSSFile& fileOut)
{
	CProjectSQLRecordSetPluInfo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportDeptInfo(CSSFile& fileOut)
{
	CProjectSQLRecordSetDeptInfo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportStaffCards(CSSFile& fileOut)
{
	CProjectSQLRecordSetStaffCard RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportStaffGiftExceptions(CSSFile& fileOut)
{
	CProjectSQLRecordSetStaffGiftException RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportCCNo(CSSFile& fileOut)
{
	CProjectSQLRecordSetCCNo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportAuditPeriods(CSSFile& fileOut)
{
	CProjectSQLRecordSetAuditPeriod RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportOfferStamps(CSSFile& fileOut)
{
	CProjectSQLRecordSetOfferStamp RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportStampOfferInfo(CSSFile& fileOut)
{
	CProjectSQLRecordSetStampOfferInfo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);	
}

//**********************************************************************

void CSQLScriptExport::ExportEcrmanDatabases(CSSFile& fileOut)
{
	CProjectSQLRecordSetEcrmanDatabase RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportSchemes(CSSFile& fileOut)
{
	CProjectSQLRecordSetScheme RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::WriteFileEnd(CSSFile& fileOut)
{
	CStringArray arrayDummy;
	CSQLOut SQLOut(fileOut, "Dummy", arrayDummy);
	SQLOut.WriteFileEnd();
}

//**********************************************************************
