//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetStaffCard.h"
//**********************************************************************

CProjectSQLRecordSetStaffCard::CProjectSQLRecordSetStaffCard() : CSQLRecordSetStaffCard(NULL, RSParams_StaffCard_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetStaffCard::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(StaffCards::FolderIndexDbNo.Label);
	arrayHeader.Add(StaffCards::FolderIndexSetNo.Label);
	arrayHeader.Add(StaffCards::ServerNo.Label);
	arrayHeader.Add(StaffCards::UserID.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::StaffCards, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowStaffCard RowStaffCard;
	while (StepSelectAll(RowStaffCard) == TRUE)
	{
		SQLOut.Add(RowStaffCard.GetFolderIndexDbNo());
		SQLOut.Add(RowStaffCard.GetFolderIndexSetNo());
		SQLOut.Add(RowStaffCard.GetServerNo());
		SQLOut.Add(RowStaffCard.GetUserID());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
