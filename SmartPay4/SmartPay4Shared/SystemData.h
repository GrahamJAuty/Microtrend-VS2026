#pragma once
//*******************************************************************
#define nINTERFACE_BIOMETRIC		0
#define nINTERFACE_MIFAREv1			1			// UserID = MiFare number
#define nINTERFACE_MIFAREv2			2			// ExtRef = MiFare number
#define nINTERFACE_DUAL				3			// Biometric & MiFarev2
//*******************************************************************
#define nWEBPAYMENT_NONE			0			// m_nWebPaymentTypes
#define nWEBPAYMENT_PARENTPAY		1
#define nWEBPAYMENT_TUCASI			2			// Tucasi (SOAP)
#define nWEBPAYMENT_WISEPAY			3
#define nWEBPAYMENT_SCHOOLCOMMS		4
#define nWEBPAYMENT_TUCASIv2		5			// TUCASI (JSON)	
#define nWEBPAYMENT_SQUID			6			// 
#define nWEBPAYMENT_PARENTMAIL		7
#define nWEBPAYMENT_BUCKINGHAM		8			// not implemented
#define nWEBPAYMENT_BROMCOM			9
#define nWEBPAYMENT_PEBBLE			10
//*******************************************************************
#define nWEBPAYMENT_EXPORTSALES		99			// System Export sales \ topups to csv file (SFX college )
//*******************************************************************
#define nIMPORT_METHOD_ALL			0
#define nIMPORT_METHOD_NEWONLY		1
#define nIMPORT_METHOD_EXISTONLY	2
//*******************************************************************
#define nIMPORT_FORMAT_CSV			0
#define nIMPORT_FORMAT_SIMS			1
#define nIMPORT_FORMAT_CUSTOM		2
#define nIMPORT_FORMAT_WONDE	3
//*******************************************************************
#define MAX_PATHLEN_DBBACKUP 100
#define REPORT_ENDTEXT_COUNT 5
#define MAX_GROUPSHIFT_DURATION 999
//*******************************************************************

namespace SystemDataFields
{
	//GENERAL
	const char* const Interface = "Interface";
	const char* const AutoDeletePCtrlType = "AutoDeletePCtrlType";
	const char* const AssumeBioConsent = "AssumeBioConsent";
	const char* const AllowPINNumbers = "AllowPINNumbers";
	const char* const GroupSets = "GroupSets";
	const char* const Passwords = "Passwords";
	const char* const Background = "Background";
	const char* const AllowGroupShiftDateRange = "AllowGroupShiftDateRange";
	const char* const GroupShiftDurations = "GroupShiftDurations";
	const char* const AllowGroupShiftOnDuty = "AllowGroupShiftOnDuty";
	const char* const AST = "AST";
	//REPORTING
	const char* const StartOfWeek = "StartOfWeek";
	const char* const PhotoIDList = "PhotoIDList";
	const char* const LeaversDbase = "LeaversDbase";
	const char* const EcrmanPath = "EcrmanPath";
	const char* const PurchaseTexts = "PurchaseTexts";
	const char* const ReportEndText = "ReportEndText";
	const char* const BioConsentTickNo = "BioConsentTickNo";
	//EDITING
	const char* const CashButton = "CashButton";
	const char* const Purse1 = "Purse1";
	const char* const Purse2 = "Purse2";
	const char* const Purse3Required = "Purse3Reqd";
	const char* const Purse3 = "Purse3";
	const char* const PointsRequired = "PointsReqd";
	const char* const Points = "Points";
	const char* const PointsButton = "PointsButton";
	const char* const ExternalAccount = "ExternalAccount";
	const char* const BatchUpdates = "BatchUpdates";
	const char* const NoAudit = "NoAudit";
	const char* const EditPage = "EditPage";
	const char* const EnableAccountColour = "AccountColour";
	const char* const AccountCancelPrompt = "AccountCancelPrompt";
	const char* const AuditImportFilter = "AuditImportFilter";
	//EXPORT
	const char* const ExportSales = "ExportSales";
	const char* const ExportOnExit = "ExportOnExit";
	const char* const ExportOnDelete = "ExportOnDelete";
	//INTERFACES
	const char* const WebPayment = "WebPayment";
	const char* const Chartwells = "Chartwells";
	const char* const EnableSims = "EnableSims";
	const char* const AutoImport = "AutoImport";
	const char* const EnableWonde = "EnableWonde";
	const char* const EnableGroupcall = "EnableGroupcall";
	const char* const ImportExtRef1TrimZero = "ImportExtRef1TrimZero";
	//EPOS
	const char* const EposPhotoFolder = "EposPhotoFolder";
	const char* const EposPhotoExport = "EposPhotoExport";
	//NON UI
	const char* const CreditOptions = "CreditOptions";
	const char* const AuditFileFilter = "AuditFileFilter";
};

//*******************************************************************

class CSystemData  
{
public:
	CSystemData();
	bool Read();
	bool Rewind();
	bool Write();

	bool IsMiFareID ( const char* szUserID );

	int GetUserIDLength();
	__int64 GetMaxCardNo(); 
	bool IsValidCardNo ( const char* szCardNo );
	CString FormatCardNo ( const char* szCardNo, bool bPadOut = TRUE );
	CString FormatCardNo ( __int64 nCardNo, bool bPadOut );

#ifndef SYSTEMTYPE_EMAILBATCHSEND
	CString GetWebPaymentPath ( const char* szFilename, int nWebPaymentType = nWEBPAYMENT_NONE );
	CString TranslateMiFareNo(const char* szMiFareNo, int nMiFareDbNo = 1);
#endif

	CString GetWebPaymentDisplayName ( int nWebPaymentType = nWEBPAYMENT_NONE );

public:
	static CString GetInternetAgentString() { return "SmartPay4"; }
	
public:
	void SetGotPCIdFlag(bool b) { m_bGotPCId = b; }
	bool GetGotPCIdFlag() { return m_bGotPCId; }

public:
	bool LockAllUsers();
	void UnlockAllUsers();

public:
	void SystemLock();
	void SystemUnlock();

public:
	void GetBetaTypeForBackup(CString& str) { str = "0"; }

public:
	//GENERAL
	int GetInterfaceType() { return m_nInterfaceType; }
	int GetAutoDeletePCtrlType() { return m_nAutoDeletePCtrlType; }	
	bool GetAssumeBioConsentFlag() { return m_bAssumeBioConsent; }
	bool GetAllowPINNumbersFlag() { return m_bAllowPINNumbers; }
	bool GetAllowGroupSetsFlag() { return m_bAllowGroupSets;  }
	bool GetUsePasswordsFlag() { return m_bUsePasswords; }
	bool GetEnableBackgroundFlag() { return m_bEnableBackground; }
	int GetAutoShutdownType() { return m_nAutoShutdownType; }
	bool GetAllowGroupShiftDateRangeFlag() { return m_bAllowGroupShiftDateRange; }
	CString GetGroupShiftDurations() { return m_strGroupShiftDurations; }
	bool GetAllowGroupShiftOnDutyFlag() { return m_bAllowGroupShiftOnDuty; }
	//REPORTING
	int GetStartOfWeek() { return m_nStartOfWeek; }
	bool GetEnablePhotoIDListFlag() { return m_bEnablePhotoIDList; }
	bool GetKeepLeaversDatabaseFlag() { return m_bKeepLeaversDatabase; }
	CString GetEcrmanPath();
	bool GetShowPurchaseTextsFlag() { return m_bShowPurchaseTexts; }
	CString GetReportEndText(int n);
	CString GetReportEndTextLabel(int n);
	int GetBioConsentTickNo() { return m_nBioConsentTickNo; }
	//EDITING
	bool GetShowTopupButtonFlag() { return m_bShowTopupButton; }
	bool GetFileEditPurse1Flag() { return m_bFileEditPurse1; }
	bool GetFileEditPurse2Flag() { return m_bFileEditPurse2; }
	bool GetEnablePurse3Flag() { return m_bEnablePurse3; }
	bool GetFileEditPurse3Flag() { return m_bFileEditPurse3; }
	bool GetFileEnablePointsFlag() { return m_bFileEnablePoints; }
	bool GetFileEditPointsFlag() { return m_bFileEditPoints; }
	bool GetFileShowPointsButtonFlag() { return m_bFileShowPointsButton; }
	bool GetExternalAccountFlag() { return m_bExternalAccount; }
	bool GetEnableBatchUpdatesFlag() { return m_bEnableBatchUpdates; }
	bool GetEnableNoAuditFlag() { return m_bEnableNoAudit; }
	int GetDefaultAccountEditPage() { return m_nDefaultAccountEditPage; }
	bool GetEnableAccountColourFlag() { return m_bEnableAccountColour; }
	bool GetAccountCancelPromptFlag() { return m_bAccountCancelPrompt; }	
	bool GetAuditImportFilterFlag() { return m_bAuditImportFilter; }
	//EXPORT
	bool GetEnableExportSalesFlag() { return m_bEnableExportSales; }
	bool GetExportOnDeleteFlag() { return m_bExportOnDelete; }
	bool GetExportOnExitFlag() { return m_bExportOnExit; }
	//INTERFACE
	int GetWebPaymentType() { return m_nWebPaymentType; }
	bool GetEnableChartwellsFlag() { return m_bEnableChartwells; }
	bool GetEnableSimsFlag() { return m_bEnableSims; }
	bool GetEnableAutoImportFlag() { return m_bEnableAutoImport; }
	bool GetEnableWondeFlag() { return m_bEnableWonde; }
	bool GetImportExtRef1TrimZeroFlag() { return m_bImportExtRef1TrimZero; }
	//EPOS
	CString GetEposPhotoIDFolder() { return m_strEposPhotoIDFolder; }
	bool GetExportEposPhotoFlag() { return m_bExportEposPhoto; }
	//NON UI
	bool GetCreditOptionsFlag() { return m_bCreditOptions; }
	bool GetAuditFileFilterFlag() { return m_bAuditFileFilter; }

#ifndef SYSTEMTYPE_SERVER
	bool IsSPOSSMTPServerSystem() { return (Sysset.GetExternalLink() & 0x2000) == 0x2000; }
#else
	bool IsSPOSSMTPServerSystem() { return FALSE; }
#endif

	//ACTIVE FUNCTIONS RETURN LOCKED VALUES FOR PEBBLE MODE
	bool GetActiveEditPurse1Flag();
	bool GetActiveEditPurse2Flag();
	bool GetActiveEditPurse3Flag();
	bool GetActiveEnablePointsFlag();
	bool GetActiveEditPointsFlag();
	bool GetActiveShowPointsButtonFlag();

public:
	//GENERAL
	void SetInterfaceType( int n ) { m_nInterfaceType = n; }
	void SetAutoDeletePCtrlType(int n);
	void SetAssumeBioConsentFlag(bool b) { m_bAssumeBioConsent = b; }
	void SetAllowPINNumbersFlag(bool b) { m_bAllowPINNumbers = b; }
	void SetAllowGroupSetsFlag(bool b) { m_bAllowGroupSets = b; }
	void SetUsePasswordsFlag(bool b) { m_bUsePasswords = b; }
	void SetEnableBackgroundFlag(bool b) { m_bEnableBackground = b; }
	void SetAutoShutdownType(int n);
	void SetAllowGroupShiftDateRangeFlag(bool b) { m_bAllowGroupShiftDateRange = b; }
	void SetGroupShiftDurations(CString str) { m_strGroupShiftDurations = str; }
	void SetAllowGroupShiftOnDutyFlag(bool b) { m_bAllowGroupShiftOnDuty = b; }
	//REPORTING
	void SetStartOfWeek(int n) { m_nStartOfWeek = n; }
	void SetEnablePhotoIDListFlag(bool b) { m_bEnablePhotoIDList = b; }
	void SetKeepLeaversDatabaseFlag(bool b) { m_bKeepLeaversDatabase = b; }
	void SetEcrmanPath(const char* szPath) { m_strEcrmanPath = szPath; }
	void SetShowPurchaseTextsFlag(bool b) { m_bShowPurchaseTexts = b; }
	void SetReportEndText(int n, CString str);
	void SetBioConsentTickNo(int n);
	//EDITING
	void SetShowTopupButtonFlag(bool b) { m_bShowTopupButton = b; }
	void SetFileEditPurse1Flag(bool b) { m_bFileEditPurse1 = b; }
	void SetFileEditPurse2Flag(bool b) { m_bFileEditPurse2 = b; }
	void SetEnablePurse3Flag(bool b) { m_bEnablePurse3 = b; }
	void SetFileEditPurse3Flag(bool b) { m_bFileEditPurse3 = b; }
	void SetFileEnablePointsFlag(bool b) { m_bFileEnablePoints = b; }
	void SetFileEditPointsFlag(bool b) { m_bFileEditPoints = b; }
	void SetFileShowPointsButtonFlag(bool b) { m_bFileShowPointsButton = b; }
	void SetExternalAccountFlag(bool b) { m_bExternalAccount = b; }
	void SetEnableBatchUpdatesFlag(bool b) { m_bEnableBatchUpdates = b; }
	void SetEnableNoAuditFlag(bool b) { m_bEnableNoAudit = b; }
	void SetDefaultAccountEditPage(int n);
	void SetEnableAccountColourFlag(bool b) { m_bEnableAccountColour = b; }
	void SetAccountCancelPromptFlag(bool b) { m_bAccountCancelPrompt = b; }
	void SetAuditImportFilterFlag(bool b) { m_bAuditImportFilter = b; }
	//EXPORT
	void SetEnableExportSalesFlag(bool b) { m_bEnableExportSales = b; }
	void SetExportOnDeleteFlag(bool b) { m_bExportOnDelete = b; }
	void SetExportOnExitFlag(bool b) { m_bExportOnExit = b; }
	//INTERFACE
	void SetWebPaymentType(int n) { m_nWebPaymentType = n; }
	void SetEnableChartwellsFlag(bool b) { m_bEnableChartwells = b; }
	void SetEnableSimsFlag(bool b) { m_bEnableSims = b; }
	void SetEnableAutoImportFlag(bool b) { m_bEnableAutoImport = b; }
	void SetEnableWondeFlag(bool b) { m_bEnableWonde = b; }
	void SetImportExtRef1TrimZeroFlag(bool b) { m_bImportExtRef1TrimZero = b; }
	//EPOS
	void SetEposPhotoIDFolder(CString str) { m_strEposPhotoIDFolder = str; }
	void SetExportEposPhotoFlag(bool b) { m_bExportEposPhoto = b; }
	//NON UI
	void SetCreditOptionsFlag(bool b) { m_bCreditOptions = b; }
	void SetAuditFileFilterFlag(bool b) { m_bAuditFileFilter = b; }

public:
	CString GetLowCardNo() { return m_strLowCardNo; }
	CString GetHighCardNo() { return m_strHighCardNo; }
	CString GetBlankCardNo() { return m_strBlankCardNo; }
	int GetDPValue() { return m_nDPValue;  }

public:
	bool GetDoneSQLFileSizeFlag() { return m_bDoneSQLFileSize; }
	bool GetDoneSQLShrinkLogFlag() { return m_bDoneSQLShrinkLog; }
	
public:
	void SetDoneSQLFileSizeFlag( bool b ) { m_bDoneSQLFileSize = b; }
	void SetDoneSQLShrinkLogFlag( bool b ) { m_bDoneSQLShrinkLog = b; }

public:
	void RestoreDefaultGroupShiftDurations();

#ifndef SYSTEMTYPE_EMAILBATCHSEND
	void ValidateGroupShiftDurations(CString strIn, CString& strOut, CString& strError);
#endif

public:
	CString GetEposTaxRateFilename();
	CString GetEposPaymentTypeFilename();
	CString GetEposPaymentGroupFilename();
	CString GetEposAllergyTextsFilename();

private:
	void MoveLogFiles(const char* szDestPath);
	void RewindInternal(CSSIniFile& file);
	
private:
	//GENERAL
	int m_nInterfaceType;
	int m_nAutoDeletePCtrlType;
	bool m_bAssumeBioConsent;
	bool m_bAllowPINNumbers;
	bool m_bAllowGroupSets;
	bool m_bUsePasswords;
	bool m_bEnableBackground;
	int m_nAutoShutdownType;
	bool m_bAllowGroupShiftDateRange;
	CString m_strGroupShiftDurations;
	bool m_bAllowGroupShiftOnDuty;
	//REPORTING
	int m_nStartOfWeek;
	bool m_bEnablePhotoIDList;
	bool m_bKeepLeaversDatabase;
	CString m_strEcrmanPath;
	bool m_bShowPurchaseTexts;
	CString m_strReportEndText[REPORT_ENDTEXT_COUNT];
	int m_nBioConsentTickNo;	
	//EDITING
	bool m_bShowTopupButton;
	bool m_bFileEditPurse1;
	bool m_bFileEditPurse2;
	bool m_bEnablePurse3;
	bool m_bFileEditPurse3;
	bool m_bFileEnablePoints;
	bool m_bFileEditPoints;
	bool m_bFileShowPointsButton;
	bool m_bExternalAccount;
	bool m_bEnableBatchUpdates;
	bool m_bEnableNoAudit;
	int m_nDefaultAccountEditPage;
	bool m_bEnableAccountColour;
	bool m_bAccountCancelPrompt;
	bool m_bAuditImportFilter;
	//EXPORT
	bool m_bEnableExportSales;
	bool m_bExportOnExit;
	bool m_bExportOnDelete;
	//INTERFACE
	int m_nWebPaymentType;
	bool m_bEnableChartwells;
	bool m_bEnableSims;
	bool m_bEnableAutoImport;
	bool m_bEnableWonde;
	bool m_bImportExtRef1TrimZero;
	//EPOS
	CString m_strEposPhotoIDFolder;
	bool m_bExportEposPhoto;
	//NON UI
	bool m_bCreditOptions;		
	bool m_bAuditFileFilter;
	
private:
	CString m_strLowCardNo;
	CString m_strHighCardNo;
	CString m_strBlankCardNo;

private:
	bool m_bGotPCId;	
	int m_nDPValue;								// not saved
			
private:
	int m_nUserLockCount;
	int m_nSystemLockCount;
	bool m_bSystemForegroundEnabled;
	CWnd* m_pSystemForegroundWnd;

private:
	bool m_bDoneSQLFileSize;
	bool m_bDoneSQLShrinkLog;
};

//*******************************************************************
extern CSystemData System;
//*******************************************************************
