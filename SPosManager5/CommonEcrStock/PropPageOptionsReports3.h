#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsReports3 : public CSSPropertyPage
{
public:
	CPropPageOptionsReports3();
	~CPropPageOptionsReports3();

	//{{AFX_DATA(CPropPageOptionsReports3)
	enum { IDD = IDD_PROPPAGE_OPTIONS_REPORTS3 };
	
	CSSComboBox	m_comboQuantity;
	CSSComboBox	m_comboValue;
	CSSComboBox m_comboSort;
	CSSComboBox m_comboSalesTaxType;
	
	CButton	m_checkReportpoints;
	CButton	m_checkLocationSets;
	CStatic	m_staticSites;
	CButton	m_checkSites;
	CButton m_checkRegion;
	CButton	m_checkHideStockZeros;
	CButton	m_checkHidePluZeros;
	CButton m_checkCashRSPText;
	CButton m_checkCashRSPDiscountPenny;

	CStatic m_staticPrinterBox;
	CEdit m_editPrinterClient;
	CEdit m_editPrinterHost;

	CToolTipCtrl m_ToolTip;
	TOOLINFO m_ToolInfo;

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsReports3)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnSelectTaxType();
	afx_msg void OnButtonPrinterClient();
	afx_msg void OnButtonPrinterHost();
	BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	//{{AFX_MSG(CPropPageOptionsReports3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRecordControls();

public:
	static void ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction );
	static void PrinterListError( int n );

private:
	bool SaveRecord();
	void SetOptionTicks();

private:
	CString m_strFYear = "";
	CString m_strSystemDefaultPrinterClient = "";
	CString m_strSystemDefaultPrinterHost = "";
	CString m_strToolTipText = "";
};

/**********************************************************************/
#endif
/**********************************************************************/
