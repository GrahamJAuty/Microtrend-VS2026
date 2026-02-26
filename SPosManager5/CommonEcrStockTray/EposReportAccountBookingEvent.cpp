/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "HistoryFileTypes.h"
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportAccountBookingEvent.h"
/**********************************************************************/

CEposReportAccountBookingEvent::CEposReportAccountBookingEvent( CEposSelectArray& SelectArray ) : CEposReportAccountBooking( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_BOOKING_EVENT;
	m_strUnderline = "\t\t\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 7;
}

/**********************************************************************/

CEposReportAccountBookingEvent::CEposReportAccountBookingEvent( CEposSelectArray& SelectArray, const char* szCustomParams ) : CEposReportAccountBooking( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_BOOKING_EVENT;
	m_strUnderline = "\t\t\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 7;

	CString strParams = szCustomParams;
	m_ReportSettings.SetSettingsCSVLine( strParams );
}

/**********************************************************************/

void CEposReportAccountBookingEvent::GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	m_SelectArray.TagOriginalSelectionStatus( TRUE );
	DataManagerNonDb.SessionDateTimeFilter.SetDepositDateRangeEvent();
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );
	DataManagerNonDb.SessionDateTimeFilter.SetDepositDateRangeNormal();
}

/**********************************************************************/

void CEposReportAccountBookingEvent::ValidatePurchaseTime( int nLocIdx, CString& strPurchaseDate, CString& strPurchaseTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction, bool& bValidPurchaseTime, bool& bValidEventTime )
{
	bValidPurchaseTime = TRUE; 
	bValidEventTime = FALSE;
}

/**********************************************************************/

