#pragma once
//*******************************************************************

//*******************************************************************
#ifndef SYSTEMTYPE_BACKGOUND
//*******************************************************************
#ifndef SYSTEMTYPE_SERVER
//*******************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*******************************************************************

void DeleteWithConfirmation ( const char* szMask, const char* szTitle, CWnd* pParent );

bool ValidateFolderAccess ( const char* szPath );

CString GetTidyAppName( bool bIsServer );

void GetDataProgramPath( CString& strPath );

INT_PTR DoFileDialog(CFileDialog& dlg);

bool DoBrowseFolder(CString& strPathname, const char* szText, const char* szStartFolder, CWnd* pWnd);

int DoMessageBox(CWnd* pParent, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);

int DoAfxMessageBox(LPCTSTR lpszText, UINT nType);

//*******************************************************************
#endif
//*******************************************************************
#endif
//*******************************************************************
#endif
//*******************************************************************

//********************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//********************************************************************
#ifndef SMP_TRAYICON
//********************************************************************

int Compare8601Times(const CString strTime1, const CString strTime2);
void Convert8601TimeToUKTime(const CString strTime1, CString& strTime2);

//********************************************************************
#endif
//********************************************************************
#endif
//********************************************************************

//*******************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*******************************************************************

bool IsAccountInuse(const char* szCardNo);

bool RemoveFileLines(const char* szSourceFilename, int nLines, bool bHaveHeader, const char* szDestFilename = "");

bool CheckDoubleMatch(double dValue1, double dValue2);

//*******************************************************************
#endif
//*******************************************************************

template <class T>
T FastMax(const T& left, const T& right)
{
	return left > right ? left : right;
}

//*******************************************************************

template <class T>
T FastMin(const T& left, const T& right)
{
	return left < right ? left : right;
}

//*******************************************************************

void CKUnlock();

int HexToInt(const char* szHex);
bool TestNumeric ( const CString& strText, int nMaxLen = 0 );
bool TestNumericHex ( const CString& strText, int nMaxLen = 0 );

bool FileExists ( const char* szFile );
CString OpenFileFailureMessage ( const char* szFilename, int nMode = DB_READWRITE );

void SendToRecycleBin ( CStringArray* pArray );

CString FormatGroupNo ( int nGroupNo );

CString FormatDoubleValue ( double dValue );
CString FormatIntValue ( int nValue );
CString FormatInt64Value(__int64 nValue);
CString FormatQty ( double dQtyValue );

double RoundDoubleValue ( double dValue );

int GetFileLineCount ( const char* szFilename );

CString StripFilename ( const char* szFullPathname );
void StripFilename ( const char* szFullPathname, CString& strPath );

CString GetFileExtension ( const char* szFilename );
CString StripFileExtension ( const char* szFilename );
CString StripPathname ( const char* szPathname );

bool AppendDataFile ( const char* szSourceFilename, const char* szDestFilename, bool bHaveHeader = FALSE );

int GetMonthNo ( const char* szMonth );
CString ExtractDate ( const char* szDateString );
bool IsStringNumeric ( const char* szBuf, int nMin = 0, int nMax = 0 );

void TrimSpacesFromString ( CString& strText, bool bRemoveLeadingZeros = FALSE );
CString RemoveSpaces ( const char* szText );

CString FormatXmlDate ( CSSDate* pDate, CSSTime* pTime = NULL );
CString FormatXmlDate ( const char* szDate, const char* szTime = "" );

CString GetUniqueTempFilename ( const char* szFileExtension, const char* szPrefix = "TMP" );

int CountFiles ( const char* szFileMask );
int CopyFileLines ( const char* szSourceFilename,  const char* szDestFilename, int nMaxLines, bool bHaveHeader );

CString CompileName ( const char* szName1, const char* szName2, char cSeparator = ' ' );

bool LogError ( const char* szError, const char* szFilename );
bool LogSysFileError ( const char* szError, const char* szFilename, const char* szPrefixText );

void CreateSubdirectory ( const char* szDir );
void GetPCPathHost( CString& strPath );
void GetPCPathClient(CString& strPath);
void GetSyssetProgramPath( CString& strPath );

int Rename_Trap(CString strSource, CString strDest);

int CompareDoubles(double dDouble1, double dDouble2, int nDp);
bool CompareNoCase( CString str1, CString str2 );

CString SimpleFormatIntValue(int nValue);
CString SimpleFormatInt64Value(__int64 n);

CString GetSimpleDateTimeNowString();
CString ConvertISO8601ToSimpleDateTime(CString strISO);
CString ConvertSimpleDateTimeToISO8601(CString strSimple);
CString ConvertAuditDateTimeToISO8601(CString strDate, CString strTime);
void ConvertOleDateTimeTo8601Time(const COleDateTime oleTime, CString& strTime);
void AdjustAuditDate(CString& strDate, CString& strTime, int nMinutes);

CString GetGCSafeTag(CString strLabel, CString strValue);
CString GetGCSafeText(CString strOriginal);

void WriteBackgroundBusyFile();

void GetMessageLogDateTime(CString& strDate, CString& strTime);