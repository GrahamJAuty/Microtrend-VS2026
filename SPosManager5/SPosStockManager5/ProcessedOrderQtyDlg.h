#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderQtyDlg : public CSSDialog
{
public:
	CProcessedOrderQtyDlg( int nSpIdx, int nType, int nDUItems, double dDUItemSize, CWnd* pParent = NULL);   
	void SetDUItemOrder( int nOrder );

	//{{AFX_DATA(CProcessedOrderQtyDlg)
	enum { IDD = IDD_ORDER_QTY };
	CStatic	m_staticSupplierItems;
	CStatic	m_staticDelUnits;
	double	m_dStockUnits;
	int		m_nSupplierItems;
	int		m_nDelUnits;
	int		m_nType;
	//}}AFX_DATA
	CButton	m_radioDelUnits;
	CButton	m_radioSupplierItems;
	CButton	m_radioStockUnits;

	//{{AFX_VIRTUAL(CProcessedOrderQtyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderQtyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSupplierItems();
	afx_msg void OnRadioStockUnits();
	afx_msg void OnRadioDelUnits();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditStockUnits();
	CEdit* GetEditSupplierItems();
	CEdit* GetEditDelUnits();
	
private:
	int m_nSpIdx;
	int m_nDUItems;
	double m_dDUItemSize;
};

/**********************************************************************/
#endif
/**********************************************************************/

