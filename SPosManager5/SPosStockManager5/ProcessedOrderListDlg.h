#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SingleListHelpersOrder.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CProcessedOrderListDlg : public CSSDialog
{
public:
	CProcessedOrderListDlg( bool bClosed, bool bDbOrders, CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL);
	virtual ~CProcessedOrderListDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectLocation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListSupplier(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListSupplier(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void ConfigureDialog() = 0;
	virtual void HandleButton1() = 0;
	virtual void HandleButton2() = 0;
	virtual void HandleButton3() = 0;
	virtual void HandleDoubleClick() = 0;
	
protected:
	enum { IDD = IDD_ORDER_LIST };
	CSSListCtrl m_listLocation;
	CSSListCtrlVirtual m_listSupplier;
	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_buttonCancel;

protected:
	void SelectLocationLine( int nLineIdx );
	
protected:
	void BuildSupplierList( int nLineIdx );
	void SelectSupplier( int nSuppIdx );
	int GetSelectedSuppIdx();
	int GetSupplierListSize();
	void SelectSupplierLine ( int nIndex );
	int GetSafeSupplierSelection();

protected:
	void UpdateOrderLines( int nSuppIdx );
	bool CanProcessOrders( int nSuppIdx, int nLineCount, CString& strError );
	void GetOrderInfo( int nIndex, CSupplierOrderInfo& info );

protected:
	bool PrepareLine( int nIndex, CString& strLine );

protected:
	bool ReadDatabaseFiles( CDataManagerInfo& info );

protected:
	bool m_bDbOrders;
	CArray<int,int>& m_arrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayLocation;
	bool m_bFiltered;

protected:
	int m_nLocLineIdx;
	CSingleListDisplayHelpers m_DisplayHelpers;
	CSingleListContentHelpersOrderList m_ContentHelpers;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
