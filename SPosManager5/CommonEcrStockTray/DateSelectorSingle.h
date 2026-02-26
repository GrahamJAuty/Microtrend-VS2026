#pragma once
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CDateSelectorSingle
{
public:
	CDateSelectorSingle( CSSComboBox& combo, CDateTimeCtrl& date );
	~CDateSelectorSingle(void);

public:
	void SetCalendarMinYear( int n ){ m_nCalendarMinYear = n; }
	void SetCalendarMaxYear( int n ){ m_nCalendarMaxYear = n; }

public:
	void InitialiseDateControl();

public:
	void AddReportRange( const char* sz );
	void AddStandardReportRanges( bool bAll );
	
public:
	void SetComboSelection( int nIdx, const char* szRange );
	void UpdateDateControl();
	void CheckForChangedDate();
	
private:
	CSSComboBox&	m_comboDates;
	CDateTimeCtrl&	m_DatePicker;
	
private:
	CTime m_dateCurrent;
	int m_nCalendarMinYear;
	int m_nCalendarMaxYear;
};

/**********************************************************************/
