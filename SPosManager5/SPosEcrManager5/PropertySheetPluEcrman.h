#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PluChangesBuffer.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonEcrStock\PropPagePluBarcode.h"
#include "PropPagePluEcrmanRecipe.h"
#include "..\CommonEcrStock\PropPagePluGeneral.h"
#include "..\CommonEcrStock\PropPagePluInfo.h"
#include "..\CommonEcrStock\PropPagePluLink.h"
#include "..\CommonEcrStock\PropPagePluModifier.h"
#include "..\CommonEcrStock\PropPagePluProgram.h"
#include "PropPagePluStock.h"
#include "..\CommonEcrStock\PropPagePluUnitPrice.h"
/**********************************************************************/

class CPropertySheetPluEcrman : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluEcrman)

public:
	CPropertySheetPluEcrman( WORD wSpinID, CPluFilterArray* pPluFilterArray, bool bReadOnly, CWnd* pParentWnd = NULL);
	void UndoChanges();

public:
	CPropPagePluGeneral m_PagePluGeneral;
	CPropPagePluModifier m_PagePluPrice;
	CPropPagePluProgram m_PagePluProgram;
	CPropPagePluUnitPrice m_PagePluUnitPrice;
	CPropPagePluBarcode m_PagePluBarcode;
	CPropPagePluStock m_PagePluStock;
	CPropPagePluEcrmanRecipe m_PagePluRecipe;
	CPropPagePluLink m_PagePluLink;
	CPropPagePluInfo m_PagePluInfo;

public:
	//{{AFX_VIRTUAL(CPropertySheetPluEcrman)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPluEcrman();

public:
	void SaveRecord();
	int GetIndex() { return m_pPluFilterArray -> GetPluItemIndex(); }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPluEcrman)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluChangesBuffer m_PluChangesBuffer;
	CPluFilterArray* m_pPluFilterArray;
	CStringArray m_arrayPluAuditMemory;
	CPluCSVRecord m_PluRecord;
	CArray<int,int> m_PageTypeArray;
	int m_nPreviousPriceSetStatus;
	bool m_bGotModifierInfo;
	bool m_bReadOnly;
};

/**********************************************************************/
#endif	
/**********************************************************************/
#endif
/**********************************************************************/
