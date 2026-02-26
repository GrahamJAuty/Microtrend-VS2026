/**********************************************************************/
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCustomerActivity.h"
/**********************************************************************/

CEposReportConsolCustomerActivityID::CEposReportConsolCustomerActivityID()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCustomerActivityID::Reset()
{
	m_nLocIdx = 0;
	m_nCustomerID = 0;
	m_strDate = "";
	m_strTime = "";
	m_nAction = 0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolCustomerActivityID::Add (CEposReportConsolCustomerActivityID& source )
{
	m_dValue += source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolCustomerActivityID::Compare( CEposReportConsolCustomerActivityID& source, int nHint )
{
	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( m_nCustomerID != source.m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );
	
	if ( m_nAction != source.m_nAction )
		return ( ( m_nAction > source.m_nAction ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolCustomerActivitySortIdx::CEposReportConsolCustomerActivitySortIdx()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCustomerActivitySortIdx::Reset()
{
	m_nLocSort = 0;
	m_nSortIdxType = 0;
	m_nSortIdxNum = 0;
	m_strDate = "";
	m_strTime = "";
	m_nAction = 0;
	m_nCustomerID = 0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolCustomerActivitySortIdx::Add (CEposReportConsolCustomerActivitySortIdx& source )
{
	m_dValue += source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolCustomerActivitySortIdx::Compare( CEposReportConsolCustomerActivitySortIdx& source, int nHint )
{
	if ( m_nLocSort != source.m_nLocSort )
		return ( ( m_nLocSort > source.m_nLocSort ) ? 1 : -1 );

	if ( m_nSortIdxType != source.m_nSortIdxType )
		return ( ( m_nSortIdxType > source.m_nSortIdxType ) ? 1 : -1 );

	if ( m_nSortIdxNum != source.m_nSortIdxNum )
		return ( ( m_nSortIdxNum > source.m_nSortIdxNum ) ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );
	
	if ( m_nAction != source.m_nAction )
		return ( ( m_nAction > source.m_nAction ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolCustomerActivitySortIdx::CompareCustomer ( CEposReportConsolCustomerActivitySortIdx& source )
{
	if ( m_nLocSort != source.m_nLocSort )
		return FALSE;

	if ( m_nSortIdxType != source.m_nSortIdxType )
		return FALSE;

	if ( m_nSortIdxNum != source.m_nSortIdxNum )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomerActivity::CEposReportCustomerActivity( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_CUSTOMER_ACTIVITY;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportCustomerActivity::Consolidate()
{
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();
	
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Checking customer transactions" );
	
	bool bInclude = FALSE;

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
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
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVAccountHistory csvIn ( strBuffer );

			int nID = 0;
			int nAction = -1;
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
					nAction = 0;
					nID = csvIn.GetChargeID();
					dLineValue = csvIn.GetReceiveValue();
				}
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ( ( TRUE == bValidTime ) && ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_CUSTOMER ) )
				{
					nAction = 1;
					nID = csvIn.GetChargeID();
					dLineValue = csvIn.GetChargeTender();
				}
				break;
			}

			if ( nAction != -1 )
			{
				CEposReportConsolCustomerActivityID infoActivity;
				infoActivity.m_nLocIdx = infoFile.m_nLocIdx;
				infoActivity.m_nCustomerID = csvIn.GetChargeID();
				infoActivity.m_strDate = strDate;
				infoActivity.m_strTime = strTime;
				infoActivity.m_nAction = nAction;
				infoActivity.m_dValue = dLineValue;

				m_ConsolArrayID.Consolidate( infoActivity );

				m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
				m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, dateTransaction );
			}
		}
		
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();

	//SORT BY NAME
	int nSize = m_ConsolArrayID.GetSize();
	StatusProgress.Lock( TRUE, "Sorting by name" );

	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );

		int nIdx = ( nSize - n ) - 1;

		CEposReportConsolCustomerActivityID infoActivityID;
		m_ConsolArrayID.GetAt( nIdx, infoActivityID );

		int nSortIdxType = 0;
		int nSortIdxNum = 0;

		CCustomerSelectionListAccountInfo infoListSelect;
		infoListSelect.m_nLocIdx = infoActivityID.m_nLocIdx;
		infoListSelect.m_nCustomerID = infoActivityID.m_nCustomerID;

		int nType, nDummy;
		DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoListSelect, nType, nDummy );
		switch( nType )
		{
		case NODE_CUSTOMER_ACCOUNT:
			nSortIdxType = 0;
			nSortIdxNum = infoListSelect.m_nSortIdx;
			break;

		case NODE_CUSTOMER_ONEOFF:
			nSortIdxType = 1;
			nSortIdxNum = infoListSelect.m_nSortIdx;
			break;

		case NODE_PLACEHOLDER:
		default:
			nSortIdxType = 2;
			nSortIdxNum = infoActivityID.m_nCustomerID;
			break;
		}

		CEposReportConsolCustomerActivitySortIdx infoActivitySortIdx;
		infoActivitySortIdx.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( infoActivityID.m_nLocIdx );
		infoActivitySortIdx.m_nSortIdxType = nSortIdxType;
		infoActivitySortIdx.m_nSortIdxNum = nSortIdxNum;
		infoActivitySortIdx.m_strDate = infoActivityID.m_strDate;
		infoActivitySortIdx.m_strTime = infoActivityID.m_strTime;
		infoActivitySortIdx.m_nAction = infoActivityID.m_nAction;
		infoActivitySortIdx.m_nCustomerID = infoActivityID.m_nCustomerID;
		infoActivitySortIdx.m_dValue = infoActivityID.m_dValue;
		
		m_ConsolArraySortIdx.Consolidate( infoActivitySortIdx );

		m_ConsolArrayID.RemoveAt( nIdx );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCustomerActivity::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Customer ID", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Action", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 100 );
	
	CEposReportConsolCustomerActivitySortIdx CurrentCustomer;
	CurrentCustomer.m_nSortIdxType = -1;

	double dGrandTotalSpend = 0.0;
	double dGrandTotalPayment = 0.0;
	double dTotalSpend = 0.0;
	double dTotalPayment = 0.0;

	CString strID = "";

	int nSize = m_ConsolArraySortIdx.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	for ( int nPos = 0; nPos < m_ConsolArraySortIdx.GetSize(); nPos++ )
	{
		CEposReportConsolCustomerActivitySortIdx infoActivity;
		m_ConsolArraySortIdx.GetAt( nPos, infoActivity );
		int nLocIdx = dbLocation.GetLocIdxFromSortIdx( infoActivity.m_nLocSort );

		if ( infoActivity.CompareCustomer( CurrentCustomer ) == FALSE )
		{
			if ( CurrentCustomer.m_nSortIdxType != -1 )
				WriteTotal( "", dTotalSpend, dTotalPayment );

			CurrentCustomer = infoActivity;

			strID.Format( "%8.8d", infoActivity.m_nCustomerID );

			CString strHeader = "";

			if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE )
			{
				if ( 0 == infoActivity.m_nSortIdxType )
				{
					CCustomerSelectionListAccountInfo infoSelect;
					infoSelect.m_nLocIdx = nLocIdx;
					infoSelect.m_nCustomerID = infoActivity.m_nCustomerID;
					DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( infoSelect );

					strHeader.Format( "<..>%s, Account Customer %8.8d, %s",
						dbLocation.GetName( nLocIdx ), 
						infoActivity.m_nCustomerID,
						infoSelect.GetReportName() );
				}
				else if ( 1 == infoActivity.m_nSortIdxType )
				{
					CCustomerSelectionListAccountInfo infoSelect;
					infoSelect.m_nLocIdx = nLocIdx;
					infoSelect.m_nCustomerID = infoActivity.m_nCustomerID;
					DataManagerNonDb.CustomerNameTable.OneOff.GetCustomerStatus( infoSelect );

					strHeader.Format( "<..>%s, One Off Customer %8.8d, %s",
						dbLocation.GetName( nLocIdx ), 
						infoActivity.m_nCustomerID,
						infoSelect.GetReportName() );
				}
			}
				
			if ( strHeader == "" )
			{
				strHeader.Format( "<..>%s, Unknown Customer %8.8d",
					dbLocation.GetName( nLocIdx ), 
					infoActivity.m_nCustomerID );
			}

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			dTotalSpend = 0.0;
			dTotalPayment = 0.0;
		}
	
		CString strActivity = "";
		double dAmount = infoActivity.m_dValue;
			
		if ( 1 == infoActivity.m_nAction )
		{
			dTotalSpend += dAmount;
			dGrandTotalSpend += dAmount;
			strActivity = "Post to Account";
		}
		else
		{
			dTotalPayment += dAmount;
			dGrandTotalPayment += dAmount;
			strActivity = "Payment (RA)";
		}

		CString strDate;
		strDate.Format( "%s/%s/%s",
			infoActivity.m_strDate.Right(2),
			infoActivity.m_strDate.Mid(4,2),
			infoActivity.m_strDate.Left(4) );

		CString strTime;
		strTime.Format( "%s:%s:%s",
			infoActivity.m_strTime.Left(2),
			infoActivity.m_strTime.Mid(2,2),
			infoActivity.m_strTime.Right(2) );

		CCSV csv( '\t' );
		csv.Add( strDate );
		csv.Add( strTime );
		csv.Add( strID );
		csv.Add( strActivity );
		csv.Add( dAmount, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportDataLine( csv.GetLine() );
	}

	StatusProgress.Unlock();

	if (CurrentCustomer.m_nSortIdxType != -1)
	{
		WriteTotal("", dTotalSpend, dTotalPayment);
	}

	m_ReportFile.RequestBlankLines(2);
	WriteTotal( "Grand Total", dGrandTotalSpend, dGrandTotalPayment );
				
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomerActivity::WriteTotal( const char* sz, double dTotalSpend, double dTotalPayment )
{
	CString str = "<..>";
	str += sz;

	if (str == "<..>")
	{
		m_ReportFile.WriteReportMiscLine("\t\t\t\t<LI>");
	}
	else
	{
		m_ReportFile.WriteReportMiscLine ( str );
		m_ReportFile.WriteReportMiscLine ( "<LI>" );
	}
		
	CCSV csv( '\t' );
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( "<..>Total Post to Account" );
	csv.Add( "" );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );

	csv.RemoveAll();
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( "<..>Total Payment (RA)" );
	csv.Add( "" );
	csv.Add( dTotalPayment, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( "\t\t\t\t<LI>" );

	csv.RemoveAll();
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( "<..>Period Balance" );
	csv.Add( "" );

	double dNet = dTotalSpend - dTotalPayment;

	switch ( CPriceHelpers::CompareDoubles( dNet, 0.0, 3 ) )
	{
	case -1:
		csv.Add( -dNet, SysInfo.GetDPValue() );
		csv.Add( "CR" );
		break;

	case 0:
		csv.Add( 0.0, SysInfo.GetDPValue() );
		break;

	case 1:
	default:
		csv.Add( dNet, SysInfo.GetDPValue() );
		break;
	}

	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
