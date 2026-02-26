/**********************************************************************/
#include "..\SPosLoyalty4Shared\DateRangeHelpers.h"
/**********************************************************************/
#include "StandardCombos.h"
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

void CDateSelectorRange::SetComboSelection( const char* szRange )
{
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboDates, szRange, 0);
	UpdateDateControls();
}

/**********************************************************************/

void CDateSelectorRange::UpdateDateControls()
{
	switch( m_nMode )
	{
	case DATE_SELECTOR_MODE_EXPIRY:
		UpdateDateControlsExpiry(TRUE);
		break;
	
	case DATE_SELECTOR_MODE_STANDARD:
	default:
		UpdateDateControlsStandard();
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

void CDateSelectorRange::UpdateDateControlsExpiry( bool bFromToday )
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
		DateRangeHelpers.GetExpiryDateRange ( strComboText, timeFrom, timeTo, bFromToday );
		m_DatePickerFrom.SetTime ( &timeFrom );
		m_DatePickerTo.SetTime ( &timeTo );
		m_DatePickerFrom.EnableWindow ( TRUE );
		m_DatePickerTo.EnableWindow ( TRUE );
	}

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
		if (nSelection >= 0 && nSelection < m_comboDates.GetCount())
		{
			m_comboDates.GetLBText(nSelection, strComboText);
		}

		if ((strComboText != szRANGE_CUSTOM_DATE) && (strComboText != szRANGE_CUSTOM_RANGE))
		{
			SetComboSelection(szRANGE_CUSTOM_RANGE);
		}
	}
}

/**********************************************************************/

void CDateSelectorRange::SetCurrentDatesFromControls()
{
	m_DatePickerFrom.GetTime( m_dateCurrentStart );
	m_DatePickerTo.GetTime( m_dateCurrentEnd );
}

/**********************************************************************/
