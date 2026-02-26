/**********************************************************************/
#include "CCSVReportLine.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#include "EposReports.h"
/**********************************************************************/

CEposReportCoversGroup::CEposReportCoversGroup( CEposSelectArray& SelectArray, bool bDeptMode ) : CEposReport( SelectArray )
{
	m_bDeptMode = bDeptMode;
	m_nReportType = (m_bDeptMode) ? REPORT_TYPE_COVERS_DEPT : REPORT_TYPE_COVERS_GROUP;
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailCoversFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailCovers();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxCoversFlag();
}

/**********************************************************************/

CEposReportCoversGroup::~CEposReportCoversGroup()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pAdjustPremium );
		delete ( m_arrayReportBlocks[n].m_pAdjustDiscount );
	}
}

/**********************************************************************/

void CEposReportCoversGroup::InitialiseConsolidationBlocks()
{
	bool bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalDeptGroupEposFlag() == TRUE ) ); 

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

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}
	
	if ( TRUE == Params.m_bConsolSys )
	{
		if ( ( NODE_SYSTEM != m_nConLevel ) || ( FALSE == bAllowGlobal ) )
		{
			Params.m_bConsolSys = FALSE;
			Params.m_bConsolDb = TRUE;
		}
	}
		
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( Params.m_bConsolSys );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportCoversDataPointBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolCoversDataPoint>;
		block.m_pAdjustPremium = new CReportConsolidationArray<CEposReportConsolCoversAdjust>;
		block.m_pAdjustDiscount = new CReportConsolidationArray<CEposReportConsolCoversAdjust>;
		block.m_nCoversCount = 0;
		block.m_BlockTotal.m_dValBeforeDiscount = 0.0;
		block.m_BlockTotal.m_dDiscountVal =  0.0;
		block.m_BlockTotal.m_dPremiumVal = 0.0;
	
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportCoversGroup::Consolidate()
{ 
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	int nLastSelectArrayIdx = -1;
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		if ( ( infoFile.m_nSelectArrayIdx != nLastSelectArrayIdx ) || ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) ) )
		{
			m_arrayTran.RemoveAll();
			nLastSelectArrayIdx = infoFile.m_nSelectArrayIdx;
		}

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		CString strLastSaleDate = strThisSaleDate;
		CString strLastTranTime = "";

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );
		
		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( m_PMSModes.CheckPMSTimeLine( bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude ) == FALSE )
					continue;
			
				csv.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csv.GetLineType();
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
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;

				//CHECK SALE DATE AND TIME
				if ( CheckPluSaleDateTime( bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime ) == FALSE )
					continue;

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastSaleDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				int nLineCovers = csv.GetCovers();
				if ( 0 == nLineCovers ) 
				{
					if ( DealerFlags.GetAssumeCoversFlag() == TRUE )
						nLineCovers = 1;
					else
						continue;
				}

				int nCovers = 0;
				CString strThisTranTime = csv.GetTranTime();
				if ( strThisTranTime != strLastTranTime )
				{
					CEposReportConsolTran infoTran;
					infoTran.m_strDateTran = infoFile.m_strDateTran;
					infoTran.m_strTimeTran = strThisTranTime;

					int nPos;
					if ( m_arrayTran.Find( infoTran, nPos ) == FALSE )
					{
						nCovers = nLineCovers;
						m_arrayTran.InsertAt( nPos, infoTran );
					}

					strLastTranTime = strThisTranTime;
				}
	
				CEposReportConsolCoversDataPoint ConsolInfo;

				if ( TRUE == m_bDeptMode )
				{
					ConsolInfo.m_nDataPoint = nDeptNo;
				}
				else
				{
					int	nGroupNo = csv.GetGroupNo() - 1;
					
					if ( nGroupNo < 0 )
					{
						nGroupNo = 0;
					}

					ConsolInfo.m_nDataPoint = nGroupNo;
				}

				ConsolInfo.m_nTaxDateCode = nTaxDateCode;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = 0;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = GetDatabaseTaxBand( csv, nLineType );
				
				int nSubtotalAdjustType = 0;
				double dSubtotalAdjustDiscount = 0.0;
				double dSubtotalAdjustPremium = 0.0;

				switch( nLineType )
				{
				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					if (csv.IsPremium() == TRUE)
					{
						dSubtotalAdjustPremium = csv.GetValue() * (-1);
						nSubtotalAdjustType = 2;
					}
					else
					{
						dSubtotalAdjustDiscount = csv.GetValue();
						nSubtotalAdjustType = 1;
					}
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;
				}
				
				if (0 == nSubtotalAdjustType)
				{		
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{	
						int nBlock = m_arrayTerminalBlocks.GetAt(n);

						m_arrayReportBlocks[ nBlock ].m_nCoversCount += nCovers;
						m_arrayReportBlocks[ nBlock ].m_BlockTotal.m_dValBeforeDiscount += ConsolInfo.m_dValBeforeDiscount;
						m_arrayReportBlocks[ nBlock ].m_BlockTotal.m_dDiscountVal += ConsolInfo.m_dDiscountVal;
						m_arrayReportBlocks[ nBlock ].m_BlockTotal.m_dPremiumVal += ConsolInfo.m_dPremiumVal;
						m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
					}
				}
				else
				{
					CEposReportConsolCoversAdjust infoAdjust;
					infoAdjust.m_nTaxDateCode = ConsolInfo.m_nTaxDateCode;
					infoAdjust.m_nTaxBand = ConsolInfo.m_nTaxBand;
					
					if (2 == nSubtotalAdjustType)
					{
						infoAdjust.m_dValue = dSubtotalAdjustPremium;
					}
					else
					{
						infoAdjust.m_dValue = dSubtotalAdjustDiscount;
					}

					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);

						if (2 == nSubtotalAdjustType)
						{
							m_arrayReportBlocks[nBlock].m_pAdjustPremium->Consolidate(infoAdjust);
						}
						else
						{
							m_arrayReportBlocks[nBlock].m_pAdjustDiscount->Consolidate(infoAdjust);
						}
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCoversGroup::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( m_bDeptMode ? "Department" : "Group", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Total Spend", TA_RIGHT, 300 );

	if ( TRUE == m_bDiscountDetail )
	{
		m_ReportFile.AddColumn ( "Discount", TA_RIGHT, 300 );

		if (TRUE == m_bPremiumDetail)
		{
			m_ReportFile.AddColumn("Premium", TA_RIGHT, 300);
		}

		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, 300 );
	}

	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 300 );

	if ( TRUE == m_bNetOfTax )
		m_ReportFile.AddColumn ( "Est. Net", TA_RIGHT, 300 );

	m_ReportFile.AddColumn ( "Covers", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Per Head", TA_RIGHT, 300 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportCoversDataPointBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportCoversDataPointBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		CreateSalesSection( ReportBlock );
		
		if ((ReportBlock.m_pAdjustPremium->GetSize() == 0) && (ReportBlock.m_pAdjustDiscount->GetSize() == 0))
		{
			WriteBlockTotals(ReportBlock, FALSE, 0, 0, 0, 0);
		}
		else
		{
			double dAdjustSalesPremium, dAdjustSalesDiscount, dAdjustTaxPremium, dAdjustTaxDiscount;
			PostConsolidateAdjustTax( ReportBlock, dAdjustSalesPremium, dAdjustSalesDiscount, dAdjustTaxPremium, dAdjustTaxDiscount );

			WriteBlockTotals( ReportBlock, FALSE, 0, 0, 0, 0);

			m_ReportFile.WriteReportMiscLine( "" );

			if (TRUE == m_bPremiumDetail)
			{
				CCSVReportLine csv;
				csv.Add("SBTL Adjust");

				csv.AppendBlanks(1);
				csv.AppendVal(dAdjustSalesDiscount);
				csv.AppendVal(dAdjustSalesPremium);
				
				csv.AppendVal(dAdjustSalesPremium - dAdjustSalesDiscount);
				csv.AppendVal(dAdjustTaxPremium - dAdjustTaxDiscount);

				if (TRUE == m_bNetOfTax)
				{
					csv.AppendVal(dAdjustSalesPremium - dAdjustSalesDiscount - dAdjustTaxPremium + dAdjustTaxDiscount);
				}

				m_ReportFile.WriteReportDataLine(csv.GetLine());
			}
			else
			{
				if (ReportBlock.m_pAdjustPremium->GetSize() > 0)
				{
					CCSVReportLine csv;
					csv.Add("SBTL Adjust(Premium)");

					if (TRUE == m_bDiscountDetail)
					{
						csv.AppendBlanks(1);
						csv.AppendVal(-dAdjustSalesPremium);
					}

					csv.AppendVal(dAdjustSalesPremium);
					csv.AppendVal(dAdjustTaxPremium);

					if (TRUE == m_bNetOfTax)
					{
						csv.AppendVal(dAdjustSalesPremium - dAdjustTaxPremium);
					}

					m_ReportFile.WriteReportDataLine(csv.GetLine());
				}

				if (ReportBlock.m_pAdjustDiscount->GetSize() > 0)
				{
					CCSVReportLine csv;
					csv.Add("SBTL Adjust(Discount)");

					if (TRUE == m_bDiscountDetail)
					{
						csv.AppendBlanks(1);
						csv.AppendVal(dAdjustSalesDiscount);
					}

					csv.AppendVal(-dAdjustSalesDiscount);
					csv.AppendVal(-dAdjustTaxDiscount);

					if (TRUE == m_bNetOfTax)
					{
						csv.AppendVal(dAdjustTaxDiscount-dAdjustSalesDiscount);
					}

					m_ReportFile.WriteReportDataLine(csv.GetLine());
				}
			}

			WriteBlockTotals( ReportBlock, TRUE, dAdjustSalesPremium, dAdjustSalesDiscount, dAdjustTaxPremium, dAdjustTaxDiscount );
		}
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversGroup::PostConsolidateSalesTax( CEposReportCoversDataPointBlock& Block, CArray<CEposReportConsolCoversDataPointTax,CEposReportConsolCoversDataPointTax>& arraySales )
{
	arraySales.RemoveAll();

	CEposReportConsolCoversDataPointTax infoSales;
	infoSales.m_nDataPoint = -1;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolCoversDataPoint Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( Sales.m_nDataPoint != infoSales.m_nDataPoint )
		{
			if ( infoSales.m_nDataPoint != -1 )
				arraySales.Add( infoSales );
			
			infoSales.Reset();
			infoSales.m_nDataPoint = Sales.m_nDataPoint;
		}

		infoSales.m_nCoversCount += Sales.m_nCoversCount;
		infoSales.m_dValBeforeDiscount += Sales.m_dValBeforeDiscount;
		infoSales.m_dDiscountVal += Sales.m_dDiscountVal;
		infoSales.m_dPremiumVal += Sales.m_dPremiumVal;
		
		if ( Sales.m_nTaxBand != 0 )
		{
			double dNet = Sales.m_dValBeforeDiscount - Sales.m_dDiscountVal + Sales.m_dPremiumVal;;
			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( Sales.m_nTaxDateCode, Sales.m_nTaxBand );
			double dTaxAmount = ( dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate ) );
			infoSales.m_dTaxAmount += dTaxAmount;
			Block.m_BlockTotal.m_dTaxAmount += dTaxAmount;
		}
	}

	if ( infoSales.m_nDataPoint != -1 )
		arraySales.Add( infoSales );
}

/**********************************************************************/

void CEposReportCoversGroup::PostConsolidateAdjustTax( CEposReportCoversDataPointBlock& Block, double& dAdjustSalesUp, double& dAdjustSalesDown, double& dAdjustTaxUp, double& dAdjustTaxDown  )
{
	dAdjustSalesUp = 0.0;
	dAdjustSalesDown = 0.0;
	dAdjustTaxUp = 0.0;
	dAdjustTaxDown = 0.0;

	for ( int s = 0; s < Block.m_pAdjustPremium -> GetSize(); s++ )
	{
		CEposReportConsolCoversAdjust Adjust;
		Block.m_pAdjustPremium -> GetAt( s, Adjust );
		dAdjustSalesUp += Adjust.m_dValue;

		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( Adjust.m_nTaxDateCode, Adjust.m_nTaxBand );
		double dTaxAmount = ( Adjust.m_dValue - ReportHelpers.CalcNonTax ( Adjust.m_dValue, dTaxRate ) );
			
		dAdjustTaxUp += dTaxAmount;
	}

	for ( int s = 0; s < Block.m_pAdjustDiscount -> GetSize(); s++ )
	{
		CEposReportConsolCoversAdjust Adjust;
		Block.m_pAdjustDiscount -> GetAt( s, Adjust );
		dAdjustSalesDown += Adjust.m_dValue;

		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( Adjust.m_nTaxDateCode, Adjust.m_nTaxBand );
		double dTaxAmount = ( Adjust.m_dValue - ReportHelpers.CalcNonTax ( Adjust.m_dValue, dTaxRate ) );
			
		dAdjustTaxDown += dTaxAmount;
	}
}

/**********************************************************************/

void CEposReportCoversGroup::CreateSalesSection( CEposReportCoversDataPointBlock& Block )
{
	CArray<CEposReportConsolCoversDataPointTax,CEposReportConsolCoversDataPointTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );

	int nDPVal= SysInfo.GetDPValue();

	for ( int s = 0; s < arraySales.GetSize(); s++ )
	{
		CEposReportConsolCoversDataPointTax Sales = arraySales.GetAt(s);

		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineSpend = ( m_bNetOfTax ) ? dNET : dRetail;

		CString strText = "";
		
		if ( TRUE == m_bDeptMode )
		{
			int nDeptIdx;		
			if ( DataManager.Department.FindDeptByNumber( Sales.m_nDataPoint, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord dept;
				DataManager.Department.GetAt( nDeptIdx, dept );
				strText = dept.GetReportText();
			}
			else
			{
				strText.Format( "Dept %d", Sales.m_nDataPoint );
			}
		}
		else
		{
			int nGroupIdx;		
			if ( DataManager.EposGroup.FindGroupByNumber( Sales.m_nDataPoint, nGroupIdx ) == TRUE )
			{
				CGroupCSVRecordEpos group;
				DataManager.EposGroup.GetAt( nGroupIdx, group );
				strText = group.GetReportText();
			}
			else
			{
				strText.Format( "Group %d", Sales.m_nDataPoint );
			}
		}

		CCSV csv ( '\t' );
		csv.Add ( strText );

		if ( TRUE == m_bDiscountDetail )
		{
			csv.Add ( dGross, nDPVal );
			
			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(dDiscount, nDPVal);
				csv.Add(dPremium, nDPVal);
			}
			else
			{
				csv.Add(dDiscount - dPremium, nDPVal);
			}
		}
			
		csv.Add ( dRetail, nDPVal );
		csv.Add ( dTax, nDPVal );

		if (TRUE == m_bNetOfTax)
		{
			csv.Add(dNET, nDPVal);
		}

		csv.Add ( Block.m_nCoversCount );
		csv.Add ( ReportHelpers.CalcAverage ( Block.m_nCoversCount, dLineSpend ), nDPVal );
		
		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportCoversGroup::WriteBlockTotals( CEposReportCoversDataPointBlock& Block, bool bAfterDiscount, double dAdjustSalesPremium, double dAdjustSalesDiscount, double dAdjustTaxPremium, double dAdjustTaxDiscount )
{
	CString strType = GetTotalTypeName( TRUE, Block.m_nEntityType ); 
	strType += ( bAfterDiscount ) ? " Net Total" : " Total";

	int nDPVal = SysInfo.GetDPValue();

	double dGross = Block.m_BlockTotal.m_dValBeforeDiscount;
	double dDiscount = Block.m_BlockTotal.m_dDiscountVal;
	double dPremium = Block.m_BlockTotal.m_dPremiumVal;
	double dRetail = dGross - dDiscount + dPremium;
	double dTax = Block.m_BlockTotal.m_dTaxAmount;
	
	if (TRUE == bAfterDiscount)
	{
		dDiscount += dAdjustSalesDiscount;
		dPremium += dAdjustSalesPremium;
		dRetail += dAdjustSalesPremium - dAdjustSalesDiscount;
		dTax += dAdjustTaxPremium - dAdjustTaxDiscount;
	}

	double dNET = dRetail - dTax;
	double dTotalSpend = ( m_bNetOfTax ) ? dNET : dRetail;

	{
		CUnderlineMaker UnderlineMaker;
		UnderlineMaker.AddBlanks(1);
		UnderlineMaker.AddTotals(4);
		if ( TRUE == m_bDiscountDetail ) UnderlineMaker.AddTotals(2);
		if ( TRUE == m_bPremiumDetail ) UnderlineMaker.AddTotals(1);
		if ( TRUE == m_bNetOfTax ) UnderlineMaker.AddTotals(1);
		m_ReportFile.WriteReportMiscLine( UnderlineMaker.GetLine() );
	}

	CCSV csv ( '\t' );
	csv.Add ( strType );
		
	if ( TRUE == m_bDiscountDetail )
	{
		csv.Add ( dGross, nDPVal );
	
		if (TRUE == m_bPremiumDetail)
		{
			csv.Add(dDiscount, nDPVal);
			csv.Add(dPremium, nDPVal);
		}
		else
		{
			csv.Add(dDiscount - dPremium, nDPVal);
		}
	}
	
	csv.Add ( dRetail, nDPVal );
	csv.Add ( dTax, nDPVal );

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNET, nDPVal);
	}

	csv.Add ( Block.m_nCoversCount );
	csv.Add ( ReportHelpers.CalcAverage ( Block.m_nCoversCount, dTotalSpend ), nDPVal );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
