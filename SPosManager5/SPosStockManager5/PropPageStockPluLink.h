#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockChangesBuffer.h"
/**********************************************************************/

class CPropPageStockPluLink : public CPropertyPage
{
public:
	CPropPageStockPluLink();
	~CPropPageStockPluLink();

	//{{AFX_DATA(CPropPageStockPluLink)
	enum { IDD = IDD_PROPPAGE_STOCK_PLULINK };
	CButton	m_buttonRemove;
	CButton	m_buttonEditRecipe;
	CButton	m_buttonEdit;
	CButton	m_buttonAdd;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageStockPluLink)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageStockPluLink)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEditRecipe();
	afx_msg void OnDoubleClickListRecipe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonGP();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetStockChangesBuffer ( CStockChangesBuffer* pBuffer ) { m_pStockChangesBuffer = pBuffer; }
	void SetStockIdx ( int nStockIdx );
	void Refresh();
	bool UpdateRecord();
	void AddList( __int64 nPluNo );
	void AddListRecipe();

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CStockChangesBuffer* m_pStockChangesBuffer;
	int m_nStockIdx;
	CStockCSVRecord m_StockRecord;
	
private:
	CSSListCtrl m_listRedirections;
	CSSListCtrl m_listRecipes;
	CDWordArray m_IndexArray;
	CDWordArray m_IndexArrayRecipe;

private:
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/

