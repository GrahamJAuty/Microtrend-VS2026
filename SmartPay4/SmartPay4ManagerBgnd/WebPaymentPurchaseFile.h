#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "DbReportHandler.h"
#include "WebPaymentAtcTransactionFile.h"
/**********************************************************************/

class CWebPaymentPurchaseFile
{
public:
	CWebPaymentPurchaseFile ( int nWebPaymentType );
	bool ConvertWebUpdates ( const char* szWebAtcUpdateFile, const char* szWebCsvFile1, const char* szWebCsvFile2 = "" );

	const char* GetError() { return m_strError; }
	int GetExportLineCount() { return m_nExportLineCount; }

private:
	CString NextReferenceNo ( int nEcrCCNO = -1 );

	int SaveAtcWebFields ( CMemberDatabase& DbBalanceList, const char* szUpdatesFile, const char* szWebCsvFile, const char* szHeader, const char* szWebCsvFile2 = "", const char* szHeader2 = "" );
	bool SaveService1 ( const char* szWebCsvFile1, const char* szHeader1, const char* szLine );
	bool SaveService2 ( const char* szWebCsvFile2, const char* szHeader2, const char* szLine );

	CString SaveTucasiSoapDetails ( CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile );
	void SaveWisePayDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile, CString& strService1, CString& strService2 );
	CString SaveSchoolcommsDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile );
	CString SaveTucasiJsonDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile );
	CString SaveExportSalesDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile );

	bool LogAtcUpdates ( const char* szNewUpdates, const char* szPrefix );
	CString GetAccountName (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile );

private:
	int m_nWebPaymentType;

	CString m_strError;
	int m_nExportLineCount;

	int m_nAccountNameFormat;						// 0=Forename Surname, 1=Account Name, 2= Surname only, 3= Surname Forename
	int m_nService1Purse;
	bool m_bIncludePurse3;
	bool m_bIncludePurse2;
	bool m_bIncludePurse1;
	int m_nBalance1Purse;
	int m_nBalance2Purse;

	int m_nService2ID;
	int m_nService2Purse;

	CDbReportHandler m_dbReporter;
	CString m_strAdditionalHeader;

	CString m_strDbBalanceList;
};
