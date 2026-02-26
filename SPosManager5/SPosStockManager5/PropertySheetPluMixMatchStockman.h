#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "PluChangesBuffer.h"
#include "..\CommonEcrStock\PropPagePluGeneral.h"
/**********************************************************************/

class CPropertySheetPluMixMatchStockman : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluMixMatchStockman)

public:
	CPropertySheetPluMixMatchStockman( int nPluIdx, CWnd* pParentWnd = NULL);
	
public:
	CPropPagePluGeneral m_Page1;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetPluMixMatchStockman)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPluMixMatchStockman();

public:
	void SaveRecord();
	
private:
	void UpdateTitle();
	
protected:
	//{{AFX_MSG(CPropertySheetPluMixMatchStockman)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nEditStock;
	bool m_bNewStock;
	CPluChangesBuffer m_PluChangesBuffer;
	CPluCSVRecord m_PluRecord;
	CRecipeCSVRecord m_RecipeRecord;
	CByteArray m_PageArray;
	int m_nPluIdx;
};

/**********************************************************************/
#endif	// __PROPERTYSHEETPLU_H__
/**********************************************************************/
