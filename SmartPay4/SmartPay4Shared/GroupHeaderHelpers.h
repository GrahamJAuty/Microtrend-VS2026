#pragma once
/******************************************************************************/
#define GROUPFIELD_GROUPNO 0
#define GROUPFIELD_GROUPNAME 1
#define GROUPFIELD_USERTYPE 2
#define GROUPFIELD_SETNO 3
#define GROUPFIELD_PRICELEVEL 4
#define GROUPFIELD_VATSHIFT 5
/******************************************************************************/
#define GROUPFIELD_REFRESH_TYPE 6
#define GROUPFIELD_REFRESH_FLAGS 7
#define GROUPFIELD_REFRESH_VALUE 8
#define GROUPFIELD_REFRESH_APPEND 9
#define GROUPFIELD_REFRESH_TYPE_PERIOD1 10
#define GROUPFIELD_REFRESH_TYPE_PERIOD2 11
#define GROUPFIELD_REFRESH_TYPE_PERIOD3 12
#define GROUPFIELD_REFRESH_TYPE_PERIOD4 13
#define GROUPFIELD_REFRESH_TYPE_PERIOD5 14
#define GROUPFIELD_REFRESH_TYPE_PERIOD6 15
#define GROUPFIELD_REFRESH_VALUE_PERIOD1 16
#define GROUPFIELD_REFRESH_VALUE_PERIOD2 17
#define GROUPFIELD_REFRESH_VALUE_PERIOD3 18
#define GROUPFIELD_REFRESH_VALUE_PERIOD4 19
#define GROUPFIELD_REFRESH_VALUE_PERIOD5 20
#define GROUPFIELD_REFRESH_VALUE_PERIOD6 21
/******************************************************************************/
#define GROUPFIELD_PURSE1_ALLOW 22
#define GROUPFIELD_PURSE2_ALLOW 23
#define GROUPFIELD_PURSE3_ALLOW 24
/******************************************************************************/
#define GROUPFIELD_MAXSPEND_TYPE 25
/******************************************************************************/
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD1 26
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD2 27
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD3 28
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD4 29
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD5 30
#define GROUPFIELD_MAXSPEND_PURSE1_PERIOD6 31
/******************************************************************************/
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD1 32
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD2 33
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD3 34
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD4 35
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD5 36
#define GROUPFIELD_MAXSPEND_PURSE3_PERIOD6 37
/******************************************************************************/
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD1 38
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD2 39
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD3 40
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD4 41
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD5 42
#define GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD6 43
/******************************************************************************/
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1 44
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD2 45
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD3 46
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD4 47
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD5 48
#define GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD6 49
/******************************************************************************/
#define GROUPFIELD_DEPTINHIBIT_PURSE1 50
#define GROUPFIELD_DEPTINHIBIT_PURSE2 51
#define GROUPFIELD_DEPTINHIBIT_PURSE3 52
#define GROUPFIELD_DEPTINHIBIT_CASH 53
/******************************************************************************/
#define GROUPFIELD_LOWBALANCE_PURSE1 54
#define GROUPFIELD_LOWBALANCE_PURSE2 55
#define GROUPFIELD_LOWBALANCE_PURSE3 56
/******************************************************************************/
#define GROUPFIELD_POINTS_ACHIEVEMENT 57
/******************************************************************************/
#define GROUPFIELD_REVALUE_FLAG 58
#define GROUPFIELD_REVALUE_DISABLE 59
#define GROUPFIELD_REVALUE_TYPE 60
#define GROUPFIELD_REVALUE_TRIP1 61
#define GROUPFIELD_REVALUE_TRIP2 62
#define GROUPFIELD_REVALUE_POINTS 63
#define GROUPFIELD_REVALUE_BONUS 64
/******************************************************************************/
#define GROUPFIELD_REWARD_FLAG 65
#define GROUPFIELD_REWARD_DISABLE 66
#define GROUPFIELD_REWARD_TYPE 67
#define GROUPFIELD_REWARD_TRIP 68
#define GROUPFIELD_REWARD_POINTS 69
#define GROUPFIELD_REWARD_BONUS 70
#define GROUPFIELD_REWARD_LIMIT 71
#define GROUPFIELD_REWARD_TRIGGER 72
/******************************************************************************/
#define GROUPFIELD_REDEEM_FLAG 73
#define GROUPFIELD_REDEEM_DISABLE 74
#define GROUPFIELD_REDEEM_TRIP 75
#define GROUPFIELD_REDEEM_VALUE 76
#define GROUPFIELD_REDEEM_PURSE 77
/******************************************************************************/

class CGroupHeaderHelpers
{
public:
	CGroupHeaderHelpers();
	
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
