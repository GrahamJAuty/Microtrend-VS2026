#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CUnitechOptionsDlg : public CDialog
{
public:
	CUnitechOptionsDlg( CWnd* pParent = NULL );  

	//{{AFX_DATA(CUnitechOptionsDlg)
	//}}AFX_DATA
	CButton m_checkUnitechEnable;
	CEdit m_editPathProgram;
	CButton m_buttonBrowseProgram;
	CEdit m_editFileProgram;
	CButton m_radioStatusNone;
	CButton m_radioStatusRegistry;
	CButton m_radioStatusFile;
	CEdit m_editPathStatus;
	CButton m_buttonBrowseStatus;
	CEdit m_editFileStatus;
	CSSComboBox m_comboPort;
	CSSComboBox m_comboFastSpeed;
	CStatic m_staticMap;
	CSSComboBox m_comboMap;

	//{{AFX_VIRTUAL(CUnitechOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CUnitechOptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnToggleEnable();
	afx_msg void OnButtonBrowseProgram();
	afx_msg void OnButtonBrowseStatus();
	afx_msg void OnStatusNone();
	afx_msg void OnStatusRegistry();
	afx_msg void OnStatusFile();

private:
	void SetStatusType( int n );

private:
	bool m_bAllowNetUse;
	int m_nStatusType;
};

/**********************************************************************/
#endif
/**********************************************************************/
