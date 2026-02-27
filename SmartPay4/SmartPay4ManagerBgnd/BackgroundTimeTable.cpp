//*****************************************************************
#include "BackgroundTimeTable.h"
//*****************************************************************

CBackgroundTimeTable::CBackgroundTimeTable()
{
	m_strFilename = Filenames.GetBackgroundTimeTableFilename();

	for (int n = 1; n <= BTT_MAXTIMES; n++)
	{
		m_strTimeLabel[n].Format("Time%d", n);
		m_strTime[n] = "";
	}
}

//*****************************************************************

bool CBackgroundTimeTable::ReadTimetable()
{
	bool bReply = FALSE;

	CSSIniFile file;
	if ((bReply = file.Read(m_strFilename)) == TRUE)
	{
		for (int n = 1; n <= BTT_MAXTIMES; n++)
		{
			m_strTime[n] = file.GetString(m_strTimeLabel[n], "");
		}

		m_nPurchaseDay = file.GetInt("EPD", 0);			// Email Purchases Day
		m_nWeeklyPointsDay = file.GetInt("EWPD", 0);	// Email Weekly Points Day
		m_nAccountLinkDay = file.GetInt("ALD", 0);		// Account Link Day
	}
	return bReply;
}

//*****************************************************************

bool CBackgroundTimeTable::WriteTimetable()
{
	CSSIniFile file;

	for (int n = 1; n <= BTT_MAXTIMES; n++)
	{
		file.Set(m_strTimeLabel[n], m_strTime[n]);
	}

	file.Set("EPD", m_nPurchaseDay);				// Email Purchases Day
	file.Set("EWPD", m_nWeeklyPointsDay);			// Email Weekly Points Day
	file.Set("ALD", m_nAccountLinkDay);				// Account Link Day

	return file.Write(m_strFilename);
}

//*******************************************************************

void CBackgroundTimeTable::SetTime ( int n, const char* szTime )
{
	if ( ( n >= 1 ) && ( n <= BTT_MAXTIMES) )
	{
		m_strTime[n] = szTime;
	}
}

//*****************************************************************

CString CBackgroundTimeTable::GetTime ( int n )
{
	CString strReply = "";

	if ((n >= 1) && (n <= BTT_MAXTIMES))
	{
		strReply = m_strTime[n];
	}
	
	return strReply;
}

//***********************************************************
