/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "PosTrayDateRange.h"
#include "PosTrayExportConsolidatorS4Labour.h"
#include "PosTrayOptions.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "PosTrayExportCreatorS4Labour.h"
/**********************************************************************/

CPosTrayExportCreatorS4Labour::CPosTrayExportCreatorS4Labour( CPosTrayS4LabourScheduleInfo& infoExport, CS4LabourLocationOptions& Options, bool& bQuitNow ) : m_infoExport( infoExport ), m_Options( Options ), m_bQuitNow( bQuitNow )
{
	m_bQuitNow = FALSE;
}

/**********************************************************************/

CPosTrayExportCreatorS4Labour::~CPosTrayExportCreatorS4Labour(void)
{
}

/**********************************************************************/

void CPosTrayExportCreatorS4Labour::SetExportDate( COleDateTime& date )
{
	COleDateTime dateExport = COleDateTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	m_dateExport = dateExport;
}

/**********************************************************************/

__int64 CPosTrayExportCreatorS4Labour::ProcessTask()
{
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	__int64 nLineCount = ProcessTaskInternal();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	return nLineCount;
}

/**********************************************************************/

__int64 CPosTrayExportCreatorS4Labour::ProcessTaskInternal()
{
	__int64 nLineCount = 0;

	m_strExportFolder = "";
	m_strExportFolder += dbLocation.GetFolderPathSysset( m_infoExport.m_nLocIdx );
	m_strExportFolder += "\\S4LABOUR";
	::CreateSubdirectory( m_strExportFolder );
	m_strExportFolder += "\\EXPORTS";
	::CreateSubdirectory( m_strExportFolder );

	if ( ::ExistSubdirectory( m_strExportFolder ) == FALSE )
		return -1;

	{
		m_ReportSession.ClearRecord();
		int nHour = EcrmanOptions.GetReportsDayStartHour();
		int nMinute = EcrmanOptions.GetReportsDayStartMinute();
		
		m_ReportSession.SetSessionStartHour( nHour );
		m_ReportSession.SetSessionStartMinute( nMinute );

		nMinute--;
		if ( nMinute < 0 )
		{
			nMinute = 0;
			nHour--;

			if ( nHour < 0 )
				nHour = 23;
		}

		m_ReportSession.SetSessionEndHour( nHour );
		m_ReportSession.SetSessionEndMinute( nMinute );
		m_ReportSession.SetHistoryDateType(1);
		m_ReportSession.SetSessionStartType(0);
		m_ReportSession.SetEODFlag( FALSE );
	}

	{
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( m_infoExport.m_nLocIdx );

		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		}
	}

	CEposSelectArray SelectArray;
	SelectArray.SelectLocation( m_infoExport.m_nLocIdx );
	
	CPosTrayExportConsolidatorS4Labour ExportConsolidator( SelectArray, m_bQuitNow );
	ExportConsolidator.SetUseTermIDFlag( m_Options.GetUseTermIDFlag() );
	ExportConsolidator.ResetS4Labour( m_dateExport, m_ReportSession );
	
	CString strLocID = m_Options.GetLocID();
	::TrimSpaces( strLocID, FALSE );

	if ( strLocID == "" )
	{
		nLineCount = -1;
	}
	else
	{
		if ( ExportConsolidator.PrepareFields() == TRUE )
		{
			ExportConsolidator.Consolidate();
		
			CString strTempFilepath = "";
			strTempFilepath.Format( "%s\\Cash-%s-%4.4d%2.2d%2.2d.csv",
				(const char*) m_strExportFolder,
				(const char*) strLocID,
				m_dateExport.GetYear(),
				m_dateExport.GetMonth(),
				m_dateExport.GetDay() );
			
			if ( FALSE == m_bQuitNow )
			{
				bool bResult = FALSE;
				nLineCount = ExportConsolidator.CreateExport( strTempFilepath, bResult );

				if ( FALSE == bResult )
					nLineCount = -1;		
			}
		}
	}
	
	return nLineCount;
}

/**********************************************************************/
