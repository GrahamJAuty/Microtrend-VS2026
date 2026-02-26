#pragma once
/**********************************************************************/

class CSPOSKeyboardLayoutOptions 
{
public:
	CSPOSKeyboardLayoutOptions();
	void Reset();

public:
	bool Read();
	void ReadV1To2( CCSV& csv, int nVer );
	void ReadV3( CCSV& csv );
	bool Write();
	const char* GetCSVLine();

	const char* GetDataPath(){ return m_strDataPath; }
	const char* GetSQLInFilename(){ return m_strSQLInFilename; }
	const char* GetSQLOutFilename(){ return m_strSQLOutFilename; }
	const char* GetImageFolder() { return m_strImageFolder; }
	const char* GetReportFolder() { return m_strReportFolder; }
	const char* GetReportSettingsFolder() { return m_strReportSettingsFolder; }
	
	int GetXMarginDisplay() { return m_nXMarginDisplay; }
	int GetSharpPosResolutionType(){ return m_nSharpPosResolutionType; }
	bool GetShowPluNoFlag() { return m_bShowPluNo; }

	int GetMaxBasePluLen() { return m_nMaxBasePluLen; }
	int GetPluTextOrder() { return m_nPluTextOrder; }

	int GetKeyListHeight(){ return m_nKeyListHeight; }
	int GetLastKeyType() { return m_nLastKeyType; }
	__int64 GetLastKeyValue() { return m_nLastKeyValue; }
	int GetLastFgndId() { return m_nLastFgndId; }
	int GetLastBgndId() { return m_nLastBgndId; }
	int GetLastFontSize() { return m_nLastFontSize; }
	int GetKeyListWidth() { return m_nKeyListWidth; }
	bool GetDefaultBoldFlag() { return m_bDefaultBold; }
	bool GetEditKeyTopLeftFlag() { return m_bEditKeyTopLeft; }
	bool GetImageWithStyleFlag() { return m_bImageWithStyle; }
	int GetInitialViewMode() { return m_nInitialViewMode; }
	int GetInitialZoomType() { return m_nInitialZoomType; }

	void SetXMarginDisplay( int nSize ) { m_nXMarginDisplay = nSize; }
	void SetSharpPosResolutionType( int n ){ m_nSharpPosResolutionType = n; }
	void SetShowPluNoFlag( bool bFlag ) { m_bShowPluNo = bFlag; }

	void SetKeyListHeight( int nHeight ) { m_nKeyListHeight = nHeight; }
	void SetLastKeyType ( int nType ) { m_nLastKeyType = nType; }
	void SetLastKeyValue( __int64 nValue ) { m_nLastKeyValue = nValue; }
	void SetLastFgndId( int nId ) { m_nLastFgndId = nId; }
	void SetLastBgndId( int nId ) { m_nLastBgndId = nId; }
	void SetLastFontSize( int nSize ) { m_nLastFontSize = nSize; }
	void SetKeyListWidth( int nWidth ) { m_nKeyListWidth = nWidth; }
	void SetDefaultBoldFlag( bool bFlag ){ m_bDefaultBold = bFlag; }
	void SetEditKeyTopLeftFlag( bool bFlag ){ m_bEditKeyTopLeft = bFlag; }
	void SetImageWithStyleFlag( bool bFlag ){ m_bImageWithStyle = bFlag; }
	void SetInitialViewMode( int nMode ) { m_nInitialViewMode = nMode; }
	void SetInitialZoomType( int nType ) { m_nInitialZoomType = nType; }
	
	void SetSQLInFilename( const char* szFilename ) { m_strSQLInFilename = szFilename; }
	void SetSQLOutFilename( const char* szFilename ) { m_strSQLOutFilename = szFilename; }
	void SetDataPath( const char* szPath ) { m_strDataPath = szPath; }
	void SetFilename( const char* szFilename ) { m_strFilename = szFilename; }
	void SetImageFolder( const char* sz ){ m_strImageFolder = sz; }
	void SetReportFolder( const char* sz ){ m_strReportFolder = sz; }
	void SetReportSettingsFolder( const char* sz ){ m_strReportSettingsFolder = sz; }
	void RestoreDefaults();

	const char* GetKeyboardName(){ return m_strKeyboardName; }
	void SetKeyboardName( const char* sz ){ m_strKeyboardName = sz; }

	void SetMaxBasePluLen( int n );
	void SetPluTextOrder( int n );
	
private:
	CString m_strCSVLine;
	CString m_strDataPath;
	CString m_strSQLInFilename;
	CString m_strSQLOutFilename;
	CString m_strFilename;
	CString m_strImageFolder;
	CString m_strReportFolder;
	CString m_strReportSettingsFolder;
	int m_nXMarginDisplay;
	int m_nSharpPosResolutionType;
	bool m_bShowPluNo;
	int m_nKeyListHeight;
	int m_nLastKeyType;
	__int64 m_nLastKeyValue;
	int m_nLastFgndId;
	int m_nLastBgndId;
	int m_nLastFontSize;
	int m_nKeyListWidth;
	bool m_bDefaultBold;
	bool m_bEditKeyTopLeft;
	bool m_bImageWithStyle;
	int m_nInitialViewMode;
	int m_nInitialZoomType;
	int m_nMaxBasePluLen;
	int m_nPluTextOrder;

	CString m_strKeyboardName;
};

/**********************************************************************/


