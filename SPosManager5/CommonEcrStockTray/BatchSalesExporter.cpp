/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "RepCSV.h"
/**********************************************************************/
#include "BatchSalesExporter.h"
/**********************************************************************/

CBatchSalesExporterItem::CBatchSalesExporterItem()
{
	Reset();
}

/**********************************************************************/

void CBatchSalesExporterItem::Reset()
{
	m_strDate = "";
	m_nPluNo = 0;
	m_dQty = 0.0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CBatchSalesExporterItem::Add ( CBatchSalesExporterItem& source )
{
	m_dQty += source.m_dQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CBatchSalesExporterItem::Compare( CBatchSalesExporterItem& source, int nHint )
{
	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CBatchSalesExporter::CBatchSalesExporter()
{
	m_nNwkIdx = 0;
	m_bReadyToGo = FALSE;
}

/**********************************************************************/

void CBatchSalesExporter::CheckNwkIdx()
{
	if ( m_nNwkIdx < 0 || m_nNwkIdx >= dbNetwork.GetSize() )
		m_nNwkIdx = 0;
}

/**********************************************************************/

void CBatchSalesExporter::GotoNextNetwork()
{
	m_nNwkIdx++;
	if ( m_nNwkIdx < 0 || m_nNwkIdx >= dbNetwork.GetSize() )
		m_nNwkIdx = 0;
}

/**********************************************************************/

void CBatchSalesExporter::ExportBatchSalesIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	CheckNwkIdx();

	ExportBatchSalesExternal( m_nNwkIdx );
}

/**********************************************************************/

void CBatchSalesExporter::ExportBatchSalesExternal( int nNwkIdx )
{
	if ( ( nNwkIdx < 0 ) || ( nNwkIdx >= dbNetwork.GetSize() ) )
		return;

	int nLocStartIdx, nLocEndIdx;
	dbLocation.GetNetworkLocIdxRange( nNwkIdx, nLocStartIdx, nLocEndIdx );

	CReportConsolidationArray<CSortedStringByInt> arrayTerminals;

	//MAP TERMINAL NUMBERS TO DATABASES AND LOCATIONS
	for ( int nLocIdx = nLocStartIdx; nLocIdx <= nLocEndIdx; nLocIdx++ )
	{
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			nDbIdx++;
		else
			nDbIdx = 0;

		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
		{
			CSortedStringByInt item;

			item.m_nItem = dbLocation.GetTNo( nLocIdx, nTermIdx );

			int nPos;
			if ( arrayTerminals.Find( item, nPos ) == TRUE )
				return;

			item.m_strItem.Format( "%5.5d%5.5d", nDbIdx, nLocIdx );
				
			arrayTerminals.InsertAt( nPos, item );
		}
	}

	CString strPendingPath = dbNetwork.GetFolderPathSyssetBatchExport( nNwkIdx );
	strPendingPath += "\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPendingPath + "*.*" );

	CReportConsolidationArray<CSortedStringItem> arrayFiles;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
		strFilename.MakeUpper();
		
		if ( strFilename.GetLength() != 23 )
			continue;

		if ( ::TestNumeric( strFilename.Left(3) ) == FALSE )
			continue;

		if ( strFilename.Mid(3,1) != "_" )
			continue;

		if ( ::TestNumeric( strFilename.Mid(4,8) ) == FALSE )
			continue;

		if ( strFilename.Mid(12,1) != "_" )
			continue;

		if ( ::TestNumeric( strFilename.Mid(13,6) ) == FALSE )
			continue;

		if ( strFilename.Right(4) != ".DAT" )
			continue;

		CSortedStringByInt itemTNo;
		itemTNo.m_nItem = atoi( strFilename.Left(3) );

		int nPos;
		if ( arrayTerminals.Find( itemTNo, nPos ) == FALSE )
			continue;

		arrayTerminals.GetAt( nPos, itemTNo );
		CString strDbLocIdx = itemTNo.m_strItem;

		if ( strDbLocIdx.GetLength() != 10 )
			continue;

		if ( ::TestNumeric( strDbLocIdx ) == FALSE )
			continue;

		if ( strDbLocIdx.Left(5) == "00000" )
			continue;

		CSortedStringItem item;
		item.m_strItem = "";
		item.m_strItem += strDbLocIdx;
		item.m_strItem += "_";
		item.m_strItem += strFilename;
		arrayFiles.Consolidate( item );
	}

	CreateSyssetSubdirectory( "Exports" );	
	CString strExportFile = "Exports\\Sales.csv";
	GetSyssetProgramPath( strExportFile );
		
	int nCurrentDbIdx = -1;
	for ( int nFileIdx = 0; nFileIdx < arrayFiles.GetSize(); nFileIdx++ )
	{
		CSortedStringItem item;
		arrayFiles.GetAt( nFileIdx, item );
		CString strFile = item.m_strItem;

		int nDbIdx = atoi( strFile.Left(5) ) - 1;
		CString strFilename = strFile.Right(23);
		int nTNo = atoi( strFilename.Left(3) );

		if ( nDbIdx != nCurrentDbIdx )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info );
			nCurrentDbIdx = nDbIdx;
		}

		bool bNeedHeader = ( ::FileExists( strExportFile ) == FALSE );

		CSSFile fileExport;
		if ( fileExport.Open( strExportFile, "ab" ) == TRUE )
		{
			CSSFile filePending;
			if ( filePending.Open( strPendingPath + strFilename, "rb" ) == TRUE )
			{
				CString strSaleDate = "";
				bool bInSaleTransaction = FALSE;
				CReportConsolidationArray<CBatchSalesExporterItem> arraySales;

				CString strBuffer;
				while( filePending.ReadString( strBuffer ) == TRUE )
				{
					CCSVEposTermLine csvSale( strBuffer );

					switch( csvSale.GetLineTypeInt() )
					{
					case CASHRSP_LINETYPE_TRAN_NORMAL:
						{
							CString strTranDate = csvSale.GetTransactionDate();

							if ( strTranDate.GetLength() == 8 )
							{
								strSaleDate = strTranDate.Right(2) + strTranDate.Mid(3,2) + strTranDate.Left(2);
								bInSaleTransaction = TRUE;
							}
							else
								bInSaleTransaction = FALSE;
						}
						break;

					case CASHRSP_LINETYPE_TRAN_VOID:
					case CASHRSP_LINETYPE_WASTAGE:
					case CASHRSP_LINETYPE_NOSALE:
					case CASHRSP_LINETYPE_AGEVERIFY:
					case CASHRSP_LINETYPE_ATTENDANCE:
						bInSaleTransaction = FALSE;
						break;

					case CASHRSP_LINETYPE_ITEM:
						if ( TRUE == bInSaleTransaction )
							ProcessTransactionItem( csvSale, arraySales, strSaleDate );
						break;
					}
				}

				for ( int nSaleIdx = 0; nSaleIdx < arraySales.GetSize(); nSaleIdx++ )
				{
					CBatchSalesExporterItem item;
					arraySales.GetAt( nSaleIdx, item );

					CString strDate;
					strDate.Format( "%s/%s/20%s",
						(const char*) item.m_strDate.Right(2),
						(const char*) item.m_strDate.Mid(2,2),
						(const char*) item.m_strDate.Left(2) );

					CString strPluNo;
					strPluNo.Format( "%*.*I64d", 
						Super.MaxPluLen(),
						Super.MaxPluLen(),
						item.m_nPluNo );

					if ( TRUE == bNeedHeader )
					{
						fileExport.WriteLine( "JobDate,LocationName,PluNumber,Value,Qty" );
						bNeedHeader = FALSE;
					}

					CCSV csvExport;
					csvExport.Add( strDate );
					csvExport.Add( nTNo );
					csvExport.Add( strPluNo );
					csvExport.Add( item.m_dVal, 2 );
					csvExport.Add( item.m_dQty, 2 );
					fileExport.WriteLine( csvExport.GetLine() );
				}
			}
			filePending.Close();

			CFileRemover FileRemover( strPendingPath + strFilename );
		}
	}
}

/**********************************************************************/

void CBatchSalesExporter::ProcessTransactionItem( CCSVEposTermLine& csvIn, CReportConsolidationArray<CBatchSalesExporterItem>& arraySales, CString& strSaleDate )
{
	__int64 nPluNo = csvIn.GetItemPluNo();
	int nItemValue = csvIn.GetItemUnitPrice();
	int nLineValue = csvIn.GetItemLinePrice();
	int nDbValue = csvIn.GetItemDatabasePrice();
	int nPriceBand = csvIn.GetItemPriceBand();
	double dQty = csvIn.GetItemQty();
	CString strVoid = csvIn.GetItemVoidFlag();
	CString strRefund = csvIn.GetItemRefundFlag();

	if ( strVoid == "V" )
		return;

	bool bIsDeliveryLine = FALSE;

	int nPluDeliveryType = EcrmanOptions.GetFeaturesEposDeliveryType();
	if ( ( nPluDeliveryType >= 2 ) && ( nPluDeliveryType <= 10 ) )
	{
		bIsDeliveryLine &= ( ( nPriceBand + 1 ) == nPluDeliveryType );
		bIsDeliveryLine &= ( 0 == nItemValue );
		bIsDeliveryLine &= ( 0 == nLineValue );
	}

	//bIsDeliveryLine &= ( strRefund != "R" );

	if ( TRUE == bIsDeliveryLine )
		return;

	if ( DealerFlags.GetCashRSPPromoFixFlag() == TRUE )
	{
		if ( ( dQty > 0.0 ) && ( nItemValue < 0 ) && ( nDbValue > 0.0 ) )
			dQty = 0.0;
	}
				
	//REFUNDED ITEMS COME BACK FROM EPOS WITH +VE QTY AND -VE VAL
	if ( strRefund == "R" )
		dQty = -dQty;

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = nPluNo;
	::ProcessPluNo( infoPluNo, FALSE, TRUE );

	if ( TRUE == infoPluNo.m_bValid )
	{
		if ( infoPluNo.m_nModifier != 0 )
			dQty *=	DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );

		CString strPluNo;
		strPluNo.Format( "%*.*I64d", 
			Super.MaxPluLen(),
			Super.MaxPluLen(),
			infoPluNo.m_nBasePluNo );

		CBatchSalesExporterItem item;
		item.m_strDate = strSaleDate;
		item.m_nPluNo = infoPluNo.m_nBasePluNo;
		item.m_dQty = dQty;
		item.m_dVal = csvIn.GetItemLineValueReport();
		arraySales.Consolidate( item );
	}
}

/**********************************************************************/
