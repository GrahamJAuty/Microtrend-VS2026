#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CBackupPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CBackupPromptDlg)

public:
	CBackupPromptDlg( bool bDefaultWait, CWnd* pParent = NULL);   
	virtual ~CBackupPromptDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
	//{{AFX_DATA(CBackupPromptDlg)
	enum { IDD = IDD_BACKUP_PROMPT };
	CStatic m_staticSystem;
	CStatic m_staticTime;
	CStatic m_staticAutoSelect;
	CButton m_buttonAction1;
	CButton m_buttonAction2;
	CButton m_buttonAction3;
	CButton m_buttonAction4;

	//{{AFX_MSG(CBackupPromptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();
	afx_msg void OnButtonAction3();
	afx_msg void OnButtonAction4();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	virtual void OnOK(){}
	virtual void OnCancel(){}
	DECLARE_MESSAGE_MAP()

private:
	void ShowAutoSelectText( COleDateTime& timeCompare );
	
public:
	int m_nSelectedAction;	

private:
	bool m_bDefaultWait;
	CString m_strDefaultAction;
	COleDateTime m_AutoSelectTime;
	UINT m_hTimer;
	bool m_bInTimer;
	bool m_bBlockTimer;
};

/**********************************************************************/
#endif
/**********************************************************************/
