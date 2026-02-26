//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetDeptInfo.h"
//**********************************************************************

CProjectSQLRecordSetDeptInfo::CProjectSQLRecordSetDeptInfo() : CSQLRecordSetDeptInfo(NULL, RSParams_DeptInfo_NormalNoParams		{})
{
}

//**********************************************************************

void CProjectSQLRecordSetDeptInfo::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(DeptInfo::DbNo.Label);
	arrayHeader.Add(DeptInfo::DeptNo.Label);
	arrayHeader.Add(DeptInfo::Name.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusTrip.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusType.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusPoints.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusValue.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusPercent.Label);
	arrayHeader.Add(DeptInfo::GeneralSpendBonusFlag.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::DeptInfo, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowDeptInfo RowDeptInfo;
	while (StepSelectAll(RowDeptInfo) == TRUE)
	{
		SQLOut.Add(RowDeptInfo.GetDbNo());
		SQLOut.Add(RowDeptInfo.GetDeptNo());
		SQLOut.Add(RowDeptInfo.GetName());
		SQLOut.Add(RowDeptInfo.GetLoyaltyBonusTrip(), 2);
		SQLOut.Add(RowDeptInfo.GetLoyaltyBonusType());
		SQLOut.Add(RowDeptInfo.GetLoyaltyBonusPoints());
		SQLOut.Add(RowDeptInfo.GetLoyaltyBonusValue(), 2);
		SQLOut.Add(RowDeptInfo.GetLoyaltyBonusPercent());
		SQLOut.Add(RowDeptInfo.GetGeneralSpendBonusFlag());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
