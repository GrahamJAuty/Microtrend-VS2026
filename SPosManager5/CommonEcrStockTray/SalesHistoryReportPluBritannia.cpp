/**********************************************************************/
#include "EposReports.h"
#include "PluSalesHistoryFileArray.h"
#include "PMSOptions.h"
#include "ReportHelpers.h"
#include "ReportPointCSVArray.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportPlu.h"
/**********************************************************************/

void CSalesHistoryReportPlu::ResetForBritannia( CLocationSelectorEntity& LocSelEntity, bool bGuestToTerminal )
{
	m_arrayReportFields.RemoveAll();

	m_nReportType = SH_CUSTOM;
	m_strReportName = "CSV Sales Export for Britannia Hotels";

	m_bEposSales = TRUE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;
	m_bGuestToTerminal = bGuestToTerminal;

	LocSelEntity.GetEPOSSelectArray( m_SelectArray );
	m_nConLevel = LocSelEntity.GetConType();
	m_nTableNo = LocSelEntity.GetEntityNo();
	m_nDbIdx = LocSelEntity.GetDbIdx();

	m_SelectArray.MakeList( TRUE );

	m_bEODMode = FALSE;
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);

	m_bEstimatedProfit = FALSE;
	m_bPercentSale = FALSE;
	m_nPercentDepth = 0;
	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	m_bVIPSales = FALSE;

	m_arrayBufferedLines.RemoveAll();
	m_arraySalesChart.RemoveAll();
	m_arrayUnsoldItems.RemoveAll();
	m_QtyValueTable.ClearConsolidatedLevels();
	m_QtyValueTable.ClearFieldStructure();
}

/**********************************************************************/

void CSalesHistoryReportPlu::ConsolidateForBritannia( CBritanniaHotelsExportArray& ExportArray ) 
{
#ifdef STOCKMAN_SYSTEM
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbReportpoint.BuildSortIdxTables();

	m_QtyValueTable.ClearConsolidatedLevels();
	
	CPluSalesHistoryFileArray arrayFileInfo( m_SelectArray );
	arrayFileInfo.SetEposSalesFlag( m_bEposSales );
	arrayFileInfo.SetImportSalesFlag( m_bImportSales );
	arrayFileInfo.SetManualSalesFlag( m_bManualSales );
	arrayFileInfo.SetEODModeFlag( m_bEODMode );
	arrayFileInfo.SetGuestToTerminalFlag( m_bGuestToTerminal );
	arrayFileInfo.SetSaleTimeFlag( FALSE );
	arrayFileInfo.BuildFileList();

	//INCLUDE ALL ITEMS IN EXPORT IF REQUIRED
	if ( DealerFlags.GetBritanniaAllItemsFlag() == TRUE )
	{
		for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			__int64 nSSPluNo = PluRecord.GetPluNoInt();

			if ( SysInfo.IsModifiableSSPluNo( nSSPluNo ) == FALSE )
			{
				int nDeptNo = PluRecord.GetBaseDeptNo();

				if (m_ReportFilters.DeptFilter.CheckDepartment(nDeptNo) == TRUE)
				{
					CBritanniaHotelsExportInfo infoSale;
					infoSale.m_nBasePluNo = nSSPluNo;
					infoSale.m_nModifier = 0;
					infoSale.m_nDeptNo = nDeptNo;
					infoSale.m_nEposPluNo = nSSPluNo;

					ExportArray.Consolidate(infoSale);
				}
			}
			else
			{
				for ( int nMod = 0; nMod <= 9; nMod++ )
				{
					if ( ( 0 == nMod ) || ( PluRecord.GetModifierEnable( nMod ) == TRUE ) )
					{
						int nDeptNo = PluRecord.GetModifierDeptNoForReport(nMod);

						if (m_ReportFilters.DeptFilter.CheckDepartment(nDeptNo) == TRUE)
						{
							CBritanniaHotelsExportInfo infoSale;
							infoSale.m_nBasePluNo = nSSPluNo;
							infoSale.m_nModifier = nMod;
							infoSale.m_nEposPluNo = ( nSSPluNo * 10 ) + nMod;
							infoSale.m_nDeptNo = nDeptNo;
							
							ExportArray.Consolidate(infoSale);
						}
					}
				}
			}
		}
	}

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetFileCount(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetFileInfo( nFileIdx, infoFile );

		CString strCalendarDate = infoFile.m_strDateTran;
		
		COleDateTime oleCalendarDate = COleDateTime ( 
			atoi ( strCalendarDate.Left(4) ), 
			atoi ( strCalendarDate.Mid(4,2) ), 
			atoi ( strCalendarDate.Right(2) ), 
			0, 0, 0 );

		if (oleCalendarDate.m_status != COleDateTime::valid)
		{
			continue;
		}

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
		{
			continue;
		}

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		CPluSalesLine csvSales( infoFile.IsLegacySalesFolder() );

		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED FOR TAX AND GROUP INFO
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

		int nLinesToRead = 0;
		CString strBuffer = "";
		bool bGotSalesToday = FALSE;
		CBusinessDateInfo BusinessDateInfo;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), arrayFileInfo.GetTotalFileSize() );

				csvSales.ParseLine( strBuffer );

				//IGNORE WASTAGE
				if (csvSales.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csvSales.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if (SysInfo.IsPMSSystem() == TRUE)
				{
					if (FALSE == m_bGuestToTerminal)
					{
						if (DataManager.Payment.IsPMSPaymentType(csvSales.GetPluPaymentNo()))
						{
							continue;
						}
					}
				}

				int nLevel = csvSales.GetPriceBand() + 1;
				if ((nLevel < 1) || (nLevel > MAX_PLU_PRICES))
				{
					nLevel = 1;
				}

				//IGNORE SALES FROM BANDS OUTSIDE BRITANNIA RANGE
				if ((nLevel < 1) || (nLevel > DealerFlags.GetMaxBritanniaPriceLevel()))
				{
					continue;
				}

				//FILTER BY DATE AND TIME
				CString strSaleTime = "";
				if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == FALSE )
				{
					strSaleTime = csvSales.GetTranTime();
				
					switch( strSaleTime.GetLength() )
					{
					case 6:
						break;

					case 4:
						strSaleTime += "00";
						break;

					default:
						strSaleTime = "000000";
						break;
					}
				}

#ifdef COMPILE_PMS

				if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == TRUE )
				{	
					strSaleTime.Format( "%2.2d%2.2d00",
						PMSOptions.GetEposReportHour(),
						PMSOptions.GetEposReportMinute() );
				}

#endif
				//CHECK FOR PREVIOUS DATE
				//NO NEED TO CHECK REST OF PMS FILE IF TIME IS REJECTED
				if (BusinessDateInfo.ValidateBusinessDate(infoFile.m_nLocIdx, m_bEODMode, strCalendarDate, strSaleTime) == FALSE)
				{
					if (dbLocation.IsPMSPseudoLocation(infoFile.m_nLocIdx) == TRUE)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				
				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = csvSales.GetPluNoNew();
				::ProcessPluNo( infoPluNo, FALSE, TRUE );
				
				if (FALSE == infoPluNo.m_bValid)
				{
					continue;
				}

				int nPluIdx = 0;
				int nDeptNo = 0;
				if (DataManager.Plu.FindPluByNumber(infoPluNo.m_nBasePluNo, nPluIdx) == TRUE)
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt(nPluIdx, PluRecord);
					
					if (infoPluNo.m_nModifier == 0)
					{
						nDeptNo = PluRecord.GetBaseDeptNo();
					}
					else
					{
						nDeptNo = PluRecord.GetModifierDeptNoForReport(infoPluNo.m_nModifier);
					}
				}

				if (m_ReportFilters.DeptFilter.CheckDepartment(nDeptNo) == FALSE)
				{
					continue;
				}

				double dLineQty = csvSales.GetSaleQty();
				double dLineVal = csvSales.GetValue();

				switch ( csvSales.GetLineType() )
				{
				case CSVPLUDATA_PLU_SALE:
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					dLineVal *= -1;
					break;

				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
					continue;
				}

				CBritanniaHotelsExportInfo infoSale;
				infoSale.m_nDeptNo = nDeptNo;
				infoSale.m_nEposPluNo = infoPluNo.m_nEposPluNo;
				infoSale.m_nBasePluNo = infoPluNo.m_nBasePluNo;
				infoSale.m_nModifier = infoPluNo.m_nModifier;
				infoSale.m_dPriceQty[ nLevel - 1 ] = dLineQty;
				infoSale.m_dPriceValue[ nLevel - 1 ] = dLineVal;
				ExportArray.Consolidate( infoSale );

				bGotSalesToday = TRUE;
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();

		if ( bGotSalesToday == TRUE )
		{
			if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
			{
				m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, oleCalendarDate );
				m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleCalendarDate );
			}
		}
	}
	StatusProgress.Unlock();
#endif
}

/**********************************************************************/

void CSalesHistoryReportPlu::CreateReportForBritannia(int nLocIdx, CBritanniaHotelsExportArray& ExportArray, CWnd* pParentWnd)
{
#ifdef STOCKMAN_SYSTEM
	StatusProgress.Lock(TRUE, "Finalising Variance Values");

	int nPriceBand = dbLocation.GetBritanniaDefaultPriceBand(nLocIdx);
	if ((nPriceBand < 1) || (nPriceBand > MAX_PLU_PRICES))
	{
		nPriceBand = 1;
	}

	int nSize = ExportArray.GetSize();
	for (int nIdx = 0; nIdx < nSize; nIdx++)
	{
		StatusProgress.SetPos(nIdx, nSize);

		CBritanniaHotelsExportInfo info;
		ExportArray.GetAt(nIdx, info);

		int nMod = info.m_nModifier;

		int nPluIdx;
		if (DataManager.Plu.FindPluByNumber(info.m_nBasePluNo, nPluIdx) == TRUE)
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt(nPluIdx, PluRecord);

			info.m_strDescription = PluRecord.GetModifierEposText(nMod);
			if ("" == info.m_strDescription)
			{
				info.m_strDescription = PluRecord.GetReportText();
			}

			info.m_dDefaultPriceRetail = PluRecord.GetModifierPrice(nMod, nPriceBand - 1);
		}
		else
		{
			info.m_strDescription = "Unknown";
			info.m_dDefaultPriceRetail = 0.0;
		}

		ExportArray.SetAt(nIdx, info);
	}

	StatusProgress.Unlock();

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strFilename;
	strFilename.Format("Exports\\BH_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.csv",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	::GetSyssetProgramPath(strFilename);

	CSSFile fileExport;
	if (fileExport.Open(strFilename, "wb") == FALSE)
	{
		Prompter.Error("Unable to create Export File");
		return;
	}

	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		Prompter.Error("Unable to create Export File");
		return;
	}

	ReportFile.SetStyle1("CSV Sales Export for Britannia Hotels");
	ReportFile.AddColumn("Plu No", TA_LEFT, 350);
	ReportFile.AddColumn("Description", TA_LEFT, 350);
	ReportFile.AddColumn("Dept No", TA_LEFT, 200);
	ReportFile.AddColumn("Dept Name", TA_LEFT, 350);
	ReportFile.AddColumn("Tilled Sales (Units)", TA_RIGHT, 330);
	ReportFile.AddColumn("Tilled Sales (£'s)", TA_RIGHT, 330);
	ReportFile.AddColumn("Price Allowance", TA_RIGHT, 330);
	ReportFile.AddColumn("Stock Price", TA_RIGHT, 250);

	StatusProgress.Lock(TRUE, "Creating Export File");

	double dGrandTotalQty = 0.0;
	double dGrandTotalVal = 0.0;
	double dGrandTotalAllowance = 0.0;

	for (int nIdx = 0; nIdx < nSize; nIdx++)
	{
		StatusProgress.SetPos(nIdx, nSize);

		CBritanniaHotelsExportInfo info;
		ExportArray.GetAt(nIdx, info);

		double dTotalQty = 0.0;
		double dTotalVal = 0.0;

		for (int n = 0; n < 10; n++)
		{
			dTotalQty += info.m_dPriceQty[n];
			dTotalVal += info.m_dPriceValue[n];
		}

		double dVariance = (dTotalQty * info.m_dDefaultPriceRetail) - dTotalVal;

		CString strPluNoReport = ReportHelpers.GetDisplayPluNo(info.m_nEposPluNo, TRUE);
		CString strPluNoExport = strPluNoReport;
		::TrimSpaces(strPluNoExport, TRUE);

		CCSV csv;
		csv.Add(strPluNoExport);
		csv.Add(info.m_nDeptNo);
		csv.Add(dTotalQty, 3);
		csv.Add(dTotalVal, 2);
		csv.Add(info.m_dDefaultPriceRetail, 2);
		csv.Add(info.m_strDescription);
		fileExport.WriteLine(csv.GetLine());

		CCSV csv2('\t');
		csv2.Add(strPluNoReport);
		csv2.Add(info.m_strDescription);
		csv2.Add(info.m_nDeptNo);
		csv2.Add(DataManager.Department.GetReportTextByDeptNo(info.m_nDeptNo));
		csv2.Add(dTotalQty, 3);
		csv2.Add(dTotalVal, 2);
		csv2.Add(dVariance, 2);
		csv2.Add(info.m_dDefaultPriceRetail, 2);
		ReportFile.WriteLine(csv2.GetLine());

		dGrandTotalQty += dTotalQty;
		dGrandTotalVal += dTotalVal;
		dGrandTotalAllowance += dVariance;
	}

	ReportFile.WriteLine("\t\t\t\t<LI>\t<LI>\t<LI>");

	CCSV csvTotal('\t');
	csvTotal.Add("");
	csvTotal.Add("");
	csvTotal.Add("");
	csvTotal.Add("");
	csvTotal.Add(dGrandTotalQty, 3);
	csvTotal.Add(dGrandTotalVal, 2);
	csvTotal.Add(dGrandTotalAllowance, 2);
	ReportFile.WriteLine(csvTotal.GetLine());
	ReportFile.WriteLine("");

	StatusProgress.Unlock();
	ReportFile.Close();
	WriteTerminalList();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::BritanniaExportPrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", pParentWnd);
#endif
}

/**********************************************************************/
