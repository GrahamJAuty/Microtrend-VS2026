/**********************************************************************/
#include "EposReportFile.h"
/**********************************************************************/

CEposReportFile::CEposReportFile(void)
{
	m_bFirstLineOnPage = TRUE;
	m_bLockedBlankLines = FALSE;
	m_bGotReportLine = FALSE;
	m_bGotDataLine = FALSE;
	m_bCreateMD5Hash = FALSE;
	m_nBlankLineCount = 0;
}

/**********************************************************************/

CEposReportFile::~CEposReportFile()
{
}

/**********************************************************************/

void CEposReportFile::HandleNewPage()
{
	if ( FALSE == m_bFirstLineOnPage )
	{
		if (EcrmanOptions.GetReportsGeneralPageFlag() == FALSE)
		{
			RequestBlankLines(2);
		}
		else
		{
			RequestBlankLines(1);
			m_ReportFile.WriteLine( "<NEWPAGE>" );
			m_nBlankLineCount = 0;
		}
			
		m_bFirstLineOnPage = TRUE;
	}
}

/**********************************************************************/

void CEposReportFile::WriteReportLine( const char* szLine, bool bData )
{
	CString strLine = szLine;
	
	if (strLine == "")
	{
		RequestBlankLines(1);
	}
	else
	{
		m_nBlankLineCount = 0;
		m_bLockedBlankLines = FALSE;

		/*
		if (FALSE == bData)
		{
			CString strColour = "<CF#FF0000>";

			CCSV csv(strLine,'\t');
			for (int n = 0; n < csv.GetSize(); n++)
			{
				CString str = csv.GetString(n);
				csv.SetAt(n, strColour + str);
			}

			strLine = csv.GetLine();
		}
		*/

		m_ReportFile.WriteLine( strLine );
		m_bFirstLineOnPage = FALSE;
		m_bGotReportLine = TRUE;

		if (TRUE == bData)
		{
			if (TRUE == m_bCreateMD5Hash)
			{
				m_MD5Stream.Begin();
				m_MD5Stream.Process(strLine);
			}

			m_bGotDataLine = TRUE;
		}
	}
}

/**********************************************************************/

void CEposReportFile::RequestBlankLines( int nCount )
{
	if ( ( FALSE == m_bFirstLineOnPage ) && ( FALSE == m_bLockedBlankLines ) )
	{
		while( m_nBlankLineCount < nCount )
		{
			m_ReportFile.WriteLine( "" );
			m_nBlankLineCount++;
		}
	}
}

/**********************************************************************/

void CEposReportFile::LockBlankLines( int nCount )
{
	if ( FALSE == m_bFirstLineOnPage )
	{
		while( m_nBlankLineCount < nCount )
		{
			m_ReportFile.WriteLine( "" );
			m_nBlankLineCount++;
		}

		m_bLockedBlankLines = TRUE;
	}
}

/**********************************************************************/

void CEposReportFile::PreventEmptyFile()
{
	if ( FALSE == m_bGotReportLine )
	{
		m_ReportFile.WriteLine( "" );
		m_bGotReportLine = TRUE;
		m_nBlankLineCount = 1;
		m_bLockedBlankLines = FALSE;
		m_bFirstLineOnPage = FALSE;
	}
}

/**********************************************************************/
