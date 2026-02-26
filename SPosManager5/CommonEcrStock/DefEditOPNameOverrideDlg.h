#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "OPNameArray.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditOPNameOverrideDlg : public CSSDialog
{
public:
	CDefEditOPNameOverrideDlg( int nIndex, int nConLevel, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditOPNameOverrideDlg)
	enum { IDD = IDD_DEF_EDIT_OPNAME_OVERRIDE };
	CStatic m_staticSettings;
	CButton m_checkOverride;
	CStatic m_staticOrigin;
	//}}AFX_DATA
	CEdit m_editLocName;
	CEdit m_editSysName;

	//{{AFX_VIRTUAL(CDefEditOPNameOverrideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditOPNameOverrideDlg)
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
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	COPNameMiniInfo m_infoDefault;
	int m_nIndex;
	int m_nConLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/

