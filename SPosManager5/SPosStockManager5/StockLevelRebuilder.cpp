/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "SalesTypes.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockLevelRebuilder.h"
/**********************************************************************/

CStockLevelRebuilderInfo::CStockLevelRebuilderInfo()
{
	Reset();
}

/**********************************************************************/

void CStockLevelRebuilderInfo::Reset()
{
	m_strStockCode = "";
	m_strDescription = "";
	m_strActions = "";
}

/**********************************************************************/

void CStockLevelRebuilderInfo::Add ( CStockLevelRebuilderInfo& source )
{
	m_strActions += source.m_strActions;
}

/**********************************************************************/

int CStockLevelRebuilderInfo::Compare( CStockLevelRebuilderInfo& source, int nHint )
{
	if ( m_strStockCode != source.m_strStockCode )
		return ( ( m_strStockCode > source.m_strStockCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStockLevelRebuilder::CStockLevelRebuilder()
{
	m_nDbIdx = DataManager.GetActiveDbIdx();
	m_nConLevel = NODE_DATABASE;
	m_nTableNo = dbDatabase.GetDbNo( m_nDbIdx );
	m_oleDateFrom = COleDateTime( 2017, 1, 24, 0, 0, 0 );
	m_oleDateTo = COleDateTime::GetCurrentTime();
	m_nSalesType = SALES_TYPE_E1;
	m_bStocktakeAdjust = FALSE;

	m_arrayNeverRedirect.Add( 20013288 );
	m_arrayNeverRedirect.Add( 20014834 );
	m_arrayNeverRedirect.Add( 886798536354 );
}

/**********************************************************************/

bool CStockLevelRebuilder::RebuildStockLevels()
{
	bool bSuccess = FALSE;

	CSSFile fileResults;
	CString strPath = "REBUILD.TXT";
	GetTempPath( strPath );
	if ( fileResults.Open( strPath, "wb" ) == TRUE )
	{
		bSuccess = TRUE;
		
		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		{
			if ( MatchStockpoint( nSpIdx ) == TRUE )
			{
				m_arrayMovements.RemoveAll();
				m_arrayErrors.RemoveAll();

				for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
				{
					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt( nStockIdx, StockRecord );

					CStockLevelsCSVRecord StockLevels;
					int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
					DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
					
					if ( StockLevels.GetStocktakeNo() == 0 )
					{
						double dQtyOnOrder = StockLevels.GetQtyOnOrder();
						double dMinimumLevelQty = StockLevels.GetMinimumLevelQty();
						double dMaximumLevelQty = StockLevels.GetMaximumLevelQty();
						double dReorderLevelQty = StockLevels.GetReorderLevelQty();
						double dAutoWastagePercent = StockLevels.GetAutoWastagePercent();

						StockLevels.Reset();
						StockLevels.SetQtyOnOrder( dQtyOnOrder );
						StockLevels.SetMinimumLevelQty( dMinimumLevelQty );
						StockLevels.SetMaximumLevelQty( dMaximumLevelQty );
						StockLevels.SetReorderLevelQty( dReorderLevelQty );
						StockLevels.SetAutoWastagePercent( dAutoWastagePercent );

						DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
					}
				}

				StatusProgress.Lock( TRUE, "Fixing Stock Levels" );
				m_arrayStockChangeMap.RemoveAll();
				m_bStocktakeAdjust = FALSE;
				m_oleDateFrom = COleDateTime( 2016, 1, 24, 0, 0, 0 );
				ProcessStockpoint( nSpIdx, 1 );
				m_oleDateFrom = COleDateTime( 2017, 1, 24, 0, 0, 0 );
				ProcessStockpoint( nSpIdx, 2 );
				StatusProgress.Unlock();

				int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
				for ( int n = 0; n < DataManager.Stock.GetSize(); n++ )
				{
					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt( n, StockRecord );

					CStockLevelsCSVRecord StockLevels;
					DataManager.Stock.GetStockpointTotals( n, StockRecord, nSpNo, StockLevels );
						
					double dAdjustQty = StockLevels.GetAdjustmentQty();
					double dAdjustVal = StockLevels.GetAdjustmentValue();
					double dSalesQty = StockLevels.GetEposSalesQty(0);
					double dSalesVal = StockLevels.GetEposSalesValue(0);

					if ( ( CPriceHelpers::CompareDoubles( dAdjustQty, 0.0, 3 ) != 0 ) && ( CPriceHelpers::CompareDoubles( dAdjustVal, 0.0, 3 ) == 0 ) )
					{
						if ( CPriceHelpers::CompareDoubles( dSalesQty, 0.0, 3 ) != 0 )
						{
							StockLevels.SetAdjustmentValue( dAdjustQty * ( dSalesVal / dSalesQty ) );
							DataManager.Stock.SetStockpointTotals( n, StockRecord, nSpNo, StockLevels );
						}
						else
						{
							int nStockPluIdx;
							if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == TRUE )
							{
								if ( DataManager.StockPlu.GetPluNoCount( nStockPluIdx ) > 0 )
								{
									__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
			
									int nPluIdx;
									if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
									{
										CPluCSVRecord PluRecord;
										DataManager.Plu.GetAt ( nPluIdx, PluRecord );
										double dRetailVal = PluRecord.GetPrice(0);

										double dStockQty;
										DataManager.Plu.GetPluStockQty ( PluRecord.GetPluNoInt(), dStockQty );
										
										if ( CPriceHelpers::CompareDoubles( dStockQty, 0.0, 3 ) != 0 )
										{
											StockLevels.SetAdjustmentValue( ( dAdjustQty * dRetailVal ) / dStockQty );
											DataManager.Stock.SetStockpointTotals( n, StockRecord, nSpNo, StockLevels );
										}
									}
								}
							}
						}
					}
				}
			
				fileResults.WriteLine( dbStockpoint.GetName( nSpIdx ) );
				fileResults.WriteLine( "**********" );
				
				for ( int n = 0; n < m_arrayMovements.GetSize(); n++ )
				{
					fileResults.WriteLine( "" );
					
					CStockLevelRebuilderInfo infoMovement;
					m_arrayMovements.GetAt( n, infoMovement );

					{
						CString str;
						str = infoMovement.m_strStockCode;
						str += " ";
						str += infoMovement.m_strDescription;
						fileResults.WriteLine( str );
					}

					CCSV csvActions( infoMovement.m_strActions );
					{
						for( int x = 0; x < csvActions.GetSize() - 2; x += 3 )
						{
							CString str = "==> ";
							str += csvActions.GetString(x);
							str += " ";
							str += csvActions.GetString(x+1);
							str += " ";
							str += csvActions.GetString(x+2);

							fileResults.WriteLine( str );
						}
					}
				}

				if ( m_arrayErrors.GetSize() != 0 )
				{
					fileResults.WriteLine( "" );

					for ( int n = 0; n < m_arrayErrors.GetSize(); n++ )
						fileResults.WriteLine( m_arrayErrors.GetAt(n) );
				}

				if ( m_arrayRedirects.GetSize() != 0 )
				{
					fileResults.WriteLine( "" );

					for ( int n = 0; n < m_arrayRedirects.GetSize(); n++ )
						fileResults.WriteLine( m_arrayRedirects.GetAt(n) );
				}

				CString str;
				str.Format( "Stock Items Matched %d", m_arrayMovements.GetSize() );
				fileResults.WriteLine( "" );
				fileResults.WriteLine( str );
			}
		}
	}
	return bSuccess;
}

/**********************************************************************/

bool CStockLevelRebuilder::MatchStockpoint( int nSpIdx )
{
	if ( m_nConLevel == NODE_SYSTEM )
		return TRUE;

	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		return FALSE;

	int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
	if ( dbStockpoint.GetDbNo( nSpIdx ) != nDbNo )
		return FALSE;

	bool bMatch = FALSE;
	switch ( m_nConLevel )
	{
	case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == m_nTableNo );		break;
	case NODE_SITE:			bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == m_nTableNo );	break;
	case NODE_SYSTEM:		
	case NODE_DATABASE:		bMatch = TRUE;		break;
	default:				bMatch = FALSE;		break;
	}
	return bMatch;
}

/**********************************************************************/

void CStockLevelRebuilder::GetAuditFileList( int nSpIdx, CReportConsolidationArray<CSortedStringItem>& arrayFiles, __int64& nTotalFileSize )
{
	nTotalFileSize = 0;
	arrayFiles.RemoveAll();

	CString strDateFrom, strDateTo;
	strDateFrom.Format ( "%4.4d%2.2d%2.2d", m_oleDateFrom.GetYear(), m_oleDateFrom.GetMonth(), m_oleDateFrom.GetDay() );
	strDateTo.Format ( "%4.4d%2.2d%2.2d", m_oleDateTo.GetYear(), m_oleDateTo.GetMonth(), m_oleDateTo.GetDay() );

	CString strPath = "";
	strPath += dbStockpoint.GetFolderPathAudit( nSpIdx );
	strPath += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

	CStringArray arrayTemp;
	
	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();

		bool bAcceptExtension = FALSE;

		if ( ( strFilename.GetLength() == 12 ) && ( strFilename.Right(4) == ".001" ) )
			bAcceptExtension = TRUE;
		else if ( ( strFilename.GetLength() == 13 ) && ( strFilename.Right(5) == ".001b" ) )
			bAcceptExtension = TRUE;
		
		if ( FALSE == bAcceptExtension )
			continue;

		CString strDate = strFilename.Left(8);
		if ( strDate < strDateFrom || strDate > strDateTo || ( ::TestNumeric( strDate ) == FALSE ) )
			continue;

		arrayTemp.Add( strFilename );
	}
		
	strPath += "\\";
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CString strFilename = arrayTemp.GetAt(n);

		CSortedStringItem item;
		item.m_strItem = strFilename;
		arrayFiles.Consolidate( item );

		CFileStatus FileStatus;
		CFile::GetStatus( strPath + strFilename, FileStatus );
		nTotalFileSize += FileStatus.m_size;
	}
}

/**********************************************************************/

void CStockLevelRebuilder::ProcessStockpoint( int nSpIdx, int nPass )
{
	__int64 nTotalFileSize = 0;
	CReportConsolidationArray<CSortedStringItem> arrayFiles;
	GetAuditFileList( nSpIdx, arrayFiles, nTotalFileSize );

	__int64 nProgressSoFar = 0;
	StatusProgress.SetDialogText( dbStockpoint.GetName( nSpIdx ) );
	StatusProgress.SetPos( 0, nTotalFileSize );

	for ( int nIndex = 0; nIndex < arrayFiles.GetSize(); nIndex++ )
	{
		CString strFilename = dbStockpoint.GetFolderPathAudit( nSpIdx );
			
		CSortedStringItem item;
		arrayFiles.GetAt( nIndex, item );

		strFilename += "\\";
		strFilename += item.m_strItem;

		bool bTransferIn = ( strFilename.Right(1) == "b" );

		CSSFile fileAudit;
		if ( fileAudit.Open ( strFilename, "rb" ) == TRUE )
		{	
			CString strBuffer;
			bool bProcessBlock = FALSE;
				
			while ( fileAudit.ReadString ( strBuffer ) == TRUE )
			{
				StatusProgress.SetPos( nProgressSoFar + fileAudit.Tell(), nTotalFileSize );
			
				if ( strBuffer.Left(1) == "H" )
					bProcessBlock = ProcessHeader( strBuffer, nPass );
				else if ( bProcessBlock == TRUE )
					ProcessLine( nSpIdx, bTransferIn, strBuffer, nPass );
			}

			nProgressSoFar += fileAudit.GetLength();
			fileAudit.Close();
		}
	}
}

/**********************************************************************/

bool CStockLevelRebuilder::ProcessHeader( const char* szHeader, int nPass )
{
	bool bResult = FALSE;

	CCSV csv ( szHeader );

	int nAction = csv.GetInt(1);
	switch( nAction )
	{
	case AUDIT_DELIVERY:
	case AUDIT_ADJUSTMENT_STOCK:
	case AUDIT_OPENING:
	case AUDIT_STOCKTAKE:
	case AUDIT_RETURNS:
	case AUDIT_SALES:
	case AUDIT_TRANSFER:
	case AUDIT_ADJUSTMENT_PLU:
		
		switch ( csv.GetInt(2) )
		{
		case 1:
		case 2:
			bResult = ( 2 == nPass ) || ( AUDIT_SALES == nAction );
			break;

		default:
			bResult = FALSE;
			break;
		}
	}

	m_bStocktakeAdjust = FALSE;

	return bResult;
}

/**********************************************************************/

void CStockLevelRebuilder::ProcessAction( int nSpIdx, int nType, int nMode, double dQty, double dVal, const char* szStockCode )
{
	CSortedStringByString itemStockChange;
	itemStockChange.m_strKey = szStockCode;
	bool bRedirected = FALSE;
	
	{
		int nIdx;
		if ( m_arrayStockChangeMap.Find( itemStockChange, nIdx ) == TRUE )
		{
			m_arrayStockChangeMap.GetAt( nIdx, itemStockChange );
			itemStockChange.m_strKey = itemStockChange.m_strData;
			bRedirected = TRUE;
		}
	}
	
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( itemStockChange.m_strKey, nStockIdx ) == TRUE )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		CStockLevelsCSVRecord StockLevels;
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		if ( StockLevels.GetStocktakeNo() == 0 )
		{
			CStockLevelRebuilderInfo infoMove;
			infoMove.m_strStockCode = itemStockChange.m_strKey;		
		
			int nMovePos;
			bool bFirstMove = ( m_arrayMovements.Find( infoMove, nMovePos ) == FALSE );

			CCSV csvAction;

			if ( TRUE == bRedirected )
			{
				csvAction.Add( 99 );
				csvAction.Add( szStockCode );
				csvAction.Add( itemStockChange.m_strKey );
			}

			csvAction.Add( nType );
			csvAction.Add( nMode );
			csvAction.Add( dQty, SysInfo.GetDPQty() );

			if ( TRUE == bFirstMove )
			{
				infoMove.m_strDescription = StockRecord.GetDescription();
				infoMove.m_strActions = csvAction.GetLine();
				m_arrayMovements.InsertAt( nMovePos, infoMove );
			}
			else
			{
				m_arrayMovements.GetAt( nMovePos, infoMove );
				infoMove.m_strActions += ",";
				infoMove.m_strActions += csvAction.GetLine();
				m_arrayMovements.SetAt( nMovePos, infoMove );
			}
			
			switch( nType )
			{
			case AUDIT_STOCKTAKE:
				if ( SUBTRACT_STOCK_LEVELS == nMode )
					StockLevels.AddAdjustment( StockLevels.GetApparentQty() - dQty, 0, FALSE );
				else
				{
					double dQtyOnOrder = StockLevels.GetQtyOnOrder();
					double dMinimumLevelQty = StockLevels.GetMinimumLevelQty();
					double dMaximumLevelQty = StockLevels.GetMaximumLevelQty();
					double dReorderLevelQty = StockLevels.GetReorderLevelQty();
					double dAutoWastagePercent = StockLevels.GetAutoWastagePercent();

					StockLevels.Reset();
					StockLevels.SetQtyOnOrder( dQtyOnOrder );
					StockLevels.SetMinimumLevelQty( dMinimumLevelQty );
					StockLevels.SetMaximumLevelQty( dMaximumLevelQty );
					StockLevels.SetReorderLevelQty( dReorderLevelQty );
					StockLevels.SetAutoWastagePercent( dAutoWastagePercent );
					
					StockLevels.SetOpeningQty( dQty );
				}
				break;

			case AUDIT_DELIVERY:
				if ( CPriceHelpers::CompareDoubles( dVal, 0.0, 3 ) != 0 )
					StockLevels.AddDelivery( dQty, dVal, FALSE );
				else
					StockLevels.AddFreeStock( dQty, FALSE );
				break;

			case AUDIT_RETURNS:
				StockLevels.AddReturns( dQty, dVal, FALSE );
				break;

			case AUDIT_SALES:
				switch( m_nSalesType )
				{
				case SALES_TYPE_MANUAL:
					StockLevels.AddManualSales( dQty, dVal, FALSE );
					break;

				case SALES_TYPE_IMPORT:
					StockLevels.AddImportSales( dQty, dVal, FALSE );
					break;

				case SALES_TYPE_E1:
				default:
					StockLevels.AddEposSales( 0, dQty, dVal );
					break;
				}
				break;

			case AUDIT_ADJUSTMENT_STOCK:
			case AUDIT_ADJUSTMENT_PLU:
				StockLevels.AddAdjustment( dQty, dVal, FALSE );
				break;

			case AUDIT_OPENING:
				switch( nMode )
				{
				case ADD_STOCK_LEVELS:
					StockLevels.SetOpeningQty( StockLevels.GetOpeningQty() + dQty );
					break;

				case SUBTRACT_STOCK_LEVELS:
					StockLevels.SetOpeningQty( StockLevels.GetOpeningQty() - dQty );
					break;

				case EDIT_STOCK_LEVELS:
					StockLevels.SetOpeningQty( dQty );
					break;
				}
				break;

			case AUDIT_TRANSFER:
				switch( nMode )
				{
				case ADD_STOCK_LEVELS:
					StockLevels.SetTransferInQty( StockLevels.GetTransferInQty() + dQty );
					break;

				case SUBTRACT_STOCK_LEVELS:
					StockLevels.SetTransferOutQty( StockLevels.GetTransferOutQty() + dQty );
					break;
				}
			}

			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		}
	}
	else
	{
		CString strError;
		strError.Format ( "UNKNOWN STOCK CODE,%s,%d,%d,%f,%f",
			szStockCode,
			nType,
			nMode,
			dQty,
			dVal );
	
		m_arrayErrors.Add( strError );
	}
}

/**********************************************************************/

void CStockLevelRebuilder::ProcessLine( int nSpIdx, bool bTransferIn, const char* szLine, int nPass )
{
	CCSV csvAudit( szLine );

	bool bOK = FALSE;

	switch( csvAudit.GetInt(1) )
	{
	case AUDIT_STOCKTAKE:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
			if ( FALSE == m_bStocktakeAdjust )
				ProcessAction( nSpIdx, AUDIT_STOCKTAKE, EDIT_STOCK_LEVELS, csvAudit.GetDouble(8), 0.0, csvAudit.GetString(3) );
			else
				ProcessAction( nSpIdx, AUDIT_STOCKTAKE, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(8), 0.0, csvAudit.GetString(3) );
			bOK = TRUE;
			break;

		case 99:
			m_bStocktakeAdjust = ( csvAudit.GetInt(3) == 3 );
			break;
		}
		break;

	case AUDIT_DELIVERY:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
		case 2:
		case 3:
		case 4:
			ProcessAction( nSpIdx, AUDIT_DELIVERY, ADD_STOCK_LEVELS, csvAudit.GetDouble(7), csvAudit.GetDouble(9), csvAudit.GetString(3) );
			bOK = TRUE;
			break;
		}
		break;

	case AUDIT_RETURNS:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
		case 5:
			ProcessAction( nSpIdx, AUDIT_RETURNS, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(7), csvAudit.GetDouble(9), csvAudit.GetString(3) );
			bOK = TRUE;
			break;
		}
		break;

	case AUDIT_SALES:

		if ( 1 == nPass )
		{
			bOK = TRUE;

			if ( csvAudit.GetInt(2) == 1 )
			{
				CString strPluNo = csvAudit.GetString(4);
				CString strStockCode = csvAudit.GetString(9);
			
				if ( ( strPluNo != "" ) && ( strStockCode != "" ) )
				{
					bool bNoRedirect = FALSE;

					__int64 nPluNo = _atoi64( strPluNo );
					for ( int n = 0; n < m_arrayNeverRedirect.GetSize(); n++ )
					{
						if ( m_arrayNeverRedirect.GetAt(n) == nPluNo )
						{
							bNoRedirect = TRUE;
							break;
						}
					}

					if ( FALSE == bNoRedirect )
					{
						CRecipeCSVRecord Recipe;
						DataManager.Plu.GetRecipe( -1, _atoi64( strPluNo ), Recipe );

						if ( Recipe.GetSize() == 1 )
						{
							CString strRecipeCode = Recipe.GetStockCode(0);
							
							if ( strRecipeCode != strStockCode )
							{
								CString strRedirect;
								strRedirect.Format( " REDIRECT %s %s %s",
									(const char*) strPluNo,
									(const char*) strStockCode,
									(const char*) strRecipeCode );
								m_arrayRedirects.Add( strRedirect );

								CSortedStringByString item;
								item.m_strKey = strStockCode;
								item.m_strData = strRecipeCode;
								m_arrayStockChangeMap.Consolidate( item );
							}
						}
					}
				}
			}
		}
		else
		{
			switch( csvAudit.GetInt(2) )
			{
			case 1:
			case 3:
				switch( csvAudit.GetInt(3) )
				{
				case SALES_TYPE_E1:
					m_nSalesType = SALES_TYPE_E1;
					break;

				case SALES_TYPE_MANUAL:
				case SALES_TYPE_MANUAL_STOCK:
					m_nSalesType = SALES_TYPE_MANUAL;
					break;

				case SALES_TYPE_IMPORT:
				case SALES_TYPE_IMPORT_STOCK:
					m_nSalesType = SALES_TYPE_IMPORT;
					break;
				}
			}
	
			switch( csvAudit.GetInt(2) )
			{
			case 1:
				ProcessAction( nSpIdx, AUDIT_SALES, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(11), csvAudit.GetDouble(12), csvAudit.GetString(9) );
				bOK = TRUE;
				break;

			case 3:
				ProcessAction( nSpIdx, AUDIT_SALES, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(13), csvAudit.GetDouble(14), csvAudit.GetString(4) );
				bOK = TRUE;
				break;
			}
		}
		break;

	case AUDIT_ADJUSTMENT_STOCK:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
			ProcessAction( nSpIdx, AUDIT_ADJUSTMENT_STOCK, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(7), csvAudit.GetDouble(8), csvAudit.GetString(3) );
			bOK = TRUE;
			break;
		}
		break;

	case AUDIT_ADJUSTMENT_PLU:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
			ProcessAction( nSpIdx, AUDIT_ADJUSTMENT_PLU, SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(10), csvAudit.GetDouble(11), csvAudit.GetString(8) );
			bOK = TRUE;
			break;
		}
		break;

	case AUDIT_OPENING:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
			ProcessAction( nSpIdx, AUDIT_OPENING, csvAudit.GetInt(7), csvAudit.GetDouble(8), 0.0, csvAudit.GetString(3) );
			bOK = TRUE;
			break;

		case 2:
			ProcessAction( nSpIdx, AUDIT_OPENING, csvAudit.GetInt(7), csvAudit.GetDouble(9), 0.0, csvAudit.GetString(3) );
			bOK = TRUE;
			break;
		}
		break;

	case AUDIT_TRANSFER:
		switch( csvAudit.GetInt(2) )
		{
		case 1:
			ProcessAction( nSpIdx, AUDIT_TRANSFER, bTransferIn ? ADD_STOCK_LEVELS : SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(7), 0.0, csvAudit.GetString(3) );
			bOK = TRUE;
			break;

		case 2:
			ProcessAction( nSpIdx, AUDIT_TRANSFER, bTransferIn ? ADD_STOCK_LEVELS : SUBTRACT_STOCK_LEVELS, csvAudit.GetDouble(8), 0.0, csvAudit.GetString(3) );
			bOK = TRUE;
			break;
		}
		break;
	}

	if ( FALSE == bOK )
	{
		CString strError = "UNKNOWN LINE TYPE,";
		m_arrayErrors.Add( strError + szLine );
	}
}

/**********************************************************************/
