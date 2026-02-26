#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "BitmapInfo.h"
/**********************************************************************/

struct CButtonInfo
{
public:
	CButtonInfo();
	void Reset();
	
public:
	int Compare ( CButtonInfo& source, int nHint = 0 );
	void Add	( CButtonInfo& source );
	bool Equals	( CButtonInfo& other );
	
public:
	int GetListNo()					{ return m_nListNo; }
	int GetLineNo()					{ return m_nLineNo; }
	int GetButtonNo()				{ return m_nButtonNo; }
	int GetButtonType()				{ return m_nButtonType; }
	int GetDetail1()				{ return m_nDetail1; }
	int GetDetail2()				{ return m_nDetail2; }
	int GetDetail3()				{ return m_nDetail3; }
	bool GetActionFlag1()			{ return m_bActionFlag1; }
	bool GetOwnColourFlag()			{ return m_bOwnColour; }
	int GetOwnBackColour()			{ return m_nOwnBackColour; }
	int GetOwnTextColour()			{ return m_nOwnTextColour; }
	const char* GetTextLine2()		{ return m_strTextLine2; }
	const char* GetTextLine3()		{ return m_strTextLine3; }
	const char* GetInternalParams()	{ return m_strInternalParams; }
	CRect GetDisplayRect()			{ return m_rectDisplay; }
	int GetFontSize()				{ return m_nFontSize; }
	const char* GetImageType()		{ return m_strImageType; }
	const char* GetImageTypeTemp()	{ return m_strImageTypeTemp; }
	bool GetImageTransparentFlag()	{ return m_bImageTransparent; }
	
public:
	int GetDisplayBackColour();
	int GetDisplayTextColour();

public:
	void SetListNo( int n );
	void SetLineNo( int n );
	void SetButtonNo( int n );
	void SetButtonType( int n );
	void SetDetail1( int n );
	void SetDetail2( int n );
	void SetDetail3( int n );
	void SetActionFlag1( bool b ){ m_bActionFlag1 = b; }
	void SetOwnColourFlag( bool b ){ m_bOwnColour = b; }
	void SetOwnBackColour( int n );
	void SetOwnTextColour( int n );
	void SetInternalParams( const char* sz ){ m_strInternalParams = sz; }
	void SetDisplayRect( CRect rect ){ m_rectDisplay = rect; }
	void SetImageType( const char* szType );
	void SetImageTypeTemp( const char* szType );
	void SetImageTransparentFlag( bool b ){ m_bImageTransparent = b; }
	void SetFontSize( int n );

public:
	void CopyTextLines( CButtonInfo& infoSource );
	void GetTextLines( CStringArray& arrayLines );				
	void SetTextLines( const char* szLines );
	void SetTextLines( CCSV& csv );
	void GetDisplayTexts( CStringArray& arrayTexts );
	void SetDefaultTexts();

public:
	void ParseFileText( const char* szText );
	const char* CreateFileText();

public:
	void CreateSpecialButton( int nType );
	void LibraryToDisplay( CButtonInfo& infoLibrary );

public:
	bool GetButtonImageFilename( int nListNo, int nTempMode, CString& strFilename );

private:
	int m_nListNo;
	int m_nLineNo;
	int m_nButtonNo;
	int m_nButtonType;
	int m_nDetail1;
	int m_nDetail2;
	int m_nDetail3;
	bool m_bActionFlag1;
	bool m_bOwnColour;
	int m_nOwnBackColour;
	int m_nOwnTextColour;
	CString m_strTextLine1;
	CString m_strTextLine2;
	CString m_strTextLine3;	
	CRect m_rectDisplay;
	int m_nFontSize;
	CString m_strInternalParams;
	CString m_strImageType;
	CString m_strImageTypeTemp;
	bool m_bImageTransparent;

private:
	CString m_strFileText;
};

/**********************************************************************/
#endif
/**********************************************************************/
