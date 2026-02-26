#pragma once
/******************************************************************************/
#define GROUPFIELD_GROUPNO 0
#define GROUPFIELD_GROUPNAME 1
#define GROUPFIELD_SCHEMENO 2
/******************************************************************************/
#define GROUPFIELD_REFRESH_TYPE 3
#define GROUPFIELD_REFRESH_FLAGS 4
#define GROUPFIELD_REFRESH_VALUE 5
#define GROUPFIELD_REFRESH_APPEND 6
/******************************************************************************/
#define GROUPFIELD_PURSE1_ALLOW 7
#define GROUPFIELD_PURSE2_ALLOW 8
/******************************************************************************/
#define GROUPFIELD_NOPLUBONUS 9
/******************************************************************************/
#define GROUPFIELD_MAXSPEND_TYPE 10
#define GROUPFIELD_MAXSPEND_PERIOD1 11
#define GROUPFIELD_MAXSPEND_PERIOD2 12
#define GROUPFIELD_MAXSPEND_PERIOD3 13
#define GROUPFIELD_MAXSPEND_PERIOD4 14
#define GROUPFIELD_MAXSPEND_PERIOD5 15
#define GROUPFIELD_MAXSPEND_PERIOD6 16
/******************************************************************************/
#define GROUPFIELD_MAXOVERDRAFT_PERIOD1 17
#define GROUPFIELD_MAXOVERDRAFT_PERIOD2 18
#define GROUPFIELD_MAXOVERDRAFT_PERIOD3 19
#define GROUPFIELD_MAXOVERDRAFT_PERIOD4 20
#define GROUPFIELD_MAXOVERDRAFT_PERIOD5 21
#define GROUPFIELD_MAXOVERDRAFT_PERIOD6 22
/******************************************************************************/
#define GROUPFIELD_REVALUE_FLAG 23
#define GROUPFIELD_REVALUE_DISABLE 24
#define GROUPFIELD_REVALUE_TYPE 25
#define GROUPFIELD_REVALUE_TRIP1 26
#define GROUPFIELD_REVALUE_TRIP2 27
#define GROUPFIELD_REVALUE_POINTS 28
#define GROUPFIELD_REVALUE_BONUS 29
#define GROUPFIELD_REVALUE_PERCENT 30
/******************************************************************************/
#define GROUPFIELD_REWARD_FLAG 31
#define GROUPFIELD_REWARD_DISABLE 32
#define GROUPFIELD_REWARD_TYPE 33
#define GROUPFIELD_REWARD_TRIP 34
#define GROUPFIELD_REWARD_POINTS 35
#define GROUPFIELD_REWARD_BONUS 36
#define GROUPFIELD_REWARD_LIMIT 37
#define GROUPFIELD_REWARD_TRIGGER 38
/******************************************************************************/
#define GROUPFIELD_REDEEM_FLAG 39
#define GROUPFIELD_REDEEM_DISABLE 40
#define GROUPFIELD_REDEEM_TYPE 41
#define GROUPFIELD_REDEEM_TRIP 42
#define GROUPFIELD_REDEEM_VALUE 43
/******************************************************************************/
#define GROUPFIELD_NODEPTBONUS 44
/******************************************************************************/

class CGroupHeaderHelpers
{
public:
	CGroupHeaderHelpers();
	~CGroupHeaderHelpers();
	
public:
	CString GetHeaderFull() { return m_strHeaderFull; }
	
public:
	void SetHeaderFull(CString str) { m_strHeaderFull = str;  }
	
public:
	void CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader);
	void CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader);
	int GetNumericField(CString strField);
	void UpdateLegacyField(CString& strField);

private:
	void AddFieldToHeader(CString strHeader);
	void AddFieldToHeaderMap(CString strField, int nFieldNo);
	
private:
	CMap <CString, LPCTSTR, int, int> m_HeaderMap;

private:
	CString m_strHeaderFull;
};

/******************************************************************************/
extern CGroupHeaderHelpers GroupHeaderHelpers;
/******************************************************************************/
