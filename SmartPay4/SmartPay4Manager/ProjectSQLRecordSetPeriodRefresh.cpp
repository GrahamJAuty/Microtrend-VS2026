//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPeriodRefresh.h"
//**********************************************************************

CProjectSQLRecordSetPeriodRefresh::CProjectSQLRecordSetPeriodRefresh() : CSQLRecordSetPeriodRefresh(NULL, RSParams_PeriodRefresh_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPeriodRefresh::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
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

	CSQLOut SQLOut(fileOut, SQLTableNames::PeriodRefresh, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPeriodRefresh RowPeriod;
	while (StepSelectAll(RowPeriod) == TRUE)
	{
		SQLOut.Add(RowPeriod.GetUserID());
		SQLOut.Add(RowPeriod.GetPeriod1Date());
		SQLOut.Add(RowPeriod.GetPeriod1Balance(), 2);
		SQLOut.Add(RowPeriod.GetPeriod2Date());
		SQLOut.Add(RowPeriod.GetPeriod2Balance(), 2);
		SQLOut.Add(RowPeriod.GetPeriod3Date());
		SQLOut.Add(RowPeriod.GetPeriod3Balance(), 2);
		SQLOut.Add(RowPeriod.GetPeriod4Date());
		SQLOut.Add(RowPeriod.GetPeriod4Balance(), 2);
		SQLOut.Add(RowPeriod.GetPeriod5Date());
		SQLOut.Add(RowPeriod.GetPeriod5Balance(), 2);
		SQLOut.Add(RowPeriod.GetPeriod6Date());
		SQLOut.Add(RowPeriod.GetPeriod6Balance(), 2);

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
