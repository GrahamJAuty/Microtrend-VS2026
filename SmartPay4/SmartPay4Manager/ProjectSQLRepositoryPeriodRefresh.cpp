//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPeriodRefresh.h"
//**********************************************************************

CProjectSQLRepositoryPeriodRefresh::CProjectSQLRepositoryPeriodRefresh() : CSQLRepositoryPeriodRefresh()
{
	m_strImportTableName = SQLTableNames::PeriodRefresh;
}

//**********************************************************************

bool CProjectSQLRepositoryPeriodRefresh::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Period Refresh Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Period Refresh Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PeriodRefresh::UserID.Label);
	arrayHeader.Add(PeriodRefresh::Period1Date.Label);
	arrayHeader.Add(PeriodRefresh::Period1Balance.Label);
	arrayHeader.Add(PeriodRefresh::Period2Date.Label);
	arrayHeader.Add(PeriodRefresh::Period2Balance.Label);
	arrayHeader.Add(PeriodRefresh::Period3Date.Label);
	arrayHeader.Add(PeriodRefresh::Period3Balance.Label);
	arrayHeader.Add(PeriodRefresh::Period4Date.Label);
	arrayHeader.Add(PeriodRefresh::Period4Balance.Label);
	arrayHeader.Add(PeriodRefresh::Period5Date.Label);
	arrayHeader.Add(PeriodRefresh::Period5Balance.Label);
	arrayHeader.Add(PeriodRefresh::Period6Date.Label);
	arrayHeader.Add(PeriodRefresh::Period6Balance.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPeriodRefresh RowPeriod;
		RowPeriod.SetUserID(csvIn.GetInt64(0));
		RowPeriod.SetPeriod1Date(csvIn.GetString(1), TRUE);
		RowPeriod.SetPeriod1Balance(csvIn.GetDouble(2));
		RowPeriod.SetPeriod2Date(csvIn.GetString(3), TRUE);
		RowPeriod.SetPeriod2Balance(csvIn.GetDouble(4));
		RowPeriod.SetPeriod3Date(csvIn.GetString(5), TRUE);
		RowPeriod.SetPeriod3Balance(csvIn.GetDouble(6));
		RowPeriod.SetPeriod4Date(csvIn.GetString(7), TRUE);
		RowPeriod.SetPeriod4Balance(csvIn.GetDouble(8));
		RowPeriod.SetPeriod5Date(csvIn.GetString(9), TRUE);
		RowPeriod.SetPeriod5Balance(csvIn.GetDouble(10));
		RowPeriod.SetPeriod6Date(csvIn.GetString(11), TRUE);
		RowPeriod.SetPeriod6Balance(csvIn.GetDouble(12));

		if (InsertRow(RowPeriod, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
