/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "ProjectSQLRepositoryAccount.h"
#include "ProjectSQLRepositoryAuditPeriod.h"
#include "ProjectSQLRepositoryCCNo.h"
#include "ProjectSQLRepositoryDeptInfo.h"
#include "ProjectSQLRepositoryEcrmanDatabase.h"
#include "ProjectSQLRepositoryExternalAccount.h"
#include "ProjectSQLRepositoryGroup.h"
#include "ProjectSQLRepositoryGroupTopupBonus.h"
#include "ProjectSQLRepositoryOfferStamp.h"
#include "ProjectSQLRepositoryPluInfo.h"
#include "ProjectSQLRepositoryScheme.h"
#include "ProjectSQLRepositoryStaffCard.h"
#include "ProjectSQLRepositoryStaffGiftException.h"
#include "ProjectSQLRepositoryStampOfferInfo.h"
/**********************************************************************/
#include "SQLIn.h"
/**********************************************************************/
#include "SQLTranScriptImport.h"
/**********************************************************************/

CSQLTranScriptImport::CSQLTranScriptImport() : CSQLTranBase(), m_WorkingDlg("Restoring Database from SQL Script", FALSE)
{
	m_WorkingDlg.Create();
	m_bBadDbVersion = FALSE;
	m_nIncomingDbVersion = 0;
}

/**********************************************************************/

void CSQLTranScriptImport::DoWork(bool bUseFallbackScript)
{
	m_nIncomingDbVersion = 0;

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;
		m_bBadDbVersion = TRUE;

		CSSFile fileIn;

		if (FALSE == bUseFallbackScript)
		{
			if (fileIn.Open(Filenames.GetSQLScriptLegacyFilename(), "rb") == FALSE)
			{
				return;
			}
		}
		else
		{
			if (fileIn.Open(Filenames.GetSQLScriptFallbackFilename(), "rb") == FALSE)
			{
				return;
			}
		}

		if (CheckDbVersion(fileIn) == FALSE)
		{
			return;
		}

		m_bBadDbVersion = FALSE;

		if (ImportAccounts(fileIn) == FALSE)
		{
			return;
		}

		bool bGotTopupBonuses = FALSE;
		if (ImportGroupTopupBonuses(fileIn, bGotTopupBonuses) == FALSE)
		{
			return;
		}

		if (ImportGroups(fileIn, bGotTopupBonuses) == FALSE)
		{
			return;
		}

		if (ImportExternalAccounts(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPluInfo(fileIn) == FALSE)
		{
			return;
		}

		if (ImportDeptInfo(fileIn) == FALSE)
		{
			return;
		}

		if (ImportStaffCards(fileIn) == FALSE)
		{
			return;
		}

		if (ImportStaffGiftExceptions(fileIn) == FALSE)
		{
			return;
		}

		if (ImportOfferStamps(fileIn) == FALSE)
		{
			return;
		}

		if (ImportEcrmanDatabases(fileIn) == FALSE)
		{
			return;
		}

		if (ImportSchemes(fileIn) == FALSE)
		{
			return;
		}

		if (ImportStampOfferInfo(fileIn) == FALSE)
		{
			return;
		}

		if (ImportCCNo(fileIn) == FALSE)
		{
			return;
		}

		if (ImportAuditPeriods(fileIn) == FALSE)
		{
			return;
		}
	
		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

bool CSQLTranScriptImport::CheckDbVersion(CSSFile& fileIn)
{
	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(DbVersion::LineID.Label);
	arrayHeader.Add(DbVersion::Version.Label);
	
	CSQLIn SQLIn(fileIn, SQLTableNames::DbVersion, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		if (csvIn.GetInt(0) == 1)
		{
			m_nIncomingDbVersion = csvIn.GetInt(1);

			if ((m_nIncomingDbVersion >= 1) && (m_nIncomingDbVersion <= LOYALTY_DBVERSION))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportAccounts(CSSFile& fileIn)
{
	CProjectSQLRepositoryAccount repoAccount;
	return repoAccount.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportGroupTopupBonuses(CSSFile& fileIn, bool& bGotGroupTopupBonuses)
{
	CProjectSQLRepositoryGroupTopupBonus repoBonus;
	return repoBonus.ImportFromScript(fileIn, m_pDatabase, bGotGroupTopupBonuses, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportGroups(CSSFile& fileIn, bool bGotGroupTopupBonuses)
{
	CProjectSQLRepositoryGroup repoGroup;
	return repoGroup.ImportFromScript(fileIn, m_pDatabase, bGotGroupTopupBonuses, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportExternalAccounts(CSSFile& fileIn)
{
	CProjectSQLRepositoryExternalAccount repoExt;
	return repoExt.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPluInfo(CSSFile& fileIn)
{
	CProjectSQLRepositoryPluInfo repoPlu;
	return repoPlu.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportDeptInfo(CSSFile& fileIn)
{
	CProjectSQLRepositoryDeptInfo repoDept;
	return repoDept.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportStaffCards(CSSFile& fileIn)
{
	CProjectSQLRepositoryStaffCard repoStaff;
	return repoStaff.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportStaffGiftExceptions(CSSFile& fileIn)
{
	CProjectSQLRepositoryStaffGiftException repoStaff;
	return repoStaff.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportOfferStamps(CSSFile& fileIn)
{
	CProjectSQLRepositoryOfferStamp repoStamp;
	return repoStamp.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportStampOfferInfo(CSSFile& fileIn)
{
	CProjectSQLRepositoryStampOfferInfo repoInfo;
	return repoInfo.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportEcrmanDatabases(CSSFile& fileIn)
{
	CProjectSQLRepositoryEcrmanDatabase repoDb;
	return repoDb.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportSchemes(CSSFile& fileIn)
{
	CProjectSQLRepositoryScheme repoScheme;
	return repoScheme.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportCCNo(CSSFile& fileIn)
{
	CProjectSQLRepositoryCCNo repoCC;
	return repoCC.ImportFromScript(fileIn, m_pDatabase, m_nIncomingDbVersion, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportAuditPeriods(CSSFile& fileIn)
{
	CProjectSQLRepositoryAuditPeriod repoPeriod;
	return repoPeriod.ImportFromScript(fileIn, m_pDatabase, m_nIncomingDbVersion, m_WorkingDlg);
}

/**********************************************************************/
