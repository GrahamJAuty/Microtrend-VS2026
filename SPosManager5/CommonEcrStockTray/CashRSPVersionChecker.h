#pragma once
/**********************************************************************/
#include "CashRSPCSVLines.h"
/**********************************************************************/

class CCSVEposTermLine;

class CCashRSPVersionChecker
{
public:
	CCashRSPVersionChecker(void);
	~CCashRSPVersionChecker(void);

public:
	void ForceSetVersionInfo( int nAppType, int nVersionNo );

public:
	void Reset();
	void ResetTransactionVersions();
	void ResetApplicationTypes();
	void ResetCurrentLines();
	void ResetPendingTransactionInfo();
	
public:
	void ApplyPendingTransactionInfo();
	
public:
	void ProcessCashRSPVersionLine( CCSVEposTermLine& csvIn );
	void ProcessCashRSPVersionLine( CCSVEposTermLine& csvIn, CString& strBuffer );
	void ProcessAppTypeLine( CCSVEposTermLine& csvIn );
	void ProcessAppTypeLine( CCSVEposTermLine& csvIn, CString& strBuffer );

public:
	static bool CheckCashRSPVersion( int nLineVersion, int nWantVersion, bool bAllowZero );
	static bool CheckAppType( int nLineVersion, int nWantVersion, bool bAllowZero );

private:
	bool ValidateCashRSPVersion( int nVersion );
	bool ValidateAppType( int nVersion );

public:
	int GetCurrentTransactionVersionNo(){ return m_nCurrentTransactionVersionNo; }
	int GetCurrentTransactionAppType(){ return m_nCurrentTransactionAppType; }
	const char* GetCurrentTransactionVersionLine(){ return m_strCurrentTransactionVersionLine; }
	const char* GetCurrentTransactionAppLine(){ return m_strCurrentTransactionAppLine; }

public:
	void SetFailedCashRSPVersion( int n ){ m_nFailedCashRSPVersion = n; }
	void SetFailedAppType( int n ){ m_nFailedAppType = n; }

public:
	bool GetCashRSPFormatFailFlag(){ return ( m_nFailedCashRSPVersion != 0 ) || ( m_nFailedAppType != 0 ); }
	void ClearCashRSPFormatFailFlag(){ m_nFailedCashRSPVersion = 0; m_nFailedAppType = 0; }

public:
	void ShowCashRSPFormatFailMessage( bool bNewSales );

private:
	CString m_strPendingTransactionVersionLine;
	CString m_strCurrentTransactionVersionLine;
	CString m_strPendingTransactionAppLine;
	CString m_strCurrentTransactionAppLine;
	int m_nPendingTransactionVersionNo;
	int m_nCurrentTransactionVersionNo;
	int m_nPendingTransactionAppType;
	int m_nCurrentTransactionAppType;
	int m_nFailedCashRSPVersion;
	int m_nFailedAppType;
};

/**********************************************************************/
