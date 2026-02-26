//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetAuditPeriod.h"
//**********************************************************************

CProjectSQLRecordSetAuditPeriod::CProjectSQLRecordSetAuditPeriod() : CSQLRecordSetAuditPeriod(NULL, RSParams_AuditPeriod_NormalNoParams		{})
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

	int nChecked = 0;
	CSQLRowAuditPeriod RowAP;
	while (StepSelectAll(RowAP) == TRUE)
	{
		SQLOut.Add(RowAP.GetPeriodID());
		SQLOut.Add(RowAP.GetPeriodName());
		SQLOut.Add(RowAP.GetStartTime());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
