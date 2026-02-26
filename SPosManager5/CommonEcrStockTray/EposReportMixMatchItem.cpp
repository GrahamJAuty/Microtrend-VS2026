/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportMixMatchItem.h"
/**********************************************************************/

CEposReportMixMatchItem::CEposReportMixMatchItem( CEposSelectArray& SelectArray, int nReportType ) : CEposReport( SelectArray )
{
	switch( nReportType )
	{
	case REPORT_TYPE_MIXMATCH_GROUP:
	case REPORT_TYPE_MIXMATCH_DEPT:
	case REPORT_TYPE_MIXMATCH_ITEM:
		m_nReportType = nReportType;
		m_bWantCovers = FALSE;
		break;

	case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
	case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
	case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
		m_nReportType = nReportType;
		m_bWantCovers = TRUE;
		break;

	default:
		m_nReportType = REPORT_TYPE_MIXMATCH_GROUP;
		m_bWantCovers = FALSE;
		break;
	}

	m_ReportSettings.SetOfferFilter( 0x1FF );
}

/**********************************************************************/

CEposReportMixMatchItem::CEposReportMixMatchItem( CEposSelectArray& SelectArray, int nReportType, const char* szCustomSettings ) : CEposReport( SelectArray )
{
	switch( nReportType )
	{
	case REPORT_TYPE_MIXMATCH_GROUP:
	case REPORT_TYPE_MIXMATCH_DEPT:
	case REPORT_TYPE_MIXMATCH_ITEM:
		m_nReportType = nReportType;
		m_bWantCovers = FALSE;
		break;

	case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
	case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
	case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
		m_nReportType = nReportType;
		m_bWantCovers = TRUE;
		break;

	default:
		m_nReportType = REPORT_TYPE_MIXMATCH_GROUP;
		m_bWantCovers = FALSE;
		break;
	}

	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

CEposReportMixMatchItem::~CEposReportMixMatchItem()
{
	for (int n = 0; n < m_arrayReportBlocks.GetSize(); n++)
	{
		m_arrayReportBlocks[n].m_pSales->RemoveAll();
		m_arrayReportBlocks[n].m_pMMUsageCount->RemoveAll();
		m_arrayReportBlocks[n].m_pMMUniqueID->RemoveAll();
		delete (m_arrayReportBlocks[n].m_pSales);
		delete (m_arrayReportBlocks[n].m_pMMUsageCount);
		delete (m_arrayReportBlocks[n].m_pMMUniqueID);
	}
}

/**********************************************************************/

void CEposReportMixMatchItem::InitialiseConsolidationBlocks()
{
	CString strParams = "";
	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_SAVED:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSaved();
		break;

	case EPOSREPORT_CONSOL_ADHOC:
		strParams = m_EposReportSelectInfo.GetConsolPrefsAdhoc();
		break;
		
	case EPOSREPORT_CONSOL_DEFAULT:
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		break;
		
	case EPOSREPORT_CONSOL_SYSTEM:
	default:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSummary();
		break;
	}

	CEposReportConsolParamsStandard Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}
	
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( FALSE );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportMixMatchItemBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolMMDiscountedItem>;
		block.m_pMMUsageCount = new CReportConsolidationArray<CEposReportConsolMMUsageCount>;
		block.m_pMMUniqueID = new CReportConsolidationArray<CEposReportConsolMMUniqueID>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportMixMatchItem::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	int nLastSelectArrayIdx = -1;
	m_arrayPendingTranByDateTime.RemoveAll();
	m_arrayPendingTranByOfferItem.RemoveAll();
	m_arrayPendingCoversByTran.RemoveAll();
	m_arrayPendingCoversByOffer.RemoveAll();
	m_arrayPendingDiscountedItems.RemoveAll();
	m_arrayPendingMMUsageCount.RemoveAll();
	m_arrayPendingMMUniqueID.RemoveAll();
	int nNextTranID = 0;
	int nNextOfferItemID = 0;
	int nLocIdxForPendingItems = 0;

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		if ( ( infoFile.m_nSelectArrayIdx != nLastSelectArrayIdx ) || ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) ) )
		{
			ProcessPendingItems(nLocIdxForPendingItems);
			nNextTranID = 0;
			nNextOfferItemID = 0;
			nLastSelectArrayIdx = infoFile.m_nSelectArrayIdx;
		}

		nLocIdxForPendingItems = infoFile.m_nLocIdx;
		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if (0 == nPMSStatus)
		{
			continue;
		}

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		CString strLastTranDate = "";
		CString strLastTranTime = "";
		int nLastTranID = -1;
	
		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead = 0;
		CString strBuffer = "";
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			int nTranCovers = 0;

			do
			{
				StatusProgress.SetPos(nProgress + fileSales.Tell(), nTotalFileSize);

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if (m_PMSModes.CheckPMSTimeLine(bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude) == FALSE)
				{
					continue;
				}

				csv.ParseLine(strBuffer);

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if (SysInfo.IsPMSSystem() == TRUE)
				{
					if ((m_PMSModes.GetEposFlag() == FALSE) && (FALSE == bIsPMSLocation))
					{
						if (DataManager.Payment.IsPMSPaymentType(csv.GetPluPaymentNo()))
						{
							continue;
						}
					}
				}

				//CHECK SALE DATE AND TIME
				if (CheckPluSaleDateTime(bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime) == FALSE)
				{
					continue;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if (m_ReportFilters.CheckDepartment(nDeptNo) == FALSE)
				{
					continue;
				}

				CEposReportConsolMMDiscountedItem ConsolInfo;
				ConsolInfo.m_nOfferLineType = MMITEMCONSOL_OFFER_ITEM;

				bool bProcessLine = TRUE;
								
				switch( csv.GetLineType() )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					ConsolInfo.m_nMixMatchType = -1;
					ConsolInfo.m_nOfferNo = csv.GetPromoNoSeparate();
					
					switch( m_nReportType )
					{
					case REPORT_TYPE_MIXMATCH_ITEM:
					case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
						ConsolInfo.m_nItemNo = GetSeparatedPromoPluNo( csv.GetPromoNoSeparate() );
						break;
					}
					
					ConsolInfo.m_dItemQty = 1.0;
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_nMixMatchType = csv.GetMixMatchType();
					ConsolInfo.m_nOfferNo = csv.GetMixMatchNo();

					switch( m_nReportType )
					{
					case REPORT_TYPE_MIXMATCH_ITEM:
					case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
						ConsolInfo.m_nItemNo = GetSeparatedMixMatchPluNo( csv.GetMixMatchNo() );
						break;
					}
					
					ConsolInfo.m_dItemQty = 1.0;
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
					ConsolInfo.m_nMixMatchType = -1;
					ConsolInfo.m_nOfferNo = csv.GetPromoNoForPlu();

					switch( m_nReportType )
					{
					case REPORT_TYPE_MIXMATCH_ITEM:
					case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
						ConsolInfo.m_nItemNo = csv.GetPluNoNew();
						break;
					}
					
					ConsolInfo.m_dItemQty = csv.GetDiscountQty();
					break;

				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_nMixMatchType = csv.GetMixMatchType();
					ConsolInfo.m_nOfferNo = csv.GetMixMatchNo();
					
					switch( m_nReportType )
					{
					case REPORT_TYPE_MIXMATCH_ITEM:
					case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
						ConsolInfo.m_nItemNo = csv.GetPluNoNew();
						break;
					}
					
					ConsolInfo.m_dItemQty = csv.GetDiscountQty();
					break;

				case CSVPLUDATA_MIXMATCH_USAGE:
					{
						CEposReportConsolMMUsageCount UsageCount;
						UsageCount.m_nOfferNo = csv.GetMixMatchNo();
						UsageCount.m_nOfferType = csv.GetMixMatchType();
						UsageCount.m_strUniqueID = csv.GetMixMatchUseID();
						UsageCount.m_nCount = csv.GetMixMatchUseCount();

						CEposReportConsolMMUniqueID IDItem;
						IDItem.m_nLocIdx = nLocIdxForPendingItems;
						IDItem.m_strUniqueID = csv.GetMixMatchUseID();

						int nPos = 0;
						if (m_arrayPendingMMUniqueID.Find(IDItem, nPos) == FALSE)
						{
							m_arrayPendingMMUniqueID.InsertAt(nPos, IDItem);
							m_arrayPendingMMUsageCount.Consolidate(UsageCount);
						}

						bProcessLine = FALSE;
					}
					break;

				default:
					bProcessLine = FALSE;
					break;
				}

				if ( TRUE == bProcessLine )
				{
					int nMask = m_ReportSettings.GetOfferFilter();
					switch (ConsolInfo.m_nMixMatchType)
					{
					case -1:
						bProcessLine = ((nMask & 0x001) != 0);
						break;

					case 1:
						bProcessLine = ((nMask & 0x002) != 0);
						break;

					case 2:
						bProcessLine = ((nMask & 0x004) != 0);
						break;

					case 3:
						bProcessLine = ((nMask & 0x008) != 0);
						break;

					case 4:
						bProcessLine = ((nMask & 0x010) != 0);
						break;

					case 5:
						bProcessLine = ((nMask & 0x020) != 0);
						break;

					case 6:
						bProcessLine = ((nMask & 0x040) != 0);
						break;

					case 7:
						bProcessLine = ((nMask & 0x080) != 0);
						break;

					case 8:
						bProcessLine = ((nMask & 0x100) != 0);
						break;
					}
				}

				if ( TRUE == bProcessLine )
				{
					CString strThisTranDate = infoFile.m_strDateTran;
					CString strThisTranTime = csv.GetTranTime();
					
					if ( ( strThisTranDate != strLastTranDate ) || ( strThisTranTime != strLastTranTime ) )
					{
						CEposReportConsolMMTranByDateTime infoDate;
						infoDate.m_strTranDate = strThisTranDate;
						infoDate.m_strTranTime = strThisTranTime;
						
						int nDatePos;
						if ( m_arrayPendingTranByDateTime.Find( infoDate, nDatePos ) == TRUE )
						{
							m_arrayPendingTranByDateTime.GetAt( nDatePos, infoDate );

							if ( TRUE == m_bWantCovers )
							{
								CEposReportConsolMMCoversByTran infoCovers;
								infoCovers.m_nTranID = infoDate.m_nTranID;
								
								int nTranIdx;
								if ( m_arrayPendingCoversByTran.Find( infoCovers, nTranIdx ) == TRUE )
								{
									m_arrayPendingCoversByTran.GetAt( nTranIdx, infoCovers );
									nTranCovers = infoCovers.m_nCovers;
								}
								else
								{
									nTranCovers = 0;
								}
							}
						}
						else
						{
							infoDate.m_nTranID = nNextTranID++;
							m_arrayPendingTranByDateTime.InsertAt( nDatePos, infoDate );

							if ( TRUE == m_bWantCovers )
							{
								nTranCovers = csv.GetCovers();
								if (0 == nTranCovers)
								{
									if (DealerFlags.GetAssumeCoversFlag() == TRUE)
									{
										nTranCovers = 1;
									}
								}

								CEposReportConsolMMCoversByTran infoCovers;
								infoCovers.m_nTranID = infoDate.m_nTranID;
								infoCovers.m_nCovers = nTranCovers;
								m_arrayPendingCoversByTran.Consolidate( infoCovers );
							}
						}

						strLastTranDate = strThisTranDate;
						strLastTranTime = strThisTranTime;
						nLastTranID = infoDate.m_nTranID;
					}

					bProcessLine = ( nLastTranID >= 0 );

					if ( TRUE == m_bWantCovers )
					{
						bProcessLine &= ( nTranCovers != 0 );
					}
				}

				if ( TRUE == bProcessLine )
				{		
					ConsolInfo.m_dVal = csv.GetValue();
					
					switch( m_nReportType )
					{
					case REPORT_TYPE_MIXMATCH_DEPT:
					case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
						ConsolInfo.m_nItemNo = csv.GetDeptNo();
						break;

					case REPORT_TYPE_MIXMATCH_GROUP:
					case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
						ConsolInfo.m_nItemNo = csv.GetGroupNo() - 1;

						if (ConsolInfo.m_nOfferNo < 0)
						{
							ConsolInfo.m_nItemNo = 0;
						}

						break;
					}

					int nItemPos = 0;
					if ( m_arrayPendingDiscountedItems.Find( ConsolInfo, nItemPos ) == TRUE )
					{
						double dVal = ConsolInfo.m_dVal;
						double dItemQty = ConsolInfo.m_dItemQty;
						m_arrayPendingDiscountedItems.GetAt( nItemPos, ConsolInfo );
						ConsolInfo.m_dVal += dVal;
						ConsolInfo.m_dItemQty += dItemQty;
						m_arrayPendingDiscountedItems.SetAt( nItemPos, ConsolInfo );
					}
					else
					{
						ConsolInfo.m_nOfferItemID = nNextOfferItemID++;
						m_arrayPendingDiscountedItems.InsertAt( nItemPos, ConsolInfo );
					}
					
					CEposReportConsolMMTranByOfferItem infoUsage;
					infoUsage.m_nOfferItemID = ConsolInfo.m_nOfferItemID;
					infoUsage.m_nTranID = nLastTranID;
					m_arrayPendingTranByOfferItem.Consolidate( infoUsage );	

					CEposReportConsolMMCoversByOffer infoCovers;
					infoCovers.m_nMixMatchType = ConsolInfo.m_nMixMatchType;
					infoCovers.m_nOfferNo = ConsolInfo.m_nOfferNo;
					infoCovers.m_nTranID = nLastTranID;
					infoCovers.m_nCovers = nTranCovers;
					m_arrayPendingCoversByOffer.Consolidate( infoCovers );
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();

		if ( FALSE == m_bSaleTime )
		{
			ProcessPendingItems(nLocIdxForPendingItems);
			nNextTranID = 0;
			nNextOfferItemID = 0;
		}
	}

	if ( TRUE == m_bSaleTime )
	{
		ProcessPendingItems(nLocIdxForPendingItems);
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CEposReportMixMatchItem::ProcessPendingItems(int nLocIdx)
{
	for ( int n = 0; n < m_arrayPendingDiscountedItems.GetSize(); n++ )
	{
		CEposReportConsolMMDiscountedItem infoItem;
		m_arrayPendingDiscountedItems.GetAt( n, infoItem );

		CEposReportConsolMMTranByOfferItem infoUsage;
		infoUsage.m_nOfferItemID = infoItem.m_nOfferItemID;
		infoUsage.m_nTranID = -1;

		int nStartIdx = 0;
		m_arrayPendingTranByOfferItem.Find( infoUsage, nStartIdx );

		int nEndIdx = 0;
		infoUsage.m_nOfferItemID++;
		m_arrayPendingTranByOfferItem.Find( infoUsage, nEndIdx );

		infoItem.m_nTranQty = nEndIdx - nStartIdx;

		if (infoItem.m_nTranQty < 1)
		{
			infoItem.m_nTranQty = 1;
		}

		CEposReportConsolMMDiscountedItem PromoTotal = infoItem;
		PromoTotal.m_nOfferLineType = MMITEMCONSOL_OFFER_TOTAL;
		
		CEposReportConsolMMDiscountedItem PromoHeader = infoItem;
		PromoHeader.m_nOfferLineType = MMITEMCONSOL_OFFER_HEADER;
		
		CEposReportConsolMMDiscountedItem BlockTotal = infoItem;
		BlockTotal.m_bBlockTotal = TRUE;
		
		for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
		{
			int nBlock = m_arrayTerminalBlocks.GetAt(n);
			m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( infoItem );
			m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( PromoHeader );
			m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( PromoTotal );
			m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( BlockTotal );
		}
	}

	for (int n = 0; n < m_arrayPendingMMUsageCount.GetSize(); n++)
	{
		CEposReportConsolMMUsageCount infoUsage;
		m_arrayPendingMMUsageCount.GetAt(n, infoUsage);
		
		for (int x = 0; x < m_arrayTerminalBlocks.GetSize(); x++)
		{
			int nBlock = m_arrayTerminalBlocks.GetAt(x);

			CEposReportConsolMMUniqueID itemID;
			itemID.m_nLocIdx = nLocIdx;
			itemID.m_strUniqueID = infoUsage.m_strUniqueID;

			int nPos = 0;
			if (m_arrayReportBlocks[nBlock].m_pMMUniqueID->Find(itemID, nPos) == FALSE)
			{
				m_arrayReportBlocks[nBlock].m_pMMUniqueID->InsertAt(nPos, itemID);

				CString strTemp = infoUsage.m_strUniqueID;
				infoUsage.m_strUniqueID = "";
				m_arrayReportBlocks[nBlock].m_pMMUsageCount->Consolidate(infoUsage);
				infoUsage.m_strUniqueID = strTemp;
			}
		}
	}

	if ( m_arrayPendingDiscountedItems.GetSize() > 0 )
	{
		//CALCULATE PROMO HEADER COVERS
		if ( TRUE == m_bWantCovers )
		{
			for ( int n = 0; n < m_arrayPendingCoversByOffer.GetSize(); n++ )
			{
				CEposReportConsolMMCoversByOffer infoCovers;
				m_arrayPendingCoversByOffer.GetAt( n, infoCovers );

				CEposReportConsolMMDiscountedItem infoHeader;
				infoHeader.m_nOfferLineType = MMITEMCONSOL_OFFER_HEADER;
				infoHeader.m_nMixMatchType = infoCovers.m_nMixMatchType;
				infoHeader.m_nOfferNo = infoCovers.m_nOfferNo;
				infoHeader.m_nOfferCovers = infoCovers.m_nCovers;

				CEposReportConsolMMDiscountedItem infoTotal = infoHeader;
				infoTotal.m_nOfferLineType = MMITEMCONSOL_OFFER_TOTAL;
				
				for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
				{
					int nBlock = m_arrayTerminalBlocks.GetAt(n);
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( infoHeader );
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( infoTotal );
				}
			}
		}

		//CALCULATE BLOCK TOTAL COVERS
		if ( TRUE == m_bWantCovers )
		{
			CEposReportConsolMMDiscountedItem BlockCovers;
			BlockCovers.m_bBlockTotal = TRUE;

			for ( int n = 0; n < m_arrayPendingCoversByTran.GetSize(); n++ )
			{
				CEposReportConsolMMCoversByTran infoCovers;
				m_arrayPendingCoversByTran.GetAt( n, infoCovers );
				BlockCovers.m_nOfferCovers += infoCovers.m_nCovers;
			}

			for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
			{
				int nBlock = m_arrayTerminalBlocks.GetAt(n);
				m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( BlockCovers );
			}
		}
	}

	m_arrayPendingTranByDateTime.RemoveAll();
	m_arrayPendingTranByOfferItem.RemoveAll();
	m_arrayPendingCoversByTran.RemoveAll();
	m_arrayPendingCoversByOffer.RemoveAll();
	m_arrayPendingDiscountedItems.RemoveAll();
	m_arrayPendingMMUsageCount.RemoveAll();
	m_arrayPendingMMUniqueID.RemoveAll();
}

/**********************************************************************/

bool CEposReportMixMatchItem::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );

	m_UnderlineMaker.Reset();

	switch( m_nReportType )
	{
	case REPORT_TYPE_MIXMATCH_GROUP:
	case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
		AddReportColumn ( "Group", TA_LEFT, 350, FALSE );
		break;

	case REPORT_TYPE_MIXMATCH_DEPT:
	case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
		AddReportColumn ( "Dept", TA_LEFT, 350, FALSE );
		break;
		
	case REPORT_TYPE_MIXMATCH_ITEM:
	case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
	default:
		AddReportColumn ( "Plu Number", TA_LEFT, 350, FALSE );
		AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
		break;
	}

	AddReportColumn ( "Tran Qty", TA_RIGHT, 250, FALSE );
	AddReportColumn ( "Item Qty", TA_RIGHT, 250, FALSE );
	AddReportColumn ( "Discount", TA_RIGHT, 250, TRUE );

	if ( TRUE == m_bWantCovers )
	{
		AddReportColumn ( "Covers", TA_RIGHT, 300, TRUE );
		AddReportColumn ( "Per Head", TA_RIGHT, 300, TRUE );
	}

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportMixMatchItemBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
	{
		m_nCreateReportMiniTarget = 1;
	}

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportMixMatchItemBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if (ReportBlock.GotData() == FALSE)
		{
			continue;
		}

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		CreateSalesSection( ReportBlock );	
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportMixMatchItem::CreateSalesSection( CEposReportMixMatchItemBlock& Block )
{
	bool bWantHeader = TRUE;
	int nOfferCovers = 0;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolMMDiscountedItem Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( TRUE == Sales.m_bBlockTotal )
		{
			m_ReportFile.RequestBlankLines(1);
			m_ReportFile.WriteReportMiscLine( m_UnderlineMaker.GetLine() );

			CString strTotal = GetTotalTypeName( FALSE, Block.m_nEntityType );
			WriteTotalLine( Sales, strTotal );
		}
		else if ( MMITEMCONSOL_OFFER_TOTAL == Sales.m_nOfferLineType )
		{
			m_ReportFile.WriteReportMiscLine( m_UnderlineMaker.GetLine() );
			WriteTotalLine( Sales, ( Sales.m_nMixMatchType == -1 ) ? "Promo" : "Offer" );
			bWantHeader = TRUE;
		}
		else if ( TRUE == bWantHeader )
		{
			if (MMITEMCONSOL_OFFER_HEADER == Sales.m_nOfferLineType)
			{
				CString strHeader = "";

				if (-1 == Sales.m_nMixMatchType)
				{
					int nPromoIdx;
					CString strPromoName = "";
					if (DataManager.Promotion.FindTableByNumber(Sales.m_nOfferNo, nPromoIdx) == TRUE)
					{
						CPromotionsCSVRecord Promo;
						DataManager.Promotion.GetAt(nPromoIdx, Promo);
						strPromoName = Promo.GetDisplayName();
					}

					if (strPromoName == "")
					{
						strPromoName.Format("Promo %d", Sales.m_nOfferNo);
					}

					strHeader.Format("<..>P%2.2d: %s", Sales.m_nOfferNo, (const char*)strPromoName);
				}
				else
				{
					CString strType = "";
					::GetMixMatchOfferTypeName(Sales.m_nMixMatchType, strType, FALSE, "Unknown Mix Match Type");

					int nMixMatchIdx = 0;
					CString strMixMatchName = "";
					if (DataManager.MixMatch.FindTableByNumber(Sales.m_nOfferNo, nMixMatchIdx) == TRUE)
					{
						CMixMatchCSVRecord MixMatch;
						DataManager.MixMatch.GetAt(nMixMatchIdx, MixMatch);
						strMixMatchName = MixMatch.GetDisplayName();
					}

					if (strMixMatchName == "")
					{
						strMixMatchName.Format("Mix Match %d", Sales.m_nOfferNo);
					}

					CString strExtraTab = "";
					switch (m_nReportType)
					{
					case REPORT_TYPE_MIXMATCH_ITEM:
					case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
						strExtraTab = "\t";
						break;
					}

					CString strUseCount = "";

					{
						CEposReportConsolMMUsageCount infoUsage;
						infoUsage.m_nOfferNo = Sales.m_nOfferNo;
						infoUsage.m_nOfferType = Sales.m_nMixMatchType;

						int nPos = 0;
						if (Block.m_pMMUsageCount->Find(infoUsage, nPos) == TRUE)
						{
							Block.m_pMMUsageCount->GetAt(nPos, infoUsage);
							strUseCount.Format(", Use Count %d",
								infoUsage.m_nCount);
						}
					}

					strHeader.Format("<..>M%2.2d: %s\t\t\t%s<AR><..>(%s%s)", Sales.m_nOfferNo,
						(const char*)strMixMatchName,
						(const char*)strExtraTab,
						(const char*)strType,
						(const char*)strUseCount);
				}

				m_ReportFile.RequestBlankLines(2);
				m_ReportFile.WriteReportMiscLine(strHeader);
				m_ReportFile.WriteReportMiscLine( "<LI>" );

				nOfferCovers = Sales.m_nOfferCovers;
				bWantHeader = FALSE;
			}
		}
		else if ( MMITEMCONSOL_OFFER_ITEM == Sales.m_nOfferLineType )
		{
			CCSV csvOut( '\t' );
			
			switch( m_nReportType )
			{
			case REPORT_TYPE_MIXMATCH_DEPT:
			case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
				{
					CString strDeptText = "";

					int nDeptIdx = 0;
					if ( DataManager.Department.FindDeptByNumber( (int)Sales.m_nItemNo, nDeptIdx ) == TRUE )
					{
						CDepartmentCSVRecord dept;
						DataManager.Department.GetAt( nDeptIdx, dept );
						strDeptText = dept.GetReportText();
					}
					else
					{
						strDeptText.Format("Dept %d", (int)Sales.m_nItemNo);
					}

					csvOut.Add( strDeptText );
				}
				break;

			case REPORT_TYPE_MIXMATCH_GROUP:
			case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
				{
					CString strGroupText = "";

					int nGroupIdx = 0;
					if ( DataManager.EposGroup.FindGroupByNumber( (int) Sales.m_nItemNo, nGroupIdx ) == TRUE )
					{
						CGroupCSVRecordEpos group;
						DataManager.EposGroup.GetAt( nGroupIdx, group );
						strGroupText = group.GetReportText();
					}
					else
					{
						strGroupText.Format("Group %d", (int)Sales.m_nItemNo);
					}

					csvOut.Add( strGroupText );
				}
				break;

			case REPORT_TYPE_MIXMATCH_ITEM:
			default:
				{
					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = Sales.m_nItemNo;
					::ProcessPluNo( infoPluNo, FALSE, TRUE );

					CString strPluNo = ReportHelpers.GetDisplayPluNo( infoPluNo.m_nBasePluNo, TRUE );
					if ( infoPluNo.m_nModifier != 0 )
					{
						CString strExtra;
						strExtra.Format( " + %d", infoPluNo.m_nModifier );
						strPluNo += strExtra;
					}
					
					m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo );
					
					csvOut.Add ( strPluNo );
					csvOut.Add ( m_PluInfoFinder.GetPluInfoRepText(TRUE) );
				}
				break;
			}

			csvOut.Add( Sales.m_nTranQty );
			csvOut.Add( Sales.m_dItemQty, SysInfo.GetDPQty() );
			csvOut.Add( Sales.m_dVal, SysInfo.GetDPValue() );

			if ( ( TRUE == m_bWantCovers ) && ( nOfferCovers != 0 ) )
			{
				csvOut.Add( nOfferCovers );
				csvOut.Add( Sales.m_dVal / (double) nOfferCovers, SysInfo.GetDPValue() );
			}

			m_ReportFile.WriteReportDataLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

void CEposReportMixMatchItem::WriteTotalLine( CEposReportConsolMMDiscountedItem& Sales, const char* szType )
{
	CString strTotal = szType;
	strTotal += " Total";

	CCSV csvOut( '\t' );

	switch( m_nReportType )
	{
	case REPORT_TYPE_MIXMATCH_DEPT:
	case REPORT_TYPE_MIXMATCH_GROUP:
	case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
	case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
		break;

	case REPORT_TYPE_MIXMATCH_ITEM:
	case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
	default:
		csvOut.Add( "" );
		break;
	}

	csvOut.Add( strTotal );
	csvOut.Add( "" );
	csvOut.Add( "" );
	csvOut.Add( Sales.m_dVal, SysInfo.GetDPValue() );

	if ( ( TRUE == m_bWantCovers ) && ( Sales.m_nOfferCovers != 0 ) )
	{
		csvOut.Add( Sales.m_nOfferCovers );
		csvOut.Add( Sales.m_dVal / (double) Sales.m_nOfferCovers, SysInfo.GetDPValue() );
	}

	m_ReportFile.WriteReportMiscLine ( csvOut.GetLine() );
}

/**********************************************************************/
