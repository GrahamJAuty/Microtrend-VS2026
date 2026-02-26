#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditAnalysisCategoryDlg : public CDialog
{
public:
	CDefEditAnalysisCategoryDlg( int nCatNo, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent );   

	//{{AFX_DATA(CDefEditAnalysisCategoryDlg)
	enum { IDD = IDD_DEF_EDIT_ANALYSIS_CATEGORY };
	CSSComboBox	m_comboType;
	CEdit	m_editEposText;
	CString	m_strEposText;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditAnalysisCategoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditAnalysisCategoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetCatNo() { return m_nCatNo; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	int m_nCatNo;
};

/**********************************************************************/
#endif			//__DefEditAnalysisCategoryDlg_H__
/**********************************************************************/

