/**********************************************************************/
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportTableArea.h"
/**********************************************************************/

CCommsExportTableArea::CCommsExportTableArea( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportTableArea::PrepareFile()
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
	
	int nResult = COMMS_ERROR_NODATA;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;
	
	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenTableAreas( DB_READONLY, m_nLocIdx, info );
	}

	if ( DataManagerNonDb.TableAreas.GetSize() != 0 )
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx ); 
		
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile( nSPOSVersion );
		else	
			nResult = COMMS_ERROR_CREATEFILE;
	}

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseTableAreas( info );
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportTableArea::PrepareSPOSFile(int nSPOSVersion)
{
	CSSFile fileTableAreas;
	m_strSourceFilename = "TableAreas.sql";
	if (fileTableAreas.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add("[AreaID]");
	csvHeader.Add("[AreaName]");
	csvHeader.Add("[TblnoLo]");
	csvHeader.Add("[TblnoHi]");
	csvHeader.Add("[TableOpenMacro]");
	csvHeader.Add("[AutoTabsOnly]");
	csvHeader.Add("[KPEnable1]");
	csvHeader.Add("[KPEnable2]");
	csvHeader.Add("[KPEnable3]");
	csvHeader.Add("[KPEnable4]");
	csvHeader.Add("[KPEnable5]");
	csvHeader.Add("[KPEnable6]");
	csvHeader.Add("[KPEnable7]");
	csvHeader.Add("[KPEnable8]");
	csvHeader.Add("[KPEnable9]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1087) == TRUE)
	{
		csvHeader.Add("[KPEnable10]");
		csvHeader.Add("[KPEnable11]");
		csvHeader.Add("[KPEnable12]");
		csvHeader.Add("[KPEnable13]");
		csvHeader.Add("[KPEnable14]");
		csvHeader.Add("[KPEnable15]");
		csvHeader.Add("[KPEnable16]");
	}

	csvHeader.Add("[SuppressCovers]");

	CSQLOut SQLOut(fileTableAreas, "TableAreas", csvHeader.GetLine());
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManagerNonDb.TableAreas.GetSize();
	for (int nAreaIdx = 0; nAreaIdx < nSize; nAreaIdx++)
	{
		StatusProgress.SetPos(nAreaIdx, nSize);

		CTableAreaCSVRecord Area;
		DataManagerNonDb.TableAreas.GetAt(nAreaIdx, Area);

		CMinimumSPOSVersion Version;
		Area.CheckMinimumSPOSVersion(Version);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_TABLE_AREA, nErrorType);
			continue;
		}

		SQLOut.Add(Area.GetAreaNo());
		SQLOut.Add(Area.GetAreaName());
		SQLOut.Add(Area.GetTableNoLo());
		SQLOut.Add(Area.GetTableNoHi());
		SQLOut.Add(Area.GetTableOpenMacro());
		SQLOut.Add(Area.GetAutoTabsOnlyFlag());

		int nKPCount = 9;
		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1087) == TRUE)
		{
			nKPCount = 16;
		}

		for (int n = 1; n <= nKPCount; n++)
		{
			SQLOut.Add(Area.GetKPFlag(n));
		}

		SQLOut.Add(Area.GetSuppressCoversFlag());

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileTableAreas.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
