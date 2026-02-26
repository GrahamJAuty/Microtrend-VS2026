/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCustomerBalance.h"
/**********************************************************************/

CEposReportConsolCustomerBalanceID::CEposReportConsolCustomerBalanceID()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCustomerBalanceID::Reset()
{
	m_nLocIdx = 0;
	m_nSortType = 0;
	m_nSortIdx = 0;
	m_nCustomerID = 0;
	m_dBalance = 0.0;
}

/**********************************************************************/

void CEposReportConsolCustomerBalanceID::Add (CEposReportConsolCustomerBalanceID& source )
{
	m_dBalance += source.m_dBalance;
}

/**********************************************************************/

int CEposReportConsolCustomerBalanceID::Compare( CEposReportConsolCustomerBalanceID& source, int nHint )
{
	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( m_nSortType != source.m_nSortType )
		return ( ( m_nSortType > source.m_nSortType ) ? 1 : -1 );

	if ( m_nSortIdx != source.m_nSortIdx )
		return ( ( m_nSortIdx > source.m_nSortIdx ) ? 1 : -1 );

	if ( m_nCustomerID != source.m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolCustomerBalanceID::CompareLocIdx( CEposReportConsolCustomerBalanceID& source )
{
	return ( m_nLocIdx == source.m_nLocIdx );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomerBalance::CEposReportCustomerBalance( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_CUSTOMER_BALANCE;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

const char* CEposReportCustomerBalance::GetReportTitle( bool bDummy )
{
	CString strType = GetReportNameInternal( m_nReportType );
	strType += " ";
	
	m_strTitle.Format ( "%s (%s)",
		(const char*) strType,
		(const char*) m_strConLevelTitle );
				
	return m_strTitle;
}

/**********************************************************************/

void CEposReportCustomerBalance::Consolidate()
{
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();
	DataManagerNonDb.SessionDateTimeFilter.SetCustomerBalanceCheckDates();

	//DO THIS BEFORE CHECKING LOCATIONS IN SELECT ARRAY
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );

	//CONSOLIDATE INITIAL BALANCES BY CUSTOMER AND LOCATION
	int nCurrentLocIdx = -1;
	for( int n = 0; n < m_SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem( n, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			bool bGetCustomers = TRUE;
			if ( SysInfo.IsPMSSystem() == TRUE )
				if ( dbLocation.IsPMSPseudoLocation( ListItem.m_nLocIdx ) == TRUE )
					bGetCustomers = FALSE;

			if ( TRUE == bGetCustomers )
				DataManagerNonDb.CustomerNameTable.Account.GetCustomerListForLocation( ListItem.m_nLocIdx, m_ConsolArrayStage1 );

			nCurrentLocIdx = ListItem.m_nLocIdx;
		}
	}

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Checking customer transactions" );

	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( FALSE == m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		COleDateTime timeTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		COleDateTime dateTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		CString strDate = "";
		CString strTime = "";
		bool bValidTime = FALSE;

		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) == TRUE )
		{
			CCSVAccountHistory csvIn( strBuffer );

			int nID = 0;
			double dLineValue = 0.0;

			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_HEADER:
				strDate = infoFile.m_strDateTran;
				strTime = csvIn.GetHeaderLineTime();
				bValidTime = ValidateAccountDate( infoFile.m_nLocIdx, strDate, strTime, timeTransaction, dateTransaction );
				break;

			case ACCOUNT_LINETYPE_RECEIVE:
				if ( ( TRUE == bValidTime ) && ( csvIn.GetReceiveType() == CASHRSP_ITEMPAYTYPE_CUSTOMER ) )
				{
					nID = csvIn.GetReceiveID();
					dLineValue = -csvIn.GetReceiveValue();
				}
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ( ( TRUE == bValidTime ) && ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_CUSTOMER ) )
				{
					nID = csvIn.GetChargeID();
					dLineValue = csvIn.GetChargeTender();
				}
				break;
			}

			if ( nID != 0 )
			{
				CCustomerSelectionListAccountInfo infoSelect;
				infoSelect.m_nCustomerID = nID;
				infoSelect.m_nLocIdx = infoFile.m_nLocIdx;
				GetOleDateSince2011( -1, infoSelect.m_startTime );

				int nType, nDummy;
				DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoSelect, nType, nDummy );

				if ( timeTransaction >= infoSelect.m_startTime )
				{
					CEposReportConsolCustomerBalanceID Balance;
					Balance.m_nSortType = 0;
					Balance.m_nSortIdx = 0;
					Balance.m_nLocIdx = infoFile.m_nLocIdx;
					Balance.m_nCustomerID = nID;
					Balance.m_dBalance = dLineValue;
					m_ConsolArrayStage1.Consolidate( Balance );

					m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
					m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, dateTransaction );
				}
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();

	StatusProgress.Lock( "Sorting balances for report" );

	int nSize = m_ConsolArrayStage1.GetSize();
	
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );

		CEposReportConsolCustomerBalanceID BalanceID;
		m_ConsolArrayStage1.GetAt( n, BalanceID );

		if ( CPriceHelpers::CompareDoubles( BalanceID.m_dBalance, 0.0, 3 ) != 0 )
		{
			CCustomerSelectionListAccountInfo infoSelect;
			infoSelect.m_nLocIdx = BalanceID.m_nLocIdx;
			infoSelect.m_nCustomerID = BalanceID.m_nCustomerID;

			int nType, nDummy;
			DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoSelect, nType, nDummy );
			switch ( nType )
			{
			case NODE_CUSTOMER_ACCOUNT:
				BalanceID.m_nSortType = 0;
				BalanceID.m_nSortIdx = infoSelect.m_nSortIdx;
				m_ConsolArrayStage2.Consolidate( BalanceID );
				break;

			case NODE_CUSTOMER_ONEOFF:
				BalanceID.m_nSortType = 1;
				BalanceID.m_nSortIdx = infoSelect.m_nSortIdx;
				m_ConsolArrayStage2.Consolidate( BalanceID );
				break;

			case NODE_PLACEHOLDER:
			default:
				BalanceID.m_nSortType = 2;
				BalanceID.m_nSortIdx = BalanceID.m_nCustomerID;
				m_ConsolArrayStage2.Consolidate( BalanceID );
				break;
			}
		}
	}

	m_ConsolArrayStage1.RemoveAll();

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCustomerBalance::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Customer ID", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Customer Type", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Customer Name", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Balance", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 100 );
	
	CEposReportConsolCustomerBalanceID infoBalanceCurrentLoc;
	infoBalanceCurrentLoc.m_nLocIdx = -1;
	double dLocationTotal = 0.0;
	double dGrandTotal = 0.0;
	int nLocCount = 0;

	int nSize = m_ConsolArrayStage2.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	for ( int nPos = 0; nPos < m_ConsolArrayStage2.GetSize(); nPos++ )
	{
		CEposReportConsolCustomerBalanceID infoBalance;
		m_ConsolArrayStage2.GetAt( nPos, infoBalance );

		if ( infoBalance.CompareLocIdx( infoBalanceCurrentLoc ) == FALSE )
		{
			if ( infoBalanceCurrentLoc.m_nLocIdx != -1 )
			{
				WriteTotal( "Loc Total", dLocationTotal );
				nLocCount++;
			}

			CString strHeader;
			strHeader.Format( "<..>%s", 
				dbLocation.GetName( infoBalance.m_nLocIdx ) );

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );

			infoBalanceCurrentLoc = infoBalance;
			dLocationTotal = 0.0;
		}

		CString strID;
		strID.Format( "%8.8d", infoBalance.m_nCustomerID );

		CCustomerSelectionListAccountInfo infoName;
		infoName.m_nLocIdx = infoBalance.m_nLocIdx;
		infoName.m_nCustomerID = infoBalance.m_nCustomerID;

		CString strStatus = "";
		switch( infoBalance.m_nSortType )
		{
		case 1:
			DataManagerNonDb.CustomerNameTable.OneOff.GetCustomerStatus( infoName );
			strStatus = "One-off";
			break;

		case 0:
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( infoName );
			strStatus = "Account";
			break;

		case 2:
		default:
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( infoName );
			strStatus = "Unknown";
			break;
		}

		CCSV csv( '\t' );
		csv.Add( strID );
		csv.Add( strStatus );
		csv.Add( infoName.GetReportName() );
		
		switch( CPriceHelpers::CompareDoubles( infoBalance.m_dBalance, 0.0, SysInfo.GetDPValue() ) )
		{
		case -1:
			csv.Add( -infoBalance.m_dBalance, SysInfo.GetDPValue() );
			csv.Add ( "CR" );
			break;

		case 1:
			csv.Add( infoBalance.m_dBalance, SysInfo.GetDPValue() );
			break;

		default:
			csv.Add( 0.0, SysInfo.GetDPValue() );
			break;
		}

		dLocationTotal += infoBalance.m_dBalance;
		dGrandTotal += infoBalance.m_dBalance;

		m_ReportFile.WriteReportDataLine( csv.GetLine() );
	}

	StatusProgress.Unlock();

	if ( infoBalanceCurrentLoc.m_nLocIdx != -1 )
	{
		WriteTotal( "Loc Total", dLocationTotal );
		nLocCount++;
	}

	if ( nLocCount >= 2 )
		WriteTotal( "Grand Total", dGrandTotal );
	
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomerBalance::WriteTotal( const char* szTotal, double dTotal )
{
	m_ReportFile.WriteReportMiscLine( "\t\t\t<LI>" );

	CCSV csv( '\t' );
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( szTotal );

	switch( CPriceHelpers::CompareDoubles( dTotal, 0.0, SysInfo.GetDPValue() ) )
	{
	case -1:
		csv.Add( -dTotal, SysInfo.GetDPValue() );
		csv.Add ( "CR" );
		break;

	case 1:
		csv.Add( dTotal, SysInfo.GetDPValue() );
		break;

	default:
		csv.Add( 0.0, SysInfo.GetDPValue() );
		break;
	}

	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
