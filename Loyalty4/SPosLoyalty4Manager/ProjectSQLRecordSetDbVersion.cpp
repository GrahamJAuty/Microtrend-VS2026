//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetDbVersion.h"
//**********************************************************************

CProjectSQLRecordSetDbVersion::CProjectSQLRecordSetDbVersion() : CSQLRecordSetDbVersion(NULL, RSParams_DbVersion_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetDbVersion::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(DbVersion::LineID.Label);
	arrayHeader.Add(DbVersion::Version.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::DbVersion, arrayHeader);

	//NO WRITE DELETE IN CASE SOMEONE TRIES TO IMPORT SCRIPT INTO SSMS

	CSQLRowDbVersion RowVersion;
	while (StepSelectAll(RowVersion) == TRUE)
	{
		SQLOut.Add(RowVersion.GetLineID());
		SQLOut.Add(RowVersion.GetVersion());
		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
