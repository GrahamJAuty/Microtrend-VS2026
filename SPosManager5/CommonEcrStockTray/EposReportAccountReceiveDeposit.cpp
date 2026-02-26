/**********************************************************************/
#include "HistoryFileTypes.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportAccountReceiveDeposit.h"
/**********************************************************************/

CEposReportAccountReceiveDeposit::CEposReportAccountReceiveDeposit( CEposSelectArray& SelectArray ) : CEposReportAccountReceive( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_PURCHASED;
	m_strUnderline = "\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 5;
}

/**********************************************************************/

CEposReportAccountReceiveDeposit::CEposReportAccountReceiveDeposit( CEposSelectArray& SelectArray, const char* szCustomParams ) : CEposReportAccountReceive( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_PURCHASED;
	m_strUnderline = "\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 5;

	CString strParams = szCustomParams;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

void CEposReportAccountReceiveDeposit::GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	m_SelectArray.TagOriginalSelectionStatus( TRUE );

	if ( m_ReportSettings.GetEndDateCutOffFlag() == FALSE )
	{
		DataManagerNonDb.SessionDateTimeFilter.SetDepositDateRangeRedeem();
		CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
		FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );
		DataManagerNonDb.SessionDateTimeFilter.SetDepositDateRangeNormal();
	}
	else
	{
		CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
		FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );
	}
}

/**********************************************************************/

void CEposReportAccountReceiveDeposit::AddColumns()
{
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Deposit ID", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Purchaser Name", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Payment", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CEposReportAccountReceiveDeposit::CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv )
{
	bool bRecycled = FALSE;

	CString strID;
	strID.Format( "%8.8d", infoLine.m_nAccountID );

	CDepositNameCSVRecord DepositRecord;
	DataManagerNonDb.DepositNames.GetDepositByID( infoLine.m_nAccountID, DepositRecord );

	if ((infoLine.m_strPurchaseDate == "99999999") && (DEPOSIT_ADJUST_ID == infoLine.m_nAccountID))
	{
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("Recycled Deposits");
		bRecycled = TRUE;
	}		
	else
	{
		csv.Add(infoLine.GetDisplayPurchaseDate());
		csv.Add(infoLine.GetDisplayPurchaseTime());
		csv.Add(strID);
		csv.Add(DepositRecord.GetBuyerName());
	}

	csv.Add(GetPaymentLabel(infoLine.m_nPaymentType, ""));
	csv.Add(infoLine.m_dVal, 2);

	return bRecycled;
}

/**********************************************************************/

