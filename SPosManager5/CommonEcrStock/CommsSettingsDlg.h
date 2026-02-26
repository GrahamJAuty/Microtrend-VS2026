#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CCommsSettingsDlg : public CDialog
{
public:
	CCommsSettingsDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CCommsSettingsDlg)
	enum { IDD = IDD_COMMS_SETTINGS };
	BOOL	m_bBuffer;
	//}}AFX_DATA
	CSSComboBox m_comboSpeed;

	//{{AFX_VIRTUAL(CCommsSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCommsSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/


