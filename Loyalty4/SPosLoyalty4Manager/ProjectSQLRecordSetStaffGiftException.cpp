//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetStaffGiftException.h"
//**********************************************************************

CProjectSQLRecordSetStaffGiftException::CProjectSQLRecordSetStaffGiftException() : CSQLRecordSetStaffGiftException(NULL, RSParams_StaffGiftException_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetStaffGiftException::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(StaffGiftExceptions::LineID.Label);
	arrayHeader.Add(StaffGiftExceptions::CustomerID.Label);
	arrayHeader.Add(StaffGiftExceptions::FolderIndexDbNo.Label);
	arrayHeader.Add(StaffGiftExceptions::FolderIndexSetNo.Label);
	arrayHeader.Add(StaffGiftExceptions::ServerNo.Label);
	arrayHeader.Add(StaffGiftExceptions::ServerID.Label);
	arrayHeader.Add(StaffGiftExceptions::Date.Label);
	arrayHeader.Add(StaffGiftExceptions::Time.Label);
	arrayHeader.Add(StaffGiftExceptions::Value.Label);
	arrayHeader.Add(StaffGiftExceptions::ErrorCode.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::StaffGiftExceptions, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowStaffGiftException RowSG;
	while (StepSelectAll(RowSG) == TRUE)
	{
		SQLOut.Add(RowSG.GetLineID());
		SQLOut.Add(RowSG.GetCustomerID());
		SQLOut.Add(RowSG.GetFolderIndexDbNo());
		SQLOut.Add(RowSG.GetFolderIndexSetNo());
		SQLOut.Add(RowSG.GetServerNo());
		SQLOut.Add(RowSG.GetServerID());
		SQLOut.Add(RowSG.GetDate());
		SQLOut.Add(RowSG.GetTime());
		SQLOut.Add(RowSG.GetValue(), 2);
		SQLOut.Add(RowSG.GetErrorCode());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
