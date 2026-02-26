//**********************************************************************
#include "SQLFieldValidatorBase.h"
//**********************************************************************

CString CSQLFieldValidatorBase::GetSQLiteString( char* c)
{
	return c ? c : "";
}

//**********************************************************************

int CSQLFieldValidatorBase::GetSQLiteInt(char* c)
{
	return atoi(GetSQLiteString(c));
}

//**********************************************************************

__int64 CSQLFieldValidatorBase::GetSQLiteInt64(char* c)
{
	return _atoi64(GetSQLiteString(c));
}

//**********************************************************************

double CSQLFieldValidatorBase::GetSQLiteDouble( char* c)
{
	return atof(GetSQLiteString(c));
}

//**********************************************************************

bool CSQLFieldValidatorBase::ValidateString(CString str, const recString info)
{
	return ((str.GetLength() >= info.Min) && (str.GetLength() <= info.Max));
}

//**********************************************************************

bool CSQLFieldValidatorBase::ValidateDouble(double d, const recDouble info)
{
	return ((d >= info.Min) && (d <= info.Max));
}

//**********************************************************************

bool CSQLFieldValidatorBase::ValidateInt(int n, const recInt info)
{
	return ((n >= info.Min) && (n <= info.Max));
}

//**********************************************************************

bool CSQLFieldValidatorBase::ValidateInt64(__int64 n, const recInt64 info)
{
	return ((n >= info.Min) && (n <= info.Max));
}

//**********************************************************************

void CSQLFieldValidatorBase::SetString(CString str, CString& strTarget, const recString info)
{
	if (ValidateString(str, info) == TRUE)
	{
		strTarget = str;
	}
}

//**********************************************************************

void CSQLFieldValidatorBase::SetDouble(double d, double& dTarget, const recDouble info)
{
	if (ValidateDouble(d, info) == TRUE)
	{
		dTarget = d;
	}
}

//**********************************************************************

void CSQLFieldValidatorBase::SetInt(int n, int& nTarget, const recInt info)
{
	if (ValidateInt(n, info) == TRUE)
	{
		nTarget = n;
	}
}

//**********************************************************************

void CSQLFieldValidatorBase::SetInt64(__int64 n, __int64& nTarget, const recInt64 info)
{
	if (ValidateInt64(n, info) == TRUE)
	{
		nTarget = n;
	}
}

//**********************************************************************

bool CSQLFieldValidatorBase::StringToOleDate(CString strDate, COleDateTime& oleDate)
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

CString CSQLFieldValidatorBase::OleDateToString(COleDateTime oleDate)
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

bool CSQLFieldValidatorBase::ValidateDOB(COleDateTime oleDOB)
{
	bool bResult = FALSE;

	if (oleDOB.m_status == COleDateTime::valid)
	{
		bResult = (oleDOB >= GetMinDob()) && (oleDOB <= GetMaxDob());
	}

	return bResult;
}

//**********************************************************************

bool CSQLFieldValidatorBase::ValidateExpiry(COleDateTime oleExpiry)
{
	bool bResult = FALSE;

	if (oleExpiry.m_status == COleDateTime::valid)
	{
		bResult = (oleExpiry >= GetMinExpiry()) && (oleExpiry <= GetMaxExpiry());
	}

	return bResult;
}

//**********************************************************************
