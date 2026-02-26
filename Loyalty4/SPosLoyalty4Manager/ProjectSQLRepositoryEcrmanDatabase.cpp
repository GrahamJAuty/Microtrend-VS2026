//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryEcrmanDatabase.h"
//**********************************************************************

CProjectSQLRepositoryEcrmanDatabase::CProjectSQLRepositoryEcrmanDatabase() : CSQLRepositoryEcrmanDatabase()
{
	m_strImportTableName = SQLTableNames::EcrmanDatabases;
}

//**********************************************************************

bool CProjectSQLRepositoryEcrmanDatabase::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Ecrman Database Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Ecrman Database Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(EcrmanDatabase::DbNo.Label);
	arrayHeader.Add(EcrmanDatabase::DbName.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nDbNo = csvIn.GetInt(0);
		if ((nDbNo < EcrmanDatabase::DbNo.Min) || (nDbNo > EcrmanDatabase::DbNo.Max))
		{
			continue;
		}

		CSQLRowEcrmanDatabase RowOS;
		RowOS.SetDbNo(nDbNo);
		RowOS.SetDbName(csvIn.GetString(1));

		if (InsertRow(RowOS, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
