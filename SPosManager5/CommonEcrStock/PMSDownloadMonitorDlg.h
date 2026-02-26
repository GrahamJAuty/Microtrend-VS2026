#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSDownloadMonitorDlg : public CDialog
{
public:
	CPMSDownloadMonitorDlg( CWnd* pParent = NULL);
	~CPMSDownloadMonitorDlg();

	//{{AFX_DATA(CPMSDownloadMonitorDlg)
	enum { IDD = IDD_PMS_DOWNLOAD_MONITOR };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSDownloadMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL
	CStatic m_staticFileCount;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	CButton m_buttonOK;

protected:
	//{{AFX_MSG(CPMSDownloadMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateFileCount();
	void CancelNow();

private:
	UINT m_hTimer;

private:
	int m_nRoomIdx;
	int m_nAccountIdx;
	int m_nTotalFiles;
	int m_nFilesProcessed;
	int m_nNextAction;
	int m_nLastAction;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
