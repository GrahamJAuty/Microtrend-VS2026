#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
#include "..\SPosLoyalty4Shared\RedeemPoints.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferCache.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRowPluInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
#include "..\SPosLoyalty4Shared\TLogRecord_base.h"
/**********************************************************************/
#define STAFFGIFTMODE_NONE 0
#define STAFFGIFTMODE_OK 1
#define STAFFGIFTMODE_EXCEPTION 2
/**********************************************************************/

class CEposLoyaltyFile
{
public:
	CEposLoyaltyFile(const char* szFilename, CServerDataFolderSetIndex& FolderSetIndex);
	CEposLoyaltyFile(CStringArray* pExternalLogData);

private:
	void Init();

public:
	bool ScanLogFile();
	bool ScanLogFileExternal();
	bool HaveMultipleTransactions() { return m_bMultipleTransactions; }
	const char* GetCustomerCardNo() { return m_strCustomerCardNo; }
	const char* GetEposCardNo() { return m_strEposCardNo; }
	const char* GetError() { return m_strError; }
	int GetExternalCCNo() { return m_nExternalCCNo; }
	void UpdateLoyaltyAccount(CSQLRowAccountFull& RowAccount, bool bNewDBRecord);
	int GetTransactionCCNo() { return m_nTransactionCCNo; }
	int GetServerNo() { return m_nServerNo; }
	int GetDbNo() { return m_FolderSetIndex.m_nDbNo; }
	int GetFolderSet() { return m_FolderSetIndex.m_nSetNo; }
	int GetTillNo() { return m_nTillNo;  }

private:
	int GetAuditType() { return m_bExternalMode ? AUDIT_EXT : AUDIT_POS; }
	void ProcessBonusCash(double dSpend, double dBonus, CSQLRowAccountFull& RowAccount, int nAppNoSale, int nAppNoRefund);
	void ProcessBonusPoints(double dSpend, int nPoints, CSQLRowAccountFull& RowAccount, int nAppNoSale, int nAppNoRefund);

public:
	int m_nPointsAwarded;
	double m_dBonusCash;

private:
	bool ProcessTranLine ( CCSV* pCsv );
	void ProcessPaymLine ( CCSV* pCsv );
	void ProcessItemLine ( CCSV* pCsv );
	void ProcessSubtotalDiscountLine ( CCSV* pCsv );
	void ProcessItemDiscountLine(CCSV* pCsv);
	void ProcessOfferStampAwardLine(CCSV* pCsv);
	void ProcessOfferStampRedeemLine(CCSV* pCsv);
	void RemoveLines ( int nCount, const char* szFilename );

	int CalculateRewardPoints( double dSpendCard );
	double CalculateRewardValue( double dSpendCard );
	int GetBonusMultiplier();
	void AutoRedeemPoints (CSQLRowAccountFull& RowAccount, CRedeemPoints* pPointsRedeemer );

	void SaveAuditCustomer(CSQLRowAccountFull& RowAccount, const char* szComment = "");
	int GetMaxDayInMonth(int nMonth, bool bLeapYear = FALSE);
	void AdjustRenewDayNumber(int nYear, int& nRenewDayNumber);

private:
	//HELPER FUNCTIONS FOR UPDATELOYALTYACCOUNT
	void GetRevalueSettings(CSQLRowGroup& RowGroup, bool bGotGroup);
	void GetRewardSettings(CSQLRowGroup& RowGroup, bool bGotGroup);
	void GetRedeemSettings(CSQLRowGroup& RowGroup, bool bGotGroup, bool& bRedeemDisabled, CRedeemPoints& pointsRedeemer);
	void CalculatePurseSpend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, bool IsPurse1PriorityAllowed, bool bIsPurse2Allowed);
	void ProcessRevaluation(CSQLRowAccountFull& RowAccount, int nPurseNo, int nTopupNo);
	void ProcessRenewalItems(CSQLRowAccountFull& RowAccount);
	void ProcessPurse1Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup);
	void ProcessPurse2Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup);
	void ProcessCashSpend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup);
	void ProcessReward(CSQLRowAccountFull& RowAccount);
	void ProcessDeptReward(CSQLRowAccountFull& RowAccount);
	void SaveAuditInternal(const char* szComment = "");

public:
	void ProcessPurchaseHistory(CSQLRowAccountFull& RowAccount);
	void WriteAuditBuffer();
	
private:
	CString m_strFilename = "";
	
	CString m_strCustomerCardNo = "";
	CString m_strEposCardNo = "";
	bool m_bValidData = FALSE;
	bool m_bMultipleTransactions = FALSE;
	CString m_strError = ""	;

	CSSDate m_dateTransaction;
	CSSTime m_timeTransaction;
	int m_nTransactionPoints = 0;
	int m_nPluPoints = 0;
	bool m_bAllowPluOrDeptBonus = FALSE;
	double m_dCashSpend = 0.0;
	double m_dCardSpend = 0.0;
	double m_dPurse1Spend = 0.0;
	double m_dPurse2Spend = 0.0;
	double m_dGeneralBonusExcludedSpend = 0.0;
	double m_dTotalDiscount = 0.0;
	bool m_bGotItemLine = FALSE;
	double m_dAddToCardValue[3][ REVALUE_PLUNO_PERPURSE_COUNT + 1 ];
	int m_nRenewCount[ RENEW_PLUNO_COUNT + 1 ];

	int m_nTillNo = 0;
	int m_nServerNo = 0;
	int m_nTransactionCCNo = 0;
	int m_nTransactionAppNo = 0;
	CString m_strTransactionNo = "";
	CAuditRecord_base m_atc;

	CServerDataTopupInfo m_RevaluationInfo[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];

	double m_dRewardTrip = 0.0;
	int m_nRewardType = 0;
	int m_nRewardPoints = 0;
	double m_dRewardValue = 0.0;
	double m_dRewardMultiplierTrigger = 0.0;
	double m_dRewardUpperSpendLimit = 0.0;
	bool m_bHaveRewardUpperSpendLimit = FALSE;
	bool m_bRewardIncludeCashSpend = FALSE;
	bool m_bRewardIncludePurse1Spend = FALSE;
	bool m_bRewardIncludePurse2Spend = FALSE;

	CTLogRecord_base m_TLogRecordPurchase;
	
	int m_nBonusMultiplier = 0;
	bool m_bGroupDisablePluBonus = FALSE;
	bool m_bGroupDisableDeptBonus = FALSE;

private:
	int m_nStaffGiftServerNo = 0;

private:
	CReportConsolidationArray<CConsolidatedDoubleByInt> m_arrayDeptSpendBuffer;
	CSQLStampOfferCache m_StampOfferCache;
	CStringArray m_arrayPendingAudit;
	CStringArray* m_pExternalLogData;
	bool m_bExternalMode = FALSE;
	int m_nExternalCCNo = 0	;
	CServerDataFolderSetIndex m_FolderSetIndex;
};

/**********************************************************************/
