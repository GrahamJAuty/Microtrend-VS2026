#pragma once
/**********************************************************************/
#include "picture.h"
/**********************************************************************/
#pragma once
/**********************************************************************/

class CPictureCtrl : public CStatic
{
public:
	CPictureCtrl();
	~CPictureCtrl();

public:
	BOOL LoadImage(LPCTSTR pszPathName);
	CSize GetImageSize() { return m_pict.GetImageSize(); }
	const CPicture* GetPicture() { return &m_pict; }

protected:
	CPicture m_pict;		// picture
	
	// message handlers
	afx_msg void OnPaint();
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_DYNAMIC(CPictureCtrl)
	DECLARE_MESSAGE_MAP()

public:
	void SetBlankFlag( bool b ){ m_bBlank = b; }
	void SetFailedFlag( bool b ){ m_bFailed = b; }
	const char* GetWidthString(){ return m_strWidth; }
	const char* GetHeightString(){ return m_strHeight; }

private:
	bool m_bBlank;
	bool m_bFailed;
	CString m_strWidth;
	CString m_strHeight;
};

/**********************************************************************/
