#pragma once
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/
#define WALLETSTAMP_VERSION_STAMPOFFER 3
#define WALLETSTAMP_VERSION_PURCHASECONTROL 1
/**********************************************************************/

namespace WalletStampRule
{
	const recInt RuleID = { "RuleID", 1, 10 };
	const recInt Edition = { "Edition", 1, 0x7FFFFFFF };
	const recInt Action = { "Action", 1, 3 };
	const recInt OfferGroupNo = { "OfferGroupNo", 0, 999 };
	const recInt Expiry = { "Expiry", 0, 999 };
	const recInt Threshold = { "Threshold", 1, 999999 };
	const recString Description = { "Description", 0, 50 };
}

/**********************************************************************/

class CWalletStampRuleCSVRecord : public CCSVRecord
{
public:
	CWalletStampRuleCSVRecord(); 
	virtual ~CWalletStampRuleCSVRecord() {}

public:
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetRuleID() { return m_nRuleID; }
	int GetEdition() { return m_nEdition; }
	CString GetDescription() { return m_strDescription; }
	bool GetEnableFlag() { return m_bEnable;  }
	int GetAction() { return m_nAction; }	
	int GetOfferGroupNo() { return m_nOfferGroupNo; }
	int GetExpiry() { return m_nExpiry; }
	int GetThreshold() { return m_nThreshold; }
	
	const char* GetDisplayName();

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& Version );
	
public:
	void SetRuleID(int n);
	void SetEdition(int n);
	void SetDescription(const char* sz);
	void SetEnableFlag(bool b) { m_bEnable = b; }
	void SetAction(int n);
	void SetOfferGroupNo( int n );
	void SetExpiry(int n);
	void SetThreshold(int n);
	
private:
	void ClearRecord();
	void V1To3ConvertFromCSV_StampOffer(CCSV& csv, int nVer);
	void V1ConvertFromCSV_PurchaseControl(CCSV& csv);

private:
	int m_nRuleID;
	int m_nEdition;
	CString m_strDescription;
	bool m_bEnable;
	int m_nAction;
	int m_nOfferGroupNo;
	int m_nExpiry;
	int m_nThreshold;

private:
	CString m_strDisplayName;
};

/**********************************************************************/

class CWalletStampRuleCSVArray : public CSharedCSVArray
{
public:
	CWalletStampRuleCSVArray();
	~CWalletStampRuleCSVArray();
	
public:
	int Open( const char* szFilename, int nMode );

public:
	int GetRuleIDByIndex( int nIdx );
	bool FindRuleByID( int nRuleID, int& nIdx );
	int FindFirstFreeRuleID();

public:
	const char* GetTabbedComboText( int nRuleID );
	const char* GetSimpleListText( int nRuleID );

private:
	void GetComboTexts( int nRuleID, CString& strNum, CString& strName );
	void CreatePlaceholderRule(int nID, CWalletStampRuleCSVRecord& Offer);
	void ValidateArray();

private:
	CString m_strDisplayName;
	CString m_strTabbedComboText;
	CString m_strSimpleListText;
};

/**********************************************************************/
