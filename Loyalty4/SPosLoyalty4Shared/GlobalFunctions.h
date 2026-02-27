#pragma once
//**********************************************************************

bool FileExists ( const char* szFile );

void SendToRecycleBin ( const char* szFileMask);
void SendToRecycleBin ( CStringArray* pArray );

int GetFileLineCount ( const char* szFilename );
CString GetFileExtension ( const char* szFilename );

void TrimSpacesFromString ( CString& strText, bool bRemoveLeadingZeros = FALSE );

void StripFilename ( const char* szFullPathname, CString& strPath );
void StripPathname ( const char* szPathname, CString& strFilename );

bool AppendDataFile ( const char* szSourceFilename, const char* szDestFilename, bool bHaveHeader = FALSE );

CString SignedFormatLongValue ( long lValue );
CString SignedFormatDoubleValue ( double dValue );
CString SignedFormatIntValue(int nValue);
CString SimpleFormatIntValue(int nValue);
CString SimpleFormatInt64Value(__int64 n);

CString GetUniqueTempFilename ( const char* szFileExtension, const char* szPrefix = "TMP" );

bool IsStringNumeric ( const char* szBuf, int nMin = 0, int nMax = 0 );

bool ValidateFolderAccess ( const char* szPath );

int CountFiles ( const char* szFileMask );

bool TestNumeric ( const CString& strText, int nMaxLen = 0 );
bool TestNumericHex ( const CString& strText, int nMaxLen = 0 );

bool IsAccountInuse ( const char* szCardNo );

int CompareDoubles( double dDouble1, double dDouble2, int nDp );
double RoundDoubleValue(double dValue);

int HexToInt ( const char* szHex );

CString GetTidyAppName( bool bServer );

void GetDataProgramPath( CString& strPath );
void GetPCPathHost( CString& strPath );
void GetPCPathClient( CString& strPath );
void CreateSubdirectory ( const char* szDir );

CString ConvertISO8601ToSimpleDateTime(CString strISO, bool bDateOnly = FALSE);

CString GetGCSafeTag(CString strLabel, CString strValue);
CString GetGCSafeText(CString strOriginal);

void GetMessageLogDateTime(CString& strDate, CString& strTime);

/**********************************************************************/
#ifndef SERVERICON
/**********************************************************************/
void CKUnlock();
//*********************************************************************/
#endif
//*********************************************************************/
