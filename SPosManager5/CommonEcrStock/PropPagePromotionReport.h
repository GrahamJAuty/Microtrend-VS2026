#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "PromotionsCSVArray.h"
/**********************************************************************/

class CPropPagePromotionReport : public CSSPropertyPage
{
public:
	CPropPagePromotionReport();
	~CPropPagePromotionReport();

	//{{AFX_DATA(CPropPagePromotionReport)
	enum { IDD = IDD_PROPPAGE_PROMOTION_REPORT };
	//}}AFX_DATA

	CButton m_radioPlu1;
	CButton m_radioPlu2;
	CButton m_radioPlu3;
	CButton m_checkAlwaysTax;
	CEdit m_editPluNo;
	CEdit m_editDeptNoReport;
	CSSComboBox	m_comboDeptNameReport;
	CSSComboBox m_comboSalesQty;
	CStatic m_staticSage;

public:
	CEdit* GetEditSage();

	//{{AFX_VIRTUAL(CPropPagePromotionReport)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	virtual void OnRadioPlu1();
	virtual void OnRadioPlu2();
	virtual void OnRadioPlu3();

protected:
	//{{AFX_MSG(CPropPagePromotionReport)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnKillFocusDeptNoReport();
	afx_msg void OnSelectDepartmentReport();
	afx_msg void OnToggleAlwaysTax();

public:
	void SetPromotionRecord ( CPromotionsCSVRecord* pPromoRecord ) { m_pPromoRecord = pPromoRecord; }
	void SetUpdatingTabsFlag( bool b ){ m_bUpdatingTabs = b; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void SetRecordControls();
	void EnableSeparatePluSettings( bool b );
	void SetRadioPlu2Text();

private:
	CDepartmentSelector m_DepartmentSelectorReport;
	CPromotionsCSVRecord* m_pPromoRecord;
	bool m_bUpdatingTabs;
	int m_nReportAsPluType;
};

/**********************************************************************/
#endif
/**********************************************************************/

