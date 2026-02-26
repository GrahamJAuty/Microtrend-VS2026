/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "HistoryFileTypes.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportAccountBooking.h"
/**********************************************************************/

CEposReportAccountBookingPurchase::CEposReportAccountBookingPurchase( CEposSelectArray& SelectArray ) : CEposReportAccountBooking( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE;
	m_strUnderline = "\t\t\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 7;
}

/**********************************************************************/

CEposReportAccountBookingPurchase::CEposReportAccountBookingPurchase( CEposSelectArray& SelectArray, const char* szCustomParams ) : CEposReportAccountBooking( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE;
	m_strUnderline = "\t\t\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 7;

	CString strParams = szCustomParams;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

void CEposReportAccountBookingPurchase::GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
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
