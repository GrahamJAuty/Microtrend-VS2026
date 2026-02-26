/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "FontTable.h"
/**********************************************************************/
#include "PMSCalendarHelpers.h"
/**********************************************************************/

CPMSCalendarHelpers::CPMSCalendarHelpers()
{
	m_nCalendarStartCol = DataManagerNonDb.PMSBooking.GetCurrentDayNumber();
	m_nCalendarStartRoom = 0;
	m_nSelectedBookingId = 0;
	m_nSelectedClosureId = 0;

	CRect rect( 0, 0, 400, 300 );
	SetMetrics( rect );
}

/**********************************************************************/

void CPMSCalendarHelpers::SetMetrics( CRect& rect )
{	
	m_nHScrollBarX = 20;
	m_nHScrollBarY = rect.bottom - 50;
	m_nHScrollBarCX = rect.right - 75;
	m_nHScrollBarCY = 30;
	
	m_nVScrollBarX = rect.right - 50;
	m_nVScrollBarY = 80;
	m_nVScrollBarCX = 30;
	m_nVScrollBarCY = rect.bottom - 140;

	m_nDateX = 20;
	m_nDateY = 20;
	m_nDateCX = rect.right - 75;
	m_nDateCY = 50;

	m_nCalendarX = 20;
	m_nCalendarY = 80;
	m_nCalendarCX = rect.right - 75;
	m_nCalendarCY = rect.bottom - 140;
	
	/*
	m_nHScrollBarX = 50;
	m_nHScrollBarY = rect.bottom - 80;
	m_nHScrollBarCX = rect.right - 135;
	m_nHScrollBarCY = 30;
	
	m_nVScrollBarX = rect.right - 80;
	m_nVScrollBarY = 90;
	m_nVScrollBarCX = 30;
	m_nVScrollBarCY = rect.bottom - 180;

	m_nDateX = 50;
	m_nDateY = 30;
	m_nDateCX = rect.right - 135;
	m_nDateCY = 50;

	m_nCalendarX = 50;
	m_nCalendarY = 90;
	m_nCalendarCX = rect.right - 135;
	m_nCalendarCY = rect.bottom - 180;
	*/
	
	m_nDisplayRows = ( ( m_nCalendarCY - CALENDAR_BAR_TOP ) / CALENDAR_ROW_HEIGHT ) + 1;
	m_nDisplayCols = ( ( m_nCalendarCX - CALENDAR_BAR_LEFT ) / CALENDAR_DAY_WIDTH );
		
	if ( m_nDisplayCols > 35 )
		m_nDisplayCols = 35;

	CFilenameUpdater FnUp( SysFiles::PMSRooms );
	DataManagerNonDb.PMSRoom.Read( FnUp.GetFilenameToUse() );

	int nRooms = DataManagerNonDb.PMSRoom.GetSize();

	if ( nRooms > m_nDisplayRows - 1 )
	{
		m_nVScrollBarMin = 0;
		m_nVScrollBarMax = nRooms - ( m_nDisplayRows - 1 );
	}
	else
	{
		m_nVScrollBarMin = 0;
		m_nVScrollBarMax = 0;
	}
}

/**********************************************************************/

int CPMSCalendarHelpers::GetCalendarClickDay( int nOffset )
{
	int nDayToday = CPMSBookingCSVArray::GetCurrentDayNumber();
	int nColSel = m_nCalendarStartCol + nOffset;
	return ( nColSel < nDayToday ) ? nColSel : nColSel - 1;
}

/**********************************************************************/

bool CPMSCalendarHelpers::CheckReports()
{
	CString strOldReportPath = "PMS2\\Reports";
	GetDataProgramPath( strOldReportPath );

	CFileFind fileFinder;

	BOOL bWorking = fileFinder.FindFile ( strOldReportPath + "\\*.*" );

	bool bGotOldFiles = FALSE;
	
	while ( bWorking == TRUE )
	{
		bWorking = fileFinder.FindNextFile();

		if ( fileFinder.IsDots() == TRUE )
			continue;

		if ( fileFinder.IsDirectory() == TRUE )
			continue;

		bGotOldFiles = TRUE;
		break;
	}

	if ( TRUE == bGotOldFiles )
	{
		CString str = "";
		str += "Guest Account (PMS) reports cannot be created until the\n";
		str += "EPOS report files have been updated.\n\n";
		str += "These files will be updated automatically when you next\n";
		str += "open the Guest Account Calendar.";
		Prompter.Error( str );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
