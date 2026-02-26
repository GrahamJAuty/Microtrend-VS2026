/**********************************************************************/
#include "DateRangeHelpers.h"
#include "PastAndFutureDateRange.h"
/**********************************************************************/
#include "DateSelectorRange.h"
/**********************************************************************/

CDateSelectorRange::CDateSelectorRange( CSSComboBox& combo, CDateTimeCtrl& from, CDateTimeCtrl& to ) :
	m_comboDates( combo ), m_DatePickerFrom( from ), m_DatePickerTo( to )
{
	m_nCalendarMinYear = 1995;
	m_nCalendarMaxYear = 2050;
	m_nMode = DATE_SELECTOR_MODE_STANDARD;
}

/**********************************************************************/

CDateSelectorRange::~CDateSelectorRange(void)
{
}

/**********************************************************************/

void CDateSelectorRange::InitialiseDateControls()
{
	CTime start ( m_nCalendarMinYear, 1, 1, 0, 0, 0, -1 );
	CTime end ( m_nCalendarMaxYear, 1, 1, 0, 0, 0, -1 ); 
	m_DatePickerFrom.SetRange ( &start, &end );
	m_DatePickerTo.SetRange ( &start, &end );
	m_DatePickerFrom.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerTo.SetFormat( "ddd dd MMM yyy" );
}

/**********************************************************************/

void CDateSelectorRange::AddReportRange( const char* sz )
{
	m_comboDates.AddString( sz );
}

/**********************************************************************/

void CDateSelectorRange::AddStandardReportRanges()
{
	AddStandardReportRanges( m_comboDates );
}

/**********************************************************************/

void CDateSelectorRange::AddCustomerCreationDateRanges()
{
	AddCustomerCreationDateRanges( m_comboDates );
}

/**********************************************************************/

void CDateSelectorRange::AddSingleDayRanges( CSSComboBox& comboDates )
{
	CStringArray arrayThisWeek;
	arrayThisWeek.Add ( szRANGE_SUN_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_MON_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_TUE_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_WED_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_THU_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_FRI_THIS_WEEK );
	arrayThisWeek.Add ( szRANGE_SAT_THIS_WEEK );
	
	CStringArray arrayLastWeek;
	arrayLastWeek.Add ( szRANGE_SUN_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_MON_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_TUE_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_WED_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_THU_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_FRI_LAST_WEEK );
	arrayLastWeek.Add ( szRANGE_SAT_LAST_WEEK );

	switch( EcrmanOptions.GetReportsSingleDayRanges() )
	{
	case 1:
	case 3:
		for ( int n = 6 ; n >= 0; n-- )
		{
			int nPos = ( ( EcrmanOptions.GetReportsWeekStartDay() + n ) % 7 );
			comboDates.AddString( arrayThisWeek.GetAt(nPos) );
		}
		break;
	}

	switch( EcrmanOptions.GetReportsSingleDayRanges() )
	{
	case 2:
	case 3:
		for ( int n = 6 ; n >= 0; n-- )
		{
			int nPos = ( ( EcrmanOptions.GetReportsWeekStartDay() + n ) % 7 );
			comboDates.AddString( arrayLastWeek.GetAt(nPos) );
		}
		break;
	}
}

/**********************************************************************/

void CDateSelectorRange::AddLastTaxQuarterRange( CSSComboBox& comboDates )
{
	if ( EcrmanOptions.GetReportsSalesTaxType() == 1 )
		comboDates.AddString ( szRANGE_LAST_GST_QUARTER );
	else
		comboDates.AddString ( szRANGE_LAST_VAT_QUARTER );
}

/**********************************************************************/

void CDateSelectorRange::AddThisTaxQuarterRange( CSSComboBox& comboDates  )
{
	if ( EcrmanOptions.GetReportsSalesTaxType() == 1 )
		comboDates.AddString ( szRANGE_THIS_GST_QUARTER );
	else
		comboDates.AddString ( szRANGE_THIS_VAT_QUARTER );
}

/**********************************************************************/

void CDateSelectorRange::AddPreviousDayRanges( CSSComboBox& comboDates )
{
	switch ( EcrmanOptions.GetReportsPreviousDayRanges() )
	{
	case 1:
	case 3:
		comboDates.AddString( szRANGE_PREVIOUS_7DAYS );
		comboDates.AddString( szRANGE_PREVIOUS_14DAYS );
		comboDates.AddString( szRANGE_PREVIOUS_21DAYS );
		comboDates.AddString( szRANGE_PREVIOUS_28DAYS );
		break;
	}

	switch ( EcrmanOptions.GetReportsPreviousDayRanges() )
	{
	case 2:
	case 3:
		comboDates.AddString( szRANGE_PREVIOUS_13WEEKS );
		break;
	}
}

/**********************************************************************/

void CDateSelectorRange::AddStandardReportRanges( CSSComboBox& comboDates )
{
	comboDates.AddString ( szRANGE_TODAY );
	comboDates.AddString ( szRANGE_YESTERDAY );
	AddSingleDayRanges( comboDates );
	AddPreviousDayRanges( comboDates );
	comboDates.AddString ( szRANGE_THIS_WEEK );
	comboDates.AddString ( szRANGE_THIS_MONTH );
	AddThisTaxQuarterRange( comboDates );
	comboDates.AddString ( szRANGE_THIS_YEAR );
	comboDates.AddString ( szRANGE_THIS_FINANCIAL_YEAR );
	comboDates.AddString ( szRANGE_LAST_WEEK );
	comboDates.AddString ( szRANGE_LAST_MONTH );
	AddLastTaxQuarterRange( comboDates );
	comboDates.AddString ( szRANGE_LAST_YEAR );
	comboDates.AddString ( szRANGE_LAST_FINANCIAL_YEAR );
	comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	comboDates.AddString ( szRANGE_ALL );
	comboDates.AddString ( szRANGE_CUSTOM_DATE );
	comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::AddCustomerCreationDateRanges( CSSComboBox& comboDates )
{
	comboDates.AddString ( szRANGE_TODAY );
	comboDates.AddString ( szRANGE_YESTERDAY );
	comboDates.AddString ( szRANGE_PREVIOUS_7DAYS );
	comboDates.AddString ( szRANGE_PREVIOUS_14DAYS );
	comboDates.AddString ( szRANGE_PREVIOUS_21DAYS );
	comboDates.AddString ( szRANGE_PREVIOUS_28DAYS );
	comboDates.AddString ( szRANGE_THIS_WEEK );
	comboDates.AddString ( szRANGE_THIS_MONTH );
	comboDates.AddString ( szRANGE_THIS_YEAR );
	comboDates.AddString ( szRANGE_LAST_WEEK );
	comboDates.AddString ( szRANGE_LAST_MONTH );
	comboDates.AddString ( szRANGE_LAST_YEAR );
	comboDates.AddString ( szRANGE_ALL );
	comboDates.AddString ( szRANGE_CUSTOM_DATE );
	comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::AddCustomerSyncPastReportRanges()
{
	m_comboDates.AddString ( szRANGE_TODAY );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_3_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_6_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_9_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDates.AddString ( szRANGE_ALL );
	m_comboDates.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::AddCustomerStatementRanges()
{
	m_comboDates.AddString ( szRANGE_TODAY );
	m_comboDates.AddString ( szRANGE_YESTERDAY );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_WEEK );
	m_comboDates.AddString ( szRANGE_LAST_MONTH );
	m_comboDates.AddString ( szRANGE_LAST_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDates.AddString ( szRANGE_ALL );
	m_comboDates.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::AddFutureReportRanges()
{
	m_comboDates.AddString ( szRANGE_TODAY );
	m_comboDates.AddString ( szRANGE_TOMORROW );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_NEXT_WEEK );
	m_comboDates.AddString ( szRANGE_NEXT_MONTH );
	m_comboDates.AddString ( szRANGE_NEXT_YEAR );
	m_comboDates.AddString ( szRANGE_NEXT_3_MONTHS );
	m_comboDates.AddString ( szRANGE_NEXT_6_MONTHS );
	m_comboDates.AddString ( szRANGE_NEXT_9_MONTHS );
	m_comboDates.AddString ( szRANGE_NEXT_12_MONTHS );
	m_comboDates.AddString ( szRANGE_ALL );
	m_comboDates.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::AddPastReportRanges()
{
	m_comboDates.AddString ( szRANGE_TODAY );
	m_comboDates.AddString ( szRANGE_YESTERDAY );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_WEEK );
	m_comboDates.AddString ( szRANGE_LAST_MONTH );
	m_comboDates.AddString ( szRANGE_LAST_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_3_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_6_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_9_MONTHS );
	m_comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDates.AddString ( szRANGE_ALL );
	m_comboDates.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDates.AddString ( szRANGE_CUSTOM_RANGE );
}

/**********************************************************************/

void CDateSelectorRange::SetComboSelection( int nIdx, const char* szRange )
{
	m_comboDates.SelectString( nIdx, szRange );
	UpdateDateControls();
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControls()
{
	switch( m_nMode )
	{
	case DATE_SELECTOR_MODE_STANDARD:
		UpdateDateControlsStandard();
		break;

	case DATE_SELECTOR_MODE_CUSTOMER_SYNC:
		UpdateDateControlsCustomerSync();
		break;

	case DATE_SELECTOR_MODE_PAST:
		UpdateDateControlsPastFuture( FALSE );
		break;

	case DATE_SELECTOR_MODE_FUTURE:
		UpdateDateControlsPastFuture( TRUE );
		break;
	}
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControlsStandard()
{
	CString strComboText = szRANGE_CUSTOM_DATE;

	int nSelection = m_comboDates.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
		m_comboDates.GetLBText ( nSelection, strComboText );

	if ( strComboText == szRANGE_CUSTOM_RANGE )
	{
		m_DatePickerTo.EnableWindow ( TRUE );
		m_DatePickerFrom.EnableWindow ( TRUE );
	}
	else if ( strComboText == szRANGE_CUSTOM_DATE )
	{
		m_DatePickerFrom.EnableWindow( TRUE );
		m_DatePickerTo.EnableWindow( FALSE );

		CTime timeFrom;
		m_DatePickerFrom.GetTime( timeFrom );
		m_DatePickerTo.SetTime( &timeFrom );
	}
	else
	{
		CTime timeFrom, timeTo;
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetDateRange ( strComboText, timeFrom, timeTo );
		m_DatePickerFrom.SetTime ( &timeFrom );
		m_DatePickerTo.SetTime ( &timeTo );
		m_DatePickerFrom.EnableWindow ( TRUE );
		m_DatePickerTo.EnableWindow ( TRUE );
	}

	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControlsCustomerSync()
{
	CString strComboText = szRANGE_CUSTOM_DATE;

	int nSelection = m_comboDates.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
		m_comboDates.GetLBText ( nSelection, strComboText );

	if ( strComboText == szRANGE_CUSTOM_RANGE )
	{
		m_DatePickerTo.EnableWindow ( TRUE );
		m_DatePickerFrom.EnableWindow ( TRUE );
	}
	else if ( strComboText == szRANGE_CUSTOM_DATE )
	{
		m_DatePickerFrom.EnableWindow( TRUE );
		m_DatePickerTo.EnableWindow( FALSE );

		CTime timeFrom;
		m_DatePickerFrom.GetTime( timeFrom );
		m_DatePickerTo.SetTime( &timeFrom );
	}
	else
	{
		CTime timeFrom, timeTo;
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetCustomerSyncDateRange ( strComboText, timeFrom, timeTo );
		m_DatePickerFrom.SetTime ( &timeFrom );
		m_DatePickerTo.SetTime ( &timeTo );
		m_DatePickerFrom.EnableWindow ( TRUE );
		m_DatePickerTo.EnableWindow ( TRUE );
	}

	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControlsPastFuture( bool bFromToday )
{
	CString strComboText = szRANGE_CUSTOM_DATE;

	int nSelection = m_comboDates.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
		m_comboDates.GetLBText ( nSelection, strComboText );

	if ( strComboText == szRANGE_CUSTOM_RANGE )
	{
		m_DatePickerTo.EnableWindow ( TRUE );
		m_DatePickerFrom.EnableWindow ( TRUE );
	}
	else if ( strComboText == szRANGE_CUSTOM_DATE )
	{
		m_DatePickerFrom.EnableWindow( TRUE );
		m_DatePickerTo.EnableWindow( FALSE );

		CTime timeFrom;
		m_DatePickerFrom.GetTime( timeFrom );
		m_DatePickerTo.SetTime( &timeFrom );
	}
	else
	{
		CTime timeFrom, timeTo;
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetPastAndFutureDateRange ( strComboText, timeFrom, timeTo, bFromToday );
		m_DatePickerFrom.SetTime ( &timeFrom );
		m_DatePickerTo.SetTime ( &timeTo );
		m_DatePickerFrom.EnableWindow ( TRUE );
		m_DatePickerTo.EnableWindow ( TRUE );
	}

	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControlsByMonth( int nYear, int nMonth )
{
	CTime dateStart = CTime( nYear, nMonth, 1, 0, 0, 0, -1 );
	
	nMonth++;
	if ( nMonth >= 13 )
	{
		nYear++;
		nMonth = 1;
	}

	CTime dateEnd = CTime( nYear, nMonth, 1, 0, 0, 0, -1 );
	dateEnd -= CTimeSpan( 1, 0, 0, 0 );

	m_DatePickerFrom.SetTime ( &dateStart );
	m_DatePickerTo.SetTime ( &dateEnd );
	m_DatePickerFrom.EnableWindow ( FALSE );
	m_DatePickerTo.EnableWindow ( FALSE );

	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/

void CDateSelectorRange::OnChangeFromDate()
{
	CString strComboText = szRANGE_CUSTOM_DATE;

	int nSelection = m_comboDates.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
		m_comboDates.GetLBText ( nSelection, strComboText );

	if ( strComboText == szRANGE_CUSTOM_DATE )
	{
		m_DatePickerFrom.EnableWindow( TRUE );
		m_DatePickerTo.EnableWindow( FALSE );

		CTime timeFrom;
		m_DatePickerFrom.GetTime( timeFrom );
		m_DatePickerTo.SetTime( &timeFrom );
	}
}

/**********************************************************************/

void CDateSelectorRange::CheckForChangedDate()
{
	CTime dateStart, dateEnd;
	m_DatePickerFrom.GetTime( dateStart );
	m_DatePickerTo.GetTime( dateEnd );

	if ( ( dateStart != m_dateCurrentStart ) || ( dateEnd != m_dateCurrentEnd ) )
	{
		m_dateCurrentStart = dateStart;
		m_dateCurrentEnd = dateEnd;

		CString strComboText = "";

		int nSelection = m_comboDates.GetCurSel();
		if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
			m_comboDates.GetLBText ( nSelection, strComboText );

		if ( ( strComboText != szRANGE_CUSTOM_DATE ) && ( strComboText != szRANGE_CUSTOM_RANGE ) )
			SetComboSelection( 0, szRANGE_CUSTOM_RANGE );
	}
}

/**********************************************************************/

void CDateSelectorRange::SetCurrentDatesFromControls()
{
	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/

