#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDownloadDateDlg : public CSSDialog
{
public:
	CDownloadDateDlg( const char* szTitle, CWnd* pParent = NULL);
	
	void SetV3KeyboardFlag(){ m_bV3Keyboard = TRUE; }
	bool RunIfNeeded();

	//{{AFX_DATA(CDownloadDateDlg)
	enum { IDD = IDD_DOWNLOAD_DATE };
	CButton	m_staticGroup;
	CButton	m_checkImages;
	CDateTimeCtrl	m_DatePicker;
	int		m_nDownloadType;
	BOOL	m_bImages;
	//}}AFX_DATA
	CButton	m_radioImmediate;
	CButton	m_radioDeferred;
	
	//{{AFX_VIRTUAL(CDownloadDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDownloadDateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioDeferred();
	afx_msg void OnRadioImmediate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bV3Keyboard;
	CString m_strTitle;

public:
	bool m_bImmediate;
	CString m_strDate;
};

/**********************************************************************/


