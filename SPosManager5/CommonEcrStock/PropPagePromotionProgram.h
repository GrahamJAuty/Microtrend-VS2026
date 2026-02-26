#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "PromotionsCSVArray.h"
/**********************************************************************/

class CPropPagePromotionProgram : public CSSPropertyPage
{
public:
	CPropPagePromotionProgram();
	~CPropPagePromotionProgram();

	//{{AFX_DATA(CPropPagePromotionProgram)
	enum { IDD = IDD_PROPPAGE_PROMOTION_PROGRAM };
	
	CEdit m_editDescription;
	CSSComboBox	m_comboPromoType;
	CSSComboBox	m_comboValueType;
	CSSComboBox	m_comboUsage;

	CStatic m_staticValue;
	CStatic m_staticPercent;
	CStatic m_staticTrip;

	CEdit m_editDeptNoEpos;
	CSSComboBox	m_comboDeptNameEpos;

	CSSComboBox	m_comboTimeplan;

	CButton m_checkServiceChargeVATFix;

	CButton	m_checkGratuity;
	CButton	m_checkTax;
	CButton m_checkMyPromoOnPromo;
	CButton m_checkPromoOnMyPromo;
	CButton m_checkReasons;
	CButton m_checkUpTo;
	
	CString m_strDescription;
	double	m_dPercent;
	double	m_dValue;
	double	m_dTrip;
	int m_nUseGate;

	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePromotionProgram)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditValue();
	CEdit* GetEditPercent();
	CEdit* GetEditTrip();
	CEdit* GetEditUseGate();

protected:
	//{{AFX_MSG(CPropPagePromotionProgram)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnKillFocusDeptNoEpos();
	afx_msg void OnSelectDepartmentEpos();
	afx_msg void OnSelectPromoType();
	afx_msg void OnToggleSCVATFix();
	afx_msg void OnToggleUpTo();
	
public:
	void SetPropertySheet( void* p ){ m_pPropertySheet = p; }
	void SetPromotionRecord ( CPromotionsCSVRecord* pPromoRecord ) { m_pPromoRecord = pPromoRecord; }
	void SetUpdatingTabsFlag( bool b ){ m_bUpdatingTabs = b; }
	void SetSubtotalOnlyFlag( bool b ){ m_bSubtotalOnly = b; }
	void Refresh();
	bool UpdateRecord();
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CDepartmentSelector m_DepartmentSelectorEpos;
	CPromotionsCSVRecord* m_pPromoRecord;

private:
	void* m_pPropertySheet;
	bool m_bUpdatingTabs;
	bool m_bSubtotalOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/

