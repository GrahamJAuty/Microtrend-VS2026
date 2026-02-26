#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "PluChangesBuffer.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonEcrStock\PropPagePluBarcode.h"
#include "..\CommonEcrStock\PropPagePluGeneral.h"
#include "..\CommonEcrStock\PropPagePluInfo.h"
#include "..\CommonEcrStock\PropPagePluLink.h"
#include "..\CommonEcrStock\PropPagePluModifier.h"
#include "..\CommonEcrStock\PropPagePluProgram.h"
#include "..\CommonEcrStock\PropPagePluUnitPrice.h"
#include "PropPagePluRecipe.h"
/**********************************************************************/

struct CPropertySheetPluStockmanInfo
{
public:
	CPropertySheetPluStockmanInfo();

public:
	bool m_bEnableSpin;
	bool m_bNewStock;
	bool m_bReadOnly;
	bool m_bApparentTab;
	bool m_bStockLevelTab;
	int m_nEditMode;
	WORD m_wSpinID;
	CPluFilterArray* m_pPluFilter;
};

/**********************************************************************/

class CPropertySheetPluStockman : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluStockman)

public:
	CPropertySheetPluStockman( CPropertySheetPluStockmanInfo& info, CWnd* pParentWnd = NULL);
	void UndoChanges();

public:
	CPropPagePluGeneral m_PagePluGeneral;
	CPropPagePluModifier m_PagePluPrice;
	CPropPagePluProgram m_PagePluProgram;
	CPropPagePluUnitPrice m_PagePluUnitPrice;
	CPropPagePluBarcode m_PagePluBarcode;
	CPropPagePluLink m_PagePluLink;
	CPropPagePluInfo m_PagePluInfo;
	CPropPagePluRecipe m_PagePluRecipe;

public:
	//{{AFX_VIRTUAL(CPropertySheetPluStockman)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPluStockman();

public:
	void SaveRecord();
	int GetPluItemIndex() { return m_config.m_pPluFilter -> GetPluItemIndex(); }
	
private:
	void UpdateTitle();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPluStockman)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluChangesBuffer m_PluChangesBuffer;
	CPluCSVRecord m_PluRecord;
	CRecipeCSVRecord m_RecipeRecord;
	CArray<int,int> m_PageTypeArray;
	bool m_bGotModifierInfo;

private:
	int m_nPreviousPriceSetStatus;
	CPropertySheetPluStockmanInfo& m_config;
};

/**********************************************************************/
#endif
/**********************************************************************/

