//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPurchaseControlRule.h"
//**********************************************************************

CProjectSQLRecordSetPurchaseControlRule::CProjectSQLRecordSetPurchaseControlRule() : CSQLRecordSetPurchaseControlRule(NULL, RSParams_PurchaseControlRule_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPurchaseControlRule::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PurchaseControlRule::RuleID.Label);
	arrayHeader.Add(PurchaseControlRule::Enabled.Label);
	arrayHeader.Add(PurchaseControlRule::Threshold.Label);
	arrayHeader.Add(PurchaseControlRule::Expiry.Label);
	arrayHeader.Add(PurchaseControlRule::Description.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PurchaseControlRules, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPurchaseControlRule RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetRuleID());
		SQLOut.Add(RowOS.GetEnableFlag());
		SQLOut.Add(RowOS.GetThreshold());
		SQLOut.Add(RowOS.GetExpiryDays());
		SQLOut.Add(RowOS.GetDescription());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
