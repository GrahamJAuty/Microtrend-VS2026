#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\ColourButton.h"
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsStock : public CSSPropertyPage
{
public:
	CPropPageOptionsStock();
	~CPropPageOptionsStock();

	//{{AFX_DATA(CPropPageOptionsStock)
	enum { IDD = IDD_PROPPAGE_OPTIONS_STOCK };

	CTabCtrl m_TabActions;

	CButton m_checkImportStockManual;
	CButton m_checkExportStockManual;
	CStatic m_staticItems1;
	CSSComboBox m_comboSupplierCount;
	CButton m_checkSubUnits;

	CButton m_checkSimpleEdit;
	CButton m_checkDeleteStock;
	CButton m_checkBrowseStock;
	CButton m_checkRedirect;

	CButton m_checkAutoStock;
	CButton m_checkPluEntry;
	CButton m_checkFinalise;
	CButton m_checkBelowMin;
	CColourButtonBase m_buttonColourBelowMin;
	CButton m_checkAboveMax;
	CColourButtonBase m_buttonColourAboveMax;
	CButton m_checkStockmate;

	CButton	m_staticOrders1;
	CSSComboBox m_comboOrderGenerate;
	CSSComboBox m_comboOrderUnits;
	CButton	m_staticOrders2;
	CSSComboBox m_comboOrderSuggest;
	CButton m_checkOrderTemplates;
	CButton m_checkRestrictPreview;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsStock)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnButtonBelowMin();
	afx_msg void OnButtonAboveMax();
	afx_msg long OnColourButtonMessage ( WPARAM wIndex, LPARAM lParam );

protected:
	//{{AFX_MSG(CPropPageOptionsStock)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void SelectTab();

private:
	int m_nActiveTab;
	int m_nColourMode;
};

/**********************************************************************/
#endif // __PROPPAGEOPTIONSSTOCK_H__
/**********************************************************************/
