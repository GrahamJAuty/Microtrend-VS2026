#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
//include "MyComboBox.h"
//include "MyComboBoxEx.h"
/**********************************************************************/

class CPosTrayS4LabourFTPDlg : public CSSDialog
{
public:
	CPosTrayS4LabourFTPDlg( CWnd* pParent = NULL); 
	virtual ~CPosTrayS4LabourFTPDlg();

	enum { IDD = IDD_POSTRAY_S4LABOUR_FTP };
	CEdit m_editServer;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_buttonPassword;
	CSSComboBox m_comboHour;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnButtonPassword();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CString m_strNewPassword;
	bool m_bGotOldPassword;
};

/**********************************************************************/
#endif
/**********************************************************************/

