#pragma once
/**********************************************************************/

class CEditEnter : public CEdit
{
	DECLARE_DYNAMIC(CEditEnter)

public:
	CEditEnter();
	virtual ~CEditEnter();

public:
	void SetMessageTarget( CWnd* pMessageTarget, int nMessageNo );

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg long OnGetDlgCode ( WPARAM wIndex, LPARAM lParam );
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	CWnd* m_pMessageTarget;
	int m_nMessageNo;
};

/**********************************************************************/


