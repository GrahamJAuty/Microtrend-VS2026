//$$******************************************************************
#include "PasswordLogger.h"
//$$******************************************************************
#include "PasswordLogReport.h"
//$$******************************************************************

CPasswordLogReport::CPasswordLogReport(CString strStartDate, CString strEndDate, bool bSortByUser, bool bLogOnLogOff)
{
	m_strStartDate = strStartDate;
	m_strEndDate = strEndDate;
	m_bSortByUser = bSortByUser;
	m_bLogOnLogOff = bLogOnLogOff;
}

//$$******************************************************************

void CPasswordLogReport::CreateReport()
{
	CreateFileArray();
	ConsolidateReport();

	if (FALSE == m_bLogOnLogOff)
	{
		BuildLineReport();
	}
	else
	{
		BuildSessionReport();
	}
}

//$$******************************************************************

void CPasswordLogReport::ConsolidateReport()
{
	__int64 nLineNo = 0;

	for (int n = 0; n < m_arrayFiles.GetSize(); n++)
	{
		CString strPath = Filenames.GetUserLogFilename(m_arrayFiles.GetAt(n));

		CSSFile fileLog;
		if (fileLog.Open(strPath, "rb") == FALSE)
		{
			continue;
		}

		CString strBuffer;
		while (fileLog.ReadString(strBuffer) == TRUE)
		{
			CCSV csv(strBuffer);

			int nVersion = csv.GetInt(0);

			if (1 != nVersion)
			{
				continue;
			}

			CString strLogonDate = csv.GetString(1);
			CString strLogonTime = csv.GetString(2);
			int nPasswordNo = csv.GetInt(3);
			int nEventType = csv.GetInt(4);
			CString strUserName = csv.GetString(5);
			CString strLogoffDate = csv.GetString(6);
			CString strLogoffTime = csv.GetString(7);
			
			if ((strLogonDate.GetLength() != 8) || (strLogonTime.GetLength() != 6))
			{
				continue;
			}

			if ((strLogonDate >= m_strStartDate) && (strLogonDate <= m_strEndDate))
			{
				CSortedPasswordLine line;
				
				if (FALSE == m_bLogOnLogOff)
				{
					switch (nEventType)
					{
					case PASSWORD_EVENT_LOGOFF_MENU:
					case PASSWORD_EVENT_LOGOFF_SYSTEM:
						line.m_strDate = strLogoffDate;
						line.m_strTime = strLogoffTime;
						break;

					default:
						line.m_strDate = strLogonDate;
						line.m_strTime = strLogonTime;
						break;
					}
					line.m_nSortPasswordNo = (m_bSortByUser) ? nPasswordNo : 0;
					line.m_nLineNo = nLineNo++;
					line.m_nReportPasswordNo = nPasswordNo;
					line.m_nEventType = nEventType;
					line.m_strUserName = strUserName;
					m_arrayEvents.Consolidate(line);
				}
				else
				{
					switch (csv.GetInt(4))
					{
					case PASSWORD_EVENT_LOGOFF_MENU:
					case PASSWORD_EVENT_LOGOFF_SYSTEM:
						line.m_strDate = strLogonDate;
						line.m_strTime = strLogonTime;
						line.m_nSortPasswordNo = (m_bSortByUser) ? nPasswordNo : 0;
						line.m_nLineNo = nLineNo++;
						line.m_nReportPasswordNo = nPasswordNo;
						line.m_nEventType = 0;
						line.m_strUserName = strUserName;
						line.m_strDate2 = strLogoffDate;
						line.m_strTime2 = strLogoffTime;
						m_arrayEvents.Consolidate(line);
						break;
					}
				}
			}
		}
	}
}

//$$******************************************************************

void CPasswordLogReport::BuildLineReport()
{
	CReportFile ReportFile;
	ReportFile.Open(Filenames.GetUserLogReportFilename());
	ReportFile.SetStyle1("User Log");
	ReportFile.AddColumn("No", TA_RIGHT, 100);
	ReportFile.AddColumn("User Name", TA_LEFT, 400);
	ReportFile.AddColumn("Date", TA_LEFT, 300);
	ReportFile.AddColumn("Time", TA_LEFT, 200);
	ReportFile.AddColumn("Action", TA_LEFT, 500);

	int nCurrentPasswordNo = -1;

	for (int n = 0; n < m_arrayEvents.GetSize(); n++)
	{
		CSortedPasswordLine line;
		m_arrayEvents.GetAt(n, line);

		CString strReportDate;
		strReportDate.Format("%s/%s/%s",
			(const char*)line.m_strDate.Right(2),
			(const char*)line.m_strDate.Mid(4, 2),
			(const char*)line.m_strDate.Left(4));

		CString strReportTime;
		strReportTime.Format("%s:%s:%s",
			(const char*)line.m_strTime.Left(2),
			(const char*)line.m_strTime.Mid(2, 2),
			(const char*)line.m_strTime.Right(2));

		int nPasswordNo = line.m_nReportPasswordNo;
		if ( ( TRUE == m_bSortByUser ) && ( nPasswordNo != nCurrentPasswordNo) && ( nCurrentPasswordNo != -1 ) )
		{
			ReportFile.WriteLine("");
		}

		nCurrentPasswordNo = nPasswordNo;

		CCSV csv('\t');
		
		if (0 == line.m_nReportPasswordNo)
		{
			csv.Add("");
			csv.Add("Master");
		}
		else
		{
			csv.Add(line.m_nReportPasswordNo);
			csv.Add(line.m_strUserName);
		}

		csv.Add(strReportDate);
		csv.Add(strReportTime);

		switch (line.m_nEventType)
		{
		case PASSWORD_EVENT_LOGON_SYSTEM:
			csv.Add("Logon (system)");
			break;

		case PASSWORD_EVENT_LOGON_MENU:
			csv.Add("Logon (menu)");
			break;

		case PASSWORD_EVENT_LOGOFF_SYSTEM:
			csv.Add("Logoff (system)");
			break;

		case PASSWORD_EVENT_LOGOFF_MENU:
			csv.Add("Logoff (menu)");
			break;
		}

		ReportFile.WriteLine(csv.GetLine());
	}

	ReportFile.WriteLine("");
	ReportFile.Close();
	m_arrayFiles.RemoveAll();
	m_arrayEvents.RemoveAll();
}

//$$******************************************************************

void CPasswordLogReport::BuildSessionReport()
{
	CReportFile ReportFile;
	ReportFile.Open(Filenames.GetUserLogReportFilename());
	ReportFile.SetStyle1("User Log");
	ReportFile.AddColumn("No", TA_RIGHT, 100);
	ReportFile.AddColumn("User Name", TA_LEFT, 400);
	ReportFile.AddColumn("Logon Date", TA_LEFT, 300);
	ReportFile.AddColumn("Logon Time", TA_LEFT, 300);
	ReportFile.AddColumn("Logoff Date", TA_LEFT, 300);
	ReportFile.AddColumn("Logoff Time", TA_LEFT, 300);
	
	int nCurrentPasswordNo = -1;

	for (int n = 0; n < m_arrayEvents.GetSize(); n++)
	{
		CSortedPasswordLine line;
		m_arrayEvents.GetAt(n, line);

		CString strReportDate;
		strReportDate.Format("%s/%s/%s",
			(const char*)line.m_strDate.Right(2),
			(const char*)line.m_strDate.Mid(4, 2),
			(const char*)line.m_strDate.Left(4));

		CString strReportTime;
		strReportTime.Format("%s:%s:%s",
			(const char*)line.m_strTime.Left(2),
			(const char*)line.m_strTime.Mid(2, 2),
			(const char*)line.m_strTime.Right(2));

		CString strReportDate2;
		strReportDate2.Format("%s/%s/%s",
			(const char*)line.m_strDate2.Right(2),
			(const char*)line.m_strDate2.Mid(4, 2),
			(const char*)line.m_strDate2.Left(4));

		CString strReportTime2;
		strReportTime2.Format("%s:%s:%s",
			(const char*)line.m_strTime2.Left(2),
			(const char*)line.m_strTime2.Mid(2, 2),
			(const char*)line.m_strTime2.Right(2));

		int nPasswordNo = line.m_nReportPasswordNo;
		if ((TRUE == m_bSortByUser) && (nPasswordNo != nCurrentPasswordNo) && (nCurrentPasswordNo != -1))
		{
			ReportFile.WriteLine("");
		}

		nCurrentPasswordNo = nPasswordNo;

		CCSV csv('\t');

		if (0 == line.m_nReportPasswordNo)
		{
			csv.Add("");
			csv.Add("Master");
		}
		else
		{
			csv.Add(line.m_nReportPasswordNo);
			csv.Add(line.m_strUserName);
		}

		csv.Add(strReportDate);
		csv.Add(strReportTime);
		csv.Add(strReportDate2);
		csv.Add(strReportTime2);

		ReportFile.WriteLine(csv.GetLine());
	}

	ReportFile.WriteLine("");
	ReportFile.Close();
	m_arrayFiles.RemoveAll();
	m_arrayEvents.RemoveAll();
}

//$$******************************************************************

void CPasswordLogReport::CreateFileArray()
{
	m_arrayFiles.RemoveAll();

	CString strStartMonth = m_strStartDate.Left(6);
	CString strEndMonth = m_strEndDate.Left(6);

	CFileFind FileFinder;
	CString strMask = Filenames.GetUserLogFilename("*.log");
	BOOL bWorking = FileFinder.FindFile(strMask);

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
			continue;

		CString strFilename = FileFinder.GetFileName();
		if (strFilename.GetLength() == 10)
		{
			CString strMonth = strFilename.Left(6);
			if (::TestNumeric(strMonth) == TRUE)
			{
				if ((strMonth >= strStartMonth) && (strMonth <= strEndMonth))
				{
					m_arrayFiles.Add(strFilename);
				}
			}
		}
	}
}

//$$******************************************************************
