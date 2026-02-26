#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SingleListHelpersOrder.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CProcessedOrderRepeatDlg : public CDialog
{
public:
	CProcessedOrderRepeatDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL);
	virtual ~CProcessedOrderRepeatDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListOrder(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListOrder(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	
protected:
	enum { IDD = IDD_ORDER_REPEAT };
	CSSListCtrl m_listDatabase;
	CSSListCtrlVirtual m_listOrders;
	CButton m_button1;
	CButton m_button2;
	CButton m_button3;

private:
	void SelectDatabaseLine( int nLineIdx );
	void SelectLineByDate( CRepeatOrderInfo& info );
	
private:
	int GetOrderListSize();
	void BuildOrderList( int nLineIdx );
	void SelectOrderLine( int nIndex );
	int GetSafeOrderSelection();

private:
	void Button1Internal();
	bool RecreateOrder( CProcessedOrderPreviewCSVArray& OrderPreview, CRepeatOrderInfo& info );
	bool RecreateOrderInternal( CProcessedOrderPreviewCSVArray& OrderPreview, CReportFile& ReportFile, CRepeatOrderInfo& info );
	
private:
	CArray<int,int>& m_arrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayLocation;

private:
	int m_nDbLineIdx;
	CSingleListDisplayHelpers m_DisplayHelpers;
	CSingleListContentHelpersOrderRepeat m_ContentHelpers;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
