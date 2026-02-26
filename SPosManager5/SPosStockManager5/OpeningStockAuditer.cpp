/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "OpeningStockAuditer.h"
/**********************************************************************/

COpeningStockAuditer::COpeningStockAuditer()
{
	int nDbIdx = DataManager.GetActiveDbIdx();
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	CByteArray arrayStockpointFlags;
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		arrayStockpointFlags.Add(0);

	COleDateTime time = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format ( "%2.2d/%2.2d/%4.4d",
		time.GetDay(),
		time.GetMonth(),
		time.GetYear() );

	CString strTime;
	strTime.Format ( "%2.2d:%2.2d",
		time.GetHour(),
		time.GetMinute() );

	CSSFile fileOpeningTemp;
	CFilenameUpdater FnUp( SysFiles::OpeningTemp, DataManager.GetActiveDbIdx() );
	if ( fileOpeningTemp.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		StatusProgress.Lock( TRUE, "Auditing opening stock" );

		CString strBuffer;
		while( fileOpeningTemp.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos( fileOpeningTemp );

			CCSVOpeningStockTemp csvIn( strBuffer );
			
			int nSpIdx;
			int nSpNo = csvIn.GetSpNo();
			if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == FALSE )
				continue;

			CString strAuditFilename = dbStockpoint.GetFilePathAudit( nSpIdx, time );

			CSSFile fileAudit;
			if ( fileAudit.Open( strAuditFilename, "ab" ) == FALSE )
				continue;

			if ( arrayStockpointFlags.GetAt( nSpIdx ) == 0 )
			{
				CCSV csvOut;
				csvOut.Add( "H" );
				csvOut.Add( AUDIT_OPENING );
				csvOut.Add( 1 );
				csvOut.Add( strDate );
				csvOut.Add( strTime );
				fileAudit.WriteLine( csvOut.GetLine() );

				arrayStockpointFlags.SetAt( nSpIdx, 1 );
			}

			CCSV csvOut;
			csvOut.Add( "D" );
			csvOut.Add( AUDIT_OPENING );
			csvOut.Add( 2 );
			csvOut.Add( csvIn.GetStockCode() );
			csvOut.Add( csvIn.GetDescription() );
			csvOut.Add( csvIn.GetCategory() );
			csvOut.Add( csvIn.GetSubUnits() );
			csvOut.Add( EDIT_STOCK_LEVELS );
			csvOut.Add( 0.0, 1 );
			csvOut.Add( csvIn.GetOpeningQty(), 5 );
			csvOut.Add( csvIn.GetOpeningQty(), 5 );
			fileAudit.WriteLine( csvOut.GetLine() );
		}

		StatusProgress.Unlock();
	}

	fileOpeningTemp.Close();

	CFileRemover FileRemover;
	FileRemover.RemoveFile( FnUp.GetFilenameToUse() );
}

/**********************************************************************/
