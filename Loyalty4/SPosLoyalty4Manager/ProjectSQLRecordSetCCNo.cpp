//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetCCNo.h"
//**********************************************************************

CProjectSQLRecordSetCCNo::CProjectSQLRecordSetCCNo() : CSQLRecordSetCCNo(NULL, RSParams_CCNo_NormalNoParams		{})
{
}

//**********************************************************************

void CProjectSQLRecordSetCCNo::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(CCNo::Type.Label);
	arrayHeader.Add(CCNo::Number.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::CCNo, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowCCNo RowCC;
	while (StepSelectAll(RowCC) == TRUE)
	{
		SQLOut.Add(RowCC.GetTypeString());
		SQLOut.Add(RowCC.GetNumber());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
