#pragma once
//*******************************************************************
#define MAX_PLU_LEN 14
#define MAX_UNKNOWN_ACCOUNT_PROMPTLEN 80
#define MAX_UNKNOWN_ACCOUNT_NAMELEN 40
//*******************************************************************
#define RENEW_PLUNO_COUNT 16
//*******************************************************************
#define REVALUE_PLUNO_PERPURSE_COUNT 5
//*******************************************************************
#define POSDISPLAY_LINE_COUNT 8
//*******************************************************************
#define PLU_DBNO_COUNT 10
#define EPOSREQPATH_COUNT 10
//*******************************************************************

struct CServerDataEposPathInfo
{
public:
	CServerDataEposPathInfo();

public:
	CString m_strReqPath;
	CString m_strLogPath;
	CString m_strPathLabel;
	bool m_bIsExternal;
};

//*******************************************************************

struct CServerDataPosDisplayInfo
{
public:
	CServerDataPosDisplayInfo();

public:
	CString m_strField;
	CString m_strText;
};

//*******************************************************************

struct CServerDataTopupInfo
{
public:
	CServerDataTopupInfo();

public:
	CString m_strPluNo;
	double m_dTrip;
	int m_nType;
	int m_nPoints;
	double m_dValue;
	int m_nPercent;
};

//*******************************************************************

struct CServerDataPluRenewalInfo
{
public:
	CServerDataPluRenewalInfo();

public:
	CString m_strPluNo;
	int m_nMultiplier;
	int m_nType;
	int m_nYearDayNumber;
	int m_nFixedYear;
};

//*******************************************************************

struct CServerDataFolderSetIndex
{
public:
	CServerDataFolderSetIndex();
	CServerDataFolderSetIndex(int nDbNo, int nSetNo) 
	{ 
		m_nDbNo = nDbNo; m_nSetNo = nSetNo; 
	}

public:
	void Reset();

public:
	int m_nDbNo;
	int m_nSetNo;
};

//*******************************************************************

namespace ServerDataFields
{
	//TIME STAMP
	const char* const TimeStamp = "TimeStamp";
	
	//COMMUNICATIONS TAB
	const char* const LogAction = "LogAction";
	const char* const RealTimePlus = "RTPlus";
	const char* const OverSpend = "OverSpend";
	const char* const NewBalanceOnReceipt = "NewBalReceipt";
	const char* const LoyaltyLogSaveType = "LoyaltyLogSave";
	const char* const EposPathLabel = "EposPathLabel";
	const char* const EposRequestPath = "LoyReq";
	const char* const EposLogPath = "LoyLog";
	const char* const ExternalRequestPath = "ExtReq";

	//UNKNOWN ACCOUNT TAB
	const char* const AddUnknowns = "AddUnknowns";
	const char* const UnknownCardGroupRSP = "UnknownCardGroupRSP";
	const char* const UnknownCardGroupNo = "UnknownCardGroupNo";
	const char* const UnknownCardMinLength = "UnknownCardMinLength";
	const char* const UnknownCardMinID = "UnknownCardMinID";
	const char* const UnknownCardMaxID = "UnknownCardMaxID";
	const char* const UnknownAccountLine1 = "UnknownAccountLine1";
	const char* const UnknownAccountLine2 = "UnknownAccountLine2";
	const char* const UnknownAccountLine3 = "UnknownAccountLine3";
	const char* const UnknownAccountPendingName = "UnknownAccountPendingName";
	const char* const UnknownAccountCreateName = "UnknownAccountCreateName";

	//POS DISPLAY TAB
	const char* const Field1 = "Field1";
	const char* const Field2 = "Field2";
	const char* const Field3 = "Field3";
	const char* const Field4 = "Field4";
	const char* const Field5 = "Field5";
	const char* const Field6 = "Field6";
	const char* const Field7 = "Field7";
	const char* const Field8 = "Field8";
	const char* const Text1 = "Text1";
	const char* const Text2 = "Text2";
	const char* const Text3 = "Text3";
	const char* const Text4 = "Text4";
	const char* const Text5 = "Text5";
	const char* const Text6 = "Text6";
	const char* const Text7 = "Text7";
	const char* const Text8 = "Text8";
	const char* const PhotoIDFolderType = "PhotoID";
	const char* const LoyaltyUNCSystemPath = "LoyaltyUNCPath";
	const char* const LoyaltyUNCImagePath = "LoyaltyUNCImagePath";
	
	//PLU SALES TAB
	const char* const NoLegacyTopups = "NoLegacyTopups";
	const char* const LegacyRevaluePlu = "RevaluePluNo";
	const char* const LegacyRevalueTrip = "RevalueTrip";
	const char* const LegacyRevalueType = "RevalueType";
	const char* const LegacyRevaluePoints = "RevaluePoints";
	const char* const LegacyRevalueValue = "RevalueValue";
	const char* const LegacyRevaluePercent = "RevaluePercent";
	const char* const LegacyRevaluePurse = "RevaluePurse";
	const char* const LegacyRevaluePCBonusType = "PcRevalue";
	const char* const LegacayRevalueComment = "RevalueComment";
	/*****/
	const char* const TopupPlu = "Plu";
	const char* const TopupComment = "Comment";
	const char* const TopupBonusTrip = "BonusTrip";
	const char* const TopupBonusType = "BonusType";
	const char* const TopupBonusPoints = "BonusPoints";
	const char* const TopupBonusValue = "BonusValue";
	const char* const TopupBonusPercent = "BonusPercent";
	const char* const TopupPCBonusType = "PCBonusType";
	/*****/
	const char* const StaffGiftPluNo = "StaffGiftPluNo";
	const char* const StaffGiftRedirectPluNo = "StaffGiftRedirectPluNo";
	const char* const EcrManagerImportFolder = "EcrmanImportFolder";
	const char* const EcrManagerGlobalDept = "EcrmanGlobalDept";
	const char* const EcrManagerGlobalStampOffer = "EcrmanGlobalStampOffer";
	const char* const EnablePluBonus = "PluLoyaltyPoints";
	const char* const EnableDeptBonus = "EnableDeptBonus";
	const char* const NoPurseBonus = "DisablePP";
	const char* const PurchaseHistory = "PurchaseHistory";
	const char* const SpendBonusToPurse2 = "SpendBonusToPurse2";
	const char* const SpendUsePurse1First = "SpendUsePurse1First";
	
	//RENEWAL TAB
	const char* const RenewPluNo = "RenewPluNo";
	const char* const RenewMultiplier = "RenewMultiplier";
	const char* const RenewType = "RenewType";
	const char* const RenewYearDayNumber = "RenewYearDayNumber";
	const char* const RenewFixedYear = "RenewFixedYear";
	
	//SPEND BONUS TAB
	const char* const RewardPurse1Spend = "MultiplyIncCard";
	const char* const RewardPurse2Spend = "MultiplyIncP2";
	const char* const RewardCashSpend = "MultiplyIncCash";
	const char* const RewardManualSpend = "MultiplyIncManual";
	const char* const RewardUpperSpendLimit = "LoyaltyUSL";
	const char* const RewardNoBonusWithPluPoints = "NoBonusWithPluPoints";
	const char* const RewardTrip = "LoyaltyTrip";
	const char* const RewardType = "LoyaltyType";
	const char* const RewardPoints = "LoyaltyPoints";
	const char* const RewardValue = "LoyaltyValue";
	const char* const RewardComment = "LoyaltyComment";
	const char* const RewardMultiplierTrigger = "MultiplyTrigger";
	const char* const RewardMultiplierMon = "MultiplyFactorMon";
	const char* const RewardMultiplierTue = "MultiplyFactorTue";
	const char* const RewardMultiplierWed = "MultiplyFactorWed";
	const char* const RewardMultiplierThu = "MultiplyFactorThu";
	const char* const RewardMultiplierFri = "MultiplyFactorFri";
	const char* const RewardMultiplierSat = "MultiplyFactorSat";
	const char* const RewardMultiplierSun = "MultiplyFactorSun";
	const char* const RewardMonStart = "MonStart";
	const char* const RewardMonEnd = "MonEnd";
	const char* const RewardTueStart = "TueStart";
	const char* const RewardTueEnd = "TueEnd";
	const char* const RewardWedStart = "WedStart";
	const char* const RewardWedEnd = "WedEnd";
	const char* const RewardThuStart = "ThuStart";
	const char* const RewardThuEnd = "ThuEnd";
	const char* const RewardFriStart = "FriStart";
	const char* const RewardFriEnd = "FriEnd";
	const char* const RewardSatStart = "SatStart";
	const char* const RewardSatEnd = "SatEnd";
	const char* const RewardSunStart = "SunStart";
	const char* const RewardSunEnd = "SunEnd";

	//REDEMPTION TAB
	const char* const RedeemPointsTrip = "RedeemTrip";
	const char* const RedeemPointsValue = "RedeemValue";
	const char* const RedeemComment = "RedeemComment";
	const char* const RedeemDisableAuto = "RedeemDisable";

	//SEARCH TAB
	const char* const SearchSnakesAndLadders = "SearchSnakesAndLadders";
	const char* const SearchExcludeHotlist = "SearchExcludeHotlist";
	const char* const SearchExcludeInactive = "SearchExcludeInactive";
	const char* const SearchSurnameMatchFullname = "SearchSurnameMatchFullname";

	//NON UI
	const char* const SearchMax = "SearchMax";
	const char* const LockRetries = "LockRetries";
	const char* const ThreadDiagnostics = "ThreadDiagnostics";
	const char* const LockFolderTimeout = "LockFolderTimeout";
	const char* const SPOSPointsAward = "SPosPointsAward";
	const char* const SPOSPointsRedeem = "SPosPointsRedeem";
	const char* const SPOSPointsNoLog = "SPosPointsNoLog";
	const char* const SimulateReaderType = "SimulateReader";
	const char* const CopyLogFiles = "CopyLogFiles";
	const char* const SimpsInnsLookup = "BespokeFlag1";
	const char* const BypassVersionSafetyCheck = "BypassVersionSafetyCheck";
	const char* const LogCardReads = "LogCardReads";
	const char* const DoubleBonusWithHash = "DoubleBonusWithHash";
	const char* const DoubleBonusBeginDate = "DoubleBonusBeginDate";
	const char* const DoubleBonusEndDate = "DoubleBonusEndDate";

	//OBSOLETE
	const char* const ObsoleteCardNoFormat = "CardNoFormat";
	const char* const ObsoleteCardCheckDigit = "CheckDigit";
	const char* const ObsoleteCardNoPrefix = "Prefix";
};

//*******************************************************************

class CServerData
{
public:
	CServerData(void);
	bool Read(CString strDatapath);
	bool Write(CString strDatapath);
	void CopyFrom(CServerData& source);

public:
	void SetCurrentFolderSetIndex(CServerDataFolderSetIndex& index);
	CServerDataFolderSetIndex GetCurrentFolderSetIndex() { return m_CurrentFolderSetIndex; }
	
public:
	//COMMUNICATIONS TAB
	bool GetLogActionFlag() { return m_bLogAction; }
	bool GetRealTimePlusFlag() { return m_bRealTimePlus; }
	bool GetAllowOverSpendFlag() { return m_bAllowOverSpend; }
	bool GetNewBalanceOnReceiptFlag() { return m_bNewBalanceOnReceipt; }
	int GetLoyaltyLogSaveType() { return m_nLoyaltyLogSaveType; }
	CString GetEposPathLabel(CServerDataFolderSetIndex& FolderSetIndex);
	CString GetDefaultEposPathLabel(int nSet);
	CString GetEposRequestPath(CServerDataFolderSetIndex& FolderSetIndex);
	CString GetEposLogPath(CServerDataFolderSetIndex& FolderSetIndex);
	bool GetExternalRequestPathFlag(CServerDataFolderSetIndex& FolderSetIndex);
	CString GetCurrentEposRequestPath();
	CString GetCurrentEposLogPath();
	bool GetCurrentExternalRequestPathFlag();

	//UNKNOWN ACCOUNT TAB
	bool CanAddUnknownCard(CString strCardNo = "");
	bool GetAddUnknownCardsFlag() { return m_bAddUnknownCards; }
	bool GetUnknownCardGroupRSPFlag() { return m_bUnknownCardGroupRSP; }
	int GetUnknownCardMinLength() { return m_nUnknownCardMinLength; }
	__int64 GetUnknownCardMinID() { return m_nUnknownCardMinID; }
	__int64 GetUnknownCardMaxID() { return m_nUnknownCardMaxID; }
	int GetUnknownCardGroupNo() { return m_nUnknownCardGroupNo; }
	CString GetUnknownAccountLine1() { return m_strUnknownAccountLine1; }
	CString GetUnknownAccountLine2() { return m_strUnknownAccountLine2; }
	CString GetUnknownAccountLine3() { return m_strUnknownAccountLine3; }
	CString GetUnknownAccountPendingName() { return m_strUnknownAccountPendingName; }
	CString GetUnknownAccountCreateName() { return m_strUnknownAccountCreateName; }

	//POS DISPLAY TAB
	CString GetPosDisplayField(int n);
	CString GetPosDisplayText(int n);
	int GetPhotoIDFolderType() { return m_nPhotoIDFolderType; }
	CString GetLoyaltyUNCSystemPath() { return m_strLoyaltyUNCSystemPath; }
	CString GetLoyaltyUNCImagePath() { return m_strLoyaltyUNCImagePath; }
	
	//PLU SALES TAB
	CString GetTopupPluNo(int p, int t);
	double GetTopupBonusTrip(int p, int t);
	int GetTopupBonusType(int p, int t);
	int GetTopupBonusPoints(int p, int t);
	double GetTopupBonusValue(int p, int t);
	int GetTopupBonusPercent(int p, int t);
	int GetTopupPCBonusTypePurse1() { return m_nTopupPCBonusTypePurse1; }
	int GetTopupPCBonusTypePurse2() { return m_nTopupPCBonusTypePurse2;	}
	CString GetTopupCommentPurse1() { return m_strTopupCommentPurse1; }
	CString GetTopupCommentPurse2() { return m_strTopupCommentPurse2; }
	CString GetStaffGiftPluNo() { return m_strStaffGiftPluNo; }
	CString GetStaffGiftRedirectPluNo() { return m_strStaffGiftRedirectPluNo; }
	CString GetEcrManagerImportFolder() { return m_strEcrManagerImportFolder; }
	bool GetEcrManagerGlobalDeptFlag() { return m_bEcrManagerGlobalDept; }
	bool GetEcrManagerGlobalStampOfferFlag() { return m_bEcrManagerGlobalStampOffer; }
	bool GetEnablePluBonusFlag() { return m_bEnablePluBonus; }
	bool GetEnableDeptBonusFlag() { return m_bEnableDeptBonus; }
	bool GetNoPurseBonusFlag() { return m_bNoPurseBonus; }
	bool GetPurchaseHistoryFlag() { return m_bPurchaseHistory; }
	bool GetSpendBonusToPurse2Flag() { return m_bSpendBonusToPurse2; }
	bool GetSpendUsePurse1FirstFlag() { return m_bSpendUsePurse1First; }
	/*****/
	bool HaveRevaluationPluCode(int p, int t);
	bool HaveStaffGiftCode();
	bool RevaluationBonusEnabled();
	int CalculateRevaluationBonusPoints(double dValueAdded, double dTripValue, int nRevaluationPoints);
	double CalculateRevaluationBonusValue(double dValueAdded, double dTripValue, double dRevaluationValue);
	double CalculateRevaluationBonusPercent(double dValueAdded, double dTripValue, int nRevaluationPercent);
	CString GetEcrManagerImportPluFilename(int nDbNo);
	CString GetEcrManagerImportDeptFilename(int nDbNo);
	CString GetEcrManagerImportStampOfferFilename();
	CString GetEcrManagerImportLoyaltySchemeFilename();
	CString GetEcrManagerImportDbInfoFilename();

	//RENEWAL SUB TAB
	CString GetRenewPluNo(int n);
	int GetRenewMultiplier(int n);
	int GetRenewType(int n);
	int GetRenewYearDayNumber(int n);
	int GetRenewFixedYear(int n);
	
	//SPEND BONUS TAB
	bool GetRewardPurse1SpendFlag() { return m_bRewardPurse1Spend; }
	bool GetRewardPurse2SpendFlag() { return m_bRewardPurse2Spend; }
	bool GetRewardCashSpendFlag() { return m_bRewardCashSpend; }
	bool GetRewardManualSpendFlag() { return m_bRewardManualSpend; }
	double GetRewardUpperSpendLimit() { return m_dRewardUpperSpendLimit; }
	double GetRewardTrip() { return m_dRewardTrip; }
	int GetRewardType() { return m_nRewardType; }
	int GetRewardPoints() { return m_nRewardPoints; }
	double GetRewardValue() { return m_dRewardValue; }
	bool GetRewardNoBonusWithPluPointsFlag() { return m_bRewardNoBonusWithPluPoints; }
	CString GetRewardComment() { return m_strRewardComment; }
	double GetRewardMultiplierTrigger() { return m_dRewardMultiplierTrigger; }
	int GetRewardMultiplierMon() { return m_nRewardMultiplierMon; }
	int GetRewardMultiplierTue() { return m_nRewardMultiplierTue; }
	int GetRewardMultiplierWed() { return m_nRewardMultiplierWed; }
	int GetRewardMultiplierThu() { return m_nRewardMultiplierThu; }
	int GetRewardMultiplierFri() { return m_nRewardMultiplierFri; }
	int GetRewardMultiplierSat() { return m_nRewardMultiplierSat; }
	int GetRewardMultiplierSun() { return m_nRewardMultiplierSun; }
	CSSTime GetRewardMonStart() { return m_timeRewardMonStart; }
	CSSTime GetRewardMonEnd() { return m_timeRewardMonEnd; }
	CSSTime GetRewardTueStart() { return m_timeRewardTueStart; }
	CSSTime GetRewardTueEnd() { return m_timeRewardTueEnd; }
	CSSTime GetRewardWedStart() { return m_timeRewardWedStart; }
	CSSTime GetRewardWedEnd() { return m_timeRewardWedEnd; }
	CSSTime GetRewardThuStart() { return m_timeRewardThuStart; }
	CSSTime GetRewardThuEnd() { return m_timeRewardThuEnd; }
	CSSTime GetRewardFriStart() { return m_timeRewardFriStart; }
	CSSTime GetRewardFriEnd() { return m_timeRewardFriEnd; }
	CSSTime GetRewardSatStart() { return m_timeRewardSatStart; }
	CSSTime GetRewardSatEnd() { return m_timeRewardSatEnd; }
	CSSTime GetRewardSunStart() { return m_timeRewardSunStart; }
	CSSTime GetRewardSunEnd() { return m_timeRewardSunEnd; }
	/*****/
	bool SpendBonusEnabled() { return (m_dRewardTrip == 0.0) ? FALSE : TRUE; }

	//REDEMPTION TAB
	int GetRedeemPointsTrip() { return m_nRedeemPointsTrip; }
	double GetRedeemPointsValue() { return m_dRedeemPointsValue; }
	int GetRedeemPointsType() { return nREDEEM_STANDARD; }
	//int GetRedeemPointsType() { return m_nRedeemPointsType; }
	CString GetRedeemComment() { return m_strRedeemComment; }
	bool GetRedeemDisableAutoFlag() { return m_bRedeemDisableAuto; }
	/*****/
	bool RedeemPointsEnabled() { return (m_nRedeemPointsTrip == 0) ? FALSE : TRUE; }

	//SEARCH TAB
	bool GetSearchSnakesAndLaddersFlag() { return m_bSearchSnakesAndLadders; }
	bool GetSearchExcludeHotlistFlag() { return m_bSearchExcludeHotlist; }
	bool GetSearchExcludeInactiveFlag() { return m_bSearchExcludeInactive; }
	bool GetSearchSurnameMatchFullnameFlag() { return m_bSearchSurnameMatchFullname; }
	
	//NON UI
	int GetSearchMax() { return m_nSearchMax; }
	int GetLockRetries() { return m_nLockRetries; }
	int GetThreadDiagnosticsFlags() { return m_nThreadDiagnostics; }
	int GetLockFolderTimeout() { return m_nLockFolderTimeout; }
	bool GetSPosPointsAwardFlag() { return m_bSPosPointsAward; }
	bool GetSPosPointsRedeemFlag() { return m_bSPosPointsRedeem; }
	bool GetSPosPointsNoLogFlag() { return m_bSPosPointsNoLog; }
	static int GetSimulateReaderType() { return m_nSimulateReaderType; }
	bool GetCopyLogFilesFlag() { return m_bCopyLogFiles; }
	bool GetSimpsInnsLookupFlag() { return m_bSimpsInnsLookup; }
	bool GetBypassVersionSafetyCheckFlag() { return m_bBypassVersionSafetyCheck; }
	bool GetLogCardReadsFlag() { return m_bLogCardReads; }
	bool GetDoubleBonusWithHashFlag() { return m_bDoubleBonusWithHash; }
	CString GetDoubleBonusBeginDate() { return m_strDoubleBonusBeginDate; }
	CString GetDoubleBonusEndDate() { return m_strDoubleBonusEndDate; }

	//OBSOLETE
	int GetObsoleteCardNoFormat() { return m_nObsoleteCardNoFormat; }
	bool GetObsoleteCardCheckDigitFlag() { return m_bObsoleteCardCheckDigit; }
	CString GetObsoleteCardNoPrefix() { return m_strObsoleteCardNoPrefix; }

public:
	//COMMUNICATIONS TAB
	void SetLogActionFlag(bool b) { m_bLogAction = b; }
	void SetRealTimePlusFlag(bool b) { m_bRealTimePlus = b; }
	void SetAllowOverSpendFlag(bool b) { m_bAllowOverSpend = b; }
	void SetNewBalanceOnReceiptFlag(bool b) { m_bNewBalanceOnReceipt = b; }
	void SetLoyaltyLogSaveType(int n) { m_nLoyaltyLogSaveType = n; }
	void SetEposRequestPath(CServerDataFolderSetIndex& FolderSetIndex, CString str);
	void SetEposLogPath(CServerDataFolderSetIndex& FolderSetIndex, CString str);
	void SetEposPathLabel(CServerDataFolderSetIndex& FolderSetIndex, CString str);
	void SetExternalRequestPathFlag(CServerDataFolderSetIndex& FolderSetIndex, bool b);
	
	//UNKNOWN ACCOUNT TAB
	void SetAddUnknownCardsFlag(bool b) { m_bAddUnknownCards = b; }
	void SetUnknownCardGroupRSPFlag(bool b) { m_bUnknownCardGroupRSP = b; }
	void SetUnknownCardGroupNo(int n) { m_nUnknownCardGroupNo = n; }
	void SetUnknownCardMinLength(int n);
	void SetUnknownCardMinID(__int64 n);
	void SetUnknownCardMaxID(__int64 n);
	void SetUnknownAccountLine1(CString str);
	void SetUnknownAccountLine2(CString str);
	void SetUnknownAccountLine3(CString str);
	void SetUnknownAccountPendingName(CString str);
	void SetUnknownAccountCreateName(CString str);
	
	//POS DISPLAY TAB
	void SetPosDisplayField(int n, CString str);
	void SetPosDisplayText(int n, CString str);
	void SetPhotoIDFolderType(int n);
	void SetLoyaltyUNCSystemPath(CString str) { m_strLoyaltyUNCSystemPath = str; }
	void SetLoyaltyUNCImagePath(CString str) { m_strLoyaltyUNCImagePath = str; }
	
	//PLU SALES TAB
	void SetTopupPluNo(int p, int t, CString str);
	void SetTopupBonusTrip(int p, int t, double x);
	void SetTopupBonusType(int p, int t, int nType);
	void SetTopupBonusPoints(int p, int t, int nPoints);
	void SetTopupBonusValue(int p, int t, double dValue);
	void SetTopupBonusPercent(int p, int t, int nPercent);
	void SetTopupPCBonusTypePurse1(int n) { m_nTopupPCBonusTypePurse1 = n; }
	void SetTopupPCBonusTypePurse2(int n) { m_nTopupPCBonusTypePurse2 = n; }
	void SetTopupCommentPurse1(CString str) { m_strTopupCommentPurse1 = str; }
	void SetTopupCommentPurse2(CString str) { m_strTopupCommentPurse2 = str; }
	void SetStaffGiftPluNo(CString str) { m_strStaffGiftPluNo = str; }
	void SetStaffGiftRedirectPluNo(CString str) { m_strStaffGiftRedirectPluNo = str; }
	void SetEcrManagerImportFolder(CString str);
	void SetEcrManagerGlobalDeptFlag(bool b) { m_bEcrManagerGlobalDept = b; }
	void SetEcrManagerGlobalStampOfferFlag(bool b) { m_bEcrManagerGlobalStampOffer = b; }
	void SetEnablePluBonusFlag(bool b) { m_bEnablePluBonus = b; }
	void SetEnableDeptBonusFlag(bool b) { m_bEnableDeptBonus = b; }
	void SetNoPurseBonusFlag(bool b) { m_bNoPurseBonus = b; }
	void SetPurchaseHistoryFlag(bool b) { m_bPurchaseHistory = b; }
	void SetSpendBonusToPurse2Flag(bool b) { m_bSpendBonusToPurse2 = b; }
	void SetSpendUsePurse1FirstFlag(bool b) { m_bSpendUsePurse1First = b; }
	
	//RENEWAL TAB
	void SetRenewPluNo(int n, CString str);
	void SetRenewMultiplier(int n, int x);
	void SetRenewType(int n, int x);
	void SetRenewYearDayNumber(int n, int x);
	void SetRenewFixedYear(int n, int x);

	//SPEND BONUS TAB
	void SetRewardPurse1SpendFlag(bool b) { m_bRewardPurse1Spend = b; }
	void SetRewardPurse2SpendFlag(bool b) { m_bRewardPurse2Spend = b; }
	void SetRewardCashSpendFlag(bool b) { m_bRewardCashSpend = b; }
	void SetRewardManualSpendFlag(bool b) { m_bRewardManualSpend = b; }
	void SetRewardUpperSpendLimit(double d) { m_dRewardUpperSpendLimit = d; }
	void SetRewardNoBonusWithPluPointsFlag(bool b) { m_bRewardNoBonusWithPluPoints = b; }
	void SetRewardTrip(double d) { m_dRewardTrip = d; }
	void SetRewardType(int n) { m_nRewardType = n; }
	void SetRewardPoints(int n) { m_nRewardPoints = n; }
	void SetRewardValue(double d) { m_dRewardValue = d; }
	void SetRewardComment(CString str) { m_strRewardComment = str; }
	void SetRewardMultiplierTrigger(double d) { m_dRewardMultiplierTrigger = d; }
	void SetRewardMultiplierMon(int n) { m_nRewardMultiplierMon = n; }
	void SetRewardMultiplierTue(int n) { m_nRewardMultiplierTue = n; }
	void SetRewardMultiplierWed(int n) { m_nRewardMultiplierWed = n; }
	void SetRewardMultiplierThu(int n) { m_nRewardMultiplierThu = n; }
	void SetRewardMultiplierFri(int n) { m_nRewardMultiplierFri = n; }
	void SetRewardMultiplierSat(int n) { m_nRewardMultiplierSat = n; }
	void SetRewardMultiplierSun(int n) { m_nRewardMultiplierSun = n; }
	void SetRewardMonStart(CSSTime time) { m_timeRewardMonStart = time; }
	void SetRewardMonEnd(CSSTime time) { m_timeRewardMonEnd = time; }
	void SetRewardTueStart(CSSTime time) { m_timeRewardTueStart = time; }
	void SetRewardTueEnd(CSSTime time) { m_timeRewardTueEnd = time; }
	void SetRewardWedStart(CSSTime time) { m_timeRewardWedStart = time; }
	void SetRewardWedEnd(CSSTime time) { m_timeRewardWedEnd = time; }
	void SetRewardThuStart(CSSTime time) { m_timeRewardThuStart = time; }
	void SetRewardThuEnd(CSSTime time) { m_timeRewardThuEnd = time; }
	void SetRewardFriStart(CSSTime time) { m_timeRewardFriStart = time; }
	void SetRewardFriEnd(CSSTime time) { m_timeRewardFriEnd = time; }
	void SetRewardSatStart(CSSTime time) { m_timeRewardSatStart = time; }
	void SetRewardSatEnd(CSSTime time) { m_timeRewardSatEnd = time; }
	void SetRewardSunStart(CSSTime time) { m_timeRewardSunStart = time; }
	void SetRewardSunEnd(CSSTime time) { m_timeRewardSunEnd = time; }
	void SetRewardMonStart(int nHour, int nMinute) { m_timeRewardMonStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardMonEnd(int nHour, int nMinute) { m_timeRewardMonEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardTueStart(int nHour, int nMinute) { m_timeRewardTueStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardTueEnd(int nHour, int nMinute) { m_timeRewardTueEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardWedStart(int nHour, int nMinute) { m_timeRewardWedStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardWedEnd(int nHour, int nMinute) { m_timeRewardWedEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardThuStart(int nHour, int nMinute) { m_timeRewardThuStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardThuEnd(int nHour, int nMinute) { m_timeRewardThuEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardFriStart(int nHour, int nMinute) { m_timeRewardFriStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardFriEnd(int nHour, int nMinute) { m_timeRewardFriEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardSatStart(int nHour, int nMinute) { m_timeRewardSatStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardSatEnd(int nHour, int nMinute) { m_timeRewardSatEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardSunStart(int nHour, int nMinute) { m_timeRewardSunStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetRewardSunEnd(int nHour, int nMinute) { m_timeRewardSunEnd.SetTime(GetTimeString(nHour, nMinute)); }

	//REDEMPTION TAB
	void SetRedeemPointsTrip(int n) { m_nRedeemPointsTrip = n; }
	void SetRedeemPointsValue(double d) { m_dRedeemPointsValue = d; }
	void SetRedeemComment(CString str) { m_strRedeemComment = str; }
	void SetRedeemDisableAutoFlag(bool b) { m_bRedeemDisableAuto = b; }
	
	//SEARCH TAB
	void SetSearchSnakesAndLaddersFlag(bool b) { m_bSearchSnakesAndLadders = b; }
	void SetSearchExcludeHotlistFlag(bool b) { m_bSearchExcludeHotlist = b; }
	void SetSearchExcludeInactiveFlag(bool b) { m_bSearchExcludeInactive = b; }
	void SetSearchSurnameMatchFullnameFlag(bool b) { m_bSearchSurnameMatchFullname = b; }
	
	//NON UI
	void SetSearchMax(int nMax);
	void SetLockRetries(int n);
	void SetThreadDiagnosticsFlag(int n) { m_nThreadDiagnostics = n; }
	void SetLockFolderTimeout(int n);
	void SetSPosPointsAwardFlag(bool b) { m_bSPosPointsAward = b; }
	void SetSPosPointsRedeemFlag(bool b) { m_bSPosPointsRedeem = b; }
	void SetSPosPointsNoLogFlag(bool b) { m_bSPosPointsNoLog = b; }
	static void SetSimulateReaderType(int n) { m_nSimulateReaderType = n; }
	void SetCopyLogFilesFlag(bool b) { m_bCopyLogFiles = b; }
	void SetSimpsInnsLookupFlag(bool b) { m_bSimpsInnsLookup = b; }
	void SetBypassVersionSafetyCheckFlag(bool b) { m_bBypassVersionSafetyCheck = b; }
	void SetLogCardReadsFlag(bool b) { m_bLogCardReads = b; }
	void SetDoubleBonusWithHashFlag(bool b) { m_bDoubleBonusWithHash = b; }	
	void SetDoubleBonusBeginDate(CString str);
	void SetDoubleBonusEndDate(CString str);

	//OBSOLETE
	void SetObsoleteCardNoFormat( int n ) { m_nObsoleteCardNoFormat = n; }
	void SetObsoleteCardCheckDigitFlag( bool b ) { m_bObsoleteCardCheckDigit = b; }
	void SetObsoleteCardNoPrefix(CString str) { m_strObsoleteCardNoPrefix = str; }
	
public:
	void UpdateEcrmanDbInfo();
	bool CanDoDoubleBonusNow();

private:
	//HELPER FUNCTIONS:
	CString GetTimeString(int nHour, int nMinute);
	CString GetIniStringWithFallback(CSSIniFile& file, const CString& strKey, const CString& strFallbackKey, const CString& strDefault);
	bool GetIniBoolWithFallback(CSSIniFile& file, const CString& strKey, const CString& strFallbackKey, int nDefault);
	void CheckLegacyTopups(CSSIniFile& file);
	bool Validate8DigitDate(CString strDate);

private:
	//COMMUNICATIONS TAB
	bool m_bLogAction;
	bool m_bRealTimePlus;
	bool m_bAllowOverSpend;
	bool m_bNewBalanceOnReceipt;
	int m_nLoyaltyLogSaveType;
	CServerDataEposPathInfo m_EposPathInfo[PLU_DBNO_COUNT][EPOSREQPATH_COUNT];
	
	//UNKNOWN ACCOUNT TAB
	bool m_bAddUnknownCards;
	bool m_bUnknownCardGroupRSP;
	int m_nUnknownCardGroupNo;
	int m_nUnknownCardMinLength;
	__int64 m_nUnknownCardMinID;
	__int64 m_nUnknownCardMaxID;
	CString m_strUnknownAccountLine1;
	CString m_strUnknownAccountLine2;
	CString m_strUnknownAccountLine3;
	CString m_strUnknownAccountPendingName;
	CString m_strUnknownAccountCreateName;

	//POS DISPLAY TAB
	CServerDataPosDisplayInfo m_PosDisplayInfo[POSDISPLAY_LINE_COUNT + 1];
	int m_nPhotoIDFolderType;
	CString m_strLoyaltyUNCSystemPath;
	CString m_strLoyaltyUNCImagePath;
	
	//PLU SALES TAB
	CServerDataTopupInfo m_TopupInfo[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	int m_nTopupPCBonusTypePurse1;
	int m_nTopupPCBonusTypePurse2;
	CString m_strTopupCommentPurse1;
	CString m_strTopupCommentPurse2;
	CString m_strStaffGiftPluNo;
	CString m_strStaffGiftRedirectPluNo;
	CString m_strEcrManagerImportFolder;
	bool m_bEcrManagerGlobalDept;
	bool m_bEcrManagerGlobalStampOffer;
	CString m_strEcrManagerImportPluFilename;
	CString m_strEcrManagerImportDeptFilename;
	CString m_strEcrManagerImportStampOfferFilename;
	CString m_strEcrManagerImportLoyaltySchemeFilename;
	CString m_strEcrManagerImportDbInfoFilename;
	bool m_bEnablePluBonus;
	bool m_bEnableDeptBonus;
	bool m_bNoPurseBonus;
	bool m_bPurchaseHistory;
	bool m_bSpendBonusToPurse2;
	bool m_bSpendUsePurse1First;

	//RENEWAL TAB
	CServerDataPluRenewalInfo m_PluRenewalInfo[RENEW_PLUNO_COUNT + 1];
	
	//SPEND BONUS TAB
	bool m_bRewardPurse1Spend;
	bool m_bRewardPurse2Spend;
	bool m_bRewardCashSpend;
	bool m_bRewardManualSpend;
	double m_dRewardUpperSpendLimit;
	bool m_bRewardNoBonusWithPluPoints;
	double m_dRewardTrip;
	int m_nRewardType;
	int m_nRewardPoints;
	double m_dRewardValue;
	CString m_strRewardComment;
	double m_dRewardMultiplierTrigger;
	int m_nRewardMultiplierMon;
	int m_nRewardMultiplierTue;
	int m_nRewardMultiplierWed;
	int m_nRewardMultiplierThu;
	int m_nRewardMultiplierFri;
	int m_nRewardMultiplierSat;
	int m_nRewardMultiplierSun;
	CSSTime m_timeRewardMonStart;
	CSSTime m_timeRewardMonEnd;
	CSSTime m_timeRewardTueStart;
	CSSTime m_timeRewardTueEnd;
	CSSTime m_timeRewardWedStart;
	CSSTime m_timeRewardWedEnd;
	CSSTime m_timeRewardThuStart;
	CSSTime m_timeRewardThuEnd;
	CSSTime m_timeRewardFriStart;
	CSSTime m_timeRewardFriEnd;
	CSSTime m_timeRewardSatStart;
	CSSTime m_timeRewardSatEnd;
	CSSTime m_timeRewardSunStart;
	CSSTime m_timeRewardSunEnd;

	//REDEMPTION TAB
	int m_nRedeemPointsTrip;
	int m_nRedeemPointsType;
	double m_dRedeemPointsValue;
	CString m_strRedeemComment;
	bool m_bRedeemDisableAuto;

	//SEARCH TAB
	bool m_bSearchExcludeHotlist;
	bool m_bSearchExcludeInactive;
	bool m_bSearchSurnameMatchFullname;
	bool m_bSearchSnakesAndLadders;

	//NON UI
	int m_nSearchMax;
	int m_nLockRetries;
	int m_nThreadDiagnostics;
	int m_nLockFolderTimeout;
	bool m_bSPosPointsAward;
	bool m_bSPosPointsRedeem;
	bool m_bSPosPointsNoLog;
	static int m_nSimulateReaderType;
	bool m_bCopyLogFiles;
	bool m_bSimpsInnsLookup;
	bool m_bBypassVersionSafetyCheck;
	bool m_bLogCardReads;
	bool m_bDoubleBonusWithHash;
	CString m_strDoubleBonusBeginDate;
	CString m_strDoubleBonusEndDate;

	//OBSOLETE
	int m_nObsoleteCardNoFormat;				//
	bool m_bObsoleteCardCheckDigit;				// DO NOT USE - only in here to allow for convert to INI
	CString m_strObsoleteCardNoPrefix;			//				

	//INTERNAL
	CServerDataFolderSetIndex m_CurrentFolderSetIndex;
	CString m_strTimeStamp;
};

//*******************************************************************
extern CServerData Server;
extern CServerData ServerCopy;
//*******************************************************************
