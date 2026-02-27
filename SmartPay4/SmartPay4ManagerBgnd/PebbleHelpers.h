#pragma once
//$$******************************************************************
#include "afxdb.h"
#include "CkPrivateKey.h"
#include "CkRest.h"
//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLRowSetPebblePayment.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRowPebbleExportTransaction.h"
//$$******************************************************************
#define JSONWT_TTL 1800
//$$******************************************************************

struct CPebbleMemberBuffer
{
public:
	CPebbleMemberBuffer();

public:
	CString m_strMemberType = "";
	CString m_strMemberID = "";
	CString m_strMisID = "";
	CString m_strUPN = "";
	CString m_strFirstName = "";
	CString m_strLastName = "";
	CString m_strDOB = "";
	bool m_bIsActive = FALSE;
};

//$$******************************************************************

struct CPebblePurseBuffer
{
public:
	CPebblePurseBuffer();

public:
	CString m_strMemberID;
	CString m_strPurseID;
	CString m_strTitle;
	CString m_strType;
	CString m_strBalance;
};

//$$******************************************************************

struct CPebbleTransactionItem
{
public:
	CPebbleTransactionItem();

public:
	__int64 m_nPluNo;
	double m_dItemQty;
	double m_dItemCost;
	CString m_strDescription;
	CString m_strVATBand;
};

//$$******************************************************************

struct CPebbleTransactionPayment
{
public:
	CPebbleTransactionPayment();

public:
	int m_nType;
	CString m_strName;
	double m_dAmount;
};

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

class CPebbleHelpers
{
public:
	CPebbleHelpers();
	void CleanUp();

public:
	CString GetMembers(CSQLRepositoryPebbleConfig& PebbleConfig, CArray<CPebbleMemberBuffer,CPebbleMemberBuffer>& arrayMembers, CWorkingDlg* pWorkingDlg = NULL);
	CString GetTransactions(CSQLRepositoryPebbleConfig& PebbleConfig, bool bSales, CDatabase* pDatabase, CWorkingDlg* pWorkingDlg, int& nTranCountTotal, CString& strLatestDateTime );
	CString GetPurses(CSQLRepositoryPebbleConfig& PebbleConfig, CString strMemberID, CArray<CPebblePurseBuffer, CPebblePurseBuffer>& arrayPurses);
	
public:
	CString SendTopupTransactionBatch(CSQLRepositoryPebbleConfig& PebbleConfig, CSQLRowPebbleExportTransaction RowTran, CArray<CPebbleTransactionPayment, CPebbleTransactionPayment>& arrayPayments);
	CString SendSaleTransactionBatch(CSQLRepositoryPebbleConfig& PebbleConfig, CSQLRowPebbleExportTransaction RowTran, CArray<CPebbleTransactionItem, CPebbleTransactionItem>& arrayItems, CArray<CPebbleTransactionPayment, CPebbleTransactionPayment>& arrayPayments);
	CString SendTransferTransactionBatch(CSQLRepositoryPebbleConfig& PebbleConfig, CSQLRowPebbleExportTransaction RowTran, bool bXFerOut);

public:
	void ResetTokens();
	CString GetJWT() { return m_strJWT; }
	CString GetIAT() { return m_strIAT; }

public:
	CString GetCreditPurseTitle() { return "SmartPayFSM"; }

public:
	bool LoadPebblePayments();
	bool IsValidSPOSPaymentType(int nType, bool bTopup);

private:
	void SortSPOSPaymentsByPebbleType(CArray<CPebbleTransactionPayment, CPebbleTransactionPayment>& arraySPOSPayments, CReportConsolidationArray<CConsolidatedDoubleByInt>& arrayPebblePayments, bool bExcludePursePay);
	void DummyFirstAPICall(CSQLRepositoryPebbleConfig& PebbleConfig, CWorkingDlg* pWorkingDlg);

private:
	bool UpdateJWT();
	bool UpdateIAT(CSQLRepositoryPebbleConfig& PebbleConfig, CString& strErrorm, CWorkingDlg* pWorkingDlg = NULL);
	bool UpdateRestConnect(CString& strError);
	void ClearRest();

private:
	void LogRestError();
	void UpdatePendingTransactionCount(CWorkingDlg* pWorkingDlg, bool bSales, int nNew, int nDuplicate, int nLoopback, int nEmpty);
	CString GetPebbleURLRoot();

private:
	void Diagnostic(CkJsonObject* pObject, bool bReceive, CString strCaption);
	void Diagnostic(CString strMessage, bool bReceive, CString strCaption);
	void DiagnosticTranDump(CString strTranId, CString strMemberId, CString strDateTime, CString strTag, int nAmount, bool bSale, CString strResult);

private:
	CString GetJWTAuth();
	CString GetIATAuth();

private:
	CkPrivateKey m_PrivateKeyDemo;
	CkPrivateKey m_PrivateKeyProduction;
	CkRest m_Rest;
	CString m_strJWT;
	CString m_strIAT;
	int m_nJWTExpireTime;
	int m_nIATExpireTime;
	bool m_bRestConnected;
	bool m_bDoneFirstAPICall;

private:
	CSQLRowSetPebblePayment m_RowSetPebblePayment;
};

//$$******************************************************************
extern CPebbleHelpers* pPebbleHelpers;
//$$******************************************************************
