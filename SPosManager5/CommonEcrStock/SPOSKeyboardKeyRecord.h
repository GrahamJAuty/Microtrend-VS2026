#pragma once
/**********************************************************************/

class CSPOSKeyboardKeyRecord 
{
public:
	CSPOSKeyboardKeyRecord();
	void Reset();

public:
	__int64 GetListNo() { return m_nListNo; }
	int GetPageNo() { return m_nPageNo; }
	int GetKeyNo() { return m_nKeyNo; }
	__int64 GetLookUpKey();
	
	int GetType() { return m_nType; }
	__int64 GetValue() { return m_nValue; }
	int GetForeground() { return m_nForeground; }
	int GetBackground() { return m_nBackground; }
	int GetLegacyTextSize() { return m_nLegacyTextSize; }
	int GetRowSize() { return m_nRowSize; }
	int GetColSize() { return m_nColSize; }
	int GetFontSize() { return m_nFontSize; }
	bool GetBoldFlag() { return m_bBold; }
	int GetAlignmentH() { return m_nAlignmentH; }
	int GetAlignmentV() { return m_nAlignmentV; }
	const char* GetImageFilename() { return m_strImageFilename; }
	int GetImageScale(){ return m_nImageScale; }
	bool GetHideTextFlag(){ return m_bHideText; }
	bool GetDefaultTextAlwaysFlag(){ return m_bDefaultTextAlways; }
	bool GetGJANoSortFlag(){ return m_bGJANoSort; }
	
	bool GetTempMarker() { return m_bTempMarker; }

	void SetListNo( __int64 n ); 
	void SetPageNo( int n ); 
	void SetKeyNo( int n ); 
	void SetType( int n ); 
	void SetValue( __int64 n ); 
	void SetForeground( int n ); 
	void SetBackground( int n ); 
	void SetLegacyTextSize( int n ); 
	void SetRowSize( int n ); 
	void SetColSize( int n ); 
	void SetFontSize( int n );
	void SetBoldFlag( bool b ){ m_bBold = b; }
	void SetAlignmentH( int n );
	void SetAlignmentV( int n );
	void SetImageFilename( const char* sz );
	void SetImageScale( int n );
	void SetHideTextFlag( bool b ){ m_bHideText = b; }
	void SetDefaultTextAlwaysFlag( bool b ){ m_bDefaultTextAlways = b; }
	void SetGJANoSortFlag( bool b ){ m_bGJANoSort = b; }
	
	void SetTempMarker( bool b ) { m_bTempMarker = b; }

	const char* GetKeyText(){ return m_strKeyText; }
	const char* GetTrimmedKeyText();
	const char* GetEditableKeyText();
	void GetDisplayTexts( CStringArray& Texts, bool bForPluSort );
	
	void SetKeyText( const char* szText ){ m_strKeyText = szText; }
	void SetEditableKeyText( const char* szText );

	static bool IsValidFontSize(int n);
	static void GetFontSizeList(CWordArray& arrayList);

private:
	__int64 m_nListNo;
	int m_nPageNo;
	int m_nKeyNo;
	int m_nType;
	__int64 m_nValue;
	CString m_strKeyText;
	CString m_strCombiText;
	int m_nForeground;
	int m_nBackground;
	int m_nLegacyTextSize;
	int m_nRowSize;
	int m_nColSize;
	int m_nFontSize;
	bool m_bBold;
	int m_nAlignmentH;
	int m_nAlignmentV;
	CString m_strImageFilename;
	int m_nImageScale;
	bool m_bHideText;
	bool m_bDefaultTextAlways;
	bool m_bGJANoSort;

private:
	bool m_bTempMarker;

private:
	CString m_strTrimmedKeyText;
	CString m_strEditableKeyText;
};

/**********************************************************************/


