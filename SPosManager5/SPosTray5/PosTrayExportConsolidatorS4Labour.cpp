/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
#include "LocationCSVArray.h"
#include "PluSalesHistoryFileArray.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PosTrayExportConsolidatorS4Labour.h"
/**********************************************************************/

CPosTrayExportConsolidatorS4Labour::CPosTrayExportConsolidatorS4Labour( CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg ) : CPosTrayExportConsolidator( 0, SelectArray, bQuitNow, pHostDlg )
{
	m_bUseTermID = TRUE;
	m_bWantGroupNameSale = TRUE;
}

/**********************************************************************/

void CPosTrayExportConsolidatorS4Labour::ResetS4Labour( COleDateTime dateExport, CSessionCSVRecord& ReportSession )
{
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateExport, dateExport, ReportSession );
	
	m_arrayConsolidationFields.RemoveAll();
	m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_LOCNO );
	m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_DATE_BUS );
	m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_GRPNO_SALE );
	m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_S4LABOUR_HOUR );
	m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_TRMNO );

	m_arrayOutputFields.RemoveAll();
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_LOCNAME );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_EMPTY );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_EMPTY );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_DATE_BUS );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_S4LABOUR_REV );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_GRPNAME_SALE );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_S4LABOUR_HOUR );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_FULL );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_NET );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_TAX );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM );
	m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_TRMNO );

	m_SalesTree.ClearConsolidatedLevels();
	m_SalesTree.ClearFieldStructure();
	
	m_SelectArray.MakeList();
}

/**********************************************************************/

bool CPosTrayExportConsolidatorS4Labour::PrepareFields()
{
	return CPosTrayExportConsolidator::PrepareFields( m_SalesTree );
}

/**********************************************************************/

void CPosTrayExportConsolidatorS4Labour::Consolidate() 
{
	m_SalesTree.ClearConsolidatedLevels();
	
	CPluSalesHistoryFileArray arrayFileInfo( m_SelectArray );
	arrayFileInfo.SetEposSalesFlag( TRUE );
	arrayFileInfo.SetImportSalesFlag( FALSE );
	arrayFileInfo.SetManualSalesFlag( FALSE );
	arrayFileInfo.SetEODModeFlag( FALSE );
	arrayFileInfo.SetGuestToTerminalFlag( TRUE );
	arrayFileInfo.SetSaleTimeFlag( TRUE );
	arrayFileInfo.BuildFileList();

	int nProgress = 0;	
	
	if ( NULL == m_pHostDialog )
		StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nLastDbIdx = -1;
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetFileCount() ) && ( FALSE == m_bQuitNow ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetFileInfo( nFileIdx, infoFile );

		int nDbIdx;
		int nDbNo;
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( infoFile.m_nSelectArrayIdx, ListItem );
			nDbIdx = ListItem.m_nDbIdx;
			nDbNo = dbDatabase.GetDbNo( nDbIdx );
		}

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CPluSalesLine csvSales( infoFile.IsLegacySalesFolder() );

		//THESE VALUES STAY THE SAME WITHIN EACH FILE
		{
			int nTNo = 0;
			if ( ( infoFile.m_nTermIdx != 0 ) && ( TRUE == m_bUseTermID ) )
				nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );

			m_SalesTree.SetBufferInt( 0, infoFile.m_nLocIdx );
			m_SalesTree.SetBufferInt( 4, nTNo );
		}

		//IGNORE PMS SALES
		if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == TRUE )
			continue;
	
		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED FOR TAX AND GROUP INFO
		if ( nDbIdx != nLastDbIdx )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			nLastDbIdx = nDbIdx;
		}
		
		CString strThisFilterTime = "";
		CString	strThisFilterDate = infoFile.m_strDateSale;
		CString	strLastFilterDate = infoFile.m_strDateSale;
		COleDateTime oleThisFilterDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );
		
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				ShowProgress( nProgress + fileSales.Tell(), arrayFileInfo.GetTotalFileSize() );

				csvSales.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPESS
				int nLineType = csvSales.GetLineType();
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					break;

				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
					continue;
				}
				
				if (csvSales.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csvSales.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}
				
				strThisFilterTime = csvSales.GetSaleTime();
				ExpandTimeString( strThisFilterTime );
				
				if ( oleThisFilterDate.m_status != COleDateTime::valid )
					continue;

				//CHECK FOR PREVIOUS DATE
				if ( m_BusinessDateInfo.ValidateBusinessDate( infoFile.m_nLocIdx, FALSE, strThisFilterDate, strThisFilterTime ) == FALSE )
					continue;

				int	nDeptGrpSale = csvSales.GetGroupNo() - 1;
				if ( nDeptGrpSale < 0 )
					nDeptGrpSale = 0;

				if ( 0 == nDeptGrpSale )	
					nDeptGrpSale = 1000;
				
				int nTaxBand = 0;
				if ((CSVPLUDATA_PROMO_NONTAXABLE != nLineType) && (CSVPLUDATA_MIXMATCH_NONTAXABLE != nLineType))
				{
					nTaxBand = csvSales.GetNumericTaxBand();
				}
				
				//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
				m_SalesTree.SetBufferDate( 1, m_BusinessDateInfo.GetBusinessDate() );
				m_SalesTree.SetBufferInt( 2, nDeptGrpSale );
				m_SalesTree.SetBufferString( 3, strThisFilterTime.Left(2) );
				
				double dLineVal = 0.0;
				double dLineTax = 0.0;
				double dLineDiscountVal = 0.0;
				
				bool bConsolidateLine = FALSE;
				
				if ((csvSales.GetPluSaleWastageFlag() == FALSE) && (csvSales.GetPluSaleVoidFlag() == FALSE))
				{
					switch (nLineType)
					{
					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PLU_MIXMATCH:
					{
						bConsolidateLine = TRUE;
						double dDiscountAmount = csvSales.GetValue();
						double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
						dLineDiscountVal = ReportHelpers.CalcNonTax(dDiscountAmount, dTaxRate);
					}
					break;

					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bConsolidateLine = TRUE;
						dLineDiscountVal = csvSales.GetValue();
						break;

					case CSVPLUDATA_PLU_SALE:
					{
						bConsolidateLine = TRUE;
						dLineVal = csvSales.GetValue();
						double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
						dLineTax = dLineVal - (ReportHelpers.CalcNonTax(dLineVal, dTaxRate));
					}
					break;
					}
				}

				if ( TRUE == bConsolidateLine )
				{
					CEntityTreeExportS4LabourNode Node;
					Node.SetVal( dLineVal );
					Node.SetDiscount( dLineDiscountVal );
					Node.SetTaxAmount( dLineTax );
					m_SalesTree.Consolidate( Node );
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( FALSE == m_bQuitNow ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	if ( NULL == m_pHostDialog )
		StatusProgress.Unlock();
}

/**********************************************************************/

__int64 CPosTrayExportConsolidatorS4Labour::CreateExport( const char* szFilename, bool& bResult )
{
	m_bWantGroupNameSale = TRUE;
	return CPosTrayExportConsolidator::CreateExport( &m_SalesTree, "Creating S4 Labour Export", szFilename, bResult, FALSE );
}

/**********************************************************************/

void CPosTrayExportConsolidatorS4Labour::GetNodeTotals( int nBranchIdx, int nLeafIdx )
{
	CEntityTreeExportS4LabourNode SalesNode;
	m_SalesTree.GetNode( nBranchIdx, nLeafIdx, SalesNode );
	m_NodeTotals.m_dValueFull = SalesNode.GetVal();
	m_NodeTotals.m_dDiscountVal = SalesNode.GetDiscount();
	m_NodeTotals.m_dTaxAmount = SalesNode.GetTaxAmount();
	m_NodeTotals.m_dValueNet = m_NodeTotals.m_dValueFull - m_NodeTotals.m_dTaxAmount;
}

/**********************************************************************/