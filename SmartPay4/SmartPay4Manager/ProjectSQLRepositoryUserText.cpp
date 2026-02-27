//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryUserText.h"
//**********************************************************************

CProjectSQLRepositoryUserText::CProjectSQLRepositoryUserText() : CSQLRepositoryUserText()
{
	m_strImportTableName = SQLTableNames::UserTexts;
}

//**********************************************************************

bool CProjectSQLRepositoryUserText::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing User Text Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring User Text Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(UserText::FieldLabel.Label);
	arrayHeader.Add(UserText::UserText.Label);
	arrayHeader.Add(UserText::UseFlag.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowUserText RowText;
		RowText.SetFieldLabel(csvIn.GetString(0));
		RowText.SetUserText(csvIn.GetString(1));
		RowText.SetUseFlag(csvIn.GetBool(2));

		if (InsertRow(RowText, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
	
	return TRUE;
}

//**********************************************************************
