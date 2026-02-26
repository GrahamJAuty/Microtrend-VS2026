#pragma once
//**********************************************************************
#define ACCOUNTS_TABLE_CURRENT 0
#define ACCOUNTS_TABLE_LEAVERS 1
//**********************************************************************

namespace SQLTableNames
{
	const char* const Accounts = "Accounts";
	const char* const AccountGroups = "AccountGroups";
	const char* const AuditPeriods = "AuditPeriods";
	const char* const AuditPendingExports = "AuditPendingExports";
	const char* const BiometricRegister = "BiometricRegister";
	const char* const BromComBalanceChanges = "BromComBalanceChanges";
	const char* const BromComItems = "BromComItems";
	const char* const BromComPendingOrders = "BromComPendingOrders";
	const char* const BromComPendingItems = "BromComPendingItems";
	const char* const BromComTransactions = "BromComTransactions";
	const char* const ClosingBalances = "ClosingBalances";
	const char* const DbVersion = "DbVersion";
	const char* const EposTerminals = "EposTerminals";
	const char* const ExternalAccounts = "ExternalAccounts";
	const char* const ImportFilenameCache = "ImportFilenameCache";
	const char* const Leavers = "Leavers";
	const char* const OperatorNames = "OperatorNames";
	const char* const PebbleConfig = "PebbleConfig";
	const char* const PebbleExportTransactions = "PebbleExportTrans";
	const char* const PebblePendingTransactions = "PebblePendingTrans";
	const char* const PebbleUnmatchedTransactions = "PebbleUnmatchedTrans";
	const char* const PebbleRecentTransactions = "PebbleRecentTrans";
	const char* const PebbleExportPayments = "PebbleExportPayments";
	const char* const PebblePaymentTypes = "PebblePaymentTypes";
	const char* const ChartwellExportTransactions = "ChartwellExportTrans";
	const char* const ChartwellExportPayments = "ChartwellExportPayments";
	const char* const PeriodRefresh = "PeriodRefresh";
	const char* const PluInfo = "PluInfo";
	const char* const PurchaseControlRules = "PurchaseControlRules";
	const char* const PurchaseControlStamps = "PurchaseControlStamps";
	const char* const UserTexts = "UserTexts";
};

//**********************************************************************

namespace DbVersion
{
	const recInt LineID = { "LineId", 1, 999999 };
	const recInt Version = { "Version", 1, 999999 };
	const recInt ExternalVersion1 = { "ExternalVersion1", 1, 999999 };
};

//**********************************************************************

//INDEX ON MEMBER ID
//INDEX ON INFO FIELDS
//INDEX ON EXTERNAL REFS 1 and 2
namespace Account
{
	const recInt LeaverYear = { "LeaverYear", 2000, 9999 };
	const recInt64	UserID = { "UserID", 0, 9999999999 }; // Biometric length = 6, MiFare.v1 length = 10, MiFare.v2 length = 6
	const recString	Username = { "Username", 0, 40 };
	const recString	Forename = { "Forename", 0, 40 };
	const recString	Surname = { "Surname", 0, 40 };
	const recString	DOB = { "DOB", 0, 10 };
	const recString	ExpiryDate = { "ExpiryDate", 0, 10 };
	const recString	Gender = { "Gender", 0, 1 };
	const recInt GroupNo = { "GroupNo", 1 , 99 };
	const recString DayGroups = { "DayGroups", 0, 20 };	 // max = 20 - "99,99,99,99,99,99,99" (SundayGroup,MondayGroup,,,,,SaturdayGroup"
	const recInt GroupShiftStatus = { "GroupShiftStatus", 0, 2 };
	const recString GroupShiftStartDate = { "GroupShiftStartDate", 0, 10 };
	const recString GroupShiftEndDate = { "GroupShiftEndDate", 0, 10 };
	const recInt GroupShiftFutureGroup = { "GroupShiftFutureGroup", 0, 99 };
	const recString	Email = { "Email", 0, 80 };
	const recString	Info1 = { "Info1", 0, 80 };
	const recString	Info2 = { "Info2", 0, 80 };
	const recString	Info3 = { "Info3", 0, 80 };
	const recString	Info4 = { "Info4", 0, 80 };
	const recString	Info5 = { "Info5", 0, 80 };
	const recString	Info6 = { "Info6", 0, 80 };
	const recString	Info7 = { "Info7", 0, 80 };
	const recString	Info8 = { "Info8", 0, 80 };
	const recBool Tick1 = { "Tick1" };
	const recBool Tick2 = { "Tick2" };
	const recBool Tick3 = { "Tick3" };
	const recBool Tick4 = { "Tick4" };
	const recInt AllergyCodes = { "AllergyCodes", 0, 16383 }; // binary bit flags 11111111111111 ( 14 x 1 )
	const recString	Dietary = { "Dietary", 0, 100 };
	const recBool Inactive = { "Inactive" };
	/*****/
	const recString	MemberID = { "MemberID", 0, 26 };
	const recString	ExternalRef1 = { "ExternalRef1", 0, 20 };
	const recString	ExternalRef2 = { "ExternalRef2", 0, 20 };
	const recInt64	PreviousUserID{ "PreviousUserID", 1, 9999999999 };				//
	const recInt64	NextUserID = { "NextUserID", 1, 9999999999 };
	const recString	PebblePurseID = { "PebblePurseID", 0, 26 };
	/*****/
	const recInt Purse1Allow = { "Purse1Allow",	0, 63 };			// Purse1 allowed Period1 - Period6
	const recDouble	Purse1Liability = { "Purse1Liability", -99999999.99, 99999999.99 };
	const recDouble	Purse1Credit = { "Purse1Credit", -99999999.99, 99999999.99 };
	const recString	Purse1LastSpendDate = { "Purse1LastSpendDate", 0, 10 };
	const recString	Purse1LastSpendTime = { "Purse1LastSpendTime", 0, 8 };
	const recDouble	Purse1LastSpend = { "Purse1LastSpend", 0.00, 99999999.99 };
	const recDouble	Purse1SpendToDate = { "Purse1SpendToDate", 0.00, 99999999.99 };
	/*****/
	const recInt Purse2Allow = { "Purse2Allow",	0, 63 };			// Purse2 allowed Period1 - Period6
	const recDouble	Purse2Balance = { "Purse2Balance", -99999999.99, 99999999.99 };
	const recString	Purse2LastSpendDate = { "Purse2LastSpendDate", 0, 10 };
	const recString	Purse2LastSpendTime = { "Purse2LastSpendTime", 0, 8 };
	const recDouble	Purse2LastSpend = { "Purse2LastSpend", 0.00, 99999999.99 };
	const recDouble	Purse2SpendToDate = { "Purse2SpendToDate", 0.00, 99999999.99 };
	const recInt Purse2RefreshFlags = { "Purse2RefreshFlags",	0, 254 };		
	const recString	Purse2RefreshedDate = { "Purse2RefreshedDate", 0, 10 };
	const recString	Purse2RefreshedTime = { "Purse2RefreshedTime", 0, 8 };
	/*****/
	const recInt Purse3Allow = { "Purse3Allow",	0, 63 };			// Purse3 allowed Period1 - Period6
	const recDouble	Purse3Liability = { "Purse3Liability", -99999999.99, 99999999.99 };
	const recDouble	Purse3Credit = { "Purse3Credit", -99999999.99, 99999999.99 };
	const recString	Purse3LastSpendDate = { "Purse3LastSpendDate", 0, 10 };
	const recString	Purse3LastSpendTime = { "Purse3LastSpendTime", 0, 8 };
	const recDouble	Purse3LastSpend = { "Purse3LastSpend", 0.00, 99999999.99 };
	const recDouble	Purse3SpendToDate = { "Purse3SpendToDate", 0.00, 99999999.99 };
	/*****/
	const recDouble	CashSpendToDate = { "CashSpendToDate", 0.00, 99999999.99 };
	/*****/
	const recInt Points = { "Points",-99999999, 99999999 };
	const recInt PointsToDate = { "PointsToDate",-99999999, 99999999 };
	/*****/
	const recBool OwnMaxSpend = { "OwnMaxSpend" };
	/*****/
	const recDouble	MaxSpendPurse1Period1 = { "MaxSpendPurse1Period1", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period2 = { "MaxSpendPurse1Period2", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period3 = { "MaxSpendPurse1Period3", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period4 = { "MaxSpendPurse1Period4", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period5 = { "MaxSpendPurse1Period5", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period6 = { "MaxSpendPurse1Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxSpendPurse3Period1 = { "MaxSpendPurse3Period1", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period2 = { "MaxSpendPurse3Period2", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period3 = { "MaxSpendPurse3Period3", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period4 = { "MaxSpendPurse3Period4", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period5 = { "MaxSpendPurse3Period5", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period6 = { "MaxSpendPurse3Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxOverdraftPurse1Period1 = { "MaxOverdraftPurse1Period1", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period2 = { "MaxOverdraftPurse1Period2", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period3 = { "MaxOverdraftPurse1Period3", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period4 = { "MaxOverdraftPurse1Period4", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period5 = { "MaxOverdraftPurse1Period5", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period6 = { "MaxOverdraftPurse1Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxOverdraftPurse3Period1 = { "MaxOverdraftPurse3Period1", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period2 = { "MaxOverdraftPurse3Period2", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period3 = { "MaxOverdraftPurse3Period3", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period4 = { "MaxOverdraftPurse3Period4", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period5 = { "MaxOverdraftPurse3Period5", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period6 = { "MaxOverdraftPurse3Period6", 0.00, 9999.99 };
	/*****/
	const recInt AlertCode = { "AlertCode", 0, 9 };
	const recString	AlertDate = { "AlertDate", 0, 8 };
	const recString	AlertTime = { "AlertTime", 0, 8 };
	/*****/
	const recInt64 RowVersion = { "RowVersion", 0, 0x7FFFFFFFFFFFFFFF };
	/*****/
	//Non database labels
	/*****/
	/*****/
	const recDouble	TotalSpendToDate = { "TotalSpendToDate", 0, 99999999.99 };
	const recDouble	Purse1Balance = { "Purse1Balance", 0, 99999999.99 };
	const recDouble	Purse3Balance = { "Purse3Balance", 0, 99999999.99 };
	const recString	ForenameSurname = { "Forename:Surname", 0, 0 };
	const recString	FullUserID = { "FullUserID", 0, 0 };
	const recString	GroupUserID = { "GroupNo:UserID", 0, 0 };
	const recString	GroupFullUserID = { "GroupNo:FullUserID", 0, 0 };
	const recString	GroupType = { "GroupType", 0 , 0 };
	const recString	GroupTypeUserID = { "GroupType:UserID", 0, 0 };
	const recString	GroupTypeFullUserID = { "GroupType:FullUserID", 0, 0 };

	const recString	AllergyText = { "AllergyText", 0, 0 };
	const recString	AlertText = { "AlertText", 0, 0 };
	const recString	DOBNextAge = { "DOBNextAge", 0, 0 }; 	// Age next birthday
	const recString	DOBDay = { "DOBBDay", 0, 0 };		// Next birthday Day of week 
	const recString	DeletedDate = { "DeletedDate", 0, 0 };	// delete header fields
	const recString	DeletedTime = { "DeletedTime", 0, 0 };
	const recString	Blank = { "<blank>", 0, 0 };
	const recString	Text = { "<text>", 0, 40 };

	const recString	Purse1 = { "Purse1", 0, 1 };
	const recString	Purse2 = { "Purse2", 0, 1 };
	const recString	Purse3 = { "Purse3", 0, 1 };
};

//**********************************************************************

namespace Group
{
	const recInt	GroupNo = { "GroupNo", 1, 99 };
	const recString GroupName = { "GroupName", 0, 20 };
	const recInt	UserType = { "UserType", 1, 99 };
	const recInt	SetNo = { "SetNo", 1, 9 };
	const recString ShiftGroups = { "ShiftGroups", 0, 29 };
	const recInt	PriceLevel = { "PriceLevel", 0, 9 };
	const recBool VatShift = { "VatShift" };						// 0-no shift, 1=yes ( AtoD / BtoE / CtoF )
	const recBool PurchaseControlFlag = { "PurchaseControlFlag" };
	/****/
	const recInt	RefreshType = { "RefreshType", 0, 7 };
	const recInt	RefreshFlags = { "RefreshFlags", 0, 1231 };	// if daily max = 127, if monthly max = 31. if yearly max = 1231 (100x12)+ 31
	const recDouble RefreshValue = { "RefreshValue", 0, 999.99 };
	const recBool RefreshAppend = { "RefreshAppend" };			// Refresh carry forward ( 0-refresh overwrite, 1=add to existing purse )
	const recInt	RefreshTypePeriod1 = { "RefreshTypePeriod1", 0, 4 };					// Daily, weekly, monthly, yearly
	const recInt	RefreshTypePeriod2 = { "RefreshTypePeriod2", 0, 4 };
	const recInt	RefreshTypePeriod3 = { "RefreshTypePeriod3", 0, 4 };
	const recInt	RefreshTypePeriod4 = { "RefreshTypePeriod4", 0, 4 };
	const recInt	RefreshTypePeriod5 = { "RefreshTypePeriod5", 0, 4 };
	const recInt	RefreshTypePeriod6 = { "RefreshTypePeriod6", 0, 4 };
	const recDouble RefreshValuePeriod1 = { "RefreshValuePeriod1", 0, 999.99 };
	const recDouble RefreshValuePeriod2 = { "RefreshValuePeriod2", 0, 999.99 };
	const recDouble RefreshValuePeriod3 = { "RefreshValuePeriod3", 0, 999.99 };
	const recDouble RefreshValuePeriod4 = { "RefreshValuePeriod4", 0, 999.99 };
	const recDouble RefreshValuePeriod5 = { "RefreshValuePeriod5", 0, 999.99 };
	const recDouble RefreshValuePeriod6 = { "RefreshValuePeriod6", 0, 999.99 };
	/****/
	const recInt	Purse1Allowed = { "Purse1Allow", 0, 63 };		// Purse1 allowed Period1 - Period6
	const recInt	Purse2Allowed = { "Purse2Allow", 0, 63 };		// Purse2 allowed Period1 - Period6
	const recInt	Purse3Allowed = { "Purse3Allow", 0, 63 };		// Purse3 allowed Period1 - Period6
	/****/
	const recInt MaxSpendType = { "MaxSpendType",	0, 8 };				// Purse1/Purse2 - Single Daily link / Record specific / Audit Period specific
	/****/
	const recDouble	MaxSpendPurse1Period1 = { "MaxSpendPurse1Period1", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period2 = { "MaxSpendPurse1Period2", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period3 = { "MaxSpendPurse1Period3", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period4 = { "MaxSpendPurse1Period4", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period5 = { "MaxSpendPurse1Period5", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse1Period6 = { "MaxSpendPurse1Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxSpendPurse3Period1 = { "MaxSpendPurse3Period1", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period2 = { "MaxSpendPurse3Period2", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period3 = { "MaxSpendPurse3Period3", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period4 = { "MaxSpendPurse3Period4", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period5 = { "MaxSpendPurse3Period5", 0.00, 9999.99 };
	const recDouble	MaxSpendPurse3Period6 = { "MaxSpendPurse3Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxOverdraftPurse1Period1 = { "MaxOverdraftPurse1Period1", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period2 = { "MaxOverdraftPurse1Period2", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period3 = { "MaxOverdraftPurse1Period3", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period4 = { "MaxOverdraftPurse1Period4", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period5 = { "MaxOverdraftPurse1Period5", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse1Period6 = { "MaxOverdraftPurse1Period6", 0.00, 9999.99 };
	/*****/
	const recDouble	MaxOverdraftPurse3Period1 = { "MaxOverdraftPurse3Period1", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period2 = { "MaxOverdraftPurse3Period2", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period3 = { "MaxOverdraftPurse3Period3", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period4 = { "MaxOverdraftPurse3Period4", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period5 = { "MaxOverdraftPurse3Period5", 0.00, 9999.99 };
	const recDouble	MaxOverdraftPurse3Period6 = { "MaxOverdraftPurse3Period6", 0.00, 9999.99 };
	/*****/
	const recString	DeptInhibitPurse1 = { "DeptInhibitPurse1", 0, 99 };			// Purse1 Inhibited departments 
	const recString	DeptInhibitPurse2 = { "DeptInhibitPurse2", 0, 99 };			// Purse2
	const recString	DeptInhibitPurse3 = { "DeptInhibitPurse3", 0, 99 };			// Purse3
	const recString	DeptInhibitCash = { "DeptInhibitCash", 0, 99 };			// None purse inhibited departments
	/*****/
	const recDouble	LowBalancePurse1 = { "LowBalancePurse1", 0, 999.99 };		// Low Balance Purse1 warning level
	const recDouble	LowBalancePurse2 = { "LowBalancePurse2", 0, 999.99 };		// Low Balance Purse2 warning level
	const recDouble	LowBalancePurse3 = { "LowBalancePurse3", 0, 999.99 };		// Low Balance Purse3 warning level
	/*****/
	const recInt PointsAchievement = { "PointsAchievement",	0, 999999 };		// Points achievement level
	/*****/
	const recBool RevalueFlag = { "RevalueFlag" };							// Group specific Revaluation
	const recBool RevalueDisable = { "RevalueDisable" };
	const recInt	RevalueType = { "RevalueType",	0, 1 };					// 0=Points, 1=Cash
	const recDouble RevalueTrip1 = { "RevalueTrip1",	0, 999999.99 };
	const recDouble RevalueTrip2 = { "RevalueTrip2",	0, 999999.99 };
	const recInt	RevaluePoints = { "RevaluePoints",	0, 999999 };
	const recDouble RevalueBonus = { "RevalueBonus", 0, 999999.99 };
	/*****/
	const recBool RewardFlag = { "RewardFlag" };						// Group Specific Reward
	const recBool RewardDisable = { "RewardDisable" };
	const recInt	RewardType = { "RewardType", 0, 1 };
	const recDouble RewardTrip = { "RewardTrip", 0, 999999.99 };
	const recInt	RewardPoints = { "RewardPoints", 0, 999999 };
	const recDouble RewardBonus = { "RewardBonus", 0, 999999.99 };
	const recDouble RewardLimit = { "RewardLimit", 0, 999999.99 };			// minimum spend
	const recDouble RewardTrigger = { "RewardTrigger", 0, 999999.99 };			// upper limit
	/*****/
	const recBool RedeemFlag = { "RedeemFlag" };						// Group Specific Redemption
	const recBool RedeemDisable = { "RedeemDisable" };
	const recInt	RedeemTrip = { "RedeemTrip", 0, 999999 };
	const recDouble RedeemValue = { "RedeemValue", 0, 999999.99 };
	const recInt	RedeemPurse = { "RedeemPurse", 0, 3 };
};

//**********************************************************************

namespace ExternalAccount
{
	const recString	Username = { "Column1", 0, 100 };
	const recString	Password = { "Column2", 0, 50 };
	const recString	UserID = { "Column3", 0, 100 };
};

//**********************************************************************

namespace PeriodRefresh
{
	const recInt64	UserID = { "UserID", 1, 9999999999 };
	const recString	Period1Date = { "Period1Date", 0, 10 };
	const recDouble	Period1Balance = { "Period1Balance", -99999999.99, 99999999.99 };
	const recString	Period2Date = { "Period2Date", 0, 10 };
	const recDouble	Period2Balance = { "Period2Balance", -99999999.99, 99999999.99 };
	const recString	Period3Date = { "Period3Date", 0, 10 };
	const recDouble	Period3Balance = { "Period3Balance", -99999999.99, 99999999.99 };
	const recString	Period4Date = { "Period4Date", 0, 10 };
	const recDouble	Period4Balance = { "Period4Balance", -99999999.99, 99999999.99 };
	const recString	Period5Date = { "Period5Date", 0, 10 };
	const recDouble	Period5Balance = { "Period5Balance", -99999999.99, 99999999.99 };
	const recString	Period6Date = { "Period6Date", 0, 10 };
	const recDouble	Period6Balance = { "Period6Balance", -99999999.99, 99999999.99 };
};

//**********************************************************************

namespace BioRegister
{
	const recInt64	UserID = { "UserID", 1, 9999999999 };
	const recString	Username = { "Username", 0, 40 };
	const recInt64	Counter = { "Counter", 1, 9999999999 };
	const recString	RegDate = { "RegDate", 0, 8 };
	const recString	RegTime = { "RegTime", 0, 8 };
	const recString	UnRegDate = { "UnRegDate", 0, 8 };
	const recString	UnRegTime = { "UnRegTime", 0, 8 };
	const recBool UnRegFlag = { "UnRegFlag" };
	const recString	FirstDate = { "FirstDate", 0, 8 };
	const recString	FirstTime = { "FirstTime", 0, 8 };
	const recString	LastDate = { "LastDate", 0, 8 };
	const recString	LastTime = { "LastTime", 0, 8 };
};

//**********************************************************************

namespace PluInfo
{
	const recInt64 PluNo = { "PluNo", 1, 99999999999999 };
	const recString	Description = { "Description", 0, 40 };
	const recInt DeptNo = { "DeptNo", 0, 999 };
	const recString	TaxCode = { "TaxCode", 0, 1 };
	const recInt ModType = { "ModType", 0, 3 };
	const recInt Points = { "Points", 0, 99999 };
	const recInt PCtrlRuleID = { "PCtrlRuleID", 0, 65535 };
	const recInt StampWeight = { "StampWeight", 1, 999999 };
};

//**********************************************************************

namespace AuditPeriod
{
	const recInt PeriodID = { "PeriodID", 0, 5 };
	const recString	PeriodName = { "Name", 0, 15 };
	const recInt StartTime = { "StartTime", 0, 1439 };
};

//**********************************************************************

namespace UserText
{
	const recString FieldLabel = { "FieldLabel", 0, 50 };
	const recString	UserText = { "UserText", 0, 50 };
	const recBool UseFlag = { "UseFlag" };
};

//**********************************************************************

namespace ImportLocate
{
	const recString	MemberID = { "MemberID", 0, 100 };
	const recInt64	UserID = { "UserID", 0, 9999999999 };
};

//**********************************************************************

namespace ClosingBalance
{
	const recString	KeyDate = { "KeyDate", 0, 8 };
	const recString	Date = { "Date", 0, 10 };
	const recString	Time = { "Time", 0, 5 };
	const recString	ArchiveDate = { "ArchiveDate", 0, 10 };
	const recString	ArchiveTime = { "ArchiveTime", 0, 5 };
	const recDouble	Purse1LiabilityAudit = { "Purse1Liability", -99999999.99, 99999999.99 };
	const recDouble	Purse1CreditAudit = { "Purse1Credit", -99999999.99, 99999999.99 };
	const recDouble	Purse2BalanceAudit = { "Purse2Balance", -99999999.99, 99999999.99 };
	const recDouble	Purse3LiabilityAudit = { "Purse3Liability", -99999999.99, 99999999.99 };
	const recDouble	Purse3CreditAudit = { "Purse3Credit", -99999999.99, 99999999.99 };
	const recBool HaveAdjust = { "HaveAdjust" };
	const recDouble	Purse1LiabilityAdjust = { "Purse1LiabilityAdjust", -99999999.99, 99999999.99 };
	const recDouble	Purse1CreditAdjust = { "Purse1CreditAdjust", -99999999.99, 99999999.99 };
	const recDouble	Purse3LiabilityAdjust = { "Purse3LiabilityAdjust", -99999999.99, 99999999.99 };
	const recDouble	Purse3CreditAdjust = { "Purse3CreditAdjust", -99999999.99, 99999999.99 };
};

//**********************************************************************

namespace BromComPendingOrder
{
	const recInt OrderID = { "OrderID", 1, 0x7FFFFFFF };
	const recString Status = { "Status", 0, 30 };
	const recString LastModified = { "LastModified", 0, 50 };
	const recBool NewOrder = { "NewOrder" };
};

//**********************************************************************

namespace BromComPendingItem
{
	const recInt OrderID = { "OrderID", 1, 0x7FFFFFFF };
	const recInt PersonID = { "PersonID", 1, 0x7FFFFFFF };
	const recDouble Amount = { "Amount", 0, 99999999.99 };
};

//**********************************************************************

namespace BromComItem
{
	const recInt OrderID = { "OrderID", 1, 0x7FFFFFFF };
	const recInt PersonID = { "PersonID", 1, 0x7FFFFFFF };
	const recDouble Amount = { "Amount", 0, 99999999.99 };
	const recString Status = { "Status", 0, 30 };
};

//**********************************************************************

namespace BromComBalanceChanges
{
	const recInt PersonID = { "PersonID", 1, 0x7FFFFFFF };
	const recInt64 AccountID = { "AccountID", 1, 9999999999 };
};

//**********************************************************************

namespace BromComTransaction
{
	const recInt64 LineID = { "LineID", 1, 0x7FFFFFFFFFFFFFFF };
	const recInt PersonID = { "PersonID", 1, 0x7FFFFFFF };
	const recString Details = { "Details", 0, 100 };
	const recDouble Amount = { "Amount", -99999999.99, 99999999.99 };
	const recDouble Balance = { "Balance", -99999999.99, 99999999.99 };
	const recString DateTime = { "DateTime", 0, 30 };
	const recString Type = { "Type", 0, 30 };
	const recInt TransactionID = { "TransactionID", 0, 0x7FFFFFFF };
	const recString WebComment = { "WebComment", 0, 500 };
};

//**********************************************************************

namespace PurchaseControlRule
{
	const recInt RuleID = { "RuleID", 1, 10 };
	const recBool Enabled = { "Enabled" };
	const recInt Threshold = { "Threshold", 1, 999999 };
	const recInt Expiry = { "Expiry", 0, 999 };
	const recString Description = { "Description", 0, 50 };
};

//**********************************************************************

namespace PurchaseControlStamp
{
	const recInt64	UserID = { "UserID", 1, 999999999999 };
	const recInt RuleID = { "RuleID", 1, 10 };
	const recString	AwardDate = { "AwardDate", 0, 8 };
	const recString	ExpireDate = { "ExpireDate", 0, 8 };
	const recInt StampCount = { "StampCount", 0, 999999 };
};

//**********************************************************************

namespace ImportFilenameCache
{
	const recInt64 LineID = { "LineID", 1, 0x7FFFFFFFFFFFFFFF };
	const recInt Type = { "Type", 0, 1 };
	const recString Filename = { "Filename", 0, 200 };
	const recString DateTime = { "DateTime", 0, 14 };
};

//**********************************************************************

namespace PebbleConfig
{
	const recInt LineID = { "LineID", 1, 1 };
	const recString InstallationID = { "InstallationID", 0, 26 };
	const recString OrganisationID = { "OrganisationID", 0, 26 };
	const recInt MISInfoIndex = { "MISInfoIndex", 0, 8 };
	const recInt UPNInfoIndex = { "UPNInfoIndex", 0, 8 };
	const recString LastImportDateTopup = { "LastImportDateTopup", 0, 8 };
	const recString LastImportTimeTopup = { "LastImportTimeTopup", 0, 6 };
	const recString LastImportDateSale = { "LastImportDateSale", 0, 8 };
	const recString LastImportTimeSale = { "LastImportTimeSale", 0, 6 };
	const recBool FirstImportTopup{ "FirstImportTopup" };
	const recBool FirstImportSale{ "FirstImportSale" };
	const recInt RecentCacheDays = { "RecentCacheDays", 0, 999 };
	const recInt PaymentCacheDays = { "PaymentCacheDays", 0, 999 };
	const recDouble	VAT_A = { "VAT_A", 0, 9999999.9999 };
	const recDouble	VAT_B = { "VAT_B", 0, 9999999.9999 };
	const recDouble	VAT_C = { "VAT_C", 0, 9999999.9999 };
	const recDouble	VAT_D = { "VAT_D", 0, 9999999.9999 };
	const recDouble	VAT_E = { "VAT_E", 0, 9999999.9999 };
	const recDouble	VAT_F = { "VAT_F", 0, 9999999.9999 };
	const recDouble	VAT_G = { "VAT_G", 0, 9999999.9999 };
	const recDouble	VAT_H = { "VAT_H", 0, 9999999.9999 };
	const recDouble	VAT_I = { "VAT_I", 0, 9999999.9999 };
	const recDouble	VAT_J = { "VAT_J", 0, 9999999.9999 };
	const recDouble	VAT_K = { "VAT_K", 0, 9999999.9999 };
	const recDouble	VAT_L = { "VAT_L", 0, 9999999.9999 };
	const recInt64 OfficeMealPluNo = { "OfficeMealPluNo", 1, 99999999999999 };
	const recInt OfficeMealVATBand = { "OfficeMealVATBand", 1, 12  };
};

//**********************************************************************

namespace PebbleExportTransaction
{
	const recInt64 LineID = { "LineID", 1, 0x7FFFFFFFFFFFFFFF };
	const recInt64 UserID = { "UserID", 0, 9999999999 }; 
	const recString MemberID = { "MemberID", 0, 26 };
	const recString Date = { "Date", 0, 20 };
	const recString Time = { "Time", 0, 20 };
	const recInt CCNo = { "CCNo", 0, 99999999 };
	const recInt SourceType = { "SourceType", 0, 20 };
	const recInt AppNo = { "AppNo", 0, 200 };
	const recDouble Amount = { "Amount", -999999999, 999999999 };
	const recDouble Purse1 = { "Purse1", -999999999, 999999999 };
	const recDouble Purse2 = { "Purse2", -999999999, 999999999 };
	const recDouble Cash = { "Cash", -999999999, 999999999 };
	const recString WebComment = { "WebComment", 0, 500 };
};

//**********************************************************************

namespace PebblePendingTransaction
{
	const recString TransactionID = { "TransactionID", 0, 26 };
	const recString MemberID = { "MemberID", 0, 26 };
	const recString DateTime = { "DateTime", 0, 14 };
	const recString Name = { "Name", 0, 100 };
	const recInt Amount = { "Amount", -999999999, 999999999 };
	const recBool IsSale = { "IsSale" };
};

//**********************************************************************

namespace PebbleRecentTransaction
{
	const recString TransactionID = { "TransactionID", 0, 26 };
	const recString DateTime = { "DateTime", 0, 14 };
};

//**********************************************************************

namespace PebbleExportPayment
{
	const recInt64 UserID = { "UserID", 0, 9999999999 };
	const recString Date = { "Date", 0, 20 };
	const recString Time = { "Time", 0, 20 };
	const recString CreateDateTime = { "CreateDateTime", 0, 14 };
	const recInt CCNo = { "CCNo", 0, 99999999 };
	const recInt PaymentType1 = { "PaymentType1", -1, 99 };
	const recDouble PaymentAmount1 = { "PaymentAmount1", -999999999, 999999999 };
	const recInt PaymentType2 = { "PaymentType2", -1, 99 };
	const recDouble PaymentAmount2 = { "PaymentAmount2", -999999999, 999999999 };
	const recInt PaymentType3 = { "PaymentType3", -1, 99 };
	const recDouble PaymentAmount3 = { "PaymentAmount3", -999999999, 999999999 };
	const recInt PaymentType4 = { "PaymentType4", -1, 99 };
	const recDouble PaymentAmount4 = { "PaymentAmount4", -999999999, 999999999 };
	const recInt PaymentType5 = { "PaymentType5", -1, 99 };
	const recDouble PaymentAmount5 = { "PaymentAmount5", -999999999, 999999999 };
}

//**********************************************************************

namespace AuditPendingExport
{
	const recInt64 LineID = { "LineID", 1, 0x7FFFFFFFFFFFFFFF };
	const recString UserID = { "UserID", 0, 30 };
	const recString UserName = { "UserName", 0, 50 };
	const recInt GroupNo = { "GroupNo", 0 , 99 };
	const recString Date = { "Date", 0 , 20 };
	const recString Time = { "Time", 0 , 20 };
	const recInt ActionType = { "Action", 0 , 10 };
	const recInt SourceType = { "SourceType", 0 , 20 };
	const recInt AppNo = { "AppNo", 0 , 200 };
	const recInt TerminalNo = { "TerminalNo", 0 , 9999 };
	const recInt CCNo = { "CCNo", 0 , 99999999 };
	const recString Text = { "Text", 0 , 200 };
	const recInt PointsAdded = { "PointsAdded", -99999999, 99999999 };
	const recInt PointsBalance = { "PointsBalance", -99999999, 99999999 };
	const recDouble Purse1Value = { "Purse1Value", -99999999.99, 99999999.99 };
	const recDouble Purse2Value = { "Purse2Value", -99999999.99, 99999999.99 };
	const recDouble Purse3Value = { "Purse3Value", -99999999.99, 99999999.99 };
	const recDouble CashValue = { "CashValue", -99999999.99, 99999999.99 };
	const recDouble Purse1Balance = { "Purse1Balance", -99999999.99, 99999999.99 };
	const recDouble Purse2Balance = { "Purse2Balance", -99999999.99, 99999999.99 };
	const recDouble Purse3Balance = { "Purse3Balance", -99999999.99, 99999999.99 };
	const recString Notes = { "Notes", 0 , 200 };
}

//**********************************************************************

namespace PebblePaymentType
{
	const recInt SPOSPaymentType = { "SPOSPaymentType", 0, 99 };
	const recString PebblePaymentName = { "PebblePaymentName", 0, 30 };
	const recInt PebbleTranType = { "PebbleTranType", 0, 2 };
};

//**********************************************************************

namespace OperatorName
{
	const recInt OperatorNo = { "OperatorNo", 1, 999 };
	const recString OperatorName = { "OperatorName", 0, 100 };
};

//**********************************************************************

namespace EposTerminal
{
	const recInt TerminalNo = { "TerminalNo", 1, 99 };
	const recInt TerminalType = { "TerminalType", 0, 1 };
	const recString TerminalName = { "TerminalName", 0, 100 };
	const recString LastUsedDate = { "LastUsedDate", 0, 10 };
	const recString LastUsedTime = { "LastUsedTime", 0, 5 };
	const recInt CCNo = { "CCNo", 0, 99999999 };
	const recInt64	UserID = { "UserID", -1, 9999999999 }; 
	const recDouble CardValue = { "CardValue", -99999999.99, 99999999.99 };
	const recInt TotalCounter = { "TotalCounter", 0, 99999999 };
	const recDouble TotalInDrawer = { "TotalInDrawer", -99999999.99, 99999999.99 };
	const recBool Disabled = { "Disabled" };
	const recBool TerminalOn = { "TerminalOn" };
	const recBool InService = { "InService" };
	const recBool DoorOpen = { "DoorOpen" };
};

//**********************************************************************

namespace ChartwellExportPayment
{
	const recInt64 UserID = { "UserID", 0, 9999999999 };
	const recInt64 TransactionID = { "TransactionID", 0, 999999999999 };
	const recInt TerminalNo = { "TerminalNo", 0, 999 };
	const recString Date = { "Date", 0, 20 };
	const recString CreateDateTime = { "CreateDateTime", 0, 14 };
	const recInt GroupNo = { "GroupNo", 0, 999 };
	const recDouble PaymentPurse1 = { "PaymentPurse1", -999999999, 999999999 };
	const recDouble PaymentPurse2 = { "PaymentPurse2", -999999999, 999999999 };
	const recDouble PaymentPurse3 = { "PaymentPurse3", -999999999, 999999999 };
}

//**********************************************************************
