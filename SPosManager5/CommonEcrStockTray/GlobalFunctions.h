#pragma once
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
 /**********************************************************************/
#define IMAGE_TYPE_NONE 0
#define IMAGE_TYPE_BMP 1
#define IMAGE_TYPE_GIF 2
#define IMAGE_TYPE_JPG 3
/**********************************************************************/

struct CSQLPluNoInfo
{
public:
	CSQLPluNoInfo()
	{
		m_nEposPluNo = 0;
		m_nBasePluNo = 0;
		m_nImageFileNo = 0;
		m_nImageFileNoLen = 0;
		m_nModifier = 0;
		m_bModifiable = FALSE;
		m_bValid = FALSE;
	};

public:
	__int64 m_nEposPluNo;
	__int64 m_nBasePluNo;
	__int64 m_nImageFileNo;
	int m_nImageFileNoLen;
	int m_nModifier;
	bool m_bModifiable;
	bool m_bValid;
};

/**********************************************************************/

bool FileExists ( const char* szFile );

void CreateSubdirectory ( const char* szDir );

void CreateDataSubdirectory ( const char* szDir );

void CreateSyssetSubdirectory ( const char* szDir );

void GetDataProgramPath( CString& strPath );

void GetSyssetProgramPath( CString& strPath );

void TrimSpaces ( CString& string, bool bRemoveLeadingZeros );

bool TestNumeric ( const CString& strText, int nMaxLen = 0 );

bool TestNumericHex ( const CString& strText, int nMaxLen = 0 );

bool GetOleDateFromString( CString& strDate, COleDateTime& date );

bool GetOleDateTimeFromString( CString& strDateTime, COleDateTime& dateTime );

bool GetStringFromOleDate( COleDateTime& date, CString& strDate );

bool GetStringFromOleDateTime( COleDateTime& date, CString& strDate );

void GetReportStringFromOleDate( const COleDateTime date, CString& strDate );

void GetTempPath( CString& strPath );

void GetPCPathHost( CString& strPath );

void GetPCPathClient(CString& strPath);

bool IsBeforeStartOfDay( COleDateTime& time );

CString FilterEposText ( const char* szText );

void GetTermSaleFilename( int nCashRSPMode, int nTerminalNo, CString& strFilename );

CString GetLastRasCommsErrorMessage();

int HexToInt ( const char* szHex );
__int64 HexToInt64 ( const char* szHex );

void FormatPluNo( __int64 nPluNo, CString& strPluNo, bool bNoEan = FALSE, bool bAllowZero = FALSE );

void FormatBarcodeNo( __int64 nBarcodeNo, CString& strBarcodeNo );

void ProcessPluNo( CSQLPluNoInfo& info, bool bFiddleLongPluNo = FALSE, bool bFiddleShortPluNo = FALSE );

void FormatAuditReference( CString& strRef1, CString& strRef2, CString& strRef3 );

void MicrotrendDebugMessageBox( CString& str );

void GetPMSRoomText( int nRoomNo, const char* szRoomName, CString& strRoomText, bool bWithNumber = FALSE );

void FixAmpersandsForWindowsText( CString& strText );

bool FindFirstPluLine( CSSFile& fileSales, CString& strBuffer, int& nLinesToRead );

bool FindFirstTranLine( CSSFile& fileSales, CString& strBuffer, int& nLinesToRead );

bool FindFirstTermLine( CSSFile& fileSales, CString& strBuffer, int& nLinesToRead );

void StockMovementError( int nType, bool bPlu );

void SetHexStringFlag( CString& strHex, int nBit, bool bFlag );
bool GetHexStringFlag( CString& strHex, int nBit );

#ifndef POSTRAY_UTILITY
	void ShowPluFilterDlg( CPluFilterArray& filter, CWnd* pParent, int& nDlgResult, int& nPluIdx );
	void AddPMSPluItem( int nItem, const char* szEcrText, const char* szRepText, int nDeptNo );
	void CopyGlobalData( int nNodeType );
	void CopyServerData( int nSourceNodeType );
#endif

int GetImageType( const char* szFile, CString& strImageData, bool bFullRead );

void GetSimpleHexTimeStamp( CString& strStamp );

void TokeniseSpacedLine( CString& strText, CStringArray& arrayTokens );

void GetStockLevelString ( double dQty, int nSubUnits, CString& strQty );

void ShowLocationError();
void ShowStockpointError();

void MakeYearFirstDate( CString& strDate );

void HTMLEuroConvert( CString& strLine );

void GetMixMatchOfferTypeName( int nType, CString& strType, bool bFullLine, const char* szUnknown );

bool ChangeFileExtension( CString& strInput, const char* szNewExtension, CString& strOutput );

bool CompareFiles( const char* szFilename1, const char* szFilename2 );

void GetOleDateSince2011( int nDayNumber, COleDateTime& date );
int GetDayNumberSince2011( COleDateTime& date );

void DrawTextInDeviceContext( CDC* pDC, CStringArray& Texts, CRect rect, UINT nHAlign, int nFontSize, COLORREF crText );

bool ValidateFolderAccess(const char* szPath);

CString GetUniqueTempFilename(const char* szFileExtension, const char* szPrefix = "TMP");

int CountFiles(const char* szFileMask);

void SendToRecycleBin(const char* szFileMask);
void SendToRecycleBin(CStringArray* pArray);

void CKUnlock();

/**********************************************************************/
