#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "GroupCSVArrayBase.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditGroupDlg : public CSSDialog
{
public:
	CDefEditGroupDlg( CGroupCSVArrayBase* pArray, int nGroupType, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditGroupDlg)
	enum { IDD = IDD_DEF_EDIT_GROUP };
	CStatic m_staticLabel;
	CEdit	m_editRepText;
	CString	m_strRepText;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	CGroupCSVArrayBase* m_pGroupArray;
	int m_nGroupType;
	int m_nIndex;
};

/**********************************************************************/
#endif
/**********************************************************************/

