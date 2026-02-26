#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LabelNameHandler.h"
/**********************************************************************/

class CPropPageOptionsLabels : public CSSPropertyPage
{
public:
	CPropPageOptionsLabels();
	~CPropPageOptionsLabels();

	//{{AFX_DATA(CPropPageOptionsLabels)
	enum { IDD = IDD_PROPPAGE_OPTIONS_LABELS };

	CButton m_checkShelfOption1;
	CButton m_checkShelfOption2;
	CButton m_checkShelfOption3;
	CButton m_checkShelfOption4;

	CSSComboBox	m_comboShelfEdgePrice1;
	CSSComboBox	m_comboShelfEdgeTax1;
	CEdit m_editShelfEdgeCurrency1;

	CSSComboBox	m_comboShelfEdgePrice2;
	CSSComboBox	m_comboShelfEdgeTax2;
	CEdit m_editShelfEdgeCurrency2;
	
	CButton m_checkProductOption1;
	CButton m_checkProductOption2;
	CButton m_checkProductOption3;
	CButton m_checkProductOption4;
	
	CSSComboBox	m_comboProductPrice1;
	CSSComboBox	m_comboProductTax1;
	CEdit m_editProductCurrency1;

	CSSComboBox	m_comboProductPrice2;
	CSSComboBox	m_comboProductTax2;
	CEdit	m_editProductCurrency2;

	CStatic m_staticShelf1;
	CStatic m_staticShelf2;
	CStatic m_staticShelf3;
	CStatic m_staticShelf4;

	CStatic m_staticProduct1;
	CStatic m_staticProduct2;
	CStatic m_staticProduct3;
	CStatic m_staticProduct4;

	CEdit m_editPrinter;

	CString	m_strShelfEdgeCurrency1;
	CString	m_strShelfEdgeCurrency2;
	
	CString	m_strProductCurrency1;
	CString	m_strProductCurrency2;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsLabels)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageOptionsLabels)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPriceLevel2();
	afx_msg void OnButtonPrinter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddComboPriceLevels( CSSComboBox& combo, bool bNone );
	void AddComboTaxType( CSSComboBox& combo );
	void PrinterListError( int n );

public:
	void SetRecordControls();

private:
	void SaveRecord();
	void GetRecordData();

private:
	CString m_strSystemDefaultPrinter;
};

/**********************************************************************/
#endif
/**********************************************************************/
