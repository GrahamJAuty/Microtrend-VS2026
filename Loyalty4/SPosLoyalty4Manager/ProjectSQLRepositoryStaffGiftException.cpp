//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryStaffGiftException.h"
//**********************************************************************

CProjectSQLRepositoryStaffGiftException::CProjectSQLRepositoryStaffGiftException() : CSQLRepositoryStaffGiftException()
{
	m_strImportTableName = SQLTableNames::StaffGiftExceptions;
}

//**********************************************************************

bool CProjectSQLRepositoryStaffGiftException::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Staff Gift Exception Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Staff Gift Exception Table");

	fileIn.Seek(0, CFile::begin);

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

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowStaffGiftException RowSG;
		RowSG.SetLineID(csvIn.GetInt64(0));
		RowSG.SetCustomerID(csvIn.GetInt64(1));
		RowSG.SetFolderIndexDbNo(csvIn.GetInt(2));
		RowSG.SetFolderIndexSetNo(csvIn.GetInt(3));
		RowSG.SetServerNo(csvIn.GetInt(4));
		RowSG.SetServerID(csvIn.GetInt64(5));
		RowSG.SetDate(csvIn.GetString(6));
		RowSG.SetTime(csvIn.GetString(7));
		RowSG.SetValue(csvIn.GetDouble(8));
		RowSG.SetErrorCode(csvIn.GetInt(9));

		if (InsertRow(RowSG, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
