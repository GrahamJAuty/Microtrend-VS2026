/**********************************************************************/
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportAccountReceiveCustomer.h"
/**********************************************************************/

CEposReportAccountReceiveCustomer::CEposReportAccountReceiveCustomer( CEposSelectArray& SelectArray ) : CEposReportAccountReceive( SelectArray )
{
	m_nReportType = REPORT_TYPE_CUSTOMER_RA;

	if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE )
	{
		m_strUnderline = "\t\t\t\t\t\t<LI>";
		m_nColumnsBeforeTotal = 6;
	}
	else
	{
		m_strUnderline = "\t\t\t\t<LI>";
		m_nColumnsBeforeTotal = 4;
	}
}

/**********************************************************************/

void CEposReportAccountReceiveCustomer::GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	m_SelectArray.TagOriginalSelectionStatus( FALSE );
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );
}

/**********************************************************************/

void CEposReportAccountReceiveCustomer::AddColumns()
{
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Customer ID", TA_LEFT, 250 );
	
	if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE )
	{
		m_ReportFile.AddColumn ( "Customer Type", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Customer Name", TA_LEFT, 500 );
	}

	m_ReportFile.AddColumn ( "Payment", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CEposReportAccountReceiveCustomer::CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv )
{
	CString strID;
	strID.Format( "%8.8d", infoLine.m_nAccountID );

	CDepositNameCSVRecord DepositRecord;
	DataManagerNonDb.DepositNames.GetDepositByID( infoLine.m_nAccountID, DepositRecord );

	csv.Add( infoLine.GetDisplayPurchaseDate() );
	csv.Add( infoLine.GetDisplayPurchaseTime() );
	csv.Add( strID );

	if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE )
	{
		CCustomerSelectionListAccountInfo infoSelect;
		infoSelect.m_nLocIdx = nLocIdx;
		infoSelect.m_nCustomerID = infoLine.m_nAccountID;

		int nNodeType, nDummy;
		DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoSelect, nNodeType, nDummy );

		switch( nNodeType )
		{
		case NODE_CUSTOMER_ACCOUNT:	
			csv.Add( "Account" );
			break;

		case NODE_CUSTOMER_ONEOFF:
			csv.Add( "One-off" );
			break;

		case NODE_PLACEHOLDER:
		default:				
			csv.Add( "Unknown" );	
			break;
		}

		csv.Add( infoSelect.GetReportName() );
	}

	csv.Add( GetPaymentLabel( infoLine.m_nPaymentType, "" ) );
	csv.Add( infoLine.m_dVal, 2 );	

	return FALSE;
}

/**********************************************************************/



