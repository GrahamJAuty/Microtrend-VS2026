/**********************************************************************/

CSSRegister::CSSRegister()
{
	m_strRegFilename = "";
	m_strLogFilename = "";
	m_strEcrsetFilename = "";

	CSSIniFile ini;
	if ( ini.Read ( "register.ini" ) == TRUE )
	{
		m_strRegFilename = ini.GetString ( "RegFilename", m_strRegFilename );
		m_strLogFilename = ini.GetString ( "LogFilename", m_strLogFilename );
		m_strEcrsetFilename = ini.GetString ( "EcrsetFilename", m_strEcrsetFilename );
	}
}

/**********************************************************************/

