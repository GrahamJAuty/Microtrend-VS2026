/**********************************************************************/
#include "CommsErrors.h"
#include "CommsExportBase.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "PluInfoResourceWrapper.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportPluInfo.h"
/**********************************************************************/

CCommsExportPluInfo::CCommsExportPluInfo( CJobListCSVRecord& JobInfo, bool bFromMemory )
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_strEffectiveDate.Format( "%4.4d%2.2d%2.2d", timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay() );
	
	m_strFilename = "";
	m_strSourcePath = "";
	m_strDestPath = "";
	m_nNetworkIdx = JobInfo.GetNetworkIdx();
	m_nLocIdx = JobInfo.GetLocIdx();
	m_nTerminalNo = JobInfo.GetTerminalNo();
	m_bImmediate = JobInfo.GetIsImmediateFlag();
	m_bFromMemory = bFromMemory;
}

/**********************************************************************/

int CCommsExportPluInfo::PrepareFile( int nParam1 )
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

	m_strDestPath = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );
	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenPlu( info );
		DataManager.ReopenModifier( info );
	}
	
	PrepareFileList( nSPOSVersion, nParam1 );
	if ( ( m_arrayFileInfo.GetSize() == 0 ) && ( m_arrayResourceInfo.GetSize() == 0 ) )
		return COMMS_ERROR_NODATA;

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

void CCommsExportPluInfo::PrepareFileList( int nSPOSVersion, int nParam1 )
{
	CString strLastDownloadDate = dbLocation.GetPluInfoDate( m_nLocIdx, m_nTerminalNo );
	bool bPluByDate = EcrmanOptions.GetPluDownloadByDateFlag();

	CPluInfoResourceWrapper ResourceWrapper;
	m_strSourcePath = ResourceWrapper.GetFolderPathPluInfo();
	m_strSourcePath += "\\";
	
	m_arrayFileInfo.RemoveAll();
	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		if ( dbLocation.CheckPluDownloadRange( m_nLocIdx, PluRecord.GetPluNoInt() ) == FALSE )
			continue;

		__int64 nSSPluNo = PluRecord.GetPluNoInt();
		__int64 nSharpPluNo;
		bool bModifiable;

		if ( CCommsExportBase::CheckPluNo( nSPOSVersion, nSSPluNo, nSharpPluNo, bModifiable ) == FALSE )
			continue;

		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == FALSE )	
			if ( nSharpPluNo >= 99999000 )
				continue;

		ResourceWrapper.SetCurrentPluNo( PLUINFO_MODE_DOWNLOAD, PluRecord.GetPluNoInt() );

		{
			bool bIncludeItem = TRUE;

			CString strModifiedDate = PluRecord.GetModifiedDateTimeInfo();
			CString strDownloadDate = PluRecord.GetDownloadDatePlu();

			if ( 2 == nParam1 )
			{
				bool bModifiedSinceDownload = ( strModifiedDate == "" ) || ( strModifiedDate > strLastDownloadDate ); 

				if ( TRUE == bPluByDate )
				{
					if ( m_strEffectiveDate < strDownloadDate )
						bIncludeItem = FALSE;
					else if ( ( strDownloadDate <= strLastDownloadDate ) && ( FALSE == bModifiedSinceDownload ) )
						bIncludeItem = FALSE;
				}
				else
				{
					bIncludeItem = bModifiedSinceDownload;
				}
			}
			else
			{
				if ( ( TRUE == bPluByDate ) && ( m_strEffectiveDate < strDownloadDate ) )
					bIncludeItem = FALSE;
			}

			if ( FALSE == bIncludeItem )
				continue;
		}
	
		int nModifiers = 0;
		
		if ( TRUE == bModifiable )
		{
			int nMask = 1;
			for ( int nMod = 1; nMod <= 9; nMod++ )
			{
				if ( PluRecord.GetModifierEnable( nMod ) == TRUE )
					nModifiers += nMask;

				nMask *= 2;
			}
		}

		ResourceWrapper.GetSourceFileInfoList( m_arrayFileInfo, m_arrayResourceInfo, bModifiable, nModifiers );
	}
}

/**********************************************************************/

void CCommsExportPluInfo::GetFileInfo( int n, CPluInfoDownloadHelper& item )
{
	if ( ( n >= 0 ) && ( n < m_arrayFileInfo.GetSize() ) )
		item = m_arrayFileInfo.GetAt( n );
}

/**********************************************************************/

void CCommsExportPluInfo::GetResourceInfo( int n, CPluInfoDownloadHelper& item )
{
	if ( ( n >= 0 ) && ( n < m_arrayResourceInfo.GetSize() ) )
		item = m_arrayResourceInfo.GetAt( n );
}

/**********************************************************************/
