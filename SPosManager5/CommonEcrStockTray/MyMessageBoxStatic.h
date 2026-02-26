#pragma once
/**********************************************************************/
#include "MyMessageBoxInfo.h"
/**********************************************************************/

class CMyMessageBoxStatic : public CStatic
{
public:
	CMyMessageBoxStatic( CMyMessageBoxInfo& MessageBoxInfo );
	virtual ~CMyMessageBoxStatic();

public:
	void CalculateTextExtents();
	void AllowPaint(){ m_bCanPaint = TRUE; }
	
public:
	int GetTextHeight(){ return m_nTextHeight; }
	int GetTextWidth(){ return m_nTextWidth; }

private:
	void DrawText ( CDC* pDC, CRect rect );
	void GetFont( CDC* pDC, CFont& Font, int nFontSize );

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()	

private:
	int m_nTextWidth;
	int m_nTextHeight;
	bool m_bCanPaint;

private:
	CMyMessageBoxInfo& m_MessageBoxInfo;
};

/**********************************************************************/
