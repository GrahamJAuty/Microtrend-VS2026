#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "PropPageStockGeneral.h"
#include "PropPageStockPluLink.h"
#include "PropPageStockSupplier.h"
#include "PropPageStockApparent.h"
#include "StockChangesBuffer.h"
#include "StockFilterArray.h"
/**********************************************************************/

struct CPropertySheetStockInfo
{
public:
	CPropertySheetStockInfo();

public:
	bool m_bReadOnly;
	bool m_bEnableSpin;
	bool m_bPluLinkTab;
	bool m_bStockLevelTab;
	bool m_bApparentTab;
	bool m_bSupplierEdit;
	bool m_bSingleSpIdx;
	int m_nDefaultSuppNo;
	int m_nApparentComboPos;
	int m_nSpIdxToEdit;
	WORD m_wSpinID;
	CStockFilterArray* m_pStockFilter;
};

/**********************************************************************/

class CPropertySheetStock : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetStock)

public:
	CPropertySheetStock( CPropertySheetStockInfo& info, CWnd* pParentWnd = NULL);
	
public:
	CPropPageStockGeneral m_PageStockGeneral;
	CPropPageStockPluLink m_PageStockPluLink;
	CPropPageStockSupplier m_PageStockSupplier;
	CPropPageStockApparent m_PageStockApparent;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetStock)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetStock();

public:
	int GetApparentComboPos() { return m_PageStockApparent.GetApparentComboPos(); }
	int GetStockItemIndex() { return m_config.m_pStockFilter -> GetStockItemIndex(); }
	void UndoChanges() { m_StockChangesBuffer.UndoChanges(); }
	int GetLastSupplierTab() { return m_nLastSupplierTab; }
	
	bool GetModifiedFlag(){ return ( ( m_bDatabaseIsModified ) || ( m_StockChangesBuffer.CheckModified() ) ); }
	bool GetChangeLevelFlag() { return m_PageStockApparent.GetNeedSaveFlag(); }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();
	
protected:
	//{{AFX_MSG(CPropertySheetStock)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStockChangesBuffer m_StockChangesBuffer;
	CStockCSVRecord m_StockRecord;
	CArray<int,int> m_PageTypeArray;

private:
	int m_nStockpointSelect;

private:
	int m_nLastSupplierTab;
	bool m_bDatabaseIsModified;

private:
	CPropertySheetStockInfo& m_config;
};

/**********************************************************************/
#endif	// __PROPERTYSHEETSTOCK_H__
/**********************************************************************/
