#pragma once
//$$******************************************************************
#include "SquidData.h"
#include "TransactionReferenceDatabase.h"
#include "WebPaymentAtcTransactionFile.h"
#include "WebPaymentList.h"
//$$******************************************************************

class CSquidExport  
{
public:
	CSquidExport( CWnd* pParent = NULL );
	
//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void LinkAccounts (  const char* szListFile );
	void DisassociateCards();

private:
	int LinkNewAccounts ( const char* szTitle, const char* szListFile );
	int DisassociateCards ( const char* szTitle );

//******************************************************************
#endif
//******************************************************************

public:
	bool SendUpdates();
	const char* GetError() { return m_strError; }

private:
	void ResetTransaction ( const char* szAccountID = "" );
	
	int ConvertWebUpdates ( const char* szWebUpdatesFile, const char* szCsvFilename );
	bool IsValidSpend ( CWebPaymentAtcTransactionFile* pFile );
	bool IsValidCashAdded ( CWebPaymentAtcTransactionFile* pFile );

	bool SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp );

	void SaveLinkReference(CSQLRowAccountFull& RowAccount);
	void SaveLinkNotification (CSQLRowAccountFull& RowAccount, const char* szException = "" );

private:
	CWnd* m_pParent;
	CSquidData m_data;

	CString m_strError;
	
	bool m_bRpuTransaction;
	CString m_strAccountID;
	double m_dSalesValue;
	double m_dBalance;
	CString m_strTransType;
	CString m_strDescription;
	CString m_strTransReference;

	CString m_strTempExceptionFile;
	CString m_strTempLinkedFile;

	CWebPaymentListDatabase m_WebPaymentList;
	__int64 m_nRpuSequence;

	CTransactionReferenceDatabase m_TransRef;

	int m_nFailures;
	CString m_strLinkError;
};
