#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPageListDlg : public CDialog
{
public:
	CSPOSKeyboardPageListDlg( int nListIdx, int nPageNo, CWordArray& arrayPageTypes, CWnd* pParent = NULL);

	//{{AFX_DATA(CSPOSKeyboardPageListDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_PAGE_LIST };
	CEdit	m_editPages;
	CEdit	m_editKeys;
	CEdit	m_editFormat;
	CSSListCtrl	m_listPages;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPageListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardPageListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonMoveDown();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnSelectPage(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetView( CSPOSKeyboardView* pView ){ m_pView = pView; }

private:
	void AddList();
	void UpdateCounters();
	void HandleSelectPage();

private:
	CSPOSKeyboardView* m_pView;
	
private:
	int m_nListIdx;
	int m_nInitialPageNo;
	bool m_bDoneListItemHeight;
	CSPOSKeyboardListRecord m_ListRecord;
	CWordArray& m_arrayPageTypes;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

