#pragma once
/**********************************************************************/
#define DATE_SELECTOR_MODE_STANDARD 0
#define DATE_SELECTOR_MODE_CUSTOMER_SYNC 3
#define DATE_SELECTOR_MODE_FUTURE 1
#define DATE_SELECTOR_MODE_PAST 2
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
	void AddReportRange( const char* sz );
	void AddStandardReportRanges();
	void AddCustomerSyncPastReportRanges();
	void AddCustomerStatementRanges();
	void AddCustomerCreationDateRanges();
	void AddFutureReportRanges();
	void AddPastReportRanges();

public:
	static void AddStandardReportRanges( CSSComboBox& comboDates );
	static void AddCustomerCreationDateRanges( CSSComboBox& comboDates );
	static void AddSingleDayRanges( CSSComboBox& comboDates );
	static void AddPreviousDayRanges( CSSComboBox& comboDates );
	static void AddThisTaxQuarterRange( CSSComboBox& comboDates );
	static void AddLastTaxQuarterRange( CSSComboBox& comboDates );

public:
	void SetComboSelection( int nIdx, const char* szRange );
	void UpdateDateControls();
	void OnChangeFromDate();
	void CheckForChangedDate();
	void SetCurrentDatesFromControls();
	void UpdateDateControlsByMonth( int nYear, int nMonth );

private:
	void UpdateDateControlsStandard();
	void UpdateDateControlsCustomerSync();
	void UpdateDateControlsPastFuture( bool bFromToday );

	
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
