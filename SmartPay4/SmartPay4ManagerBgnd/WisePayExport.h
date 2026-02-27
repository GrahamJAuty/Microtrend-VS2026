#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
//*******************************************************************

class CWisePayExport  
{
public:
	CWisePayExport ( CWnd* pParent = NULL );

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	bool SendOpeningBalanceList ( const char* szListFilename );

//******************************************************************
#endif
//******************************************************************

	CString  GetError() { return m_strError; }

	bool SendTransactions1 ( const char* szWebCsvFile, const char* szLoginReply );
	bool SendTransactions2 ( const char* szWebCsvFile, const char* szLoginReply );

private:
	int CreateStartingBalancesFile ( int nService, const char* szListFilename, const char* szDestFilename );
	bool SaveStartingBalanceLine ( CSQLRowAccountFull& RowAccount, int nService, CSSFile* fp );
	bool LogBalanceUpdates ( int nServiceNo, const char* szNewUpdates, int nCount );

private:
	CWnd* m_pParent;
	CWisePayData m_data;
	CString m_strError;
};

//******************************************************************
