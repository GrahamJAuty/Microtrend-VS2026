#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "PMSInvoiceArray.h"
#include "PMSManualSalesEditDlg.h"
#include "PMSRoomStatusInfo.h"
/**********************************************************************/
#define PMS_MANUAL_SALES 0
#define PMS_MANUAL_PAYMENT 1
#define PMS_MANUAL_DEPOSIT 2
/**********************************************************************/

class CPMSManualSalesDatabaseDlg : public CDialog
{
public:
	CPMSManualSalesDatabaseDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, int nActionType, bool bTraining, CWnd* pParent = NULL );   
	~CPMSManualSalesDatabaseDlg();

	//{{AFX_DATA(CPMSManualSalesDatabaseDlg)
	enum { IDD = IDD_PMS_MANUAL_SALES_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSManualSalesDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPMSManualSalesDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnQuickSales();
	afx_msg void OnAddSales();
	afx_msg void OnAddRefunds();
	afx_msg void OnAddAllowances();
	afx_msg void OnAddPayments();
	afx_msg void OnAddSDisc();
	afx_msg void OnAddLDisc();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnProcessSalesItem ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	const char* GetDisplayLine( int nIndex );

private:
	HICON m_hIcon;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listSales;	

private:
	CString m_strDisplayLine;
	CString m_strTitle;
	
public:
	bool m_bSaveChanges;
	
private:
	CPMSManualSalesEditDlg* m_pManualSalesEditDlg;

private:
	CPMSInvoiceArray m_arraySalesInitial;
	CPMSInvoiceArray m_arraySalesCurrent;
	int m_nActionType;
	bool m_bTraining;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	int m_nRoomGridRoomIdx;
	int m_nRoomGridAccountIdx;
	CPMSRoomStatusInfo m_infoRoom;
	CPMSRoomAccountInfo m_infoAccount;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
