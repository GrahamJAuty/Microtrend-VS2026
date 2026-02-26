/**********************************************************************/
#pragma once
/**********************************************************************/

class CUserEdit : public CEdit
{
public:
	CUserEdit();
	~CUserEdit();
	BOOL SubclassDlgItem ( UINT nID, CWnd* pParent );
	void SetValidChars ( const char* szValidChars ) { m_strValidChars = szValidChars; }
	void SetBackgroundColour ( COLORREF crColour );	// NULL uses standard edit colour

protected:
	afx_msg void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

private:
	CString m_strValidChars;
	COLORREF m_crBackgroundColour;
	CBrush* m_pBrush;
};

/**********************************************************************/
