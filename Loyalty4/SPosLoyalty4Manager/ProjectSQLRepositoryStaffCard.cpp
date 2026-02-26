//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryStaffCard.h"
//**********************************************************************

CProjectSQLRepositoryStaffCard::CProjectSQLRepositoryStaffCard() : CSQLRepositoryStaffCard()
{
	m_strImportTableName = SQLTableNames::StaffCards;
}

//**********************************************************************

bool CProjectSQLRepositoryStaffCard::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Staff Card Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Staff Card Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(StaffCards::FolderIndexDbNo.Label);
	arrayHeader.Add(StaffCards::FolderIndexSetNo.Label);
	arrayHeader.Add(StaffCards::ServerNo.Label);
	arrayHeader.Add(StaffCards::UserID.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowStaffCard RowSC;
		RowSC.SetFolderIndexDbNo(csvIn.GetInt(0));
		RowSC.SetFolderIndexSetNo(csvIn.GetInt(1));
		RowSC.SetServerNo(csvIn.GetInt(2));
		RowSC.SetUserID(csvIn.GetInt64(3));

		if (InsertRow(RowSC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
