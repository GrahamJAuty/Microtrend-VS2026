#pragma once
//*******************************************************************
#include "ConsolidationMap.h"
#include "ReportConsolidationMap.h"
//*******************************************************************
#define DEF_OBDC_DRIVER_VERSION 17
//*******************************************************************

class CSQLConnectionOptions
{
public:
	CSQLConnectionOptions(void);
	bool Read();
	bool Write();
	void CopyFrom( CSQLConnectionOptions& source );

public:
	bool GetLANConnectFlag() { return m_bLANConnect; }
	CString GetLANServerName() { return m_strLANServerName; }
	CString GetPasswordPT() { return m_strPasswordPT; }
	CString GetPasswordAES() { return m_strPasswordAES; }
	CString GetODBCDriverVersion() { return m_strODBCDriverVersion; }
	int GetQueryTimeout() { return m_nQueryTimeout; }
	int GetPortNumber() { return m_nPortNumber; }

public:
	void GetODBCDriverVersions(CReportUniqueMap<CMapKeyInt, CMapDataEmpty>& mapVersions);
	CString GetODBCDriverString(int nVer);
	
public:
	void SetLANConnectFlag( bool b ) { m_bLANConnect = b; }
	void SetLANServerName( CString str ) { m_strLANServerName = str; }
	void SetPasswordPT(CString str) { m_strPasswordPT = str; }
	void SetPasswordAES(CString str) { m_strPasswordAES = str; }
	void SetQueryTimeout(int n) { m_nQueryTimeout = n; }
	void SetODBCDriverVersion(CString str) { m_strODBCDriverVersion = str; }

	void SetPortNumber(int n)
	{
		if ((n >= 0) && (n <= 65535))
		{
			m_nPortNumber = n;
		}
	}

private:
	bool m_bLANConnect;
	CString m_strLANServerName;
	CString m_strPasswordPT;
	CString m_strPasswordAES;
	CString m_strODBCDriverVersion;
	int m_nQueryTimeout;
	int m_nPortNumber;
	
private:
	bool m_bAlreadyRead;
	CString m_strDriverConnectionString;
};

//*******************************************************************
extern CSQLConnectionOptions SQLConnectionOptions;
//*******************************************************************
