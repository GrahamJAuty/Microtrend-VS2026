/**********************************************************************/
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "..\CommonEcrStockTray\SPosVersions.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportLoyaltyScheme.h"
/**********************************************************************/

CCommsExportLoyaltyScheme::CCommsExportLoyaltyScheme( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportLoyaltyScheme::PrepareFile()
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

	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nResult = COMMS_ERROR_NODATA;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return COMMS_ERROR_BADDBLINK;
	}

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles(nDbIdx,TRUE);
		DataManager.ReopenLoyaltyScheme( info );
	}

	if (DataManager.LoyaltyScheme.GetSize() != 0)
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion(m_nNetworkIdx);

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			nResult = PrepareSPOSFile(nSPOSVersion);
		}
		else
		{
			nResult = COMMS_ERROR_CREATEFILE;
		}
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportLoyaltyScheme::PrepareSPOSFile( int nSPOSVersion )
{
	CSSFile fileLoyalty;
	m_strSourceFilename = "Loyalty5.sql";
	if (fileLoyalty.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CCSV csvHeader;
	csvHeader.Add( "[SchemeNo]" );
	csvHeader.Add( "[Active]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Promotion]" );
	csvHeader.Add( "[PromoType]" );
	csvHeader.Add( "[CheckDigit]" );
	csvHeader.Add( "[Points]" );
	csvHeader.Add( "[LowerID]" );
	csvHeader.Add( "[UpperID]" );
	csvHeader.Add( "[Format]" );
	csvHeader.Add( "[IsLocal]" );
	csvHeader.Add( "[StaffCard]" );
	csvHeader.Add( "[LoyMacro]" );
	csvHeader.Add( "[LimitDay]" );
	csvHeader.Add( "[LimitWeek]" );
	csvHeader.Add( "[LimitMonth1]" );
	csvHeader.Add( "[LimitMonth2]" );
	csvHeader.Add( "[LimitMonth3]" );
	csvHeader.Add( "[LimitMonth4]" );
	csvHeader.Add( "[LimitMonth5]" );
	csvHeader.Add( "[LimitMonth6]" );
	csvHeader.Add( "[LimitMonth7]" );
	csvHeader.Add( "[LimitMonth8]" );
	csvHeader.Add( "[LimitMonth9]" );
	csvHeader.Add( "[LimitMonth10]" );
	csvHeader.Add( "[LimitMonth11]" );
	csvHeader.Add( "[LimitMonth12]" );
	csvHeader.Add( "[StaffCreditEnable]" );
	csvHeader.Add( "[Promotion2]" );
	csvHeader.Add( "[Promotion3]" );
	csvHeader.Add( "[Promotion4]" );
	csvHeader.Add( "[Promotion5]" );
	csvHeader.Add( "[PaymentPermissions]" );
	csvHeader.Add( "[PaymentPermissionsMode]" );
		
	CSQLOut SQLOut ( fileLoyalty, "Loyalty5", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.LoyaltyScheme.GetSize();
	for ( int nSchemeIdx = 0; nSchemeIdx < nSize; nSchemeIdx++ )
	{
		StatusProgress.SetPos ( nSchemeIdx, nSize );

		CLoyaltySchemeCSVRecord Scheme;
		DataManager.LoyaltyScheme.GetAt ( nSchemeIdx, Scheme );

		CMinimumSPOSVersion Version;
		Scheme.CheckMinimumSPOSVersion( Version );
		
		int nErrorType = -1;
		Version.CheckSPOSVersion( nSPOSVersion, nErrorType );

		if ( nErrorType != -1 )
		{
			TallyDownloadExclusion( NODE_LOYALTY_SCHEME, nErrorType );
			continue;
		}

		SQLOut.Add( Scheme.GetSchemeNo() );
		SQLOut.Add( Scheme.GetActiveFlag() );
		SQLOut.Add( Scheme.GetSchemeName() );
		SQLOut.Add( Scheme.GetSelection() );
		SQLOut.Add( Scheme.GetActionType() );
		SQLOut.Add( Scheme.GetCheckDigitFlag() );
		SQLOut.Add( Scheme.GetPencePerPoint() );
		SQLOut.Add( Scheme.GetNumberStartLower() );
		SQLOut.Add( Scheme.GetNumberStartUpper() );
		SQLOut.Add( Scheme.GetFormat() );
		SQLOut.Add( "1" );
		SQLOut.Add( Scheme.GetStaffCardFlag() );
		SQLOut.Add( Scheme.GetMacroNo() );
		SQLOut.Add( Scheme.GetStaffLimitDay() );
		SQLOut.Add( Scheme.GetStaffLimitWeek() );

		for ( int n = 0; n < 12; n++ )
		{
			SQLOut.Add( Scheme.GetStaffLimitMonth(n) );
		}

		SQLOut.Add( Scheme.GetStaffCreditFlag() );
		SQLOut.Add( Scheme.GetPromoNo(1) );
		SQLOut.Add( Scheme.GetPromoNo(2) );
		SQLOut.Add( Scheme.GetPromoNo(3) );
		SQLOut.Add( Scheme.GetPromoNo(4) );
		SQLOut.Add( Scheme.GetDeniedPaymentsForVPM() );
		SQLOut.Add( "1" );
			
		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileLoyalty.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
