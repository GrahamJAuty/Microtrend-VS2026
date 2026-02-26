#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditReasonLocalDlg : public CSSDialog
{
public:
	CDefEditReasonLocalDlg( int nIndex, int nLocalType, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditReasonLocalDlg)
	enum { IDD = IDD_DEF_EDIT_REASON_LOCAL };
	CEdit m_editParentType;
	CEdit m_editParentText;
	CButton m_checkParentManual;
	CStatic m_staticLocal;
	CButton m_checkLocal;
	CEdit m_editLocalText;
	CButton m_checkLocalManual;
	CButton m_buttonSpin;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditReasonLocalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditReasonLocalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnToggleLocal();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nIndex;

private:
	int m_nLocalType;
	CCascadingReasonCSVArray* m_pReasonArray;
	bool m_bAllowSpin;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
