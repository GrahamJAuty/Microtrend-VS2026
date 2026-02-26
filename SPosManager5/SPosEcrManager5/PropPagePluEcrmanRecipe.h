#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "Consolidation.h"
#include "EditEnter.h"
#include "PluChangesBuffer.h"
#include "PluCSVArray.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "afxwin.h"
/**********************************************************************/

class CPropPagePluEcrmanRecipe : public CSSPropertyPage
{
public:
	CPropPagePluEcrmanRecipe();
	~CPropPagePluEcrmanRecipe();
	//{{AFX_DATA(CPropPagePluEcrmanRecipe)
	enum { IDD = IDD_PROPPAGE_PLU_ECRMAN_RECIPE };
	//}}AFX_DATA
	CEditEnter m_editPluNo;
	CButton m_buttonBrowse;
	CButton m_buttonAdd;
	CButton m_buttonRemove;
	CButton m_buttonScan;

	//{{AFX_VIRTUAL(CPropPagePluEcrmanRecipe)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

protected:
	//{{AFX_MSG(CPropPagePluEcrmanRecipe)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusPlu();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonScan();
	afx_msg long OnAddPluMessage ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnAddScannedPlu ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnSetDefaultFocus ( WPARAM wIndex, LPARAM lParam );
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	void AddList();
	void AddRecipeItem( __int64 nPluNo );
	void AddRecipeListLine( int nPos, CEcrmanRecipeItem& infoItem );
	void RenumberListLines();

public:
	void SetPluChangesBuffer( CPluChangesBuffer* pBuffer ) { m_pPluChangesBuffer = pBuffer; }
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	bool IsInContextEdit(){ return m_listRecipeItems.GetInEditFlag(); }
	void UpdateRecipeItemArray();

public:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void Refresh();
	bool UpdateRecord();

private:
	void AddScannedPlu();
	
private:
	CPluFilterArray m_FilterArray;

private:
	CComboListCtrl m_listRecipeItems;
	CReportConsolidationArray<CEcrmanRecipeItem> m_arrayRecipeItems;
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
		
private:
	CPluCSVRecord* m_pPluRecord;
	CPluChangesBuffer* m_pPluChangesBuffer;
	bool m_bReadOnly;	
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
