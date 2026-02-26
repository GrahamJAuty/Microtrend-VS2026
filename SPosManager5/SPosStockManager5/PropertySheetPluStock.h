#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "PluChangesBuffer.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonEcrStock\PropPagePluBarcode.h"
#include "..\CommonEcrStock\PropPagePluGeneral.h"
#include "..\CommonEcrStock\PropPagePluProgram.h"
#include "..\CommonEcrStock\PropPagePluInfo.h"
#include "..\CommonEcrStock\PropPagePluLink.h"
#include "..\CommonEcrStock\PropPagePluModifier.h"
#include "..\CommonEcrStock\PropPagePluUnitPrice.h"
#include "PropPageStockGeneral.h"
#include "PropPageStockSupplier.h"
#include "PropPageStockApparent.h"
/**********************************************************************/

struct CPropertySheetPluStockInfo
{
public:
	CPropertySheetPluStockInfo();

public:
	bool m_bReadOnly;
	bool m_bEnableSpin;
	bool m_bNewStock;
	bool m_bStockLevelTab;
	bool m_bApparentTab;
	bool m_bSingleSpIdx;
	int m_nStockIdx;
	int m_nDefaultSuppNo;
	int m_nSpIdxToEdit;
	WORD m_wSpinID;
	CPluFilterArray* m_pPluFilter;
};

/**********************************************************************/

class CPropertySheetPluStock : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluStock)

public:
	CPropertySheetPluStock( CPropertySheetPluStockInfo& info, CWnd* pParentWnd = NULL);
	void UndoChanges() { m_PluChangesBuffer.UndoChanges(); DataManager.ResourceWrapperPlu.UndoRecordChanges(); DataManager.ListKeyImageWrapperPlu.UndoRecordChanges(); }
	
public:
	CPropPagePluGeneral m_PagePluGeneral;
	CPropPagePluModifier m_PagePluPrice;
	CPropPagePluProgram m_PagePluProgram;
	CPropPagePluUnitPrice m_PagePluUnitPrice;
	CPropPagePluBarcode m_PagePluBarcode;
	CPropPagePluLink m_PagePluLink;
	CPropPagePluInfo m_PagePluInfo;
	CPropPageStockGeneral m_PageStockGeneral;
	CPropPageStockSupplier m_PageStockSupplier;
	CPropPageStockApparent m_PageStockApparent;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetPluStock)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPluStock();

public:
	void SavePluRecord();
	void SetStockIdx( int nStockIdx );
	int GetPluItemIndex() { return m_config.m_pPluFilter -> GetPluItemIndex(); }
	int GetLastSupplierTab() { return m_nLastSupplierTab; }
	
private:
	void UpdateTitle();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPluStock)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluChangesBuffer m_PluChangesBuffer;
	CPluCSVRecord m_PluRecord;
	CRecipeCSVRecord m_RecipeRecord;
	CArray<int,int> m_PageTypeArray;

private:
	int m_nLastSupplierTab;
	bool m_bGotModifierInfo;

private:
	CPropertySheetPluStockInfo& m_config;
};

/**********************************************************************/
#endif
/**********************************************************************/

