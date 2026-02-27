#pragma once
//*******************************************************************
#define EPOSREQPATH_COUNT 10
//*******************************************************************
#pragma once
//*******************************************************************

namespace ServerDataFields
{
	//TIME STAMP
	const char* const TimeStamp = "TimeStamp";
	//COMMUNICATIONS
	const char* const LogAction2 = "LogAction2";
	const char* const AllowOverdraft = "OverSpend";
	const char* const AddUnknowns = "AddUnknowns";
	const char* const OfflineSpendID = "OfflineSpendID";
	const char* const OfflineSpendLimit = "OfflineSpendLimit";
	const char* const UnknownGroup = "UnknownGroup";
	const char* const NewBalanceOnReceipt = "NewBalReceipt";
	const char* const ForenameSearchFieldType = "ForenameSearchFieldType";
	const char* const EposPathLabel = "EposPathLabel";
	const char* const EposRequestPath = "LoyReq";
	const char* const EposLogPath = "LoyLog";
	const char* const ExternalRequestPath = "ExtReq";
	//PHOTO ID
	const char* const PhotoIDFolderType = "PhotoID";
	const char* const SmartPayUNCSystemPath = "LoyaltyUNCPath";
	const char* const SmartPayUNCImagePath = "LoyaltyUNCImagePath";
	//PLU SALES
	const char* const PluNo = "PluNo";
	const char* const PluNo2 = "PluNo2";
	const char* const RevaluationTrip = "RevalueTrip";
	const char* const RevaluationTrip2 = "RevalueTrip2";
	const char* const RevaluationPoints = "RevaluePoints";
	const char* const RevaluationValue = "RevalueValue";
	const char* const RevaluationType = "RevalueType";
	const char* const RevaluationComment = "RevalueComment";
	const char* const EnablePurchaseHistory = "PurchaseHistory";
	const char* const DisablePurchaseHistoryAccountZero = "DisablePurchaseHistoryAccountZero";
	const char* const EcrManagerImportFolder = "EcrmanImportFolder";
	//SPEND BONUS
	const char* const RewardUpperSpendLimit = "LoyaltyUSL";
	const char* const RewardTrip = "LoyaltyTrip";
	const char* const RewardPoints = "LoyaltyPoints";
	const char* const RewardValue = "LoyaltyValue";
	const char* const RewardMultiplierFactorMon = "MultiplyFactorMon";
	const char* const RewardMultiplierFactorTue = "MultiplyFactorTue";
	const char* const RewardMultiplierFactorWed = "MultiplyFactorWed";
	const char* const RewardMultiplierFactorThu = "MultiplyFactorThu";
	const char* const RewardMultiplierFactorFri = "MultiplyFactorFri";
	const char* const RewardMultiplierFactorSat = "MultiplyFactorSat";
	const char* const RewardMultiplierFactorSun = "MultiplyFactorSun";
	const char* const RewardMultiplierTrigger = "MultiplyTrigger";
	const char* const RewardIncludePurse1Spend = "LoyaltyPurse1";
	const char* const RewardIncludePurse2Spend = "LoyaltyPurse2";
	const char* const RewardIncludePurse3Spend = "LoyaltyPurse3";
	const char* const RewardIncludeCashSpend = "LoyaltyCash";
	const char* const RewardType = "LoyaltyType";
	const char* const RewardComment = "LoyaltyComment";
	const char* const MonStart = "MonStart";
	const char* const MonEnd = "MonEnd";
	const char* const TueStart = "TueStart";
	const char* const TueEnd = "TueEnd";
	const char* const WedStart = "WedStart";
	const char* const WedEnd = "WedEnd";
	const char* const ThuStart = "ThuStart";
	const char* const ThuEnd = "ThuEnd";
	const char* const FriStart = "FriStart";
	const char* const FriEnd = "FriEnd";
	const char* const SatStart = "SatStart";
	const char* const SatEnd = "SatEnd";
	const char* const SunStart = "SunStart";
	const char* const SunEnd = "SunEnd";
	//REDEEM POINTS
	const char* const RedeemPointsTrip = "RedeemTrip";
	const char* const RedeemPurse = "RedeemPurse";
	const char* const RedeemValue = "RedeemValue";
	const char* const RedeemDisableAuto = "RedeemDisable";
	const char* const RedeemComment = "RedeemComment";
	//NON UI
	const char* const SearchMax = "SearchMax";
	const char* const AllowRetries = "DealerFlag1";
	const char* const AllowPebbleAdjust = "DealerFlag2";
	const char* const AllowPINAdmin = "DealerFlag3";
	const char* const LockRetries = "LockRetries";
	const char* const ThreadDiagnostics = "ThreadDiagnostics";
	const char* const BypassVersionSafetyCheck = "BypassVersionSafetyCheck";
	const char* const CopyLogFiles = "CopyLogFiles";
	const char* const LockFolderTimeout = "LockFolderTimeout";
	const char* const BromComTestMode = "BromComTestMode";
	const char* const SimulateReaderType = "SimulateReader";
	const char* const PebbleDemo = "PebbleDemo";
	const char* const PebbleDiagnostic = "PebbleDiagnostic";
	const char* const LogCardReads = "LogCardReads";
};

//*******************************************************************

class CServerData
{
public:
	CServerData(void);
	bool Read( const CString strDataPath );
	bool Write( const CString strDataPath );
	void CopyFrom( CServerData& source );

public:
	//COMMUNICATIONS
	bool GetLogAction2Flag() { return m_bLogAction2; }
	bool GetAllowOverdraftFlag() { return m_bAllowOverdraft; }
	bool GetAddUnknownsFlag() { return m_bAddUnknowns; }
	CString GetOfflineSpendID() { return m_strOfflineSpendID; }
	double GetOfflineSpendLimit() { return m_dOfflineSpendLimit; }
	int GetUnknownGroup() { return m_nUnknownGroup; }
	bool GetNewBalanceOnReceiptFlag() { return m_bNewBalanceOnReceipt; }
	int GetForenameSearchFieldType() { return m_nForenameSearchFieldType; }
	int GetCurrentEposPathSet() { return m_nCurrentEposPathSet; }
	const char* GetEposPathLabel(int n);
	CString GetDefaultEposPathLabel(int n);
	const char* GetEposRequestPath(int n);
	const char* GetEposLogPath(int n);
	bool GetExternalRequestPathFlag(int n);
	const char* GetCurrentEposRequestPath();
	const char* GetCurrentEposLogPath();
	bool GetCurrentExternalRequestPathFlag();
	//PHOTO ID
	int GetPhotoIDFolderType() { return m_nPhotoIDFolderType; }
	CString GetSmartPayUNCSystemPath() { return m_strSmartPayUNCSystemPath; }
	CString GetSmartPayUNCImagePath() { return m_strSmartPayUNCImagePath; }
	//PLU SALES
	CString GetPluNo() { return m_strPluNo; }
	CString GetPluNo2() { return m_strPluNo2; }
	double GetFileRevaluationTrip() { return m_dFileRevaluationTrip; }
	double GetFileRevaluationTrip2() { return m_dFileRevaluationTrip2; }
	int GetRevaluationPoints() { return m_nRevaluationPoints; }
	double GetRevaluationValue() { return m_dRevaluationValue; }
	int GetRevaluationType() { return m_nRevaluationType; }
	CString GetRevaluationComment() { return m_strRevaluationComment; }
	bool GetFileEnablePurchaseHistoryFlag() { return m_bFileEnablePurchaseHistory; }
	bool GetDisablePurchaseHistoryAccountZeroFlag() { return m_bDisablePurchaseHistoryAccountZero; }
	CString GetEcrManagerImportFolder() { return m_strEcrManagerImportFolder; }
	/*****/
	CString GetEcrManagerImportPluFilename();
	CString GetEcrManagerImportOperatorFilename();
	CString GetEcrManagerImportEposTerminalFilename();
	CString GetEcrManagerImportPurchaseControlFilename();
	//SPEND BONUS
	double GetRewardUpperSpendLimit() { return m_dRewardUpperSpendLimit; }
	double GetFileRewardTrip() { return m_dFileRewardTrip; }
	int GetRewardPoints() { return m_nRewardPoints; }
	double GetRewardValue() { return m_dRewardValue; }
	int GetRewardMultiplierFactorMon() { return m_nRewardMultiplierFactorMon; }
	int GetRewardMultiplierFactorTue() { return m_nRewardMultiplierFactorTue; }
	int GetRewardMultiplierFactorWed() { return m_nRewardMultiplierFactorWed; }
	int GetRewardMultiplierFactorThu() { return m_nRewardMultiplierFactorThu; }
	int GetRewardMultiplierFactorFri() { return m_nRewardMultiplierFactorFri; }
	int GetRewardMultiplierFactorSat() { return m_nRewardMultiplierFactorSat; }
	int GetRewardMultiplierFactorSun() { return m_nRewardMultiplierFactorSun; }
	double GetRewardMultiplierTrigger() { return m_dRewardMultiplierTrigger; }
	bool GetRewardIncludePurse1SpendFlag() { return m_bRewardIncludePurse1Spend; }
	bool GetRewardIncludePurse2SpendFlag() { return m_bRewardIncludePurse2Spend; }
	bool GetRewardIncludePurse3SpendFlag() { return m_bRewardIncludePurse3Spend; }
	bool GetRewardIncludeCashSpendFlag() { return m_bRewardIncludeCashSpend; }
	int GetRewardType() { return m_nRewardType; }
	CString GetRewardComment() { return m_strRewardComment; }
	CSSTime GetMonStart() { return m_timeMonStart; }
	CSSTime GetMonEnd() { return m_timeMonEnd; }
	CSSTime GetTueStart() { return m_timeTueStart; }
	CSSTime GetTueEnd() { return m_timeTueEnd; }
	CSSTime GetWedStart() { return m_timeWedStart; }
	CSSTime GetWedEnd() { return m_timeWedEnd; }
	CSSTime GetThuStart() { return m_timeThuStart; }
	CSSTime GetThuEnd() { return m_timeThuEnd; }
	CSSTime GetFriStart() { return m_timeFriStart; }
	CSSTime GetFriEnd() { return m_timeFriEnd; }
	CSSTime GetSatStart() { return m_timeSatStart; }
	CSSTime GetSatEnd() { return m_timeSatEnd; }
	CSSTime GetSunStart() { return m_timeSunStart; }
	CSSTime GetSunEnd() { return m_timeSunEnd; }
	//REDEEM POINTS
	int GetFileRedeemPointsTrip() { return m_nFileRedeemPointsTrip; }
	int GetRedeemPurse() { return m_nRedeemPurse; }
	double GetRedeemValue() { return m_dRedeemValue; }
	bool GetRedeemDisableAutoFlag() { return m_bRedeemDisableAuto; }
	CString GetRedeemComment() { return m_strRedeemComment; }
	//NON UI
	int GetSearchMax() { return m_nSearchMax; }
	bool GetAllowRetriesFlag() { return m_bAllowRetries; }
	int GetAllowPebbleAdjustCode() { return m_nAllowPebbleAdjust; }
	bool GetAllowPebbleAdjustFlag() { return (3142227 == m_nAllowPebbleAdjust); }
	int GetAllowPINAdminCode() { return m_nAllowPINAdmin; }
	bool GetAllowPINAdminFlag() { return (691608 == m_nAllowPINAdmin); }
	bool GetAllowPINRangeFlag() { return (788235 == m_nAllowPINAdmin) || (691608 == m_nAllowPINAdmin); }
	int GetLockRetries() { return m_nLockRetries; }
	int GetThreadDiagnosticsFlags() { return m_nThreadDiagnostics; }
	bool GetBypassVersionSafetyCheckFlag() { return m_bBypassVersionSafetyCheck; }
	bool GetCopyLogFilesFlag() { return m_bCopyLogFiles; }
	int GetLockFolderTimeout() { return m_nLockFolderTimeout; }
	bool GetBromComTestModeFlag() { return m_bBromComTestMode; }
	bool GetPebbleDemoFlag() { return m_bPebbleDemo; }
	int GetPebbleDiagnosticType() { return m_nPebbleDiagnosticType; }
	static int GetSimulateReaderType() { return m_nSimulateReaderType; }
	bool GetLogCardReadsFlag() { return m_bLogCardReads; }

	//ACTIVE FUNCTIONS RETURN LOCKED VALUES FOR PEBBLE MODE
	double GetActiveRevaluationTrip();
	double GetActiveRevaluationTrip2();
	bool GetActiveEnablePurchaseHistoryFlag();
	double GetActiveRewardTrip();
	int GetActiveRedeemPointsTrip();

public:
	//COMMUNICATIONS
	void SetLogAction2Flag(bool b) { m_bLogAction2 = b; }
	void SetAllowOverdraftFlag(bool b) { m_bAllowOverdraft = b; }
	void SetAddUnknownsFlag(bool b) { m_bAddUnknowns = b; }
	void SetOfflineSpendID(CString str) { m_strOfflineSpendID = str; }
	void SetOfflineSpendLimit(double d) { m_dOfflineSpendLimit = d; }
	void SetUnknownGroup(int n) { m_nUnknownGroup = n; }
	void SetNewBalanceOnReceiptFlag(bool b) { m_bNewBalanceOnReceipt = b; }
	void SetForenameSearchFieldType(int n);
	void SetCurrentEposPathSet(int n);
	void SetEposPathLabel(int n, const char* sz);
	void SetEposRequestPath(int n, const char* sz);
	void SetEposLogPath(int n, const char* sz);
	void SetExternalRequestPathFlag(int n, bool b);
	//PHOTO ID
	void SetPhotoIDFolderType(int n);
	void SetSmartPayUNCSystemPath(const char* sz) { m_strSmartPayUNCSystemPath = sz; }
	void SetSmartPayUNCImagePath(const char* sz) { m_strSmartPayUNCImagePath = sz; }
	//PLU SALES
	void SetPluNo(CString str) { m_strPluNo = str; }
	void SetPluNo2(CString str) { m_strPluNo2 = str; }
	void SetFileRevaluationTrip(double d) { m_dFileRevaluationTrip = d; }
	void SetFileRevaluationTrip2(double d) { m_dFileRevaluationTrip2 = d; }
	void SetRevaluationPoints(int n) { m_nRevaluationPoints = n; }
	void SetRevaluationValue(double d) { m_dRevaluationValue = d; }
	void SetRevaluationType(int n) { m_nRevaluationType = n; }
	void SetRevaluationComment(CString str) { m_strRevaluationComment = str; }
	void SetFileEnablePurchaseHistoryFlag(bool b) { m_bFileEnablePurchaseHistory = b; }
	void SetDisablePurchaseHistoryAccountZeroFlag(bool b) { m_bDisablePurchaseHistoryAccountZero = b; }
	void SetEcrManagerImportFolder(CString str) { m_strEcrManagerImportFolder = str; }
	//SPEND BONUS
	void SetRewardUpperSpendLimit(double d) { m_dRewardUpperSpendLimit = d; }
	void SetFileRewardTrip(double d) { m_dFileRewardTrip = d; }
	void SetRewardPoints(int n) { m_nRewardPoints = n; }
	void SetRewardValue(double d) { m_dRewardValue = d; }
	void SetRewardMultiplierFactorMon(int n) { m_nRewardMultiplierFactorMon = n; }
	void SetRewardMultiplierFactorTue(int n) { m_nRewardMultiplierFactorTue = n; }
	void SetRewardMultiplierFactorWed(int n) { m_nRewardMultiplierFactorWed = n; }
	void SetRewardMultiplierFactorThu(int n) { m_nRewardMultiplierFactorThu = n; }
	void SetRewardMultiplierFactorFri(int n) { m_nRewardMultiplierFactorFri = n; }
	void SetRewardMultiplierFactorSat(int n) { m_nRewardMultiplierFactorSat = n; }
	void SetRewardMultiplierFactorSun(int n) { m_nRewardMultiplierFactorSun = n; }
	void SetRewardMultiplierTrigger(double d) { m_dRewardMultiplierTrigger = d; }
	void SetRewardIncludePurse1SpendFlag(bool b) { m_bRewardIncludePurse1Spend = b; }
	void SetRewardIncludePurse2SpendFlag(bool b) { m_bRewardIncludePurse2Spend = b; }
	void SetRewardIncludePurse3SpendFlag(bool b) { m_bRewardIncludePurse3Spend = b; }
	void SetRewardIncludeCashSpendFlag(bool b) { m_bRewardIncludeCashSpend = b; }
	void SetRewardType(int n) { m_nRewardType = n; }
	void SetRewardComment(CString str) { m_strRewardComment = str; }
	void SetMonStart ( CSSTime time ) { m_timeMonStart = time; }
	void SetMonEnd   ( CSSTime time ) { m_timeMonEnd = time; }
	void SetTueStart ( CSSTime time ) { m_timeTueStart = time; }
	void SetTueEnd   ( CSSTime time ) { m_timeTueEnd = time; }
	void SetWedStart ( CSSTime time ) { m_timeWedStart = time; }
	void SetWedEnd   ( CSSTime time ) { m_timeWedEnd = time; }
	void SetThuStart ( CSSTime time ) { m_timeThuStart = time; }
	void SetThuEnd   ( CSSTime time ) { m_timeThuEnd = time; }
	void SetFriStart ( CSSTime time ) { m_timeFriStart = time; }
	void SetFriEnd   ( CSSTime time ) { m_timeFriEnd = time; }
	void SetSatStart ( CSSTime time ) { m_timeSatStart = time; }
	void SetSatEnd   ( CSSTime time ) { m_timeSatEnd = time; }
	void SetSunStart ( CSSTime time ) { m_timeSunStart = time; }
	void SetSunEnd   ( CSSTime time ) { m_timeSunEnd = time; }
	void SetMonStart(int nHour, int nMinute) { m_timeMonStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetMonEnd(int nHour, int nMinute) { m_timeMonEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetTueStart(int nHour, int nMinute) { m_timeTueStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetTueEnd(int nHour, int nMinute) { m_timeTueEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetWedStart(int nHour, int nMinute) { m_timeWedStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetWedEnd(int nHour, int nMinute) { m_timeWedEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetThuStart(int nHour, int nMinute) { m_timeThuStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetThuEnd(int nHour, int nMinute) { m_timeThuEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetFriStart(int nHour, int nMinute) { m_timeFriStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetFriEnd(int nHour, int nMinute) { m_timeFriEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetSatStart(int nHour, int nMinute) { m_timeSatStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetSatEnd(int nHour, int nMinute) { m_timeSatEnd.SetTime(GetTimeString(nHour, nMinute)); }
	void SetSunStart(int nHour, int nMinute) { m_timeSunStart.SetTime(GetTimeString(nHour, nMinute)); }
	void SetSunEnd(int nHour, int nMinute) { m_timeSunEnd.SetTime(GetTimeString(nHour, nMinute)); }
	//REDEEM POINTS
	void SetFileRedeemPointsTrip(int n) { m_nFileRedeemPointsTrip = n; }
	void SetRedeemPurse(int n) { m_nRedeemPurse = n; }
	void SetRedeemValue(double d) { m_dRedeemValue = d; }
	void SetRedeemDisableAutoFlag(bool b) { m_bRedeemDisableAuto = b; }
	void SetRedeemComment(CString str) { m_strRedeemComment = str; }
	//NON UI
	void SetSearchMax(int n) { m_nSearchMax = n; }
	void SetAllowRetriesFlag(bool b) { m_bAllowRetries = b; }
	void SetAllowPebbleAdjustCode(int n) { m_nAllowPebbleAdjust = n; }
	void SetAllowPINAdminCode(int n) { m_nAllowPINAdmin	 = n; } 
	void SetLockRetries(int n);
	void SetThreadDiagnosticsFlags(int n) { m_nThreadDiagnostics = n; }
	void SetBypassVersionSafetyCheckFlag(bool b) { m_bBypassVersionSafetyCheck = b; }
	void SetCopyLogFilesFlag(bool b) { m_bCopyLogFiles = b; }
	void SetLockFolderTimeout(int n);
	void SetBromComTestModeFlag(bool b) { m_bBromComTestMode = b; }
	static void SetSimulateReaderType(int n);
	void SetPebbleDemoFlag(bool b);
	void SetPebbleDiagnosticType(int n);
	void SetLogCardReadsFlag(bool b) { m_bLogCardReads = b; }
	
public:
	bool ValidateUserID ( const char* szInputUserNo, CString& strOutputUserNo );
	const char* GetError(){ return m_strError; }
	bool RedeemPointsEnabled() { return (GetActiveRedeemPointsTrip() != 0); }
	bool RevaluationPlu2Inuse();
	bool RevaluationBonusEnabled();
	bool SpendBonusEnabled();

public:
	void SetPebbleDiagnosticMessageBoxFlag( bool b ) { m_bPebbleDiagnosticMessageBox = b; }
	bool GetPebbleDiagnosticMessageBoxFlag() { return m_bPebbleDiagnosticMessageBox; }

private:
	CString GetTimeString(int nHour, int nMinute);

private:
	//COMMUNICATIONS
	bool m_bLogAction2;
	bool m_bAllowOverdraft;
	bool m_bAddUnknowns;
	CString m_strOfflineSpendID;
	double m_dOfflineSpendLimit;
	int m_nUnknownGroup;
	bool m_bNewBalanceOnReceipt;
	int m_nForenameSearchFieldType;
	CString m_strEposPathLabel[EPOSREQPATH_COUNT];
	CString m_strEposRequestPath[EPOSREQPATH_COUNT];
	CString m_strEposLogPath[EPOSREQPATH_COUNT];
	bool m_bExternalRequestPath[EPOSREQPATH_COUNT];
	//PHOTO ID
	int m_nPhotoIDFolderType;
	CString m_strSmartPayUNCSystemPath;
	CString m_strSmartPayUNCImagePath;
	//PLU SALES
	CString m_strPluNo;
	CString m_strPluNo2;
	double m_dFileRevaluationTrip;
	double m_dFileRevaluationTrip2;
	int m_nRevaluationPoints;
	double m_dRevaluationValue;
	int m_nRevaluationType;
	CString m_strRevaluationComment;
	bool m_bFileEnablePurchaseHistory;
	bool m_bDisablePurchaseHistoryAccountZero;
	CString m_strEcrManagerImportFolder;
	/*****/
	CString m_strEcrManagerImportPluFilename;
	CString m_strEcrManagerImportOperatorFilename;
	CString m_strEcrManagerImportEposTerminalFilename;
	CString m_strEcrManagerImportPurchaseControlFilename;
	//SPEND BONUS
	double m_dRewardUpperSpendLimit;
	double m_dFileRewardTrip;
	int m_nRewardPoints;
	double m_dRewardValue;
	int m_nRewardMultiplierFactorMon;
	int m_nRewardMultiplierFactorTue;
	int m_nRewardMultiplierFactorWed;
	int m_nRewardMultiplierFactorThu;
	int m_nRewardMultiplierFactorFri;
	int m_nRewardMultiplierFactorSat;
	int m_nRewardMultiplierFactorSun;
	double m_dRewardMultiplierTrigger;
	bool m_bRewardIncludePurse1Spend;
	bool m_bRewardIncludePurse2Spend;
	bool m_bRewardIncludePurse3Spend;
	bool m_bRewardIncludeCashSpend;
	int m_nRewardType;
	CString m_strRewardComment;
	CSSTime m_timeMonStart;
	CSSTime m_timeMonEnd;
	CSSTime m_timeTueStart;
	CSSTime m_timeTueEnd;
	CSSTime m_timeWedStart;
	CSSTime m_timeWedEnd;
	CSSTime m_timeThuStart;
	CSSTime m_timeThuEnd;
	CSSTime m_timeFriStart;
	CSSTime m_timeFriEnd;
	CSSTime m_timeSatStart;
	CSSTime m_timeSatEnd;
	CSSTime m_timeSunStart;
	CSSTime m_timeSunEnd;
	//REDEEM POINTS
	int m_nFileRedeemPointsTrip;
	int m_nRedeemPurse;
	double m_dRedeemValue;
	CString m_strRedeemComment;
	bool m_bRedeemDisableAuto;
	//NON UI
	int m_nSearchMax;
	bool m_bAllowRetries;
	int m_nAllowPebbleAdjust;
	int m_nAllowPINAdmin;
	int m_nLockRetries;
	int m_nThreadDiagnostics;
	bool m_bBypassVersionSafetyCheck;
	bool m_bCopyLogFiles;
	int m_nLockFolderTimeout;
	static int m_nSimulateReaderType;
	bool m_bBromComTestMode;
	bool m_bPebbleDemo;
	bool m_bLockPebbleDemoFlag;
	int m_nPebbleDiagnosticType;
	bool m_bLogCardReads;

private:
	CString m_strError;
	int m_nCurrentEposPathSet;
	CString m_strTimeStamp;	
	bool m_bPebbleDiagnosticMessageBox;
};

//*******************************************************************
extern CServerData Server;
extern CServerData ServerCopy;
//*******************************************************************
