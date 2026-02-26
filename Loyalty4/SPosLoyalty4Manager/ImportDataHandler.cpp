//*******************************************************************
#include "DatabaseImportForManager.h"
#include "LoyaltyManager.h"
//*******************************************************************
#include "ImportDataHandler.h"
//*******************************************************************

CImportDataHandler::CImportDataHandler ( CWnd* pParent )
{
	m_pParent = pParent;
	m_bBackgroundMode = TRUE;
}

//*******************************************************************

void CImportDataHandler::ScanAll()
{
	ImportDataRecords();
}

//*******************************************************************

void CImportDataHandler::ImportDataRecords()
{
	CString strFilename = Filenames.GetImportsPath ( "cardxfer.csv" );

	CString strTitle = "Cardxfer Import";
	CImportDatabaseForManager cardxfer ( strFilename, m_pParent );
	cardxfer.SetBackgroundMode ( m_bBackgroundMode ); 

	if ( cardxfer.IsValidFile() == TRUE )
	{
		CString strTmpName = Filenames.GetImportsPath ( ::GetUniqueTempFilename("$04") );
		if ( ::rename ( strFilename, strTmpName ) == 0 )			// move & rename
		{
			cardxfer.SetFilename ( strTmpName );
			cardxfer.DirectImport ( IMPORT_METHOD_ALL );
			::SendToRecycleBin ( strTmpName );
		}
	}
}

//*******************************************************************
