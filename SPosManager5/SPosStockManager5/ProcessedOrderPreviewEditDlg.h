#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

class CProcessedOrderPreviewEditDlg : public CDialog
{
public:
	CProcessedOrderPreviewEditDlg( CStockCSVRecord& StockRecord, CProcessedOrderPreviewCSVRecord& OrderRecord, CLocationSelectorMiniLookup& LocSelMiniLookup, CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CProcessedOrderPreviewEditDlg)
	enum { IDD = IDD_ORDER_PREVIEW_EDIT };
	CEdit	m_editSupplier;
	CEdit	m_editReference;
	CEdit	m_editDUItemSize;
	CEdit	m_editDUItems;
	CEdit	m_editDelUnit;
	CEdit	m_editCost;
	CButton	m_buttonEditSupplier;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderPreviewEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderPreviewEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnButtonEditSupplier();
	afx_msg void OnSelectSupplier(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickSupplier(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddStockpointList();
	void AddSupplierList();
	void SelectSupplier ( int nStockSuppIdx );

private:
	CProcessedOrderPreviewCSVRecord m_OrderPreviewRecordArray [ MAX_ITEM_SUPPLIERS ];
	CStockCSVRecord& m_StockRecord;
	CProcessedOrderPreviewCSVRecord& m_OrderRecord;

private:
	int m_nStockSuppIdx;
	int m_nSupplierCount;

private:
	CSSListCtrl m_listStockpoint;
	CSSListCtrl m_listSuppliers;
	CLocationSelectorMiniLookup& m_LocSelMiniLookup;
};

/**********************************************************************/
#endif
/**********************************************************************/
