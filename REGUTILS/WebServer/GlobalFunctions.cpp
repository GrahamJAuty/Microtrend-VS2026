/**********************************************************************/
#include "GlobalFunctions.h"
/**********************************************************************/

void LogMessage(const CString strMessage)
{
	CSSFile fileLog;
	if (fileLog.Open(".\\WebServiceLog.txt", "ab") == TRUE)
	{
		COleDateTime dt = COleDateTime::GetCurrentTime();

		CString strLogLine = "";
		strLogLine.Format("%04d-%02d-%02d %02d:%02d:%02d : %s\n",
			dt.GetYear(),
			dt.GetMonth(),
			dt.GetDay(),
			dt.GetHour(),
			dt.GetMinute(),
			dt.GetSecond(),
			(const char*)strMessage);

		fileLog.WriteLine(strLogLine);
	}
}

/**********************************************************************/
