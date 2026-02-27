//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryAuditPeriod.h"
//**********************************************************************

CProjectSQLRepositoryAuditPeriod::CProjectSQLRepositoryAuditPeriod() : CSQLRepositoryAuditPeriod()
{
	m_strImportTableName = SQLTableNames::AuditPeriods;
}

//**********************************************************************

bool CProjectSQLRepositoryAuditPeriod::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Audit Period Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Audit Period Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(AuditPeriod::PeriodID.Label);
	arrayHeader.Add(AuditPeriod::PeriodName.Label);
	arrayHeader.Add(AuditPeriod::StartTime.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowAuditPeriod RowAP;
		RowAP.SetPeriodID(csvIn.GetInt(0));
		RowAP.SetPeriodName(csvIn.GetString(1));
		RowAP.SetStartTime(csvIn.GetInt(2));

		if (InsertRow(RowAP, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
