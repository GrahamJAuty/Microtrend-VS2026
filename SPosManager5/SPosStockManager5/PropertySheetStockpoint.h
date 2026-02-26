#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "PropPageStockpointGeneral.h"
#include "PropPageStockpointTransfers.h"
#include "StockpointCSVArray.h"
/**********************************************************************/

class CPropertySheetStockpoint : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetStockpoint)

public:
	CPropertySheetStockpoint( int nSpIdx, bool bAdd, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetStockpoint();
	
public:
	//{{AFX_VIRTUAL(CPropertySheetStockpoint)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetStockpoint)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveRecord();

public:
	CPropPageStockpointGeneral m_Page1;
	CPropPageStockpointTransfers m_Page2;
	
private:
	int m_nSpIdx;
	bool m_bAdd;

	CAddressInfo m_infoAddress;
	CStockpointCSVArray m_StockpointCSVArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
