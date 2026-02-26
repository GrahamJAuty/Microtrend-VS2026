/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "JobNumbers.h"
#include "KeyboardJobInfo.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "JobListCSVArray.h"
/**********************************************************************/

CJobListCSVRecord::CJobListCSVRecord()
{
	m_nLineType = JOBLIST_LINETYPE_BLANK;
	m_nNetworkIdx = 0;
	m_nLocIdx = 0;
	m_nTerminalNo = 0;
	m_bIsImmediate = FALSE;
	m_nJobType = RX_SALES;
	m_nParam1 = 0;
	m_nStatus = JOBLIST_STATUS_NOTSTARTED;
	m_nAttempts = 0;
	m_bLocDownload = FALSE;
	m_nRejectedRecords = 0;
}

/**********************************************************************/

void CJobListCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nLineType );
	csv.Add ( m_nNetworkIdx );
	csv.Add ( m_nLocIdx );
	csv.Add ( m_nTerminalNo );
	csv.Add ( m_bIsImmediate );
	csv.Add ( m_nJobType );
	csv.Add ( m_nStatus );
	csv.Add ( m_nAttempts );
	csv.Add ( m_nParam1 );
	csv.Add ( m_bLocDownload );
	csv.Add ( m_nRejectedRecords );
}

/**********************************************************************/

void CJobListCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	SetLineType ( csv.GetInt(0) );
	SetNetworkIdx( csv.GetInt(1) );
	SetLocIdx ( csv.GetInt(2) );
	SetTerminalNo ( csv.GetInt(3) );
	SetIsImmediateFlag( csv.GetBool(4) );
	SetJobType ( csv.GetInt(5) );
	SetStatus ( csv.GetInt(6) );
	SetAttempts( csv.GetInt(7) );
	SetParam1( csv.GetInt(8) );
	SetLocDownloadFlag( csv.GetBool(9) );
	SetRejectedRecordCount( csv.GetInt(10) );
}

/**********************************************************************/

const char* CJobListCSVRecord::GetLogLine()
{
	m_strLogLine = "";
	if ( m_nLineType == JOBLIST_LINETYPE_JOB )
	{
		switch( m_nJobType )
		{
		case RX_SALES:
		case RX_REASON:
			m_strLogLine.Format ( "%s (%s)",
				GetJobName ( m_nJobType, m_nParam1, 0 ),
				dbNetwork.GetName( m_nNetworkIdx ) );
			break;

		default:
			m_strLogLine.Format ( "%s (%s,%s)",
				GetJobName ( m_nJobType, m_nParam1, 0 ),
				dbNetwork.GetName( m_nNetworkIdx ),
				dbLocation.GetName ( m_nLocIdx ) );
			break;
		}
	}
	return m_strLogLine;
}

/**********************************************************************/

const char* CJobListCSVRecord::GetDownloadExclusionLogLine()
{
	m_strLogLine = "";
	if ( m_nLineType == JOBLIST_LINETYPE_JOB )
	{
		CString strSPOSVersion;
		CNetworkCSVRecord::GetTreeNameExternal( CONNECTION_TYPE_STANDARD_NONE, dbNetwork.GetSPOSVersion( m_nNetworkIdx ), strSPOSVersion, TRUE );

		switch( m_nJobType )
		{
		case RX_SALES:
		case RX_REASON:
			break;

		default:
			m_strLogLine.Format ( "%s %s",	GetJobName ( m_nJobType, m_nParam1, 0 ), (const char*) strSPOSVersion );
			break;
		}
	}
	return m_strLogLine;
}

/**********************************************************************/

const char* CJobListCSVRecord::GetJobName( int nJobType, int nParam1, int nExtraJobParam1 )
{
	CKeyboardJobInfo infoKb;
	int nKbNo = infoKb.GetKbNoFromJobNo( nJobType );

	if (nKbNo != 0)
	{
		m_strJobName.Format("Download Keyboard %2.2d", nKbNo);

		if (1 == nParam1)
		{
			m_strJobName += " (Reset Macros)";
		}
		else
		{
			m_strJobName += " (Merge Macros)";
		}
	}
	else
	{
		switch ( nJobType )
		{
		//RX JOBS
		case RX_SALES:			
			m_strJobName = "Get Sales Data";
			break;
			
		case RX_REASON:
			m_strJobName = "Import EOD Reason Files";
			switch( nParam1 )
			{
			case 0:				
				m_strJobName += " (today)";	
				break;

			case 1:			
				m_strJobName += " (yesterday)";	
				break;

			case 2:		
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				{
					CString strExtra = "";
					strExtra.Format( " (%d days ago)", nParam1 );
					m_strJobName += strExtra;
				}
				break;

			case 8:				
				m_strJobName += " (all dates)";	
				break;
			}
			break;

		case RX_EODPLU:
			m_strJobName = "Import Plu Changes";
			break;

		case RX_EODCUSTOMER:	
			m_strJobName = "Import Customer Changes";	
			break;

		//DN JOBS
		case DN_PLU:			
			m_strJobName = "Download Plu";
			break;

		case DN_PLU_KBD:		
			m_strJobName = "Download Plu & Keyboard";
			break;

		case DN_PLU_SET1:	
			m_strJobName = "Download Price Set 1";	
			break;

		case DN_PLU_SET1_KBD:	
			m_strJobName = "Download Price Set 1 & Keyboard";
			break;

		case DN_PLU_SET2:	
			m_strJobName = "Download Price Set 2";
			break;

		case DN_PLU_SET2_KBD:
			m_strJobName = "Download Price Set 2 & Keyboard";
			break;

		case DN_PLU_SET3:	
			m_strJobName = "Download Price Set 3";	
			break;

		case DN_PLU_SET3_KBD:	
			m_strJobName = "Download Price Set 3 & Keyboard";
			break;

		case DN_PLU_INFO:	
			m_strJobName = "Download Plu Info Files";
			break;

		case DN_BAR:	
			m_strJobName = "Download Barcodes";
			break;

		case DN_MIX:		
			m_strJobName = "Download Mix Match";
			break;

		case DN_PROMO:		
			m_strJobName = "Download Promotions";
			break;
			
		case DN_WALLETSTAMPRULE:	
			if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
			{
				m_strJobName = "Download Purchase Controls";
			}
			else
			{
				m_strJobName = "Download Stamp Offers";
			}	
			break;
			
		case DN_ALLOFFER:	
			m_strJobName = "Download All Offers";
			break;

		case DN_TPLAN:	
			m_strJobName = "Download Time Plan";
			break;

		case DN_TAX:	
			m_strJobName = EcrmanOptions.GetTaxString( "Download %T Rates" );
			break;

		case DN_DEPT:	
			m_strJobName = "Download Departments";	
			break;

		case DN_GROUP:		
			m_strJobName = "Download Dept Groups";	
			break;

		case DN_ALLERGEN:	
			m_strJobName = "Download Allergens";	
			break;

		case DN_LOYALTY_SCHEME:	
			m_strJobName = "Download Loyalty Schemes";
			break;

		case DN_TABLE_AREA:	
			m_strJobName = "Download Table Areas";
			break;

		case DN_TABLE_NAME:	
			m_strJobName = "Download Table Names";
			break;

		case DN_MOD:	
			m_strJobName = "Download Modifiers";
			break;

		case DN_PAY:	
			m_strJobName = "Download Payment Methods";		
			break;

		case DN_SERVER_NAME:
			m_strJobName = "Download Server Names";	
			break;

		case DN_ACAT:	
			m_strJobName = "Download Analysis Categories";
			break;

		case DN_RECEIPT:
			m_strJobName = "Download Receipt and Bill Texts";
			break;

		case DN_SERVER_PRESET:
			m_strJobName = "Download Server Presets";	
			break;

		case DN_FUNCTION:
			m_strJobName = "Download Function Settings";
			break;

		case DN_PAIDIO:	
			m_strJobName = "Download Paid In / Out Texts";
			break;

		case DN_IMAGES:		
			m_strJobName = "Download Button Images";		
			break;

		case DN_MACRO:	
			m_strJobName = "Download Macros";	
			break;

		case DN_REASON:		
			m_strJobName = "Download Reasons";	
			break;
			
		//OTHER
		default:	
			m_strJobName = "Unknown Job";	
			break;
		}
	}

	switch( nJobType )
	{
	case DN_PLU:
	case DN_PLU_SET1:		
	case DN_PLU_SET2:		
	case DN_PLU_SET3:		
	case DN_PLU_INFO:
		switch( nParam1 )
		{
		case 1:
			m_strJobName += " (All)";
			break;

		case 2:
			m_strJobName += " (Changes)";
			break;

		case 0:
		default:
			m_strJobName += " (Clear)";		
			break;
		}
		break;
	
	case DN_PLU_KBD:		
	case DN_PLU_SET1_KBD:	
	case DN_PLU_SET2_KBD:	
	case DN_PLU_SET3_KBD:
		{
			CCSV csvExtra;

			switch( nParam1 )
			{
			case 1:	
				csvExtra.Add( "Plu All" );
				break;

			case 2:
				csvExtra.Add( "Plu Changes" );
				break;

			case 0:
			default:
				csvExtra.Add( "Plu Clear" );
				break;
			}

			if ( nExtraJobParam1 == 1 )
			{
				csvExtra.Add( "Reset Macros" );
			}
			else
			{
				csvExtra.Add( "Merge Macros" );
			}

			m_strJobName += " (";
			m_strJobName += csvExtra.GetLine();
			m_strJobName += ")";
		}
		break;
	}
			
	return m_strJobName;
}

/**********************************************************************/

int CJobListCSVRecord::GetPriceSet()
{
	switch ( m_nJobType )
	{
	case DN_PLU_SET1:
		return 1;

	case DN_PLU_SET2:	
		return 2;

	case DN_PLU_SET3:	
		return 3;
	}
	return 0;
}

/**********************************************************************/

CJobListCSVArray::CJobListCSVArray() : CCSVArray()
{
	m_strDate = "";
}

/**********************************************************************/

bool CJobListCSVArray::JobNeedsDbLink( int nJobType )
{
	switch( nJobType )
	{
	case DN_BAR:
	case DN_IMAGES:
	case DN_ACAT:
	case DN_RECEIPT:
	case DN_SERVER_NAME:
	case DN_SERVER_PRESET:
	case DN_FUNCTION:
	case DN_MACRO:
	case DN_REASON:
	case DN_PAIDIO:
	case DN_DEPT:
	case DN_GROUP:
	case DN_ALLERGEN:
	case DN_LOYALTY_SCHEME:
	case DN_MIX:
	case DN_PROMO:
	case DN_WALLETSTAMPRULE:
	case DN_ALLOFFER:
	case DN_MOD:
	case DN_PAY:
	case DN_PLU:
	case DN_PLU_SET1:
	case DN_PLU_SET2:
	case DN_PLU_SET3:
	case DN_PLU_INFO:
	case DN_TAX:
	case DN_TPLAN:
	case DN_KBD1:				
		return TRUE;	

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CJobListCSVArray::IsLocationJob( int nJobType )
{
	switch( nJobType )
	{
	case DN_PLU:
	case DN_PLU_SET1:
	case DN_PLU_SET2:
	case DN_PLU_SET3:
	case DN_PLU_INFO:
	case DN_ACAT:
	case DN_RECEIPT:
	case DN_SERVER_NAME:
	case DN_SERVER_PRESET:
	case DN_FUNCTION:
	case DN_MACRO:
	case DN_REASON:
	case DN_PAIDIO:
	case DN_IMAGES:
	case DN_BAR:
	case DN_MOD:
	case DN_MIX:
	case DN_PROMO:
	case DN_WALLETSTAMPRULE:
	case DN_ALLOFFER:
	case DN_TPLAN:
	case DN_TAX:
	case DN_DEPT:
	case DN_GROUP:
	case DN_ALLERGEN:
	case DN_LOYALTY_SCHEME:
	case DN_TABLE_AREA:
	case DN_TABLE_NAME:
	case DN_PAY:
	case DN_KBD1:
	case RX_EODPLU:
	case RX_EODCUSTOMER:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CJobListCSVArray::AddBlankLine()
{
	CJobListCSVRecord JobInfo;
	JobInfo.SetLineType ( JOBLIST_LINETYPE_BLANK );
	Add ( JobInfo );
}

/**********************************************************************/

void CJobListCSVArray::AddNetworkLine( int nNetworkIdx)
{
	CJobListCSVRecord JobInfo;
	JobInfo.SetLineType ( JOBLIST_LINETYPE_NETWORK );
	JobInfo.SetNetworkIdx ( nNetworkIdx );
	Add ( JobInfo );
}

/**********************************************************************/

void CJobListCSVArray::AddLocationLine( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bImmediate )
{
	CJobListCSVRecord JobInfo;
	JobInfo.SetLineType ( JOBLIST_LINETYPE_LOCATION );
	JobInfo.SetNetworkIdx ( nNetworkIdx );
	JobInfo.SetLocIdx ( nLocIdx );
	JobInfo.SetTerminalNo ( nTerminalNo );
	JobInfo.SetIsImmediateFlag( bImmediate );
	Add ( JobInfo );
}

/**********************************************************************/

void CJobListCSVArray::SetLocDownloadFlag( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
	{
		CJobListCSVRecord JobInfo;
		GetAt( nIdx, JobInfo );

		if ( JobInfo.GetLineType() == JOBLIST_LINETYPE_LOCATION )
		{
			JobInfo.SetLocDownloadFlag( TRUE );
			SetAt( nIdx, JobInfo );
		}
	}
}

/**********************************************************************/

void CJobListCSVArray::AddJobLine( int nNetworkIdx, int nLocIdx, int nTerminalNo, int nJobType, int nParam1, bool bIsImmediate )
{
	CJobListCSVRecord JobInfo;
	JobInfo.SetLineType ( JOBLIST_LINETYPE_JOB );
	JobInfo.SetNetworkIdx ( nNetworkIdx );
	JobInfo.SetLocIdx ( nLocIdx );
	JobInfo.SetTerminalNo ( nTerminalNo );
	JobInfo.SetJobType ( nJobType );
	JobInfo.SetParam1( nParam1 );
	JobInfo.SetIsImmediateFlag( bIsImmediate );
	Add ( JobInfo );
}

/**********************************************************************/

const char* CJobListCSVArray::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if (nIndex < 0 || nIndex >= GetSize())
	{
		return m_strDisplayLine;
	}

	CJobListCSVRecord JobInfo;
	GetAt( nIndex, JobInfo );

	bool bImmediateJob = JobInfo.GetIsImmediateFlag();
	
	int nNetworkIdx = JobInfo.GetNetworkIdx();
	
	switch ( JobInfo.GetLineType() )
	{
	case JOBLIST_LINETYPE_NETWORK:
		m_strDisplayLine = dbNetwork.GetName ( nNetworkIdx );
		break;

	case JOBLIST_LINETYPE_LOCATION:
		{
			CCSV csvLine ( '\t' );
			csvLine.Add ( "" );
			
			CString strName = "";
			
			if (JobInfo.GetTerminalNo() == 0)
			{
				strName = dbLocation.GetName(JobInfo.GetLocIdx());
			}
			else
			{
				strName = LocationMasterMap.GetMasterName(JobInfo.GetLocIdx(), JobInfo.GetTerminalNo());
			}

			if ( TRUE == JobInfo.GetLocDownloadFlag() )
			{
				if (bImmediateJob == TRUE)
				{
					strName += " (Epos Import Now)";
				}
				else
				{
					CString strDate = m_strDate;
					if ( strDate.GetLength() != 8 ) strDate = "00000000";

					CString strDisplayDate;
					strDisplayDate.Format ( " (Epos Import %s/%s/%s)",
						(const char*) strDate.Right(2),
						(const char*) strDate.Mid(4,2),
						(const char*) strDate.Left(4) );

					strName += strDisplayDate;
				}
			}

			csvLine.Add ( strName );
			m_strDisplayLine = csvLine.GetLine();
		}
		break;

	case JOBLIST_LINETYPE_JOB:
		{
			CCSV csvLine ( '\t' );
			csvLine.Add ( "" );
			csvLine.Add ( "" );

			CString strJobName = JobInfo.GetJobName( JobInfo.GetJobType(), JobInfo.GetParam1(), 0 );

			csvLine.Add( strJobName );

			switch ( JobInfo.GetStatus() )
			{
			case JOBLIST_STATUS_NOTSTARTED:	
				csvLine.Add ( "Not Started" );
				break;

			case JOBLIST_STATUS_NOTFINISHED:
				csvLine.Add ( "Not Finished" );
				break;

			case JOBLIST_STATUS_FINISHED:
				csvLine.Add ( "Finished" );	
				break;

			case JOBLIST_STATUS_NODATA:	
				csvLine.Add ( "No Data" );	
				break;

			case JOBLIST_STATUS_FAILED:			
				{
					CString strFailed;
					strFailed.Format ( "Failed(%d)", JobInfo.GetAttempts() );
					csvLine.Add ( strFailed );
				}
				break;

			default:							
				csvLine.Add ( "Unknown" );		
				break;
			}

			switch ( JobInfo.GetStatus() )
			{
			case JOBLIST_STATUS_FINISHED:		
				{
					CString strExtra = "";
					int nRejected = JobInfo.GetRejectedRecordCount();
					
					if (1 == nRejected)
					{
						strExtra = "(Rejected 1 record)";
					}
					else if (nRejected != 0)
					{
						strExtra.Format("(Rejected %d records)", nRejected);
					}

					csvLine.Add( strExtra );
				}
				break;
			}
			
			m_strDisplayLine = csvLine.GetLine();
		}
		break;

	case JOBLIST_LINETYPE_BLANK:
	default:
		m_strDisplayLine = "";
		break;
	}
	return m_strDisplayLine;
}
			
/**********************************************************************/

int CJobListCSVArray::BuildFromTaskRecord(CEposTaskDatabase& TaskDatabase, CString& strDate, CArray<CJobSetupInfo, CJobSetupInfo>& JobSetupInfo, int nDbIdx, CArray<CSelectMultipleEntityInfo, CSelectMultipleEntityInfo>* pDownloadMasters)
{
	while (GetSize() > 0)
	{
		RemoveAt(GetSize() - 1);
	}

	JobSetupInfo.RemoveAll();

	int nJobCount = 0;

	bool bCanDoDownloads, bIsImmediateTask;
	dbNetwork.GetDownloadPermissions(bCanDoDownloads);

	COleDateTime date = COleDateTime::GetCurrentTime();
	int nHourNow = date.GetHour();
	int nMinuteNow = date.GetMinute();

	int nHourSOD = EcrmanOptions.GetReportsDayStartHour();
	int nMinuteSOD = EcrmanOptions.GetReportsDayStartMinute();

	if (nHourNow > nHourSOD)
	{
		date += COleDateTimeSpan(1, 0, 0, 0);
	}
	else if ((nHourNow == nHourSOD) && (nMinuteNow > nMinuteSOD))
	{
		date += COleDateTimeSpan(1, 0, 0, 0);
	}

	CString strStartOfDay = date.Format("%Y%m%d");

	CEposTaskHeader TaskHeader;
	TaskDatabase.GetEditableTaskHeader(TaskHeader);

	switch (TaskHeader.GetImportType())
	{
	case IMPORT_STARTOFDAY:
		bIsImmediateTask = FALSE;
		SetDate(strStartOfDay);
		break;

	case IMPORT_SPECIFIED:
		bIsImmediateTask = FALSE;
		SetDate(strDate);
		break;

	case IMPORT_IMMEDIATE:
	default:
		bIsImmediateTask = TRUE;
		SetDate(strStartOfDay);
		break;
	}

	CWordArray arrayNetworkIdx;

	if (EcrmanOptions.GetReportsLocationSortType() != 0)
	{
		dbNetwork.SortByName(arrayNetworkIdx);
	}
	else
	{
		for (int n = 0; n < dbNetwork.GetSize(); n++)
		{
			arrayNetworkIdx.Add(n);
		}
	}

	//LOCATE NETWORK JOBS
	int nJobIdxSales = -1;
	int nJobIdxReasons = -1;

	for (int nJobIdx = 0; nJobIdx < TaskDatabase.GetEditableTaskJobCount(); nJobIdx++)
	{
		CEposTaskJob TaskJob;
		TaskDatabase.GetEditableTaskJob(nJobIdx, TaskJob);

		switch (TaskJob.GetJobNo())
		{
		case RX_SALES:
			nJobIdxSales = nJobIdx;
			break;

		case RX_REASON:
			nJobIdxReasons = nJobIdx;
			break;
		}
	}

	if ((nJobIdxSales != -1) || (nJobIdxReasons != -1))
	{
		for (int n = 0; n < arrayNetworkIdx.GetSize(); n++)
		{
			int nNetworkIdx = arrayNetworkIdx.GetAt(n);

			bool bCanGetSales = TRUE;

			if (PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM)
			{
				int nStartIdx, nEndIdx;
				dbLocation.GetNetworkLocIdxRange(nNetworkIdx, nStartIdx, nEndIdx);

				for (int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++)
				{
					if (PasswordArray.CheckLocationAccessLocIdx(nLocIdx) == FALSE)
					{
						bCanGetSales = FALSE;
						break;
					}
				}
			}

			if (FALSE == bCanGetSales)
				continue;

			bool bGotSales = FALSE;
			if (nJobIdxSales != -1)
			{
				CEposTaskJob TaskJob;
				TaskDatabase.GetEditableTaskJob(nJobIdxSales, TaskJob);
				bGotSales = TaskJob.IsRunAtNetwork(nNetworkIdx);
			}

			bool bGotReasons = FALSE;
			if (nJobIdxReasons != -1)
			{
				CEposTaskJob TaskJob;
				TaskDatabase.GetEditableTaskJob(nJobIdxReasons, TaskJob);
				bGotReasons = TaskJob.IsRunAtNetwork(nNetworkIdx);
			}

			bool bGotNetworkHeader = FALSE;

			if (TRUE == bGotSales)
			{
				CJobSetupInfo info;
				info.m_nJobType = RX_SALES;
				info.m_nNwkIdx = nNetworkIdx;
				info.m_nLocIdx = -1;

				bool bFail = FALSE;
				switch (dbNetwork.GetConnectionType(nNetworkIdx))
				{
				case CONNECTION_TYPE_SPTBOOK_WEB:
				{
					int nDbIdx;
					if (dbNetwork.CanDoWebAPIComms(nNetworkIdx, nDbIdx) == FALSE)
					{
						info.m_nStatus = JOBLIST_SETUP_NEED_SPTBOOK;
						JobSetupInfo.Add(info);
						bFail = TRUE;
					}
				}
				break;

				case CONNECTION_TYPE_SMARTENT_WEB:
				{
					int nDbIdx;
					if (dbNetwork.CanDoWebAPIComms(nNetworkIdx, nDbIdx) == FALSE)
					{
						info.m_nStatus = JOBLIST_SETUP_NEED_SMARTENT;
						JobSetupInfo.Add(info);
						bFail = TRUE;
					}
				}
				break;

				case CONNECTION_TYPE_STANDARD_REALTIME:
					info.m_nStatus = JOBLIST_SETUP_NEED_BATCH;
					JobSetupInfo.Add(info);
					bFail = TRUE;
					break;

				case CONNECTION_TYPE_STANDARD_BATCH:
					bFail = FALSE;
					break;

				case CONNECTION_TYPE_STANDARD_NONE:
				case CONNECTION_TYPE_SPTBOOK_NONE:
				case CONNECTION_TYPE_SMARTENT_NONE:
				default:
					info.m_nStatus = JOBLIST_SETUP_NEED_COMMS;
					JobSetupInfo.Add(info);
					bFail = TRUE;
					break;
				}

				if (FALSE == bFail)
				{
					if (GetSize() != 0)
					{
						AddBlankLine();
					}

					AddNetworkLine(nNetworkIdx);
					AddJobLine(nNetworkIdx, 0, 0, RX_SALES, 0, FALSE);

					info.m_nStatus = JOBLIST_SETUP_ACCEPTED;
					JobSetupInfo.Add(info);
					nJobCount++;
					bGotNetworkHeader = TRUE;
				}
			}

			if (TRUE == bGotReasons)
			{
				CEposTaskJob TaskJob;
				TaskDatabase.GetEditableTaskJob(nJobIdxReasons, TaskJob);
				int nParam1 = TaskJob.GetParam1BaseJob();

				CJobSetupInfo info;
				info.m_nJobType = RX_REASON;
				info.m_nParam1 = nParam1;
				info.m_nNwkIdx = nNetworkIdx;
				info.m_nLocIdx = -1;

				switch (dbNetwork.GetConnectionType(nNetworkIdx))
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
				case CONNECTION_TYPE_SPTBOOK_WEB:
					info.m_nStatus = JOBLIST_SETUP_REJECT_SPTBOOK;
					JobSetupInfo.Add(info);
					break;

				case CONNECTION_TYPE_SMARTENT_NONE:
				case CONNECTION_TYPE_SMARTENT_WEB:
					info.m_nStatus = JOBLIST_SETUP_REJECT_SMARTENT;
					JobSetupInfo.Add(info);
					break;

				case CONNECTION_TYPE_STANDARD_BATCH:
				case CONNECTION_TYPE_STANDARD_REALTIME:

					if (FALSE == bGotNetworkHeader)
					{
						if (GetSize() != 0)
						{
							AddBlankLine();
						}

						AddNetworkLine(nNetworkIdx);
					}

					AddJobLine(nNetworkIdx, 0, 0, RX_REASON, nParam1, FALSE);

					info.m_nStatus = JOBLIST_SETUP_ACCEPTED;
					JobSetupInfo.Add(info);
					nJobCount++;
					bGotNetworkHeader = TRUE;
					break;

				default:
					info.m_nStatus = JOBLIST_SETUP_NEED_COMMS;
					JobSetupInfo.Add(info);
					break;
				}
			}
		}
	}

	for (int n = 0; n < arrayNetworkIdx.GetSize(); n++)
	{
		int nNetworkIdx = arrayNetworkIdx.GetAt(n);

		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange(nNetworkIdx, nStartIdx, nEndIdx);
		if (nEndIdx < nStartIdx)
		{
			continue;
		}

		CArray<int, int> arrayLocIdx;
		if (EcrmanOptions.GetReportsLocationSortType() != 0)
		{
			dbLocation.SortByName(arrayLocIdx, -1, nNetworkIdx);
		}
		else
		{
			for (int x = nStartIdx; x <= nEndIdx; x++)
			{
				arrayLocIdx.Add(x);
			}
		}

		if (nDbIdx != -1)
		{
			for (int n = arrayLocIdx.GetSize() - 1; n >= 0; n--)
			{
				int nLocIdx = arrayLocIdx.GetAt(n);
				int nLocDbNo = dbLocation.GetDbNo(nLocIdx);

				int nLocDbIdx = 0;
				if (dbDatabase.FindDatabaseByNumber(nLocDbNo, nLocDbIdx) == FALSE)
				{
					arrayLocIdx.RemoveAt(n);
				}
				else if (nLocDbIdx != nDbIdx)
				{
					arrayLocIdx.RemoveAt(n);
				}
			}
		}

		for (int n = arrayLocIdx.GetSize() - 1; n >= 0; n--)
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			if (PasswordArray.CheckLocationAccessLocIdx(nLocIdx) == FALSE)
			{
				arrayLocIdx.RemoveAt(n);
			}
		}

		int nConnectionType = dbNetwork.GetConnectionType(nNetworkIdx);

		int nRollbackNetwork = GetSize();
		bool bGotNetworkJob = FALSE;

		if (GetSize() != 0)
		{
			AddBlankLine();
		}

		AddNetworkLine(nNetworkIdx);

		for (int x = 0; x < arrayLocIdx.GetSize(); x++)
		{
			int nLocIdx = arrayLocIdx.GetAt(x);

			CArray<CLocationMasterInfo, CLocationMasterInfo> arrayMasters;
			LocationMasterMap.GetLocationMasterList(nLocIdx, arrayMasters, TRUE, FALSE);

			int nEndIdx = arrayMasters.GetCount() - 1;

			if (nEndIdx > 0)
			{
				switch (dbNetwork.GetConnectionType(nNetworkIdx))
				{
				case CONNECTION_TYPE_STANDARD_BATCH:
				case CONNECTION_TYPE_STANDARD_REALTIME:
					break;

				default:
					nEndIdx = 0;
					break;
				}
			}

			for (int nIdx = 0; nIdx <= nEndIdx; nIdx++)
			{
				CLocationMasterInfo infoMaster = arrayMasters.GetAt(nIdx);

				if (pDownloadMasters != NULL)
				{
					bool bRunHere = FALSE;

					for (int n = 0; n < pDownloadMasters->GetSize(); n++)
					{
						CSelectMultipleEntityInfo infoEntity = pDownloadMasters->GetAt(n);

						if (infoMaster.GetNwkNo() == dbNetwork.GetNetworkNo(infoEntity.m_nNwkIdx))
						{
							if (infoMaster.GetLocNo() == dbLocation.GetLocNo(infoEntity.m_nLocIdx))
							{
								if (infoMaster.GetTerminalNo() == infoEntity.m_nTNo)
								{
									bRunHere = TRUE;
									break;
								}
							}
						}
					}

					if (FALSE == bRunHere)
					{
						continue;
					}
				}

				int nRollbackLocation = GetSize();
				bool bGotLocationJob = FALSE;
				bool bGotDownloadJob = FALSE;

				int nDbIdx = 0;
				int nDbNo = dbLocation.GetDbNo(nLocIdx);
				bool bDbOk = dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx);

				AddLocationLine(nNetworkIdx, nLocIdx, infoMaster.GetTerminalNo(), bIsImmediateTask);

				for (int nJobIdx = 0; nJobIdx < TaskDatabase.GetEditableTaskJobCount(); nJobIdx++)
				{
					CEposTaskJob TaskJob;
					TaskDatabase.GetEditableTaskJob(nJobIdx, TaskJob);
					int nJobType = TaskJob.GetJobNo();

					//RX SALES JOB IS HANDLED IN A SEPARATE LOOP
					//RX REASON JOB IS HANDLED IN A SEPARATE LOOP
					switch (nJobType)
					{
					case RX_SALES:
					case RX_REASON:
						continue;
					}

					//NOTHING TO DO IF THIS JOB WAS NOT REQUESTED HERE
					if (TaskJob.IsRunAtLocation(nLocIdx, infoMaster.GetTerminalNo()) == FALSE)
					{
						continue;
					}

					CJobSetupInfo info;
					info.m_nJobType = nJobType;
					info.m_nExecuteJobType = nJobType;
					info.m_nValidateJobType = nJobType;
					info.m_nExtraJobType = 0;
					info.m_nExtraParam1 = 0;
					info.m_nNwkIdx = nNetworkIdx;
					info.m_nLocIdx = nLocIdx;

					//ALL JOBS EXCEPT BATCH SALES ARE REJECTED FOR SPORTS BOOKER NETWORKS
					switch (dbNetwork.GetConnectionType(info.m_nNwkIdx))
					{
					case CONNECTION_TYPE_SPTBOOK_NONE:
					case CONNECTION_TYPE_SPTBOOK_WEB:
						info.m_nStatus = JOBLIST_SETUP_REJECT_SPTBOOK;
						info.m_nJobType = info.m_nExecuteJobType;
						JobSetupInfo.Add(info);
						continue;

					case CONNECTION_TYPE_SMARTENT_NONE:
					case CONNECTION_TYPE_SMARTENT_WEB:
						info.m_nStatus = JOBLIST_SETUP_REJECT_SMARTENT;
						info.m_nJobType = info.m_nExecuteJobType;
						JobSetupInfo.Add(info);
						continue;
					}

					switch (nJobType)
					{
					case DN_PLU_KBD:
						info.m_nExecuteJobType = DN_PLU;
						info.m_nValidateJobType = DN_PLU;
						info.m_nExtraJobType = dbLocation.GetPriceSetKeyboardJob(nLocIdx, 0);
						info.m_nExtraParam1 = TaskJob.GetParam1ExtraJob();
						break;

					case DN_PLU_SET1_KBD:
						info.m_nExecuteJobType = DN_PLU_SET1;
						info.m_nValidateJobType = DN_PLU_SET1;
						info.m_nExtraJobType = dbLocation.GetPriceSetKeyboardJob(nLocIdx, 1);
						info.m_nExtraParam1 = TaskJob.GetParam1ExtraJob();
						break;

					case DN_PLU_SET2_KBD:
						info.m_nExecuteJobType = DN_PLU_SET2;
						info.m_nValidateJobType = DN_PLU_SET2;
						info.m_nExtraJobType = dbLocation.GetPriceSetKeyboardJob(nLocIdx, 2);
						info.m_nExtraParam1 = TaskJob.GetParam1ExtraJob();
						break;

					case DN_PLU_SET3_KBD:
						info.m_nExecuteJobType = DN_PLU_SET3;
						info.m_nValidateJobType = DN_PLU_SET3;
						info.m_nExtraJobType = dbLocation.GetPriceSetKeyboardJob(nLocIdx, 3);
						info.m_nExtraParam1 = TaskJob.GetParam1ExtraJob();
						break;
					}

					CKeyboardJobInfo infoKb;
					if (infoKb.GetKbNoFromJobNo(info.m_nJobType) != 0)
						info.m_nValidateJobType = DN_KBD1;

					//REJECT JOB IF NO CONNECTION AT NETWORK
					switch (nConnectionType)
					{
					case CONNECTION_TYPE_STANDARD_BATCH:
					case CONNECTION_TYPE_STANDARD_REALTIME:
						break;

					default:
						info.m_nStatus = JOBLIST_SETUP_NEED_COMMS;
						JobSetupInfo.Add(info);
						continue;
					}

					//REJECT JOB IF DB NEEDED BUT NOT LINKED
					if (JobNeedsDbLink(info.m_nValidateJobType))
					{
						if (FALSE == bDbOk)
						{
							info.m_nStatus = JOBLIST_SETUP_NEED_DBLINK;
							info.m_nJobType = info.m_nExecuteJobType;
							JobSetupInfo.Add(info);
							continue;
						}
					}

					//REJECT JOB IF THE JOB TYPE IS UNKNOWN
					if (CEposTaskDatabase::GetJobSortOrder(info.m_nValidateJobType) == 0)
					{
						info.m_nStatus = JOBLIST_SETUP_UNKNOWNJOB;
						info.m_nJobType = info.m_nExecuteJobType;
						JobSetupInfo.Add(info);
						continue;
					}

					//REJECT MODIFIER DOWNLOAD FOR A NO MODIFIER SYSTEM
					if (SysInfo.GetMaxBasePluLen() == 0)
					{
						if (DN_MOD == info.m_nValidateJobType)
						{
							info.m_nStatus = JOBLIST_SETUP_UNKNOWNJOB;
							info.m_nJobType = info.m_nExecuteJobType;
							JobSetupInfo.Add(info);
							continue;
						}
					}

					if (RX_EODPLU == info.m_nValidateJobType)
					{
						//REJECT EOD PLU FOR REAL TIME NETWORKS
						if (EcrmanOptions.GetPluProdCSVImportType() == 2)
						{
							if (CONNECTION_TYPE_STANDARD_REALTIME == nConnectionType)
							{
								info.m_nStatus = JOBLIST_SETUP_NEED_BATCH;
								info.m_nJobType = info.m_nExecuteJobType;
								JobSetupInfo.Add(info);
								continue;
							}
						}

						int nType = dbLocation.GetProdCSVImportFolderType(nLocIdx);
						if ((EODIMPORT_FOLDERTYPE_ROOT != nType) && (EODIMPORT_FOLDERTYPE_INTRADING != nType))
						{
							info.m_nStatus = JOBLIST_SETUP_LOC_DISABLE;
							info.m_nJobType = info.m_nExecuteJobType;
							JobSetupInfo.Add(info);
							continue;
						}
					}

					//REJECT EOD CUSTOMER FOR REAL TIME NETWORKS
					if (RX_EODCUSTOMER == info.m_nValidateJobType)
					{
						if (EcrmanOptions.GetFeaturesCustomerNameImportType() == 2)
						{
							if (CONNECTION_TYPE_STANDARD_REALTIME == nConnectionType)
							{
								info.m_nStatus = JOBLIST_SETUP_NEED_BATCH;
								info.m_nJobType = info.m_nExecuteJobType;
								JobSetupInfo.Add(info);
								continue;
							}
						}
					}

					info.m_nParam1 = 0;
					switch (info.m_nValidateJobType)
					{
					case DN_KBD1:
					case DN_REASON:
						info.m_nParam1 = TaskJob.GetParam1BaseJob();
						break;

					case DN_PLU:
					case DN_PLU_SET1:
					case DN_PLU_SET2:
					case DN_PLU_SET3:
						info.m_nParam1 = TaskJob.GetParam1BaseJob();

						if (2 == info.m_nParam1)
						{
							//REJECT PLU CHANGES ONLY IF NOT IMMEDIATE TASK
							if (FALSE == bIsImmediateTask)
							{
								info.m_nStatus = JOBLIST_SETUP_NEED_IMMEDIATE;
								info.m_nJobType = info.m_nExecuteJobType;
								JobSetupInfo.Add(info);
								continue;
							}

							//REJECT PLU CHANGES ONLY FOR OLDER VERSIONS OF SPOS
							int nSPOSVersion;
							int nConnectionType;
							dbLocation.GetLocSPOSVersion(nLocIdx, nSPOSVersion, nConnectionType);
							if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_992) == FALSE)
							{
								info.m_nStatus = JOBLIST_SETUP_NEED_V4_992;
								info.m_nJobType = info.m_nExecuteJobType;
								JobSetupInfo.Add(info);
								continue;
							}
						}
						break;

					case DN_PLU_INFO:
						info.m_nParam1 = TaskJob.GetParam1BaseJob();
						if (FALSE == bIsImmediateTask)
						{
							info.m_nStatus = JOBLIST_SETUP_NEED_IMMEDIATE;
							info.m_nJobType = info.m_nExecuteJobType;
							JobSetupInfo.Add(info);
							continue;
						}
						break;

					case DN_ALLERGEN:
						{
							int nSPOSVersion = 0;
							int nConnectionType = 0;
							dbLocation.GetLocSPOSVersion(nLocIdx, nSPOSVersion, nConnectionType);
							if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2606) == FALSE)
							{
								info.m_nStatus = JOBLIST_SETUP_NEED_V4_2606;
								info.m_nJobType = info.m_nExecuteJobType;
								JobSetupInfo.Add(info);
								continue;
							}
						}
						break;

					case DN_WALLETSTAMPRULE:
						{
							int nSPOSVersion = 0;
							int nConnectionType = 0;
							dbLocation.GetLocSPOSVersion(nLocIdx, nSPOSVersion, nConnectionType);

							switch (SysInfo.GetLoyaltyOrSmartPayType())
							{
							case LOYALTY_TYPE_SMP_V4:
								if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == FALSE)
								{
									info.m_nStatus = JOBLIST_SETUP_NEED_V4_2_1579;
									info.m_nJobType = info.m_nExecuteJobType;
									JobSetupInfo.Add(info);
									continue;
								}
								break;

							case LOYALTY_TYPE_LOY_V4:
							default:
								if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1530) == FALSE)
								{
									info.m_nStatus = JOBLIST_SETUP_NEED_V4_2_1530;
									info.m_nJobType = info.m_nExecuteJobType;
									JobSetupInfo.Add(info);
									continue;
								}
								break;
							}
						}
						break;
					}

					if (IsLocationJob(info.m_nValidateJobType))
					{
						bGotLocationJob = TRUE;
						bGotNetworkJob = TRUE;

						switch (nJobType)
						{
						case RX_EODPLU:
						case RX_EODCUSTOMER:
							break;

						default:
							bGotDownloadJob = TRUE;
							break;
						}

						int nTemp = info.m_nJobType;
						info.m_nJobType = info.m_nExecuteJobType;
						AddJobLine(nNetworkIdx, nLocIdx, infoMaster.GetTerminalNo(), info.m_nJobType, info.m_nParam1, bIsImmediateTask);
						info.m_nStatus = JOBLIST_SETUP_ACCEPTED;
						JobSetupInfo.Add(info);
						info.m_nJobType = nTemp;

						nJobCount++;
					}

					if (info.m_nExtraJobType > 0)
					{
						info.m_nJobType = info.m_nExtraJobType;
						info.m_nParam1 = info.m_nExtraParam1;
						AddJobLine(nNetworkIdx, nLocIdx, infoMaster.GetTerminalNo(), info.m_nJobType, info.m_nParam1, bIsImmediateTask);
						info.m_nStatus = JOBLIST_SETUP_ACCEPTED;
						JobSetupInfo.Add(info);
						nJobCount++;
					}
					else if (-1 == info.m_nExtraJobType)
					{
						info.m_nStatus = JOBLIST_SETUP_NEED_DBPRICESET;
						info.m_nParam1 = 0;
						JobSetupInfo.Add(info);
					}
					else if (-2 == info.m_nExtraJobType)
					{
						info.m_nStatus = JOBLIST_SETUP_NEED_KBDLINK;
						info.m_nParam1 = 0;
						JobSetupInfo.Add(info);
					}
				}

				if (bGotLocationJob == FALSE)
				{
					while (GetSize() > nRollbackLocation)
					{
						RemoveAt(GetSize() - 1);
					}
				}
				else if (TRUE == bGotDownloadJob)
				{
					SetLocDownloadFlag(nRollbackLocation);
				}
			}
		}

		if (bGotNetworkJob == FALSE)
		{
			while (GetSize() > nRollbackNetwork)
			{
				RemoveAt(GetSize() - 1);
			}
		}
	}
	return nJobCount;
}

/**********************************************************************/

void CJobListCSVArray::GetDownloadMasterList( CEposTaskDatabase& TaskDatabase, CReportConsolidationArray<CSortedIntAndInt>& arrayDownloadMasters )
{
	bool bCanDoDownloads;
	dbNetwork.GetDownloadPermissions( bCanDoDownloads );

	CEposTaskHeader TaskHeader;
	TaskDatabase.GetEditableTaskHeader( TaskHeader );

	bool bIsImmediateTask = ( TaskHeader.GetImportType() == IMPORT_IMMEDIATE );
	
	for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange ( nNetworkIdx, nStartIdx, nEndIdx );
		if (nEndIdx < nStartIdx)
		{
			continue;
		}
		
		CArray<int,int> arrayLocIdx;
		for (int x = nStartIdx; x <= nEndIdx; x++)
		{
			arrayLocIdx.Add(x);
		}
	
		for (int n = arrayLocIdx.GetSize() - 1; n >= 0; n--)
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			if (PasswordArray.CheckLocationAccessLocIdx(nLocIdx) == FALSE)
			{
				arrayLocIdx.RemoveAt(n);
			}
		}
	
		int nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );
	
		for ( int x = 0; x < arrayLocIdx.GetSize(); x++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(x);

			CArray<CLocationMasterInfo,CLocationMasterInfo> arrayMasters;
			LocationMasterMap.GetLocationMasterList( nLocIdx, arrayMasters, TRUE, FALSE );

			int nEndIdx = arrayMasters.GetCount() - 1;
			
			if ( nEndIdx > 0 )
			{
				switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
				{
				case CONNECTION_TYPE_STANDARD_BATCH:	
				case CONNECTION_TYPE_STANDARD_REALTIME:
					break;

				default:
					nEndIdx = 0;
					break;
				}
			}

			for ( int nIdx = 0; nIdx <= nEndIdx; nIdx++ )
			{
				CLocationMasterInfo infoMaster = arrayMasters.GetAt( nIdx );

				bool bGotLocationJob = FALSE;
				
				int nDbIdx;
				int nDbNo = dbLocation.GetDbNo( nLocIdx );
				bool bDbOk = dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx );

				for ( int nJobIdx = 0; ( nJobIdx < TaskDatabase.GetEditableTaskJobCount() ) && ( FALSE == bGotLocationJob ); nJobIdx++ )
				{
					CEposTaskJob TaskJob;
					TaskDatabase.GetEditableTaskJob( nJobIdx, TaskJob );
					int nJobType = TaskJob.GetJobNo();

					//RX SALES JOB IS HANDLED IN A SEPARATE LOOP
					//RX REASON JOB IS HANDLED IN A SEPARATE LOOP
					switch ( nJobType )
					{
					case RX_SALES:
					case RX_REASON:
						continue;
					}

					//NOTHING TO DO IF THIS JOB WAS NOT REQUESTED HERE
					if (TaskJob.IsRunAtLocation(nLocIdx, infoMaster.GetTerminalNo()) == FALSE)
					{
						continue;
					}

					int nValidateJobType = nJobType;

					//ALL JOBS EXCEPT BATCH SALES ARE REJECTED FOR SPORTS BOOKER NETWORKS
					switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
					{
					case CONNECTION_TYPE_SPTBOOK_NONE:
					case CONNECTION_TYPE_SPTBOOK_WEB:
					case CONNECTION_TYPE_SMARTENT_NONE:
					case CONNECTION_TYPE_SMARTENT_WEB:
						continue;
					}
					
					switch( nJobType )
					{
					case DN_PLU_KBD:		
						nValidateJobType = DN_PLU;
						break;
					
					case DN_PLU_SET1_KBD:	
						nValidateJobType = DN_PLU_SET1;
						break;

					case DN_PLU_SET2_KBD:	
						nValidateJobType = DN_PLU_SET2;
						break;
					
					case DN_PLU_SET3_KBD:	
						nValidateJobType = DN_PLU_SET3;
						break;
					}

					CKeyboardJobInfo infoKb;
					if (infoKb.GetKbNoFromJobNo(nJobType) != 0)
					{
						nValidateJobType = DN_KBD1;
					}

					//REJECT JOB IF NO CONNECTION AT NETWORK
					switch( nConnectionType )
					{
					case CONNECTION_TYPE_STANDARD_BATCH:
					case CONNECTION_TYPE_STANDARD_REALTIME:
						break;

					default:
						continue;
					}

					//REJECT JOB IF DB NEEDED BUT NOT LINKED
					if ( JobNeedsDbLink( nValidateJobType ) )
					{
						if (FALSE == bDbOk)
						{
							continue;
						}
					}

					//REJECT JOB IF THE JOB TYPE IS UNKNOWN
					if (CEposTaskDatabase::GetJobSortOrder(nValidateJobType) == 0)
					{
						continue;
					}

					//REJECT MODIFIER DOWNLOAD FOR A NO MODIFIER SYSTEM
					if (SysInfo.GetMaxBasePluLen() == 0)
					{
						if (DN_MOD == nValidateJobType)
						{
							continue;
						}
					}

					if (RX_EODPLU == nValidateJobType)
					{
						//REJECT EOD PLU FOR REAL TIME NETWORKS
						if (EcrmanOptions.GetPluProdCSVImportType() == 2)
						{
							if (CONNECTION_TYPE_STANDARD_REALTIME == nConnectionType)
							{
								continue;
							}
						}

						int nType = dbLocation.GetProdCSVImportFolderType(nLocIdx);
						if ((EODIMPORT_FOLDERTYPE_ROOT != nType) && (EODIMPORT_FOLDERTYPE_INTRADING != nType))
						{
							continue;
						}
					}

					//REJECT EOD CUSTOMER FOR REAL TIME NETWORKS
					if (RX_EODCUSTOMER == nValidateJobType)
					{
						if (EcrmanOptions.GetFeaturesCustomerNameImportType() == 2)
						{
							if (CONNECTION_TYPE_STANDARD_REALTIME == nConnectionType)
							{
								continue;
							}
						}
					}

					int nParam1 = 0;
					switch( nValidateJobType )
					{
					case DN_PLU:
					case DN_PLU_SET1:
					case DN_PLU_SET2:
					case DN_PLU_SET3:
						nParam1 = TaskJob.GetParam1BaseJob();

						if (2 == nParam1)
						{
							//REJECT PLU CHANGES ONLY IF NOT IMMEDIATE TASK
							if (FALSE == bIsImmediateTask)
							{
								continue;
							}


							//REJECT PLU CHANGES ONLY FOR OLDER VERSIONS OF SPOS
							int nSPOSVersion;
							int nConnectionType;
							dbLocation.GetLocSPOSVersion(nLocIdx, nSPOSVersion, nConnectionType);
							if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_992) == FALSE)
							{
								continue;
							}
						}
						break;

					case DN_PLU_INFO:
			
						if (FALSE == bIsImmediateTask)
						{
							continue;
						}
						
						break;

					case DN_ALLERGEN:
						{
							int nSPOSVersion;
							int nConnectionType;
							dbLocation.GetLocSPOSVersion( nLocIdx, nSPOSVersion, nConnectionType );
							if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2606) == FALSE)
							{
								continue;
							}
						}
						break;
					}

					if ( IsLocationJob( nValidateJobType ) )
					{
						bGotLocationJob = TRUE;		
						CSortedIntAndInt infoSortedMaster;
						infoSortedMaster.m_nInt1 = nLocIdx;
						infoSortedMaster.m_nInt2 = infoMaster.GetTerminalNo();
						arrayDownloadMasters.Consolidate( infoSortedMaster );
					}
				}
			}
		}
	}
}

/**********************************************************************/

const char* CJobListCSVArray::GetDateFolder(CJobListCSVRecord& JobInfo)
{
	if (JobInfo.GetIsImmediateFlag() == FALSE)
	{
		return m_strDate;
	}
	else
	{
		return "";
	}
}

/**********************************************************************/
