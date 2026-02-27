#pragma once
//************************************************************************
#include "SQLAuditRecord.h"
#include "ClosingBalanceDatabase.h"
#include "SmartPayAuditFile.h"
//************************************************************************

class CClosingBalance : public CClosingBalanceDatabase
{
public:
	CClosingBalance();
	~CClosingBalance(void) {}

	bool Read ( int nMode );
	CString GetCurrentKey() { return m_strCurrentKey; }

	bool GetNearestDate ( const char* szDate );

//	bool HaveBalances();
	bool HaveBalances ( CSSDate* pDate );
	bool HaveBalances ( const char* szDate );

	bool SaveBalances ( double dLiabilityPurse1, double dBalancePurse2, double dLiabilityPurse3, double dCreditPurse1, double dCreditPurse3 );
	bool SaveBalances ( CSSDate* pDate, CSSTime* pTime, double dLiabilityPurse1, double dBalancePurse2, double dLiabilityPurse3, double dCreditPurse1, double dCreditPurse3 );

	bool CalculateBalances ( const char* szDate, CSmartPayAuditFile* pAudit );

private:
	int IsValidAtc ( CSQLAuditRecord* atcRecord );

	bool SaveCalculatedBalances ( CSSDate* pDate );

	void SaveStartingBalanceLine ( bool bCalcBalance );
	void SaveDatabaseLine ( const char* szUserID );

private:
	CSQLAuditRecord m_atc;

	CSSDate m_dateBalance;

	CString m_strCurrentKey;
	double m_dLiabilityPurse1;
	double m_dBalancePurse2;
	double m_dLiabilityPurse3;
	double m_dCreditPurse1;
	double m_dCreditPurse3;
};

//************************************************************************
