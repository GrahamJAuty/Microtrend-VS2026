#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "MacroHelpDlg.h"
/**********************************************************************/

class CDefEditMacroLocalDlg : public CSSDialog
{
public:
	CDefEditMacroLocalDlg( int nIndex, int nLocalType, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  
	~CDefEditMacroLocalDlg();

	//{{AFX_DATA(CDefEditMacroLocalDlg)
	enum { IDD = IDD_DEF_EDIT_MACRO_LOCAL };
	CEdit m_editParentType;
	CEdit m_editParentName;
	CEdit m_editParentData;
	CStatic m_staticLocal;
	CButton m_checkLocal;
	CEdit m_editLocalName;
	CEdit m_editLocalData;
	CButton m_buttonSpin;
	CButton m_buttonMacroHelp;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditMacroLocalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }
	void SetNoHelpButtonFlag( bool b ) { m_bNoHelpButton = b; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditMacroLocalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnToggleLocal();
	afx_msg void OnButtonMacroHelp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nIndex;

private:
	int m_nLocalType;
	CCascadingMacroCSVArray* m_pMacroArray;
	CMacroHelpDlg* m_pMacroHelpDlg;
	bool m_bNoHelpButton;
	bool m_bAllowSpin;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
