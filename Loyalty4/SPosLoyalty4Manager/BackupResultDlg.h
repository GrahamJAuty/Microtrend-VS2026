#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CBackupResultDlg : public CSSDialog
{
public:
	CBackupResultDlg( CBackupResultDlg** pParentToThis, CWnd* pParent = NULL);   
	virtual ~CBackupResultDlg();

	enum { IDD = IDD_BACKUP_RESULT };

	//{{AFX_VIRTUAL(CBackupResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonClose();
	virtual void PostNcDestroy();
	afx_msg void OnGetDispInfoListResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListNext(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonMore();
	DECLARE_MESSAGE_MAP()

public:
	void UpdateDisplay();

private:
	void SetDialogSize();

private:
	CSSListCtrlVirtual m_listResult;
	CSSListCtrlVirtual m_listNext;
	CStatic m_staticNext;
	CButton m_buttonMore;
	CButton m_buttonClose;

private:
	CBackupResultDlg** m_pParentToThis;
	bool m_bFullSize;
};

/**********************************************************************/
