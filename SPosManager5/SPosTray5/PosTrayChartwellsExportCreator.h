#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "..\SmartPaySQL\SQLDb.h"
/**********************************************************************/
#include "ReportConsolidationArray.h"
#include "EposSelectArray.h"
#include "ChartwellsOptions.h"
#include "PosTrayTaskInfo.h"
#include "TermFileListInfo.h"
/**********************************************************************/

struct CPosTrayChartwellsTransactionInfo
{
public:
	CPosTrayChartwellsTransactionInfo()
	{
		m_nNewTransactionID = 0;
		m_strNewAccountID = "";
		m_strNewDate = "";
		m_nNewTerminalNo = 0;
		m_strCurrentAccountID = "";
		m_nCurrentTransactionID = 0;
		m_strCurrentDate = "";
		m_nCurrentTerminalNo = 0;
		m_nGroupNo = 0;
		m_strGroupName = "";
		m_strYear = "";
		m_strClass = "";
		m_nTranPriceBand = 0;
	}

public:
	void CheckNewSettings(bool bForceCheck);

public:
	__int64 m_nNewTransactionID;
	CString m_strNewAccountID;
	CString m_strNewDate;
	int m_nNewTerminalNo;

public:
	__int64 m_nCurrentTransactionID;
	CString m_strCurrentAccountID;
	CString m_strCurrentDate;
	int m_nCurrentTerminalNo;

public:
	int m_nGroupNo;
	CString m_strGroupName;
	CString m_strYear;
	CString m_strClass;
	int m_nTranPriceBand;
};

/**********************************************************************/

struct CPosTrayChartwellsPaymentInfo
{
public:
	CPosTrayChartwellsPaymentInfo()
	{
		m_nPaymentNo = 0;
		m_nPaymentType = 0;
		m_strPaymentName = "";
		m_dAmount = 0.0;
	}

public:
	int m_nPaymentNo;
	int m_nPaymentType;
	CString m_strPaymentName;
	double m_dAmount;
};

/**********************************************************************/

struct CPosTrayChartwellsPaymentBuffer
{
public:
	CPosTrayChartwellsPaymentBuffer()
	{
		Reset();
	}

	void Reset()
	{
		m_arrayPayment.RemoveAll();
		m_bInTransaction = FALSE;
		m_strDate = "";
		m_strTime = "";
		m_nServerNo = 0;
		m_dPursePayment = 0.0;
	}

public:
	bool m_bInTransaction;
	CString m_strDate;
	CString m_strTime;
	int m_nServerNo;
	double m_dPursePayment;
	CArray<CPosTrayChartwellsPaymentInfo, CPosTrayChartwellsPaymentInfo> m_arrayPayment;
};

/**********************************************************************/

struct CPosTrayChartwellsExportSalesItemSubType
{
public:
	CPosTrayChartwellsExportSalesItemSubType()
	{
		Reset();
	}

	void Reset()
	{
		m_bInUse = FALSE;
		m_nQty = 0;
		m_nValue = 0;
		m_nDiscount = 0;
	}

public:
	bool m_bInUse;
	int m_nQty;
	int m_nValue;
	int m_nDiscount;
};

/**********************************************************************/

struct CPosTrayChartwellsExportSalesItem
{
public:
	CPosTrayChartwellsExportSalesItem();
	void Reset();

public:
	int Compare(CPosTrayChartwellsExportSalesItem& source, int nHint = 0);
	void Add(CPosTrayChartwellsExportSalesItem& source);

public:
	void AllocateDiscounts();

public:
	__int64 m_nTransactionID;
	CString m_strDate;
	CString m_strTime;
	CString m_strAccountID;
	__int64 m_nPluNo;
	int m_nDeptNo;
	int m_nGroupNo;
	int m_nCategoryNo;
	int m_nPriceBand;
	int m_nServerNo;
	CString m_strVATBand;

public:
	// 0 = Sale, 1 = Refund, 2 = Void, 3 = Discount
	CPosTrayChartwellsExportSalesItemSubType m_SubType[4];
	
public:
	int m_nTotalValue;
	int m_nTotalQty;
	int m_nTotalDiscount;
	bool m_bGotDiscount;
};

/**********************************************************************/

struct CPosTrayChartwellsExportTranItem
{
public:
	CPosTrayChartwellsExportTranItem();
	void Reset();

public:
	int Compare(CPosTrayChartwellsExportTranItem& source, int nHint = 0);
	void Add(CPosTrayChartwellsExportTranItem& source);

public:
	__int64 m_nTransactionID;
	CString m_strDate;
	CString m_strTime;
	CString m_strAccountID;

public:
	int m_nQty;
	int m_nValue;
	int m_nDiscount;
	int m_nVAT;
	bool m_bGotDiscount;
};

/**********************************************************************/

class CPosTrayChartwellsExportCreator
{
public:
	CPosTrayChartwellsExportCreator( CPosTrayChartwellsScheduleInfo& infoExport );
	~CPosTrayChartwellsExportCreator(void);

public:
	bool CheckDbAccess(CSQLDb& SQLDb, int& nErrorNo, int& nVersionNo, int& nExternalVersionNo);
	bool ProcessExport( COleDateTime& dateReport );
	
private:
	bool CreateHeaderFile(CString& strFolder, CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo);
	
private:
	bool CreatePaymentFile(CString& strFolder, CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo);
	void WritePaymentBuffer(CSSFile& fileOut, CTermFileListInfo& infoFile);
	void LogCanteenPayment(CSSFile& fileOut, CTermFileListInfo& infoFile, CString strOutputDate, int nPurse, double dAmount, CString strName, double& dLogged);
	CString GetPaymentTypeName(int nPaymentType);	

private:
	bool CreateSalesFile(CString& strFolder, CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo);

private:
	void GetTermFileList(CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize, bool bSales);
	CString GetServerName(int nServerNo);
	CString GetOutputDate(CString strDate);
	CString PenceToString(int nPence);
	CString QtyToString(int nQty);
	int DoubleToPence(double dValue);
	int DoubleToQty(double dQty);

private:
	void RedirectToEmail(CString strOutputFile, CString strType);

private:
	CPosTrayChartwellsScheduleInfo& m_infoExport;
	
private:
	CEposSelectArray m_SelectArray;
	CPosTrayChartwellsTransactionInfo m_TransactionInfo;
	CPosTrayChartwellsPaymentBuffer m_PaymentBuffer;
	CReportConsolidationArray<CPosTrayChartwellsExportTranItem> m_TranBuffer;
	CReportConsolidationArray<CPosTrayChartwellsExportSalesItem> m_SalesBuffer;
	CString m_strCheckDate;
};

/**********************************************************************/
#endif
/**********************************************************************/
