#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditWalletStampRuleDlg : public CSSDialog
{
public:
	CDefEditWalletStampRuleDlg( int nIndex, CReportConsolidationArray<CSortedIntByInt>& arrayEditions, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditWalletStampRuleOfferDlg)
	enum { IDD = IDD_DEF_EDIT_WALLETSTAMPRULE };
	
	CEdit m_editDescription;
	CButton m_checkEnable; 
	CStatic m_staticAction;
	CSSComboBox m_comboAction;
	CSpinButtonCtrl	m_SpinCtrl;
	CStatic m_staticOfferGroup;
	CSSComboBox	m_comboOfferGroup;
	CButton m_buttonEditGroup;
	CStatic m_staticThreshold;
	CStatic m_staticExpiry;
	CStatic m_staticNeverExpire;
	CStatic m_staticEdition;
	CEdit m_editEdition;
	CButton m_buttonEdition;
	CButton m_buttonDefault;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditWalletStampRuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	CEdit* GetEditThreshold();
	CEdit* GetEditExpiry();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditOfferWalletStampRuleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnButtonEditGroup();
	afx_msg void OnButtonEdition();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nIndex;
	bool m_bAllowSpin;

private:
	CString m_strDescription;

private:
	CString m_strInitialName;
	int m_nInitialType;
	int m_nInitialSelection;

private:
	CReportConsolidationArray<CSortedIntByInt>& m_arrayOriginalEditions;
	int m_nOriginalEdition;
	bool m_bIsOriginalEdition;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
