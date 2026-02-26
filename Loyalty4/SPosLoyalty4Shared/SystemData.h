#pragma once
//$$******************************************************************
#include "AccountIdentifier.h"
//$$******************************************************************
#define nINTERFACE_SWIPEv1		0			// MagSwipe - Short (upto 6 digit)
#define nINTERFACE_SWIPEv2		1			// MagSwipe - Long ( prefix + upto 6 digit ) 
#define nINTERFACE_MIFAREv1		2			// CardNo = MiFare number (10 digits)
#define nINTERFACE_MIFAREv2		3			// ExtRef field = MiFare number
#define nINTERFACE_GENERALv1	4			// General number up to 12 digits
#define nINTERFACE_MIFAREv3		5			// As MiFare2 but with balance search by card number
#define nINTERFACE_QRINFO		6			// Used internally for QR=xxxx REQ file
#define nINTERFACE_SIMPSINNS	7			// Used internally for SimpsInn Legacy BlueRunner
//$$******************************************************************
#define nREDEEM_STANDARD		0
#define nREDEEM_FAMILY			1
//$$******************************************************************
#define MAX_PATHLEN_DBBACKUP	100
//$$******************************************************************
#define MAX_QRCODE_PREFIX 50
//$$******************************************************************

namespace SystemDataFields
{
	//GENERAL TAB
	const char* const InterfaceType = "Interface";
	const char* const CardNoPrefix = "Prefix";
	const char* const CheckDigitFlag = "CheckDigit";
	const char* const ShowQRInfoFlag = "ShowQRInfo";
	const char* const EnablePurse2Flag = "EnablePurse2";
	const char* const Purse2PendingRefreshFlag = "Purse2PendingRefresh";
	const char* const SignOnMethodOld = "Passwords";
	const char* const SignOnMethodNew = "Misc1";
	const char* const ExternalAccountFlag = "ExternalAccount";
	const char* const SrvImportUnknownFlag = "SrvImportUnknown";
	const char* const EnableFamilyFlag = "EnableFamily";
	const char* const PaxtonModeFlag = "PaxtonMode";
	const char* const QRCodeEmbedFlag = "QRCodeEmbedFlag";
	const char* const QRCodeEmbedPrefix = "QRCodeEmbedPrefix";

	//EDITING TAB
	const char* const ListRefreshRate = "ListRefreshRate";
	const char* const DefaultAccountEditPage = "EditPage";
	const char* const ShowButtonManualBonus = "ShowButtonManualBonus";
	const char* const ShowButtonPurse1Topup = "ShowButtonPurse1Topup";
	const char* const ShowButtonPurse2Topup = "ShowButtonPurse2Topup";
	const char* const ShowButtonPurse1Spend = "ShowButtonPurse1Spend";
	const char* const ShowButtonPointsTopup = "ShowButtonPointsTopup";
	const char* const ShowButtonPointsRedeem = "ShowButtonPointsRedeem";
	const char* const EditPurse1Flag = "EditPurse1";
	const char* const EditPurse2Flag = "EditPurse2";
	const char* const EditPointsFlag = "EditPoints";
	const char* const EditCashSpendFlag = "EditCashSpend";
	const char* const EnableBatchUpdatesFlag = "BatchUpdates";
	const char* const EnableBatchPointsFlag = "BatchPoints";
	const char* const EnableBatchPurse1Flag = "BatchPurse1";
	const char* const EnableBatchPurse2Flag = "BatchPurse2";
	const char* const EnableBatchSpendTDFlag = "BatchSpendTD";
	const char* const ContextMenuPurse1TopupFlag = "ContextMenuPurse1Topup";
	const char* const ContextMenuPurse2TopupFlag = "ContextMenuPurse2Topup";
	const char* const AccountFindValidFlag = "AccountFindValid";
	const char* const AccountFindExpiredFlag = "AccountFindExpired";
	const char* const AccountFindHotlistedFlag = "AccountFindHotlisted";
	const char* const AccountFindAlertSetFlag = "AccountFindAlertSet";
	const char* const AccountFindInactiveFlag = "AccountFindInactive";
	const char* const AccountFindUsernameFlag = "AccountFindUsername";
	const char* const AccountFindForenameFlag = "AccountFindForename";
	const char* const AccountFindSurnameFlag = "AccountFindSurname";
	const char* const AccountFindAddressFlag = "AccountFindAddress";
	const char* const AccountFindPostcodeFlag = "AccountFindPostcode";
	const char* const AccountFindPhoneFlag = "AccountFindPhone";
	const char* const AccountFindInfoFlag = "AccountFindInfo";
	const char* const EnableAccountColourFlag = "AccountColour";
	const char* const AccountPreloadFlag = "AccountPreload";
	const char* const AccountCancelPrompt = "AccountCancelPrompt";

	//EXPORT TAB
	const char* const ExportOnExitFlag = "ExportExit";
	const char* const ExportOnDeleteFlag = "ExportDelete";

	//REPORTING TAB
	const char* const StartOfWeek = "StartOfWeek";
	const char* const EnablePhotoIDListFlag = "PhotoIDList";
	const char* const ShowAuditPurchasesFlag = "ShowPurchases";
};

//$$******************************************************************

class CSystemData  
{
public:
	CSystemData();
	bool Read();
	bool Write();
	bool RewindExternal();
	void FixInternalSettings();

	bool HaveBarman();
	bool IsSwipeSystem();
	bool IsMiFareSystem();
	bool IsBackupInProgress ( CWnd* pParent );

	CString TranslateCardNo ( CAccountIdentifier& AccountIdentifier, CString& strOutputCardNo, bool bFromCheckBalance = FALSE);

	const char* GetCardNoText();
	int GetMaxCardNoDlgLen();
	int GetUserIDLength();
	__int64 GetMaxCardNo(); 
	CString FormatCardNo ( CString strCardNo, bool bPadOut = TRUE );
	CString FormatCardNo ( __int64 nCardNo, bool bPadOut );
	CString FormatFullCardNo ( CString strCardNo );

public:
	//GENERAL TAB
	int GetInterfaceType() { return m_nInterfaceType; }
	CString GetCardNoPrefix() { return m_strCardNoPrefix; }
	bool GetCheckDigitFlag() { return m_bCheckDigit; }
	bool GetShowQRInfoFlag() { return m_bShowQRInfo; }
	bool GetEnablePurse2Flag() { return m_bEnablePurse2; }
	bool GetPurse2PendingRefreshFlag() { return m_bPurse2PendingRefresh; }
	int GetSignOnMethod() { return m_nSignOnMethod; }
	bool GetSrvImportUnknownFlag() { return m_bSrvImportUnknown; }
	bool GetExternalAccountFlag() { return m_bExternalAccount; }
	bool GetPaxtonModeFlag() { return FALSE; }
	bool GetEnableFamilyFlag() { return FALSE; }
	bool GetQRCodeEmbedFlag() { return m_bQRCodeEmbedFlag; }
	CString GetQRCodeEmbedPrefix() { return m_strQRCodeEmbedPrefix; }

	//EDITING TAB
	int GetListRefreshRate(){ return m_nListRefreshRate; }
	int GetDefaultAccountEditPage() { return m_nDefaultAccountEditPage; }
	bool GetShowButtonManualBonusFlag() { return m_bShowButtonManualBonus; }
	bool GetShowButtonPurse1TopupFlag() { return m_bShowButtonPurse1Topup; }
	bool GetShowButtonPurse2TopupFlag() { return m_bShowButtonPurse2Topup; }
	bool GetShowButtonPurse1SpendFlag() { return m_bShowButtonPurse1Spend; }
	bool GetShowButtonPointsTopupFlag() { return m_bShowButtonPointsTopup; }
	bool GetShowButtonPointsRedeemFlag() { return m_bShowButtonPointsRedeem; }
	bool GetEditPurse1Flag() { return m_bEditPurse1; }
	bool GetEditPurse2Flag() { return m_bEditPurse2; }
	bool GetEditPointsFlag() { return m_bEditPoints; }
	bool GetEditCashSpendFlag() { return m_bEditCashSpend; }
	bool GetEnableBatchUpdatesFlag() { return m_bEnableBatchUpdates; }
	bool GetBatchUpdatePointsFlag() { return m_bBatchUpdatePoints; }
	bool GetBatchUpdatePurse1Flag() { return m_bBatchUpdatePurse1; }
	bool GetBatchUpdatePurse2Flag() { return m_bBatchUpdatePurse2; }
	bool GetBatchUpdateSpendTDFlag() { return m_bBatchUpdateSpendTD; }
	bool GetContextMenuPurse1TopupFlag() { return m_bContextMenuPurse1Topup; }
	bool GetContextMenuPurse2TopupFlag() { return m_bContextMenuPurse2Topup; }
	bool GetAccountFindValidFlag() { return m_bAccountFindValid; }
	bool GetAccountFindExpiredFlag() { return m_bAccountFindExpired; }
	bool GetAccountFindHotlistedFlag() { return m_bAccountFindHotlisted; }
	bool GetAccountFindAlertSetFlag() { return m_bAccountFindAlertSet; }
	bool GetAccountFindInactiveFlag() { return m_bAccountFindInactive; }
	bool GetAccountFindUsernameFlag() { return m_bAccountFindUsername; }
	bool GetAccountFindForenameFlag() { return m_bAccountFindForename; }
	bool GetAccountFindSurnameFlag() { return m_bAccountFindSurname; }
	bool GetAccountFindAddressFlag() { return m_bAccountFindAddress; }
	bool GetAccountFindPostcodeFlag() { return m_bAccountFindPostcode; }
	bool GetAccountFindPhoneFlag() { return m_bAccountFindPhone; }
	bool GetAccountFindInfoFlag() { return m_bAccountFindInfo; }
	bool GetAccountPreloadFlag() { return m_bAccountPreload; }
	bool GetEnableAccountColourFlag() { return m_bEnableAccountColour; }
	bool GetAccountCancelPromptFlag() { return m_bAccountCancelPrompt; }

	//EXPORT TAB
	bool GetExportOnExitFlag() { return m_bExportOnExit; }
	bool GetExportOnDeleteFlag() { return m_bExportOnDelete; }

	//REPORTING TAB
	int GetStartOfWeek(){ return m_nStartOfWeek; }
	bool GetEnablePhotoIDListFlag() { return m_bEnablePhotoIDList; }
	bool GetShowAuditPurchasesFlag() { return m_bShowAuditPurchases; }
	
	//VALUES NOT SAVED IN FILE
	int GetDPFactor() { return m_nDPFactor; }
	int GetDPValue() { return m_nDPValue; }
	bool GetGotPCIdFlag() { return m_bGotPCId; }
	bool GetDoneSQLFileSizeFlag() { return m_bDoneSQLFileSize; }
	bool GetDoneSQLShrinkLogFlag() { return m_bDoneSQLShrinkLog; }
	bool IsSPOSSMTPServerSystem() { return (Sysset.GetExternalLink() & 0x2000) == 0x2000; }
	int GetMaxEcrmanDatabases();

	//CARD RANGES
	int GetMaxCardNoLength() { return m_nMaxCardNoLength; }
	CString GetLowCardNo() { return m_strLowCardNo; }
	CString GetHighCardNo() { return m_strHighCardNo; }
	CString GetBlankCardNo() { return m_strBlankCardNo; }

public:
	//GENERAL TAB
	void SetInterfaceType(int n);
	void SetCardNoPrefix(CString str) { m_strCardNoPrefix = str; }
	void SetCheckDigitFlag(bool b) { m_bCheckDigit = b; }
	void SetShowQRInfoFlag(bool b) { m_bShowQRInfo = b; }
	void SetEnablePurse2Flag(bool b) { m_bEnablePurse2 = b; }
	void SetPurse2PendingRefreshFlag(bool b) { m_bPurse2PendingRefresh = b; }
	void SetSignOnMethod(int n);
	void SetSrvImportUnknownFlag(bool b) { m_bSrvImportUnknown = b; }
	void SetExternalAccountFlag(bool b) { m_bExternalAccount = b; }
	void SetPaxtonModeFlag(bool b) { m_bPaxtonMode = b; }
	void SetQRCodeEmbedFlag(bool b) { m_bQRCodeEmbedFlag = b; }
	void SetQRCodeEmbedPrefix(CString str);
	
	//EDITING TAB
	void SetListRefreshRate( int n );
	void SetDefaultAccountEditPage(int n) { m_nDefaultAccountEditPage = n; }
	void SetShowButtonManualBonusFlag(bool b) { m_bShowButtonManualBonus = b; }
	void SetShowButtonPurse1TopupFlag(bool b) { m_bShowButtonPurse1Topup = b; }
	void SetShowButtonPurse2TopupFlag(bool b) { m_bShowButtonPurse2Topup = b; }
	void SetShowButtonPurse1SpendFlag(bool b) { m_bShowButtonPurse1Spend = b; }
	void SetShowButtonPointsTopupFlag(bool b) { m_bShowButtonPointsTopup = b; }
	void SetShowButtonPointsRedeemFlag(bool b) { m_bShowButtonPointsRedeem = b; }
	void SetEditPurse1Flag(bool b) { m_bEditPurse1 = b; }
	void SetEditPurse2Flag(bool b) { m_bEditPurse2 = b; }
	void SetEditPointsFlag(bool b) { m_bEditPoints = b; }
	void SetEditCashSpendFlag(bool b) { m_bEditCashSpend = b; }
	void SetEnableBatchUpdatesFlag(bool b) { m_bEnableBatchUpdates = b; }
	void SetBatchUpdatePointsFlag(bool b) { m_bBatchUpdatePoints = b; }	
	void SetBatchUpdatePurse1Flag(bool b) { m_bBatchUpdatePurse1 = b; }
	void SetBatchUpdatePurse2Flag(bool b) { m_bBatchUpdatePurse2 = b; }
	void SetBatchUpdateSpendTDFlag(bool b) { m_bBatchUpdateSpendTD = b; }
	void SetContextMenuPurse1TopupFlag(bool b) { m_bContextMenuPurse1Topup = b; }
	void SetContextMenuPurse2TopupFlag(bool b) { m_bContextMenuPurse2Topup = b; }
	void SetAccountFindValidFlag(bool b) { m_bAccountFindValid = b; }
	void SetAccountFindExpiredFlag(bool b) { m_bAccountFindExpired = b; }
	void SetAccountFindHotlistedFlag(bool b) { m_bAccountFindHotlisted = b; }
	void SetAccountFindAlertSetFlag(bool b) { m_bAccountFindAlertSet = b; }
	void SetAccountFindInactiveFlag(bool b) { m_bAccountFindInactive = b; }
	void SetAccountFindUsernameFlag(bool b) { m_bAccountFindUsername = b; }
	void SetAccountFindForenameFlag(bool b) { m_bAccountFindForename = b;	}
	void SetAccountFindSurnameFlag(bool b) { m_bAccountFindSurname = b; }
	void SetAccountFindAddressFlag(bool b) { m_bAccountFindAddress = b; }
	void SetAccountFindPostcodeFlag(bool b) { m_bAccountFindPostcode = b; }
	void SetAccountFindPhoneFlag(bool b) { m_bAccountFindPhone = b; }
	void SetAccountFindInfoFlag(bool b) { m_bAccountFindInfo = b; }
	void SetEnableAccountColourFlag(bool b) { m_bEnableAccountColour = b; }
	void SetAccountPreloadFlag(bool b) { m_bAccountPreload = b; }
	void SetAccountCancelPromptFlag(bool b) { m_bAccountCancelPrompt = b; }
	
	//EXPORT TAB
	void SetExportOnExitFlag(bool b) { m_bExportOnExit = b; }
	void SetExportOnDeleteFlag(bool b) { m_bExportOnDelete = b; }

	//REPORTING TAB
	void SetStartOfWeek( int n );
	void SetEnablePhotoIDListFlag(bool b) { m_bEnablePhotoIDList = b; }
	void SetShowAuditPurchasesFlag(bool b) { m_bShowAuditPurchases = b; }
	
	//VALUES NOT SAVED IN FILE
	void SetDPFactor( int n ) { m_nDPFactor = n; }
	void SetDPValue( int n ) { m_nDPValue = n; }
	void SetGotPCIdFlag(bool b) { m_bGotPCId = b; }
	void SetDoneSQLFileSizeFlag(bool b) { m_bDoneSQLFileSize = b; }
	void SetDoneSQLShrinkLogFlag(bool b) { m_bDoneSQLShrinkLog = b; }
	
public:
	bool LockAllUsers();
	void UnlockAllUsers();

public:
	void GetBetaTypeForBackup(CString& str) { str = "0"; }

private:
	void ConvertOldCSVFile();
	void RewindInternal(CSSIniFile& file, bool bNewFileNo);
	CString ExtractSwipe1Format ( CString strInputCardNo, CString& strOutputCardNo );
	CString ExtractSwipe2Format (CString strInputCardNo, CString& strOutputCardNo );
	CString ExtractMiFare1Format (CString strMiFareNo, CString& strOutputCardNo );
	CString ExtractMiFare2Format (CString strMiFareNo, CString& strOutputCardNo );
	CString ExtractMiFare3Format (CString strMiFareNo, CString& strOutputCardNo, bool bFromCheckBalance = FALSE );
	CString ExtractQRInfoFormat (CString strQRInfo, CString& strOutputCardNo);
	CString ExtractGeneralFormat (CString strInputCardNo, CString& strOutputCardNo );
	CString ExtractSimpsInnsFormat(CString strInputCardNo, CString& strOutputCardNo);
	int CalculateCheckSum (CString strCardNo );

private:
	void RewindAndUpdateBool(CSSIniFile& file, bool& bField, CString strNewLabel, CString strLegacyLabel);
	
private:
	//GENERAL TAB
	int m_nInterfaceType;
	CString m_strCardNoPrefix;
	bool m_bCheckDigit;
	bool m_bShowQRInfo;
	bool m_bEnablePurse2;
	bool m_bPurse2PendingRefresh;
	int m_nSignOnMethod;
	bool m_bSrvImportUnknown;
	bool m_bExternalAccount;
	bool m_bPaxtonMode;
	bool m_bQRCodeEmbedFlag;
	CString m_strQRCodeEmbedPrefix;

	//EDITING TAB
	int m_nListRefreshRate;
	int m_nDefaultAccountEditPage;
	bool m_bShowButtonManualBonus;
	bool m_bShowButtonPurse1Topup;
	bool m_bShowButtonPurse2Topup;
	bool m_bShowButtonPurse1Spend;
	bool m_bShowButtonPointsTopup;
	bool m_bShowButtonPointsRedeem;
	bool m_bEditPurse1;
	bool m_bEditPurse2;
	bool m_bEditPoints;
	bool m_bEditCashSpend;
	bool m_bEnableBatchUpdates;
	bool m_bBatchUpdatePoints;
	bool m_bBatchUpdatePurse1;
	bool m_bBatchUpdatePurse2;
	bool m_bBatchUpdateSpendTD;
	bool m_bContextMenuPurse1Topup;
	bool m_bContextMenuPurse2Topup;
	bool m_bAccountFindValid;
	bool m_bAccountFindExpired;
	bool m_bAccountFindHotlisted;
	bool m_bAccountFindAlertSet;
	bool m_bAccountFindInactive;
	bool m_bAccountFindUsername;
	bool m_bAccountFindForename;
	bool m_bAccountFindSurname;
	bool m_bAccountFindAddress;
	bool m_bAccountFindPostcode;
	bool m_bAccountFindPhone;
	bool m_bAccountFindInfo;
	bool m_bEnableAccountColour;
	bool m_bAccountPreload;
	bool m_bAccountCancelPrompt;

	//EXPORT TAB
	bool m_bExportOnExit;
	bool m_bExportOnDelete;

	//REPORTING TAB
	int m_nStartOfWeek;
	bool m_bEnablePhotoIDList;
	bool m_bShowAuditPurchases;

	//CARD RANGES
	int m_nMaxCardNoLength;
	CString m_strLowCardNo;
	CString m_strHighCardNo;
	CString m_strBlankCardNo;
	int m_nUserLockCount;

	//VALUES NOT SAVED IN FILE
	int m_nDPFactor;							
	int m_nDPValue;								
	bool m_bGotPCId;
	bool m_bDoneSQLFileSize;
	bool m_bDoneSQLShrinkLog;
};

//$$******************************************************************
extern CSystemData System;
//$$******************************************************************
