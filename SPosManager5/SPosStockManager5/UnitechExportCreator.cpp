/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "UnitechExportCreator.h"
/**********************************************************************/

CUnitechExportCreator::CUnitechExportCreator(void)
{
	int	nDbIdx = DataManager.GetActiveDbIdx();
	if ( ( nDbIdx < 0 ) || ( nDbIdx >= dbDatabase.GetSize() ) )
		return;
	
	CFilenameUpdater FnUp( SysFiles::UnitechPlu, nDbIdx );
	
	CSSFile filePlu;
	if ( filePlu.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
		return;

	StatusProgress.Lock( TRUE, "Exporting PLU records" );
		
	int nCount = DataManager.Plu.GetSize();
	for ( int nProgress = 0; nProgress < nCount; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nCount );	
	
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nProgress, PluRecord );

		CString strStockCode;
		DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, FALSE );

		double dApparent = 0.0;
		CStockCSVRecord StockRecord;
			
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
		{
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals( nStockIdx, StockRecord, StockLevels );
			dApparent = StockLevels.GetApparentQty();
		}

		int nDPQty = SysInfo.GetDPQty();
		int nDPVal = SysInfo.GetDPValue();

		CCSV csv;
		csv.Add( PluRecord.GetPluNoInt() );
		csv.Add( PluRecord.GetEposText() );
		csv.Add( dApparent, nDPQty );
		csv.Add( PluRecord.GetPrice(0), nDPVal );
		csv.Add( PluRecord.GetPrice(1), nDPVal );
		csv.Add( PluRecord.GetPrice(2), nDPVal );
		csv.Add( PluRecord.GetBaseDeptNo() );
		csv.Add( PluRecord.GetBaseTaxBandInt() );
		csv.Add( StockRecord.GetWrapper(0) );

		filePlu.WriteLine( csv.GetLine() );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/
