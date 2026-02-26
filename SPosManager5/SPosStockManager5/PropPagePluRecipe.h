#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluChangesBuffer.h"
/**********************************************************************/
#define RECIPE_LISTITEM_INFO 0
#define RECIPE_LISTITEM_BLANK 1
#define RECIPE_LISTITEM_INDEX 2
/**********************************************************************/

struct CRecipeListItem
{
public:
	int m_nType;
	int m_nIndex;
};

/**********************************************************************/

class CPropPagePluRecipe : public CPropertyPage
{
public:
	CPropPagePluRecipe();
	~CPropPagePluRecipe();

	//{{AFX_DATA(CPropPagePluRecipe)
	enum { IDD = IDD_PROPPAGE_PLU_RECIPE };
	CButton	m_buttonAdd;
	CButton	m_buttonEditQuantity;
	CButton	m_buttonEditSettings;
	CButton	m_buttonRemove;
	CButton	m_buttonStock;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePluRecipe)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetEditMode( int nMode ) { m_nEditMode = nMode; }
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void SetRecipeRecord ( CRecipeCSVRecord* pRecipeRecord ) { m_pRecipeRecord = pRecipeRecord; }
	void Refresh();
	void AddList();
	bool UpdateRecord();
	void SetButtons();

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }
	void SetApparentTabFlag( bool b ){ m_bApparentTab = b; }
	void SetStockLevelTabFlag( bool b ){ m_bStockLevelTab = b; }
	
private:
	void SaveRecord();
	void GetRecordData();
	bool CreateStockItem ( const char* szCode );
	void SelectRecipeItem( int nIndex );
	void SelectListItem( CRecipeListItem& Target );
	void GetSelectedItem( CRecipeListItem& Item );

protected:
	//{{AFX_MSG(CPropPagePluRecipe)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEditQuantity();
	afx_msg void OnButtonEditSettings();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonStock();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSSListCtrl m_listItems;

private:
	int m_nEditMode;
	CPluChangesBuffer* m_pPluChangesBuffer;
	CPluCSVRecord* m_pPluRecord;
	CRecipeCSVRecord* m_pRecipeRecord;
	CArray<CRecipeListItem,CRecipeListItem> m_arrayListItems;

private:
	bool m_bReadOnly;
	bool m_bApparentTab;
	bool m_bStockLevelTab;
};

/**********************************************************************/
#endif
/**********************************************************************/