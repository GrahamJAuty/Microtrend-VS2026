#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/
#ifdef SMP_TRAYICON
/**********************************************************************/

class CPresentationOptions
{
public:
	CPresentationOptions() {};

public:

	bool GetCustomMessageBoxFlag() { return TRUE; }
	int GetCustomMessageBoxFont() { return 12; }
	int GetCustomMessageBoxLineGap() { return 10; }
	const char* GetCustomMessageBoxColourBack() { return "FFFFFF"; }
	const char* GetCustomMessageBoxColourText() { return "000000"; }
	int GetCustomMessageBoxColourTextInt() { return 0xFFFFFF; }
	int GetCustomMessageBoxColourBackInt() { return 0x0; }
};

/**********************************************************************/
#else
/**********************************************************************/

class CPresentationOptions 
{
public:
	CPresentationOptions();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CPresentationOptions& source );	
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void LegacyReloadFromString( CString& strBuffer );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	void MakeIntSettings();
	
public:
	//CUSTOM MESSAGE BOX
	bool GetCustomMessageBoxFlag()				{ return m_CustomMessageBox.GetValue(); }
	int GetCustomMessageBoxFont()				{ return m_CustomMessageBoxFont.GetValue(); }
	int GetCustomMessageBoxLineGap()			{ return m_CustomMessageBoxLineGap.GetValue(); }
	const char* GetCustomMeesageBoxColourBack()	{ return m_CustomMessageBoxColourBack.GetValue(); }
	const char* GetCustomMeesageBoxColourText()	{ return m_CustomMessageBoxColourText.GetValue(); }

	//COLOURS
	int GetCustomMessageBoxColourBackInt()		{ return m_nCustomMessageBoxColourBack; }
	int GetCustomMessageBoxColourTextInt()		{ return m_nCustomMessageBoxColourText; }

private:
	int MakeColourInt( CIniFileString& iniString );
	bool MakeColourString( int nColour, CString& strColour );

public:
	//CUSTOM MESSAGE BOX
	void SetCustomMessageBoxFlag( bool b )				{ m_CustomMessageBox.SetValue(b); }
	void SetCustomMessageBoxFont( int n );
	void SetCustomMessageBoxLineGap( int n )			{ m_CustomMessageBoxLineGap.SetValue(n); }
	void SetCustomMessageBoxColourBack( const char* sz ){ m_CustomMessageBoxColourBack.SetValue(sz); }
	void SetCustomMessageBoxColourText( const char* sz ){ m_CustomMessageBoxColourText.SetValue(sz); }

	//COLOURS
	void SetCustomMessageBoxColourBackInt( int n );
	void SetCustomMessageBoxColourTextInt( int n );
	
private:
	//CUSTOM MESSAGE BOX
	CIniFileBool m_CustomMessageBox;
	CIniFileInt m_CustomMessageBoxFont;
	CIniFileInt m_CustomMessageBoxLineGap;
	CIniFileString m_CustomMessageBoxColourBack;
	CIniFileString m_CustomMessageBoxColourText;

	//COLOURS
	int m_nCustomMessageBoxColourBack;
	int m_nCustomMessageBoxColourText;
	
private:
	bool m_bFatalReadError;
};

/**********************************************************************/
#endif
/**********************************************************************/
extern CPresentationOptions PresentationOptions;		/* global */
/**********************************************************************/
