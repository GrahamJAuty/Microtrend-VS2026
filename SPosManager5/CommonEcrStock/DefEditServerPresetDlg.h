#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "ServerPresetArray.h"
#include "ServerSelector.h"
/**********************************************************************/

class CDefEditServerPresetDlg : public CSSDialog
{
public:
	CDefEditServerPresetDlg( int nIndex, const char* szDeferName, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditServerPresetDlg)
	enum { IDD = IDD_DEF_EDIT_SVRPRESET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditServerPresetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	CStatic m_staticName;
	CEdit m_editServer;
	CSSComboBox m_comboServer;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditServerPresetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnKillFocusServerNo();
	afx_msg void OnSelectServer();
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
	CServerSelector m_ServerSelector;
	CString m_strDeferName;
	int m_nIndex;
};

/**********************************************************************/
#endif
/**********************************************************************/

