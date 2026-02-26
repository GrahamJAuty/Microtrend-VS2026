#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "BitmapInfo.h"
/**********************************************************************/

class CColourButtonBase : public CButton
{
public:
	CColourButtonBase();
	
public:
	int m_nButtonNo;
	bool m_bBlackBorder;
	COLORREF m_ColourRefBack;
	
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourButtonBase)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual void HandleDraw( LPDRAWITEMSTRUCT lpDrawItemStruct );

protected:
	//{{AFX_MSG(CColourButtonBase)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/

class CColourButtonText : public CColourButtonBase
{
public:
	CColourButtonText();
	~CColourButtonText();

public:
	void SetTransparentFlag( bool b ){ m_bTransparent = b; }
	bool LoadImage( const char* szFilename );
	void SwapImage();

private:
	CBitmapInfo* GetBitmapInfo();

public:
	int m_nFontSize;
	CString m_strText;
	COLORREF m_ColourRefText;

public:
	void HandleDraw( LPDRAWITEMSTRUCT lpDrawItemStruct );

private:
	CBitmapInfo m_BitmapInfo1;
	CBitmapInfo m_BitmapInfo2;
	int m_nCurrentImage;
	bool m_bTransparent;
};

/**********************************************************************/

class CColourButtonPMS : public CColourButtonBase
{
public:
	CColourButtonPMS();

public:
	int m_nFontSize;
	COLORREF m_ColourRefText;
	COLORREF m_ColourRefBottomLineBack;
	COLORREF m_ColourRefBottomLineText;
	CString m_strLine1;
	CString m_strLine2;
	CString m_strLine3;
	CString m_strLine4;
	CString m_strLine5;
	CString m_strLine6;
	CString m_strLine7;

public:
	void HandleDraw( LPDRAWITEMSTRUCT lpDrawItemStruct );
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
