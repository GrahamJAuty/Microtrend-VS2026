#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/

class CPropPageIDraughtOptionsLocation : public CSSPropertyPage
{
public:
	CPropPageIDraughtOptionsLocation();
	~CPropPageIDraughtOptionsLocation();

	//{{AFX_DATA(CPropPageIDraughtOptionsLocation)
	enum { IDD = IDD_PROPPAGE_IDRAUGHT_OPTIONS_LOCATION };
	//}}AFX_DATA
	CButton m_buttonFilter;

	//{{AFX_VIRTUAL(CPropPageIDraughtOptionsLocation)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonFilter();
	//}}AFX_VIRTUAL

private:
	BOOL PreTranslateMessage(MSG* pMsg);

public:
	CArray<int,int> m_arrayLineIndex;
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listLocations;

protected:
	//{{AFX_MSG(CPropPageIDraughtOptionsLocation)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ToggleFilter( bool bFilter );
	void MarkDuplicates( bool bRedrawList );

private:
	void SaveRecord();
	void GetRecordData();

private:
	bool m_bFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
