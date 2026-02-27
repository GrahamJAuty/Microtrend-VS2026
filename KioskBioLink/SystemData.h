#pragma once
//*********************************************************************
#include "Defines.h"
//*********************************************************************

class CSystemData  
{
public:
	CSystemData();
	bool Read();
	void Write();
	CString AddSysFilePath ( const char* szFilename );

	bool AutoRefreshBiometrics ( int nRetries, CWnd* pParent );
	bool RefreshBiometrics ( int nRetries = 10 );
	bool RefreshBiometrics ( const char* szNotifyPath );

	long GetPaylinkSerialNo() { return m_lPaylinkSerialNo; }
	void SetPaylinkSerialNo ( long lSerialNo );

	int GetUserIDLength();
	__int64 GetMaxCardNo();
	CString FormatCardNo ( const char* szCardNo, bool bPadOut = TRUE );
	CString FormatCardNo ( __int64 nCardNo, bool bPadOut = TRUE  );

	bool HaveMultiplePurses() { return (m_bTopupPurse1 && m_bTopupPurse3); }

public:
	bool GetEnableKeypadEntryFlag();
	bool GetBalanceOnlyFlag() { return m_bBalanceOnly; }
	bool GetNoSmallCoinWarningFlag() { return m_bNoSmallCoinWarning; }
	bool GetShowPhotoIDFlag() { return m_bShowPhotoID; }
	bool GetShowPointsFlag() { return m_bShowPoints; }
	bool GetShowGroupFlag() { return m_bShowGroup; }
	bool GetShowUserIDCancelFlag() { return m_bShowUserIDCancel; }
	bool GetDisableAlertsFlag() { return m_bDisableAlerts; }
	bool GetShowAvailableSpendFlag() { return m_bShowAvailableSpend; }
	bool GetHideAccountIDFlag() { return m_bHideAccountID; }
	bool GetEnrolmentAllowedFlag() { return m_bEnrolmentAllowed; }
	bool GetShowDateTimeFlag() { return m_bShowDateTime; }
	bool GetTopupPurse1Flag() { return m_bTopupPurse1; }
	bool GetTopupPurse3Flag() { return m_bTopupPurse3; }
	bool GetSystemPasswordReqdFlag() { return m_bSystemPasswordReqd; }
	bool GetActualPINModeFlag() { return m_bActualPINMode; }

public:
	void SetEnableKeypadEntryFlag(bool b) { m_bEnableKeypadEntry = b; }
	void SetBalanceOnlyFlag(bool b) { m_bBalanceOnly = b; }
	void SetNoSmallCoinWarningFlag(bool b) { m_bNoSmallCoinWarning = b; }
	void SetShowPhotoIDFlag(bool b) { m_bShowPhotoID = b; }
	void SetShowPointsFlag(bool b) { m_bShowPoints = b; }
	void SetShowGroupFlag(bool b) { m_bShowGroup = b; }
	void SetShowUserIDCancelFlag(bool b) { m_bShowUserIDCancel = b; }
	void SetDisableAlertsFlag(bool b) { m_bDisableAlerts = b; }
	void SetShowAvailableSpendFlag(bool b) { m_bShowAvailableSpend = b; }
	void SetHideAccountIDFlag(bool b) { m_bHideAccountID = b; }
	void SetEnrolmentAllowedFlag(bool b) { m_bEnrolmentAllowed = b; }
	void SetShowDateTimeFlag(bool b) { m_bShowDateTime = b; }
	void SetTopupPurse1Flag(bool b) { m_bTopupPurse1 = b; }
	void SetTopupPurse3Flag(bool b) { m_bTopupPurse3 = b; }
	void SetSystemPasswordReqdFlag(bool b) { m_bSystemPasswordReqd = b; }
	void SetActualPINModeFlag(bool b) { m_bActualPINMode = b; }

public:
	int m_nInterfaceType = nINTERFACE_BIOMETRIC;
	CString m_strLowCardNo = "";
	CString m_strHighCardNo = "";
	CString m_strBlankCardNo = "";
	CString m_strEnrolmentPrompt = "Scan Barcode";
	CString m_strPurse1Text = "";
	CString m_strPurse3Text = "";
	
	CString m_strUserIDFilename = "";

	CString	m_strPasswordFile = "";
	
	CString m_strNotifyPath = "";
	CString m_strImportFile = "";
	CString m_strCommandFilename = "";
	CString m_strAdminDatabaseFilename = "";
	CString m_strGTFilename = "";				// holds ruuning total of cash in drawer cleared on EmptyCash
	CString m_strGTExportFilename = "";			// data file to hold coinage when cash emptied

	int m_nTerminalNo = 0;
	int m_nRefreshInterval = 0;
	int m_nEnquiryTimeout = 0;

	CString m_strLoyReqFolder = "";
	CString m_strLoyReqFilename = "";
	CString m_strLoyReqTempFilename = "";
	CString m_strLoyReqReplyFilename = "";

	CString m_strLoyLogFolder = "";
	CString m_strLoyLogTransFilename = "";
	CString m_strLoyLogReplyFilename = "";

	CString m_strSysFilesImagePath = "";
	CString m_strOfflineLoyLogFilename = "";

	CString m_strPayLinkTTLFilename = "";
	CString m_strPayLinkLogFilename = "";
	CString m_strPayLinkServiceFilename = "";
	CString m_strPayLinkLevelsFilename = "";
	CString m_strPendingLogFilename = "";
	CString m_strPayLinkLevelFilename = "";
	CString m_strReaderFilename = "";
	CString m_strCardNoText = "card";

	int m_nDPValue = 2;

private:
	bool m_bEnableKeypadEntry = FALSE;
	bool m_bBalanceOnly = FALSE;				// set by service times
	bool m_bNoSmallCoinWarning = FALSE;
	bool m_bShowPhotoID = TRUE;
	bool m_bShowPoints = TRUE;
	bool m_bShowGroup = TRUE;
	bool m_bShowUserIDCancel = TRUE;
	bool m_bDisableAlerts = FALSE;
	bool m_bShowAvailableSpend = TRUE;
	bool m_bHideAccountID = FALSE;
	bool m_bEnrolmentAllowed = FALSE;
	bool m_bShowDateTime = TRUE;
	bool m_bTopupPurse1 = TRUE;
	bool m_bTopupPurse3 = FALSE;
	bool m_bSystemPasswordReqd = FALSE;
	bool m_bActualPINMode = FALSE;

private:
	void CreateFilenames();
	bool RefreshBiometrics ( int nRetries, const char* szNotifyPath );

private:
	COleDateTime m_timeNextCheck;
	CString m_strSystemFilename = "";
	long m_lPaylinkSerialNo = 0;
};

//*******************************************************************
extern CSystemData System;
//*******************************************************************
