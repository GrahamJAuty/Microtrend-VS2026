//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetEposTerminal.h"
//**********************************************************************

CProjectSQLRecordSetEposTerminal::CProjectSQLRecordSetEposTerminal() : CSQLRecordSetEposTerminal(NULL, RSParams_EposTerminal_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetEposTerminal::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(EposTerminal::TerminalNo.Label);
	arrayHeader.Add(EposTerminal::TerminalType.Label);
	arrayHeader.Add(EposTerminal::TerminalName.Label);
	arrayHeader.Add(EposTerminal::LastUsedDate.Label);
	arrayHeader.Add(EposTerminal::LastUsedTime.Label);
	arrayHeader.Add(EposTerminal::CCNo.Label);
	arrayHeader.Add(EposTerminal::UserID.Label);
	arrayHeader.Add(EposTerminal::CardValue.Label);
	arrayHeader.Add(EposTerminal::TotalCounter.Label);
	arrayHeader.Add(EposTerminal::TotalInDrawer.Label);
	arrayHeader.Add(EposTerminal::Disabled.Label);
	arrayHeader.Add(EposTerminal::TerminalOn.Label);
	arrayHeader.Add(EposTerminal::InService.Label);
	arrayHeader.Add(EposTerminal::DoorOpen.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::EposTerminals, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowEposTerminal RowEP;
	while (StepSelectAll(RowEP) == TRUE)
	{
		SQLOut.Add(RowEP.GetTerminalNo());
		SQLOut.Add(RowEP.GetTerminalType());
		SQLOut.Add(RowEP.GetTerminalName());
		SQLOut.Add(RowEP.GetLastUsedDate());
		SQLOut.Add(RowEP.GetLastUsedTime());
		SQLOut.Add(RowEP.GetCCNo());
		SQLOut.Add(RowEP.GetUserID());
		SQLOut.Add(RowEP.GetCardValue(), 2);
		SQLOut.Add(RowEP.GetTotalCounter());
		SQLOut.Add(RowEP.GetTotalInDrawer(), 2);
		SQLOut.Add(RowEP.GetDisableFlag());
		SQLOut.Add(RowEP.GetTerminalOnFlag());
		SQLOut.Add(RowEP.GetInServiceFlag());
		SQLOut.Add(RowEP.GetDoorOpenFlag());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
