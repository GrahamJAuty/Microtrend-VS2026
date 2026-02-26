#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolAccountReceive.h"
#include "EposReportCustomSettings.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportAccountReceiveInfo
{
public:
	CEposReportAccountReceiveInfo();

public:
	CString m_strEventDate;
	CString m_strEventTime;
	CString m_strPurchaseDate;
	CString m_strPurchaseTime;
	COleDateTime m_oleDate;
	int m_nLocIdx;
	int m_nAccountID;
	int m_nSelectArrayIdx;
	int m_nAmount;
	int m_nTender;
	double m_dReceiveVal;
};

/**********************************************************************/

class CEposReportAccountReceive : public CEposReport
{
public:
	CEposReportAccountReceive( CEposSelectArray& SelectArray );
	virtual bool IsDepositReport() = 0;

public:
	void Consolidate();

protected:
	virtual void GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize ) = 0;
	void ConsolidatePaymentBuffer( bool bProcessTransaction );
	void ConsolidatePaymentBufferInternal( bool bRefund );
	void ConsolidateReceiptsToPaymentType( bool bRefund, int nPaymentType );
	void ProcessChargedAccounts();
	bool ProcessCharge( int nLocIdx, int nAccountLineType, CCSVAccountHistory& csvIn );
	bool CheckAccountType( int nType );
	virtual void SetConsolidationLocation( CTermFileListInfo& infoFile, CEposReportAccountReceiveInfo& infoReceive, bool& bGotDepositNames );
		
public:
	bool CreateReport();

protected:
	virtual void AddColumns() = 0;
	void ShowReceiveBlock( int nLocIdx, int nStartPos, int nEndPos, CArray<int,int>& arrayLineIdx, bool bRefund, int nNewDepositStatus );
	void ShowPaymentBlock( int nLocIdx );
	int FindReportItem( int nLocSort, int nType );
	void WriteTotal( double dTotal );

protected:
	const char*	GetReceiveBlockHeaderLine( int nLocIdx, bool bRefund, int nNewDepositStatus );
	virtual bool CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv ) = 0;
	const char* GetPaymentLabel( int nPaymentType, const char* szDefault );
	
protected:
	virtual void ValidatePurchaseTime( int nLocIdx, CString& strPurchaseDate, CString& strPurchaseTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction, bool& bValidPurchaseTime, bool& bValidEventTime );
	virtual bool ValidateEventTime( int nLocIdx, int nAccountID, CString& strEventDate, CString& strEventTime );
		
protected:
	CStringArray m_arrayPaymentBuffer;
	CReportConsolidationArray<CEposReportConsolAccountReceive> m_ConsolArray;
	CReportConsolidationArray<CEposReportConsolAccountPayment> m_PaymentArray;
	CReportConsolidationArray<CEposReportConsolAccountCharged> m_arrayAccountCharged;
	CArray<CEposReportAccountReceiveInfo,CEposReportAccountReceiveInfo> m_arrayReceiveBuffer;
	CArray<CEposReportAccountReceiveInfo,CEposReportAccountReceiveInfo> m_arrayRefundBuffer;
	
protected:
	bool m_bMixedTransaction;
	CString m_strPaymentLabel;
	CString m_strReceiveBlockHeaderLine;

protected:
	CString m_strUnderline;
	int m_nColumnsBeforeTotal;

protected:
	CEposReportCustomSettingsDeposit m_ReportSettings;
};

/**********************************************************************/

