#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DelUnitSelector.h"
//include "MyComboBox.h"
//include "MySSListCtrl.h"
#include "ProcessedOrderItem.h"
/**********************************************************************/
#define ORDER_EDIT_NORMAL 0
#define ORDER_EDIT_REPLACE 1
#define ORDER_EDIT_PART 2
#define ORDER_EDIT_COST 3
/**********************************************************************/

struct COrderSpIdxInfo
{
	int m_nSystemSpIdx;
	int m_nOrderSpIdx;
};

/**********************************************************************/

class CProcessedOrderEditDlg : public CSSDialog
{
public:
	CProcessedOrderEditDlg( CProcessedOrderItem& OrderItemNew, CProcessedOrderItem& OrderItemOld, int nStockIdx, CStockCSVRecord& StockRecord, int nDbIdx, int nMode, int nSuppNo, int nSingleSpIdx, CWnd* pParent = NULL);  
	bool GetChangedStockRecordFlag(){ return m_bChangedStockRecord; }

	//{{AFX_DATA(CProcessedOrderEditDlg)
	enum { IDD = IDD_ORDER_EDIT };
	CButton	m_buttonAuto;
	CEdit	m_editNewQty;
	CEdit	m_editNewCost;
	CEdit	m_editOldQty;
	CEdit	m_editOldCost;
	CEdit	m_editSuppRef;
	CButton	m_buttonOk;
	CButton	m_buttonEdit;
	CButton	m_groupExpected;
	CButton	m_groupActual;
	CSSComboBoxEx	m_comboDeliveryUnit;
	CStatic m_staticList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusDUCost();
	afx_msg void OnKillFocusDUQty();
	afx_msg void OnKillFocusDUYield();
	afx_msg void OnSelectDeliveryUnit();
	afx_msg void OnButtonEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateQtyAndCostDisplay();
	void UpdateDeliveryMethod();
	void AddList();
	void SetSUQty();
	
private:
	CEdit* GetEditDUItems();
	CEdit* GetEditDUItemSize();
	CEdit* GetEditDUQty();

private:
	CSSListCtrl m_listStockpoint;
	CProcessedOrderItem& m_OrderItemNew;
	CProcessedOrderItem& m_OrderItemOld;
	CStockCSVRecord& m_StockRecord;
	int m_nDbIdx;
	int m_nSingleSpIdx;
	bool m_bChangedStockRecord;

private:
	int m_nMode;
	int m_nStockIdx;
	int m_nDUItems;
	double m_dDUItemSize;
	double m_dDUCost;
	int m_nDUQty;
	int m_nSuppNo;

	CDelUnitSelector m_DelUnitSelector;
	CString m_strSuppRef;

	CArray<COrderSpIdxInfo,COrderSpIdxInfo> m_arrayOrderSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
