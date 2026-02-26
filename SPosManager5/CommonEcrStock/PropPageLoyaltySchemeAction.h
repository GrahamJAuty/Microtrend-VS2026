#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LoyaltySchemeCSVArray.h"
#include "CascadingMacroSelector.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLoyaltySchemeAction : public CSSPropertyPage
{
public:
	CPropPageLoyaltySchemeAction();
	~CPropPageLoyaltySchemeAction();

	//{{AFX_DATA(CPropPageLoyaltySchemeAction)
	enum { IDD = IDD_PROPPAGE_LOYALTYSCHEME_ACTION };	
	CEdit	m_editName;
	CButton m_checkActive;
	CSSComboBox	m_comboActionType;
	CSSTabbedComboBox m_comboActionDetail;
	CButton m_buttonEdit;
	CButton m_buttonSelect;
	CSSListCtrlVirtual m_listPromotion;
	CStatic m_staticMacro;
	CEdit m_editMacro;
	CSSComboBox m_comboMacro;
	CStatic m_staticPence;
	CStatic m_staticPence2;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLoyaltySchemeAction)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLoyaltySchemeAction)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectType();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonSelect();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusMacro();
	afx_msg void OnSelectMacro();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPence();

public:
	void SetSchemeRecord ( CLoyaltySchemeCSVRecord* pSchemeRecord ) { m_pSchemeRecord = pSchemeRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void FillSelectionCombo();
	void ForceSelection( int nSelection );
	void EditPromo();
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
		
private:
	CLoyaltySchemeCSVRecord* m_pSchemeRecord;
	CCascadingMacroSelector m_MacroSelector;
	int m_nLastActionComboSel;
	int m_nMacroNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

