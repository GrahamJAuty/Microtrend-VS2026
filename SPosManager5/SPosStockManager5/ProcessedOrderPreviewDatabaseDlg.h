#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "ProcessedOrderPreviewDatabase.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CProcessedOrderPreviewDatabaseDlg : public CDialog
{
public:
	CProcessedOrderPreviewDatabaseDlg( const char* szTitle, CProcessedOrderPreviewCSVArray& OrderArray, CLocationSelectorMiniLookup& LocSelMiniLookup, CWnd* pParent = NULL);   
	void SetPreviousOrderDetails( const char* szFilename, const char* szReference, bool bAllowSaveTemplate );

	//{{AFX_DATA(CProcessedOrderPreviewDatabaseDlg)
	enum { IDD = IDD_ORDER_PREVIEW_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderPreviewDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderPreviewDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeleteCurrent();
	afx_msg void OnDeleteRange();
	afx_msg void OnButtonAbandon();
	afx_msg void OnButtonContinue();
	afx_msg void OnEditOrder();
	afx_msg void OnAddOrder();
	afx_msg void OnPrintOrder();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	void EditOrder ( int nIndex );
	bool AssignSurplusStock();
	const char* GetApparentStockString( CProcessedOrderPreviewCSVRecord& OrderRecord, CStockCSVRecord& StockRecord );

	int GetDefaultStockpointForItem( int nStockIdx, CStockCSVRecord& StockRecord, int nSpNoPreferred );

	void HandleContinueWithTemplates();
	void HandleContinueWithoutTemplates();

	void CreateOrderTemplate( CString& strDataFilename, int& nLineCount );

private:
	bool m_bInitDialog;

private:
	HICON m_hIcon;
	CSSListCtrlVirtual m_listOrder;

private:
	CString m_strTitle;
	CStockFilterArray m_StockFilterArray;
	CProcessedOrderPreviewCSVArray& m_OrderArray;
	CLocationSelectorMiniLookup& m_LocSelMiniLookup;
	CArray<int,int> m_StockpointArray;
	int m_nDefaultSpIdx;

	CString m_strPreviousOrderFilename;
	CString m_strPreviousOrderReference;
	bool m_bAllowSaveTemplate;

private:
	CColumnManager m_ColumnManager;
	CString m_strApparentStock;
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

/**********************************************************************/
#endif
/**********************************************************************/
