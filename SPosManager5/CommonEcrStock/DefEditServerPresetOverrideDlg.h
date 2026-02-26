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

class CDefEditServerPresetOverrideDlg : public CSSDialog
{
public:
	CDefEditServerPresetOverrideDlg( int nPresetIdx, int nConLevel, int nServerEntityIdx, bool bDeferName, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditServerPresetOverrideDlg)
	enum { IDD = IDD_DEF_EDIT_SVRPRESET_OVERRIDE };
	CButton m_checkOverride;
	CStatic m_staticOrigin;
	CEdit m_editServerDefault;
	CSSComboBox m_comboServerDefault;
	CStatic m_staticSettings;
	CEdit m_editServer;
	CSSComboBox m_comboServer;
	CStatic m_staticName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditServerPresetOverrideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditServerPresetOverrideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleOverride();
	afx_msg void OnDefaultButton();
	afx_msg void OnKillFocusServerNo();
	afx_msg void OnSelectServer();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nPresetIdx; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
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
	CServerSelector m_ServerSelector;
	CServerSelector m_ServerSelectorDefault;
	CServerPresetMiniInfo m_infoDefault;
	int m_nPresetIdx;
	int m_nConLevel;
	int m_nServerEntityIdx;
	bool m_bDeferName;
};

/**********************************************************************/
#endif
/**********************************************************************/

