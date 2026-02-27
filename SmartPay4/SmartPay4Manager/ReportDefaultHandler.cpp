/**********************************************************************/

//#include "FilenameHandler.h"
#include "ReportDefaultHandler.h"

/********************************************************************
//	See DbExportHnadler for other Report Labels

	R101			Audit Report
	R102			Revaluation audit
	R103			Spend bonus
	R104			Purchase History ALL
	R105			Purchase History Single Card

**********************************************************************/

bool CReportDefaultHandler::Read ( const char *szLabel, CString& strLine )
{
	CString strLabel = szLabel;
	if ( strLabel.GetLength() > 0 )
	{
		CSharedStringArray Array;
		if ( Array.Open ( Filenames.GetReportDefaultHandlerFilename(), DB_READONLY ) == DB_ERR_NONE )
		{
			strLabel.MakeUpper();

			for ( int nIndex = 0; nIndex < Array.GetSize(); nIndex++ )
			{
				CCSV csv ( Array.GetAt( nIndex ) );

				CString strCheck = csv.GetString(0);
				strCheck.MakeUpper();
		
				if ( strCheck == strLabel )
				{
					strLine = csv.GetString(1);					// get csv parameters
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

void CReportDefaultHandler::Save ( const char* szLabel, CCSV* pCsv )
{
	CString strLabel = szLabel;
	if ( strLabel.GetLength() == 0 )
		return;

	CSharedStringArray Array;
	if ( Array.Open ( Filenames.GetReportDefaultHandlerFilename(), DB_READWRITE ) == DB_ERR_NONE )
	{
		strLabel.MakeUpper();

		int nIndex;
		for ( nIndex = 0; nIndex < Array.GetSize(); nIndex++ )
		{
			CCSV csvLine ( Array.GetAt( nIndex ) );
			CString strCheck = csvLine.GetString(0);
			strCheck.MakeUpper();
		
			if ( strCheck == strLabel )
				break;
		}

		CCSV csv;
		csv.Add ( strLabel );					// label
		csv.Add ( pCsv->GetLine() );			// csv file defaults

		if ( nIndex < Array.GetSize() )		Array.SetAt ( nIndex, csv.GetLine() );
		else								Array.InsertAt ( nIndex, csv.GetLine() );

		Array.Close();
	}
}

/**********************************************************************/
