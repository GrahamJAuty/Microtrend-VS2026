#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "GroupSelector.h"
#include "KbdListSelector.h"
#include "PluChangesBuffer.h"
/**********************************************************************/

class CPropPagePluGeneral : public CSSPropertyPage
{
public:
	CPropPagePluGeneral();
	~CPropPagePluGeneral();

	//{{AFX_DATA(CPropPagePluGeneral)
	enum { IDD = IDD_PROPPAGE_PLU_GENERAL };

	CEdit m_editEposText;
	CEdit m_editRepText;
	CEdit m_editKeyText;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	/*****/
	CStatic m_staticPriceGroup;
	/*****/
	CStatic m_staticAllergenGroup;
	/*****/
	CEdit m_editAgressoGLCode;
	CEdit m_editAgressoCostCentre;
	CEdit m_editAgressoProject;
	/*****/
	CStatic	m_staticDate;
	CDateTimeCtrl m_DatePicker;
	CStatic	m_staticStatus;
	CSSComboBox	m_comboStatus;
	/*****/
	CSSTabbedComboBox m_comboCategory;
	CSSComboBox	m_comboPluType;
	CStatic m_staticProcess;
	CSSComboBox	m_comboProcess;
	CStatic m_staticActive;
	CSSComboBox m_comboActive;
	CButton m_checkAllowMod;
	/*****/
	CEdit m_editFirstQualifier;
	CEdit m_editFirstMenu;
	CEdit m_editFirstExtra;
	CSSComboBox	m_comboFirstQualifier;
	CSSComboBox	m_comboFirstMenu;
	CSSComboBox	m_comboFirstExtra;
	CStatic m_staticSortCode;
	CEdit m_editSortCode;
	/*****/
	CButton	m_buttonStock;
	CStatic	m_staticStock;
	CEdit m_editStock;
	/*****/
	CStatic m_staticTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CStatic m_staticPurchaseCost;
	CButton m_checkReportCost;
	CStatic	m_staticPoints;
	CStatic	m_staticStampWeight;
	/*****/
	CButton	m_radioGPType;
	CButton	m_radioGPFixed;
	CButton	m_radioGPPercent;
	/*****/
	CString	m_strEposText;
	CString	m_strRepText;
	CString m_strKeyText;
	/*****/
	CString m_strSortCode;
	/*****/
	CString m_strAgressoGLCode;
	CString m_strAgressoCostCentre;
	CString m_strAgressoProject;
	/*****/
	int	m_nPoints;
	int	m_nStampWeight;
	double m_dPurchaseCost;
	double m_dReportCost;
	/*****/
	int	m_nGPType;
	double m_dGPFixed;
	double m_dGPPercent;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePluGeneral)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }

#ifdef STOCKMAN_SYSTEM
	void SetRecipeRecord ( CRecipeCSVRecord* pRecipeRecord ) { m_pRecipeRecord = pRecipeRecord; }
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void SetPropertySheetPluStock ( void* pSheet ) { m_pPropertySheetPluStock = pSheet; }
	void SetNewStockFlag( bool bFlag ) { m_bNewStock = bFlag; }
	void SetSimpleEditFlag( bool bFlag ) { m_bSimpleEdit = bFlag; }
#endif
	
	void Refresh();
	bool UpdateRecord();
	void EditDepartments ( bool bFlag ) { m_bEditDepartments = bFlag; }

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void UpdatePrice1( int nTaxBand );
	void SetGPEnables();
	void CopyPrice( int n );

private:
	int GetStockIdxForCost();
	void UpdateStockCost();
	void UpdateStockDisplay();
	bool CanEditReportCost();
	bool CanEditPurchaseCost();

private:
	void FillTypeCombo();
	void FillActiveCombo();
	void FillProcessCombo();
	void FillCategoryCombo();
	void FillTaxCombo();
	int GetPluType();
	int GetExternalProcess();
	int GetAnalysisCategory();
	int GetTaxBand();

private:
	void SetPrice1GPFixed ( double dTaxRate );
	void SetPrice1GPPercent ( double dTaxRate );

protected:
	//{{AFX_MSG(CPropPagePluGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectAllergen1();
	afx_msg void OnKillFocusAllergen1();
	afx_msg void OnSelectAllergen2();
	afx_msg void OnKillFocusAllergen2();
	afx_msg void OnSelectAllergen3();
	afx_msg void OnKillFocusAllergen3();
	afx_msg void OnSelectAllergen4();
	afx_msg void OnKillFocusAllergen4();
	afx_msg void OnSelectAllergen5();
	afx_msg void OnKillFocusAllergen5();
	afx_msg void OnSelectAllergen6();
	afx_msg void OnKillFocusAllergen6();
	afx_msg void OnCopyPrice2();
	afx_msg void OnCopyPrice3();
	afx_msg void OnCopyPrice4();
	afx_msg void OnCopyPrice5();
	afx_msg void OnCopyPrice6();
	afx_msg void OnCopyPrice7();
	afx_msg void OnCopyPrice8();
	afx_msg void OnCopyPrice9();
	afx_msg void OnCopyPrice10();
	afx_msg void OnChangePluStatus();
	afx_msg void OnSelectType();
	afx_msg void OnKillFocusFirstMenu();
	afx_msg void OnSelectFirstMenu();
	afx_msg void OnKillFocusFirstExtra();
	afx_msg void OnKillFocusFirstQualifier();
	afx_msg void OnSelectFirstExtra();
	afx_msg void OnSelectFirstQualifier();
	afx_msg void OnButtonStock();
	afx_msg void OnKillFocusPurchaseCost();
	afx_msg void OnCalculateProfit();
	afx_msg void OnRadioGpFixed();
	afx_msg void OnRadioGpPercent();
	afx_msg void OnRadioGpType();
	afx_msg void OnToggleReportCost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SelectAllergen( CGroupSelector& AllergenSelector );
	void KillFocusAllergen( CGroupSelector& AllergenSelector );

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluCSVRecord* m_pPluRecord;

#ifdef STOCKMAN_SYSTEM
	CRecipeCSVRecord* m_pRecipeRecord;
	CPluChangesBuffer* m_pPluChangesBuffer;
	void* m_pPropertySheetPluStock;
	bool m_bNewStock;
	bool m_bSimpleEdit;
#endif

private:
	CEdit* GetEditPurchaseCost();
	CEdit* GetEditReportCost();
	CEdit* GetEditGPFixed();
	CEdit* GetEditGPPercent();
	CEdit* GetEditBasePrice( int nLevel );
	CEdit* GetEditPoints();
	CEdit* GetEditStampWeight();
	
private:
	int m_staticAllergenID[6];
	CStatic m_staticAllergen[6];
	int m_editAllergenID[6];
	CEdit m_editAllergen[6];
	int m_comboAllergenID[6];
	CSSComboBox m_comboAllergen[6];
	CGroupSelector m_AllergenSelector1;
	CGroupSelector m_AllergenSelector2;
	CGroupSelector m_AllergenSelector3;
	CGroupSelector m_AllergenSelector4;
	CGroupSelector m_AllergenSelector5;
	CGroupSelector m_AllergenSelector6;

private:
	int m_editBasePriceID[10];
	double m_dBasePrice[10];
	int m_staticBasePriceID[10];
	CStatic m_staticBasePrice[10];

private:
	CKbdListSelector m_KbdListSelectorMenu;
	CKbdListSelector m_KbdListSelectorExtra;
	CKbdListSelector m_KbdListSelectorQualifier;
	int m_nFirstMenu;
	int m_nFirstExtra;
	int m_nFirstQualifier;
	
private:
	bool m_bEditDepartments;
	int m_nDeptNo;
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
