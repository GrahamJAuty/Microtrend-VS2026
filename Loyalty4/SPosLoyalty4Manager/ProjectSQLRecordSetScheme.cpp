//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetScheme.h"
//**********************************************************************

CProjectSQLRecordSetScheme::CProjectSQLRecordSetScheme() : CSQLRecordSetScheme(NULL, RSParams_Scheme_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetScheme::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(Scheme::SchemeNo.Label);
	arrayHeader.Add(Scheme::SchemeName.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::Schemes, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowScheme RowSC;
	while (StepSelectAll(RowSC) == TRUE)
	{
		SQLOut.Add(RowSC.GetSchemeNo());
		SQLOut.Add(RowSC.GetSchemeName());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
