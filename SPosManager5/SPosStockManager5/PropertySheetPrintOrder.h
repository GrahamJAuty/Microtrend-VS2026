#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderDetails.h"
#include "PropPagePrintOrderGeneral.h"
#include "PropPagePrintOrderAddress.h"
/**********************************************************************/

class CPropertySheetPrintOrder : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPrintOrder)

public:
	CPropertySheetPrintOrder( bool bEmail, CProcessedOrderDetails& OrderDetails, int nDefSpIdx, CWnd* pParentWnd = NULL);

public:
	CPropPagePrintOrderAddress m_Page1;
	CPropPagePrintOrderGeneral m_Page2;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetPrintOrder)
	protected:
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPrintOrder();
	
protected:
	//{{AFX_MSG(CPropertySheetPrintOrder)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CButton m_editButton;

private:
	CProcessedOrderDetails& m_OrderDetails;
	bool m_bEmail;
	int m_nDefSpIdx;
};

/**********************************************************************/
#endif	
/**********************************************************************/
