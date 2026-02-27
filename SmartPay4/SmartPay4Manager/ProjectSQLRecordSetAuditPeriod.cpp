//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetAuditPeriod.h"
//**********************************************************************

CProjectSQLRecordSetAuditPeriod::CProjectSQLRecordSetAuditPeriod() : CSQLRecordSetAuditPeriod(NULL, RSParams_AuditPeriod_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetAuditPeriod::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(AuditPeriod::PeriodID.Label);
	arrayHeader.Add(AuditPeriod::PeriodName.Label);
	arrayHeader.Add(AuditPeriod::StartTime.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::AuditPeriods, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowAuditPeriod RowPeriod;
	while (StepSelectAll(RowPeriod) == TRUE)
	{
		SQLOut.Add(RowPeriod.GetPeriodID());
		SQLOut.Add(RowPeriod.GetPeriodName());
		SQLOut.Add(RowPeriod.GetStartTime());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
