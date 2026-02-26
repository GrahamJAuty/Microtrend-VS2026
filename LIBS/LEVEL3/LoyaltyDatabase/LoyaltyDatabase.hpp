#pragma once

//**********************************************************************

extern const char* szVERSION_LOYALTYDB;

//**********************************************************************
#define LOYALTYFIELD_CARDNO 0
#define LOYALTYFIELD_GROUPNO 1
#define LOYALTYFIELD_FULLNAME 2
#define LOYALTYFIELD_FORENAME 3
#define LOYALTYFIELD_SURNAME 4
#define LOYALTYFIELD_DOB 5
#define LOYALTYFIELD_GENDER 6
#define LOYALTYFIELD_CTS 7
#define LOYALTYFIELD_POINTS 8
#define LOYALTYFIELD_POINTSTD 9
#define LOYALTYFIELD_PURSE1 10
#define LOYALTYFIELD_PURSE1_LSDATE 11
#define LOYALTYFIELD_PURSE1_LSTIME 12
#define LOYALTYFIELD_PURSE1_LS 13
#define LOYALTYFIELD_PURSE1_STD 14
#define LOYALTYFIELD_PURSE2 15
#define LOYALTYFIELD_PURSE2_LSDATE 16
#define LOYALTYFIELD_PURSE2_LSTIME 17
#define LOYALTYFIELD_PURSE2_LS 18
#define LOYALTYFIELD_PURSE2_STD 19
#define LOYALTYFIELD_PURSE2_RFDATE 20
#define LOYALTYFIELD_PURSE2_RFTIME 21
#define LOYALTYFIELD_CASH_LSDATE 22
#define LOYALTYFIELD_CASH_LSTIME 23
#define LOYALTYFIELD_CASH_LS 24
#define LOYALTYFIELD_CASH_STD 25
#define LOYALTYFIELD_MAXSPEND 26
#define LOYALTYFIELD_OVERSPEND 27
#define LOYALTYFIELD_EXPIRYDATE 28
#define LOYALTYFIELD_ALERT 29
#define LOYALTYFIELD_INACTIVE 30
#define LOYALTYFIELD_HOTCODE 31
#define LOYALTYFIELD_HOTDATE 32
#define LOYALTYFIELD_HOTTIME 33
#define LOYALTYFIELD_PREVCARD 34
#define LOYALTYFIELD_NEXTCARD 35
#define LOYALTYFIELD_ADDRESS1 36
#define LOYALTYFIELD_ADDRESS2 37
#define LOYALTYFIELD_ADDRESS3 38
#define LOYALTYFIELD_ADDRESS4 39
#define LOYALTYFIELD_ADDRESS5 40
#define LOYALTYFIELD_TEL1 41
#define LOYALTYFIELD_TEL2 42
#define LOYALTYFIELD_EXTREF 43
#define LOYALTYFIELD_INFO1 44
#define LOYALTYFIELD_INFO2 45
#define LOYALTYFIELD_INFO3 46
#define LOYALTYFIELD_INFO4 47
//**********************************************************************
#define LOYALTYFIELD_BLANK 48
#define LOYALTYFIELD_TOTAL_STD 49
#define LOYALTYFIELD_DOB_NEXTAGE 50
#define LOYALTYFIELD_DOB_DAY 51
#define LOYALTYFIELD_DOB_CHECK 52
#define LOYALTYFIELD_PURSE1_TOPUP 53
#define LOYALTYFIELD_DELDATE 54
#define LOYALTYFIELD_DELTIME 55
#define LOYALTYFIELD_TEXT 56
#define LOYALTYFIELD_NAME_BALANCE 57
#define LOYALTYFIELD_BALANCE 58
#define LOYALTYFIELD_BONUS_CHECK 59
#define LOYALTYFIELD_SKIP_CHECK 60
#define LOYALTYFIELD_ALERT_CHECK 61
#define LOYALTYFIELD_FULLCARDNO 62
#define LOYALTYFIELD_GROUPCARDNO 63
#define LOYALTYFIELD_GROUPFULLCARDNO 64
//**********************************************************************

namespace Loyalty
{
	const recString		ExCardNo = { "CardNo", 1, 12 };				// Biometric length = 6, MiFare.v1 length = 10
	const recInt		GroupNo = { "GroupNo", 1 , 99 };
	const recString		FullName = { "FullName", 0, 40 };
	const recString		Forename = { "Forename", 0, 40 };
	const recString		Surname = { "Surname", 0, 40 };
	const recString		Dob = { "Dob", 0, 10 };
	const recString		Gender = { "Gender", 0, 1 };
	const recInt		Points = { "Points",-99999999, 99999999 };
	const recInt		PointsTD = { "PointsTD",-99999999, 99999999 };
	const recDouble		Purse1 = { "Purse1", -99999999.99, 99999999.99 };
	const recString		Purse1LastSpendDate = { "Purse1LastSpendDate", 0, 8 };
	const recString		Purse1LastSpendTime = { "Purse1LastSpendTime", 0, 8 };
	const recDouble		Purse1LastSpend = { "Purse1LastSpend", -99999999.99, 99999999.99 };
	const recDouble		Purse1SpendTD = { "Purse1SpendTD", -99999999.99, 99999999.99 };
	const recDouble		Purse2 = { "Purse2", -99999999.99, 99999999.99 };
	const recString		Purse2LastSpendDate = { "Purse2LastSpendDate", 0, 8 };
	const recString		Purse2LastSpendTime = { "Purse2LastSpendTime", 0, 8 };
	const recDouble		Purse2LastSpend = { "Purse2LastSpend", -99999999.99, 99999999.99 };
	const recDouble		Purse2SpendTD = { "Purse2SpendTD", -99999999.99, 99999999.99 };
	const recString		Purse2RefreshedDate = { "Purse2RefreshedDate", 0, 8 };
	const recString		Purse2RefreshedTime = { "Purse2RefreshedTime", 0, 8 };
	const recString		CashLastSpendDate = { "CashLastSpendDate", 0, 8 };
	const recString		CashLastSpendTime = { "CashLastSpendTime", 0, 8 };
	const recDouble		CashLastSpend = { "CashLastSpend", -99999999.99, 99999999.99 };
	const recDouble		CashSpendTD = { "CashSpendTD", -99999999.99, 99999999.99 };
	const recDouble		MaxSpend = { "MaxSpend", 0.00, 99999999.99 };
	const recDouble		OverSpend = { "OverSpend", 0.00, 99999999.99 };
	const recString		ExpiryDate = { "ExpiryDate", 0, 10 };
	const recString		AlertCodes = { "Alert",0, 17 };
	const recBool		Inactive = { "Inactive" };
	const recInt		HotlistCode = { "HotlistCode",0, 9 };
	const recString		HotlistDate = { "HotlistDate", 0, 10 };
	const recString		HotlistTime = { "HotlistTime", 0, 10 };
	const recString		ExPreviousCardNo = { "PreviousNo", 0, 12 };		// use System.GetUserIDLength() to get max length
	const recString		ExNextCardNo = { "NextNo", 0, 12 };				
	const recString		Add1 = { "Address1", 0, 40 };
	const recString		Add2 = { "Address2", 0, 40 };
	const recString		Add3 = { "Address3", 0, 40 };
	const recString		Add4 = { "Address4", 0, 40 };
	const recString		Add5 = { "Address5", 0, 40 };
	const recString		Tel1 = { "Telephone1", 0, 40 };
	const recString		Tel2 = { "Telephone2", 0, 40 };
	const recString		ExtRef = { "ExtRef", 1, 20 };			// hold internal MiFare number
	const recString		Info1 = { "Info1", 0, 80 };				// v2.01 was 40
	const recString		Info2 = { "Info2", 0, 80 };				// v2.01 was 40
	const recString		Info3 = { "Info3", 0, 80 };				// v2.01 was 40
	const recString		Info4 = { "Info4", 0, 80 };				// v2.01 was 40

	//none database fields
	const recString		Blank = { "<blank>", 0, 0 };
	const recString		Text = { "<text>", 0, 40 };
	const recString		DobCheck = { "<Anniversary>", 0, 40 };
	const recString		AlertCheck = { "<Alert>", 0, 40 };
	const recString		BonusCheck = { "<Bonus>", 0, 40 };
	const recString		SkipCheck = { "<Skip>", 0, 40 };

	// below only used for label
	const recDouble		TotalSpendTD = { "TotalSpendTD", 0, 99999999.99 };
	const recString		FullCardNo = { "FullCardNo", 0, 0 };
	const recString		GroupCardNo = { "GroupNo:CardNo", 0, 0 };
	const recString		GroupFullCardNo = { "GroupNo:FullCardNo", 0, 0 };
	const recString		DobNextAge = { "DobNextAge", 0, 0 }; 	// Age next birthday
	const recString		DobDay = { "DobDay", 0, 0 };		// Next birthday Day of week 
	const recString		DeletedDate = { "DeletedDate", 0, 0 };	// delete header fields
	const recString		DeletedTime = { "DeletedTime", 0, 0 };
	const recString		AlertText = { "AlertText", 0, 0 };
	const recString		CardNameWithBalance = { "FullName+Balance", 0, 0 };
	const recString		Balance = { "Balance", 0, 0 };
}

//*********************************************************************

struct CLoyaltyLegacyField
{
public:
	CString m_strLegacyField;
	CString m_strNewField;
};

//*********************************************************************

class CLoyaltyHeaderHelpers
{
public:
	CLoyaltyHeaderHelpers();
	
public:
	CString GetHeaderFull() { return m_strHeaderFull; }
	CString GetHeaderNoPurse2() { return m_strHeaderNoPurse2; }
	CString GetLegacyHeaders() { return m_strLegacyHeaders; }

public:
	void SetHeaderFull(CString str) { m_strHeaderFull = str;  }
	void SetHeaderNoPurse2(CString str) { m_strHeaderNoPurse2 = str; }
	void SetLegacyHeaders(CString str) { m_strLegacyHeaders = str; }

public:
	void UpdateLegacyField(CString& strField);
	void CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader);
	void CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader);
	int GetNumericField(CString strField);

private:
	void AddFieldToHeader(CString strHeader, bool bIsPurse2);
	void AddFieldToHeaderMap(CString strField, int nFieldNo);
	void AddLegacyField(CString strLegacyField, CString strNewField, CString& strCSV);

private:
	CArray<CLoyaltyLegacyField, CLoyaltyLegacyField> m_arrayLegacyFields;
	CMap <CString, LPCTSTR, int, int> m_HeaderMap;

private:
	CString m_strHeaderFull;
	CString m_strHeaderNoPurse2;
	CString m_strLegacyHeaders;
};

//*********************************************************************

class CLoyaltyDatabase : public CSSStringDatabase
{
public:
	CLoyaltyDatabase(CLoyaltyHeaderHelpers* pHeader);

public:
	void InitHeaders(CLoyaltyHeaderHelpers& header);
	virtual const char* GetHeader() { return m_strHeader; }
	
public:
	CString GetCardNo() { return (CString)CSSDatabase::GetString(Loyalty::ExCardNo.Label); }
	CString GetGroupNoString();
	int GetGroupNo();
	CString GetFullName() { return (CString)CSSDatabase::GetString(Loyalty::FullName.Label); }
	CString GetForename() { return (CString)CSSDatabase::GetString(Loyalty::Forename.Label); }
	CString GetSurname() { return (CString)CSSDatabase::GetString(Loyalty::Surname.Label); }
	CString GetDobDate() { return (CString)CSSDatabase::GetString(Loyalty::Dob.Label); }
	CString GetGender() { return (CString)CSSDatabase::GetString(Loyalty::Gender.Label); }
	int GetPoints() { return (int)CSSDatabase::GetInt(Loyalty::Points.Label); }
	CString GetPointsString();
	int GetPointsTD() { return (int)CSSDatabase::GetInt(Loyalty::PointsTD.Label); }
	CString GetPointsTDString();
	double GetPurse1Balance() { return (double)CSSDatabase::GetDouble(Loyalty::Purse1.Label); }
	CString GetPurse1BalanceString();
	CString GetPurse1LastSpendDate() { return(CString)CSSDatabase::GetString(Loyalty::Purse1LastSpendDate.Label); }
	CString GetPurse1LastSpendTime() { return (CString)CSSDatabase::GetString(Loyalty::Purse1LastSpendTime.Label); }
	double GetPurse1LastSpend() { return (double)CSSDatabase::GetDouble(Loyalty::Purse1LastSpend.Label); }
	double GetPurse1SpendTD() { return (double)CSSDatabase::GetDouble(Loyalty::Purse1SpendTD.Label); }
	double GetPurse2Balance() { return (double)CSSDatabase::GetDouble(Loyalty::Purse2.Label); }
	CString GetPurse2BalanceString();
	CString GetPurse2LastSpendDate() { return(CString)CSSDatabase::GetString(Loyalty::Purse2LastSpendDate.Label); }
	CString GetPurse2LastSpendTime() { return (CString)CSSDatabase::GetString(Loyalty::Purse2LastSpendTime.Label); }
	double GetPurse2LastSpend() { return (double)CSSDatabase::GetDouble(Loyalty::Purse2LastSpend.Label); }
	double GetPurse2SpendTD() { return (double)CSSDatabase::GetDouble(Loyalty::Purse2SpendTD.Label); }
	CString GetPurse2RefreshedDate() { return (CString)CSSDatabase::GetString(Loyalty::Purse2RefreshedDate.Label); }
	CString GetPurse2RefreshedTime() { return (CString)CSSDatabase::GetString(Loyalty::Purse2RefreshedTime.Label); }
	CString GetCashLastSpendDate() { return(CString)CSSDatabase::GetString(Loyalty::CashLastSpendDate.Label); }
	CString GetCashLastSpendTime() { return (CString)CSSDatabase::GetString(Loyalty::CashLastSpendTime.Label); }
	double GetCashLastSpend() { return (double)CSSDatabase::GetDouble(Loyalty::CashLastSpend.Label); }
	double GetCashSpendTD() { return (double)CSSDatabase::GetDouble(Loyalty::CashSpendTD.Label); }
	double GetMaxSpend() { return (double)CSSDatabase::GetDouble(Loyalty::MaxSpend.Label); }
	double GetOverSpend() { return (double)CSSDatabase::GetDouble(Loyalty::OverSpend.Label); }
	CString GetExpiryDate() { return (CString)CSSDatabase::GetString(Loyalty::ExpiryDate.Label); }
	CString GetAlertCodes() { return (CString)CSSDatabase::GetString(Loyalty::AlertCodes.Label); }
	bool GetInactive() { return CSSDatabase::GetBool(Loyalty::Inactive.Label); }
	int GetHotlistCode() { return (int)CSSDatabase::GetInt(Loyalty::HotlistCode.Label); }
	CString GetHotlistDate() { return (CString)CSSDatabase::GetString(Loyalty::HotlistDate.Label); }
	CString GetHotlistTime() { return (CString)CSSDatabase::GetString(Loyalty::HotlistTime.Label); }
	CString GetPreviousCardNo() { return (CString)CSSDatabase::GetString(Loyalty::ExPreviousCardNo.Label); }
	CString GetNextCardNo() { return (CString)CSSDatabase::GetString(Loyalty::ExNextCardNo.Label); }
	CString GetAdd1() { return (CString)CSSDatabase::GetString(Loyalty::Add1.Label); }
	CString GetAdd2() { return (CString)CSSDatabase::GetString(Loyalty::Add2.Label); }
	CString GetAdd3() { return (CString)CSSDatabase::GetString(Loyalty::Add3.Label); }
	CString GetAdd4() { return (CString)CSSDatabase::GetString(Loyalty::Add4.Label); }
	CString GetAdd5() { return (CString)CSSDatabase::GetString(Loyalty::Add5.Label); }
	CString GetTel1() { return (CString)CSSDatabase::GetString(Loyalty::Tel1.Label); }
	CString GetTel2() { return (CString)CSSDatabase::GetString(Loyalty::Tel2.Label); }
	CString GetExtRef() { return (CString)CSSDatabase::GetString(Loyalty::ExtRef.Label); }
	CString GetInfo1() { return (CString)CSSDatabase::GetString(Loyalty::Info1.Label); }
	CString GetInfo2() { return (CString)CSSDatabase::GetString(Loyalty::Info2.Label); }
	CString GetInfo3() { return (CString)CSSDatabase::GetString(Loyalty::Info3.Label); }
	CString GetInfo4() { return (CString)CSSDatabase::GetString(Loyalty::Info4.Label); }

public:
	void SetGroupNo(int nGroupNo);
	void SetFullName(const char* szText);
	void SetForename(const char* szText);
	void SetSurname(const char* szText);
	void SetDob(const char* szDate);
	void SetGender(const char* szGender);
	void SetGender(int nGender);
	void SetPoints(int nPoints);
	void AddToPoints(int nPoints) { SetPoints(GetPoints() + nPoints); }
	void SubtractPoints(int nPoints) { SetPoints(GetPoints() - nPoints); }
	void SetPointsTD(int nPoints);
	void AddToPointsTD(int nPoints) { SetPointsTD(GetPointsTD() + nPoints); }
	void SetPurse1Balance(double dValue);
	void AddToPurse1(double dValue) { SetPurse1Balance(GetPurse1Balance() + dValue); }
	void SubtractFromPurse1(double dValue) { SetPurse1Balance(GetPurse1Balance() - dValue); }
	void SetPurse1LastSpendDate(const char* szDate);
	void SetPurse1LastSpendTime(const char* szTime);
	void SetPurse1LastSpend(double dValue);
	void AddToPurse1LastSpend(double dValue) { SetPurse1LastSpend(GetPurse1LastSpend() + dValue); }
	void SetPurse1SpendTD(double dValue);
	void AddToPurse1SpendTD(double dValue) { SetPurse1SpendTD(GetPurse1SpendTD() + dValue); }
	void SetPurse2Balance(double dValue);
	void AddToPurse2(double dValue) { SetPurse2Balance(GetPurse2Balance() + dValue); }
	void SubtractFromPurse2(double dValue) { SetPurse2Balance(GetPurse2Balance() - dValue); }
	void SetPurse2LastSpendDate(const char* szDate);
	void SetPurse2LastSpendTime(const char* szTime);
	void SetPurse2LastSpend(double dValue);
	void AddToPurse2LastSpend(double dValue) { SetPurse2LastSpend(GetPurse2LastSpend() + dValue); }
	void SetPurse2SpendTD(double dValue);
	void AddToPurse2SpendTD(double dValue) { SetPurse2SpendTD(GetPurse2SpendTD() + dValue); }
	void SetPurse2RefreshedDate(const char* szDate);
	void SetPurse2RefreshedTime(const char* szTime);
	void SetCashLastSpendDate(const char* szDate);
	void SetCashLastSpendTime(const char* szTime);
	void SetCashLastSpend(double dValue);
	void AddToCashLastSpend(double dValue) { SetCashLastSpend(GetCashLastSpend() + dValue); }
	void SetCashSpendTD(double dValue);
	void AddToCashSpendTD(double dValue) { SetCashSpendTD(GetCashSpendTD() + dValue); }
	void SetMaxSpend(double dValue);
	void SetOverSpend(double dValue);
	void SetExpiry(const char* szDate);
	void SetAlertCodes(const char* szCodes);
	void SetInactive(bool bFlag) { Set(Loyalty::Inactive.Label, bFlag); }
	void SetHotlistCode(int nCode);
	void SetHotlistDate(const char* szDate);
	void SetHotlistTime(const char* szTime);
	void SetPreviousCardNo(const char* szCardNo);
	void SetNextCardNo(const char* szCardNo);
	void SetAdd1(const char* szText);
	void SetAdd2(const char* szText);
	void SetAdd3(const char* szText);
	void SetAdd4(const char* szText);
	void SetAdd5(const char* szText);
	void SetTel1(const char* szText);
	void SetTel2(const char* szText);
	void SetExtRef(const char* szExtRef);
	void SetInfo1(const char* szText);
	void SetInfo2(const char* szText);
	void SetInfo3(const char* szText);
	void SetInfo4(const char* szText);

public:
	bool IsValidCardNo(const char* szCardNo, CString& strCardNo, int nMaxLength);
	CString IncrementCardNo(__int64 nMaxNo);
	bool HaveNextCardNo() { return (GetNextCardNo() == "") ? FALSE : TRUE; }
	bool HavePreviousCardNo() { return (GetPreviousCardNo() == "") ? FALSE : TRUE; }
	bool HasCardExpired();

public:
	CString GetGenderText();
	bool IsGenderMale() { return (GetGender() == "M") ? TRUE : FALSE; }
	bool IsGenderFemale() { return (GetGender() == "F") ? TRUE : FALSE; }
	bool IsGenderUnknown() { return (GetGender() == "") ? TRUE : FALSE; }

public:
	bool IsBirthdayToday();
	const char* GetBirthdayMonth();
	int GetAge(bool bNextBDay = FALSE);
	CString GetAgeNextBirthday();
	const char* GetDOBDayofWeek();

public:
	void DivideCardName(const char* szCardName, CString& strFirstName, CString& strSecondName);
	void RefreshPurse2(double dValue, const char* szDate = "", const char* szTime = "");

public:
	virtual const char* FormatKey(const char* szKey);
	void CopyToDatabase(CLoyaltyDatabase* pDbCopy);

public:
	int OpenReadOnlyForSharing(const char* szFilename);
	int OpenReadWriteForSharing(const char* szFilename, const char* szBackupFolder);
	int ReOpenReadOnlyForSharing();
	int ReOpenReadWriteForSharing(const char* szBackupFolder);
	bool Close();

	int GetPercentPosition();
	int GetRecordStatusColour();
	bool IsRecordEmpty();
	bool IsCardUsed();

	bool IsAvailable(const char* szFilename);
	bool GetLastUsed(CSSDate& date, CSSTime& time);
	void UpdatePoints(int nPoints);
	
	double GetTotalSpendTD() { return (GetPurse1SpendTD() + GetPurse2SpendTD() + GetCashSpendTD()); }
	CString GetTotalSpendTDString();
	
	bool IsCardHotlisted() { return (GetHotlistCode() == 0) ? FALSE : TRUE; }
	void SetHotlistCard(int nHotlistCode);

	bool GetBoolData(const char* szData);

	bool CopyRecordToFile(const char* szFilename);
	bool CopyFileToRecord(const char* szFilename);

private:
	CString MakeInUseFilename(const char* szFullPath);

	bool IsStringEmpty(const char* szText);
	void SetField(const char* strLabel, const char* szText, int nMax);
	void SetField(const char* szLabel, int nValue, int nMin, int nMax);
	void SetField(const char* szLabel, __int64 nValue, __int64 nMin, __int64 nMax);
	void SetField(const char* szLabel, double dValue, double dMin, double dMax);
	void SetDateField(const char* szLabel, const char* szDate);
	void SetTimeField(const char* szLabel, const char* szTime);
	void SetCardNoField(const char* szLabel, const char* szCardNo);

private:
	int OpenForSharing(const char* szFilename, const char* szBackupFolder, int nMode);
	int ReOpenForSharing(const char* szBackupFolder, int nMode);

private:
	CString m_strDatabaseFilename;
	CString m_strInUseFilename;
	CString m_strKey;
	CString m_strHeader;
	CSSFile m_fileInUse;
};

//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************

extern const char* szVERSION_LOYALTYGROUPDATABASE;

//**********************************************************************

#define nREFRESHTYPE_NORMAL		0				// No refresh
#define nREFRESHTYPE_DAILY		1				// Daily Refresh
#define nREFRESHTYPE_WEEKLY		2	
#define nREFRESHTYPE_MONTHLY	3
#define nREFRESHTYPE_YEARLY		4

#define nSPENDTYPE_DAILY		0				// Single Daily Spend
#define nSPENDTYPE_RECORD		1				// Record Specific
#define nSPENDTYPE_PERIOD		2				// Use Audit Periods

//**********************************************************************

namespace LoyaltyGroup
{
	const recInt	GroupNo = { "GroupNo", 1, 99 };
	const recString	GroupName = { "Name", 0, 16 };
	const recInt	SchemeNo = { "SchemeNo", 0, 99 };
	const recBool	Complimentary = { "Compment" };
	const recInt	RefreshType = { "RFType", 0, 5 };
	const recInt	RefreshAllowed = { "RFAllow", 0, 1231 };		// if daily max = 127, if monthly max = 31. if yearly max = 1231 (100x12)+ 31
	const recDouble	RefreshValue = { "RFVal", 0, 999.99 };
	const recBool	PointsDisabled = { "NoPts" };
	const recInt	Purse1Allowed = { "P1Allow", 0, 63 };		// Purse1 allowed Period1 - Period6
	const recInt	Purse2Allowed = { "P2Allow", 0, 63 };		// Purse2 allowed Period1 - Period6

	const recInt	MaxSpendType = { "MSType", 0, 2 };			// Single Daily link / Record specific / Audit Period specific
	const recDouble	MaxSpendValue = { "MSV1", 0, 999.99 };	// Max daily spend or Max daily spend Period1
	const recDouble	MaxSpendPeriod2 = { "MSV2", 0, 999.99 };	// Max daily spend Period2
	const recDouble	MaxSpendPeriod3 = { "MSV3", 0, 999.99 };	// Max daily spend Period3
	const recDouble	MaxSpendPeriod4 = { "MSV4", 0, 999.99 };	// Max daily spend Period4
	const recDouble	MaxSpendPeriod5 = { "MSV5", 0, 999.99 };	// Max daily spend Period5
	const recDouble	MaxSpendPeriod6 = { "MSV6", 0, 999.99 };	// Max daily spend Period6
	//const recDouble	OverSpend			= { "OSV1",			0, 999.99 };	// OverSpend
	const recDouble	OverSpend = { "OverSpend",	0, 999999.99 };
	const recDouble	OverSpendPeriod2 = { "OSV2", 0, 999.99 };
	const recDouble	OverSpendPeriod3 = { "OSV3", 0, 999.99 };
	const recDouble	OverSpendPeriod4 = { "OSV4", 0, 999.99 };
	const recDouble	OverSpendPeriod5 = { "OSV5", 0, 999.99 };
	const recDouble	OverSpendPeriod6 = { "OSV6", 0, 999.99 };

	const recBool	RevalueFlag = { "RFlag" };
	const recBool	RevalueDisable = { "RDis" };
	const recDouble	RevalueTrip = { "RTrip", 0, 999999.99 };
	const recDouble	RevalueTrip2 = { "RTrip2", 0, 999999.99 };
	const recInt	RevalueType = { "RType", 0, 3 };
	const recInt	RevaluePoints = { "RPts", 0, 999999 };
	const recDouble	RevalueValue = { "RVal", 0, 999999.99 };
	const recInt	RevaluePercent = { "RPcnt",	0, 100 };

	const recBool	LoyaltyFlag = { "LFlag" };
	const recBool	LoyaltyDisable = { "LDis" };
	const recDouble	LoyaltyTrip = { "LTrip", 0, 999999.99 };
	//const recDouble	LoyaltyCash			= { "LTrip",	0, 999999.99 };
	//const recDouble	LoyaltyCard			= { "LCard",	0, 999999.99 };
	const recInt	LoyaltyType = { "LType", 0, 2 };
	const recInt	LoyaltyPoints = { "LPts", 0, 999999 };
	const recDouble	LoyaltyValue = { "LVal", 0, 999999.99 };
	const recDouble	LoyaltyTrigger = { "LTrig",	0, 999999.99 };
	const recDouble	LoyaltyLimit = { "LUlmt", 0, 999999.99 };

	const recBool	RedeemFlag = { "RDFlag" };						// Group Specific Redemption
	const recBool	RedeemDisable = { "RDDis" };
	const recInt	RedeemTrip = { "RDTrp",	0, 999999 };
	const recDouble	RedeemValue = { "RDVal", 0, 999999.99 };
	const recInt	RedeemType = { "RDTyp",	0, 1 };
	//const recInt	RedeemPurse			= { "RDPur",	0, 3};
};

/**********************************************************************/

class CLoyaltyGroupDatabase : public CSSIntDatabase
{
public:
	CLoyaltyGroupDatabase() {}

public:
	int OpenReadOnlyForSharing( const char* szFilename );
	int OpenReadWriteForSharing(  const char* szFilename, const char* szBackupFolder );

	int IncrementGroupNo();
	CString GetListboxText();

	int GetGroupNo()				{ return CSSDatabase::GetInt ( LoyaltyGroup::GroupNo.Label ); }
	void SetGroupNo ( int nGroupNo ){ CSSDatabase::Set ( LoyaltyGroup::GroupNo.Label, nGroupNo ); }

	CString GetName()					{ return (CString)CSSDatabase::GetString ( LoyaltyGroup::GroupName.Label ); }
	void SetName ( const char* szText ) { CSSDatabase::Set ( LoyaltyGroup::GroupName.Label, szText ); }

	int GetSchemeNo() { return CSSDatabase::GetInt(LoyaltyGroup::SchemeNo.Label); }
	void SetSchemeNo(int nSchemeNo) { CSSDatabase::Set(LoyaltyGroup::SchemeNo.Label, nSchemeNo); }

	bool GetComplimentary()				{ return CSSDatabase::GetBool ( LoyaltyGroup::Complimentary.Label ); }
	void SetComplimentary(bool bFlag)	{ CSSDatabase::Set ( LoyaltyGroup::Complimentary.Label, bFlag ); }

	bool GetPointsDisabled()			{ return CSSDatabase::GetBool ( LoyaltyGroup::PointsDisabled.Label ); }
	void SetPointsDisabled(bool bFlag)	{ CSSDatabase::Set ( LoyaltyGroup::PointsDisabled.Label, bFlag ); }

	int GetRefreshType()				{ return CSSDatabase::GetInt ( LoyaltyGroup::RefreshType.Label ); }
	void SetRefreshType(int nType)		{ CSSDatabase::Set ( LoyaltyGroup::RefreshType.Label, nType ); }
	CString GetRefreshTypeText();

// Revaluation bonus

	bool GetUseGroupRevaluation()			{ return CSSDatabase::GetBool ( LoyaltyGroup::RevalueFlag.Label ); }
	void SetUseGroupRevaluation(bool bFlag)	{ CSSDatabase::Set ( LoyaltyGroup::RevalueFlag.Label, bFlag ); }

	bool GetRevaluationDisabled()				{ return CSSDatabase::GetBool ( LoyaltyGroup::RevalueDisable.Label ); }
	void SetRevaluationDisabled ( bool bReqd )	{ CSSDatabase::Set ( LoyaltyGroup::RevalueDisable.Label, bReqd ); }

	int GetRevaluationType()				{ return CSSDatabase::GetInt ( LoyaltyGroup::RevalueType.Label ); }
	void SetRevaluationType(int nType)		{ CSSDatabase::Set ( LoyaltyGroup::RevalueType.Label, nType ); }

	double GetRevaluationTrip()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::RevalueTrip.Label ); }
	void SetRevaluationTrip(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::RevalueTrip.Label, dValue, 2 ); }

	double GetRevaluationTrip2()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::RevalueTrip2.Label ); }
	void SetRevaluationTrip2(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::RevalueTrip2.Label, dValue, 2 ); }

	int GetRevaluationPoints()				{ return CSSDatabase::GetInt ( LoyaltyGroup::RevaluePoints.Label ); }
	void SetRevaluationPoints(int nType)	{ CSSDatabase::Set ( LoyaltyGroup::RevaluePoints.Label, nType ); }

	double GetRevaluationValue()			{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::RevalueValue.Label ); }
	void SetRevaluationValue(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::RevalueValue.Label, dValue, 2 ); }

	int GetRevaluationPercent();
	void SetRevaluationPercent(int nPcnt );
	
// Loyalty bonus

	bool GetUseGroupLoyalty()				{ return CSSDatabase::GetBool ( LoyaltyGroup::LoyaltyFlag.Label ); }
	void SetUseGroupLoyalty(bool bFlag)		{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyFlag.Label, bFlag ); }

	bool GetLoyaltyDisabled()					{ return CSSDatabase::GetBool ( LoyaltyGroup::LoyaltyDisable.Label ); }
	void SetLoyaltyDisabled ( bool bReqd )		{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyDisable.Label, bReqd ); }

	int GetLoyaltyType()					{ return CSSDatabase::GetInt ( LoyaltyGroup::LoyaltyType.Label ); }
	void SetLoyaltyType(int nType)			{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyType.Label, nType ); }

	double GetLoyaltyTrip()					{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyTrip.Label ); }
	void SetLoyaltyTrip(double dValue)		{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyTrip.Label, dValue, 2 ); }
//	double GetLoyaltyCashTrip()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyCash.Label ); }
//	void SetLoyaltyCashTrip(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyCash.Label, dValue, 2 ); }
//	double GetLoyaltyCardTrip()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyCard.Label ); }
//	void SetLoyaltyCardTrip(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyCard.Label, dValue, 2 ); }

	int GetLoyaltyPoints()				{ return CSSDatabase::GetInt ( LoyaltyGroup::LoyaltyPoints.Label ); }
	void SetLoyaltyPoints(int nType)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyPoints.Label , nType ); }

	double GetLoyaltyValue()			{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyValue.Label ); }
	void SetLoyaltyValue(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyValue.Label, dValue, 2 ); }

	double GetLoyaltyMultiplierTrigger()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyTrigger.Label ); }
	void SetLoyaltyMultiplierTrigger(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyTrigger.Label, dValue, 2 ); }

	double GetLoyaltyUpperSpendLimit()				{ return (double)CSSDatabase::GetDouble ( LoyaltyGroup::LoyaltyLimit.Label ); }
	void SetLoyaltyUpperSpendLimit(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::LoyaltyLimit.Label, dValue, 2 ); }

// Redeem

	bool GetUseGroupRedeem()				{ return CSSDatabase::GetBool ( LoyaltyGroup::RedeemFlag.Label ); }
	void SetUseGroupRedeem ( bool bReqd )	{ CSSDatabase::Set ( LoyaltyGroup::RedeemFlag.Label, bReqd ); }

	bool GetRedeemDisabled()				{ return CSSDatabase::GetBool ( LoyaltyGroup::RedeemDisable.Label ); }
	void SetRedeemDisabled ( bool bReqd )	{ CSSDatabase::Set ( LoyaltyGroup::RedeemDisable.Label, bReqd ); }

	int GetRedeemTrip()						{ return CSSDatabase::GetInt ( LoyaltyGroup::RedeemTrip.Label ); }
	void SetRedeemTrip ( int nValue )		{ CSSDatabase::Set ( LoyaltyGroup::RedeemTrip.Label, nValue ); }

	double GetRedeemValue()					{ return CSSDatabase::GetDouble ( LoyaltyGroup::RedeemValue.Label ); }
	void SetRedeemValue ( double dValue )	{ Set ( LoyaltyGroup::RedeemValue.Label, dValue, 2 ); }

	int GetRedeemType()						{ return CSSDatabase::GetInt ( LoyaltyGroup::RedeemType.Label ); }
	void SetRedeemType(int nType)			{ CSSDatabase::Set ( LoyaltyGroup::RedeemType.Label, nType ); }


//	int GetRedeemPurse()					{ return CSSDatabase::GetInt ( LoyaltyGroup::RedeemPurse.Label ); }
//	void SetRedeemPurse ( int nPurse )		{ CSSDatabase::Set ( LoyaltyGroup::RedeemPurse.Label, nPurse ); }

// Refreshing

	double GetRefreshValue()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::RefreshValue.Label ); }
	void SetRefreshValue(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::RefreshValue.Label, dValue, 2 ); }

	void ClearRefreshFlags()			{ SetRefreshFlags(0); }

	CString GetRefreshDaysText();
	bool IsRefreshAllowed ( int nDay );
	void SetRefreshAllowed ( int nDay, bool bSet );

	CString GetRefreshWeekText();
	int GetRefreshWeeklyDayNo ( int nDefaultDayNo = 0 );
	void SetRefreshWeeklyDayNo ( int nDayNo );

	CString GetRefreshMonthText();

	CString GetRefreshYearText();
	void GetRefreshYearDate( int& nDayNo, int& nMonthNo );
	void SetRefreshYearDate( int nDayNo, int nMonthNo );

// purses

	bool IsPurseAllowed ( int nPurseNo, int nPeriodIndex );
	void SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet );

	int GetMaxSpendType()				{ return CSSDatabase::GetInt ( LoyaltyGroup::MaxSpendType.Label ); }
	void SetMaxSpendType(int nNo)		{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendType.Label, nNo ); }

	double GetMaxSpendValue()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendValue.Label ); }
	void SetMaxSpendValue(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendValue.Label, dValue, 2 ); }
//NB: MaxSpendPeriod1 is same as MaxSpendValue
	double GetMaxSpendPeriod1()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendValue.Label ); }
	void SetMaxSpendPeriod1(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendValue.Label, dValue, 2 ); }

	double GetMaxSpendPeriod2()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendPeriod2.Label ); }
	void SetMaxSpendPeriod2(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendPeriod2.Label, dValue, 2 ); }

	double GetMaxSpendPeriod3()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendPeriod3.Label ); }
	void SetMaxSpendPeriod3(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendPeriod3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod4()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendPeriod4.Label ); }
	void SetMaxSpendPeriod4(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendPeriod4.Label, dValue, 2 ); }

	double GetMaxSpendPeriod5()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendPeriod5.Label ); }
	void SetMaxSpendPeriod5(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendPeriod5.Label, dValue, 2 ); }

	double GetMaxSpendPeriod6()				{ return CSSDatabase::GetDouble ( LoyaltyGroup::MaxSpendPeriod6.Label ); }
	void SetMaxSpendPeriod6(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::MaxSpendPeriod6.Label, dValue, 2 ); }

	double GetOverSpend()					{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpend.Label ); }
	void SetOverSpend(double dValue)		{ CSSDatabase::Set ( LoyaltyGroup::OverSpend.Label, dValue, 2 ); }
	double GetOverSpendPeriod1()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpend.Label ); }
	void SetOverSpendPeriod1(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpend.Label, dValue, 2 ); }

	double GetOverSpendPeriod2()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpendPeriod2.Label ); }
	void SetOverSpendPeriod2(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpendPeriod2.Label, dValue, 2 ); }

	double GetOverSpendPeriod3()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpendPeriod3.Label ); }
	void SetOverSpendPeriod3(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpendPeriod3.Label, dValue, 2 ); }

	double GetOverSpendPeriod4()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpendPeriod4.Label ); }
	void SetOverSpendPeriod4(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpendPeriod4.Label, dValue, 2 ); }

	double GetOverSpendPeriod5()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpendPeriod5.Label ); }
	void SetOverSpendPeriod5(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpendPeriod5.Label, dValue, 2 ); }

	double GetOverSpendPeriod6()			{ return CSSDatabase::GetDouble ( LoyaltyGroup::OverSpendPeriod6.Label ); }
	void SetOverSpendPeriod6(double dValue)	{ CSSDatabase::Set ( LoyaltyGroup::OverSpendPeriod6.Label, dValue, 2 ); }

private:
	int OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode );

private:
	int GetRefreshFlags()					{ return CSSDatabase::GetInt ( LoyaltyGroup::RefreshAllowed.Label); }
	void SetRefreshFlags ( int nFlags ) 	{ CSSDatabase::Set (LoyaltyGroup::RefreshAllowed.Label, nFlags); }
	bool ValidateYearlyRefreshFactor( int nFactor );

private:
	CString m_strKey;
};

//**********************************************************************
