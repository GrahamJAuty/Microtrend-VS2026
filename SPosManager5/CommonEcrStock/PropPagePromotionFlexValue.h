#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PromotionsCSVArray.h"
/**********************************************************************/

class CPropPagePromotionFlexValue : public CSSPropertyPage
{
public:
	CPropPagePromotionFlexValue();
	~CPropPagePromotionFlexValue();

	//{{AFX_DATA(CPropPagePromotionFlexValue)
	enum { IDD = IDD_PROPPAGE_PROMOTION_FLEX_VALUE };
	CStatic m_staticDiscount;
	CEdit m_editDescription;
	//}}AFX_DATA
	CEdit m_editFrom0;
	CEdit m_editTo4;
	CEdit m_editTo[5];
	double m_dFrom[5];
	double m_dPercent[5];
	
	//{{AFX_VIRTUAL(CPropPagePromotionFlexValue)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPagePromotionFlexValue)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void UpdateFromValue1();
	afx_msg void UpdateFromValue2();
	afx_msg void UpdateFromValue3();
	afx_msg void UpdateFromValue4();
	
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditFrom(int n);
	CEdit* GetEditPercent(int n);
	
public:
	void SetPromotionRecord ( CPromotionsCSVRecord* pPromoRecord ) { m_pPromoRecord = pPromoRecord; }
	void SetUpdatingTabsFlag( bool b ){ m_bUpdatingTabs = b; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void UpdateFromValue( int n );
	
private:
	CPromotionsCSVRecord* m_pPromoRecord;
	bool m_bUpdatingTabs;

private:
	WORD m_IDFrom[5];
	WORD m_IDTo[5];
	WORD m_IDPercent[5];
};

/**********************************************************************/
#endif
/**********************************************************************/

