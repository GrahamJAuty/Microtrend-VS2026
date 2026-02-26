#pragma once
/******************************************************************************/
#define LOYALTYFIELD_USERID 0
#define LOYALTYFIELD_FULLNAME 1
#define LOYALTYFIELD_FORENAME 2
#define LOYALTYFIELD_SURNAME 3
#define LOYALTYFIELD_DOB 4
#define LOYALTYFIELD_EXPIRYDATE 5
#define LOYALTYFIELD_GENDER 6
#define LOYALTYFIELD_GROUPNO 7
#define LOYALTYFIELD_INFO1 8
#define LOYALTYFIELD_INFO2 9
#define LOYALTYFIELD_INFO3 10
#define LOYALTYFIELD_INFO4 11
#define LOYALTYFIELD_ADDRESS1 12
#define LOYALTYFIELD_ADDRESS2 13
#define LOYALTYFIELD_ADDRESS3 14
#define LOYALTYFIELD_ADDRESS4 15
#define LOYALTYFIELD_ADDRESS5 16
#define LOYALTYFIELD_PHONE1 17
#define LOYALTYFIELD_PHONE2 18
#define LOYALTYFIELD_INACTIVE 19
#define LOYALTYFIELD_EXTERNALREF 20
#define LOYALTYFIELD_PREVUSERID 21
#define LOYALTYFIELD_NEXTUSERID 22
#define LOYALTYFIELD_PURSE1_BALANCE 23
#define LOYALTYFIELD_PURSE1_LASTSPENDDATE 24
#define LOYALTYFIELD_PURSE1_LASTSPENDTIME 25
#define LOYALTYFIELD_PURSE1_LASTSPEND 26
#define LOYALTYFIELD_PURSE1_SPENDTODATE 27
#define LOYALTYFIELD_PURSE2_BALANCE 28
#define LOYALTYFIELD_PURSE2_LASTSPENDDATE 29
#define LOYALTYFIELD_PURSE2_LASTSPENDTIME 30
#define LOYALTYFIELD_PURSE2_LASTSPEND 31
#define LOYALTYFIELD_PURSE2_SPENDTODATE 32
#define LOYALTYFIELD_PURSE2_REFRESHEDDATE 33
#define LOYALTYFIELD_PURSE2_REFRESHEDTIME 34
#define LOYALTYFIELD_CASH_LASTSPENDDATE 35
#define LOYALTYFIELD_CASH_LASTSPENDTIME 36
#define LOYALTYFIELD_CASH_LASTSPEND 37
#define LOYALTYFIELD_CASH_SPENDTODATE 38
#define LOYALTYFIELD_POINTS 39
#define LOYALTYFIELD_POINTSTODATE 40
#define LOYALTYFIELD_HOTLISTCODE 41
#define LOYALTYFIELD_HOTLISTDATE 42
#define LOYALTYFIELD_HOTLISTTIME 43
#define LOYALTYFIELD_MAXSPEND 44
#define LOYALTYFIELD_MAXOVERDRAFT 45
#define LOYALTYFIELD_ALERTCODES 46
/******************************************************************************/
#define LOYALTYFIELD_BLANK 47
#define LOYALTYFIELD_TOTAL_SPENDTODATE 48
#define LOYALTYFIELD_DOB_NEXTAGE 49
#define LOYALTYFIELD_DOB_DAY 50
#define LOYALTYFIELD_DOB_CHECK 51
#define LOYALTYFIELD_PURSE1_TOPUP 52
#define LOYALTYFIELD_DELDATE 53
#define LOYALTYFIELD_DELTIME 54
#define LOYALTYFIELD_TEXT 55
#define LOYALTYFIELD_NAME_BALANCE 56
#define LOYALTYFIELD_BALANCE 57
#define LOYALTYFIELD_BONUS_CHECK 58
#define LOYALTYFIELD_SKIP_CHECK 59
#define LOYALTYFIELD_ALERT_CHECK 60
#define LOYALTYFIELD_FULLCARDNO 61
#define LOYALTYFIELD_GROUPCARDNO 62
#define LOYALTYFIELD_GROUPFULLCARDNO 63
#define LOYALTYFIELD_AVAILABLESPEND 64
/******************************************************************************/
#define LOYALTYFIELD_QRINFO 65
#define LOYALTYFIELD_STAMPOFFER 66
/******************************************************************************/
#define LOYALTYFIELD_PURSE2_TOPUP 67
/******************************************************************************/
#define LOYALTYFIELD_SCHEME 68
/******************************************************************************/

class CLoyaltyHeaderHelpers
{
public:
	CLoyaltyHeaderHelpers();
	~CLoyaltyHeaderHelpers();
	
public:
	CString GetHeaderFull() { return m_strHeaderFull; }
	CString GetHeaderNoPurse2() { return m_strHeaderNoPurse2; }

public:
	void SetHeaderFull(CString str) { m_strHeaderFull = str;  }
	void SetHeaderNoPurse2(CString str) { m_strHeaderNoPurse2 = str; }

public:
	void CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader);
	void CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader);
	void CreateNumericHeaderMemberMojo(CString strHeader, CArray<int, int>& arrayHeader);
	void CreateNumericHeaderMemberMojo(CCSV& csvHeader, CArray<int, int>& arrayHeader);
	int GetNumericField(CString strField);
	void UpdateLegacyField(CString& strField);

private:
	void AddFieldToHeader(CString strHeader, bool bIsPurse2);
	void AddFieldToHeaderMap(CString strField, int nFieldNo);
	void AddFieldToHeaderMapMemberMojo(CString strField, int nFieldNo);
	
private:
	CMap <CString, LPCTSTR, int, int> m_HeaderMap;
	CMap <CString, LPCTSTR, int, int> m_HeaderMapMemberMojo;

private:
	CString m_strHeaderFull;
	CString m_strHeaderNoPurse2;
};

/******************************************************************************/
extern CLoyaltyHeaderHelpers LoyaltyHeaderHelpers;
/******************************************************************************/
