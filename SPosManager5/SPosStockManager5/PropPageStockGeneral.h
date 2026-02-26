#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
/**********************************************************************/

class CPropPageStockGeneral : public CSSPropertyPage
{
public:
	CPropPageStockGeneral();
	~CPropPageStockGeneral();

	//{{AFX_DATA(CPropPageStockGeneral)
	enum { IDD = IDD_PROPPAGE_STOCK_GENERAL };
	CEdit	m_editDescription;
	CEdit	m_editImportSheetCode;

	CSSComboBox	m_comboCategory;
	CButton	m_buttonCategory;

	CStatic m_staticCustomField[5];
	CSSComboBox m_comboCustomField[5];
	CButton m_buttonCustomField[5];

	CStatic m_staticStockUnit;
	CSSComboBox	m_comboStockUnit;
	CButton	m_buttonStockUnit;
	CStatic m_staticStockmate;
	CEdit m_editStockmate;

	CStatic m_staticSubUnit;
	CButton	m_buttonSubUnit;
	CStatic m_staticSubUnitX;
	CSSComboBox	m_comboSubUnit;

	CStatic m_staticWeight;
	CStatic m_staticWeightX;
	CSSComboBox	m_comboUnitWeight;
	CStatic m_staticSellBy;
	CSSComboBox	m_comboSaleWeight;

	CStatic m_staticWastage;

	CStatic m_staticSupplier;
	CEdit	m_editSupplier;

	CStatic m_staticCost;
	
	CButton	m_checkDisablePluRange;
	CButton	m_checkDisableAutoOrder;
	CButton	m_checkAutoLabel;

	int		m_nSubUnits;
	double	m_dWeight;
	double	m_dWastage;

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageStockGeneral)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageStockGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCategory();
	afx_msg void OnSelectCustomField1();
	afx_msg void OnSelectCustomField2();
	afx_msg void OnSelectCustomField3();
	afx_msg void OnSelectCustomField4();
	afx_msg void OnSelectCustomField5();
	afx_msg void OnButtonCustomField1();
	afx_msg void OnButtonCustomField2();
	afx_msg void OnButtonCustomField3();
	afx_msg void OnButtonCustomField4();
	afx_msg void OnButtonCustomField5();
	afx_msg void OnButtonStockUnit();
	afx_msg void OnButtonSubUnit();
	afx_msg void OnSelectUnitWeight();
	afx_msg void OnKillFocusWeight();
	afx_msg void OnKillFocusSubUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetCategoryFilter ( int nFilter ) { m_nCategoryFilter = nFilter; }
	void SetStockIdx ( int nStockIdx );
	void FillStockUnitCombo();
	void FillSubUnitCombo();
	void FillUnitWeightCombo();
	void FillCategoryCombo();
	void FillCustomFieldCombo( int nPos );
	void EditCustomFieldValues( int nPos );
	void SelectCustomField( int nPos );
	void Refresh();
	bool UpdateRecord();

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void UpdateControls();
	void UpdateWeightCombos( int nUnitWeightType, int nSaleWeightType );
	void GetWeightTypes( int& nUnitWeightType, int& nSaleWeightType );

private:
	CEdit* GetEditCost();
	CEdit* GetEditSubUnit();
	CEdit* GetEditWeight();
	CEdit* GetEditWastage();
			
private:
	int m_nStockIdx;
	CStockCSVRecord m_StockRecord;

private:
	int m_nCategoryFilter;
	CCategorySelector m_CategorySelector;

private:
	int m_nCustomFieldCount;
	int m_nCustomFieldNo[5];
	int m_nCustomFieldValueNo[5];
	WORD m_IDStaticCustomField[5];
	WORD m_IDComboCustomField[5];
	WORD m_IDButtonCustomField[5];
	
private:
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
