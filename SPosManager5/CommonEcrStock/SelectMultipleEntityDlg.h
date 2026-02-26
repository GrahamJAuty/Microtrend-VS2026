#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SelectMultipleEntityInfo.h"
/**********************************************************************/

class CSelectMultipleEntityDlg : public CSSDialog
{
public:
	CSelectMultipleEntityDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bWantAllNone, bool bAutoSelectSingle, CWnd* pParent = NULL);   
	void SetDoubleClickExitFlag(){ m_bDoubleClickExit = TRUE; }

	//{{AFX_DATA(CSelectMultipleEntityDlg)
	enum { IDD = IDD_SELECT_MULTIPLE_ENTITY };
	CButton m_checkOption;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CButton m_buttonAll;
	CButton m_buttonNone;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectMultipleEntityDlg)

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	virtual void SetListColumnns() = 0;
	virtual void GetListData() = 0;
	virtual bool HandleOK();
	virtual void HandleCancel();
	virtual void HandleAll();
	void RedrawList();

protected:
	//{{AFX_MSG(CSelectMultipleEntityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bWantAllNone;
	bool m_bAutoSelectSingle;
	
protected:
	int m_nColumnCount;
	int m_nInitialSel;
	CString m_strTitle;
	CString m_strOption;
	bool m_bEndAsCancel;
	bool m_bDoubleClickExit;

protected:
	CSSListMultiSelectCtrl m_listEntities;
	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& m_arrayEntityInfo;
};

/**********************************************************************/
#endif		//__SELECTEENTITYMULTIPLEDLG_H__
/**********************************************************************/

