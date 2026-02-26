#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define STOCK3_TAB_OPENING 0
#define STOCK3_TAB_ADHOC 1
#define STOCK3_TAB_RETURN 2
#define STOCK3_TAB_ADJUST 3
#define STOCK3_TAB_TRANSFER 4
#define STOCK3_TAB_SALES 5
#define STOCK3_TAB_CLOSING 6
#define STOCK3_TAB_REORDER 7
/**********************************************************************/

class CPropPageOptionsStock2 : public CSSPropertyPage
{
public:
	CPropPageOptionsStock2();
	~CPropPageOptionsStock2();

	//{{AFX_DATA(CPropPageOptionsStock2)
	enum { IDD = IDD_PROPPAGE_OPTIONS_STOCK2 };
	//}}AFX_DATA
	CTabCtrl m_TabActions;
	
	CButton m_checkRef;
	CStatic m_staticRefPrefix;
	CEdit m_editRefPrefix;
	CStatic m_staticRefNumber;

	CStatic m_staticImport;
	CStatic m_staticImportCSV;
	CButton m_checkImportPLU;
	CButton m_checkImportWrapper;
	CButton m_checkImportSheet;
	CStatic m_staticImportPLU;
	CEdit m_editImportPLU;
	CStatic m_staticImportWRP;
	CEdit m_editImportWRP;
	CButton m_checkImportDelValueUse;
	CButton m_checkImportDelValueSave;
	CButton m_checkImportAdjustValueUse;
	CButton m_checkImportReturnValueUse;
	CButton m_checkImportSalesException;

	CStatic m_staticDeliver;
	CStatic m_staticDeliverCost;
	CSSComboBox m_comboDeliverCost;
	CStatic m_staticDeliverSuppRef;
	CSSComboBox m_comboDeliverSuppRef;

	CStatic m_staticSaleEntry;
	CSSComboBox m_comboSaleEntry;
	CStatic m_staticAdjustEntry;
	CSSComboBox m_comboAdjustEntry;

	//{{AFX_VIRTUAL(CPropPageOptionsStock2)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnToggleEnableRef();
	afx_msg void OnKillFocusNumber();
	afx_msg void OnToggleImportDelValue();

protected:
	//{{AFX_MSG(CPropPageOptionsStock2)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckImportValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRefNumber();

private:
	void LoadTabSettings();
	void SaveTabSettings();
	void SetTabControls();

private:
	int m_nActiveTab;
};

/**********************************************************************/
#endif 
/**********************************************************************/
