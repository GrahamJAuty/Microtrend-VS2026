//**********************************************************************
#include "AccountFieldValidator.h"
//**********************************************************************

CDateStringTester::CDateStringTester(CString strDate)
{
	m_strDate = strDate;
}

//**********************************************************************

bool CDateStringTester::TN(int nStart, int nCount)
{
	return TestNumeric(m_strDate.Mid(nStart, nCount));
}

//**********************************************************************

bool CDateStringTester::TX(int nPos)
{
	return (TestNumeric(m_strDate.Mid(nPos, 1)) == FALSE );
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

bool CAccountFieldValidator::ValidateStringDate(CString& strDate)
{
	CString strInputDate = strDate;

	if ((strInputDate.GetLength() != 8) && (strInputDate.GetLength() != 10))
	{
		return FALSE;
	}

	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;

	if (strInputDate.GetLength() == 8)
	{
		if (::TestNumeric(strInputDate) == FALSE)
		{
			return FALSE;
		}

		nDay = atoi(strInputDate.Left(2));
		nMonth = atoi(strInputDate.Mid(2,2));
		nYear = atoi(strInputDate.Right(4));
	}
	else
	{
		CDateStringTester DST(strInputDate);
		if (DST.TN(0,4) && DST.TX(4) && DST.TN(5,2) && DST.TX(7) && DST.TN(8,2))
		{
			nYear = atoi(strInputDate.Left(4));
			nMonth = atoi(strInputDate.Mid(5,2));
			nDay = atoi(strInputDate.Right(2));
		}
		else if (DST.TN(0,2) && DST.TX(2) && DST.TN(3,2) && DST.TX(5) && DST.TN(6,4))
		{
			nDay = atoi(strInputDate.Left(2));
			nMonth = atoi(strInputDate.Mid(3,2));
			nYear = atoi(strInputDate.Right(4));
		}
	}

	COleDateTime timeTest = COleDateTime(nYear, nMonth, nDay, 0, 0, 0);
	bool bValid = (timeTest.m_status == COleDateTime::valid);

	if (TRUE == bValid)
	{
		strDate.Format("%2.2d/%2.2d/%4.4d",
			nDay,
			nMonth,
			nYear);
	}

	return bValid;
}

//**********************************************************************

bool CAccountFieldValidator::StringToOleDate(CString strDate, COleDateTime& oleDate)
{
	bool bResult = FALSE;
	if (strDate.GetLength() == 10)
	{
		CString strTestDate = strDate.Left(2) + strDate.Mid(3, 2) + strDate.Right(4);

		if (::TestNumeric(strTestDate))
		{
			oleDate = COleDateTime(
				atoi(strTestDate.Right(4)),
				atoi(strTestDate.Mid(2, 2)),
				atoi(strTestDate.Left(2)),
				0, 0, 0);

			bResult = oleDate.m_status == COleDateTime::valid;
		}
	}

	return bResult;
}

//**********************************************************************

CString CAccountFieldValidator::OleDateToString(COleDateTime oleDate)
{
	CString strResult = "";

	if (oleDate.m_status == COleDateTime::valid)
	{
		strResult.Format("%2.2d/%2.2d/%4.4d",
			oleDate.GetDay(),
			oleDate.GetMonth(),
			oleDate.GetYear());
	}

	return strResult;
}

//**********************************************************************

bool CAccountFieldValidator::ValidateDOB(COleDateTime oleDOB)
{
	bool bResult = FALSE;

	if (oleDOB.m_status == COleDateTime::valid)
	{
		bResult = (oleDOB >= GetMinDob()) && (oleDOB <= GetMaxDob());
	}

	return bResult;
}

//**********************************************************************

bool CAccountFieldValidator::ValidateExpiry(COleDateTime oleExpiry)
{
	bool bResult = FALSE;

	if (oleExpiry.m_status == COleDateTime::valid)
	{
		bResult = (oleExpiry >= GetMinExpiry()) && (oleExpiry <= GetMaxExpiry());
	}

	return bResult;
}

//**********************************************************************
