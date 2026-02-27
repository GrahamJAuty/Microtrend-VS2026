#pragma once
/**********************************************************************/
#define DATE_SELECTOR_MODE_STANDARD 0
#define DATE_SELECTOR_MODE_EXPIRY 1
/**********************************************************************/

class CDateSelectorRange
{
public:
	CDateSelectorRange( CSSComboBox& combo, CDateTimeCtrl& from, CDateTimeCtrl& to );
	~CDateSelectorRange(void);

public:
	void SetCalendarMinYear( int n ){ m_nCalendarMinYear = n; }
	void SetCalendarMaxYear( int n ){ m_nCalendarMaxYear = n; }
	void SetMode( int n ){ m_nMode = n; }

public:
	void InitialiseDateControls();
	
public:
	void SetComboSelection( const char* szRange );
	void UpdateDateControls();
	void UpdateDateControlsExpiry( bool bFromToday );
	void OnChangeFromDate();
	void CheckForChangedDate();
	void SetCurrentDatesFromControls();
	
private:
	void UpdateDateControlsStandard();
	
private:
	CSSComboBox&	m_comboDates;
	CDateTimeCtrl&	m_DatePickerTo;
	CDateTimeCtrl&	m_DatePickerFrom;

private:
	CTime m_dateCurrentStart;
	CTime m_dateCurrentEnd;
	int m_nCalendarMinYear;
	int m_nCalendarMaxYear;
	int m_nMode;
};

/**********************************************************************/
