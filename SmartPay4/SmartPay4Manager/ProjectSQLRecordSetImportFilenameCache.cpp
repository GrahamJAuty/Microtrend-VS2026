//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetImportFilenameCache.h"
//**********************************************************************

CProjectSQLRecordSetImportFilenameCache::CProjectSQLRecordSetImportFilenameCache() : CSQLRecordSetImportFilenameCache(NULL, RSParams_ImportFilenameCache_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetImportFilenameCache::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(ImportFilenameCache::LineID.Label);
	arrayHeader.Add(ImportFilenameCache::Type.Label);
	arrayHeader.Add(ImportFilenameCache::Filename.Label);
	arrayHeader.Add(ImportFilenameCache::DateTime.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::ImportFilenameCache, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowImportFilenameCache RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetLineID());
		SQLOut.Add(RowOS.GetType());
		SQLOut.Add(RowOS.GetFilename());
		SQLOut.Add(RowOS.GetDateTime());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
