/**********************************************************************/

CModemAnswerSetupDialog::CModemAnswerSetupDialog ( const char* szAnswerFilename, const char* szDllFilename, CWnd* pParent )
{
	m_strAnswerFilename = szAnswerFilename;
	m_strDllFilename = szDllFilename;
	m_strError = "";
}

int CModemAnswerSetupDialog::DoModal()
{
	int nReply;

	HINSTANCE hInst = ::LoadLibrary ( m_strDllFilename );
	if ( hInst != NULL )
	{
		void (*pModemAnswerSetup)(const char*);

		pModemAnswerSetup = (void(*)(const char*)) ::GetProcAddress ( hInst, "ModemAnswerSetup" );

		if ( pModemAnswerSetup != NULL )
		{
			pModemAnswerSetup ( m_strAnswerFilename );
			m_strError = "Ok";
			nReply = 0;
		}
		else
		{
			m_strError = "Dll function error";
			nReply = 2;
		}

		::FreeLibrary ( hInst );
	}
	else
	{
		m_strError = "Dll load error";
		nReply = 1;
	}

	return nReply;
}

/**********************************************************************/

CModemDialSetupDialog::CModemDialSetupDialog ( const char* szDialFilename, const char* szDllFilename, CWnd* pParent )
{
	m_strDialFilename = szDialFilename;
	m_strDllFilename = szDllFilename;
	m_strError = "";
}

int CModemDialSetupDialog::DoModal()
{
	int nReply;

	HINSTANCE hInst = ::LoadLibrary ( m_strDllFilename );
	if ( hInst != NULL )
	{
		void (*pModemDialSetup)(const char*);

		pModemDialSetup = (void(*)(const char*)) ::GetProcAddress ( hInst, "ModemDialSetup" );

		if ( pModemDialSetup != NULL )
		{
			pModemDialSetup ( m_strDialFilename );
			m_strError = "Ok";
			nReply = 0;
		}
		else
		{
			m_strError = "Dll function error";
			nReply = 2;
		}

		::FreeLibrary ( hInst );
	}
	else
	{
		m_strError = "Dll load error";
		nReply = 1;
	}

	return nReply;
}

/**********************************************************************/

CModemDial::CModemDial ( const char* szDialFilename, const char* szDllFilename, CWnd* pParent )
{
	m_strDialFilename = szDialFilename;
	m_strDllFilename = szDllFilename;
	m_strError = "";
}

bool CModemDial::Dial ( CComms* pPort, const char* szPhone, const char* szMessage )
{
	bool bReply = FALSE;

	HINSTANCE hInst = ::LoadLibrary ( m_strDllFilename );
	if ( hInst != NULL )
	{
		BOOL (*pModemDial)(CComms*,const char*,const char*,const char*,char*);

		pModemDial = (BOOL(*)(CComms*,const char*,const char*,const char*,char*)) ::GetProcAddress ( hInst, "ModemDial" );

		if ( pModemDial != NULL )
		{
			char szResponse[128];

			bReply = pModemDial ( pPort, szPhone, m_strDialFilename, szMessage, szResponse ) != 0;
			m_strError = (const char*) szResponse;
		}
		else
			m_strError = "Dll function error";

		::FreeLibrary ( hInst );
	}
	else
		m_strError = "Dll load error";

	return bReply;
}

/**********************************************************************/

bool ModemVersion ( CString& strVersion, const char* szDllFilename )
{
	bool bReply = FALSE;

	HINSTANCE hInst = ::LoadLibrary ( szDllFilename );
	if ( hInst != NULL )
	{
		char szResponse[128];

		void (*pModemVersion)( char* );

		pModemVersion = (void(*)( char* )) ::GetProcAddress ( hInst, "ModemVersion" );

		if ( pModemVersion != NULL )
		{
			pModemVersion ( szResponse );
			strVersion = (const char*) szResponse;
			bReply = TRUE;
		}
		else
			strVersion = "0.0b, modem.lib";

		::FreeLibrary ( hInst );
	}
	else
		strVersion = "0.0a, modem.lib";

	return bReply;
}

/**********************************************************************/
