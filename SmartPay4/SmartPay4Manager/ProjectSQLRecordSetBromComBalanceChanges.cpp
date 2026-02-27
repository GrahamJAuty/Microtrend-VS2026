//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBromComBalanceChanges.h"
//**********************************************************************

CProjectSQLRecordSetBromComBalanceChanges::CProjectSQLRecordSetBromComBalanceChanges() : CSQLRecordSetBromComBalanceChanges(NULL, RSParams_BromComBalanceChanges_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBromComBalanceChanges::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BromComBalanceChanges::PersonID.Label);
	arrayHeader.Add(BromComBalanceChanges::AccountID.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BromComBalanceChanges, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowBromComBalanceChanges RowBC;
	while (StepSelectAll(RowBC) == TRUE)
	{
		SQLOut.Add(RowBC.GetPersonID());
		SQLOut.Add(RowBC.GetAccountID());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");
	}

	WorkingDlg.IncrementRecordsChecked(10, TRUE);
}

//**********************************************************************
