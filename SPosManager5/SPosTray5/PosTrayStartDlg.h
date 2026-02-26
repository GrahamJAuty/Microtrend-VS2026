#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPosTrayStartDlg : public CDialog
{
public:
	CPosTrayStartDlg( CWnd* pParent = NULL);
	~CPosTrayStartDlg();

	//{{AFX_DATA(CPosTrayStartDlg)
	enum { IDD = IDD_POSTRAY_START };
	CEdit	m_editTime;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayStartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	bool GetRetryFlag(){ return m_bRetry; }

private:
	void ShowCountdown();
	void CancelNow();
	
protected:
	//{{AFX_MSG(CPosTrayStartDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT m_hTimer;

private:
	bool m_bRetry;
	int m_nCountdown;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
