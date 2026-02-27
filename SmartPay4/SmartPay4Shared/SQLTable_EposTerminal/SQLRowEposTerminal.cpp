//**********************************************************************
#include "SQLRowEposTerminal.h"
//**********************************************************************

CSQLRowEposTerminal::CSQLRowEposTerminal()
{
	Reset();
}

//**********************************************************************

void CSQLRowEposTerminal::Reset()
{
	m_nTerminalNo = 1;
	m_nTerminalType = 0;
	m_strTerminalName = "";
	m_strLastUsedDate = "";
	m_strLastUsedTime = "";
	m_nCCNo = 0;
	m_nUserID = -1;
	m_dCardValue = 0.0;
	m_nTotalCounter = 0;
	m_dTotalInDrawer = 0.0;
	m_bDisable = FALSE;
	m_bTerminalOn = FALSE;
	m_bInService = FALSE;
	m_bDoorOpen = FALSE;
}

//**********************************************************************

void CSQLRowEposTerminal::SetLastUsedDateAndTimeToNow()
{
	COleDateTime dateNow = COleDateTime::GetCurrentTime();

	m_strLastUsedDate.Format("%2.2d/%2.2d/%4.4d",
		dateNow.GetDay(),
		dateNow.GetMonth(),
		dateNow.GetYear());

	m_strLastUsedTime.Format("%2.2d:%2.2d",
		dateNow.GetHour(),
		dateNow.GetMinute());
}

//**********************************************************************

void CSQLRowEposTerminal::SetLastUsedDate(CString strDate)
{
	if (strDate == "")
	{
		m_strLastUsedDate = "";
	}
	else if (ValidateDate(strDate) == TRUE)
	{
		if (strDate.GetLength() == 8)
		{
			strDate.Insert(2, "/");
			strDate.Insert(5, "/");
		}

		m_strLastUsedDate = strDate;
	}	
}

//**********************************************************************

void CSQLRowEposTerminal::SetLastUsedTime(CString strTime)
{
	if (strTime == "")
	{
		m_strLastUsedTime = "";
	}
	else if (ValidateTime(strTime) == TRUE)
	{
		if (strTime.GetLength() == 4)
		{
			strTime.Insert(2, ":");
		}

		m_strLastUsedTime = strTime;
	}
}

//**********************************************************************

CString CSQLRowEposTerminal::GetTerminalText()
{
	if (nRECORDTYPE_KIOSK == m_nTerminalType)
	{
		return "Kiosk";
	}
	else
	{
		return "Epos";
	}
}

//**********************************************************************

bool CSQLRowEposTerminal::IsDateSet()
{
	return ValidateDate(m_strLastUsedDate);
}

//**********************************************************************

bool CSQLRowEposTerminal::ValidateDate(CString strDate)
{
	bool bResult = FALSE;

	CString strDateToTest = strDate;

	if (strDate.GetLength() == 10)
	{
		if ((strDate.Mid(2, 1) == "/") && (strDate.Mid(5, 1) == "/"))
		{
			strDateToTest = strDate.Left(2) + strDate.Mid(3, 2) + strDate.Right(4);
		}
		else
		{
			strDateToTest = "";
		}
	}

	if (strDateToTest.GetLength() == 8)
	{
		if (::TestNumeric(strDateToTest) == TRUE)
		{
			COleDateTime dateTest = COleDateTime(
				atoi(strDateToTest.Right(4)),
				atoi(strDateToTest.Mid(2, 2)),
				atoi(strDateToTest.Left(2)),
				0, 0, 0);

			bResult = (dateTest.GetStatus() == COleDateTime::valid);
		}
	}

	return bResult;
}

//**********************************************************************

bool CSQLRowEposTerminal::ValidateTime(CString strTime)
{
	bool bResult = FALSE;

	CString strIimeToTest = strTime;

	if (strTime.GetLength() == 5)
	{
		if (strTime.Mid(2, 1) == ":")
		{
			strIimeToTest = strTime.Left(2) + strTime.Right(2);
		}
		else
		{
			strIimeToTest = "";
		}
	}

	if (strIimeToTest.GetLength() == 4)
	{
		if (::TestNumeric(strIimeToTest) == TRUE)
		{
			COleDateTime dateTest = COleDateTime(
				2025, 1, 1,
				atoi(strIimeToTest.Left(2)),
				atoi(strIimeToTest.Right(2)),
				0);

			bResult = (dateTest.GetStatus() == COleDateTime::valid);
		}
	}

	return bResult;
}

//**********************************************************************

void CSQLRowEposTerminal::SetCashRemoved(int nCCNo, double dGT, const char* szCashFilename)
{
	CString strFilename = szCashFilename;
	if (strFilename != "")
	{
		if ( CompareDoubles(m_dTotalInDrawer, 0.0, 3 ) != 0)
		{
			CSSFile file;
			if (file.Open(strFilename, "ab") == TRUE)	// SmartPay Monitor cash log
			{
				CCSV csv;
				csv.Add(GetCSVDate());
				csv.Add(GetCSVTime());
				csv.Add(m_nCCNo);						// Kiosk CCNo
				csv.Add(m_nTotalCounter);				// total visits
				csv.Add(m_dTotalInDrawer, 2);			// cash removed
				csv.Add(dGT, 2);						// GT when cash removed
				file.Write(csv);
				file.Close();
			}
		}
	}

	m_nUserID = -1;
	m_dCardValue = 0.0;
	m_nTotalCounter = 0;
	m_dTotalInDrawer = 0.0;
	m_bDoorOpen = FALSE;	
}

//**********************************************************************

CString CSQLRowEposTerminal::GetCSVDate()
{
	CString strCSVDate = "";

	if (ValidateDate(m_strLastUsedDate) == TRUE)
	{
		strCSVDate = m_strLastUsedDate.Left(2) + m_strLastUsedDate.Mid(3, 2) + m_strLastUsedDate.Right(4);
	}
	
	return strCSVDate;
}

//**********************************************************************

CString CSQLRowEposTerminal::GetCSVTime()
{
	CString strCSVTime = "";

	if (ValidateTime(m_strLastUsedTime) == TRUE)
	{
		strCSVTime = m_strLastUsedTime.Left(2) + m_strLastUsedTime.Right(2);
	}

	return strCSVTime;
}

//**********************************************************************

void CSQLRowEposTerminal::UpdateTotalInDrawer(double dValue)
{
	++m_nTotalCounter;
	m_dTotalInDrawer += dValue;
}

//********************************************************************

int CSQLRowEposTerminal::IncrementCCNo()
{
	if (++m_nCCNo > 9999)
	{
		m_nCCNo = 1;
	}

	return m_nCCNo;
}

//********************************************************************