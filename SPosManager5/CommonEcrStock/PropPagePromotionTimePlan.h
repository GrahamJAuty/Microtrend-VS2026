#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ComboListCtrl.h"
#include "PromotionsCSVArray.h"
#include "TimePlanTemplateListManager.h"
/**********************************************************************/

class CPropPagePromotionTimePlan : public CSSPropertyPage
{
public:
	CPropPagePromotionTimePlan();
	~CPropPagePromotionTimePlan();

	//{{AFX_DATA(CPropPagePromotionTimePlan)
	enum { IDD = IDD_PROPPAGE_PROMOTION_TIMEPLAN };
	CEdit m_editDescription;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePromotionTimePlan)
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
	
protected:
	//{{AFX_MSG(CPropPagePromotionTimePlan)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	void SetPromotionRecord ( CPromotionsCSVRecord* pPromoRecord ) { m_pPromoRecord = pPromoRecord; }
	void Refresh();
	bool UpdateRecord();

public:
	void SetUpdatingTabsFlag(bool b) { m_bUpdatingTabs = b; }

private:
	void SaveRecord();
	void SetRecordControls();

private:
	CTimePlanTemplateListManager m_ListManager;
	CComboListCtrl m_listSettings;

private:
	CPromotionsCSVRecord* m_pPromoRecord = nullptr;
	bool m_bUpdatingTabs = FALSE;
	bool m_bFiltered = FALSE;
};

/**********************************************************************/
#endif
/**********************************************************************/

