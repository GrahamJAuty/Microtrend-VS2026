#pragma once

/**********************************************************************/
#define nREFRESHTYPE_DISABLED	0				// Purse2 disabled
#define nREFRESHTYPE_DAILY		1				// Daily Refresh
#define nREFRESHTYPE_WEEKLY		2	
#define nREFRESHTYPE_MONTHLY	3
#define nREFRESHTYPE_YEARLY		4
#define nREFRESHTYPE_MANUAL		5				// manual topup
#define nREFRESHTYPE_PERIOD		6				// daily period refresh
#define nREFRESHTYPE_EOD		7				// refresh after EOD
/**********************************************************************/
#define nOLD_GRPSPENDTYPE_DAILY			0			// Single Daily Spend
#define nOLD_GRPSPENDTYPE_RECORD		1			// Record Specific
#define nOLD_GRPSPENDTYPE_PERIOD		2			// Use Audit Periods
#define nGRPSPENDTYPE_DAILY_LOCKED		3			// Cannot override by account
#define nGRPSPENDTYPE_DAILY_RECORD1		4			// Override value by account
#define nGRPSPENDTYPE_DAILY_RECORD2		5			// Full override by account
#define nGRPSPENDTYPE_PERIOD_LOCKED		6			// Cannot override by account
#define nGRPSPENDTYPE_PERIOD_RECORD1	7			// Override value by account
#define nGRPSPENDTYPE_PERIOD_RECORD2	8			// Full override by account
/**********************************************************************/

extern const char* szVERSION_SMARTPAYDB;

namespace SmartPayGroup
{
const recInt	GroupNo				= { "GroupNo",	1, 99 };
const recString	GroupName			= { "Name",		0, 20 };
const recInt	UserType			= { "UType",	1, 2 };
const recInt	SetNo				= { "SetNo",	1, 9 };
const recInt	PriceLevel			= { "PLevel",	1, 9 };
const recBool	VatShift			= { "VShift"};						// 0-no shift, 1=yes ( AtoD / BtoE / CtoF )
const recInt	RefreshType			= { "RFType",	0, 7 };
const recInt	RefreshAllowed		= { "RFAllow",	0, 1231 };			// if daily max = 127, if monthly max = 31. if yearly max = 1231 (100x12)+ 31
const recDouble	RefreshValue		= { "RFVal",	0, 999.99 };
const recBool	RefreshAppend		= { "RFAppend"};					// Refresh carry forward ( 0-refresh overwrite, 1=add to existing purse )
const recInt	Purse1Allowed		= { "P1Allow",	0, 63 };			// Purse1 allowed Period1 - Period6
const recInt	Purse2Allowed		= { "P2Allow",	0, 63 };			// Purse2 allowed Period1 - Period6
const recInt	Purse3Allowed		= { "P3Allow",	0, 63 };			// Purse3 allowed Period1 - Period6

const recInt	MaxSpendType		= { "MSType",	0, 8 };				// Purse1/Purse2 - Single Daily link / Record specific / Audit Period specific
const recDouble	MaxSpendValue		= { "MSV1",		0, 999.99 };		// Max daily spend or Max daily spend Period1
const recDouble	MaxSpendPeriod2		= { "MSV2",		0, 999.99 };		// Max daily spend Period2
const recDouble	MaxSpendPeriod3		= { "MSV3",		0, 999.99 };		// Max daily spend Period3
const recDouble	MaxSpendPeriod4		= { "MSV4",		0, 999.99 };		// Max daily spend Period4
const recDouble	MaxSpendPeriod5		= { "MSV5",		0, 999.99 };		// Max daily spend Period5
const recDouble	MaxSpendPeriod6		= { "MSV6",		0, 999.99 };		// Max daily spend Period6
const recDouble	OverSpend			= { "OSV1",		0, 999.99 };		// OverSpend
const recDouble	OverSpendPeriod2	= { "OSV2",		0, 999.99 };
const recDouble	OverSpendPeriod3	= { "OSV3",		0, 999.99 };
const recDouble	OverSpendPeriod4	= { "OSV4",		0, 999.99 };
const recDouble	OverSpendPeriod5	= { "OSV5",		0, 999.99 };
const recDouble	OverSpendPeriod6	= { "OSV6",		0, 999.99 };

const recInt	MaxSpendTypeP3		= { "MSType3",	0, 2 };				// Purse3 - Single Daily link / Record specific / Audit Period specific
const recDouble	MaxSpendValueP3		= { "MSV31",	0, 999.99 };		// Purse3 Max daily spend or Max daily spend Period1
const recDouble	MaxSpendPeriod2P3	= { "MSV32",	0, 999.99 };		// Max daily spend Period2
const recDouble	MaxSpendPeriod3P3	= { "MSV33",	0, 999.99 };		// Max daily spend Period3
const recDouble	MaxSpendPeriod4P3	= { "MSV34",	0, 999.99 };		// Max daily spend Period4
const recDouble	MaxSpendPeriod5P3	= { "MSV35",	0, 999.99 };		// Max daily spend Period5
const recDouble	MaxSpendPeriod6P3	= { "MSV36",	0, 999.99 };		// Max daily spend Period6
const recDouble	OverSpendP3			= { "OSV31",	0, 999.99 };		// OverSpend Purse3
const recDouble	OverSpendPeriod2P3	= { "OSV32",	0, 999.99 };
const recDouble	OverSpendPeriod3P3	= { "OSV33",	0, 999.99 };
const recDouble	OverSpendPeriod4P3	= { "OSV34",	0, 999.99 };
const recDouble	OverSpendPeriod5P3	= { "OSV35",	0, 999.99 };
const recDouble	OverSpendPeriod6P3	= { "OSV36",	0, 999.99 };

const recDouble	RefreshPeriod1		= { "RF1",		0, 999.99 };
const recDouble	RefreshPeriod2		= { "RF2",		0, 999.99 };
const recDouble	RefreshPeriod3		= { "RF3",		0, 999.99 };
const recDouble	RefreshPeriod4		= { "RF4",		0, 999.99 };
const recDouble	RefreshPeriod5		= { "RF5",		0, 999.99 };
const recDouble	RefreshPeriod6		= { "RF6",		0, 999.99 };

const recInt	RefreshPeriod1Type	= { "RFT1",		0, 4 };					// Daily, weekly, monthly, yearly
const recInt	RefreshPeriod2Type	= { "RFT2",		0, 4 };
const recInt	RefreshPeriod3Type	= { "RFT3",		0, 4 };
const recInt	RefreshPeriod4Type	= { "RFT4",		0, 4 };
const recInt	RefreshPeriod5Type	= { "RFT5",		0, 4 };
const recInt	RefreshPeriod6Type	= { "RFT6",		0, 4 };


const recString	DeptInhibitPurse1	= { "DLP1",		0, 99 };			// Purse1 Inhibited departments 
const recString	DeptInhibitPurse2	= { "DLP2",		0, 99 };			// Purse2
const recString	DeptInhibitPurse3	= { "DLP3",		0, 99 };			// Purse3
const recString	DeptInhibitCash		= { "DLP0",		0, 99 };			// None purse inhibited departments

//const recString	DeptRestrictedP3List = { "P3List",	0, 99 };			// Purse3 restricted list 
//const recDouble	DeptRestrictedP3Max	 = { "P3Max",	0, 999.99 };		// Purse3 restricted max spend

const recDouble	LowBalancePurse1	= { "LBP1",		0, 999.99 };		// Low Balance Purse1 warning level
const recDouble	LowBalancePurse2	= { "LBP2",		0, 999.99 };		// Low Balance Purse2 warning level
const recDouble	LowBalancePurse3	= { "LBP3",		0, 999.99 };		// Low Balance Purse3 warning level

const recInt	PointsAchievement	= { "PAL1",		0, 999999 };		// Points achievement level

const recBool	RevalueFlag			= { "RVFlag" };							// Group specific Revaluation
const recBool	RevalueDisable		= { "RVDis" };
const recInt	RevalueType			= { "RVType",	0, 1 };					// 0=Points, 1=Cash
const recDouble	RevalueTrip			= { "RVTrp1",	0, 999999.99 };
const recDouble	RevalueTrip2		= { "RVTrp2",	0, 999999.99 };
const recInt	RevaluePoints		= { "RVPts",	0, 999999 };
const recDouble	RevalueValue		= { "RVVal",	0, 999999.99 };

const recBool	RewardFlag			= { "LYFlag" };						// Group Specific Reward
const recBool	RewardDisable		= { "LYDis" };
const recInt	RewardType			= { "LYType",	0, 1 };
const recDouble	RewardTrip			= { "LYTrp",	0, 999999.99 };
const recInt	RewardPoints		= { "LYPts",	0, 999999 };
const recDouble	RewardValue			= { "LYVal",	0, 999999.99 };	
const recDouble	RewardLimit			= { "LYLmt",	0, 999999.99 };			// minimum spend
const recDouble	RewardTrigger		= { "LYTrg",	0, 999999.99 };			// upper limit

const recBool	RedeemFlag			= { "RDFlag" };						// Group Specific Redemption
const recBool	RedeemDisable		= { "RDDis" };
const recInt	RedeemTrip			= { "RDTrp",	0, 999999 };
const recDouble	RedeemValue			= { "RDVal",	0, 999999.99 };	
const recInt	RedeemPurse			= { "RDPur",	0, 3};
};

/**********************************************************************/

class CSmartPayGroupDatabase : public CSSIntDatabase
{
public:
	CSmartPayGroupDatabase(){ CreateHeaders(); }
	int OpenReadOnlyForSharing ( const char* szFilename );
	int OpenReadWriteForSharing ( const char* szFilename, const char* szBackupFolder );
	void SetModified ( bool bFlag ) { CSSDatabase::m_bModified = bFlag; }
	bool GetModified() { return CSSDatabase::m_bModified; }

private:
	void CreateHeaders();
	void AddHeaderField( const char* szField );

public:
	void SetNewRecord();
	int IncrementGroupNo();
	CString GetListboxText();

	void SetGroupNo ( int nGroupNo )	{ CSSDatabase::Set ( SmartPayGroup::GroupNo.Label, nGroupNo ); }
	int GetGroupNo()					{ return CSSDatabase::GetInt ( SmartPayGroup::GroupNo.Label ); }

	CString GetName()					{ return CSSDatabase::GetString ( SmartPayGroup::GroupName.Label ); }
	void SetName ( const char* szText ) { CSSDatabase::Set ( SmartPayGroup::GroupName.Label, szText ); }

	int GetRefreshType()				{ return CSSDatabase::GetInt ( SmartPayGroup::RefreshType.Label ); }
	void SetRefreshType(int nType)		{ CSSDatabase::Set ( SmartPayGroup::RefreshType.Label, nType ); }
	CString GetRefreshTypeText();

	int GetUserType()				{ return CSSDatabase::GetInt ( SmartPayGroup::UserType.Label ); }
	void SetUserType(int nNo)		{ CSSDatabase::Set ( SmartPayGroup::UserType.Label, nNo ); }

	int GetSetNo()					{ return CSSDatabase::GetInt ( SmartPayGroup::SetNo.Label ); }
	void SetSetNo(int nNo)			{ CSSDatabase::Set ( SmartPayGroup::SetNo.Label, nNo ); }

	int GetPriceLevel()				{ return CSSDatabase::GetInt ( SmartPayGroup::PriceLevel.Label ); }
	void SetPriceLevel(int nNo)		{ CSSDatabase::Set ( SmartPayGroup::PriceLevel.Label, nNo ); }

	bool GetVatShift()				{ return CSSDatabase::GetBool ( SmartPayGroup::VatShift.Label ); }
	void SetVatShift ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::VatShift.Label, bReqd ); }

// Refreshing

	bool IsPurse2Refreshable();

	bool GetRefreshAppend()					{ return CSSDatabase::GetBool ( SmartPayGroup::RefreshAppend.Label ); }
	void SetRefreshAppend ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::RefreshAppend.Label, bReqd ); }

	double GetRefreshValue()			{ return CSSDatabase::GetDouble ( SmartPayGroup::RefreshValue.Label ); }
	void SetRefreshValue(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::RefreshValue.Label, dValue, 2 ); }

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

	bool HavePeriodRefresh();
	double GetPeriodRefreshValue ( int nIndex );
	void SetPeriodRefreshValue ( int nIndex, double dValue );

	int GetRefreshPeriodType ( int nIndex );
	void SetRefreshPeriodType ( int nIndex, int nType );
	CString GetRefreshPeriodTypeText ( int nIndex );


//** purse 1 & 2

	bool IsPurseAllowed ( int nPurseNo, int nPeriodIndex );
	void SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet );

	int GetMaxSpendType();
	void SetMaxSpendType(int nNo)			{ CSSDatabase::Set ( SmartPayGroup::MaxSpendType.Label, nNo ); }

	double GetMaxSpendValue()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendValue.Label ); }
	void SetMaxSpendValue(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendValue.Label, dValue, 2 ); }
//NB: MaxSpendPeriod1 is same as MaxSpendValue
	double GetMaxSpendPeriod1()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendValue.Label ); }
	void SetMaxSpendPeriod1(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendValue.Label, dValue, 2 ); }

	double GetMaxSpendPeriod2()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod2.Label ); }
	void SetMaxSpendPeriod2(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod2.Label, dValue, 2 ); }

	double GetMaxSpendPeriod3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod3.Label ); }
	void SetMaxSpendPeriod3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod4()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod4.Label ); }
	void SetMaxSpendPeriod4(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod4.Label, dValue, 2 ); }

	double GetMaxSpendPeriod5()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod5.Label ); }
	void SetMaxSpendPeriod5(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod5.Label, dValue, 2 ); }

	double GetMaxSpendPeriod6()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod6.Label ); }
	void SetMaxSpendPeriod6(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod6.Label, dValue, 2 ); }

	double GetOverSpend()					{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpend.Label ); }
	void SetOverSpend(double dValue)		{ CSSDatabase::Set ( SmartPayGroup::OverSpend.Label, dValue, 2 ); }
	double GetOverSpendPeriod1()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpend.Label ); }
	void SetOverSpendPeriod1(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpend.Label, dValue, 2 ); }

	double GetOverSpendPeriod2()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod2.Label ); }
	void SetOverSpendPeriod2(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod2.Label, dValue, 2 ); }

	double GetOverSpendPeriod3()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod3.Label ); }
	void SetOverSpendPeriod3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod3.Label, dValue, 2 ); }

	double GetOverSpendPeriod4()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod4.Label ); }
	void SetOverSpendPeriod4(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod4.Label, dValue, 2 ); }

	double GetOverSpendPeriod5()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod5.Label ); }
	void SetOverSpendPeriod5(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod5.Label, dValue, 2 ); }

	double GetOverSpendPeriod6()			{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod6.Label ); }
	void SetOverSpendPeriod6(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod6.Label, dValue, 2 ); }

// purse3

	int GetMaxSpendTypeP3()				{ return CSSDatabase::GetInt ( SmartPayGroup::MaxSpendTypeP3.Label ); }
	void SetMaxSpendTypeP3(int nNo)		{ CSSDatabase::Set ( SmartPayGroup::MaxSpendTypeP3.Label, nNo ); }

	double GetMaxSpendValueP3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendValueP3.Label ); }
	void SetMaxSpendValueP3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendValueP3.Label, dValue, 2 ); }
//NB: MaxSpendPeriod1 is same as MaxSpendValue
	double GetMaxSpendPeriod1P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendValueP3.Label ); }
	void SetMaxSpendPeriod1P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendValueP3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod2P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod2P3.Label ); }
	void SetMaxSpendPeriod2P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod2P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod3P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod3P3.Label ); }
	void SetMaxSpendPeriod3P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod3P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod4P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod4P3.Label ); }
	void SetMaxSpendPeriod4P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod4P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod5P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod5P3.Label ); }
	void SetMaxSpendPeriod5P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod5P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod6P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::MaxSpendPeriod6P3.Label ); }
	void SetMaxSpendPeriod6P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::MaxSpendPeriod6P3.Label, dValue, 2 ); }

	double GetOverSpendP3()						{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendP3.Label ); }
	void SetOverSpendP3(double dValue)			{ CSSDatabase::Set ( SmartPayGroup::OverSpendP3.Label, dValue, 2 ); }
	double GetOverSpendPeriod1P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendP3.Label ); }
	void SetOverSpendPeriod1P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendP3.Label, dValue, 2 ); }

	double GetOverSpendPeriod2P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod2P3.Label ); }
	void SetOverSpendPeriod2P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod2P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod3P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod3P3.Label ); }
	void SetOverSpendPeriod3P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod3P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod4P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod4P3.Label ); }
	void SetOverSpendPeriod4P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod4P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod5P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod5P3.Label ); }
	void SetOverSpendPeriod5P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod5P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod6P3()				{ return CSSDatabase::GetDouble ( SmartPayGroup::OverSpendPeriod6P3.Label ); }
	void SetOverSpendPeriod6P3(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::OverSpendPeriod6P3.Label, dValue, 2 ); }

//
	bool ValidateDeptInhibitList ( const char* szText );
	void DisplayDeptListError ( CWnd* pParent );

	CString GetDeptInhibitPurse1()						{ return (CString)CSSDatabase::GetString ( SmartPayGroup::DeptInhibitPurse1.Label ); }
	void SetDeptInhibitPurse1 ( const char* szText )	{ CSSDatabase::Set ( SmartPayGroup::DeptInhibitPurse1.Label, RemoveSpaces ( szText ) ); }

	CString GetDeptInhibitPurse2()						{ return (CString)CSSDatabase::GetString ( SmartPayGroup::DeptInhibitPurse2.Label ); }
	void SetDeptInhibitPurse2 ( const char* szText )	{ CSSDatabase::Set ( SmartPayGroup::DeptInhibitPurse2.Label, RemoveSpaces ( szText ) ); }

	CString GetDeptInhibitPurse3()						{ return (CString)CSSDatabase::GetString ( SmartPayGroup::DeptInhibitPurse3.Label ); }
	void SetDeptInhibitPurse3 ( const char* szText )	{ CSSDatabase::Set ( SmartPayGroup::DeptInhibitPurse3.Label, RemoveSpaces ( szText ) ); }

	CString GetDeptInhibitCash()						{ return (CString)CSSDatabase::GetString ( SmartPayGroup::DeptInhibitCash.Label ); }
	void SetDeptInhibitCash ( const char* szText )		{ CSSDatabase::Set ( SmartPayGroup::DeptInhibitCash.Label, RemoveSpaces ( szText ) ); }


	//bool HaveDeptRestrictionsP3();
	//CString GetDeptRestrictedP3List()					{ return (CString)CSSDatabase::GetString ( SmartPayGroup::DeptRestrictedP3List.Label ); }
	//void SetDeptRestrictedP3List ( const char* szList )	{ CSSDatabase::Set ( SmartPayGroup::DeptRestrictedP3List.Label, RemoveSpaces ( szList ) ); }

	//double GetDeptRestrictedP3Max()					{ return CSSDatabase::GetDouble ( SmartPayGroup::DeptRestrictedP3Max.Label ); }
	//void SetDeptRestrictedP3Max ( double dValue )		{ CSSDatabase::Set ( SmartPayGroup::DeptRestrictedP3Max.Label, dValue, 2 ); }

//
	double GetLowBalancePurse1()						{ return CSSDatabase::GetDouble ( SmartPayGroup::LowBalancePurse1.Label ); }
	void SetLowBalancePurse1(double dValue)				{ CSSDatabase::Set ( SmartPayGroup::LowBalancePurse1.Label, dValue, 2 ); }

	double GetLowBalancePurse2()						{ return CSSDatabase::GetDouble ( SmartPayGroup::LowBalancePurse2.Label ); }
	void SetLowBalancePurse2(double dValue)				{ CSSDatabase::Set ( SmartPayGroup::LowBalancePurse2.Label, dValue, 2 ); }

	double GetLowBalancePurse3()						{ return CSSDatabase::GetDouble ( SmartPayGroup::LowBalancePurse3.Label ); }
	void SetLowBalancePurse3(double dValue)				{ CSSDatabase::Set ( SmartPayGroup::LowBalancePurse3.Label, dValue, 2 ); }

	int GetPointsAchievement()							{ return CSSDatabase::GetInt ( SmartPayGroup::PointsAchievement.Label ); }
	void SetPointsAchievement(int nPoints)				{ CSSDatabase::Set ( SmartPayGroup::PointsAchievement.Label, nPoints ); }

// Revaluation bonus

	bool GetUseGroupRevaluation()				{  return CSSDatabase::GetBool ( SmartPayGroup::RevalueFlag.Label ); }
	void SetUseGroupRevaluation ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::RevalueFlag.Label, bReqd ); }

	bool GetRevaluationDisabled()				{ return CSSDatabase::GetBool ( SmartPayGroup::RevalueDisable.Label ); }
	void SetRevaluationDisabled ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::RevalueDisable.Label, bReqd ); }

	int GetRevaluationType()					{ return CSSDatabase::GetInt ( SmartPayGroup::RevalueType.Label ); }
	void SetRevaluationType ( int nType )		{ CSSDatabase::Set ( SmartPayGroup::RevalueType.Label, nType ); }

	double GetRevaluationTrip()					{ return CSSDatabase::GetDouble ( SmartPayGroup::RevalueTrip.Label ); }
	void SetRevaluationTrip ( double dValue )	{ CSSDatabase::Set ( SmartPayGroup::RevalueTrip.Label, dValue, 2 ); }

	double GetRevaluationTrip2()				{ return CSSDatabase::GetInt ( SmartPayGroup::RevalueTrip2.Label ); }
	void SetRevaluationTrip2 ( double dValue )	{ CSSDatabase::Set ( SmartPayGroup::RevalueTrip2.Label, dValue, 2 ); }

	int GetRevaluationPoints()					{ return CSSDatabase::GetInt ( SmartPayGroup::RevaluePoints.Label ); }
	void SetRevaluationPoints ( int nPoints )	{ Set ( SmartPayGroup::RevaluePoints.Label, nPoints ); }

	double GetRevaluationValue()				{ return CSSDatabase::GetDouble ( SmartPayGroup::RevalueValue.Label ); }
	void SetRevaluationValue ( double dValue )	{ Set ( SmartPayGroup::RevalueValue.Label, dValue, 2 ); }

// bonus

	bool GetUseGroupReward()					{ return CSSDatabase::GetBool ( SmartPayGroup::RewardFlag.Label ); }
	void SetUseGroupReward ( bool bReqd )		{ Set ( SmartPayGroup::RewardFlag.Label, bReqd ); }

	bool GetRewardDisabled()					{ return CSSDatabase::GetBool ( SmartPayGroup::RewardDisable.Label ); }
	void SetRewardDisabled ( bool bReqd )		{ CSSDatabase::Set ( SmartPayGroup::RewardDisable.Label, bReqd ); }

	int GetRewardType()							{ return CSSDatabase::GetInt ( SmartPayGroup::RewardType.Label ); }
	void SetRewardType ( int nType )			{ CSSDatabase::Set ( SmartPayGroup::RewardType.Label, nType ); }

	double GetRewardSpendTrip()					{ return CSSDatabase::GetDouble ( SmartPayGroup::RewardTrip.Label ); }	
	void SetRewardSpendTrip ( double dValue )	{ CSSDatabase::Set ( SmartPayGroup::RewardTrip.Label, dValue, 2 ); }	

	int GetRewardPoints()						{ return CSSDatabase::GetInt ( SmartPayGroup::RewardPoints.Label ); }
	void SetRewardPoints ( int nPoints )		{ CSSDatabase::Set ( SmartPayGroup::RewardPoints.Label, nPoints ); }

	double GetRewardValue()						{ return CSSDatabase::GetDouble ( SmartPayGroup::RewardValue.Label ); }
	void SetRewardValue ( double dValue )		{ Set( SmartPayGroup::RewardValue.Label, dValue, 2 ); }

	double GetRewardMultiplierTrigger()				{ return CSSDatabase::GetDouble ( SmartPayGroup::RewardTrigger.Label ); }
	void SetRewardMultiplierTrigger ( double dValue )	{ Set ( SmartPayGroup::RewardTrigger.Label, dValue, 2 ); }

	double GetRewardUpperSpendLimit()				{ return (double)CSSDatabase::GetDouble ( SmartPayGroup::RewardLimit.Label ); }
	void SetRewardUpperSpendLimit(double dValue)	{ CSSDatabase::Set ( SmartPayGroup::RewardLimit.Label, dValue, 2 ); }

// Redeem

	bool GetUseGroupRedeem()				{ return CSSDatabase::GetBool ( SmartPayGroup::RedeemFlag.Label ); }
	void SetUseGroupRedeem ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::RedeemFlag.Label, bReqd ); }

	bool GetRedeemDisabled()				{ return CSSDatabase::GetBool ( SmartPayGroup::RedeemDisable.Label ); }
	void SetRedeemDisabled ( bool bReqd )	{ CSSDatabase::Set ( SmartPayGroup::RedeemDisable.Label, bReqd ); }

	int GetRedeemTrip()						{ return CSSDatabase::GetInt ( SmartPayGroup::RedeemTrip.Label ); }
	void SetRedeemTrip ( int nValue )		{ CSSDatabase::Set ( SmartPayGroup::RedeemTrip.Label, nValue ); }

	double GetRedeemValue()					{ return CSSDatabase::GetDouble ( SmartPayGroup::RedeemValue.Label ); }
	void SetRedeemValue ( double dValue )	{ Set ( SmartPayGroup::RedeemValue.Label, dValue, 2 ); }

	int GetRedeemPurse()					{ return CSSDatabase::GetInt ( SmartPayGroup::RedeemPurse.Label ); }
	void SetRedeemPurse ( int nPurse )		{ CSSDatabase::Set ( SmartPayGroup::RedeemPurse.Label, nPurse ); }

private:
	int OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode );
	
private:
	int GetRefreshFlags()					{ return CSSDatabase::GetInt ( SmartPayGroup::RefreshAllowed.Label); }
	void SetRefreshFlags ( int nFlags ) 	{ CSSDatabase::Set (SmartPayGroup::RefreshAllowed.Label, nFlags); }
	CString RemoveSpaces ( const char* szText );

private:
	CString m_strHeader;
};

//**********************************************************************
//**********************************************************************
//**********************************************************************
#define nRECORD_SPENDTYPE_GROUP 0
#define nRECORD_SPENDTYPE_DAILY 1
#define nRECORD_SPENDTYPE_PERIOD 2
//**********************************************************************

namespace SmartPay
{
const recString		Blank			= { "<blank>", 0, 0 };
const recString		Text			= { "<text>", 0, 40 };
const recString		DobCheck		= { "<Anniversary>", 0, 40 };

//const recString		UserID			= { "UserID", 1, 6 };
//const recString		NextCardNo		= { "NextNo", 1, 6 };
///const recString		PreviousCardNo	= { "PreviousNo", 1, 6 };
const recString		ExUserID			= { "UserID", 1, 10 };				// Biometric length = 6, MiFare.v1 length = 10, MiFare.v2 length = 6
const recString		ExNextCardNo		= { "NextNo", 1, 10 };				//
const recString		ExPreviousCardNo	= { "PreviousNo", 1, 10 };			// use System.GetUserIDLength() to get max length

const recString		ExtRef			= { "ExtRef", 1, 20 };
const recString		ExtRef2			= { "ExtRef2", 1, 20 };
const recString		MemberID		= { "MemberID", 1, 13 };
const recInt		GroupNo			= { "GroupNo", 1 , 99 };
const recString		UserName		= { "UserName", 0, 40 };
const recString		Forename		= { "Forename", 0, 40 };
const recString		Surname			= { "Surname", 0, 40 };
const recString		Gender			= { "Gender", 0, 1 };
const recString		Dob				= { "DOB", 0, 10 };
const recString		Expiry			= { "Expiry", 0, 10 };
const recInt		AllergyCodes	= { "Allergy", 0, 16383 };				// binary bit flags 11111111111111 ( 14 x 1 )
const recString		Dietary			= { "Dietary", 0, 100 };
const recString		AlertDate		= { "AlertDate", 0, 8 };
const recString		AlertTime		= { "AlertTime", 0, 8 }; 
const recInt		AlertCode		= { "Alert",0, 9 };
const recBool		Inactive		= { "Inactive" };

const recBool		OwnMaxSpend			= { "OwnMaxSpend" };
const recInt		Purse1Allowed		= { "P1Allow",	0, 63 };			// Purse1 allowed Period1 - Period6
const recInt		Purse2Allowed		= { "P2Allow",	0, 63 };			// Purse2 allowed Period1 - Period6
const recInt		Purse3Allowed		= { "P3Allow",	0, 63 };			// Purse3 allowed Period1 - Period6
const recDouble		MaxSpendValue		= { "MaxSpend", 0.00, 999.99 };
const recDouble		MaxSpendPeriod2		= { "MSV2",		0, 999.99 };		// Max daily spend Period2
const recDouble		MaxSpendPeriod3		= { "MSV3",		0, 999.99 };		// Max daily spend Period3
const recDouble		MaxSpendPeriod4		= { "MSV4",		0, 999.99 };		// Max daily spend Period4
const recDouble		MaxSpendPeriod5		= { "MSV5",		0, 999.99 };		// Max daily spend Period5
const recDouble		MaxSpendPeriod6		= { "MSV6",		0, 999.99 };		// Max daily spend Period6
const recDouble		OverSpend			= { "OverSpend", 0.00, 999.99 };
const recDouble		OverSpendPeriod2	= { "OSV2",		0, 999.99 };
const recDouble		OverSpendPeriod3	= { "OSV3",		0, 999.99 };
const recDouble		OverSpendPeriod4	= { "OSV4",		0, 999.99 };
const recDouble		OverSpendPeriod5	= { "OSV5",		0, 999.99 };
const recDouble		OverSpendPeriod6	= { "OSV6",		0, 999.99 };
const recDouble		MaxSpendValueP3		= { "MaxSpendP3", 0.00, 999.99 };
const recDouble		MaxSpendPeriod2P3	= { "MSV32",	0, 999.99 };		// Max daily spend Period2
const recDouble		MaxSpendPeriod3P3	= { "MSV33",	0, 999.99 };		// Max daily spend Period3
const recDouble		MaxSpendPeriod4P3	= { "MSV34",	0, 999.99 };		// Max daily spend Period4
const recDouble		MaxSpendPeriod5P3	= { "MSV35",	0, 999.99 };		// Max daily spend Period5
const recDouble		MaxSpendPeriod6P3	= { "MSV36",	0, 999.99 };		// Max daily spend Period6
const recDouble		OverSpendP3			= { "OverSpendP3", 0.00, 999.99 };
const recDouble		OverSpendPeriod2P3	= { "OSV32",	0, 999.99 };
const recDouble		OverSpendPeriod3P3	= { "OSV33",	0, 999.99 };
const recDouble		OverSpendPeriod4P3	= { "OSV34",	0, 999.99 };
const recDouble		OverSpendPeriod5P3	= { "OSV35",	0, 999.99 };
const recDouble		OverSpendPeriod6P3	= { "OSV36",	0, 999.99 };
const recString		DayGroups			= { "DayGroups", 0, 20 };			// max = 20 - "99,99,99,99,99,99,99" (SundayGroup,MondayGroup,,,,,SaturdayGroup"

const recDouble		Purse1				= { "Purse1", -99999999.99, 99999999.99 };
const recDouble		Purse1Credit		= { "Purse1Credit", -99999999.99, 99999999.99 };
const recString		Purse1DateLastUsed	= { "Purse1Date", 0, 8 };
const recString		Purse1TimeLastUsed	= { "Purse1Time", 0, 8 }; 
const recDouble		Purse1LastSpend		= { "Purse1Spend", 0.00, 99999999.99 };
const recDouble		Purse1SpendTD		= { "Purse1SpendTD", 0.00, 99999999.99 };

const recDouble		Purse2				= { "Purse2", -99999999.99, 99999999.99 };
const recString		Purse2DateLastUsed	= { "Purse2Date", 0, 8 };
const recString		Purse2TimeLastUsed	= { "Purse2Time", 0, 8 }; 
const recDouble		Purse2LastSpend		= { "Purse2Spend", 0.00, 99999999.99 };
const recDouble		Purse2SpendTD		= { "Purse2SpendTD", 0.00, 99999999.99 };
const recString		RefreshedDate		= { "RefreshDate", 0, 8 };
const recString		RefreshedTime		= { "RefreshTime", 0, 8 }; 
const recInt		RefreshRef			= { "RefreshRef",	0, 254 };		// if daily max = 510 ( not used -> monthly max = 31. if yearly max = 1231 (100x12)+ 31 )

const recDouble		Purse3				= { "Purse3", -99999999.99, 99999999.99 };
const recDouble		Purse3Credit		= { "Purse3Credit", -99999999.99, 99999999.99 };
const recString		Purse3DateLastUsed	= { "Purse3Date", 0, 8 };
const recString		Purse3TimeLastUsed	= { "Purse3Time", 0, 8 }; 
const recDouble		Purse3LastSpend		= { "Purse3Spend", 0.00, 99999999.99 };
const recDouble		Purse3SpendTD		= { "Purse3SpendTD", 0.00, 99999999.99 };

//const recString		P3StrictedList		= { "R3List", 0, 40 };
//const recDouble		P3StrictedLimit		= { "R3Max", 0.00, 9999.99 };
const recString		P3StrictedDate		= { "R3Date", 0, 8 };
const recDouble		P3StrictedSpend		= { "R3Spend", 0.00, 9999.99 };

const recInt		Points				= { "Points",-99999999, 99999999 };
const recInt		PointsTD			= { "PointsTD",-99999999, 99999999 };

const recDouble		CashSpendTD			= { "CashSpendTD", 0.00, 99999999.99 };

const recString		Email				= { "Email", 0, 80 };
const recString		Info1				= { "Info1", 0, 80 };
const recString		Info2				= { "Info2", 0, 80 };
const recString		Info3				= { "Info3", 0, 80 };
const recString		Info4				= { "Info4", 0, 80 };
const recString		Info5				= { "Info5", 0, 80 };
const recString		Info6				= { "Info6", 0, 80 };
const recString		Info7				= { "Info7", 0, 80 };
const recString		Info8				= { "Info8", 0, 80 };

const recBool		Tick1				= { "Tick1" };
const recBool		Tick2				= { "Tick2" };	
const recBool		Tick3				= { "Tick3" };	
const recBool		Tick4				= { "Tick4" };	

// below only used for label
const recDouble		TotalSpendTD		= { "TotalSpendTD", 0, 99999999.99 };
const recString		ForenameSurname		= { "Forename:Surname", 0, 0 };
const recString		FullUserID			= { "FullUserID", 0, 0 };
const recString		GroupUserID			= { "GroupNo:UserID", 0, 0 };
const recString		GroupFullUserID		= { "GroupNo:FullUserID", 0, 0 };
const recString		GroupType			= { "GroupType", 0 , 0 };
const recString		GroupTypeUserID		= { "GroupType:UserID", 0, 0 };
const recString		GroupTypeFullUserID	= { "GroupType:FullUserID", 0, 0 };

const recString		AllergyText			= { "AllergyText", 0, 0 };
const recString		AlertText			= { "AlertText", 0, 0 };
const recString		DobNextAge			= { "DOBNextAge", 0, 0 }; 	// Age next birthday
const recString		DobDay				= { "DOBBDay", 0, 0 };		// Next birthday Day of week 
const recString		DeletedDate			= { "DeletedDate", 0, 0 };	// delete header fields
const recString		DeletedTime			= { "DeletedTime", 0, 0 };
}

/**********************************************************************/

class CSmartPayDatabase : public CSSStringDatabase
{
public:
	CSmartPayDatabase();
	virtual const char* FormatKey ( const char* szKey );
	const char* GetError() { return m_strError; }

private:
	void CreateHeaders();
	void AddHeaderField( const char* szField, bool bAddToBioHeader = TRUE );

public:
	const char* GetHeader();
	const char* GetBiometricHeader();
	void SetModified ( bool bFlag ) { CSSDatabase::m_bModified = bFlag; }
	bool GetModified() { return CSSDatabase::m_bModified; }

	bool IsAvailable ( const char* szFilename );
	int OpenReadOnlyForSharing( const char* szFilename );
	int OpenReadWriteForSharing(  const char* szFilename, const char* szBackupFolder );
	int ReOpenReadOnlyForSharing();
	int ReOpenReadWriteForSharing( const char* szBackupFolder );
	bool Close();

	int GetPercentPosition();
	int GetRecordStatusColour();
	bool IsRecordEmpty();

	bool CopyRecord ( const char* szFromCardNo, const char* szToCardNo, bool bFailIfExists );
	bool CopyRecordToFile ( const char* szFilename );
	bool CopyFileToRecord ( const char* szFilename );
	CString CopyRecordToString();

	void CopyToDatabase ( CSmartPayDatabase* pDbCopy );

	bool GetLastUsed ( CSSDate& date, CSSTime& time );

	CString GetUserID() { return (CString)CSSDatabase::GetString ( SmartPay::ExUserID.Label ); }
	bool IsValidUserID ( const char* szCardNo, CString& strCardNo, int nMaxLength );

//	CString IncrementUserID (  __int64 nMaxID );
//	__int64 GetNextFreeUserID();

	CString GetNextFreeUserID ( const char* szStartFrom, const char* szMaxID );
	CString IncrementUserID ( const char* szUserID, const char* szMaxID );

	CString GetMemberID() { return (CString)CSSDatabase::GetString ( SmartPay::MemberID.Label ); }
	void SetMemberID ( const char* szMemberID );

	CString GetExtRef() { return (CString)CSSDatabase::GetString ( SmartPay::ExtRef.Label ); }
	void SetExtRef ( const char* szExtRef );

	CString GetExtRef2() { return (CString)CSSDatabase::GetString ( SmartPay::ExtRef2.Label ); }
	void SetExtRef2 ( const char* szExtRef );

	CString GetNextCardNo() { return (CString)CSSDatabase::GetString ( SmartPay::ExNextCardNo.Label ); }
	void SetNextCardNo ( const char* szCardNo );
	bool HaveNextCardNo() { return ( GetNextCardNo() == "" ) ? FALSE : TRUE; }

	CString GetPreviousCardNo() { return (CString)CSSDatabase::GetString ( SmartPay::ExPreviousCardNo.Label ); }
	void SetPreviousCardNo ( const char* szCardNo );
	bool HavePreviousCardNo() { return ( GetPreviousCardNo() == "" ) ? FALSE : TRUE; }

	CString GetUserName();
	void SetUserName ( const char* szText );
	CString GetAccountDisplayName();

	CString GetForename() { return (CString)CSSDatabase::GetString ( SmartPay::Forename.Label ); }
	void SetForename ( const char* szText );

	CString GetSurname() { return (CString)CSSDatabase::GetString ( SmartPay::Surname.Label ); }
	void SetSurname ( const char* szText );

	CString GetEmail() { return (CString)CSSDatabase::GetString ( SmartPay::Email.Label ); }
	void SetEmail ( const char* szText );

	int GetGroupNo();
	void SetGroupNo ( int nGroupNo );
	CString GetGroupNoString();

	bool HaveDailyGroups();
	void SetDailyGroups ( const char* szText );
	CString GetDailyGroups()	{ return (CString)CSSDatabase::GetString ( SmartPay::DayGroups.Label ); }
	void SetDailyGroupNo ( int nDay, int nGroup );
	int GetDailyGroupNo ( int nDayNo );							// 0=sunday, 1=monday...

	CString GetGender() { return (CString)CSSDatabase::GetString ( SmartPay::Gender.Label ); }
	void SetGender ( const char* szGender );
	CString GetGenderText();
	bool IsGenderMale()		{ return ( GetGender() == "M" ) ? TRUE : FALSE;  }
	bool IsGenderFemale()	{ return ( GetGender() == "F" ) ? TRUE : FALSE; }
	bool IsGenderUnknown()	{ return ( GetGender() == "" ) ? TRUE : FALSE; }

//** points

	int GetPoints() { return (int)CSSDatabase::GetInt ( SmartPay::Points.Label ); }
	void SetPoints ( int nPoints );
	void AddToPoints ( int nPoints)		{ SetPoints ( GetPoints() + nPoints ); }
	void SubtractPoints ( int nPoints )	{ SetPoints ( GetPoints() - nPoints ); }
	void UpdatePoints ( int nPoints );
	CString GetPointsString();

	int GetPointsTD() { return (int)CSSDatabase::GetInt ( SmartPay::PointsTD.Label ); }
	void SetPointsTD ( int nPoints );
	void AddToPointsTD ( int nPoints) { SetPointsTD ( GetPointsTD() + nPoints ); }
	CString GetPointsTDString();

// cash spend

	double GetCashSpendTD() { return (double)CSSDatabase::GetDouble ( SmartPay::CashSpendTD.Label ); }
	void SetCashSpendTD ( double dValue );
	void AddToCashSpendTD ( double dValue )	{ SetCashSpendTD ( GetCashSpendTD() + dValue ); }

	CString GetPurseBalanceString(int nPurseNo);

// Purse1

	void SetPurse1DateLastUsed ( const char* szDate )	{ SetDateField ( SmartPay::Purse1DateLastUsed.Label, szDate ); }
	CString GetPurse1DateLastUsed()	{ return(CString)CSSDatabase::GetString ( SmartPay::Purse1DateLastUsed.Label ); }
	void SetPurse1TimeLastUsed ( const char* szTime )	{ SetTimeField ( SmartPay::Purse1TimeLastUsed.Label, szTime ); }
	CString GetPurse1TimeLastUsed() { return (CString)CSSDatabase::GetString ( SmartPay::Purse1TimeLastUsed.Label ); }

	double GetPurse1Liability() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse1.Label ); }
	void SetPurse1Liability ( double dValue );
	void AddToPurse1Liability ( double dValue )		{ SetPurse1Liability ( GetPurse1Liability() + dValue ); }
	void SubtractFromPurse1Liability ( double dValue )	 { SetPurse1Liability ( GetPurse1Liability() - dValue ); }

	double GetPurse1Credit() { return (double)CSSDatabase::GetDouble(SmartPay::Purse1Credit.Label); }
	void SetPurse1Credit(double dValue);
	void AddToPurse1Credit(double dValue) { SetPurse1Credit(GetPurse1Credit() + dValue); }
	void SubtractFromPurse1Credit(double dValue) { SetPurse1Credit(GetPurse1Credit() - dValue); }

	double GetPurse1Balance() { return GetPurse1Credit() + GetPurse1Liability(); }
	void AllocatePurse1Spend(double dValue, double& dLiability, double& dCredit );

	double GetPurse1LastSpend() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse1LastSpend.Label ); }
	void SetPurse1LastSpend ( double dValue );
	void AddToPurse1LastSpend ( double dValue )	{ SetPurse1LastSpend ( GetPurse1LastSpend() + dValue ); }

	double GetPurse1SpendTD() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse1SpendTD.Label ); }
	void SetPurse1SpendTD ( double dValue );
	void AddToPurse1SpendTD ( double dValue )	{ SetPurse1SpendTD ( GetPurse1SpendTD() + dValue ); }

// purse2

	bool IsRefreshAllowed ( int nDay );
	void SetRefreshAllowed ( int nDay, bool bSet );
	bool IsRefreshSet() { return ( CSSDatabase::GetInt (SmartPay::RefreshRef.Label) == 0 ) ? FALSE : TRUE; }
	void ClearRefreshFlags() { CSSDatabase::Set (SmartPay::RefreshRef.Label, 0 ); }
	CString GetRefreshTextDays();
	void SetRefreshRef ( const char* szDaysText );

	void RevaluePurse2 ( double dValue, const char* szDate = "", const char* szTime = "" );

	void SetPurse2DateLastUsed ( const char* szDate )	{ SetDateField ( SmartPay::Purse2DateLastUsed.Label, szDate ); }
	CString GetPurse2DateLastUsed()						{ return(CString)CSSDatabase::GetString ( SmartPay::Purse2DateLastUsed.Label ); }

	void SetPurse2TimeLastUsed ( const char* szTime )	{ SetTimeField ( SmartPay::Purse2TimeLastUsed.Label, szTime ); }
	CString GetPurse2TimeLastUsed()						{ return (CString)CSSDatabase::GetString ( SmartPay::Purse2TimeLastUsed.Label ); }

	double GetPurse2Balance() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse2.Label ); }
	void SetPurse2Balance ( double dValue );
	void AddToPurse2Balance ( double dValue )			 { SetPurse2Balance ( GetPurse2Balance() + dValue ); }
	void SubtractFromPurse2Balance ( double dValue )	 { SetPurse2Balance ( GetPurse2Balance() - dValue ); }

	double GetPurse2LastSpend() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse2LastSpend.Label ); }
	void SetPurse2LastSpend ( double dValue );
	void AddToPurse2LastSpend ( double dValue )	{ SetPurse2LastSpend ( GetPurse2LastSpend() + dValue ); }

	double GetPurse2SpendTD() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse2SpendTD.Label ); }
	void SetPurse2SpendTD ( double dValue );
	void AddToPurse2SpendTD ( double dValue )	{ SetPurse2SpendTD ( GetPurse2SpendTD() + dValue ); }

	CString GetRefreshedDate() { return(CString)CSSDatabase::GetString ( SmartPay::RefreshedDate.Label ); }
	void SetRefreshedDate ( const char* szDate ) { SetDateField ( SmartPay::RefreshedDate.Label, szDate ); }

	CString GetRefreshedTime() { return (CString)CSSDatabase::GetString ( SmartPay::RefreshedTime.Label ); }
	void SetRefreshedTime ( const char* szTime ) { SetTimeField ( SmartPay::RefreshedTime.Label, szTime ); }

// Purse3

	void SetPurse3DateLastUsed ( const char* szDate )	{ SetDateField ( SmartPay::Purse3DateLastUsed.Label, szDate ); }
	CString GetPurse3DateLastUsed()	{ return(CString)CSSDatabase::GetString ( SmartPay::Purse3DateLastUsed.Label ); }
	void SetPurse3TimeLastUsed ( const char* szTime )	{ SetTimeField ( SmartPay::Purse3TimeLastUsed.Label, szTime ); }
	CString GetPurse3TimeLastUsed() { return (CString)CSSDatabase::GetString ( SmartPay::Purse3TimeLastUsed.Label ); }

	double GetPurse3Liability() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse3.Label ); }
	void SetPurse3Liability( double dValue );
	void AddToPurse3Liability( double dValue )			 { SetPurse3Liability( GetPurse3Liability() + dValue ); }
	void SubtractFromPurse3Liability( double dValue )	 { SetPurse3Liability( GetPurse3Liability() - dValue ); }

	double GetPurse3Credit() { return (double)CSSDatabase::GetDouble(SmartPay::Purse3Credit.Label); }
	void SetPurse3Credit(double dValue);
	void AddToPurse3Credit(double dValue) { SetPurse3Credit(GetPurse3Credit() + dValue); }
	void SubtractFromPurse3Credit(double dValue) { SetPurse3Credit(GetPurse3Credit() - dValue); }

	double GetPurse3Balance() { return GetPurse3Credit() + GetPurse3Liability(); }
	void AllocatePurse3Spend(double dValue, double& dLiability, double& dCredit);

	double GetPurse3LastSpend() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse3LastSpend.Label ); }
	void SetPurse3LastSpend ( double dValue );
	void AddToPurse3LastSpend ( double dValue )	{ SetPurse3LastSpend ( GetPurse3LastSpend() + dValue ); }

	double GetPurse3SpendTD() { return (double)CSSDatabase::GetDouble ( SmartPay::Purse3SpendTD.Label ); }
	void SetPurse3SpendTD ( double dValue );
	void AddToPurse3SpendTD ( double dValue )	{ SetPurse3SpendTD ( GetPurse3SpendTD() + dValue ); }

// Purse3 restricted spend

//	CString GetP3RestrictedList()					{ return(CString)CSSDatabase::GetString ( SmartPay::P3StrictedList.Label ); }
//	void SetP3RestrictedList ( const char* szList )	{ SetField ( SmartPay::P3StrictedList.Label, szList, SmartPay::P3StrictedList.Max );}	
//
//	double GetP3RestrictedLimit()					{ return (double)CSSDatabase::GetDouble ( SmartPay::P3StrictedLimit.Label ); }
//	void SetP3RestrictedLimit ( double dValue )	{ SetField ( SmartPay::P3StrictedLimit.Label, dValue, SmartPay::P3StrictedLimit.Min, SmartPay::P3StrictedLimit.Max );	}

//	CString GetP3RestrictedDate()	{ return(CString)CSSDatabase::GetString ( SmartPay::P3StrictedDate.Label ); }
//	void SetP3RestrictedDate ( const char* szDate )	{ SetDateField ( SmartPay::P3StrictedDate.Label, szDate );	}

//	double GetP3StrictedSpend()						{ return (double)CSSDatabase::GetDouble ( SmartPay::P3StrictedSpend.Label ); }
//	void SetP3RestrictedSpend ( double dValue )		{ SetField ( SmartPay::P3StrictedSpend.Label, dValue, SmartPay::P3StrictedSpend.Min, SmartPay::P3StrictedSpend.Max );	}
//	void AddToP3RetrictedSpend ( double dValue )	{ SetP3RestrictedSpend ( GetP3StrictedSpend() + dValue ); }


//** other spends

	double GetTotalSpendTD() { return ( GetPurse1SpendTD() + GetPurse2SpendTD()  + GetPurse3SpendTD() + GetCashSpendTD() ); }
	CString GetTotalSpendTDString();

	bool IsPurseAllowed ( int nPurseNo, int nPeriodIndex );
	void SetPurseAllowed ( int nPurseNo, int nPeriodIndex, bool bSet );
	
	int GetPurse1AllowedFlags()				{ return CSSDatabase::GetInt( SmartPay::Purse1Allowed.Label ); }
	void SetPurse1AllowedFlags( int n )		{ CSSDatabase::Set( SmartPay::Purse1Allowed.Label, n ); }

	int GetPurse2AllowedFlags()				{ return CSSDatabase::GetInt( SmartPay::Purse2Allowed.Label ); }
	void SetPurse2AllowedFlags( int n )		{ CSSDatabase::Set( SmartPay::Purse2Allowed.Label, n ); }

	int GetPurse3AllowedFlags()				{ return CSSDatabase::GetInt( SmartPay::Purse3Allowed.Label ); }
	void SetPurse3AllowedFlags( int n )		{ CSSDatabase::Set( SmartPay::Purse3Allowed.Label, n ); }

	bool GetOwnMaxSpend(){ return CSSDatabase::GetBool ( SmartPay::OwnMaxSpend.Label );  }
	void SetOwnMaxSpend( bool b ){ Set( SmartPay::OwnMaxSpend.Label, b ); }

	double GetMaxSpendValue()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendValue.Label ); }
	void SetMaxSpendValue(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendValue.Label, dValue, 2 ); }
	//NB: MaxSpendPeriod1 is same as MaxSpendValue
	double GetMaxSpendPeriod1()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendValue.Label ); }
	void SetMaxSpendPeriod1(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendValue.Label, dValue, 2 ); }

	double GetMaxSpendPeriod2()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod2.Label ); }
	void SetMaxSpendPeriod2(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod2.Label, dValue, 2 ); }

	double GetMaxSpendPeriod3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod3.Label ); }
	void SetMaxSpendPeriod3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod4()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod4.Label ); }
	void SetMaxSpendPeriod4(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod4.Label, dValue, 2 ); }

	double GetMaxSpendPeriod5()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod5.Label ); }
	void SetMaxSpendPeriod5(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod5.Label, dValue, 2 ); }

	double GetMaxSpendPeriod6()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod6.Label ); }
	void SetMaxSpendPeriod6(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod6.Label, dValue, 2 ); }

	double GetOverSpend()					{ return CSSDatabase::GetDouble ( SmartPay::OverSpend.Label ); }
	void SetOverSpend(double dValue)		{ CSSDatabase::Set ( SmartPay::OverSpend.Label, dValue, 2 ); }
	double GetOverSpendPeriod1()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpend.Label ); }
	void SetOverSpendPeriod1(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpend.Label, dValue, 2 ); }

	double GetOverSpendPeriod2()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod2.Label ); }
	void SetOverSpendPeriod2(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod2.Label, dValue, 2 ); }

	double GetOverSpendPeriod3()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod3.Label ); }
	void SetOverSpendPeriod3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod3.Label, dValue, 2 ); }

	double GetOverSpendPeriod4()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod4.Label ); }
	void SetOverSpendPeriod4(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod4.Label, dValue, 2 ); }

	double GetOverSpendPeriod5()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod5.Label ); }
	void SetOverSpendPeriod5(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod5.Label, dValue, 2 ); }

	double GetOverSpendPeriod6()			{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod6.Label ); }
	void SetOverSpendPeriod6(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod6.Label, dValue, 2 ); }

	double GetMaxSpendValueP3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendValueP3.Label ); }
	void SetMaxSpendValueP3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendValueP3.Label, dValue, 2 ); }
//NB: MaxSpendPeriod1 is same as MaxSpendValue
	double GetMaxSpendPeriod1P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendValueP3.Label ); }
	void SetMaxSpendPeriod1P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendValueP3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod2P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod2P3.Label ); }
	void SetMaxSpendPeriod2P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod2P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod3P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod3P3.Label ); }
	void SetMaxSpendPeriod3P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod3P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod4P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod4P3.Label ); }
	void SetMaxSpendPeriod4P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod4P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod5P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod5P3.Label ); }
	void SetMaxSpendPeriod5P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod5P3.Label, dValue, 2 ); }

	double GetMaxSpendPeriod6P3()				{ return CSSDatabase::GetDouble ( SmartPay::MaxSpendPeriod6P3.Label ); }
	void SetMaxSpendPeriod6P3(double dValue)	{ CSSDatabase::Set ( SmartPay::MaxSpendPeriod6P3.Label, dValue, 2 ); }

	double GetOverSpendP3()						{ return CSSDatabase::GetDouble ( SmartPay::OverSpendP3.Label ); }
	void SetOverSpendP3(double dValue)			{ CSSDatabase::Set ( SmartPay::OverSpendP3.Label, dValue, 2 ); }
	double GetOverSpendPeriod1P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendP3.Label ); }
	void SetOverSpendPeriod1P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendP3.Label, dValue, 2 ); }

	double GetOverSpendPeriod2P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod2P3.Label ); }
	void SetOverSpendPeriod2P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod2P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod3P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod3P3.Label ); }
	void SetOverSpendPeriod3P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod3P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod4P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod4P3.Label ); }
	void SetOverSpendPeriod4P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod4P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod5P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod5P3.Label ); }
	void SetOverSpendPeriod5P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod5P3.Label, dValue, 2 ); }

	double GetOverSpendPeriod6P3()				{ return CSSDatabase::GetDouble ( SmartPay::OverSpendPeriod6P3.Label ); }
	void SetOverSpendPeriod6P3(double dValue)	{ CSSDatabase::Set ( SmartPay::OverSpendPeriod6P3.Label, dValue, 2 ); }
	
//** Alert

	void SetUserAlert ( int nAlertCode );
	bool IsAlertSet() { return ( GetAlertCode() == 0 ) ? FALSE : TRUE; }

	void SetAlertCode ( int nCode );
	int GetAlertCode() { return (int)CSSDatabase::GetInt ( SmartPay::AlertCode.Label ); }

	CString GetAlertDate() { return(CString)CSSDatabase::GetString ( SmartPay::AlertDate.Label ); }
	void SetAlertDate ( const char* szDate ) { SetDateField ( SmartPay::AlertDate.Label, szDate ); }

	CString GetAlertTime() { return (CString)CSSDatabase::GetString ( SmartPay::AlertTime.Label ); }
	void SetAlertTime ( const char* szTime ) { SetTimeField ( SmartPay::AlertTime.Label, szTime ); }

//

	CString GetInfo ( int n );
	CString GetInfo1() { return (CString)CSSDatabase::GetString ( SmartPay::Info1.Label ); }
	CString GetInfo2() { return (CString)CSSDatabase::GetString ( SmartPay::Info2.Label ); }
	CString GetInfo3() { return (CString)CSSDatabase::GetString ( SmartPay::Info3.Label ); }
	CString GetInfo4() { return (CString)CSSDatabase::GetString ( SmartPay::Info4.Label ); }
	CString GetInfo5() { return (CString)CSSDatabase::GetString ( SmartPay::Info5.Label ); }
	CString GetInfo6() { return (CString)CSSDatabase::GetString ( SmartPay::Info6.Label ); }
	CString GetInfo7() { return (CString)CSSDatabase::GetString ( SmartPay::Info7.Label ); }
	CString GetInfo8() { return (CString)CSSDatabase::GetString ( SmartPay::Info8.Label ); }
	void SetInfo ( int n, const char* szText );
	void SetInfo1 ( const char* szText );
	void SetInfo2 ( const char* szText );
	void SetInfo3 ( const char* szText );
	void SetInfo4 ( const char* szText );
	void SetInfo5 ( const char* szText );
	void SetInfo6 ( const char* szText );
	void SetInfo7 ( const char* szText );
	void SetInfo8 ( const char* szText );

	bool GetTick ( int n );
	bool GetTick1() { return CSSDatabase::GetBool ( SmartPay::Tick1.Label ); }
	bool GetTick2() { return CSSDatabase::GetBool ( SmartPay::Tick2.Label ); }
	bool GetTick3() { return CSSDatabase::GetBool ( SmartPay::Tick3.Label ); }
	bool GetTick4() { return CSSDatabase::GetBool ( SmartPay::Tick4.Label ); }
	void SetTick ( int n, bool bSet );
	void SetTick1 ( bool bSet );
	void SetTick2 ( bool bSet );
	void SetTick3 ( bool bSet );
	void SetTick4 ( bool bSet );

	void SetExpiry ( const char* szDate );
	CString GetExpiryDate() { return (CString)CSSDatabase::GetString ( SmartPay::Expiry.Label ); }
	bool HasUserExpired();

	CString GetDobDate() { return (CString)CSSDatabase::GetString ( SmartPay::Dob.Label ); }
	void SetDob ( const char* szDate );
	bool IsBirthdayToday();
	const char* GetBirthdayMonth();
	int GetAge ( bool bNextBDay = FALSE );
	CString GetAgeNextBirthday();
	const char* GetDOBDayofWeek();

	bool HaveAllergies() { return ( GetAllergyCodes() == 0 ) ? FALSE : TRUE; }
	void SetAllergyCodes ( int nFlags );
	int GetAllergyCodes() { return (int)CSSDatabase::GetInt ( SmartPay::AllergyCodes.Label ); }

	void SetInactive ( bool bFlag )	{ Set ( SmartPay::Inactive.Label, bFlag ); }
	bool GetInactive()				{ return CSSDatabase::GetBool ( SmartPay::Inactive.Label ); }

	CString GetDietary()  { return (CString)CSSDatabase::GetString ( SmartPay::Dietary.Label ); }
	void SetDietary ( const char* szText );

	void CopyGroupMaxSpendSettings( CSmartPayGroupDatabase* pGroupDatabase );

private:
	int OpenForSharing ( const char* szFilename, const char* szBackupFolder, int nMode );
	int ReOpenForSharing ( const char* szBackupFolder, int nMode );
	
private:
	CString MakeInuseFilename ( const char* szFullPath );

	bool IsStringEmpty ( const char* szText );
	void SetField ( const char* strLabel, const char* szText, int nMax );
	void SetField ( const char* szLabel, int nValue, int nMin, int nMax );
	void SetField ( const char* szLabel, __int64 nValue, __int64 nMin, __int64 nMax);
	void SetField ( const char* szLabel, double dValue, double dMin, double dMax );
	void SetDateField ( const char* szLabel, const char* szDate );
	void SetTimeField ( const char* szLabel, const char* szTime );

	int GetRefreshFlags()					{ return CSSDatabase::GetInt ( SmartPay::RefreshRef.Label); }
	void SetRefreshFlags ( int nFlags ) 	{ CSSDatabase::Set (SmartPay::RefreshRef.Label, nFlags); }

private:
	CString m_strDatabaseFilename;
	CString m_strInuseFilename;
	CString m_strKey;
	CSSFile m_fileInuse;
	CString m_strError;

private:
	CString m_strHeader;
	CString m_strBioHeader;
};

/**********************************************************************/
