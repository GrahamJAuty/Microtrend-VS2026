//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetClosingBalance.h"
//**********************************************************************

CProjectSQLRecordSetClosingBalance::CProjectSQLRecordSetClosingBalance() : CSQLRecordSetClosingBalance(NULL, RSParams_ClosingBalance_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetClosingBalance::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(ClosingBalance::KeyDate.Label);
	arrayHeader.Add(ClosingBalance::Date.Label);
	arrayHeader.Add(ClosingBalance::Time.Label);
	arrayHeader.Add(ClosingBalance::ArchiveDate.Label);
	arrayHeader.Add(ClosingBalance::ArchiveTime.Label);
	arrayHeader.Add(ClosingBalance::Purse1LiabilityAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse2BalanceAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse3LiabilityAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse1CreditAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse3CreditAudit.Label);
	arrayHeader.Add(ClosingBalance::HaveAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse1LiabilityAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse3LiabilityAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse1CreditAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse3CreditAdjust.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::ClosingBalances, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowClosingBalance RowClosing;
	while (StepSelectAll(RowClosing) == TRUE)
	{
		SQLOut.Add(RowClosing.GetKeyDate());
		SQLOut.Add(RowClosing.GetDate());
		SQLOut.Add(RowClosing.GetTime());
		SQLOut.Add(RowClosing.GetArchiveDate());
		SQLOut.Add(RowClosing.GetArchiveTime());
		SQLOut.Add(RowClosing.GetPurse1LiabilityAudit(), 2);
		SQLOut.Add(RowClosing.GetPurse2BalanceAudit(), 2);
		SQLOut.Add(RowClosing.GetPurse3LiabilityAudit(), 2);
		SQLOut.Add(RowClosing.GetPurse1CreditAudit(), 2);
		SQLOut.Add(RowClosing.GetPurse3CreditAudit(), 2);
		SQLOut.Add(RowClosing.GetHaveAdjustFlag());
		SQLOut.Add(RowClosing.GetPurse1LiabilityAdjust(), 2);
		SQLOut.Add(RowClosing.GetPurse3LiabilityAdjust(), 2);
		SQLOut.Add(RowClosing.GetPurse1CreditAdjust(), 2);
		SQLOut.Add(RowClosing.GetPurse3CreditAdjust(), 2);

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
