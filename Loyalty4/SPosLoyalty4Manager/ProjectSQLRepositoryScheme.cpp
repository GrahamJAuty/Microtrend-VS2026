//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryScheme.h"
//**********************************************************************

CProjectSQLRepositoryScheme::CProjectSQLRepositoryScheme() : CSQLRepositoryScheme()
{
	m_strImportTableName = SQLTableNames::Schemes;
}

//**********************************************************************

bool CProjectSQLRepositoryScheme::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Loyalty Scheme Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Loyalty Scheme Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(Scheme::SchemeNo.Label);
	arrayHeader.Add(Scheme::SchemeName.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nSchemeNo = csvIn.GetInt(0);
		if ((nSchemeNo < Scheme::SchemeNo.Min) || (nSchemeNo > Scheme::SchemeNo.Max))
		{
			continue;
		}

		CSQLRowScheme RowSC;
		RowSC.SetSchemeNo(nSchemeNo);
		RowSC.SetSchemeName(csvIn.GetString(1));

		if (InsertRow(RowSC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
