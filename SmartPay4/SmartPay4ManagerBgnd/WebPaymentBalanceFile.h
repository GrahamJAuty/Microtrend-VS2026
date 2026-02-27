//*******************************************************************
#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CWebPaymentBalanceFile
{
public:
	CWebPaymentBalanceFile();
	bool Create(const char* szDbBalanceList);

	const char* GetError() { return m_strError; }
	int GetCount() { return m_nBalanceCount; }
	const char* GetCSVFilename() { return m_strCsvFilename; }

private:
	bool SaveCSVHeader(CSSFile* fp);
	void SaveCSVBalance(CSQLRowAccountFull& RowAccount, CSSFile* fp);

private:
	CString m_strCsvHeader;
	CString m_strCsvFilename;

	CString m_strError;
	int m_nBalanceCount;
	int m_nAccountNameFormat;							// 0=Forename Surname, 1=Account Name, 2= Surname only, 3= Surname Forename


	bool m_bIncludePurse1;
	bool m_bIncludePurse2;
	bool m_bIncludePurse3;

	int m_nBalance1Purse;
	int m_nBalance2Purse;
};

//*******************************************************************
