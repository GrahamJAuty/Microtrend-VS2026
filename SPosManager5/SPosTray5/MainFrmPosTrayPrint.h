#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/

class CMainFramePosTrayPrint : public CFrameWnd
{
public:
	CMainFramePosTrayPrint();
	virtual ~CMainFramePosTrayPrint();

public:
	void PrintReports();

protected:
	DECLARE_DYNAMIC(CMainFramePosTrayPrint)

	//{{AFX_MSG(CMainFramePosTrayPrint)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};

/**********************************************************************/
#endif
/**********************************************************************/
