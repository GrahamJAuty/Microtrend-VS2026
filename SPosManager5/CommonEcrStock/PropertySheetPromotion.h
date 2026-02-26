#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PromotionsCSVArray.h"
#include "PropPagePromotionProgram.h"
#include "PropPagePromotionFilter.h"
#include "PropPagePromotionFlexCovers.h"
#include "PropPagePromotionFlexValue.h"
#include "PropPagePromotionReport.h"
#include "PropPagePromotionTimePlan.h"
/**********************************************************************/

class CPropertySheetPromotion : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPromotion)

public:
	CPropertySheetPromotion( WORD wSpinID, bool bAllowSpin, int nPromoIdx, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPromotion();

public:
	void SetSubtotalOnlyFlag( bool b ){ m_Page1.SetSubtotalOnlyFlag(b); }

public:
	CPropPagePromotionProgram m_Page1;
	CPropPagePromotionFilter m_Page2;
	CPropPagePromotionFlexValue m_Page3;
	CPropPagePromotionFlexCovers m_Page4;
	CPropPagePromotionReport m_Page5;
	CPropPagePromotionTimePlan m_PageTimePlan;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetPromotion)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	int GetIndex() { return m_nPromoIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }
	void AddFlexTabs();

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPromotion)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nPromoIdx = 0;
	CPromotionsCSVRecord m_PromoRecord;
	CByteArray m_PageArray;

private:
	CListDataDlg* m_pListDataDlg = nullptr;
	bool m_bShowTimePlanPage = FALSE;
};

/**********************************************************************/
#endif
/**********************************************************************/
