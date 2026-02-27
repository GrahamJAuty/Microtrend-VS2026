#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//*******************************************************************
#include "ParentMailData.h"
#include "WebPaymentAtcTransactionFile.h"
//*******************************************************************

class CParentMailExport  
{
public:
	CParentMailExport ( CWnd* pParent = NULL );

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void LinkAccounts (  const char* szListFile );
	
private:
	int LinkNewAccounts ( const char* szListFile, CWorkingDlg* pProgress );

//******************************************************************
#endif
//******************************************************************

public:
	const char* GetError() { return m_strError; }

	bool SendUpdates ( bool bPostData, bool bEODAllowed );
	bool GetEODSent() { return m_bEODSent; }

	int RequestLinkedAccounts ( CWorkingDlg* pProgress );

private:
	void ResetTransaction ( const char* szAccountID = "" );

	int ConvertWebUpdates ( const char* szUpdatesFile, const char* szCsvFilename );

	bool AppendUpdates ( const char* szNewUpdatesFile, const char* szCsvFilename );
	bool RemovedProcessedUpdates ( const char* szUpdateFilename, __int64 nLastProcessedID );
	void AddEndOfDayTransaction ( const char* szUpdateFilename );

	bool ExportUpdatesFile ( const char* szCsvFilename );
	bool IsValidSpend ( CWebPaymentAtcTransactionFile* pFile );
	bool IsValidCashAdded ( CWebPaymentAtcTransactionFile* pFile );
	bool SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp );

	void AuditOpeningBalance(CSQLRowAccountFull& RowAccount);

	void SaveLinkNotification (CSQLRowAccountFull& RowAccount, const char* szException = "" );

private:
	CWnd* m_pParent;
	CParentMailData m_data;
	CSQLRepositoryWebPaymentOptions m_webOptions;

	CString m_strAccountID;
	double m_dSalesValue;
	double m_dFreeValue;
	double m_dBalance;
	CString m_strTransDateTime;
	int m_nTransType;
	int m_nProductID;
	CString m_strEposID;

	CString m_strError;
	bool m_bEODSent;

	int m_nFailedCounter;
	int m_nUpdatesCount;
	int m_nTransactionCount;

	CString m_strTempExceptionFile;
	CString m_strTempLinkedFile;
};
