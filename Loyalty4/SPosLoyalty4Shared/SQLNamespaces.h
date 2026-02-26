#pragma once
//**********************************************************************

namespace SQLTableNames
{
	const char* const Accounts = "Accounts";
	const char* const AccountGroups = "AccountGroups";
	const char* const GroupTopupBonuses = "GroupTopupBonuses";
	const char* const AuditPeriods = "AuditPeriods";
	const char* const CCNo = "CCNo";
	const char* const DbVersion = "DbVersion";
	const char* const DeptInfo = "DeptInfo";
	const char* const EcrmanDatabases = "EcrmanDatabase";
	const char* const ExternalAccounts = "ExternalAccounts";
	const char* const OfferStamps = "OfferStamps";
	const char* const PluInfo = "PluInfo";
	const char* const StaffCards = "StaffCards";
	const char* const StaffGiftExceptions = "StaffGiftExceptions";
	const char* const Schemes = "LoyaltySchemes";
	const char* const StampOfferInfo = "StampOfferInfo";
};

//**********************************************************************

namespace DbVersion
{
	const recInt LineID = { "LineId", 1, 999999 };
	const recInt Version = { "Version", 1, 999999 };
};

//**********************************************************************

namespace Account
{
	const recInt64	UserID = { "UserID", 1, 999999999999 };
	const recString	Fullname = { "Fullname", 0, 40 };
	const recString	Forename = { "Forename", 0, 40 };
	const recString	Surname = { "Surname", 0, 40 };
	const recString	DOB = { "DOB", 0, 10 };
	const recString	ExpiryDate = { "ExpiryDate", 0, 10 };
	const recString	Gender = { "Gender", 0, 1 };
	const recInt GroupNo = { "GroupNo", 1 , 99 };
	const recInt SchemeNo = { "SchemeNo", 0, 99 };
	const recString	Info1 = { "Info1", 0, 80 };
	const recString	Info2 = { "Info2", 0, 80 };
	const recString	Info3 = { "Info3", 0, 80 };
	const recString	Info4 = { "Info4", 0, 80 };
	const recString	Address1 = { "Address1", 0, 40 };
	const recString	Address2 = { "Address2", 0, 40 };
	const recString	Address3 = { "Address3", 0, 40 };
	const recString	Address4 = { "Address4", 0, 40 };
	const recString	Address5 = { "Address5", 0, 40 };
	const recString	Phone1 = { "Phone1", 0, 40 };
	const recString	Phone2 = { "Phone2", 0, 40 };
	const recBool Inactive = { "Inactive" };
	/*****/
	const recString	ExternalRef = { "ExternalRef", 0, 20 };
	const recString	QRInfo = { "QRInfo", 0, 256 };
	const recInt64	PreviousUserID{ "PreviousUserID", 1, 999999999999 };
	const recInt64	NextUserID = { "NextUserID", 1, 999999999999 };
	/*****/
	const recDouble	Purse1Balance = { "Purse1Balance", -99999999.99, 99999999.99 };
	const recString	Purse1LastSpendDate = { "Purse1LastSpendDate", 0, 10 };
	const recString	Purse1LastSpendTime = { "Purse1LastSpendTime", 0, 8 };
	const recDouble	Purse1LastSpend = { "Purse1LastSpend", -99999999.99, 99999999.99 };
	const recDouble	Purse1SpendToDate = { "Purse1SpendToDate", -99999999.99, 99999999.99 };
	/*****/
	const recDouble	Purse2Balance = { "Purse2Balance", -99999999.99, 99999999.99 };
	const recString	Purse2LastSpendDate = { "Purse2LastSpendDate", 0, 10 };
	const recString	Purse2LastSpendTime = { "Purse2LastSpendTime", 0, 8 };
	const recDouble	Purse2LastSpend = { "Purse2LastSpend", -99999999.99, 99999999.99 };
	const recDouble	Purse2SpendToDate = { "Purse2SpendToDate", -99999999.99, 99999999.99 };
	const recString	Purse2RefreshedDate = { "Purse2RefreshedDate", 0, 10 };
	const recString	Purse2RefreshedTime = { "Purse2RefreshedTime", 0, 8 };
	/*****/
	const recString	CashLastSpendDate = { "CashLastSpendDate", 0, 10 };
	const recString	CashLastSpendTime = { "CashLastSpendTime", 0, 8 };
	const recDouble	CashLastSpend = { "CashLastSpend", -99999999.99, 99999999.99 };
	const recDouble	CashSpendToDate = { "CashSpendToDate", -99999999.99, 99999999.99 };
	/*****/
	const recInt Points = { "Points",-99999999, 99999999 };
	const recInt PointsToDate = { "PointsToDate",-99999999, 99999999 };
	/*****/
	const recInt HotlistCode = { "HotlistCode", 0, 9 };
	const recString	HotlistDate = { "HotlistDate", 0, 10 };
	const recString	HotlistTime = { "HotlistTime", 0, 10 };
	/*****/
	const recDouble	MaxSpend = { "MaxSpend", 0.00, 99999.99 };
	const recDouble	MaxOverdraft = { "MaxOverdraft", 0.00, 99999.99 };
	/*****/
	const recString AlertCodes = { "AlertCodes", 0, 17 };
	/*****/
	const recInt64 RowVersion = { "RowVersion", 0, 0x7FFFFFFFFFFFFFFF };
	/*****/
	//Non database labels
	/*****/
	const recString	Blank = { "<blank>", 0, 0 };
	const recString	Text = { "<text>", 0, 40 };
	const recString	DOBCheck = { "<Anniversary>", 0, 40 };
	const recString	AlertCheck = { "<Alert>", 0, 40 };
	const recString	BonusCheck = { "<Bonus>", 0, 40 };
	const recString	SkipCheck = { "<Skip>", 0, 40 };
	/*****/
	const recDouble	TotalSpendToDate = { "TotalSpendToDate", 0, 99999999.99 };
	const recString	FullUserID = { "FullUserID", 0, 0 };
	const recString	GroupUserID = { "GroupNo:UserID", 0, 0 };
	const recString	GroupFullUserID = { "GroupNo:FullUserID", 0, 0 };
	const recString	DOBNextAge = { "DOBNextAge", 0, 0 }; 	// Age next birthday
	const recString	DOBDay = { "DOBDay", 0, 0 };		// Next birthday Day of week 
	const recString	DeletedDate = { "DeletedDate", 0, 0 };	// delete header fields
	const recString	DeletedTime = { "DeletedTime", 0, 0 };
	const recString	AlertText = { "AlertText", 0, 0 };
	const recString	CardNameWithBalance = { "FullName+Balance", 0, 0 };
	const recString	Balance = { "Balance", 0, 0 };
	const recString	StampOffers = { "Stamp Offer Info", 0, 0 };
	const recString	AvailableSpend = { "AvailableSpend", 0, 0 };
	/*****/
};

//**********************************************************************

namespace Group
{
	const recInt GroupNo = { "GroupNo", 1, 99 };
	const recString GroupName = { "GroupName", 0, 40 };
	const recInt SchemeNo = { "SchemeNo", 0, 99 };
	const recBool StampOfferFlag = { "StampOfferFlag" };
	/****/
	const recInt RefreshType = { "RefreshType", 0, 5 };
	const recInt RefreshFlags = { "RefreshFlags", 0, 1231 };	// if daily max = 127, if monthly max = 31. if yearly max = 1231 (100x12)+ 31
	const recDouble RefreshValue = { "RefreshValue", 0, 999.99 };
	const recInt RefreshAppend = { "RefreshAppend", 0, 1 };			// Refresh carry forward ( 0-refresh overwrite, 1=add to existing purse )
	/****/
	const recInt Purse1Allowed = { "Purse1Allow", 0, 63 };		// Purse1 allowed Period1 - Period6
	const recInt Purse2Allowed = { "Purse2Allow", 0, 63 };		// Purse2 allowed Period1 - Period6
	/****/
	const recBool NoPluBonus = { "PointsDisable" };	
	const recBool NoDeptBonus = { "NoDeptBonus" };
	/****/
	const recInt MaxSpendType = { "MaxSpendType",	0, 2 };				// Purse1/Purse2 - Single Daily link / Record specific / Audit Period specific
	/****/
	const recDouble	MaxSpendPeriod1 = { "MaxSpendPeriod1", 0.00, 99999.99 };
	const recDouble	MaxSpendPeriod2 = { "MaxSpendPeriod2", 0.00, 99999.99 };
	const recDouble	MaxSpendPeriod3 = { "MaxSpendPeriod3", 0.00, 99999.99 };
	const recDouble	MaxSpendPeriod4 = { "MaxSpendPeriod4", 0.00, 99999.99 };
	const recDouble	MaxSpendPeriod5 = { "MaxSpendPeriod5", 0.00, 99999.99 };
	const recDouble	MaxSpendPeriod6 = { "MaxSpendPeriod6", 0.00, 99999.99 };
	/*****/
	const recDouble	MaxOverdraftPeriod1 = { "MaxOverdraftPeriod1", 0.00, 99999.99 };
	const recDouble	MaxOverdraftPeriod2 = { "MaxOverdraftPeriod2", 0.00, 99999.99 };
	const recDouble	MaxOverdraftPeriod3 = { "MaxOverdraftPeriod3", 0.00, 99999.99 };
	const recDouble	MaxOverdraftPeriod4 = { "MaxOverdraftPeriod4", 0.00, 99999.99 };
	const recDouble	MaxOverdraftPeriod5 = { "MaxOverdraftPeriod5", 0.00, 99999.99 };
	const recDouble	MaxOverdraftPeriod6 = { "MaxOverdraftPeriod6", 0.00, 99999.99 };
	/*****/
	const recBool LegacyRevalueFlag1 = { "RevalueFlag" };							// Group specific Revaluation
	const recBool LegacyRevalueDisable1 = { "RevalueDisable" };
	const recDouble LegacyRevalueTrip1 = { "RevalueTrip1", 0, 999999.99 };
	const recInt LegacyRevalueType1 = { "RevalueType", 0, 3 };					// 0=Points, 1=Cash
	const recInt LegacyRevaluePoints1 = { "RevaluePoints", 0, 999999 };
	const recDouble LegacyRevalueBonus1 = { "RevalueBonus", 0, 999999.99 };
	const recInt LegacyRevaluePercent1 = { "RevaluePercent", 0, 100 };
	/*****/
	const recBool LegacyRevalueFlag2 = { "RevalueFlag2" };
	const recBool LegacyRevalueDisable2 = { "RevalueDisable2" };
	const recDouble LegacyRevalueTrip2 = { "RevalueTrip2", 0, 999999.99 };
	const recInt LegacyRevalueType2 = { "RevalueType2", 0, 3 };
	const recInt LegacyRevaluePoints2 = { "RevaluePoints2", 0, 999999 };
	const recDouble LegacyRevalueBonus2 = { "RevalueBonus2", 0, 999999.99 };
	const recInt LegacyRevaluePercent2 = { "RevaluePercent2", 0, 100 };
	/*****/
	const recBool LegacyRevalueFlag3 = { "RevalueFlag3" };
	const recBool LegacyRevalueDisable3 = { "RevalueDisable3" };
	const recDouble LegacyRevalueTrip3 = { "RevalueTrip3", 0, 999999.99 };
	const recInt LegacyRevalueType3 = { "RevalueType3", 0, 3 };
	const recInt LegacyRevaluePoints3 = { "RevaluePoints3", 0, 999999 };
	const recDouble LegacyRevalueBonus3 = { "RevalueBonus3", 0, 999999.99 };
	const recInt LegacyRevaluePercent3 = { "RevaluePercent3", 0, 100 };
	/*****/
	const recBool LegacyRevalueFlag4 = { "RevalueFlag4" };
	const recBool LegacyRevalueDisable4 = { "RevalueDisable4" };
	const recDouble LegacyRevalueTrip4 = { "RevalueTrip4", 0, 999999.99 };
	const recInt LegacyRevalueType4 = { "RevalueType4", 0, 3 };
	const recInt LegacyRevaluePoints4 = { "RevaluePoints4", 0, 999999 };
	const recDouble LegacyRevalueBonus4 = { "RevalueBonus4", 0, 999999.99 };
	const recInt LegacyRevaluePercent4 = { "RevaluePercent4", 0, 100 };
	/*****/
	const recBool RewardFlag = { "RewardFlag" };						// Group Specific Reward
	const recBool RewardDisable = { "RewardDisable" };
	const recInt RewardType = { "RewardType", 0, 2 };
	const recDouble RewardTrip = { "RewardTrip", 0, 999999.99 };
	const recInt RewardPoints = { "RewardPoints", 0, 999999 };
	const recDouble RewardBonus = { "RewardBonus", 0, 999999.99 };
	const recDouble RewardLimit = { "RewardLimit", 0, 999999.99 };			// minimum spend
	const recDouble RewardTrigger = { "RewardTrigger", 0, 999999.99 };			// upper limit
	/*****/
	const recBool RedeemFlag = { "RedeemFlag" };						// Group Specific Redemption
	const recBool RedeemDisable = { "RedeemDisable" };
	const recInt RedeemType = { "RedeemType", 0, 1 };
	const recDouble RedeemTripAsDouble = { "RedeemTrip", 0, 999999.99 };
	const recInt RedeemTripAsInt = { "RedeemTrip", 0, 999999 };
	const recDouble RedeemValue = { "RedeemValue", 0, 999999.99 };
};

//**********************************************************************

namespace GroupTopupBonus
{
	const recInt GroupNo = { "GroupNo", 1, 99 };
	const recInt PurseNo = { "PurseNo", 1, 2 };
	const recInt TopupNo = { "TopupNo", 1, 5 };
	const recBool BonusFlag = { "BonusFlag" };						// Group specific bonus
	const recBool BonusDisable = { "BonusDisable" };
	const recDouble BonusTrip = { "BonusTrip", 0, 999999.99 };
	const recInt BonusType = { "BonusType", 0, 3 };					// 0=Points, 1=Cash
	const recInt BonusPoints = { "BonusPoints", 0, 999999 };
	const recDouble BonusValue = { "BonusValue", 0, 999999.99 };
	const recInt BonusPercent = { "BonusPercent", 0, 100 };
};

//**********************************************************************

namespace ExternalAccount
{
	const recString	Username = { "Column1", 0, 200 };
	const recString	Password = { "Column2", 0, 100 };
	const recString	UserID = { "Column3", 0, 200 };
};

//**********************************************************************

namespace PluInfo
{
	const recInt DbNo = { "DbNo", 1, 10 };
	const recInt64 PluNo = { "PluNo", 1, 99999999999999 };
	const recString	Description = { "Description", 0, 40 };
	const recInt DeptNo = { "DeptNo", 0, 999 };
	const recString	TaxCode = { "TaxCode", 0, 1 };
	const recInt Points = { "Points", 0, 99999 };
	const recInt ModType = { "ModType", 0, 3 };
	const recInt StampOfferID = { "StampOfferID", 0, 65535 };
};

//**********************************************************************

namespace DeptInfo
{
	const recInt DbNo = { "DbNo", 1, 10 };
	const recInt DeptNo = { "DeptNo", 0, 999 };
	const recString	Name = { "Name", 0, 40 };
	const recDouble LoyaltyBonusTrip = { "LoyaltyBonusTrip", 0.0, 999999.99 };
	const recInt LoyaltyBonusType = { "LoyaltyBonusType", 0, 2 };
	const recInt LoyaltyBonusPoints = { "LoyaltyBonusPoints", 0, 999999 };
	const recDouble LoyaltyBonusValue = { "LoyaltyBonusValue", 0.0, 999999.99 };
	const recInt LoyaltyBonusPercent = { "LoyaltyBonusPercent", 0, 100 };
	const recBool GeneralSpendBonusFlag = { "GeneralSpendBonusFlag" };
};

//**********************************************************************

namespace StaffCards
{
	const recInt FolderIndexDbNo = { "DbNo", 0, 10 };
	const recInt FolderIndexSetNo = { "FolderSet", 0, 10 };
	const recInt ServerNo = { "ServerNo", 1, 9999 };
	const recInt64 UserID = { "UserID", 1, 999999999999 };
}

//**********************************************************************

namespace StaffGiftExceptions
{
	const recInt64 LineID = { "LineID", 1, 0x7FFFFFFFFFFFFFFF };
	const recInt64 CustomerID = { "CustomerID", 1, 999999999999 };
	const recInt FolderIndexDbNo = { "DbNo", 0, 10 };
	const recInt FolderIndexSetNo = { "FolderSet", 0, 10 };
	const recInt ServerNo = { "ServerNo", 1, 9999 };
	const recInt64 ServerID = { "ServerID", 0, 999999999999 };
	const recString Date = { "Date", 0, 10 };
	const recString Time = { "Time", 0, 8 };
	const recDouble Value = { "Value", 0, 999999.99 };
	const recInt ErrorCode = { "ErrorCode", 0, 99 };
};

//**********************************************************************

namespace CCNo
{
	const recString Type = { "Type", 0, 20 };
	const recInt TypeInt = { "TypeInt", 0, 4 };
	const recInt Number = { "Number", 0, 999999 };
};

//**********************************************************************

namespace AuditPeriod
{
	const recInt PeriodID = { "PeriodID", 0, 5 };
	const recString	PeriodName = { "Name", 0, 15 };
	const recInt StartTime = { "StartTime", 0, 1439 };
};

//**********************************************************************

namespace OfferStamp
{
	const recInt64	UserID = { "UserID", 1, 999999999999 };
	const recInt StampOfferID = { "StampOfferID", 1, 10 };
	const recInt Edition = { "Edition", 1, 0x7FFFFFFF };
	const recString	AwardDate = { "AwardDate", 0, 8 };
	const recString	ExpireDate = { "ExpireDate", 0, 8 };
	const recInt StampCount = { "StampCount", 0, 999999 };
};

//**********************************************************************

namespace StampOfferInfo
{
	const recInt StampOfferID = { "StampOfferID", 1, 10 };
	const recInt Edition = { "Edition", 1, 0x7FFFFFFF };
	const recBool Enabled = { "Enabled" };
	const recInt Threshold = { "Threshold", 1, 999999 };
	const recInt Expiry = { "Expiry", 0, 999 };
	const recString Description = { "Description", 0, 50 };
};

//**********************************************************************

namespace EcrmanDatabase
{
	const recInt DbNo = { "DatabaseNo", 1, 10 };
	const recString DbName = { "DatabaseName", 0, 40 };
};

//**********************************************************************

namespace Scheme
{
	const recInt SchemeNo = { "SchemeNo", 1, 99 };
	const recString SchemeName = { "SchemeName", 0, 40 };
};

//**********************************************************************