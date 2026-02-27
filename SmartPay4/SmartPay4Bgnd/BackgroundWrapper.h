#pragma once
//***************************************************************
#include "..\SmartPay4Shared\TrackedFile.h"
#include "..\SmartPay4Shared\BackgroundOptions.h"
//***************************************************************
#include "BusyFileUpdateThreadManager.h"
//***************************************************************

class CBackgroundWrapper
{
public:
	CBackgroundWrapper();
	void InitSystem(bool bForce);
	void Pulse(CBusyFileUpdateThreadManager& BFUThreadManager);

private:
	void RequestChecks();
	bool AnyChecksRequired();
	void UpdateTimeTable();

	bool DBReadOnlyAccessRequired();
	bool DBReadWriteAccessRequired();

	void ImportNewRecords();
	void ImportAccountLinks();
	void ImportWebPayments();
	void ExportEODBalances();
	void EmailLowBalances();
	void EmailPurchases();
	void ExportSales();
	void EmailPointsAchievement();
	void EmailWeeklyPoints();

	CString FormatTime(CSSTime* pTime);
	CString FormatTime(COleDateTime* pTime);
	CString GetDateTomorrow(int nAdvanceDay = 1);

	void LogAccountLinkResults(CString strType, int nCount);

private:
	bool m_bCheckNewRecords = FALSE;
	bool m_bCheckWebPayments = FALSE;
	bool m_bCheckImportLinks = FALSE;
	bool m_bCheckEODBalances = FALSE;
	bool m_bCheckLowBalances = FALSE;
	bool m_bCheckPurchases = FALSE;
	bool m_bCheckExportSales = FALSE;
	bool m_bCheckAchievement = FALSE;
	bool m_bCheckWeeklyPoints = FALSE;
	int m_nEmailPurchaseDay = 0;
	int m_nEmailWeeklyPointsDay = 0;

	CBackgroundOptions m_options;
	COleDateTime m_timeNextCheck;
	COleDateTime m_timeNextStdCheck;
	COleDateTime m_timeNextNewCheck;
	COleDateTime m_timeNextWebCheck;
	COleDateTime m_timeNextExportCheck;
	COleDateTime m_timeNextEmailCheck;
	COleDateTime m_timeNextImportLinkCheck;
	CTime m_timeEmailOptionsModified;

	CSSDate m_dateNextEODCheck;
	CSSDate m_dateNextLowBalanceCheck;
	CSSDate m_dateNextPurchaseCheck;
	CSSDate m_dateNextPointsAchievement;
	CSSDate m_dateNextWeeklyPoints;

	CSSTime m_timeEmailPurchase;
	CSSTime m_timeEmailWeeklyPoints;

private:
	CTrackedFile m_TrackedFileEmailOptions;
	CTrackedFile m_TrackedFilePasswords;
	CTrackedFile m_TrackedFileServiceOptions;
	CTrackedFile m_TrackedFileServer;
	CTrackedFile m_TrackedFileSystem;
	CTrackedFile m_TrackedFileBackgroundOptions;
	CTrackedFile m_TrackedFileSmartPayVersion;

private:
	bool m_bVersionMatch = FALSE;
	CString m_strVersionError = "";
	CString m_strLastEODError = "";
	bool m_bEODAlreadyDue = FALSE;
};

//***************************************************************
