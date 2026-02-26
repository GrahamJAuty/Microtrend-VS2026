#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "OPGroupArray.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditOPGroupOverrideDlg : public CSSDialog
{
public:
	CDefEditOPGroupOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditOPGroupOverrideDlg)
	enum { IDD = IDD_DEF_EDIT_OPGROUP_OVERRIDE };
	CStatic m_staticSettings;
	CButton m_checkOverride;
	CStatic m_staticOrigin;
	//}}AFX_DATA
	CButton	m_checkLocPrinter[PLU_ORDER_PRINTERS_NOW];
	CButton	m_checkSysPrinter[PLU_ORDER_PRINTERS_NOW];
	CStatic	m_staticSysPrinter[PLU_ORDER_PRINTERS_NOW];

	//{{AFX_VIRTUAL(CDefEditOPGroupOverrideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditOPGroupOverrideDlg)
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
	WORD m_IDLocPrinter[PLU_ORDER_PRINTERS_NOW];
	WORD m_IDSysPrinter[PLU_ORDER_PRINTERS_NOW];
	WORD m_IDStaticPrinter[PLU_ORDER_PRINTERS_NOW];

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	COPGroupMiniInfo m_infoDefault;
	int m_nIndex;
	int m_nConLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/

