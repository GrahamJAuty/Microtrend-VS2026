#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "FunctionAuthArray.h"
/**********************************************************************/

class CMacroHelpDlg : public CSSDialog
{
public:
	CMacroHelpDlg( CMacroHelpDlg** pParentToThis, CWnd* pParent = NULL);   
	virtual ~CMacroHelpDlg();

	enum { IDD = IDD_MACRO_HELP };

	//{{AFX_VIRTUAL(CMacroHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonClose();
	virtual void PostNcDestroy();
	afx_msg void OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListMacro(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListFunction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void PrepareList(CSSListCtrlVirtual& list, int nID, int nCount, CString strColumn1, CString strColumn2, int nColWidth1, int nColWidth2 );
	void SelectTab();

private:
	CTabCtrl m_TabSelect;
	CSSListCtrlVirtual m_listMacro;
	CSSListCtrlVirtual m_listFunction;
	CSSListCtrlVirtual m_listControl;
	CButton m_buttonCancel;

private:
	CStringArray m_arrayMacro;
	CFunctionAuthTable m_FunctionAuthTable;
	CStringArray m_arrayControl;
	int m_nTabPage;

private:
	CMacroHelpDlg** m_pParentToThis;
};

/**********************************************************************/
#endif
/**********************************************************************/
