#pragma once
/**********************************************************************/
#include "AfxTempl.h"
/**********************************************************************/

class CStoredFont 
{
public:
	CStoredFont ( CDC* pDC, int nFontSize, bool bBold );

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

class CFontTable 
{
public:
	CFontTable();
	~CFontTable();
	
	void BuildFontTable ( CDC* pDC );
	
	int GetDialogFontCount() { return m_arrayDialogFonts.GetSize(); }
	CFont* GetDialogFont ( int nFontSize, bool bBold );
	void GetKeyDisplayFont( CDC* pDC, CFont* Font, int nFontSize, bool bBold );

private:
	void AddDialogFont ( CDC* pDC, int nFontSize, bool bBold );
	
private:
	CArray<CStoredFont*,CStoredFont*> m_arrayDialogFonts;

private:
	bool m_bIsValid;
};

/**********************************************************************/
extern CFontTable FontTable;
/**********************************************************************/
