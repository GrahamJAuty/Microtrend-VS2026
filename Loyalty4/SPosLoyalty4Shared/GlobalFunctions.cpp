/******************************************************************************/
#include <io.h>
/******************************************************************************/
#ifndef SERVERICON
/******************************************************************************/
#include <ckglobal.h>
/******************************************************************************/
#endif
/******************************************************************************/
#include <chrono>
#include <iomanip>
#include <sstream>
/******************************************************************************/
#include "GlobalFunctions.h"
/******************************************************************************/

bool FileExists  ( const char* szFile ) { return ( _access ( szFile, 0 ) == 0 ) ? TRUE : FALSE; }

/******************************************************************************/

void SendToRecycleBin ( CStringArray* pArray )
{
	for (int i = 0; i < pArray->GetSize(); i++)
	{
		remove(pArray->GetAt(i));
	}
}

/******************************************************************************/

void SendToRecycleBin ( const char* szFileMask )
{
	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( szFileMask );

	while (bWorking)   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		remove ( FileFinder.GetFilePath() );
	}
}

//*******************************************************************
//*** append data file ( DO NOT USE FOR AUDIT FILES )****************
//***                  ( Use AuditFile::AppendAuditFile function )  *
//*******************************************************************

bool AppendDataFile ( const char* szSourceFilename, const char* szDestFilename, bool bHaveHeader )
{
	CString strBuf;
	CSSFile fileOut;

	if ( bHaveHeader == TRUE )								// if using headers - check destinatioon file has one
	{
		bHaveHeader = FALSE;									// assume not
		if ( fileOut.Open ( szDestFilename, "rb" ) == TRUE )	// file may not exist
			bHaveHeader = fileOut.ReadLine(strBuf);				// try & read header line

		fileOut.Close();
	}

	CSSFile fileIn;
	if ( fileIn.Open ( szSourceFilename, "rb" ) == FALSE )
		return FALSE;

	if ( fileOut.Open ( szDestFilename, "ab" ) == FALSE )
	{
		fileIn.Close();
		return FALSE;
	}

	bool bReply = TRUE;

	if ( bHaveHeader == TRUE )
		fileIn.ReadLine(strBuf);						// skip header line

	while ( fileIn.ReadLine ( strBuf ) == TRUE )
		bReply = fileOut.WriteString ( strBuf );			// line already has cr/lf

	fileIn.Close();
	fileOut.Close();

	return bReply;
}

//*******************************************************************

CString SignedFormatLongValue ( long lValue )
{
	CString strBuf = "";

	if (lValue != 0L)
	{
		strBuf.Format("%+d", lValue);
	}
	else
	{
		strBuf = "0";
	}

	return strBuf;
}

//*******************************************************************

CString SignedFormatIntValue(int nValue)
{
	CString strBuf = "";

	if (nValue != 0L)
	{
		strBuf.Format("%+d", nValue);
	}
	else
	{
		strBuf = "0";
	}

	return strBuf;
}

//*******************************************************************

CString SimpleFormatIntValue(int nValue)
{
	CString strBuf = "";
	strBuf.Format("%d", nValue);
	return strBuf;
}

//*******************************************************************

CString SimpleFormatInt64Value(__int64 n)
{
	CString strResult = "";
	strResult.Format("%I64d", n);
	return strResult;
}

/**********************************************************************/

CString SignedFormatDoubleValue ( double dValue )
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

void TrimSpacesFromString( CString& strText, bool bRemoveLeadingZeros )
{
	strText.TrimLeft ( ' ' );
	strText.TrimRight ( ' ' );
	
	if ( TRUE == bRemoveLeadingZeros ) 
	{
		strText.TrimLeft ( '0' );
	}
}

//*******************************************************************
// szpathname = "c:\path1\path2\fred.dat, strFilename = fred.dat

CString GetFileExtension(const char* szFilename)
{
	CString strExtension = "";
	CString strFilename = szFilename;

	for (int i = strFilename.GetLength() - 1; i >= 0; i--)
	{
		unsigned char uc = strFilename.GetAt(i);

		if (uc == '.')
		{
			break;
		}

		strExtension += uc;
	}

	strExtension.MakeReverse();
	return strExtension;
}

//*******************************************************************
// szpathname = "c:\path1\path2\fred.dat, strFilename = fred.dat

void StripPathname ( const char* szPathname, CString& strFilename )
{
	unsigned char uc;
	CString strPathname = szPathname;

	for ( int i = strPathname.GetLength() - 1 ; i >= 0 ; i-- )
	{
		uc = strPathname.GetAt(i);

		if ( uc == '//' )	break;
		if ( uc == ':' )	break;
		if ( uc == '\\' )	break;

		strFilename += uc;
	}

	strFilename.MakeReverse();
}

//*******************************************************************
// szfullpath = "c:\path1\path2\fred.dat, strpath = c:\path1\path2

void StripFilename ( const char* szFullPathname, CString& strPath )
{
	strPath = "";

	CString strFullPathname = szFullPathname;
	int nFullPathLength = strFullPathname.GetLength();

	if (nFullPathLength == 0)							// no pathname
	{
		return;
	}

	CString strFilename;
	StripPathname ( strFullPathname, strFilename );		// strip trailing filename from path

	int nFilenameLength = strFilename.GetLength();		// make sure have a filename
	if (nFilenameLength == 0)
	{
		return;
	}

	if (nFullPathLength > nFilenameLength + 1)		// lose filename from rest of pathname
	{
		strPath = strFullPathname.Left(nFullPathLength - (nFilenameLength + 1));
	}
}

//*******************************************************************

bool IsStringNumeric ( const char* szBuf, int nMin, int nMax )
{
	CString strBuf = szBuf;
	int nLength = strBuf.GetLength();

	if (nLength < nMin)
	{
		return FALSE;
	}

	if (nMax > 0 && nLength > nMax)
	{
		return FALSE;
	}

	bool bReply = TRUE;
	for ( int i = 0 ; i < nLength ; i++ )
	{
		unsigned char uc = strBuf.GetAt(i);

		if ( uc < '0' || uc > '9' )
		{
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*******************************************************************

bool ValidateFolderAccess ( const char* szPath )
{
	bool bReply = FALSE;
	CString strFilename = "";
	strFilename.Format ( "%s\\%s", 
		(const char*) szPath, 
		(const char*) GetUniqueTempFilename("$$$") );			// cretae a temp file

	CSSFile file;
	if ( ( bReply = file.Open ( strFilename, "wb" ) ) == TRUE )
	{
		file.Close();
		SendToRecycleBin ( strFilename );
	}
	return bReply;
}

//*******************************************************************

CString GetUniqueTempFilename(const char* szFileExtension, const char* szPrefix)
{
	CString strFilename = "";

	CTime t = CTime::GetCurrentTime();
	strFilename.Format("%s-%d%2.2d%2.2d%2.2d%2.2d%2.2d.%s", szPrefix, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), szFileExtension);

	return strFilename;
}

//*******************************************************************

int CountFiles ( const char* szFileMask )
{
	int nCount = 0;

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( szFileMask );
	while (bWorking)   
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		++nCount;
	}

	return nCount;
}

//*******************************************************************

bool TestNumeric ( const CString& strText, int nMaxLen )
{
	if ((nMaxLen > 0) && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for ( int nPos = 0; nPos < strText.GetLength(); nPos++ )
	{
		char c = strText.GetAt( nPos );
		if (c < '0' || c > '9')
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

bool TestNumericHex ( const CString& strText, int nMaxLen )
{
	if (nMaxLen > 0 && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for ( int nPos = 0; nPos < strText.GetLength(); nPos++ )
	{
		char c = strText.GetAt(nPos);

		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

bool IsAccountInuse ( const char* szCardNo )
{
	bool bReply = FALSE;
	CString strFilename = Filenames.GetRecordInuseFilename ( szCardNo );

	int nRetries = 1;
	CSSFile file;
	if (file.Open(strFilename, "wb", _SH_DENYRW, nRetries) == FALSE)
	{
		bReply = TRUE;
	}
	else
	{
		file.Close();
		::SendToRecycleBin ( strFilename );
	}

	return bReply;
}

/**********************************************************************/

int CompareDoubles( double dDouble1, double dDouble2, int nDp )
{
	CString strDouble1, strDouble2;
	strDouble1.Format ( "%.*f", nDp, dDouble1 );
	strDouble2.Format ( "%.*f", nDp, dDouble2 );

	if (strDouble1 == strDouble2)
	{
		return 0;
	}

	if ( ( strDouble1.Left(1) == "-" ) != ( strDouble2.Left(1) == "-" ) )
	{
		CString strNegativeZero = "-0";

		if ( nDp != 0 )
		{
			strNegativeZero += ".";
			::AddTrailing( strNegativeZero, nDp + 3, '0' );
		}

		if (strDouble1 == strNegativeZero)
		{
			strDouble1 = strDouble1.Right(strDouble1.GetLength() - 1);
		}

		if (strDouble2 == strNegativeZero)
		{
			strDouble2 = strDouble2.Right(strDouble2.GetLength() - 1);
		}
	}

	if (strDouble1 == strDouble2)
	{
		return 0;
	}
	else if (dDouble1 < dDouble2)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/**********************************************************************/

double RoundDoubleValue(double dValue)
{
	CString strValue = "";
	strValue.Format("%.2lf", dValue);			// lose rounding errors on value
	return atof((const char*)strValue);
}

//********************************************************************

int HexToInt ( const char* szHex )
{
	int nResult = 0;

	CString strHex = szHex;
	TrimSpacesFromString( strHex, TRUE );

	for ( int nPos = 0; nPos < strHex.GetLength(); nPos++ )
	{
		nResult *= 16;

		char c = strHex.GetAt ( nPos );

		if (c >= '0' && c <= '9')
		{
			nResult += c - '0';
		}
		else if (c >= 'A' && c <= 'F')
		{
			nResult += (c + 10 - 'A');
		}
		else if (c >= 'a' && c <= 'f')
		{
			nResult += (c + 10 - 'a');
		}
	}
	return nResult;
}

/**********************************************************************/

CString GetTidyAppName( bool bServer )
{
	CString strAppName = bServer ? "SPos Loyalty Server" : "SPos Loyalty Manager";
	return strAppName;
}

/**********************************************************************/

void GetDataProgramPath( CString& strPath )
{
	CString strTemp = Sysset.GetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathHost( CString& strPath )
{
	CString strTemp = Sysset.GetPCPathHost();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathClient(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathClient();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void CreateSubdirectory ( const char* szDir )
{
	CString strDir = szDir;
	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/

CString ConvertISO8601ToSimpleDateTime(CString strISO, bool bDateOnly)
{
	CString strDateTime = "";

	if (TRUE == bDateOnly)
	{
		if (strISO.GetLength() == 10)
		{
			strISO += "T00:00:00"; 
		}
	}

	if (strISO.GetLength() < 19)
	{
		return "";
	}

	strDateTime = strISO.Left(4);
	if (::TestNumeric(strDateTime) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(4, 1) != "-")
	{
		return "";
	}

	strDateTime += strISO.Mid(5, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(7, 1) != "-")
	{
		return "";
	}

	strDateTime += strISO.Mid(8, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(10, 1) != "T")
	{
		return "";
	}

	strDateTime += strISO.Mid(11, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(13, 1) != ":")
	{
		return "";
	}

	strDateTime += strISO.Mid(14, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(16, 1) != ":")
	{
		return "";
	}

	strDateTime += strISO.Mid(17, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	COleDateTime timeTest = COleDateTime(
		atoi(strDateTime.Left(4)),
		atoi(strDateTime.Mid(4, 2)),
		atoi(strDateTime.Mid(6, 2)),
		atoi(strDateTime.Mid(8, 2)),
		atoi(strDateTime.Mid(10, 2)),
		atoi(strDateTime.Right(2)));

	if (timeTest.m_status != COleDateTime::valid)
	{
		return "";
	}
	else
	{
		return strDateTime;
	}
}

/**********************************************************************/

CString GetGCSafeTag(CString strLabel, CString strValue)
{
	CString strResult = "";
	strResult += strLabel;
	strResult += "=";
	strResult += GetGCSafeText(strValue);
	return strResult;
}

/**********************************************************************/

CString GetGCSafeText(CString strOriginal)
{
	CString strResult = strOriginal;
	strResult.Replace(',', ' ');
	strResult.Replace('"', ' ');
	return strResult;
}

/**********************************************************************/

void GetMessageLogDateTime(CString& strDate, CString& strTime)
{
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &t);

	// Format date as DD/MM/YYYY
	std::ostringstream ossDate;
	ossDate << std::setw(2) << std::setfill('0') << tm.tm_mday << "/"
		<< std::setw(2) << std::setfill('0') << (tm.tm_mon + 1) << "/"
		<< std::setw(4) << std::setfill('0') << (tm.tm_year + 1900);
	strDate = CString(ossDate.str().c_str());

	// Format time as HH:MM:SS.mmm
	std::ostringstream ossTime;
	ossTime << std::setw(2) << std::setfill('0') << tm.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << tm.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << tm.tm_sec << "."
		<< std::setw(3) << std::setfill('0') << ms.count();
	strTime = CString(ossTime.str().c_str());
}

/**********************************************************************/
#ifndef SERVERICON
/**********************************************************************/

void CKUnlock()
{
	CkGlobal glob;
	bool success = glob.UnlockBundle("SPOSUK.CB1082028_DzA3mERU2EoF");
	if (success != true) {
		AfxMessageBox(glob.lastErrorText());
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2)
	{
		//UNLOCKED WITH PURCHASED CODE;
	}
	else
	{
		//UNLOCKED IN TRIAL MODE;
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
