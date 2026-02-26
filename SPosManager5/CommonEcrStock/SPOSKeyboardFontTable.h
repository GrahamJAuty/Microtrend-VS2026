#pragma once
/**********************************************************************/
#include "AfxTempl.h"
/**********************************************************************/

class CSPOSKeyboardStoredFont 
{
public:
	CSPOSKeyboardStoredFont ( CDC* pDC, int nFontSize, bool bBold );

public:
	CFont* GetFont() { return &m_Font; }
	int GetFontSize(){ return m_nFontSize; }
	bool GetBoldFlag() { return m_bBold; }

private:
	CFont m_Font;
	int m_nFontSize;
	bool m_bBold;
};

/**********************************************************************/

class CSPOSKeyboardFontTable 
{
public:
	CSPOSKeyboardFontTable();
	~CSPOSKeyboardFontTable();
	void Reset();
	
	void BuildFontTable ( CDC* pDC );
	
	void GetKeyDisplayFont( CDC* pDC, CFont* Font, int nFontSize, bool bBold, double dZoomRatio );
	
	int GetDialogFontCount() { return m_arrayDialogFonts.GetSize(); }
	CFont* GetDialogFont ( int nFontSize, bool bBold );

private:
	void AddDialogFont ( CDC* pDC, int nFontSize, bool bBold );
	
private:
	CArray<CSPOSKeyboardStoredFont*,CSPOSKeyboardStoredFont*> m_arrayDialogFonts;

private:
	bool m_bIsValid;
};

/**********************************************************************/
