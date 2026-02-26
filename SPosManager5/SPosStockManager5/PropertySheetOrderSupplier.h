#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "PropPageStockSupplier.h"
#include "StockChangesBuffer.h"
/**********************************************************************/

class CPropertySheetOrderSupplier : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetOrderSupplier)

public:
	CPropertySheetOrderSupplier( int nStockIdx, int nStockSuppIdx, CWnd* pParentWnd = NULL);

public:
	CPropPageStockSupplier m_Page1;

public:
	//{{AFX_VIRTUAL(CPropertySheetOrderSupplier)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetOrderSupplier();

public:
	bool GetModifiedFlag() { return m_StockChangesBuffer.CheckModified(); }
	bool GetMakePreferredFlag() { return m_Page1.GetMakePreferredFlag(); }
	void UndoChanges() { m_StockChangesBuffer.UndoChanges(); }

private:
	void UpdateTitle();
	
protected:
	//{{AFX_MSG(CPropertySheetOrderSupplier)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStockChangesBuffer m_StockChangesBuffer;
	CStockCSVRecord m_StockRecord;
	CByteArray m_PageArray;

private:
	CString m_strSupplier1;
};

/**********************************************************************/
#endif	_
/**********************************************************************/

