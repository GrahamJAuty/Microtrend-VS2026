#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "StockFilterArray.h"
#include "ProcessedOrderArray.h"
#include "ProcessedOrderEntity.h"
#include "ProcessedOrderFilterArray.h"
#include "ProcessedOrderItem.h"
#include "afxcmn.h"
#include "afxwin.h"
/**********************************************************************/
#define PROCESSMODE_EDIT 0
#define PROCESSMODE_DELIVER 1
#define PROCESSMODE_NEWORDER 2
#define PROCESSMODE_COSTPRICE 3
/**********************************************************************/

class CProcessedOrderDatabaseDlg : public CDialog
{
public:
	CProcessedOrderDatabaseDlg( CProcessedOrderArray& OrderArray, int nMode, int nSuppIdx, int nDbIdx, int nSingleSpIdx, const char* szReference, CWnd* pParent );   

	//{{AFX_DATA(CProcessedOrderDatabaseDlg)
	enum { IDD = IDD_ORDER_DATABASE };
	CButton	m_buttonReplace;
	CButton	m_buttonUndeliverOrder;
	CButton	m_buttonUndeliverItem;
	CButton	m_buttonDeliverPart;
	CButton	m_buttonDeliverAll;
	CButton	m_buttonDeliver;
	CSSComboBox	m_comboOrderNum;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditOrder();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteItem();
	afx_msg void OnAddOrder();
	afx_msg void OnDeliverItem();
	afx_msg void OnPrintOrder();
	afx_msg void OnDeleteOrder();
	afx_msg void OnCancel();
	afx_msg void OnEditStockitem();
	afx_msg void OnDeliverOrder();
	afx_msg void OnSelectOrderNumber();
	afx_msg void OnDeliverPart();
	afx_msg void OnDeleteAllOrders();
	afx_msg void OnPrintPurchaseOrder();
	afx_msg void OnButtonUndeliverItem();
	afx_msg void OnButtonUndeliverOrder();
	afx_msg void OnEmailPurchaseOrder();
	afx_msg void HandleDoubleClick();
	afx_msg void OnButtonReplace();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnFilterOrders();
	afx_msg void OnNoFilterOrders();
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void SetStockItemIndexForOrderLines();
	void SaveChangedStockRecord( CStockCSVRecord& StockRecord );
	int SaveChangedStockRecordInternal( CStockCSVRecord& StockRecord, CDataManagerInfo& infoDM );
	int EditStockItemInternal( int nItemIdx, CDataManagerInfo& infoDM );
	void FillOrderNumCombo();
	void AuditDeliveries();
	void AuditOrder( COleDateTime date, CProcessedOrderItem& OrderItem, int nOrderNum );
	void DisplayScreen ( int nIndex );
	void BuildRecordList();
	void CreatePurchaseOrder ( bool bEmail );
	bool ToggleDelivery ( int nHeaderIdx, int nItemIdx );
	void PartialDelivery( int nMode );
	bool CanDeliver ( int nOrderIdx );

	void AuditDeliveriesInternal( COleDateTime date );

	void UpdateDeleteMenu();

private:
	bool GetSelectedHeader( CProcessedOrderHeader& HeaderRecord, CProcessedOrderEntity& HeaderEntity );

private:
	bool m_bInitDialog;

private:
	CString m_strReference;
	CString m_strAuditFile;
	CString m_strTempFile;
	HICON m_hIcon;
	CSSListCtrlVirtual m_listOrder;
	int m_nDbIdx;
	int m_nSelectedSuppIdx;
	int m_nSelectedSuppNo;

private:
	bool m_bWrittenHeader;
	CWordArray m_arrayWrittenHeader;

private:
	CMenuChanger m_MenuChanger;
	CStockFilterArray m_StockFilterArray;
	int m_nMode;
	int m_nSingleSpIdx;
	int m_nNewOrderNo;

private:
	CArray<CProcessedOrderEntity,CProcessedOrderEntity> m_arrayHeaderEntities;
	CProcessedOrderEntity m_SelectedHeaderEntity;

private:
	CProcessedOrderArray& m_OrderArray;
	CProcessedOrderFilterArray m_OrderFilterArray;
	CProcessedOrderFilterArray m_PreviousOrderFilterArray;
	CLabelListCSVArray m_arrayLabels;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

/**********************************************************************/
#endif
/**********************************************************************/
