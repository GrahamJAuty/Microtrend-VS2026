/**********************************************************************/
#include "PosTrayDateRange.h"
#include "PosTrayOptions.h"
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
#include "SalesHistoryReportPlu.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayReportCreatorHistory.h"
/**********************************************************************/

CPosTrayReportCreatorHistory::CPosTrayReportCreatorHistory( CPosTrayTask& Task ) : m_Task( Task )
{
	m_nSkippedReportsEmpty = 0;
	m_nSkippedReportsNoChange = 0;

	//EMAIL FOLDER
	{
		m_strEmailFolder = "PosTray\\Email";
		::GetSyssetProgramPath( m_strEmailFolder );
		::CreateSubdirectory( m_strEmailFolder );
		m_strEmailFolder += "\\PLUHISTORY";
		::CreateSubdirectory( m_strEmailFolder );
	}

	//PRINT FOLDER
	{
		m_strPrintFolder = "PosTray\\Print";
		::GetSyssetProgramPath( m_strPrintFolder );
		::CreateSubdirectory( m_strPrintFolder );

		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_strPrintFilenamePrefix.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_2%3.3d_",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			m_Task.GetTaskNo() );

		m_nNextPrintFileNumber = 1;
	}

	//EXPORT FOLDER
	{
		m_strExportFolderTarget = m_Task.GetTaskExportFolder();
		::TrimSpaces( m_strExportFolderTarget, FALSE );
		
		if ( m_strExportFolderTarget == "" )
		{
			m_strExportFolderTarget = "PosTray\\Exports";
			::GetSyssetProgramPath( m_strExportFolderTarget );
		}

		::CreateSubdirectory( m_strExportFolderTarget );

		m_strExportFolderPending = "PosTray\\Pending";
		::GetSyssetProgramPath( m_strExportFolderPending );
		::CreateSubdirectory( m_strExportFolderPending );
		m_strExportFolderPending += "\\PluHistory";
		::CreateSubdirectory( m_strExportFolderPending );
	}

	//MD5CACHE
	m_strMD5CacheFolder = "";
	if (PosTrayOptions.GetSkipNoChangeFlag() == TRUE)
	{
		m_strMD5CacheFolder = "PosTray\\MD5Cache";
		::GetSyssetProgramPath(m_strMD5CacheFolder);
		::CreateSubdirectory(m_strMD5CacheFolder);
	}
}

/**********************************************************************/

CPosTrayReportCreatorHistory::~CPosTrayReportCreatorHistory(void)
{
}

/**********************************************************************/

void CPosTrayReportCreatorHistory::RoundDates()
{
	switch ( m_Task.GetBatchDateRoundTo() )
	{
	case 1:	//ROUND TO WEEK
		{
			int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
				
			int nWeekDayFrom = m_oleDateFrom.GetDayOfWeek() - 1;
			int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
			if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
			m_oleDateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

			int nWeekDayTo = m_oleDateTo.GetDayOfWeek() - 1;
			int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
			if ( nOffsetTo < 0 ) nOffsetTo += 7;
			m_oleDateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
		}
		break;

	case 2:	//ROUND TO MONTH
		{
			int nYearFrom = m_oleDateFrom.GetYear();
			int nMonthFrom = m_oleDateFrom.GetMonth();
			m_oleDateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
		
			int nYearTo = m_oleDateTo.GetYear();
			int nMonthTo = m_oleDateTo.GetMonth();
				
			nMonthTo++;
			if ( nMonthTo > 12 )
			{
				nYearTo++;
				nMonthTo = 1;
			}

			m_oleDateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
			m_oleDateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
		}

	default:
		break;
	}
}

/**********************************************************************/

bool CPosTrayReportCreatorHistory::ProcessTask( COleDateTime& dateTask )
{
	m_dateTask = dateTask;

	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenSalesHistoryPlu( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
	}

	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	
	bool bResult = ProcessTaskInternal();

	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseSalesHistoryPlu( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSystemPluFilterNames( info );
		DataManagerNonDb.CloseSystemPluFilterItems( info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
	}

	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.CustomerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	
	return bResult;
}

/**********************************************************************/

bool CPosTrayReportCreatorHistory::ProcessTaskInternal()
{
	COleDateTime oleDummy;
	CPosTrayDateRange dateRange;
	
	if (PosTrayOptions.GetBusinessDateFlag() == TRUE)
	{
		dateRange.SetDateToday(m_dateTask, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute());
	}
	else
	{
		dateRange.SetDateToday(m_dateTask);
	}

	dateRange.GetRange( m_Task.GetBatchDateType(), m_oleDateFrom, m_oleDateTo, oleDummy );
	RoundDates();

	{
		CString strSession = m_Task.GetBatchSessionEntity();
		if (strSession == "")
		{
			strSession = m_Task.GetBatchSessionLegacy();
		}

		m_ReportSession.ClearRecord();

		if ( strSession == "EOD" )
		{
			m_ReportSession.SetEODFlag( TRUE );
		}
		else
		{
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord arrayRecord;
				DataManagerNonDb.ReportSession.GetAt( n, arrayRecord );

				if ( strSession == arrayRecord.GetEntityID() )
				{
					DataManagerNonDb.ReportSession.GetAt( n, m_ReportSession );				
					break;
				}
			}
		}

		CString strName = m_ReportSession.GetName();
		if ( strName == "" )
		{
			strName.Format ( "%2.2d:%2.2d to %2.2d:%2.2d",
				m_ReportSession.GetSessionStartHour(),
				m_ReportSession.GetSessionStartMinute(),
				m_ReportSession.GetSessionEndHour(),
				m_ReportSession.GetSessionEndMinute() );

			m_ReportSession.SetName( strName );
		}
	}

	CTimeSliceMapCSVRecord TimeSliceRecord;
	{
		CString strTimeSlice = m_Task.GetBatchTimeSlice();

		for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
		{
			CTimeSliceMapCSVRecord arrayRecord;
			DataManagerNonDb.TimeSliceList.GetAt( n, arrayRecord );

			if ( strTimeSlice == arrayRecord.GetEntityID() )
			{
				DataManagerNonDb.TimeSliceList.GetAt( n, TimeSliceRecord );				
				break;
			}
		}
	}

	CCSV csvReports( m_Task.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayEntityIDs;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayEntityIDs.Consolidate( item );
	}

	int nExtraReportID = 1;
	CReportConsolidationArray<CSortedStringItem> arrayExportLabels;

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	m_Task.CreateEmailReportMap(arrayEmailByReport);

	for (int n = 0; n < DataManagerNonDb.SalesHistoryPlu.GetSize(); n++)
	{
		CSalesHistoryReportPlu HistoryReport;
		CSalesHistoryCSVRecordPlu ReportRecord;

		DataManagerNonDb.SalesHistoryPlu.GetAt(n, ReportRecord);

		CSortedStringItem item;
		item.m_strItem = ReportRecord.GetEntityID();

		int nPos;
		if (arrayEntityIDs.Find(item, nPos) == FALSE)
		{
			continue;
		}

		int nDbIdx = 0;
		int nDbNo = m_Task.GetBatchConDbNo();
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
		{
			nDbIdx = 0;
		}

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);

		int nReportType = ReportRecord.GetReportType();
		if (nReportType == SH_PLU_TOP100)
		{
			ReportRecord.SetReportMode(SALESHISTORY_REPORTMODE_CHART);
		}

		if (m_Task.GetBatchDeptSortFlag() && m_Task.GetBatchMonthSortFlag())
		{
			switch (nReportType)
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_MONTH_DEPT;	break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_MONTH_DEPT;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_MONTH_DEPT;	break;
			}
		}
		else if (m_Task.GetBatchMonthSortFlag())
		{
			switch (nReportType)
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_MONTH;	break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_MONTH;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_MONTH;	break;
			case SH_PLU_DEPT:	nReportType = SH_PLU_DEPT_MONTH;	break;
			}
		}
		else if (m_Task.GetBatchDeptSortFlag())
		{
			switch (nReportType)
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_DEPT;	break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_DEPT;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_DEPT;	break;
			}
		}
		ReportRecord.SetReportType(nReportType);

		HistoryReport.Reset(ReportRecord, m_Task);
		HistoryReport.SetReportDates(m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord);
		HistoryReport.SetSessionTitle(m_ReportSession.GetName());

		if (m_Task.GetBatchPluRangeFlag() == TRUE)
		{
			HistoryReport.SetPluRangeFlag(TRUE);
			HistoryReport.SetPluFrom(m_Task.GetBatchPluNoFrom());
			HistoryReport.SetPluTo(m_Task.GetBatchPluNoTo());
		}

		bool bReportShowsProfit = FALSE;
		bool bReportShowsPercent = FALSE;

		if (ReportRecord.CanDoPercentSale())
		{
			bReportShowsPercent = m_Task.GetBatchPercentageFlag();
		}

		if (ReportRecord.CanDoEstimatedProfit())
		{
			bReportShowsProfit = m_Task.GetBatchEstProfitFlag();
		}

		HistoryReport.SetPercentSaleFlag(bReportShowsPercent);
		HistoryReport.SetEstimatedProfitFlag(bReportShowsProfit);
		HistoryReport.SetCustomerFlag(m_Task.GetBatchCustomerOnlyFlag());
		HistoryReport.SetVIPSalesFlag(m_Task.GetBatchVIPOnlyFlag());

		{
			int nConType = m_Task.GetBatchConType();

			HistoryReport.m_ReportFilters.PluFilter.SetConsolidationType(nConType);
			HistoryReport.m_ReportFilters.PluFilter.SetMasterFilterNo(m_Task.GetBatchPluFilter());

			int nDepartmentFilter = m_Task.GetBatchDeptFilter();
			if (0 == nDepartmentFilter) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;
			HistoryReport.m_ReportFilters.DeptFilter.SetConsolidationType(nConType);
			HistoryReport.m_ReportFilters.DeptFilter.SetDeptFilterNo(nDepartmentFilter);
		}

		HistoryReport.SetDepartmentByItemFlag(m_Task.GetBatchDeptFilterType() == 1);
		HistoryReport.SetSaleTimeFlag(m_Task.GetBatchSaleTimeFlag());

		int nTaxFilter = TAXBAND_FILTER_ALL;

		{
			int nTaxFilterType = m_Task.GetBatchTaxBandFilter();

			if ((nTaxFilterType >= 1) && (nTaxFilterType <= MAX_TAX_BANDS + 1))
			{
				nTaxFilter = 1 << (nTaxFilterType - 1);
			}
			else if (999 == nTaxFilterType)
			{
				nTaxFilter = m_Task.GetBatchTaxBandCustom();
			}
		}

		HistoryReport.SetTaxFilter(nTaxFilter);

		int nPriceFilter = PRICELEVEL_FILTER_ALL;
		switch (m_Task.GetBatchPriceLevelFilter())
		{
		case 1:		nPriceFilter = 1;		break;
		case 2:		nPriceFilter = 2;		break;
		case 3:		nPriceFilter = 4;		break;
		case 4:		nPriceFilter = 8;		break;
		case 5:		nPriceFilter = 16;		break;
		case 6:		nPriceFilter = 32;		break;
		case 7:		nPriceFilter = 64;		break;
		case 8:		nPriceFilter = 128;		break;
		case 9:		nPriceFilter = 256;		break;
		case 10:	nPriceFilter = 512;		break;
		case 999:	nPriceFilter = m_Task.GetBatchPriceLevelCustom(); break;
		}
		HistoryReport.SetPriceLevelFilter(nPriceFilter);

		int nModifierFilter = MODIFIER_FILTER_ALL;
		switch (m_Task.GetBatchModifierFilter())
		{
		case 1:		nModifierFilter = 1;	break;
		case 2:		nModifierFilter = 2;	break;
		case 3:		nModifierFilter = 4;	break;
		case 4:		nModifierFilter = 8;	break;
		case 5:		nModifierFilter = 16;	break;
		case 6:		nModifierFilter = 32;	break;
		case 7:		nModifierFilter = 64;	break;
		case 8:		nModifierFilter = 128;	break;
		case 9:		nModifierFilter = 256;	break;
		case 10:	nModifierFilter = 512;	break;
		case 999:	nModifierFilter = m_Task.GetBatchModifierCustom(); break;
		}
		HistoryReport.SetModifierFilter(nModifierFilter);

		if (HistoryReport.PrepareFields() == FALSE)
		{
			continue;
		}

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		if (ReportRecord.GetReportMode() == SALESHISTORY_REPORTMODE_CHART)
		{
			ReportRecord.SetChartType(m_Task.GetBatchChartType());
			ReportRecord.SetBottom100Flag(m_Task.GetBatchChartBottom100Flag());
			ReportRecord.SetSortedByValueFlag(m_Task.GetBatchChartByValueFlag());
			ReportRecord.SetChartCutOff(m_Task.GetBatchChartCutOff());
			ReportRecord.SetChartSize(m_Task.GetBatchChartSize());
			HistoryReport.SetChartDetails(ReportRecord);
		}

		CString strTaskFilename = m_Task.GetTaskExportFilename();
		::TrimSpaces(strTaskFilename, FALSE);

		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			CTableFilter TableFilter;
			TableFilter.m_nType = m_Task.GetBatchTableFilterType();
			TableFilter.m_nLevel = m_Task.GetBatchTableFilterLevel();
			TableFilter.m_nEntityNo = m_Task.GetBatchTableFilterEntityNo();
			HistoryReport.SetTableFilter(TableFilter);
		}

		{
			int nTermDateType = m_Task.GetBatchTermDateType();

			if ((nTermDateType < 0) || (nTermDateType > 2))
			{
				nTermDateType = 0;
			}

			HistoryReport.SetTermDateType(nTermDateType);
		}

		HistoryReport.SetCreateMD5HashFlag(PosTrayOptions.GetSkipNoChangeFlag());
		HistoryReport.Consolidate();
		if (HistoryReport.CreatePluSalesHistoryReport() == TRUE)
		{
			if ((PosTrayOptions.GetSkipEmptyFlag() == TRUE) && (HistoryReport.GotDataLine() == FALSE))
			{
				m_nSkippedReportsEmpty++;
			}
			else
			{
				bool bSkipped = FALSE;

				if (PosTrayOptions.GetSkipNoChangeFlag() == TRUE)
				{
					CString strMD5Hash = HistoryReport.GetMD5Hash();

					CString strFilename = "";
					strFilename.Format("%s\\PLUHIST_%4.4d_%d_%s.dat",
						(const char*)m_strMD5CacheFolder,
						m_Task.GetTaskNo(),
						nReportType,
						ReportRecord.GetEntityID());

					{
						CSSFile fileMD5;
						if (fileMD5.Open(strFilename, "rb") == TRUE)
						{
							CString strPreviousMD5 = "";
							if (fileMD5.ReadString(strPreviousMD5) == TRUE)
							{
								if (strPreviousMD5 == strMD5Hash)
								{
									m_nSkippedReportsNoChange++;
									bSkipped = TRUE;
								}
							}
						}
					}

					if (FALSE == bSkipped)
					{
						CSSFile fileMD5;
						if (fileMD5.Open(strFilename, "wb") == TRUE)
						{
							fileMD5.WriteLine(strMD5Hash);
						}
					}
				}

				if (FALSE == bSkipped)
				{
					HistoryReport.WriteTerminalList();

					CReportFileDoc ReportFileDoc(Super.ReportFilename());

					int nEmailFormat = m_Task.GetTaskFormatEmail();
					if ((0 == nEmailFormat) || (1 == nEmailFormat))
					{
						int nEmailIdx = 1;
						{
							int nPos = 0;
							CSortedIntByString item;
							item.m_strItem = ReportRecord.GetEntityID();
							if (arrayEmailByReport.Find(item, nPos) == TRUE)
							{
								arrayEmailByReport.GetAt(nPos, item);
								nEmailIdx = item.m_nItem;
							}
						}

						if ((nEmailIdx >= 1) && (nEmailIdx <= 5))
						{
							CString strEmailFilename;
							strEmailFilename.Format("\\%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.%s",
								ReportRecord.GetEntityID(),
								timeNow.GetYear(),
								timeNow.GetMonth(),
								timeNow.GetDay(),
								timeNow.GetHour(),
								timeNow.GetMinute(),
								timeNow.GetSecond(),
								(1 == nEmailFormat) ? "htm" : "csv");

							CString strNKNFilename;
							strNKNFilename = strEmailFilename.Left(strEmailFilename.GetLength() - 3);
							strNKNFilename += "nkn";

							if (0 == nEmailFormat)
							{
								ReportFileDoc.SaveAsCSV(m_strEmailFolder + strEmailFilename);
							}
							else
							{
								CTabRuler TabRuler;
								CReportHelpers ReportHelpers;
								CFilenameUpdater FnUpParams(SysFiles::CustomHistPrm);
								CString strParamsFilename = FnUpParams.GetFilenameToUse();
								CString strParamsKey = ReportRecord.GetKey(bReportShowsPercent, bReportShowsProfit);
								ReportHelpers.CreateTabRuler(ReportFileDoc, strParamsFilename, strParamsKey, TabRuler);
								ReportFileDoc.SaveAsHTML(m_strEmailFolder + strEmailFilename, TabRuler);
							}

							CSSFile fileNKN;
							if (fileNKN.Open(m_strEmailFolder + strNKNFilename, "wb") == TRUE)
							{
								fileNKN.WriteLine(m_Task.GetTaskEmailNickname(nEmailIdx - 1));
								fileNKN.WriteLine(HistoryReport.GetEmailSubject());
								fileNKN.Close();
							}
						}
					}

					int nExportFormat = m_Task.GetTaskFormatExport();
					if ((0 == nExportFormat) || (1 == nExportFormat))
					{
						CString strFilenameBody = ReportRecord.GetExportFilename();
						::TrimSpaces(strFilenameBody, FALSE);

						if (strFilenameBody == "")
						{
							ReportRecord.GetDefaultExportFilename(arrayExportLabels, nExtraReportID, strFilenameBody);
						}

						CPosTrayTaskConsolInfo infoQuick;
						infoQuick.m_nQuickType = 0;

						CString strExportFilenameTarget = "";
						strExportFilenameTarget += m_Task.GetTaskExportFilename();
						strExportFilenameTarget += strFilenameBody;
						strExportFilenameTarget += m_Task.GetExportFilenameSuffix(m_dateTask, infoQuick);

						CString strSlash = "\\";
						CString strExportFilenamePending = "";
						strExportFilenamePending += strSlash;
						strExportFilenamePending += CSSMD5::CalcMD5(m_strExportFolderTarget + strSlash + strExportFilenameTarget);
						strExportFilenamePending += (1 == nExportFormat) ? ".htm" : ".csv";

						CString str001Filename;
						str001Filename = strExportFilenamePending.Left(strExportFilenamePending.GetLength() - 3);
						str001Filename += "001";

						CSSFile file001;
						if (file001.Open(m_strExportFolderPending + str001Filename, "wb") == TRUE)
						{
							file001.WriteLine(strExportFilenameTarget + strExportFilenamePending.Right(4));
							file001.WriteLine(m_Task.GetFilename(FALSE));
							file001.Close();
						}

						if (0 == nExportFormat)
						{
							ReportFileDoc.SaveAsCSV(m_strExportFolderPending + strExportFilenamePending);
						}
						else
						{
							CTabRuler TabRuler;
							CReportHelpers ReportHelpers;
							CFilenameUpdater FnUpParams(SysFiles::CustomHistPrm);
							CString strParamsFilename = FnUpParams.GetFilenameToUse();
							CString strParamsKey = ReportRecord.GetKey(bReportShowsPercent, bReportShowsProfit);
							ReportHelpers.CreateTabRuler(ReportFileDoc, strParamsFilename, strParamsKey, TabRuler);
							ReportFileDoc.SaveAsHTML(m_strExportFolderPending + strExportFilenamePending, TabRuler);
						}
					}

					if (TRUE == m_Task.GetTaskAutoPrintFlag())
					{
						CString strPrintFilename = "";
						strPrintFilename.Format("%s\\%s%6.6d",
							(const char*)m_strPrintFolder,
							(const char*)m_strPrintFilenamePrefix,
							m_nNextPrintFileNumber++);

						{
							CSSFile fileHelper;
							if (fileHelper.Open(strPrintFilename + ".dat", "wb") == TRUE)
							{
								CFilenameUpdater FnUp(SysFiles::CustomHistPrm);
								fileHelper.WriteLine(FnUp.GetFilenameToUse());
								fileHelper.WriteLine(ReportRecord.GetKey(HistoryReport.GetEstimatedProfitFlag(), HistoryReport.GetPercentSaleFlag()));
							}
						}

						::CopyFile(Super.ReportFilename(), strPrintFilename + ".rep", FALSE);
					}
				}
			}
		}
	}
	
	return TRUE;
}

/**********************************************************************/
