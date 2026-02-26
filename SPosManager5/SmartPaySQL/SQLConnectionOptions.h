#pragma once
//*******************************************************************

class CSQLConnectionOptions
{
public:
	CSQLConnectionOptions(void);
	bool Read();
	void CopyFrom( CSQLConnectionOptions& source );

public:
	bool GetLANConnectFlag() { return m_bLANConnect; }
	CString GetLANServerName() { return m_strLANServerName; }
	CString GetPasswordPT() { return m_strPasswordPT; }
	CString GetPasswordAES() { return m_strPasswordAES; }
	int GetQueryTimeout() { return m_nQueryTimeout; }
	int GetODBCDriverVersion() { return m_nODBCDriverVersion; }
	
	CString GetODBCDriverString()
	{
		if ( ( m_nODBCDriverVersion >= 1 ) && ( m_nODBCDriverVersion <= 99 ) )
		{
			m_strODBCDriver.Format("ODBC Driver %d for SQL Server", m_nODBCDriverVersion);
		}
		else
		{
			m_strODBCDriver = "SQL Server";
		}

		return m_strODBCDriver;
	}
	
public:
	void SetLANConnectFlag( bool b ) { m_bLANConnect = b; }
	void SetLANServerName( CString str ) { m_strLANServerName = str; }
	void SetPasswordPT(CString str) { m_strPasswordPT = str; }
	void SetPasswordAES(CString str) { m_strPasswordAES = str; }
	void SetQueryTimeout(int n) { m_nQueryTimeout = n; }
	void SetODBCDriverVersion(int n)
	{
		if ((n >= 0) && ( n <= 99))
		{
			m_nODBCDriverVersion = n;
		}
	}

private:
	bool m_bLANConnect;
	CString m_strLANServerName;
	CString m_strPasswordPT;
	CString m_strPasswordAES;
	int m_nQueryTimeout;
	int m_nODBCDriverVersion;
	CString m_strODBCDriver;
	
private:
	bool m_bAlreadyRead;
};

//*******************************************************************
extern CSQLConnectionOptions SQLConnectionOptions;
//*******************************************************************
