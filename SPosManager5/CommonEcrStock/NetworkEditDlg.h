#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "NetworkSPOSVersionArray.h"
/**********************************************************************/

class CNetworkEditDlg : public CSSDialog
{
public:
	CNetworkEditDlg( int nNetworkIdx, CWnd* pParent = NULL);  

	//{{AFX_DATA(CNetworkEditDlg)
	enum { IDD = IDD_NETWORK_EDIT };

	CEdit				m_editSPOSVersion;
	CString				m_strName;

	CSSComboBox			m_comboConnection;
	int					m_nConnectionType;

	CStatic				m_staticLine1;
	
	CEdit				m_editFilePath;
	CButton				m_buttonFilePath;
	CString				m_strFilePath;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CNetworkEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CNetworkEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
	afx_msg void OnSelectConnection();
	afx_msg void OnButtonFilePath();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nNetworkIdx;
	int m_nNetworkNo;
	int m_nSPOSVersion;
	
private:
	CNetworkSPOSVersionArray m_SPOSVersionArray;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

