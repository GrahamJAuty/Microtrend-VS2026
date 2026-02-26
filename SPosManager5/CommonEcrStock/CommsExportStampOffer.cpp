/**********************************************************************/
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportStampOffer.h"
/**********************************************************************/

CCommsExportStampOffer::CCommsExportStampOffer( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportStampOffer::PrepareFile()
{
	if (m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize())
	{
		return COMMS_ERROR_BADNETIDX;
	}

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";
	
	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nDbIdx = 0;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return COMMS_ERROR_BADDBLINK;
	}

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenStampOffer( info );
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( DataManager.StampOffer.GetSize() != 0 )
	{
		if (CSPOSVersions::CheckSPOSVersion(dbNetwork.GetSPOSVersion(m_nNetworkIdx), SPOS_V4_2_9999) == TRUE)
		{
			nResult = PrepareSPOSFile();
		}
		else
		{
			nResult = COMMS_ERROR_CREATEFILE;
		}
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportStampOffer::PrepareSPOSFile()
{
	CSSFile fileOffer;
	m_strSourceFilename = "StampOffers.sql";
	if (fileOffer.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CCSV csvHeader;
	csvHeader.Add( "[StampOfferID]" );
	csvHeader.Add( "[Enabled]" );
	csvHeader.Add( "[OfferGroupID]");
	csvHeader.Add( "[Threshold]");
	csvHeader.Add( "[Expiry]");
	csvHeader.Add( "[Description]");
	csvHeader.Add( "[Action]");
	
	CSQLOut SQLOut ( fileOffer, "StampOffers", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.StampOffer.GetSize();
	for ( int nOfferIdx = 0; nOfferIdx < nSize; nOfferIdx++ )
	{
		StatusProgress.SetPos ( nOfferIdx, nSize );
		
		CStampOfferCSVRecord Offer;
		DataManager.StampOffer.GetAt( nOfferIdx, Offer );
		
		SQLOut.Add(Offer.GetStampOfferID() );
		SQLOut.Add(Offer.GetEnableFlag());
		SQLOut.Add(Offer.GetOfferGroupNo());
		SQLOut.Add(Offer.GetThreshold());
		SQLOut.Add(Offer.GetExpiry());
		SQLOut.Add(Offer.GetDescription());
		SQLOut.Add(0); //ACTION

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileOffer.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

