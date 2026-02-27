//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryImportFilenameCache.h"
//**********************************************************************

CProjectSQLRepositoryImportFilenameCache::CProjectSQLRepositoryImportFilenameCache() : CSQLRepositoryImportFilenameCache()
{
	m_strImportTableName = SQLTableNames::ImportFilenameCache;
}

//**********************************************************************

bool CProjectSQLRepositoryImportFilenameCache::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Import Filename Cache Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Import Filename Cache Table");
	
	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(ImportFilenameCache::LineID.Label);
	arrayHeader.Add(ImportFilenameCache::Type.Label);
	arrayHeader.Add(ImportFilenameCache::Filename.Label);
	arrayHeader.Add(ImportFilenameCache::DateTime.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowImportFilenameCache RowCache;
		RowCache.SetLineID(csvIn.GetInt64(0));
		RowCache.SetType(csvIn.GetInt(1));
		RowCache.SetFilename(csvIn.GetString(2));
		RowCache.SetDateTime(csvIn.GetString(3));

		if (InsertRow(RowCache, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
