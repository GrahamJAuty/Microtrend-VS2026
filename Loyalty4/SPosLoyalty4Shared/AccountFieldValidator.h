#pragma once
//**********************************************************************

class CDateStringTester
{
public:
	CDateStringTester(CString strDate);
	bool TN(int nStart, int nLen);
	bool TX(int nPos);

private:
	CString m_strDate;
};

//**********************************************************************

class CAccountFieldValidator 
{
public:
	static bool ValidateDOB(COleDateTime oleDOB);
	static bool ValidateExpiry(COleDateTime oleExpiry);
	static bool ValidateStringDate(CString& strDate);

public:
	static bool StringToOleDate(CString strDate, COleDateTime& oleDate);
	static CString OleDateToString(COleDateTime oleDate);
	
public:
	static COleDateTime GetMinDob() { return COleDateTime(1900, 1, 1, 0, 0, 0); }
	static COleDateTime GetMaxDob() { return COleDateTime::GetCurrentTime(); }
	static COleDateTime GetMinExpiry() { return COleDateTime(1900, 1, 1, 0, 0, 0); }
	static COleDateTime GetMaxExpiry() { return COleDateTime(3000, 12, 31, 0, 0, 0); }
};

//**********************************************************************

