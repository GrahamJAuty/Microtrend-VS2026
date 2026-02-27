//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPurchaseControlStamp.h"
//**********************************************************************

CProjectSQLRecordSetPurchaseControlStamp::CProjectSQLRecordSetPurchaseControlStamp() : CSQLRecordSetPurchaseControlStamp(NULL, RSParams_PurchaseControlStamp_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPurchaseControlStamp::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PurchaseControlStamp::UserID.Label);
	arrayHeader.Add(PurchaseControlStamp::RuleID.Label);
	arrayHeader.Add(PurchaseControlStamp::AwardDate.Label);
	arrayHeader.Add(PurchaseControlStamp::ExpireDate.Label);
	arrayHeader.Add(PurchaseControlStamp::StampCount.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PurchaseControlStamps, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPurchaseControlStamp RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetUserID());
		SQLOut.Add(RowOS.GetRuleID());
		SQLOut.Add(RowOS.GetAwardDate());
		SQLOut.Add(RowOS.GetExpireDate());
		SQLOut.Add(RowOS.GetStampCount());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
