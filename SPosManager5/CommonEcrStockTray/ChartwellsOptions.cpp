/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "ChartwellsOptions.h"
/**********************************************************************/

CChartwellsOptions::CChartwellsOptions()
{
	m_FTPServer.Create("FTPServer", MAX_LENGTH_FTP_SERVER, "");
	m_FTPUserName.Create("FTPUserName", MAX_LENGTH_FTP_USERNAME, "");
	m_FTPPassword.Create("FTPPassword", MAX_LENGTH_CHARTWELLS_PASSWORD, "");
	m_ChartwellsSiteNo.Create("ChartwellsSiteNo", MAX_LENGTH_CHARTWELLS_SITENO, "");
	m_SmartPaySerialNo.Create("SmartPaySerialNo", GetMinSerialNo(), GetMaxSerialNo(), GetDefaultSerialNo());

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CChartwellsOptions::CopyFrom(CChartwellsOptions& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CChartwellsOptions::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadString(m_FTPServer);
	iniFile.ReadString(m_FTPUserName);
	iniFile.ReadString(m_FTPPassword);
	iniFile.ReadString(m_ChartwellsSiteNo);
	iniFile.ReadInt(m_SmartPaySerialNo);
}

/**********************************************************************/

void CChartwellsOptions::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteString(m_FTPServer);
	iniFile.WriteString(m_FTPUserName);
	iniFile.WriteString(m_FTPPassword);
	iniFile.WriteString(m_ChartwellsSiteNo);
	iniFile.WriteInt(m_SmartPaySerialNo);
}

/**********************************************************************/

void CChartwellsOptions::Reset()
{
	m_FTPServer.Reset();
	m_FTPUserName.Reset();
	m_FTPPassword.Reset();
	m_ChartwellsSiteNo.Reset();
	m_SmartPaySerialNo.Reset();
}

/**********************************************************************/

bool CChartwellsOptions::Read(bool bTemp)
{
	CString strFilename = GetFilename(bTemp);

	if (::FileExists(strFilename) == TRUE)
	{
		CEnhancedIniFile iniFile;
		if (iniFile.Read(strFilename) == FALSE)
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}

	return TRUE;
}

/**********************************************************************/

bool CChartwellsOptions::Write(bool bTemp)
{
	bool bResult = FALSE;

	CString strFilename = GetFilename(bTemp);

	if (DataManager.LockAllUsers() == TRUE)
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile(iniFile);

		bResult = iniFile.Write(strFilename);

		DataManager.UnlockAllUsers();
	}
	return bResult;
}

/**********************************************************************/

const char* CChartwellsOptions::GetFilename(bool bTemp)
{
	if (FALSE == bTemp)
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptions);
		m_strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsOptionsTemp);
		m_strFilename = FnUp.GetFilenameToUse();
	}

	return m_strFilename;
}

/**********************************************************************/

void CChartwellsOptions::SetLastExportDate(COleDateTime& date, bool bTemp)
{
	if (date.m_status == COleDateTime::valid)
	{
		CString strDate;
		strDate.Format("%4.4d%2.2d%2.2d",
			date.GetYear(),
			date.GetMonth(),
			date.GetDay());

		CString strChartwellsFolder = "";
		GetSyssetProgramPath(strChartwellsFolder);
		strChartwellsFolder += "\\Chartwells";
		::CreateSubdirectory(strChartwellsFolder);

		CSSFile fileDate;
		CString strFilename = GetDateFilename(bTemp);
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(strDate);
		}
	}
}

/**********************************************************************/

void CChartwellsOptions::SetLastExportDate( const char* sz, bool bTemp )
{
	CString str = sz;

	bool bValid = FALSE;

	if (str == "")
	{
		bValid = TRUE;
	}
	else if ((str.GetLength() == 8) && (::TestNumeric(str) == TRUE))
	{
		bValid = TRUE;
	}

	if (TRUE == bValid)
	{
		CString strChartwellsFolder = "";
		GetSyssetProgramPath(strChartwellsFolder);
		strChartwellsFolder += "\\Chartwells";
		::CreateSubdirectory(strChartwellsFolder);

		CSSFile fileDate;
		CString strFilename = GetDateFilename(bTemp);
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(str);
		}
	}
}

/**********************************************************************/

const char* CChartwellsOptions::GetLastExportDate( bool bTemp )
{
	m_strLastExportDate = "";

	CSSFile fileDate;
	CString strFilename = GetDateFilename( bTemp );
	if (fileDate.Open(strFilename, "rb") == TRUE)
	{
		CString strDate = "";
		fileDate.ReadString(strDate);

		bool bValid = FALSE;

		if (strDate == "")
		{
			bValid = TRUE;
		}
		else if ((strDate.GetLength() == 8) && (::TestNumeric(strDate) == TRUE))
		{
			bValid = TRUE;
		}

		if (TRUE == bValid)
		{
			m_strLastExportDate = strDate;
		}
	}

	return m_strLastExportDate;
}

/**********************************************************************/

bool CChartwellsOptions::GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate )
{
	COleDateTime dateLastExport;
	bool bGotLastExportDate = FALSE;

	{
		CString str = strLastExportDate;
	
		if ( ( str.GetLength() == 8 ) && ( ::TestNumeric( str ) == TRUE ) )
		{
			dateLastExport = COleDateTime( atoi( str.Left(4) ), atoi( str.Mid(4,2) ), atoi( str.Right(2) ), 0, 0, 0 );
			bGotLastExportDate = ( dateLastExport.m_status == COleDateTime::valid );
		}
	}

	if ( FALSE == bGotLastExportDate )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		dateLastExport = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
		dateLastExport -= COleDateTimeSpan( 1, 0, 0, 0 );
	}

	oleNextDayToExport = dateLastExport;
	oleNextDayToExport += COleDateTimeSpan( 1, 0, 0, 0 );
	
	oleNextExportTime = dateLastExport;
	oleNextExportTime += COleDateTimeSpan( 1, 0, 0, 0 );

	if (PosTrayOptions.GetChartwellsNextDayFlag() == TRUE)
	{
		oleNextExportTime += COleDateTimeSpan(1, 0, 0, 0);
	}

	int nMinutes = PosTrayOptions.GetChartwellsExportTime();
	if ((nMinutes < 0) || (nMinutes > 1439))
	{
		nMinutes = 0;
	}	

	oleNextExportTime += COleDateTimeSpan( 0, nMinutes / 60,  nMinutes % 60, 0 );

	return TRUE;
}

/**********************************************************************/

const char* CChartwellsOptions::GetDateFilename(bool bTemp)
{
	m_strDateFilename = "";

	if (FALSE == bTemp)
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsExportDate);
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp(SysFiles::ChartwellsExportDateTemp);
		m_strDateFilename = FnUp.GetFilenameToUse();
	}

	return m_strDateFilename;
}

/**********************************************************************/

