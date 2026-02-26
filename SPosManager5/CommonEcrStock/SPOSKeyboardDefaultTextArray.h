#pragma once
/**********************************************************************/
#include "SPOSKeyboardManager.h"
/**********************************************************************/

class CSPOSKeyboardDefaultTextRecord 
{
public:
	CSPOSKeyboardDefaultTextRecord(); 
	void Reset();

public:
	__int64 GetValue() { return m_nValue; }
	int GetForeground() { return m_nForeground; }
	int GetBackground() { return m_nBackground; }
	int GetLegacyTextSize() { return m_nLegacyTextSize; }
	int GetFontSize() { return m_nFontSize; }
	int GetHAlign() { return m_nHAlign; }
	int GetVAlign() { return m_nVAlign; }
	bool GetBoldFlag() { return m_bBold; }
	const char* GetImageFilename() { return m_strImageFilename; }
	int GetImageScale() { return m_nImageScale; }

	void SetValue( __int64 n ) { m_nValue = n; }
	void SetForeground( int n ); 
	void SetBackground( int n );
	void SetLegacyTextSize( int n ){ m_nLegacyTextSize = n; }
	void SetFontSize( int n );
	void SetHAlign( int n );
	void SetVAlign( int n );
	void SetBoldFlag( bool b ){ m_bBold = b; }
	void SetImageFilename( const char* sz );
	void SetImageScale( int n );
	
	void SetSystemDefaultText( const char* szText );
	void SetUserDefaultText( const char* szText );

	void GetDefaultText( CString& strText );
	void GetUserDefaultText( CString& strText );
	void GetSystemDefaultText( CString& strText );
	
private:
	__int64 m_nValue;
	int m_nForeground;
	int m_nBackground;
	int m_nLegacyTextSize;
	int m_nFontSize;
	int m_nHAlign;
	int m_nVAlign;
	bool m_bBold;
	CString m_strImageFilename;
	int m_nImageScale;

	CString m_strSystemDefaultText;
	CString m_strUserDefaultText;
};

/**********************************************************************/

class CSPOSKeyboardDefaultTextArray 
{
public:
	CSPOSKeyboardDefaultTextArray();
	void Reset(){ m_arrayDefaultTexts.RemoveAll(); }

public:
	void SetTextType( const char* szDataPath, int nTextType );
	bool FindTextByValue ( __int64 nValue, int& nIndex );
	void AddText ( __int64 nValue, const char* szText );
	
	bool Read();
	bool Write();

	int GetSize() { return m_arrayDefaultTexts.GetSize(); }
	bool GetTextRecord( int nTextIdx, CSPOSKeyboardDefaultTextRecord& record );
	bool SetTextRecord( int nTextIdx, CSPOSKeyboardDefaultTextRecord& record );

	__int64 GetMinValue(){ return m_nMinValue; }
	__int64 GetMaxValue(){ return m_nMaxValue; }
	bool IsValidValue( __int64 nValue );

private:
	void AddDefaultLists();
	void AddDefaultFunctions();
	void AddDefaultPayments();
	void AddDefaultControls();
	void AddDefaultModifiers();
	void AddDefaultSvrPresets();
	void AddDefaultMacros();
	void AddDefaultEmptys();
	void RemoveAll();
	
private:
	CArray<CSPOSKeyboardDefaultTextRecord,CSPOSKeyboardDefaultTextRecord> m_arrayDefaultTexts;

private:
	CString m_strFilename;
	CString m_strCombiText;
	int m_nKeyType;
	__int64 m_nMinValue;
	__int64 m_nMaxValue;
};

/**********************************************************************/
