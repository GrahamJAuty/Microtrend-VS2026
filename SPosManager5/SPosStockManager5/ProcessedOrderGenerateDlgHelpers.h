#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CProcessedOrderGenerateDlgHelpers
{
public:
	CProcessedOrderGenerateDlgHelpers(void);
	
public:
	void HandleInitDialog( CWnd* pWnd, const char* szTitle );
	void HandleSelectDatabase();
	void FillCategoryCombo();
	void FillSupplierCombo();
	void BuildSupplierArray();
	void SelectSupplierSet();
	void AddSupplierList( CByteArray& SupplierFlags );
	void SelectSuppliersAll();
	void SelectSuppliersNone();
	void GenerateOrder( int nMethod, CProcessedOrderPreviewCSVArray& OrderArray );

public:
	int GetDbIdx(){ return m_nDbIdx; }
	void OpenDatabaseReadOnly();
	bool OpenDatabaseReadWrite( CDataManagerInfo& infoDM );
	void OpenStockLevels();
	void UpdateDatabaseControls();

private:
	CWordArray* m_pSuppSelectArray;
	CCategorySelector* m_pCategorySelector;
	CLocationSelector* m_pLocationSelector;

private:
	CSSListCtrl* m_pListDatabase;
	CSSComboBox* m_pComboSupplier;
	CSSListMultiSelectCtrl* m_pListSupplier;
	CSSComboBox* m_pComboCategory;
	CSSComboBoxEx* m_pComboConLevel;
	
	CArray<int,int>* m_pArrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity>* m_pArrayLocation;
	int m_nDbIdx;

	friend class CProcessedOrderGenerateStockDlg;
	friend class CProcessedOrderGenerateSalesDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
