/**********************************************************************/
#include "..\CommonEcrStockTray\CashRSPImporterDefines.h"
#include "..\CommonEcrStockTray\LocationCSVArray.h"
#include "..\CommonEcrStockTray\LocationSelectorEntity.h"
#include "..\CommonEcrStockTray\NetworkCSVArray.h"
/**********************************************************************/
#include "PosTrayDateRange.h"
#include "PosTrayOptions.h"
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
#include "ReportTimeAndAttendance.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayReportCreatorTimeAttend.h"
/**********************************************************************/

CPosTrayReportCreatorTimeAttend::CPosTrayReportCreatorTimeAttend(COleDateTime& dateTask, CPosTrayTask& Task ) : m_Task( Task )
{
	m_dateTask = dateTask;

	m_strEmailDatePrefix.Format("%4.4d%2.2d%2.2d",
		m_dateTask.GetYear(),
		m_dateTask.GetMonth(),
		m_dateTask.GetDay());

	//EMAIL FOLDER
	{
		m_strEmailFolder = "PosTray\\Email";
		::GetSyssetProgramPath( m_strEmailFolder );
		::CreateSubdirectory( m_strEmailFolder );
		m_strEmailFolder += "\\TIMEATTEND";
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
		m_strExportFolderPending += "\\TimeAttend";
		::CreateSubdirectory( m_strExportFolderPending );
	}
}

/**********************************************************************/

CPosTrayReportCreatorTimeAttend::~CPosTrayReportCreatorTimeAttend(void)
{
}

/**********************************************************************/

void CPosTrayReportCreatorTimeAttend::RoundDates()
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

bool CPosTrayReportCreatorTimeAttend::ProcessTask()
{
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession(DB_READONLY, info);
		DataManagerNonDb.OpenTimeAttendReportNames(DB_READONLY, info);
	}

	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();

	bool bResult = ProcessTaskInternal();

	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseReportSession(info);
		DataManagerNonDb.CloseTimeAttendReportNames(info);
	}

	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();

	return bResult;
}

/**********************************************************************/

bool CPosTrayReportCreatorTimeAttend::ProcessTaskInternal()
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
		if ( strSession == "" )
			strSession = m_Task.GetBatchSessionLegacy();

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

	CCSV csvReports( m_Task.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayEntityIDs;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayEntityIDs.Consolidate( item );
	}

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorLookup LocSelLookup(m_Task);
	CLocationSelectorEntity LocSelEntity(LocSelLookup);
	LocSelEntity.GetEPOSSelectArray(m_SelectArray);
	m_SelectArray.MakeList(TRUE, TRUE);

	CStringArray FileList;
	CWordArray DbIdxList, LocIdxList, SelIdxList;
	__int64 nTotalFileSize;
	CReportTimeAndAttendance::GetFileList(m_SelectArray, FileList, DbIdxList, LocIdxList, SelIdxList, nTotalFileSize);

	CArray<int, int> arrayReportTypes;
	for (int nReportType = 0; nReportType <= 5; nReportType++)
	{
		CSortedStringItem item;
		item.m_strItem.Format("%d", nReportType);

		int nPos;
		if (arrayEntityIDs.Find(item, nPos) == TRUE)
		{
			arrayReportTypes.Add(nReportType);
		}
	}

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	m_Task.CreateEmailReportMap(arrayEmailByReport);

	for ( int x = 0; x < arrayReportTypes.GetSize(); x++ )
	{
		int nReportType = arrayReportTypes.GetAt(x);

		int nInternalReportType = 0;
		switch (nReportType)
		{
		case 0:	nInternalReportType = 0;	break;
		case 1:	nInternalReportType = 3;	break;
		case 2:	nInternalReportType = 4;	break;
		case 3:	nInternalReportType = 1;	break;
		case 4:	nInternalReportType = 2;	break;
		case 5:	nInternalReportType = 5;	break;
		}
		
		//ADD TITLE TO REPORT
		CString strDateFrom = m_oleDateFrom.Format("%d/%m/%Y");
		CString strDateTo = m_oleDateTo.Format("%d/%m/%Y");

		CString strConTitle = LocSelEntity.GetConsolidationTitle();

		CString strTitle;
		strTitle.Format(" ( %s, %s - %s, %s )",
			(const char*)strConTitle,
			(const char*)strDateFrom,
			(const char*)strDateTo,
			(const char*)m_ReportSession.GetName());

		CServerSelectionList ServerSelectionList;
		ServerSelectionList.BuildList();

		CReportTimeAndAttendance ServerReport(m_SelectArray, ServerSelectionList, ( m_Task.GetBatchTAGroup() == 1 ) );
		ServerReport.SetServerPickType(m_Task.GetBatchTAServerPickType());
		ServerReport.SetServerRange(m_Task.GetBatchTAServerPickFrom(),m_Task.GetBatchTAServerPickTo());
		ServerReport.SetTAReportTitleInfo(strTitle);
		ServerReport.SetDates(m_oleDateFrom, m_oleDateTo, m_ReportSession);
		ServerReport.SetShowEditDetailType( m_Task.GetBatchTAEdit());
		ServerReport.SetTAReportType( nInternalReportType);
		ServerReport.SetOpenShiftFlag(m_Task.GetBatchTAOpenShiftFlag());
		ServerReport.SetClosingTimeFlag( m_Task.GetBatchTAClosingTimeFlag());
		ServerReport.SetEventsBySecondFlag( m_Task.GetBatchTATotalHours() == 1 );

		for (int n = 0; n < FileList.GetSize(); n++)
		{
			CSSFile fileServer;
			if (fileServer.Open(FileList.GetAt(n), "rb") == FALSE)
			{
				continue;
			}

			int nDbNo = 0;
			int nDbIdx = DbIdxList.GetAt(n) - 1;
			if (nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize())
			{
				nDbNo = dbDatabase.GetDbNo(nDbIdx);
			}

			int nLocNo = 0;
			int nNwkNo = 0;
			int nLocIdx = LocIdxList.GetAt(n) - 1;
			if (nLocIdx >= 0 && nLocIdx < dbLocation.GetSize())
			{
				nLocNo = dbLocation.GetLocNo(nLocIdx);
				nNwkNo = dbLocation.GetNetworkNo(nLocIdx);
			}

			CString strBuffer;
			while (fileServer.ReadString(strBuffer))
			{
				int nSelArrayIdx = SelIdxList.GetAt(n);
				CCSVEposTimeAttendData csv(strBuffer);
				ServerReport.ConsolidateServerData(csv, nNwkNo, nDbNo, nLocNo, m_SelectArray, nSelArrayIdx);
			}
		}

		ServerReport.SortByEventDateAndTime();
		ServerReport.CreateReport();

		//SHOWS ECRS AND THE DATES FOR WHICH WE HAVE DATA
		CSSFile fileReport;
		if (fileReport.Open(Super.ReportFilename(), "ab") == TRUE)
		{
			for (int n = 0; n < m_SelectArray.GetListSize(); n++)
			{
				CEposSelectArrayListItem ListItem;
				m_SelectArray.GetListItem(n, ListItem);

				{
					int nSPOSVersion;
					int nConnectionType;
					dbLocation.GetLocSPOSVersion(ListItem.m_nLocIdx, nSPOSVersion, nConnectionType);
					if (CONNECTION_TYPE_STANDARD_NONE != nConnectionType)
						continue;
				}

				CString strName;
				bool bHideNoData = FALSE;
				strName.Format("<..>Time & Attendance for %s %s",
					dbLocation.GetName(ListItem.m_nLocIdx),
					m_SelectArray.GetDateInfo(ListItem, bHideNoData));

				if (FALSE == bHideNoData)
					fileReport.WriteLine(strName);
			}
			fileReport.Close();
		}

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strKey = "";
		strKey.Format("%3.3d", nReportType + 1);

		CString strTAExportFilename = "";

		{
			int nPos = 0;
			if (DataManagerNonDb.TimeAttendReportNames.FindReportByNumber(nReportType + 1, nPos) == TRUE)
			{
				CTimeAttendReportNameCSVRecord record;
				DataManagerNonDb.TimeAttendReportNames.GetAt(nPos, record);
				strTAExportFilename = record.GetReportFilename();
				::TrimSpaces(strTAExportFilename,FALSE);
			}
		}

		if (strTAExportFilename == "")
		{
			strTAExportFilename.Format("TA%3.3d", nReportType + 1);
		}

		{
			CReportFileDoc ReportFileDoc ( Super.ReportFilename() );

			int nEmailFormat = m_Task.GetTaskFormatEmail();
			if ( ( 0 == nEmailFormat ) || ( 1 == nEmailFormat ) )
			{
				int nEmailIdx = 1;
				{
					int nPos = 0;
					CSortedIntByString item;
					item.m_strItem.Format("%d", nReportType);
					if (arrayEmailByReport.Find(item, nPos) == TRUE)
					{
						arrayEmailByReport.GetAt(nPos, item);
						nEmailIdx = item.m_nItem;
					}
				}

				if ((nEmailIdx >= 1) && (nEmailIdx <= 5))
				{
					CString strEmailFilename;
					strEmailFilename.Format("\\TA%3.3d_%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.%s",
						nReportType + 1,
						(const char*)m_strEmailDatePrefix,
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
						CFilenameUpdater FnUpParams(SysFiles::TimeAttendPrm);
						CString strParamsFilename = FnUpParams.GetFilenameToUse();
						ReportHelpers.CreateTabRuler(ReportFileDoc, strParamsFilename, strKey, TabRuler);
						ReportFileDoc.SaveAsHTML(m_strEmailFolder + strEmailFilename, TabRuler);
					}

					CSSFile fileNKN;
					if (fileNKN.Open(m_strEmailFolder + strNKNFilename, "wb") == TRUE)
					{
						fileNKN.WriteLine(m_Task.GetTaskEmailNickname(nEmailIdx - 1));
						fileNKN.WriteLine(ServerReport.GetTAReportTitleFull());
						fileNKN.Close();
					}
				}
			}

			int nExportFormat = m_Task.GetTaskFormatExport();
			if ( ( 0 == nExportFormat ) || ( 1 == nExportFormat ) )
			{
				CPosTrayTaskConsolInfo infoQuick{};
				infoQuick.m_nQuickType = 0;

				CString strExportFilenameTarget = "";
				strExportFilenameTarget += m_Task.GetTaskExportFilename();
				strExportFilenameTarget += strTAExportFilename;
				strExportFilenameTarget += m_Task.GetExportFilenameSuffix( m_dateTask, infoQuick );

				CString strSlash = "\\";
				CString strExportFilenamePending = "";
				strExportFilenamePending += strSlash; 
				strExportFilenamePending += CSSMD5::CalcMD5( m_strExportFolderTarget + strSlash + strExportFilenameTarget );
				strExportFilenamePending += ( 1 == nExportFormat ) ? ".htm" : ".csv"; 
				
				CString str001Filename;
				str001Filename = strExportFilenamePending.Left( strExportFilenamePending.GetLength() - 3 );
				str001Filename += "001";
	
				CSSFile file001;
				if ( file001.Open( m_strExportFolderPending + str001Filename, "wb" ) == TRUE )
				{
					file001.WriteLine( strExportFilenameTarget + strExportFilenamePending.Right(4) );
					file001.WriteLine( m_Task.GetFilename( FALSE ) );
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
					CFilenameUpdater FnUpParams( SysFiles::TimeAttendPrm );
					CString strParamsFilename = FnUpParams.GetFilenameToUse();
					ReportHelpers.CreateTabRuler( ReportFileDoc, strParamsFilename, strKey, TabRuler );
					ReportFileDoc.SaveAsHTML( m_strExportFolderPending + strExportFilenamePending, TabRuler );
				}
			}

			if ( TRUE == m_Task.GetTaskAutoPrintFlag() )
			{
				CString strPrintFilename = "";
				strPrintFilename.Format( "%s\\%s%6.6d",
					(const char*) m_strPrintFolder,
					(const char*) m_strPrintFilenamePrefix,
					m_nNextPrintFileNumber++ );

				{
					CSSFile fileHelper;
					if ( fileHelper.Open( strPrintFilename + ".dat", "wb" ) == TRUE )
					{
						CFilenameUpdater FnUp( SysFiles::TimeAttendPrm );
						fileHelper.WriteLine( FnUp.GetFilenameToUse() );
						fileHelper.WriteLine( strKey );
					}
				}

				::CopyFile( Super.ReportFilename(), strPrintFilename + ".rep", FALSE );
			}

			if (x < arrayReportTypes.GetSize() - 1)
			{
				m_SelectArray.ResetDateInfo();
			}
		}
	}
	
	return TRUE;
}

/**********************************************************************/

