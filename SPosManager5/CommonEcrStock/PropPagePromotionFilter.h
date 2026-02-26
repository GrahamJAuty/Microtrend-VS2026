#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "HexBitArray.h"
#include "PromotionsCSVArray.h"
/**********************************************************************/

class CPropPagePromotionFilter : public CSSPropertyPage
{
public:
	CPropPagePromotionFilter();
	~CPropPagePromotionFilter();

	//{{AFX_DATA(CPropPagePromotionFilter)
	enum { IDD = IDD_PROPPAGE_PROMOTION_FILTER };
	
	CEdit m_editDescription;

	CSSComboBox	m_comboFilterType;
	CStatic	m_staticFilter;
	CSSComboBox	m_comboFilterGroup;
	CButton	m_buttonFilterGroup;	
	CSSListTaggedSelectCtrl m_listCategory;
	CButton m_buttonAll;
	CButton m_buttonNone;
	CButton m_buttonFilter;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePromotionFilter)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonFilter();

protected:
	//{{AFX_MSG(CPropPagePromotionFilter)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSelectFilterType();
	afx_msg void OnButtonFilterGroup();
	
public:
	void SetPromotionRecord ( CPromotionsCSVRecord* pPromoRecord ) { m_pPromoRecord = pPromoRecord; }
	void SetUpdatingTabsFlag( bool b ){ m_bUpdatingTabs = b; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void FillAnalysisList();
	void UpdateHexBitArray();

private:
	CPromotionsCSVRecord* m_pPromoRecord;
	CHexBitArray m_HexBitArray;
	bool m_bFiltered;
	bool m_bUpdatingTabs;
};

/**********************************************************************/
#endif
/**********************************************************************/

