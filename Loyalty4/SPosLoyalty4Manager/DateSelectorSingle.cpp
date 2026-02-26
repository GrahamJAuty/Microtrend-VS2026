/**********************************************************************/
#include "..\SPosLoyalty4Shared\DateRangeHelpers.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "StandardCombos.h"
/**********************************************************************/
#include "DateSelectorSingle.h"
/**********************************************************************/

CDateSelectorSingle::CDateSelectorSingle( CSSComboBox& combo, CDateTimeCtrl& date ) :
	m_comboDates( combo ), m_DatePicker( date )
{
	m_nCalendarMinYear = 1995;
	m_nCalendarMaxYear = 2050;
}

/**********************************************************************/

CDateSelectorSingle::~CDateSelectorSingle(void)
{
}

/**********************************************************************/

void CDateSelectorSingle::InitialiseDateControl()
{
	CTime start ( m_nCalendarMinYear, 1, 1, 0, 0, 0, -1 );
	CTime end ( m_nCalendarMaxYear, 1, 1, 0, 0, 0, -1 ); 
	m_DatePicker.SetRange ( &start, &end );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );
}

/**********************************************************************/

void CDateSelectorSingle::SetComboSelection( const char* szRange )
{
	CStandardCombos::SelectComboStringWithDefaultPos(m_comboDates, szRange, 0);
	UpdateDateControl();
}

/**********************************************************************/

void CDateSelectorSingle::UpdateDateControl()
{
	CString strComboText = szRANGE_CUSTOM_DATE;

	int nSelection = m_comboDates.GetCurSel();
	if (nSelection >= 0 && nSelection < m_comboDates.GetCount())
	{
		m_comboDates.GetLBText(nSelection, strComboText);
	}

	if ( strComboText != szRANGE_CUSTOM_DATE )
	{
		CTime timeFrom, timeTo;
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetDateRange ( strComboText, timeFrom, timeTo );
		m_DatePicker.SetTime ( &timeFrom );
	}

	m_DatePicker.GetTime( m_dateCurrent );
}

/**********************************************************************/

void CDateSelectorSingle::CheckForChangedDate()
{
	CTime date;
	m_DatePicker.GetTime( date );

	if ( date != m_dateCurrent )
	{
		m_dateCurrent = date;
		SetComboSelection( szRANGE_CUSTOM_DATE );
	}
}

/**********************************************************************/
