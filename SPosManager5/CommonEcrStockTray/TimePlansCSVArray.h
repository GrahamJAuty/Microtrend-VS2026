#pragma once
/**********************************************************************/
#define TIMEPLANS_VERSION 1
/**********************************************************************/
#define TIMEPLAN_SORT_NUMBER 0
#define TIMEPLAN_SORT_NAME_UP 1
#define TIMEPLAN_SORT_NAME_DOWN 2
/**********************************************************************/
#define TIMEPLAN_MAX_PLANS 999
/**********************************************************************/

namespace TimePlans
{
const recInt PlanNo			= { "PlanNo", 1, TIMEPLAN_MAX_PLANS };	
const recString PlanName	= { "PlanName", 0, 40 };
};

/**********************************************************************/

class CTimePlansCSVRecord : public CCSVRecord
{
public:
	CTimePlansCSVRecord();
	virtual ~CTimePlansCSVRecord() {}
	virtual void ConvertToCSV(CCSV& csv);
	virtual void ConvertFromCSV(CCSV& csv);

public:
	int GetPlanNo() { return m_nPlanNo; }
	const char* GetPlanName() { return m_strPlanName; }
	const char* GetDisplayName();

	void SetPlanNo(int n);
	void SetPlanName(const char* sz);

private:
	void ClearRecord();
	void V1ConvertFromCSV(CCSV& csv);

private:
	int m_nPlanNo;
	CString m_strPlanName;

private:
	CString m_strDisplayName;
	CString m_strDefaultText;
};

/**********************************************************************/

class CTimePlansCSVArray : public CSharedCSVArray
{
public:
	CTimePlansCSVArray();
	~CTimePlansCSVArray();

	int Open( const char* szFilename, int nMode );

public:
	bool FindPlanByNumber( int nPlanNo, int& nPlanIdx );
	int FindFirstFreeNumber();
	
public:
	void SetSortMode( int nMode );
	void RefreshSort(){ SetSortMode( m_nSortMode ); }
	int GetSortMode(){ return m_nSortMode; }
	int GetSortArraySize();
	int GetArrayIdxFromSortIdx( int n );
	int GetSortIdxFromArrayIdx( int n );

private:
	int m_nSortMode = 0;
	CArray<int,int> m_arraySort;
};

/**********************************************************************/
