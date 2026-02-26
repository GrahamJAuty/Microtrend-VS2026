#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "MacroHelpDlg.h"
/**********************************************************************/

class CDefEditMacroSystemDlg : public CSSDialog
{
public:
	CDefEditMacroSystemDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  
	~CDefEditMacroSystemDlg();

	//{{AFX_DATA(CDefEditMacroSystemDlg)
	enum { IDD = IDD_DEF_EDIT_MACRO_SYSTEM };
	CEdit m_editName;
	CEdit m_editData;
	CButton m_buttonMacroHelp;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditMacroSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }
	void SetNoHelpButtonFlag(bool b) { m_bNoHelpButton = b; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditMacroSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnButtonMacroHelp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	CMacroHelpDlg* m_pMacroHelpDlg;
	bool m_bNoHelpButton;
	int m_nIndex;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
