/**********************************************************************/

CCommandLineBuilder::CCommandLineBuilder()
{
	Reset();
}

/**********************************************************************/

void CCommandLineBuilder::Reset()
{
	m_strLine = "";
}

/**********************************************************************/

void CCommandLineBuilder::AddToken( const char* szToken )
{
	if ( m_strLine != "" )
		m_strLine += " ";

	m_strLine += "\"";
	m_strLine += szToken;
	m_strLine += "\"";
}

/**********************************************************************/
