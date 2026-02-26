/**********************************************************************/
#include "CommsErrors.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include <array>
/**********************************************************************/
#include "CommsExportTimePlan.h"
/**********************************************************************/

CCommsExportTimePlan::CCommsExportTimePlan( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportTimePlan::PrepareFile()
{
	if ( m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize() )
		return COMMS_ERROR_BADNETIDX;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return COMMS_ERROR_BADLOCIDX;

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	int nResult = COMMS_ERROR_NODATA;
	int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );

	CDataManagerInfo info;

	if (FALSE == m_bFromMemory)
	{
		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			DataManager.OpenDatabaseQuickNoFiles(nDbIdx, TRUE);
			DataManager.ReopenTimePlan(info);
			DataManager.ReopenTimePlanEntry(info);
			DataManager.ReopenTimePlanTemplateEntry(info);
			DataManager.ReopenPromotion(info);
			DataManager.ReopenMixMatch(info);
		}
		else
		{
			return COMMS_ERROR_NODATA;
		}
	}

	if ( DataManager.TimePlan.GetSize() != 0 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile( nSPOSVersion );
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportTimePlan::PrepareSPOSFile( int nSPOSVersion )
{
	CSSFile fileTimePlan;
	m_strSourceFilename = "TimePlan.sql";
	if ( fileTimePlan.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[PlanNo]" );
	csvHeader.Add( "[Name]" );
	
	CSQLOut SQLOut ( fileTimePlan, "TimePlans", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	int nSize = DataManager.TimePlan.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos ( n, nSize );

		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt( n, Plan );

		SQLOut.Add( Plan.GetPlanNo() );
		SQLOut.Add( Plan.GetPlanName() );

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	csvHeader.RemoveAll();
	csvHeader.Add( "[PlanNo]" );
	csvHeader.Add( "[EntryNo]" );
	csvHeader.Add( "[Active]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Type]" );
	csvHeader.Add( "[Sunday]" );
	csvHeader.Add( "[Monday]" );
	csvHeader.Add( "[Tuesday]" );
	csvHeader.Add( "[Wednesday]" );
	csvHeader.Add( "[Thursday]" );
	csvHeader.Add( "[Friday]" );
	csvHeader.Add( "[Saturday]" );
	csvHeader.Add( "[StartHours]" );
	csvHeader.Add( "[StartMins]" );
	csvHeader.Add( "[EndHours]" );
	csvHeader.Add( "[EndMins]" );
	csvHeader.Add( "[EndPeriod]" );
	csvHeader.Add( "[IPam1]" );
	csvHeader.Add( "[IPam2]" );
	csvHeader.Add( "[SPam1]" );
	csvHeader.Add( "[DateFrom]" );
	csvHeader.Add( "[DateTo]" );

	CSQLOut SQLOut2 ( fileTimePlan, "TimePlanEntrys", csvHeader.GetLine() );
	
	SQLOut2.WriteDelete();

	std::array<int, TIMEPLAN_MAX_PLANS + 1> nMaxEntry{};

	StatusProgress.Lock();
	nSize = DataManager.TimePlanEntry.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos ( n, nSize );

		CTimePlanEntriesCSVRecord Entry;
		DataManager.TimePlanEntry.GetAt( n, Entry );

		CMinimumSPOSVersion Version;
		Entry.CheckMinimumSPOSVersion( Version );
		
		int nErrorType = -1;
		Version.CheckSPOSVersion( nSPOSVersion, nErrorType );

		if ( nErrorType != -1 )
		{
			TallyDownloadExclusion( NODE_TIMEPLAN_ENTRY, nErrorType );
			continue;
		}

		int nPlanNo = Entry.GetPlanNo();
		int nEntryNo = Entry.GetEntryNo();

		if ( ( nPlanNo >= 1 ) || ( nPlanNo <= TIMEPLAN_MAX_PLANS ) )
		{
			if ( nEntryNo > nMaxEntry[nPlanNo] )
			{
				nMaxEntry[nPlanNo] = nEntryNo;
			}
		}

		AddSQLTimeplanEntry(FALSE, SQLOut2, Entry);
	}

	if (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE)
	{
		AddPromotionTemplatePlans(nSPOSVersion, SQLOut2, nMaxEntry);
		AddMixMatchTemplatePlans(nSPOSVersion, SQLOut2, nMaxEntry);
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileTimePlan.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

void CCommsExportTimePlan::AddPromotionTemplatePlans(int nSPOSVersion, CSQLOut& SQLOut, std::array<int, TIMEPLAN_MAX_PLANS + 1>& nMaxEntry)
{
	for (int p = 0; p < DataManager.Promotion.GetSize(); p++)
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt(p, Promo);

		CReportConsolidationArray<CSortedIntByInt> TimeplanMap;
		Promo.GetTimeplanMap(TimeplanMap);

		for (int m = 0; m < TimeplanMap.GetSize(); m++)
		{
			CSortedIntByInt item;
			TimeplanMap.GetAt(m, item);

			int nPlanNo = item.m_nKey;
			int nEntryNo = item.m_nVal;	

			if ( (nPlanNo < 1) || (nPlanNo > TIMEPLAN_MAX_PLANS) || ( nEntryNo < 1 ))
			{
				continue;
			}

			if ( nMaxEntry[nPlanNo] >= TimePlanEntries::EntryNo.Max )
			{
				TallyDownloadExclusion(NODE_TIMEPLAN_ENTRY, 9998);
				continue;
			}

			CTimePlanEntriesCSVRecord Entry;
			Entry.SetPlanNo(nPlanNo);
			Entry.SetEntryNo(nEntryNo);

			int nPos = 0;
			if (DataManager.TimePlanTemplateEntry.FindKey(Entry, nPos) == FALSE)
			{
				continue;
			}

			DataManager.TimePlanTemplateEntry.GetAt(nPos, Entry);

			CMinimumSPOSVersion Version;
			Entry.CheckMinimumSPOSVersion(Version);

			int nErrorType = -1;
			Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

			if (nErrorType != -1)
			{
				TallyDownloadExclusion(NODE_TIMEPLAN_ENTRY, nErrorType);
				continue;
			}

			nMaxEntry[nPlanNo]++;
			Entry.SetEntryNo(nMaxEntry[nPlanNo]);
			Entry.SetTypeAndSelection(1, Promo.GetPromoNo());
			AddSQLTimeplanEntry(TRUE, SQLOut, Entry);
		}
	}
}

/**********************************************************************/

void CCommsExportTimePlan::AddMixMatchTemplatePlans(int nSPOSVersion, CSQLOut& SQLOut, std::array<int, TIMEPLAN_MAX_PLANS + 1>& nMaxEntry)
{
	for (int p = 0; p < DataManager.MixMatch.GetSize(); p++)
	{
		CMixMatchCSVRecord MixMatch;
		DataManager.MixMatch.GetAt(p, MixMatch);

		CReportConsolidationArray<CSortedIntByInt> TimeplanMap;
		MixMatch.GetTimeplanMap(TimeplanMap);

		for (int m = 0; m < TimeplanMap.GetSize(); m++)
		{
			CSortedIntByInt item;
			TimeplanMap.GetAt(m, item);

			int nPlanNo = item.m_nKey;
			int nEntryNo = item.m_nVal;

			if ((nPlanNo < 1) || (nPlanNo > TIMEPLAN_MAX_PLANS) || (nEntryNo < 1))
			{
				continue;
			}

			if (nMaxEntry[nPlanNo] >= TimePlanEntries::EntryNo.Max)
			{
				TallyDownloadExclusion(NODE_TIMEPLAN_ENTRY, 9997);
				continue;
			}

			CTimePlanEntriesCSVRecord Entry;
			Entry.SetPlanNo(nPlanNo);
			Entry.SetEntryNo(nEntryNo);

			int nPos = 0;
			if (DataManager.TimePlanTemplateEntry.FindKey(Entry, nPos) == FALSE)
			{
				continue;
			}

			DataManager.TimePlanTemplateEntry.GetAt(nPos, Entry);

			CMinimumSPOSVersion Version;
			Entry.CheckMinimumSPOSVersion(Version);

			int nErrorType = -1;
			Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

			if (nErrorType != -1)
			{
				TallyDownloadExclusion(NODE_TIMEPLAN_ENTRY, nErrorType);
				continue;
			}

			nMaxEntry[nPlanNo]++;
			Entry.SetEntryNo(nMaxEntry[nPlanNo]);
			Entry.SetTypeAndSelection(2, MixMatch.GetMixMatchNo());
			AddSQLTimeplanEntry(TRUE, SQLOut, Entry);
		}
	}
}

/**********************************************************************/

void CCommsExportTimePlan::AddSQLTimeplanEntry(bool bTemplate, CSQLOut& SQLOut, CTimePlanEntriesCSVRecord& Entry)
{
	SQLOut.Add(Entry.GetPlanNo());
	SQLOut.Add(Entry.GetEntryNo());
	SQLOut.Add(Entry.GetActiveFlag());
	SQLOut.Add(Entry.GetName());
	SQLOut.Add(Entry.GetType());
	SQLOut.Add(Entry.GetSundayFlag());
	SQLOut.Add(Entry.GetMondayFlag());
	SQLOut.Add(Entry.GetTuesdayFlag());
	SQLOut.Add(Entry.GetWednesdayFlag());
	SQLOut.Add(Entry.GetThursdayFlag());
	SQLOut.Add(Entry.GetFridayFlag());
	SQLOut.Add(Entry.GetSaturdayFlag());
	SQLOut.Add(Entry.GetStartHour());
	SQLOut.Add(Entry.GetStartMinute());
	SQLOut.Add(Entry.GetEndHour());
	SQLOut.Add(Entry.GetEndMinute());
	SQLOut.Add(Entry.GetEndPeriod());
	SQLOut.Add(Entry.GetSelection());
	SQLOut.Add(bTemplate ? 987 : 0);
	SQLOut.Add("");

	COleDateTime oleStartDate = Entry.GetStartDate();
	COleDateTime oleEndDate = Entry.GetEndDate();

	if (oleStartDate.GetStatus() == COleDateTime::valid && oleEndDate.GetStatus() == COleDateTime::valid)
	{
		SQLOut.Add(oleStartDate.m_dt - 2.0, 0);
		SQLOut.Add(oleEndDate.m_dt - 2.0, 0);
	}
	else
	{
		SQLOut.Add(0.0, 0);
		SQLOut.Add(0.0, 0);
	}

	SQLOut.WriteInsert();
	TallyDownloadAccept();
}

/**********************************************************************/
