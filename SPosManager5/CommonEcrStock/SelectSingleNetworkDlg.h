#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CSelectSingleNetworkDlg : public CDialog
{
public:
	CSelectSingleNetworkDlg( const char* szTitle, CWnd* pParent = NULL);   
	int GetNetworkIdx() { return m_nNetworkIdx; }
	
	//{{AFX_DATA(CSelectSingleNetworkDlg)
	enum { IDD = IDD_SELECT_INDIVIDUAL_NETWORK };
	CSSComboBoxEx	m_comboDatabase;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectSingleNetworkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSelectSingleNetworkDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;
	CString m_strTitle;
	int m_nNetworkIdx;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
