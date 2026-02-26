#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "FunctionAuthArray.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditFunctionDlg : public CSSDialog
{
public:
	CDefEditFunctionDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditFunctionDlg)
	enum { IDD = IDD_DEF_EDIT_FUNCTION };
	CButton m_checkPassword;
	CButton m_checkAudit;
	CButton m_checkLog;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditFunctionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditFunctionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	CEdit* GetEditLevel();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	int m_nIndex;
};

/**********************************************************************/
#endif
/**********************************************************************/

