#pragma once
/**********************************************************************/
#include "PictCtrl.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardImageSelectDlg : public CDialog
{
public:
	CSPOSKeyboardImageSelectDlg( const char* szOld, CWnd* pParent = NULL);

	//{{AFX_DATA(CSPOSKeyboardImageSelectDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_IMAGE_SELECT };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	CEdit	m_editFilter;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardImageSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardImageSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusFilter();
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	const char* GetNewSelectedImage(){ return m_strNewSelectedImage; }

private:
	void CreateList();
	void SelectLine( int nIndex );
	void DisplaySelectedImage();
	bool GetSelectedFilename( CString& strFilename );
	
private:
	CSSListCtrlVirtual m_listFiles;

private:
	CPictureCtrl m_staticImage;
	CString m_strOldSelectedImage;
	CString m_strNewSelectedImage;

private:
	CString m_strFilter;
	CArray<int,int> m_arrayImageIdx;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
