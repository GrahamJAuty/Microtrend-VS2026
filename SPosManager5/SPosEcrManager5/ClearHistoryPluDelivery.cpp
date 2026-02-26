 /**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ClearHistoryPluDelivery.h"
/**********************************************************************/

CClearHistoryPluDelivery::CClearHistoryPluDelivery( int nYear, int nMonth, int nDay, CLocationSelectorEntity& LocSelEntity )
{	
	CString strSearchDate;
	strSearchDate.Format ( "%4.4d%2.2d%2.2d", nYear, nMonth, nDay );

	int nStartDbIdx = LocSelEntity.GetDbIdx();
	int nEndDbIdx = LocSelEntity.GetDbIdx();

	if ( LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		nStartDbIdx = 0;
		nEndDbIdx = dbDatabase.GetSize() - 1;
	}

	for ( int nDbIdxToClear = nStartDbIdx; nDbIdxToClear <= nEndDbIdx; nDbIdxToClear++ )
	{
		CString strAuditFile = dbDatabase.GetFilePathDataPluStockAudit ( nDbIdxToClear );	
		CString strTempFile = dbDatabase.GetFilePathDataPluStockAuditTemp ( nDbIdxToClear );
		CString strBackupFile = dbDatabase.GetFilePathDataPluStockAuditBackup ( nDbIdxToClear );

		CSSFile fileHistory;
		CSSFile fileTemp;

		if ( fileHistory.Open ( strAuditFile, "rb" ) == FALSE )
			continue;
	
		if ( fileTemp.Open ( strTempFile, "wb" ) == FALSE )
			continue;

		CString strBuffer;	

		StatusProgress.Lock( TRUE, "Clearing delivery audits" );
		while ( fileHistory.ReadString ( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( fileHistory );

			//EXTRACT DETAILS FROM THIS DELIVERY LINE
			CCSVPluDelivery csv ( strBuffer );
			CString strCSVDate = csv.GetDate();

			bool bInclude = TRUE;
			if ( strCSVDate.GetLength() == 8 )
			{
				CString strCSVSearchDate = strCSVDate.Right(4) + strCSVDate.Mid(2,2) + strCSVDate.Left(2);

				//COPY THIS LINE IF WE ARE AFTER THE CUTOFF RANGE
				if ( strSearchDate > strCSVSearchDate )
					bInclude = FALSE;
			}

			if ( bInclude == TRUE )
				fileTemp.WriteLine ( strBuffer );
		}
		StatusProgress.Unlock();
		fileTemp.Close();
		fileHistory.Close();
		CFileRemover FileRemover ( strBackupFile );
		rename ( strAuditFile, strBackupFile );
		rename ( strTempFile, strAuditFile );
	}
}

/**********************************************************************/
