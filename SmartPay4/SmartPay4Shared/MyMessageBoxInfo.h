#pragma once
/**********************************************************************/

class CMyMessageBoxInfo
{
public:
	CMyMessageBoxInfo();

public:
	const char* GetTitle(){ return m_strTitle; }
	int GetTextCount(){ return m_arrayTexts.GetSize(); }
	const char* GetText( int nIdx );
	COLORREF GetBackColour(){ return m_colourBack; }
	COLORREF GetTextColour(){ return m_colourText; }
	int GetFontSize(){ return m_nFontSize; }
	int GetLineGap(){ return m_nLineGap; }
	int GetIconType(){ return m_nIconType; }
	int GetButtonMode(){ return m_nButtonMode; }
	int GetDefButton(){ return m_nDefButton; }

public:
	void SetTitle( const char* sz );
	void SetTexts( CStringArray& arrayTexts );
	void SetTexts( const char* szText );
	void SetBackColour( COLORREF cr );
	void SetTextColour( COLORREF cr );
	void SetFontSize( int n );
	void SetLineGap( int n );
	void SetIconType( int n );
	void SetButtonMode( int n );
	void SetDefButton( int n );

private:
	CString m_strTitle;
	CStringArray m_arrayTexts;
	COLORREF m_colourBack;
	COLORREF m_colourText;
	int m_nFontSize;
	int m_nLineGap;
	int m_nIconType;
	int m_nButtonMode;
	int m_nDefButton;

private:
	CString m_strText;
};

/**********************************************************************/
