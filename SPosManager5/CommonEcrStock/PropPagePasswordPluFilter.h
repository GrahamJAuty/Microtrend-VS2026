#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStockTray\SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\Password.h"
/**********************************************************************/

class CPropPagePasswordPluFilter : public CPropertyPage
{
public:
	CPropPagePasswordPluFilter();
	~CPropPagePasswordPluFilter();

	//{{AFX_DATA(CPropPagePasswordPluFilter)
	enum { IDD = IDD_PROPPAGE_PASSWORD_PLUFILTER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePasswordPluFilter)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	//{{AFX_MSG(CPropPagePasswordPluFilter)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool GetBlockSpinButtonFlag() { return m_bBlockSpinButton; }

public:
	void SetPasswordRecord ( CPasswordCSVRecord* pPasswordRecord ) { m_pPasswordRecord = pPasswordRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void AddList();
	
private:
	CArray<CellTypeInfo, CellTypeInfo> m_arrayCellTypes;
	CellTypeInfoTextBuffer m_arrayTexts;
	CComboListCtrl m_listDatabases;

private:
	CPasswordCSVRecord* m_pPasswordRecord;
	bool m_bBlockSpinButton;
};

/**********************************************************************/
#endif
/**********************************************************************/
