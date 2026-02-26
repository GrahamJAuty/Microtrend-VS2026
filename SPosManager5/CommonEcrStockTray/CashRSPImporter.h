#pragma once
/**********************************************************************/
#include "CashRSPImporterDefines.h"
#include "CashRSPImporterDepositInfo.h"
#include "CashRSPImporterDiscountHelpers.h"
#include "CashRSPImporterOutputFile.h"
#include "CashRSPImporterStatusDisplay.h"
#include "CashRSPSpooler.h"
#include "DepartmentCSVArray.h"
#include "LockManager.h"
/**********************************************************************/

class CCashRSPImporter  
{
public:
	CCashRSPImporter();

	void SetRebuildRemoveDuplicatesFlag( bool b ){ m_bRebuildRemoveDuplicates = b; }
	int GetDuplicatedTransactionCount(){ return m_nDuplicatedTransactions; }
	void ClearDuplicatedTransactionCount(){ m_nDuplicatedTransactions = 0 ; } 
	
	void RebuildLocationSales( int nNetworkIdx, CString& strFrom, CString& strTo );	
	bool ProcessNetworkSales( int nNetworkIdx, CLockManager& LockManager, CCashRSPImporterStatusDisplay& StatusDisplay );

	bool GetCashRSPVersionCheckerFailFlag(){ return m_TransactionInfo.m_CashRSPVersionChecker.GetCashRSPFormatFailFlag(); }
	void ClearCashRSPVersionCheckerFailFlag(){ m_TransactionInfo.m_CashRSPVersionChecker.ClearCashRSPFormatFailFlag(); }
	void ShowCashRSPVersionCheckerFailMessage(){ m_TransactionInfo.m_CashRSPVersionChecker.ShowCashRSPFormatFailMessage(TRUE); }
	int GetCurrentTransactionVersionNo() { return m_TransactionInfo.GetCurrentTransactionVersionNo(); }

private:
	int ValidateSSFileTransactionFilename( CString& strFilename );
	int GetLocIdxByFilename( const char* szFilename, int nNetwkIdx );
	void ProcessCashRSPFile( const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx, int nFileType, int nProcessType, CCashRSPImporterStatusDisplay& StatusDisplay );
	void ProcessDepositFile( const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx );
	void ProcessEODDateFile( const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx );

private:
	void FinaliseTransaction( int nConnectionType, CCashRSPSpooler& CashRSPSpooler );
	void KludgeDailyItemLines();
	void BuildItemBuffer();
	void ProcessDiscountVoidBuffer();
	void ProcessPaymentsBuffer(CCashRSPSpooler& CashRSPSpooler);
	void ProcessTransactionPostToAccount( CCSVEposTermLine& csvIn, int nType, __int64 nID );
	void ProcessItemBufferSale();
	void ProcessSingleItemSales( CCashRSPImporterItem& csvItem, int nItemServerNo );
	void ProcessSingleItemReasons( CCashRSPImporterItem& csvItem, int nItemServerNo );
	void ProcessItemBufferDelivery();
	void ProcessSeparateDiscount( double dValue, int nDiscountMode, const char* szDiscountType, int nDiscountRate, bool bTaxable, const char* szTaxBand = "", int nMixMatchType = 0, int nMixMatchNo = 0 );
	bool ProcessDepositInfo( CCashRSPImporterDepositInfo& DepositInfo );
	void ProcessMixMatchUseLine(CCSVEposTermLine& csvIn);
	
private:
	void PrepareDiscounts();
	void ProcessDiscounts();
	void ProcessDiscounts_Item();
	void ProcessDiscounts_LegacySubtotal();
	void ProcessDiscounts_MMDisc();
	void ProcessDiscounts_PDisc();
	bool CheckPromoRules();
	bool CanUsePromoRules(int nPromoNo);
	bool GotMMDiscLines(int nPromoNo, int nOfferNo );

private:
	void ConsolidateItemDiscountToWrite( CCashRSPImporterDiscountShareList& DiscountShare, int nIdx, CCashRSPImporterDiscountHelperPendingItem& infoPending );
	void ConsolidateItemDiscountToWrite( CCashRSPImporterDiscountShareList& DiscountShare, int nIdx, CCashRSPImporterDiscountHelperPendingItem& infoPending, double dVal );
	void ConsolidateDiscountToWrite( int nSaleIdx, CCashRSPImporterDiscountHelperPendingSubtotal& infoSubtotal, double dVal );

private:
	void WriteDailyTermFile( bool& bGotDuplicate );
	void WriteDailySalesFiles();
	void WritePendingStockFiles( int nConnectionType );

private:
	bool GetDbAndLocIdx( int nNetworkIdx, int nExt, int& nDbIdx, int& nLocIdx );
	void GetServerFileInfo( int nNetworkIdx, int nExt, int& nServerDbIdx, int& nServerLocIdx );
	void OpenServerFile( int nServerDbIdx, int nServerLocIdx );

private:
	void Diagnostic(CString str) { /*AfxMessageBox(str);*/ }

private:
	CCashRSPImporterBatchInfo m_BatchInfo;
	CCashRSPImporterTransaction m_TransactionInfo;
	
private:
	CCashRSPImporterOutputFile* m_pOutputFileTermNonDuplicates;
	CCashRSPImporterOutputFile* m_pOutputFileDailyTerm;
	CCashRSPImporterOutputFile* m_pOutputFileDailySales;
	CCashRSPImporterOutputFile* m_pOutputFileDailyReasons;
	CCashRSPImporterOutputFile* m_pOutputFileDailyAccount;
	CCashRSPImporterOutputFile* m_pOutputFileStockSales;
	CCashRSPImporterOutputFile* m_pOutputFileStockDelivery;
	CCashRSPImporterOutputFile* m_pOutputFileDailyItemPay;

private:
	CPromotionsCSVArray m_arrayPromotions;
	CDepartmentCSVArray m_arrayDepartments;
	CPluCSVArray m_arrayPluItems;

private:
	bool m_bRebuildRemoveDuplicates;
	int m_nDuplicatedTransactions;
};

/**********************************************************************/
extern CCashRSPImporter CashRSPImporter;
/**********************************************************************/