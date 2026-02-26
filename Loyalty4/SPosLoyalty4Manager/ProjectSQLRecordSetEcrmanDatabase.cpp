//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetEcrmanDatabase.h"
//**********************************************************************

CProjectSQLRecordSetEcrmanDatabase::CProjectSQLRecordSetEcrmanDatabase() : CSQLRecordSetEcrmanDatabase(NULL, RSParams_EcrmanDatabase_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetEcrmanDatabase::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(EcrmanDatabase::DbNo.Label);
	arrayHeader.Add(EcrmanDatabase::DbName.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::EcrmanDatabases, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowEcrmanDatabase RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetDbNo());
		SQLOut.Add(RowOS.GetDbName());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
