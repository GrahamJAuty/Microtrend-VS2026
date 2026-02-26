#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CascadingMacroSelector.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditTableAreaDlg : public CSSDialog
{
public:
	CDefEditTableAreaDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditTableAreaDlg)
	enum { IDD = IDD_DEF_EDIT_TABLE_AREA };
	CEdit m_editName;
	CButton m_checkKP[16];
	CButton m_checkAutoTab;
	CButton m_checkSuppressCovers;
	CEdit m_editMacro;
	CSSComboBox m_comboMacro;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditTableAreaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	WORD m_wIDKP[16];

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	CEdit* GetEditTableFrom();
	CEdit* GetEditTableTo();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditTableAreaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnKillFocusMacro();
	afx_msg void OnSelectMacro();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CCascadingMacroSelector m_MacroSelector;
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nIndex;
	int m_nMacroNo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
