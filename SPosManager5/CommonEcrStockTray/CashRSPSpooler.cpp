 /**********************************************************************/
#include "CashRSPSpooler.h"
/**********************************************************************/

CCashRSPSpooler::CCashRSPSpooler( CSSFile& File ) : m_File( File )
{
	m_bManualMode = FALSE;
	m_nManualPos = 0;
};

/**********************************************************************/

void CCashRSPSpooler::AddManualLine(CString strLine)
{
	m_arrayManual.Add(strLine);
}

/**********************************************************************/

bool CCashRSPSpooler::ReadString(CString& strLine)
{
	strLine = "";

	if (FALSE == m_bManualMode)
	{
		if (m_File.ReadString(strLine) == TRUE)
		{
			return TRUE;
		}
		else
		{
			m_File.Close();
			m_bManualMode = TRUE;
			m_nManualPos = 0;
		}
	}

	if (TRUE == m_bManualMode)
	{
		if (m_nManualPos < m_arrayManual.GetSize())
		{
			strLine = m_arrayManual.GetAt(m_nManualPos++);
			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

void CCashRSPSpooler::CloseFile()
{
	if (FALSE == m_bManualMode)
	{
		m_File.Close();
	}
}

/**********************************************************************/

