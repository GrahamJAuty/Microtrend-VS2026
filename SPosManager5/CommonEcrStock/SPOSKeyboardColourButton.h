#pragma once
/**********************************************************************/

class CSPOSKeyboardColourButton : public CButton
{
public:
	CSPOSKeyboardColourButton();
	void SetActiveTableMode( bool b ){ m_bActiveTableMode = b; }
	void Set86ItemMode( bool b ){ m_b86ItemMode = b; }
	void Set86TransparentFlag( bool b ){ m_b86Transparent = b; }
	void SetFontSize( int nFontSize );
	void Set86ItemWidth( int n );
	void Set86ItemHeight( int n );
	void SetVAlign( int n );
	void SetHAlign( int n );
	void SetBoldFlag( bool b ){ m_bBold = b; }

public:
	int m_nButtonNo;
	COLORREF m_ColourRefBack;
	COLORREF m_ColourRefText;

public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPOSKeyboardColourButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual ~CSPOSKeyboardColourButton();

protected:
	//{{AFX_MSG(CSPOSKeyboardColourButton)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	bool m_bActiveTableMode;
	bool m_b86ItemMode;
	bool m_b86Transparent;
	bool m_bBold;
	int m_n86ItemWidth;
	int m_n86ItemHeight;
	int m_nHAlign;
	int m_nVAlign;
	int m_nFontSize;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
