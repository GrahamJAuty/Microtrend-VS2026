#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "CkJsonArray.h"
#include "CkJsonObject.h"
/**********************************************************************/
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
#include "..\SmartPay4Shared\TrackedFile.h"
/**********************************************************************/
#include "ExternalParamsArray.h"
#include "LoyLogThreadManager.h"
#include "LoyReqThreadManager.h"
#include "LoyReqTidyUpThreadManager.h"
#include "LoyRspThreadManager.h"
/**********************************************************************/
#define RECON_ERROR_NONE 0
#define RECON_ERROR_REQUEST 1
#define RECON_ERROR_AUDIT 2
#define RECON_ERROR_DATE 3
/**********************************************************************/

struct CJSONIncomingParams
{
	int m_nVersion = 0;
	_int64 m_nTimeStamp = 0;
	int m_nSerialNumber = 0;
	CString m_strMessageType = "";
	CString m_strSubType = "";
	int m_nSubType = 0;
	CString m_strDataEntity = "";
	int m_nDataEntity = 0;
};

/**********************************************************************/

struct CJSONIncomingError
{
	int m_nCode = 0;
	CString m_strText = "";
};

/**********************************************************************/

struct CPurse1AvailableSpendInfo
{
public:
	CPurse1AvailableSpendInfo();

public:
	void Reset();

public:
	double m_dBalanceP1;
	double m_dSpendLimitP1;
	double m_dOverdraftP1;
	double m_dAvailableSpendP1;
	double m_dBalanceP2;
};

//***************************************************************
//***************************************************************
//***************************************************************

struct CPurse3AvailableSpendInfo
{
public:
	CPurse3AvailableSpendInfo();

public:
	void Reset();

public:
	double m_dBalanceP3;
	double m_dSpendLimitP3;
	double m_dOverdraftP3;
	double m_dAvailableSpendP3;
};

//***************************************************************
//***************************************************************
//***************************************************************

class CJSONObjectCollector
{
public:
	void RememberJSONObject(CkJsonObject* jsonObject)
	{
		if (jsonObject != NULL)
		{
			m_arrayJSONObjects.Add(jsonObject);
		}
	}

	void RememberJSONArray(CkJsonArray* jsonArray)
	{
		if (jsonArray != NULL)
		{
			m_arrayJSONArrays.Add(jsonArray);
		}
	}

	~CJSONObjectCollector()
	{
		for (int n = 0; n < m_arrayJSONObjects.GetCount(); n++)
		{
			delete(m_arrayJSONObjects[n]);
		}

		for (int n = 0; n < m_arrayJSONArrays.GetCount(); n++)
		{
			delete(m_arrayJSONArrays[n]);
		}
	}

public:
	CArray<CkJsonObject*, CkJsonObject*> m_arrayJSONObjects;
	CArray<CkJsonArray*, CkJsonArray*> m_arrayJSONArrays;
};

//***************************************************************
//***************************************************************
//***************************************************************

class CServerWrapper
{
// Construction
public:
	CServerWrapper();	// standard constructor
	~CServerWrapper();

public:
	void ShutdownThreads();

private:
	CString StringMoreExists() { return "#MORE"; }
	CString StringNoneExists() { return "#NONE"; }
	CString StringVersionReq() { return "VR=2"; }
	CString StringErrorCodeDbAccess() { return "ER=101"; }
	CString StringErrorTextDbAccess() { return "EM=Unable to access SmartPay Database"; }
	CString StringErrorTextDbAccessV2() { return "EM=Unable to access card Database"; }
	CString StringErrorCodeUnknownID() { return "ER=102"; }
	CString StringErrorTextUnknownID() { return "EM=No account found"; }
	CString StringErrorCodeUnknownGroup() { return "ER=103"; }
	CString StringErrorTextUnknownGroup() { return "EM=Unknown account Group"; }
	CString StringErrorCodeIDExists() { return "ER=104"; }
	CString StringErrorTextIDExists() { return "EM=Account already exists"; }
	CString StringErrorCodeAddRecord() { return "ER=105"; }
	CString StringErrorTextAddRecord() { return "EM=Unable to add new record"; }
	CString StringErrorCodeInUse() { return "ER=106"; }
	CString StringErrorTextInUse() { return "EM=Account in use"; }
	CString StringErrorTextInUse2() { return "EM=Account not available, please try later"; }
	CString StringErrorCodePaused() { return "ER=107"; }
	CString StringErrorTextPaused() { return "EM=Server busy"; }
	CString StringErrorCodeExpired() { return "ER=201"; }
	CString StringErrorTextExpired() { return "EM=Account expired"; }
	CString StringErrorCodeAlert() { return "ER=202"; }
	CString StringErrorTextAlert() { return "EM=Account Alert"; }
	CString StringErrorCodeInactive() { return "ER=203"; }
	CString StringErrorTextInactive() { return "EM=Account set to Inactive"; }
	CString StringErrorCodeKioskBlock() { return "ER=204"; }
	CString StringErrorTextKioskBlock() { return "EM=Kiosk use blocked for this account"; }
	
public:
	void Pulse();
	
	void InitSystem( bool bForce );

public:
	void ProcessUpdatePluRequest();
	static void UpdatePluInfo();
	static void UpdateOperatorInfo();
	static void UpdateEposTerminalInfo();
	static void UpdatePurchaseControlInfo();
	
public:
	void LogAction2 ( const char* szAction );
	void LogAccountError ( const char* szError );

	void LogBioRegister (CSQLRowAccountFull& AccountRow);
	void LogBioUse (CSQLRowAccountFull& AccountRow);
	void LogBioUnRegister(CSQLRowAccountFull& AccountRow);

	void ProcessRequest ();
	void ProcessPINRequest(CString strPIN, CCSV& csvLine);
	bool ProcessLogFile ( const char* szLogFilePath, const char* szErrorFile, int nFolderSet, CString& strEposUserID );

	void CreatePausedReply(const char* szEposUserID);
	void CreatePausedReplyCSVLine(const char* szEposUserID, CCSV* pCsv);
	
	void CreateNoIDErrorReply();
	void CreateRequestReply ( CString strEposUserID, CCSV* pCsvLine);
	void CreateSearchRequestReply ( CCSV& CsvFindString );
	void ProcessCommand ( CCSV* pCsv );
	
	void ProcessExternalCommand();
	void CreateExternalAuthenticateReply();
	void CreateExternalResetPasswordReply();
	void CreateExternalBalanceReply(bool bDetail);
	void CreateExternalPaymentReply();
	void CreateExternalCreateUserReply();
	void CreateExternalReply(CExternalReplyArray& arrayReply);
	void CreateExternalUserExistsReply();

	bool CanAddNewRecord(const char* szUserID);
	bool AddNewRecord ( const char* szUserID, CCSV* pCsvLine , CCSV* pCsvReply );

	void ProcessRegistration (  const char*szMiFareNo, int nMiFareDbNo, CCSV* pCsvLine );
	void RegistrationError ( const char* szAccountID, const char* szCode, const char* szText );

	void SaveRequestDetails ( CSQLRowAccountFull& AccountRow, CCSV* pCsv, bool bOnDutyGroupShift );
	void ExtractNewRecordDetails (CSQLRowAccountFull& AccountRow, CCSV* pCsv );
	void AuditNewRecord(CSQLRowAccountFull& AccountRow);

	int SearchRecords ( CCSV& CsvFindString );
	
	void CalculateBalanceP1 ( CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CPurse1AvailableSpendInfo& ASInfo );
	double SpendTodayP1 (CSQLRowAccountFull& AccountRow);
	
	void CalculateBalanceP3 (CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CPurse3AvailableSpendInfo& ASInfo);
	double SpendTodayP3 (CSQLRowAccountFull& AccountRow);

	bool HaveAccountAccess ( const char* szAccountID );

	bool IsHotlistedCard(CSQLRowAccountFull&);

	void ProcessReconRequest();
	void ProcessReconRequestInternal();
	void AddReconStatusLine(int nStatus, int nDetail);
	
	CString GetIncomingJSONString();
	void GetIncomingJSONParams(CkJsonObject& jsonRequest, CJSONIncomingParams& params);
	bool ValidateIncomingJSONParams(CJSONIncomingParams& params, CJSONIncomingError& error);
	bool CreateJSONErrorReply(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error);
	void ProcessJSONRequest();
	void ProcessJSONDataRequestEODPrevious(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonRequest, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error);
	void ProcessJSONReportRequestEODPrevious(CJSONObjectCollector& jsonObjectCollector, CkJsonObject& jsonRequest, CkJsonObject& jsonReply, CJSONIncomingParams& params, CJSONIncomingError& error);
	void CheckJSONInt(CkJsonObject* jsonParent, const CString strLabel, const int nMin, const int nMax, bool& bGotValue, int& nValue, CString& strError);
	//void ProcessJSONRequestInternal(CJSONObjectCollector& JSONObjectCollector);
	bool CheckJSONParam(CkJsonObject* jsonParent, CString strLabel, CString& strValue);
	void ApplyJSONError(CkJsonObject* jsonReplyData, int nErrorCode, CString strErrorText);
	void SaveJSONReply(CkJsonObject& jsonReply);

private:
	void RequestCheckBalance();

private:
	void RequestCheckLog(CString strExtension);
	void RequestCheckLogInternal(CString strExtension);

private:
	bool GetSearchFields(CCSV& CSVFindString, CSQLServerSearchInfo& infoSearch);	
	bool SearchGroupType(CSQLRowAccountFull& AccountRow, int nGroupType);

private:
	bool ValidateReconDate(CString strDate);

private:
	bool m_bIsEposRequest;
	bool m_bIsKioskRequest;
	bool m_bIsVendingRequest;
	
	CSSDate m_dateToday;
	CSSTime m_timeToday;

private:
	CTrackedFile m_TrackedFileSystem;
	CTrackedFile m_TrackedFileServer;
	CTrackedFile m_TrackedFilePeriod;
	CTrackedFile m_TrackedFileServiceOptions;
	CTrackedFile m_TrackedFileSmartPayVersion;

private:
	bool m_bVersionMatch;
	CString m_strVersionError;
	COleDateTime m_oleNextActionLogTime;
	ULONGLONG m_nLastPurgeTime;

private:
	CExternalParamsArray m_ExternalParamsArray;

private:
	CLoyReqTidyUpThreadManager m_LoyReqTidyUpThreadManager;
	CLoyReqThreadManager m_LoyReqThreadManager;
	CLoyRspThreadManager m_LoyRspThreadManager;
	CLoyLogThreadManager m_LoyLogThreadManager;

private:
	bool m_bServerInnerLockOK;
	bool m_bBackgroundLockOK;
	LONGLONG m_dwLastOuterLockTryTime;
	LONGLONG m_dwLastOuterLockOKTime;
	LONGLONG m_dwNextOuterLockOffsetSeconds;
	CString m_strOuterLockStuckTime;
};

//*******************************************************************
