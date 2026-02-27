//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetUserText.h"
//**********************************************************************

CProjectSQLRecordSetUserText::CProjectSQLRecordSetUserText() : CSQLRecordSetUserText(NULL, RSParams_UserText_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetUserText::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(UserText::FieldLabel.Label);
	arrayHeader.Add(UserText::UserText.Label);
	arrayHeader.Add(UserText::UseFlag.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::UserTexts, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowUserText RowText;
	while (StepSelectAll(RowText) == TRUE)
	{
		SQLOut.Add(RowText.GetFieldLabel());
		SQLOut.Add(RowText.GetUserText());
		SQLOut.Add(RowText.GetUseFlag());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.SetCaption2RecordsChecked(10, TRUE);
	}
}

//**********************************************************************
