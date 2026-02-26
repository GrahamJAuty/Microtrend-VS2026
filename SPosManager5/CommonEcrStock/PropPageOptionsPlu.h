#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsPlu : public CSSPropertyPage
{
public:
	CPropPageOptionsPlu();
	~CPropPageOptionsPlu();

	//{{AFX_DATA(CPropPageOptionsPlu)
	enum { IDD = IDD_PROPPAGE_OPTIONS_PLU };
	
	CTabCtrl m_TabActions;

	CStatic m_staticEditing1;
	CButton m_checkTabBarcode;
	CButton m_checkTabLink;
	CButton m_checkTabInfo;
	CButton m_checkDownloadDate;
	CButton m_checkPriceSets;
	CButton m_checkInactiveNoDownload;
	CButton m_checkInactiveAutoHide;
	CButton m_checkTask;
	CButton m_buttonTask;
	CSSComboBox m_comboPluText;

	CButton m_checkImportCSVAuto;
	CButton m_checkImportCSVManual;
	CButton m_checkExportCSVManual;
	CButton m_checkExportPurchaseAuto;
	CButton m_checkImportProdCSVManual;
	CButton m_checkImportProdCSVAuto;

	CStatic m_staticModifiers1;
	CStatic m_staticModifiers2;
	CButton m_checkDiscardModifiers;
	CButton m_checkBarcodeModifier;
	CSSComboBox m_comboModTextEpos;
	CSSComboBox m_comboModTextKey;
	
	CButton m_checkDeleteBarcode;
	CSSComboBox m_comboDeleteBarcode;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsPlu)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageOptionsPlu)
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void SetActiveTabControls();
	//}}AFX_MSG
	afx_msg void OnEditTask();
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();
	void SetTabControls( bool bActiveTabOnly );

private:
	int m_nActiveTab;
	bool m_bAllowModifiers;
	CArray<int,int> m_arrayTabTypes;
};

/**********************************************************************/
#endif
/**********************************************************************/
