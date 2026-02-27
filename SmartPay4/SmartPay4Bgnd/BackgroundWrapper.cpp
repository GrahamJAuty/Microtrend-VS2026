//$$******************************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
#include "..\SmartPay4Shared\EmailAlertData.h"
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\ServiceOptions.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLGroupShiftChecker.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\AutoImportHandler.h"
#include "..\SmartPay4ManagerBgnd\BackgroundTimeTable.h"
#include "..\SmartPay4ManagerBgnd\EmailLowBalances.h"
#include "..\SmartPay4ManagerBgnd\EmailPointsAchievement.h"
#include "..\SmartPay4ManagerBgnd\EmailPurchases.h"
#include "..\SmartPay4ManagerBgnd\EmailWeeklyPoints.h"
#include "..\SmartPay4ManagerBgnd\ImportPebbleMembers.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportBromCom.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportPebble.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentImport.h"
//$$******************************************************************
#include "BackgroundWrapper.h"
//$$******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
//$$******************************************************************
extern CSysset Sysset;
//*******************************************************************
static const char* szNOTSET = "Not set";
//$$******************************************************************

CBackgroundWrapper::CBackgroundWrapper()
{
	m_options.Read();									// read background options

	m_timeNextCheck = COleDateTime::GetCurrentTime();
	m_timeNextStdCheck = m_timeNextCheck;
	m_timeNextNewCheck = m_timeNextCheck;
	m_timeNextWebCheck = m_timeNextCheck;
	m_timeNextExportCheck = m_timeNextCheck;
	m_timeNextEmailCheck = m_timeNextCheck;
	m_timeNextImportLinkCheck = m_timeNextCheck;

	m_timeEmailOptionsModified = CTime(2020, 1, 1, 0, 0, 0, -1);

	m_dateNextEODCheck.SetDate(m_options.m_strDateNextEODCheck);		// date for next EOD report
	m_dateNextLowBalanceCheck.SetDate(m_options.m_strDateNextLowBalanceCheck);
	m_dateNextPurchaseCheck.SetDate(m_options.m_strDateNextEmailPurchase);
	m_dateNextPointsAchievement.SetDate(m_options.m_strDateNextPointsAchievement);
	m_dateNextWeeklyPoints.SetDate(m_options.m_strDateNextWeeklyPoints);

	CEmailAlertData data;
	data.Read();
	m_nEmailPurchaseDay = data.m_nPurchaseDay;							// day for sending purchases
	m_timeEmailPurchase.SetTime(m_options.m_strTimeEmailPurchase);	// time after which to send purchaes

	m_nEmailWeeklyPointsDay = data.m_nWeeklyPointsDay;							// day for weekly points
	m_timeEmailWeeklyPoints.SetTime(m_options.m_strTimeNextWeeklyPoints);	// time after which to send weekly points

	m_TrackedFileEmailOptions.SetFilename(Filenames.GetEmailOptionsFilename());
	m_TrackedFilePasswords.SetFilename(Filenames.GetPasswordFilename());
	m_TrackedFileSystem.SetFilename(Filenames.GetSystemFilenameNew());
	m_TrackedFileServer.SetFilename(Filenames.GetServerDataFilename());
	m_TrackedFileServiceOptions.SetFilename(Filenames.GetBgndServiceOptionsFilename());
	m_TrackedFileBackgroundOptions.SetFilename(Filenames.GetBackgroundOptionsFilename());
	m_TrackedFileSmartPayVersion.SetFilename(Filenames.GetServicesVersionFilename());

	m_strVersionError = "";
	m_strLastEODError = "";
	m_bEODAlreadyDue = FALSE;

	UpdateTimeTable();
}

//$$******************************************************************

void CBackgroundWrapper::InitSystem(bool bForce)
{
	if (m_TrackedFileServer.CheckFile(bForce) == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());
		MessageLogger.LogSystemMessage("Read Server Options File");
	}

	if (m_TrackedFileSystem.CheckFile(bForce) == TRUE)
	{
		System.Read();
		MessageLogger.LogSystemMessage("Read System Options File");
	}

	if (m_TrackedFilePasswords.CheckFile(bForce) == TRUE)
	{
		Passwords.Read(Filenames.GetPasswordFilename());
		MessageLogger.LogSystemMessage("Read Password File");
	}

	if (m_TrackedFileEmailOptions.CheckFile(bForce) == TRUE)
	{
		EmailOptions.Read();
		MessageLogger.LogSystemMessage("Read Email Options File");
	}

	if (m_TrackedFileServiceOptions.CheckFile(bForce) == TRUE)
	{
		BgndServiceOptions.Read();
		MessageLogger.LogSystemMessage("Read Service Options File");

		{
			CString strMsg = "";
			strMsg.Format("PCId = %10.10I64d", BgndServiceOptions.GetPCId());
			MessageLogger.LogSystemMessage(strMsg);
		}
	}

	if (m_TrackedFileBackgroundOptions.CheckFile(bForce) == TRUE)
	{
		m_options.Read();
		MessageLogger.LogSystemMessage("Read Background Options File");
		UpdateTimeTable();
	}

	if (m_TrackedFileSmartPayVersion.CheckFile(bForce) == TRUE)
	{
		CString strManagerVersion = "";
		CString strBackgroundVersion = szVERSION_SMARTPAY_ABOUT;

		{
			CSSFile fileVersion;
			if (fileVersion.Open(Filenames.GetServicesVersionFilename(), "rb") == TRUE)
			{
				fileVersion.ReadString(strManagerVersion);
			}
		}

		MessageLogger.LogSystemMessage("Read SmartPay Version File");

		m_bVersionMatch = (strManagerVersion == strBackgroundVersion);

		if (FALSE == m_bVersionMatch)
		{
			MessageLogger.LogSystemMessage("Incompatible SmartPay Manager Version");

			CString strLine = "";
			strLine = "==> Manager : ";
			strLine += strManagerVersion;
			MessageLogger.LogSystemMessage(strLine);

			strLine = "==> Service : ";
			strLine += strBackgroundVersion;
			MessageLogger.LogSystemMessage(strLine);

			m_strVersionError.Format("Background version %s (%s)",
				(const char*)strBackgroundVersion,
				(const char*)strManagerVersion);

			if (TRUE == Server.GetBypassVersionSafetyCheckFlag())
			{
				MessageLogger.LogSystemMessage("Bypass Version Safety Check");
				m_bVersionMatch = TRUE;
			}
		}
	}
}

//$$******************************************************************

void CBackgroundWrapper::Pulse(CBusyFileUpdateThreadManager& BFUThreadManager)
{
	if (FALSE == System.GetGotPCIdFlag())
	{
		bool bCanCreatePCId = FALSE;
		Sysset.SetPCId(bCanCreatePCId);

		if (TRUE == bCanCreatePCId)
		{
			System.SetGotPCIdFlag(TRUE);
			MessageLogger.LogSystemMessage("Got PCId after retry");
		}
	}

	SQLGroupShiftChecker.CheckGroupShifts();

	bool bDesktopModeMatch = (BgndServiceOptions.GetAllowDesktopAppFlag() == g_GlobalState.IsServerDesktopApp());

	if (COleDateTime::GetCurrentTime() >= m_timeNextCheck)
	{
		InitSystem(FALSE);

		{
			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetBgndServiceActionTimeFilename(), "wb") == TRUE)
			{
				CString strInfo = "";
				strInfo.Format("PCId = %10.10d",
					Sysset.GetPCIdHost());

				CCSV csv;
				csv.Add(strInfo);
				csv.Add(GetSimpleDateTimeNowString());

				if ((BgndServiceOptions.GetPCId() != Sysset.GetPCIdHost()) || (FALSE == m_bVersionMatch))
				{
					csv.Add(1);
				}

				fileInfo.WriteLine(csv.GetLine());
			}
		}

		if (BgndServiceOptions.GetPCId() != Sysset.GetPCIdHost())
		{
			CString strError = "";
			strError.Format("PCId = %10.10d (%10.10I64d)",
				Sysset.GetPCIdHost(), BgndServiceOptions.GetPCId());

			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetBgndServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(strError);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}
		else if (FALSE == bDesktopModeMatch)
		{
			CString strError = BgndServiceOptions.GetAllowDesktopAppFlag() ?
				"Service mode not enabled" :
				"Desktop App mode not enabled";

			CSSFile fileInfo;
			if (fileInfo.Open(Filenames.GetBgndServiceErrorTimeFilename(), "wb") == TRUE)
			{
				CCSV csv;
				csv.Add(strError);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}

		if ( (BgndServiceOptions.GetPCId() == Sysset.GetPCIdHost()) && (TRUE == bDesktopModeMatch))
		{
			if (TRUE == m_bVersionMatch)
			{
				BFUThreadManager.SetCanBeBusy(TRUE);

				if (AnyChecksRequired() == TRUE)
				{
					CSmartPayLockRequest Request;
					Request.m_bWantLockBgnd = TRUE;

					CSmartPayLockManager LockManager;
					int nLockError = LockManager.AcquireLocks(Request);

					if (LOCKMANAGER_OK == nLockError)
					{
						RequestChecks();
					}
				}
			}
			else
			{
				BFUThreadManager.SetCanBeBusy(FALSE);

				CSSFile fileInfo;
				if (fileInfo.Open(Filenames.GetBgndServiceErrorTimeFilename(), "wb") == TRUE)
				{
					CCSV csv;
					csv.Add(m_strVersionError);
					csv.Add(GetSimpleDateTimeNowString());
					fileInfo.WriteLine(csv.GetLine());
				}
			}
		}
		else
		{
			BFUThreadManager.SetCanBeBusy(FALSE);
		}

		m_timeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 10);

		InitSystem(FALSE);

		if (TRUE == m_bVersionMatch)
		{
			CFileStatus FileStatus;
			if (CFile::GetStatus(Filenames.GetEmailOptionsFilename(), FileStatus) == TRUE)
			{
				if (FileStatus.m_mtime != m_timeEmailOptionsModified)
				{
					EmailOptions.Read();
					m_timeEmailOptionsModified = FileStatus.m_mtime;
				}
			}

			if (EmailOptions.UseSMTPForDirectEmail() == TRUE)
			{
				if (COleDateTime::GetCurrentTime() > m_timeNextEmailCheck)
				{
					SSspawnl(NULL, P_NOWAIT, "SposSmartPay4EmailBatchSend.exe", "SposSmartPay4EmailBatchSend.exe", "/EMAIL", NULL);
					m_timeNextEmailCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 30);
				}
			}
		}
	}
}

//$$******************************************************************

bool CBackgroundWrapper::AnyChecksRequired()
{
	CSSDate dateNow;
	dateNow.SetCurrentDate();

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	CSSTime timeStart;
	CSSTime timeEnd;

	bool bReply = FALSE;

	InitSystem(FALSE);

	// new records

	m_bCheckNewRecords = FALSE;
	if (System.GetEnableAutoImportFlag() == TRUE)
	{
		if (m_options.m_nNewRecordsMinutes > 0)			// wait time in minutes
		{
			timeStart.SetTime(m_options.m_strNewRecordStart);
			timeEnd.SetTime(m_options.m_strNewRecordEnd);

			if (timeNow >= timeStart && timeNow <= timeEnd)
			{
				if (COleDateTime::GetCurrentTime() >= m_timeNextNewCheck)
				{
					m_bCheckNewRecords = TRUE;
					bReply = TRUE;
				}
			}
		}
	}

	// web payments

	m_bCheckWebPayments = FALSE;
	if (System.GetWebPaymentType() != nWEBPAYMENT_NONE)
	{
		if (m_options.m_nWebPaymentMinutes > 0)					// wait time in minutes
		{
			if (COleDateTime::GetCurrentTime() >= m_timeNextWebCheck)
			{
				m_bCheckWebPayments = TRUE;
				bReply = TRUE;
			}
		}
	}

	m_bCheckImportLinks = FALSE;
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		if (COleDateTime::GetCurrentTime() >= m_timeNextImportLinkCheck)
		{
			m_bCheckImportLinks = TRUE;
			bReply = TRUE;
		}
	}

	// Sales & Topup exports records

	m_bCheckExportSales = FALSE;
	if (System.GetEnableExportSalesFlag() == TRUE)
	{
		if (m_options.m_nExportSalesMinutes > 0)			// wait time in minutes
		{
			timeStart.SetTime(m_options.m_strExportSalesStart);
			timeEnd.SetTime(m_options.m_strExportSalesEnd);

			if (timeNow >= timeStart && timeNow <= timeEnd)
			{
				if (COleDateTime::GetCurrentTime() >= m_timeNextNewCheck)
				{
					m_bCheckExportSales = TRUE;
					bReply = TRUE;
				}
			}
		}
	}

	// End of Day Balances

	m_bCheckEODBalances = FALSE;

	if (m_options.m_bEODExport == TRUE)
	{
		int nDay = dateNow.GetDayOfWeek() - 1;							// 1=sun .. 7 = sat, 0=not set
		if (m_options.IsEODDay(nDay) == TRUE)
		{
			if (dateNow >= m_dateNextEODCheck)
			{
				timeStart.SetTime(m_options.m_strTimeNextEODCheck);

				if (timeNow >= timeStart)
				{
					m_bCheckEODBalances = TRUE;
					bReply = TRUE;
				}
			}
		}
	}

	if (FALSE == m_bCheckEODBalances)
	{
		//ALLOW EOD TO BE LOGGED AGAIN THE NEXT TIME IT IS DUE
		m_bEODAlreadyDue = FALSE;
		m_strLastEODError = "";
	}

	// Low Balances Alerts

	m_bCheckLowBalances = FALSE;
	if (m_options.m_bEmailLowBalance == TRUE)
	{
		timeStart.SetTime(m_options.m_strTimeNextLowBalanceCheck);

		if (timeNow >= timeStart)
		{
			CSSDate dateNow;
			dateNow.SetCurrentDate();

			if (dateNow >= m_dateNextLowBalanceCheck)
			{
				m_bCheckLowBalances = TRUE;
				bReply = TRUE;
			}
		}
	}

	// email itemised purchases

	m_bCheckPurchases = FALSE;
	if (m_options.m_bEmailPurchases == TRUE)
	{
		int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

		if (nDayNo == m_nEmailPurchaseDay)								// 0=Sunday, 1=Monday
		{
			if (timeNow >= m_timeEmailPurchase)
			{
				CSSDate dateNow;
				dateNow.SetCurrentDate();

				if (dateNow >= m_dateNextPurchaseCheck)
				{
					m_bCheckPurchases = TRUE;
					bReply = TRUE;
				}
			}
		}
	}


	// Database specific - Points Achievement ****************

	if (m_options.m_bEmailPointsAchievement == TRUE)
	{
		timeStart.SetTime(m_options.m_strTimeNextPointsAchievement);

		if (timeNow >= timeStart)
		{
			CSSDate dateNow;
			dateNow.SetCurrentDate();

			if (dateNow >= m_dateNextPointsAchievement)
			{
				m_bCheckAchievement = TRUE;
				bReply = TRUE;
			}
		}
	}

	// Database specific - weekly points ***********

	if (m_options.m_bEmailWeeklyPoints == TRUE)
	{
		int nDayNo = COleDateTime::GetCurrentTime().GetDayOfWeek() - 1;		// range = 1-7 (1=Sunday, 2= Momday

		if (nDayNo == m_nEmailWeeklyPointsDay)							// 0=Sunday, 1=Monday
		{
			if (timeNow >= m_timeEmailWeeklyPoints)
			{
				CSSDate dateNow;
				dateNow.SetCurrentDate();

				if (dateNow >= m_dateNextWeeklyPoints)
				{
					m_bCheckWeeklyPoints = TRUE;
					bReply = TRUE;
				}
			}
		}
	}

	return bReply;
}

//$$******************************************************************
// to get here at least 1 form of import is required

void CBackgroundWrapper::RequestChecks()
{
	if ((BgndServiceOptions.GetPCId() != Sysset.GetPCIdHost()) /*|| ( BgndServiceOptions.GetPauseFlag() == TRUE )*/)
	{
		return;
	}

	// if database in use retry in 1 minutes time
	int nMinutes = 0;
	int nRetryMinute = 1;

	// Database Read only functions
	if (DBReadOnlyAccessRequired() == TRUE)
	{
		if (TRUE == m_bCheckExportSales)
		{
			ExportSales();
		}
		if (TRUE == m_bCheckLowBalances)
		{
			EmailLowBalances();
		}
		if (TRUE == m_bCheckPurchases)
		{
			EmailPurchases();
		}
		if (TRUE == m_bCheckAchievement)
		{
			EmailPointsAchievement();
		}
		if (TRUE == m_bCheckWeeklyPoints)
		{
			EmailWeeklyPoints();
		}

		if (TRUE == m_bCheckExportSales)
		{
			nMinutes = m_options.m_nExportSalesMinutes;
			m_timeNextExportCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, nMinutes, 0);
		}
	}

	// Databse Read\Write functions
	if (DBReadWriteAccessRequired() == TRUE)
	{
		if (m_bCheckNewRecords == TRUE)
		{
			ImportNewRecords();					// check for csv new record importing
		}

		if ( m_bCheckImportLinks == TRUE)
		{
			ImportAccountLinks();
		}

		if (m_bCheckWebPayments == TRUE)
		{
			ImportWebPayments();				// web payments
		}

		if (m_bCheckEODBalances == TRUE)
		{
			ExportEODBalances();				// eod closing daily balances
		}

		if (m_bCheckNewRecords == TRUE)
		{
			nMinutes = m_options.m_nNewRecordsMinutes;
			m_timeNextNewCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, nMinutes, 0);
		}

		if (m_bCheckWebPayments == TRUE)
		{
			nMinutes = m_options.m_nWebPaymentMinutes;
			m_timeNextWebCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, nMinutes, 0);
		}

		if ( m_bCheckImportLinks == TRUE)
		{
			m_timeNextImportLinkCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 1, 0);
		}
	}

	UpdateTimeTable();
}

//$$******************************************************************

bool CBackgroundWrapper::DBReadOnlyAccessRequired()
{
	return (m_bCheckExportSales || m_bCheckLowBalances || m_bCheckPurchases ||
		m_bCheckAchievement || m_bCheckWeeklyPoints);
}

//$$******************************************************************

bool CBackgroundWrapper::DBReadWriteAccessRequired()
{
	return (m_bCheckNewRecords || m_bCheckWebPayments || m_bCheckEODBalances || m_bCheckImportLinks);
}

//$$******************************************************************
// System wide - import new records various formats formats

void CBackgroundWrapper::ImportNewRecords()
{
	MessageLogger.LogSystemMessage("Check record updates");

	CAutoImportHandler autoImport(NULL);
	int nCount = autoImport.ImportAccounts();					// import new accounts via csv file / SIMS / Wonde

	if (nCount > 0)
	{
		CString strMsg = "";
		strMsg.Format("%d records processed", nCount);
		MessageLogger.LogSystemMessage(strMsg);
	}

	nCount = autoImport.ImportPhotoIDs();
	if (nCount > 0)
	{
		CString strMsg = "";
		strMsg.Format("%d photos processed", nCount);
		MessageLogger.LogSystemMessage(strMsg);
	}
}

//$$******************************************************************

void CBackgroundWrapper::ImportAccountLinks()
{
	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		return;
	}

	CSQLRepositoryWebPaymentOptions WebPaymentOptions;
	WebPaymentOptions.LoadRow(NULL);

	if (WebPaymentOptions.m_Row.TimeForAccountLink() == FALSE)
	{
		return;
	}

	CString strDateToday = CSSDate::GetCurrentDate().GetDate();

	if ( strDateToday == WebPaymentOptions.m_Row.GetAccountLinkLastDate() )
	{
		return;
	}
	
	MessageLogger.LogSystemMessage("Import Account Links");
	
	CSQLRepositoryPebbleConfig configPebble;
	configPebble.LoadRow(NULL);

	CArray<CPebbleMemberBuffer, CPebbleMemberBuffer> arrayMembers;

	CString strError = "";
	{
		CSmartPayLockRequest Request;
		Request.m_bWantLockPebble = TRUE;
		Request.m_bOneShot = TRUE;

		CSmartPayLockManager LockManager;
		if (LockManager.AcquireLocks(Request, NULL) != LOCKMANAGER_OK)
		{
			strError = "Unable to acquire lock";
		}
		else
		{
			strError = pPebbleHelpers->GetMembers(configPebble, arrayMembers, NULL);;
		}
	}

	if (strError == "")
	{
		if (arrayMembers.GetSize() != 0)
		{	
			CImportPebbleMembers import(&configPebble, arrayMembers, NULL);
			import.ImportMembers();

			int nRetries = 0;
			while (++nRetries <= 10)
			{
				WebPaymentOptions.LoadRow(NULL);
				WebPaymentOptions.m_Row.SetLastAccountLinkDone();
				if (WebPaymentOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
				{
					break;
				}
			}

			LogAccountLinkResults("Read", import.m_ImportStats.m_nRead);
			LogAccountLinkResults("Already linked", import.m_ImportStats.m_nAlreadyMatched);
			LogAccountLinkResults("Pebble inactive", import.m_ImportStats.m_nPebbleInactive);
			LogAccountLinkResults("Matched by UPN", import.m_ImportStats.m_nMatchedByUPN);
			LogAccountLinkResults("Matched by MIS", import.m_ImportStats.m_nMatchedByMisID);
			LogAccountLinkResults("Not matched", import.m_ImportStats.m_nNotMatched);
			LogAccountLinkResults("UPN mismatch", import.m_ImportStats.m_nUPNMismatch);
			LogAccountLinkResults("Newly linked", import.m_ImportStats.m_nNewLinks);
		}
	}
	else
	{
		MessageLogger.LogSystemMessage(CString("==> ") + strError);
	}
}

//$$******************************************************************
// System wide - webpayments

void CBackgroundWrapper::LogAccountLinkResults(CString strType, int nCount)
{
	if (nCount > 0)
	{
		CString strMsg = "==> ";
		strMsg.Format("==> %s: %d", 
			(const char*) strType,
			nCount);

		MessageLogger.LogSystemMessage(strMsg);
	}
}

//$$******************************************************************

void CBackgroundWrapper::ImportWebPayments()
{
	if (System.GetWebPaymentType() != nWEBPAYMENT_NONE)
	{
		CString strMsg = "";
		strMsg.Format("Check : %s",
			(const char*)System.GetWebPaymentDisplayName());

		bool bSuspended = ::FileExists(Filenames.GetWebPaymentStopIndicator());
		if (TRUE == bSuspended)
		{
			strMsg += " ** SUSPENDED";
		}

		MessageLogger.LogSystemMessage(strMsg);

		if (FALSE == bSuspended)
		{
			CWebPaymentImport import{};
			bool bTriedSendUpdates = FALSE;
			if (import.ImportPayments(TRUE, bTriedSendUpdates) == TRUE)				// get new payments (if any)
			{
				int nCount = import.GetPaymentCount();					//15/09/2016
				if (nCount > 0)
				{
					CString strExtra = "";
					strExtra.Format(" ==> %d payments received", nCount);
					MessageLogger.LogSystemMessage(strMsg + strExtra);
				}
				else if (import.IsExcludedByTime() == FALSE)
				{
					MessageLogger.LogSystemMessage(strMsg + " ==> No payments received.");
				}
				else
				{
					MessageLogger.LogSystemMessage(strMsg + " ==> Check excluded by time.");
				}

				if (FALSE == bTriedSendUpdates)
				{
					switch (System.GetWebPaymentType())
					{
					case nWEBPAYMENT_BROMCOM:
					{
						CWebPaymentExportBromCom web{};							// try & export balance\Purchases in background mode
						web.SendAccountUpdates(FALSE);						// no eod request - send purchases followed by balances if time is right
					}
					break;

					case nWEBPAYMENT_PEBBLE:
					{
						CWebPaymentExportPebble web{};							// try & export balance\Purchases in background mode
						web.SendAccountUpdates();							// no eod request - send purchases followed by balances if time is right
					}
					break;

					default:
					{
						CWebPaymentExport web{};							// try & export balance\Purchases in background mode
						web.SendAccountUpdates(FALSE);						// no eod request - send purchases followed by balances if time is right
					}
					break;
					}
				}
			}
		}
	}
}

//$$******************************************************************
// save current purse balances
// Eg:- Mondays Closing balance = Tuesdays Opening balance etc..

void CBackgroundWrapper::ExportEODBalances()
{
	CString strEODMsg = "";											// End of Day message to save in log file if eod called

	if (FALSE == m_bEODAlreadyDue)
	{
		m_bEODAlreadyDue = TRUE;
		MessageLogger.LogSystemMessage("Check EOD balances");
	}

	CReportEndOfDay reportEOD{};						// no display any errors

	int nReply = reportEOD.Background();							// will resume server part way through EOD report

	if (nReply == nEOD_PERFORMED)									// see if EOD performed
	{
		strEODMsg = "EOD Saved";
	}
	else
	{
		CString strError = reportEOD.GetBackgroundError();			// not done, see if any error returned
		if (strError != "")
		{
			strEODMsg.Format("EOD Error %s",
				(const char*)strError);			// log error message
		}
	}

	if (strEODMsg != "")
	{
		if (strEODMsg != m_strLastEODError)
		{
			MessageLogger.LogSystemMessage(strEODMsg);
			m_strLastEODError = strEODMsg;
		}
	}

	// set to check the next day
	switch (nReply)
	{
	case nEOD_PERFORMED:
	case nEOD_EXISTS:
	case nEOD_INPROGRESS:
	{
		CString strDateTomorrow = GetDateTomorrow();				// get next date dd/mm/yyyy
		m_options.m_strDateNextEODCheck = strDateTomorrow;
		m_options.Save();

		m_dateNextEODCheck.SetDate(strDateTomorrow);				//19/03/2014 - v1.05x
	}
	break;
	}
}

//$$******************************************************************

void CBackgroundWrapper::ExportSales()
{
	MessageLogger.LogSystemMessage("Check Export Sales");

	CWebPaymentExport web{};
	web.SpecialSalesExport();
	int nCount = web.GetTransactionCount();								// get number of lines sent

	if (0 == nCount)
	{
		return;
	}

	CString strMsg = "";
	if (-1 == nCount)
	{
		strMsg = web.GetError();						// check for an error message
	}
	else
	{
		strMsg.Format("%d sent", nCount);
	}

	if (strMsg != "")
	{
		MessageLogger.LogSystemMessage(strMsg);
	}
}

//$$******************************************************************
// save current purse balances

void CBackgroundWrapper::EmailLowBalances()
{
	MessageLogger.LogSystemMessage("Check Email low balances");

	CEmailLowBalances email{};
	email.Execute();										// in background - no show progress

	CString strMsg = "";
	strMsg.Format("%d sent", email.GetExportCount());
	MessageLogger.LogSystemMessage(strMsg);

	CString strDateTomorrow = GetDateTomorrow();				// get next date dd/mm/yyyy

	m_options.m_strDateNextLowBalanceCheck = strDateTomorrow;	// set date for next check
	m_options.Save();

	m_dateNextLowBalanceCheck.SetDate(strDateTomorrow);
}

//$$******************************************************************
// Email any new itemised purchases

void CBackgroundWrapper::EmailPurchases()
{
	MessageLogger.LogSystemMessage("Check Email purchases");

	CEmailPurchases email{};
	email.Execute();												// in background - no show progress

	CString strMsg = "";
	strMsg.Format("%d sent", email.GetExportCount());
	MessageLogger.LogSystemMessage(strMsg);

	CString strNextDate = GetDateTomorrow(7);							// next do in 7 days time

	m_options.m_strDateNextEmailPurchase = strNextDate;
	m_options.Save();

	m_dateNextPurchaseCheck.SetDate(strNextDate);
}

//$$******************************************************************

void CBackgroundWrapper::EmailPointsAchievement()
{
	MessageLogger.LogSystemMessage( "Check Email Points Achievement" );

	CEmailPointsAchievement email{};
	email.Execute();									

	CString strMsg = "";
	strMsg.Format ( "%d sent", email.GetExportCount() ); 
	MessageLogger.LogSystemMessage( strMsg );

	CString strDateTomorrow = GetDateTomorrow();				// get next date dd/mm/yyyy

	m_options.m_strDateNextPointsAchievement = strDateTomorrow;	// set date for next check
	m_options.Save();

	m_dateNextPointsAchievement.SetDate ( strDateTomorrow );
}

//$$******************************************************************
// Email weekly points

void CBackgroundWrapper::EmailWeeklyPoints()
{
	MessageLogger.LogSystemMessage("Check Email Weekly Points");

	CEmailWeeklyPoints email{};
	email.Execute();

	CString strMsg = "";
	strMsg.Format("%d sent", email.GetExportCount());
	MessageLogger.LogSystemMessage(strMsg);

	CString strNextDate = GetDateTomorrow(7);							// next do in 7 days time

	m_options.m_strDateNextWeeklyPoints = strNextDate;					// next do in 7 days time
	m_options.Save();

	m_dateNextWeeklyPoints.SetDate(strNextDate);
}

//$$******************************************************************
// get date nAdvanceDay's from today

CString CBackgroundWrapper::GetDateTomorrow(int nAdvanceDay)
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime() + COleDateTimeSpan(nAdvanceDay, 0, 0, 0);	// date ndays in future

	CString strDate = "";
	strDate.Format("%2.2d/%2.2d/%4.4d", dtNow.GetDay(), dtNow.GetMonth(), dtNow.GetYear());
	return strDate;
}

//$$******************************************************************

CString CBackgroundWrapper::FormatTime(CSSTime* pTime)
{
	CString strTime = "";
	strTime.Format("%2.2d:%2.2d:%2.2d", pTime->GetHour(), pTime->GetMinute(), pTime->GetSecond());
	return strTime;
}

//$$******************************************************************

CString CBackgroundWrapper::FormatTime(COleDateTime* pTime)
{
	CString strTime = "";
	strTime.Format("%2.2d:%2.2d:%2.2d", pTime->GetHour(), pTime->GetMinute(), pTime->GetSecond());
	return strTime;
}

//$$******************************************************************

void CBackgroundWrapper::UpdateTimeTable()
{
	CSSTime timeNow;
	timeNow.SetCurrentTime();
	CSSTime timeStart;

	// standard import

	CString strTime1 = "";						// 'Not set'
	if (m_options.m_nStandardImportMinutes > 0)
	{
		timeStart.SetTime(m_options.m_strStandardStart);
		if (timeNow >= timeStart)
		{
			strTime1 = FormatTime(&m_timeNextStdCheck);
		}
		else
		{
			strTime1 = FormatTime(&timeStart);
		}
	}

	// new records

	CString strTime2 = "";						// 'Not set'
	if (System.GetEnableAutoImportFlag() == TRUE)
	{
		if (m_options.m_nNewRecordsMinutes > 0)			// wait time in minutes
		{
			timeStart.SetTime(m_options.m_strNewRecordStart);
			if (timeNow >= timeStart)
			{
				strTime2 = FormatTime(&m_timeNextNewCheck);
			}
			else
			{
				strTime2 = FormatTime(&timeStart);
			}
		}
	}

	// web payments

	CString strTime3 = "";						// 'Not set'

	if (System.GetWebPaymentType() != nWEBPAYMENT_NONE)
	{
		if (m_options.m_nWebPaymentMinutes > 0)			// wait time in minutes
		{
			strTime3 = FormatTime(&m_timeNextWebCheck);
		}
	}

	// Closing Balance export

	CString strTime4 = "";						// 'Not set'

	if (m_options.m_bEODExport == TRUE)
	{
		timeStart.SetTime(m_options.m_strTimeNextEODCheck);
		strTime4 = FormatTime(&timeStart);
	}

	// Email Low Balance Alerts

	CString strTime5 = "";						// 'Not set'

	if (m_options.m_bEmailLowBalance == TRUE)
	{
		timeStart.SetTime(m_options.m_strTimeNextLowBalanceCheck);
		strTime5 = FormatTime(&timeStart);
	}

	// Email Itemised purchases

	CString strTime6 = "";						// 'Not set'
	if (m_options.m_bEmailPurchases == TRUE)
	{
		strTime6 = FormatTime(&m_timeEmailPurchase);
	}

	// Export Sales

	CString strTime7 = "";						// 'Not set'

	if (System.GetEnableExportSalesFlag() == TRUE)
	{
		if (m_options.m_nExportSalesMinutes > 0)
		{
			timeStart.SetTime(m_options.m_strExportSalesStart);
			if (timeNow >= timeStart)
			{
				strTime7 = FormatTime(&m_timeNextExportCheck);
			}
			else
			{
				strTime7 = FormatTime(&timeStart);
			}
		}
	}

	// Email Points Achievement

	CString strTime8 = "";						// 'Not set'

	if (m_options.m_bEmailPointsAchievement == TRUE)
	{
		timeStart.SetTime(m_options.m_strTimeNextPointsAchievement);
		strTime8 = FormatTime(&timeStart);
	}

	// Email Weekly Point

	CString strTime9 = "";						// 'Not set'
	if (m_options.m_bEmailWeeklyPoints == TRUE)
	{
		strTime9 = FormatTime(&m_timeEmailWeeklyPoints);
	}

	//sccount link
	CString strTime10 = "";						// 'Not set'	
	int nAccountLinkDay = -1;
	
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		CSQLRepositoryWebPaymentOptions WebPaymentOptions;
		WebPaymentOptions.LoadRow(NULL);

		CSSDate dateToday = CSSDate::GetCurrentDate();
		CString strDateToday = dateToday.GetDate();
		int nWeekdayToday = dateToday.GetDayOfWeek() - 1;

		bool bDoneToday = (strDateToday == WebPaymentOptions.m_Row.GetAccountLinkLastDate());
		bool bGotNextTime = FALSE;

		//TEST TO SEE IF WE ARE IN THE TIME WINDOW FOR ACCOUNT LINK
		if (WebPaymentOptions.m_Row.TimeForAccountLink() == TRUE)
		{
			//SEE IF WE HAVE ALREADY DONE IT TODAY
			if (FALSE == bDoneToday)
			{
				strTime10 = FormatTime(&timeNow);
				nAccountLinkDay = nWeekdayToday;
				bGotNextTime = TRUE;
			}
		}

		//IF NOT SEE IF WE CAN DO IT LATER TODAY
		if (FALSE == bGotNextTime)
		{
			if ((FALSE == bDoneToday) && (WebPaymentOptions.m_Row.IsAccountLinkDay(nWeekdayToday)))
			{
				CSSTime timeNow = CSSTime::GetCurrentTime();

				CSSTime timeCheck;
				timeCheck.SetTime(WebPaymentOptions.m_Row.GetAccountLinkStart());

				if (timeNow < timeCheck)								// not yet time today
				{
					strTime10 = FormatTime(&timeCheck);
					nAccountLinkDay = nWeekdayToday;
					bGotNextTime = TRUE;
				}
			}
		}

		//IF NOT SEE WHEN NEXT
		if (FALSE == bGotNextTime)
		{
			for (int nDay = 1; nDay <= 6; nDay++)
			{
				int nCheckDay = (nWeekdayToday + nDay) % 7;			// day to check
				if (WebPaymentOptions.m_Row.IsAccountLinkDay(nCheckDay))
				{
					CSSTime timeCheck;
					timeCheck.SetTime(WebPaymentOptions.m_Row.GetAccountLinkStart());
					strTime10 = FormatTime(&timeCheck);
					nAccountLinkDay = nCheckDay;
					bGotNextTime = TRUE;
					break;
				}
			}
		}
	}

	// update timetable file
	CBackgroundTimeTable timetable;
	timetable.SetTime(1, strTime1);					// standard imports
	timetable.SetTime(2, strTime2);					// new records
	timetable.SetTime(3, strTime3);					// web payments
	timetable.SetTime(4, strTime4);					// Closing balance
	timetable.SetTime(5, strTime5);					// low balance
	timetable.SetTime(6, strTime6);					// purchases
	timetable.SetPurchaseDay(m_nEmailPurchaseDay);	// purchase email day
	timetable.SetTime(7, strTime7);					// Export sales
	timetable.SetTime(8, strTime8);					// points achievement
	timetable.SetTime(9, strTime9);							// Weekly Points update
	timetable.SetWeeklyPointsDay(m_nEmailWeeklyPointsDay);	// weekly email day
	timetable.SetTime(10, strTime10);					// account link
	timetable.SetAccountLinkDay(nAccountLinkDay);	// account link day

	timetable.WriteTimetable();
}

//*******************************************************************
