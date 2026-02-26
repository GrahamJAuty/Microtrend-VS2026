#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "FunctionAuthArray.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditFunctionOverrideDlg : public CSSDialog
{
public:
	CDefEditFunctionOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditFunctionOverrideDlg)
	enum { IDD = IDD_DEF_EDIT_FUNCTION_OVERRIDE };
	CStatic m_staticSettings;
	CButton m_checkOverride;
	CButton m_checkPassword;
	CButton m_checkAudit;
	CButton m_checkLog;
	CStatic m_staticOrigin;
	CEdit m_editLevelDefault;
	CButton m_checkPasswordDefault;
	CButton m_checkAuditDefault;
	CButton m_checkLogDefault;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditFunctionOverrideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditFunctionOverrideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleOverride();
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	CEdit* GetEditLevel();
	void CopyDefaultSettings();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	CFunctionAuthMiniInfo m_infoDefault;
	int m_nIndex;
	int m_nConLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/

