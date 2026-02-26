#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "KbdListSelector.h"
#include "PluCSVArray.h"
/**********************************************************************/

class CPropPagePluModifier : public CSSPropertyPage
{
public:
	CPropPagePluModifier();
	~CPropPagePluModifier();

	//{{AFX_DATA(CPropPagePluModifier)
	enum { IDD = IDD_PROPPAGE_PLU_MODIFIER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePluModifier)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }
	
protected:
	//{{AFX_MSG(CPropPagePluModifier)
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleModifier();
	afx_msg void OnToggleEposText();
	afx_msg void OnToggleKeyText();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectFirstMenu();
	afx_msg void OnKillFocusFirstMenu();
	afx_msg void OnSelectFirstExtra();
	afx_msg void OnKillFocusFirstExtra();
	afx_msg void OnSelectFirstQualifier();
	afx_msg void OnKillFocusFirstQualifier();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCopyBasePrice2();
	afx_msg void OnCopyBasePrice3();
	afx_msg void OnCopyBasePrice4();
	afx_msg void OnCopyBasePrice5();
	afx_msg void OnCopyBasePrice6();
	afx_msg void OnCopyBasePrice7();
	afx_msg void OnCopyBasePrice8();
	afx_msg void OnCopyBasePrice9();
	afx_msg void OnCopyBasePrice10();
	afx_msg void OnCopyModPrice2();
	afx_msg void OnCopyModPrice3();
	afx_msg void OnCopyModPrice4();
	afx_msg void OnCopyModPrice5();
	afx_msg void OnCopyModPrice6();
	afx_msg void OnCopyModPrice7();
	afx_msg void OnCopyModPrice8();
	afx_msg void OnCopyModPrice9();
	afx_msg void OnCopyModPrice10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPluRecord ( CPluCSVRecord* pPluRecord ) { m_pPluRecord = pPluRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void FillDepartmentCombo();
	void FillOPGroupCombo();
	void FillTaxCombo();
	void FillCategoryCombo();
	void FillPluTypeCombo();

private:
	void InitialisePriceSetEnables();
	void UpdateModifierEnable();
	void UpdateModifierControls();
	void SaveTabChanges();
	void CopyBasePrice(int p);
	void CopyModPrice(int p);

private:
	CEdit* GetEditBasePrice( int nLevel );
	CEdit* GetEditModPrice( int nLevel );

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluCSVRecord* m_pPluRecord;
	CPluCSVRecord m_PluBuffer;

private:
	int m_staticBasePriceID[10];
	int m_editBasePriceID[10];
	int m_editModPriceID[10];
	int m_staticModPriceID[10];
	
private:
	CEdit m_editBaseEposText;
	CEdit m_editBaseKeyText;
	CStatic m_staticBasePrice[10];
	CEdit* m_pEditBasePrice[10];
	CTabCtrl m_TabModifiers;
	CButton m_checkModEnable;
	CButton m_checkModEposText;
	CEdit m_editModEposText;
	CButton m_checkModKeyText;
	CEdit m_editModKeyText;
	CStatic m_staticModPrice[10];
	CEdit* m_pEditModPrice[10];
	CStatic m_staticDept;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CStatic m_staticOPGroup;
	CSSTabbedComboBox m_comboOPGroup;
	CStatic m_staticTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CStatic m_staticCategory;
	CSSTabbedComboBox m_comboCategory;
	CStatic m_staticPluType;
	CSSTabbedComboBox m_comboPluType;
	CStatic m_staticFirstMenu;
	CStatic m_staticFirstQualifier;
	CStatic m_staticFirstExtra;
	CEdit m_editFirstMenu;
	CEdit m_editFirstQualifier;
	CEdit m_editFirstExtra;
	CSSComboBox	m_comboFirstMenu;
	CSSComboBox	m_comboFirstQualifier;
	CSSComboBox	m_comboFirstExtra;
	
private:
	bool m_bPriceSetBaseEposTextEdit;
	bool m_bPriceSetBaseKeyTextEdit;
	bool m_bPriceSetBasePrice[10];
	bool m_bPriceSetModEnable;
	bool m_bPriceSetModEposTextTick;
	bool m_bPriceSetModEposTextEdit;
	bool m_bPriceSetModKeyTextTick;
	bool m_bPriceSetModKeyTextEdit;
	bool m_bPriceSetModPrice[10];
	bool m_bPriceSetModDept;
	bool m_bPriceSetModOPG;
	bool m_bPriceSetModTaxBand;
	bool m_bPriceSetModCategory;
	bool m_bPriceSetModPluType;
	bool m_bPriceSetModFirstMenu;
	bool m_bPriceSetModFirstExtra;
	bool m_bPriceSetModFirstQualifier;

private:
	CKbdListSelector m_KbdListSelectorMenu;
	CKbdListSelector m_KbdListSelectorExtra;
	CKbdListSelector m_KbdListSelectorQualifier;
	int m_nFirstMenu;
	int m_nFirstExtra;
	int m_nFirstQualifier;
	
private:
	int m_nActiveTab;
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
