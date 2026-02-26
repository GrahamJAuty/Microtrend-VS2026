//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetGroupTopupBonus.h"
//**********************************************************************

CProjectSQLRecordSetGroupTopupBonus::CProjectSQLRecordSetGroupTopupBonus() : CSQLRecordSetGroupTopupBonus(NULL, RSParams_GroupTopupBonus_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetGroupTopupBonus::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(GroupTopupBonus::GroupNo.Label);
	arrayHeader.Add(GroupTopupBonus::PurseNo.Label);
	arrayHeader.Add(GroupTopupBonus::TopupNo.Label);
	arrayHeader.Add(GroupTopupBonus::BonusFlag.Label);
	arrayHeader.Add(GroupTopupBonus::BonusDisable.Label);
	arrayHeader.Add(GroupTopupBonus::BonusType.Label);
	arrayHeader.Add(GroupTopupBonus::BonusTrip.Label);
	arrayHeader.Add(GroupTopupBonus::BonusPoints.Label);
	arrayHeader.Add(GroupTopupBonus::BonusValue.Label);
	arrayHeader.Add(GroupTopupBonus::BonusPercent.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::GroupTopupBonuses, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowGroupTopupBonus RowGroup;
	while (StepSelectAll(RowGroup) == TRUE)
	{
		SQLOut.Add(RowGroup.GetGroupNo());
		SQLOut.Add(RowGroup.GetPurseNo());
		SQLOut.Add(RowGroup.GetTopupNo());
		SQLOut.Add(RowGroup.GetBonusFlag());
		SQLOut.Add(RowGroup.GetBonusDisable());
		SQLOut.Add(RowGroup.GetBonusType());
		SQLOut.Add(RowGroup.GetBonusTrip(), 2);
		SQLOut.Add(RowGroup.GetBonusPoints());
		SQLOut.Add(RowGroup.GetBonusValue(), 2);
		SQLOut.Add(RowGroup.GetBonusPercent());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
