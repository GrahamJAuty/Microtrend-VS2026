//*******************************************************************
//*** NOT exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************
#include "GlobalFunctions.h"
//*******************************************************************
#include "ReportManager.h"
//*******************************************************************
extern CSysset Sysset;
//*******************************************************************

void ReportManager(CWnd* pParent, const char* szReport, const char* szParams, const char* szParamsKey, bool bPrint)
{
	CString strProgName = "repman.pgm";
	if (::FileExists(strProgName) == TRUE)
	{
		if (::FileExists(szReport) == TRUE)
		{
			CString strKey = szParamsKey;
			if (strKey.GetLength() == 0)
			{
				strKey = "\"\"";							// no parameter key passed so pass blank to repman
			}

			CString strUserPath = Sysset.GetPCPathClient();
			strUserPath += "\\";							// needs trialing '\'

			if (TRUE == bPrint)
			{
				SSspawnl(pParent, P_WAIT, strProgName, strProgName, szReport, szParams, strKey, strUserPath, "/p", NULL);
			}
			else
			{
				SSspawnl(pParent, P_WAIT, strProgName, strProgName, szReport, szParams, strKey, strUserPath, NULL);
			}
		}
		else
		{
			::DisplayMessage(pParent, "No source report data found", "Report Manager");
		}
	}
	else
	{
		::DisplayMessage(pParent, "Report Manager module has not been installed", "Access Denied");
	}
}

//*******************************************************************

CString FormatDoubleValue ( double dValue )
{
	CString strBuf = "";

	if (dValue != 0.0)
	{
		strBuf.Format("%+.2lf", dValue);
	}
	else
	{
		strBuf = "0.00";
	}

	return strBuf;
}

//*******************************************************************
