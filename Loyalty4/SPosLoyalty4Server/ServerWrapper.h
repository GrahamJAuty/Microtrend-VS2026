#pragma once
/**********************************************************************/
#include "CkJsonObject.h"
#include "CkJsonArray.h"
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "ExternalParamsArray.h"
#include "ExternalReplyArray.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyLockManager.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPosLoyalty4Shared\TrackedFile.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
/**********************************************************************/
#include "LoyLogThreadManager.h"
#include "LoyReqTidyUpThreadManager.h"
#include "LoyReqThreadManager.h"
#include "LoyRspThreadManager.h"
/**********************************************************************/
#define RSPFILE_ERROR_NONE 0
#define RSPFILE_ERROR_LEGACY 1
#define RSPFILE_ERROR_ACCOUNT_NUMBER 2
#define RSPFILE_ERROR_ACCOUNT_UNKNOWN 3
#define RSPFILE_ERROR_ACCOUNT_EXPIRED 4
#define RSPFILE_ERROR_ACCOUNT_HOTLISTED 5
#define RSPFILE_ERROR_ACCOUNT_INACTIVE 6
#define RSPFILE_ERROR_ACCOUNT_GROUP 7
#define RSPFILE_ERROR_OPEN_DATABASE 8
#define RSPFILE_ERROR_BAD_QRPREFIX 9
#define RSPFILE_ERROR_BAD_QRFORMAT 10
//***************************************************************
#define RECON_ERROR_NONE 0
#define RECON_ERROR_REQUEST 1
#define RECON_ERROR_AUDIT 2
#define RECON_ERROR_DATE 3
//***************************************************************

struct CSPOSSearchInfo
{
public:
	CSPOSSearchInfo();

public:
	void SetSearchInfo(CString strFindString);

private:
	void Reset();

public:
	int m_nMaxToFind;
	bool m_bCheckForename;
	bool m_bCheckSurname;
	bool m_bCheckPostcode;
	bool m_bExcludeHotlist;
	bool m_bExcludeInactive;
	CString m_strForename;
	CString m_strSurname;
	CString m_strPostcode;
};

//***************************************************************
//***************************************************************
//***************************************************************

struct CJSONFieldInfo
{
public:
	CJSONFieldInfo(const recInt field, int nLoyaltyField)
	{
		m_strFieldName = field.Label;
		m_nLoyaltyField = nLoyaltyField;
		m_strType = "int";
		m_nMinLength = 0;
		m_nMaxLength = 0;
		m_nMinInt = field.Min;
		m_nMaxInt = field.Max;
		m_dMinDouble = 0.0;
		m_dMaxDouble = 0.0;
	}

	CJSONFieldInfo(const recString field, int nLoyaltyField)
	{
		m_strFieldName = field.Label;
		m_nLoyaltyField = nLoyaltyField;
		m_strType = "string";
		m_nMinLength = field.Min;
		m_nMaxLength = field.Max;
		m_nMinInt = 0;
		m_nMaxInt = 0;
		m_dMinDouble = 0.0;
		m_dMaxDouble = 0.0;
	}

	CJSONFieldInfo(const recDouble field, int nLoyaltyField)
	{
		m_strFieldName = field.Label;
		m_nLoyaltyField = nLoyaltyField;
		m_strType = "double";
		m_nMinLength = 0;
		m_nMaxLength = 0;
		m_nMinInt = 0;
		m_nMaxInt = 0;
		m_dMinDouble = field.Min;
		m_dMaxDouble = field.Max;
	}

public:
	CString m_strFieldName;
	int m_nLoyaltyField;
	CString m_strType;
	int m_nMinLength;
	int m_nMaxLength;
	int m_nMinInt;
	int m_nMaxInt;
	double m_dMinDouble;
	double m_dMaxDouble;
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
public:
	CServerWrapper();
	~CServerWrapper();

public:
	void ShutdownThreads();

public:
	void Pulse();
	void InitSystem( bool bForce );

public:
	void ProcessUpdatePluRequest();
	static void UpdatePluInfo(int nDbNo);
	static void UpdateDeptInfo(int nDbNo);
	static void UpdateStampOfferInfo();
	static void UpdateLoyaltySchemeInfo();

public:
	static CString RemoveSpaces(const char* szText);

private:
	void RequestCheckBalance();
	void RequestCheckBalanceInternal();
	void CreateBalanceReply1(CAccountIdentifier& AccountIdentifier, bool bPrecheckedCardNo, bool bFromCheckBalance = FALSE );
	void CreateBalanceReply2(CSQLRowAccountFull& RowAccount, int nPointsAward, double dBonusCash);
	void CreateBalanceReplyInternal(int nType, CSQLRowAccountFull& RowAccount, bool bFromCheckBalance = FALSE, int nPointsAward = 0, double dBonusCash = 0);

	void ProcessReconRequest();
	void ProcessReconRequestInternal();
	void AddReconStatusLine(int nStatus, int nDetail);

	void ProcessJSONRequest();
	void ProcessJSONRequestInternal(CJSONObjectCollector& JSONObjectCollector);
	void ProcessJSONGetConfig(CJSONObjectCollector& JSONObjectCollector, CkJsonObject* jsonReplyData, bool& bJsonCreationError);
	void ProcessJSONCreateAccount(CJSONObjectCollector& JSONObjectCollector, CkJsonObject* jsonRequestMessage, CkJsonObject* jsonReplyData, CString& strError);
	bool CheckJSONParam(CkJsonObject* jsonParent, CString strLabel, CString& strValue);
	void CheckJSONDate(CkJsonObject* jsonParent, const recString field, bool bIsDate, bool& bGotValue, CString& strValue, CString& strError);
	void CheckJSONString(CkJsonObject* jsonParent, const recString field, bool& bGotValue, CString& strValue, CString& strError);
	void CheckJSONInt(CkJsonObject* jsonParent, const recInt field, bool& bGotValue, int& nValue, CString& strError);
	bool AddJSONFieldInfo(CUserTexts& UserTexts, CkJsonArray* jsonArrayFields, const CJSONFieldInfo& fieldInfo);
	void SaveJSONReply(CkJsonObject& jsonReply);
	void ApplyJSONError(CkJsonObject* jsonReplyData, int nErrorCode, CString strErrorText);

	void ProcessExternalCommand();
	void CreateExternalAuthenticateReply();
	void CreateExternalResetPasswordReply();
	void CreateExternalBalanceReply(bool bDetail);
	void CreateExternalPaymentReply();
	void CreateExternalCreateUserReply();
	void CreateExternalReply(CExternalReplyArray& arrayReply);
	void CreateExternalUserExistsReply();

	void CreatePausedReplyCSVLine(const char* szEposUserID, CCSV* pCsv);
	void CreatePausedReply(const char* szEposUserID);

	void CreateSearchReplyAccountLookup(const char* szCsvLine);
	int SearchRecordsAccountLookup(const char* szCsvFindString);
	void CreateSearchReplySnakesAndLadders(const char* szCsvFindString);
	int SearchRecordsSnakesAndLadders(const char* szCsvFindString, CString& strCardNo);

	void RequestCheckLog(CString strExtension);
	void RequestCheckLogInternal(CString strExtension);
	bool ProcessLogFile(CSQLRowAccountFull& RowAccount, const char* szLogFilePath, const char* szErrorFilePath, CServerDataFolderSetIndex& FolderSetIndex, CString& strEposCardNo, int& nPointsAward, double& dBonusCash);

	void ProcessRegistration(const char* szMiFareNo, CCSV* pCsvLine);
	void RegistrationError(const char* szAccountID, const char* szCode, const char* szText);

	void CheckImports();

	double GetSpendToday(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup);
	void CalculateAvailableSpend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, double& dP1Balance, double& dP2Balance, double& dAvailableSpend);

	CString SetLine(const char* szData, const char* szText);
	void SaveLine(CString strLine);
	bool HaveAccountAccess(const char* szAccountID);

	CString GetRecordData(CSQLRowAccountFull& RowAccount, const char* szField, const char* szText, double dBalanceP1, double dBalanceP2, double dAvailableSpend);

private:
	CString StringSeparatorLine()		{ return "***************************************"; }
	CString StringMoreExists()			{ return "#MORE"; }
	CString StringNoneExists()			{ return "#NONE"; }
	CString StringErrorCodeDbAccess()	{ return "ER=101"; }
	CString StringErrorTextDbAccess()	{ return "EM=Unable to access Loyalty Database"; }
	CString StringErrorTextDbAccess2()	{ return "EM=Unable to access card Database"; }
	CString StringErrorCodeUnknownID()	{ return "ER=102"; }
	CString StringErrorTextUnknownID()	{ return "EM=No account found"; }
	CString StringErrorCodeInUse()		{ return "ER=106"; }
	CString StringErrorTextInUse()		{ return "EM=Account in use"; }	
	CString StringErrorCodePaused()		{ return "ER=107"; }
	CString StringErrorTextPaused()		{ return "EM=Server busy"; }
	CString StringErrorCodeUnsupported() { return "ER=108"; }
	CString StringErrorTextUnsupported() { return "EM=Unsupported request"; }

private:
	bool ValidateReconDate(CString strDate);
	
private:
	CString GetTimeNowString();
	void Diagnostic(CString strMessage);

private:
	void CheckNormalImport();
	void CheckQRInfoImport();

private:
	CSSDate m_dateToday;
	CSSTime m_timeToday;

private:
	CTrackedFile m_TrackedFileSystem;
	CTrackedFile m_TrackedFileServer;
	CTrackedFile m_TrackedFilePeriod;
	CTrackedFile m_TrackedFileServiceOptions;
	CTrackedFile m_TrackedFileLoyaltyVersion;
	
private:
	double m_dBonusCash;
	bool m_bGroupExists;
	int m_nImportCounter;

private:
	bool m_bVersionMatch;
	CString m_strVersionError;
	COleDateTime m_oleNextActionLogTime;
	ULONGLONG m_nLastPurgeTime;

private:
	CLoyReqTidyUpThreadManager m_LoyReqTidyUpThreadManager;
	CLoyReqThreadManager m_LoyReqThreadManager;
	CLoyRspThreadManager m_LoyRspThreadManager;
	CLoyLogThreadManager m_LoyLogThreadManager;

private:
	CExternalParamsArray m_ExternalParamsArray;
	
private:
	bool m_bServerInnerLockOK;
	LONGLONG m_dwLastOuterLockTryTime;
	LONGLONG m_dwLastOuterLockOKTime;
	LONGLONG m_dwNextOuterLockOffsetSeconds;
	CString m_strOuterLockStuckTime;
};

//*******************************************************************
