#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CSelectSingleDatabaseDlg : public CDialog
{
public:
	CSelectSingleDatabaseDlg( const char* szTitle, int nDbIdx = 0, CWnd* pParent = NULL);   
	int GetDbIdx() { return m_nDbIdx; }
	
	//{{AFX_DATA(CSelectSingleDatabaseDlg)
	enum { IDD = IDD_SELECT_INDIVIDUAL_DB };
	CSSComboBoxEx	m_comboDatabase;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectSingleDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSelectSingleDatabaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;
	CString m_strTitle;
	int m_nDbIdx;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
