#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define TIMEPLANENTRIES_VERSION 1
/**********************************************************************/
	
namespace TimePlanEntries
{
const recInt		PlanNo				= { "PlanNo", 1, 999 };	
const recInt		EntryNo				= { "EntryNo", 1, 999 };
const recBool		Active				= { "Active" };
const recString		Name				= { "Name", 0, 40 };
const recBool		Sunday				= { "Sunday" };
const recBool		Monday				= { "Monday" };
const recBool		Tuesday				= { "Tuesday" };
const recBool		Wednesday			= { "Wednesday" };
const recBool		Thursday			= { "Thursday" };
const recBool		Friday				= { "Friday" };
const recBool		Saturday			= { "Saturday" };
const recInt		StartHour			= { "StartHour", 0, 23 };
const recInt		StartMinute			= { "StartMinute", 0, 59 };
const recInt		EndHour				= { "EndHour", 0, 23 };
const recInt		EndMinute			= { "EndMinute", 0, 59 };
const recInt		EndPeriod			= { "EndPeriod", 0, 9999 };
};

/**********************************************************************/

class CTimePlanEntriesCSVRecord : public CCSVRecord
{
public:
	CTimePlanEntriesCSVRecord(); 
	virtual ~CTimePlanEntriesCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetPlanNo() { return m_nPlanNo; }
	int GetEntryNo() { return m_nEntryNo; }
	bool GetActiveFlag(){ return m_bActive; }
	int GetType(){ return m_nType; }
	int GetSelection(){ return m_nSelection; }
	const char* GetName(){ return m_strName; }
	bool GetSundayFlag(){ return m_bSunday; }
	bool GetMondayFlag(){ return m_bMonday; }
	bool GetTuesdayFlag(){ return m_bTuesday; }
	bool GetWednesdayFlag(){ return m_bWednesday; }
	bool GetThursdayFlag(){ return m_bThursday; }
	bool GetFridayFlag(){ return m_bFriday; }
	bool GetSaturdayFlag(){ return m_bSaturday; }
	int GetStartHour(){ return m_nStartHour; }
	int GetStartMinute(){ return m_nStartMinute; }
	int GetEndHour(){ return m_nEndHour; }
	int GetEndMinute(){ return m_nEndMinute; }
	int GetEndPeriod(){ return m_nEndPeriod; }
	COleDateTime GetStartDate(){ return m_oleStartDate; }
	COleDateTime GetEndDate(){ return m_oleEndDate; }
	
	void SetPlanNo( int n );
	void SetEntryNo( int n );
	void SetActiveFlag( bool b ){ m_bActive = b; }
	void SetTypeAndSelection( int t, int s );
	void SetName( const char* sz );
	void SetSundayFlag( bool b ){ m_bSunday = b; }
	void SetMondayFlag( bool b ){ m_bMonday = b; }
	void SetTuesdayFlag( bool b ){ m_bTuesday = b; }
	void SetWednesdayFlag( bool b ){ m_bWednesday = b; }
	void SetThursdayFlag( bool b ){ m_bThursday = b; }
	void SetFridayFlag( bool b ){ m_bFriday = b; }
	void SetSaturdayFlag( bool b ){ m_bSaturday = b; }
	void SetStartHour( int n );
	void SetStartMinute( int n );
	void SetEndHour( int n );
	void SetEndMinute( int n );
	void SetEndPeriod( int n );
	void SetStartDate( double d );
	void SetStartDate( const char* sz );
	void SetStartDate( COleDateTime& date );
	void SetEndDate( double d );
	void SetEndDate( const char* sz );
	void SetEndDate( COleDateTime& date );
	
public:
	const char* GetLookupKey();
	void MakeDummy( int n );

public:
	const char* GetDefaultText( bool bCombo);

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nPlanNo;
	int m_nEntryNo;
	bool m_bActive;
	int m_nType;
	int m_nSelection;
	CString m_strName;
	bool m_bSunday;
	bool m_bMonday;
	bool m_bTuesday;
	bool m_bWednesday;
	bool m_bThursday;
	bool m_bFriday;
	bool m_bSaturday;
	int m_nStartHour;
	int m_nStartMinute;
	int m_nEndHour;
	int m_nEndMinute;
	int m_nEndPeriod;
	COleDateTime m_oleStartDate;
	COleDateTime m_oleEndDate;

private:
	CString m_strKey;
	CString m_strDefaultText;
};

/**********************************************************************/

class CTimePlanEntriesCSVArray : public CSharedCSVArray
{
public:
	CTimePlanEntriesCSVArray(bool bTemplateMode);
	~CTimePlanEntriesCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	bool FindKey( CTimePlanEntriesCSVRecord& record, int& nIndex );
	bool InsertRecord( CTimePlanEntriesCSVRecord& record, int& nIndex );
	int FindFirstFreeNumber( int nPlanNo );

public:
	void Validate();
	void PurgePlan( int nPlanNo );
	void GetPlanRange( int nPlanNo, int& nIdx1, int& nIdx2 );
	void GetTimeplanMap(CString& strTimeplanList, CReportConsolidationArray<CSortedIntByInt>& TimeplanMap);

private:
	bool m_bTemplateMode = FALSE;
};

/**********************************************************************/
