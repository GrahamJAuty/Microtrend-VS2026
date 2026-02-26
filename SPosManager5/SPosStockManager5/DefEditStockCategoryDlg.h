#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ListDataDlg.h"
/**********************************************************************/
#define DEFEDIT_STOCKCAT_CATEGORY 0
#define DEFEDIT_STOCKCAT_ALLOWANCE 1
#define DEFEDIT_STOCKCAT_SYSTEM 2
/**********************************************************************/

class CDefEditStockCategoryDlg : public CSSDialog
{
public:
	CDefEditStockCategoryDlg( int nMode, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent);   

	//{{AFX_DATA(CDefEditStockCategoryDlg)
	enum { IDD = IDD_DEF_EDIT_STOCK_CATEGORY };
	CEdit	m_editText;
	CStatic m_staticTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CButton	m_checkEnable;
	CSpinButtonCtrl m_buttonSpin;
	CButton m_buttonDefault;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditStockCategoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditStockCategoryDlg)
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
	int m_nIndex;
	int m_nMaxIndex;
	int m_nMode;
};

/**********************************************************************/
#endif
/**********************************************************************/

