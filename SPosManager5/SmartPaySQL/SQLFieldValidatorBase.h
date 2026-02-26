#pragma once
//**********************************************************************

class CSQLFieldValidatorBase 
{
public:
	static bool ValidateDOB(COleDateTime oleDOB);
	static bool ValidateExpiry(COleDateTime oleExpiry);

public:
	static bool StringToOleDate(CString strDate, COleDateTime& oleDate);
	static CString OleDateToString(COleDateTime oleDate);
	
public:
	static COleDateTime GetMinDob() { return COleDateTime(1900, 1, 1, 0, 0, 0); }
	static COleDateTime GetMaxDob() { return COleDateTime::GetCurrentTime(); }
	static COleDateTime GetMinExpiry() { return COleDateTime(1900, 1, 1, 0, 0, 0); }
	static COleDateTime GetMaxExpiry() { return COleDateTime(3000, 12, 31, 0, 0, 0); }

public:
	static CString GetSQLiteString(char* c);
	static int GetSQLiteInt(char* c);
	static __int64 GetSQLiteInt64(char* c);
	static double GetSQLiteDouble(char* c);
	
protected:
	static void SetString(CString str, CString& strTarget, const recString info);
	static void SetDouble(double d, double& dTarget, const recDouble info);
	static void SetInt(int n, int& nTarget, const recInt info);
	static void SetInt64(__int64 n, __int64& nTarget, const recInt64 info);

protected:
	static bool ValidateString(CString str, const recString info);
	static bool ValidateDouble(double d, const recDouble info);
	static bool ValidateInt(int n, const recInt info);
	static bool ValidateInt64(__int64 n, const recInt64 info);
};

//**********************************************************************

