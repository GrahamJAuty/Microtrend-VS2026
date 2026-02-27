#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\RedeemPoints.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLPurchaseControlCache.h"
#include "..\SmartPay4Shared\TLogRecord_base.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRowEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRowPebbleExportPayment.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRowPluInfo.h"
/**********************************************************************/

class CEposSmartPayFile
{
public:
	CEposSmartPayFile(const char* szFilename,int nFolderSet, CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData);
	CEposSmartPayFile(CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData, CStringArray* pExternalLogData);

private:
	void Init();

public:
	bool ScanSharpPos();
	bool ScanExternal();
	bool HaveMultipleTransactions() { return m_bMultipleTransactions; }
	bool RemoveTransactionLines(const char* szLogFilePath, const char* szSecondaryLogFile = "");

	CString GetEposUserID() { return m_strEposUserID; }
	CString GetFullEposUserID() { return m_strFullEposUserID; }
	CString GetError() { return m_strError; }
	int GetExternalCCNo() { return m_nExternalCCNo; }

	void UpdateRecordFromLogData(CSQLRowAccountFull& RowAccount, int& nGroupNo);

	void SaveEposPaymentAudit();

	int GetTransactionCCNo() { return m_nTransactionCCNo; }
	int GetServerNo() { return atoi(m_strServerID); }
	int GetFolderSet() { return m_nFolderSet; }	
	int GetTillNo() { return m_nTillNo; }

private:
	bool ProcessTranLineSharpPos(CCSV* pCsv);
	bool ProcessTranLineExternal(CCSV* pCsv);
	void ProcessTranLineExtra(CCSV* pCsv);
	void ProcessPaymLine(CCSV* pCsv);
	void ProcessItemLine(CCSV* pCsv);
	void ProcessDiscountLine(CCSV* pCsv);
	void ProcessItemDiscountLine(CCSV* pCsv);
	void ProcessPurchaseControlStampAwardLine(CCSV* pCsv);

	int GetBonusMultiplier(double dSpendValue);

	void SaveAudit(CSQLRowAccountFull& RowAccount, const char* szComment = "");
	void LogEposPaymentEntry(int nType, double dValue1, double dValue2 = 0, double dValue3 = 0);
	void SavePurchaseDetails(const char* szUserID);

	void AutoRedeemPoints(CSQLRowAccountFull& RowAccount, CRedeemPoints* pData);

private:
	int CalculateRevaluationBonusPoints(double dValueAdded, double dTripValue, int nPointsPerTrip);
	double CalculateRevaluationBonusValue(double dValueAdded, double dTripValue, double dValuePerTrip);

private:
	void GetRevalueSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup);
	void GetRewardSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup);
	void GetRedeemSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup, bool& bIsRedeemDisabled, CRedeemPoints& pointsRedeemer);
	void ProcessPurse2Refresh(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup);
	void ProcessPurse1Revaluation(CSQLRowAccountFull& RowAccount);
	void ProcessPurse3Revaluation(CSQLRowAccountFull& RowAccount);
	void ProcessUnknownPurse(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, double dUnknownPurseAmount, int nPaymentNo);
	void ProcessPurse1Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup);
	void ProcessPurse2Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup);
	void ProcessPurse3Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup);
	void ProcessRewardSpendBonus(CSQLRowAccountFull& RowAccount);

public:
	void ProcessPurchaseHistory(CSQLRowAccountFull& RowAccount);
	void WritePendingAuditLines();

private:
	CString m_strFilename;
	CString m_strTmpPaymentAuditFile;

	CString m_strError;
	CString m_strEposUserID;
	CString m_strFullEposUserID;
	bool m_bValidData;
	bool m_bMultipleTransactions;
	int m_nTransactionLineCount;

	int m_nTillNo;
	CString m_strServerID;
	CString m_strTransactionNo;
	CSSDate m_dateTransaction;
	CSSTime m_timeTransaction;

	int m_nPluPoints;										// plu points generated from plu sales
	int m_nPointsAwarded;									// get printed on epos receipt

	double m_dAddToPurse1Value;								// Purse1 revaluation plu 
	double m_dAddToPurse3Value;								// Purse3 revaluation Plu

	int m_nTransactionItemCount;							// number of transaction items (incase all 0 priced)
	double m_dEposTransactionValue;							// $TRAN spend value
	double m_dEposSpendPurse1Total;							// $PAYM spend value
	double m_dEposSpendPurse1Liability;						// $PAYM spend value
	double m_dEposSpendPurse1Credit;						// $PAYM spend value
	double m_dEposSpendPurse2;								// $PAYM spend value							
	double m_dEposSpendPurse3Total;							// $PAYM spend value
	double m_dEposSpendPurse3Liability;						// $PAYM spend value
	double m_dEposSpendPurse3Credit;						// $PAYM spend value
	double m_dEposUnknownPursePaym20;						// $PAYM line old format no Purse indicator							
	double m_dEposUnknownPursePaym21;
	double m_dCashSpend;									// cash portion of total spend

	//CString m_strRestrictedP3List;							// list of restricted purse3 departments

	int m_nTransactionSourceType;
	int m_nTransactionAppNo;
	int m_nTransactionCCNo;

	CSQLAuditRecord_base m_atc;

	int m_nRevaluationType;									// Server or group specfic
	double m_dRevaluationTrip;
	double m_dRevaluationTrip2;
	int m_nRevaluationPoints;
	double m_dRevaluationValue;

	int m_nRewardType;										// Server or group specific
	double m_dRewardSpendTrip;
	int m_nRewardPoints;
	double m_dRewardValue;
	double m_dRewardUpperSpendLimit;
	bool m_bHaveRewardUpperSpendLimit;
	double m_dRewardMultiplierTrigger;

	int m_nFolderSet;
	int m_nREQFileType;

	CSQLRowPluInfo m_SQLRowPluInfo;
	CTLogRecord_base m_TLogRecord;
	CString m_strPluDatabaseFilename;

	bool m_bOnDutyGroupShift;

private:
	CStringArray* m_pExternalLogData;
	CSQLPurchaseControlCache m_PurchaseControlCache;
	CPendingExportPaymentData& m_PendingExportPaymentData;
	CPendingEposTerminalData& m_PendingEposTerminalData;	
	bool m_bExternalMode;
	int m_nExternalCCNo;
};

/**********************************************************************/
