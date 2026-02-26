#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonStockTray\CustomField.h"
/**********************************************************************/

class CCustomFieldEditDlg : public CSSDialog
{
public:
	CCustomFieldEditDlg( const char* szTitle, int nValueIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomFieldEditDlg)
	enum { IDD = IDD_CUSTOM_FIELD_EDIT };
	CString	m_strName;
	CString	m_strKey;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomFieldEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetValueIdx() { return m_nValueIdx; }

protected:
	//{{AFX_MSG(CCustomFieldEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nValueIdx;
	CCustomFieldValueCSVRecord m_ValueRecord;
	CString m_strOldKey;
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
