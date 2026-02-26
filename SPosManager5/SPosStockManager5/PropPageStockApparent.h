#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "StockMonitor.h"
/**********************************************************************/

class CPropPageStockApparent : public CSSPropertyPage
{
public:
	CPropPageStockApparent();
	~CPropPageStockApparent();

	//{{AFX_DATA(CPropPageStockApparent)
	enum { IDD = IDD_PROPPAGE_STOCK_APPARENT };
	
	
	CSSComboBoxEx	m_comboStockpoint;
	
	CStatic	m_staticOpeningText;
	CStatic	m_staticMinimumText;
	CStatic	m_staticReorderText;
	CStatic	m_staticMaximumText;

	CStatic	m_staticOpening;
	CStatic	m_staticMinimum;
	CStatic	m_staticReorder;
	CStatic	m_staticMaximum;

	CStatic	m_staticOpeningUnit;
	CStatic	m_staticMinimumUnit;
	CStatic	m_staticReorderUnit;
	CStatic	m_staticMaximumUnit;

	CButton	m_buttonEdit;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageStockApparent)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageStockApparent)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectStockpoint();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetStockIdx ( int nStockIdx );
	void SetDefaultSpIdx( int nSpIdx ){ m_nDefaultSpIdx = nSpIdx; }
	void SetSingleSpIdxFlag( bool b ){ m_bSingleSpIdx = b; }
	int GetApparentComboPos() { return m_nApparentComboPos; }
	void SetApparentComboPos( int nPos ) { m_nApparentComboPos = nPos; }
	void Refresh();
	void SetRecordControls();
	void SelectStockpoint();

public:
	void SetAllowEditFlag( bool b ){ m_bAllowEdit = b; }
	bool GetNeedSaveFlag(){ return m_bNeedSave; }
	
public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }

private:
	void DisplayLevels ( CStockLevelsCSVRecord& StockLevels );
	void AddStockLevelString( CStringArray& array, const char* szType, double dLevel );

private:
	CLocationSelector m_LocationSelector;
	CStockCSVRecord m_StockRecord;
	int m_nStockIdx;
	int m_nDefaultSpIdx;
	bool m_bSingleSpIdx;
	int m_nApparentComboPos;

private:
	CSSListCtrl m_listLevels;

private:
	bool m_bNeedSave;
	bool m_bAllowEdit;
	bool m_bReadOnly;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
