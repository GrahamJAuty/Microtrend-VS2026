#pragma once
/**********************************************************************/
#define DEPOSITINFO_WANT_DEPOSIT 0
#define DEPOSITINFO_WANT_HEADER 1
#define DEPOSITINFO_WANT_BOOKING 2
#define DEPOSITINFO_WANT_CUSTNAME 3
#define DEPOSITINFO_WANT_CUSTINFO 4
/**********************************************************************/

class CCashRSPImporterDepositInfo
{
public:
	CCashRSPImporterDepositInfo(void);
	~CCashRSPImporterDepositInfo(void);

public:
	void ResetNewDeposit();
	void ResetCurrentDeposit();
	void ProcessLine( const char* szLine );
	void PostProcessLine();

public:
	void ForceDepositEnd();
	bool GotCurrentDeposit(){ return m_bGotCurrentDeposit; }

public:
	int GetDepositID(){ return m_nDepositID; }
	int GetServerNo(){ return m_nServerNo; }
	int GetDepositAmount(){ return m_nDepositAmount; }
	const char* GetTaxBand(){ return m_strTaxBand; }
	int GetTaxAmount(){ return m_nTaxAmount; }
	const char* GetBooking(){ return m_strBooking; }
	const char* GetCustName(){ return m_strCustName; }
	const char* GetCustInfo( int n );
	
private:
	int m_nState;
	int m_nDepositID;
	int m_nServerNo;
	int m_nDepositAmount;
	CString m_strTaxBand;
	int m_nTaxAmount;
	CString m_strBooking;
	CString m_strCustName;
	CStringArray m_arrayCustInfo;
	CString m_strCustInfo;

private:
	bool m_bEndCurrentDeposit;
	bool m_bGotCurrentDeposit;
	bool m_bGotNewDeposit;
	int m_nNewDepositID;
	int m_nNewServerNo;
	int m_nNewDepositAmount;
	CString m_strNewTaxBand;
	int m_nNewTaxAmount;
};

/**********************************************************************/