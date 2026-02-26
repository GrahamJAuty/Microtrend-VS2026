#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
//include "MySSListCtrl.h"
#include "StockCodeSearch.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockDeleteDatabaseDlg : public CDialog
{
public:
	CStockDeleteDatabaseDlg( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales, CWnd* pParent = NULL );   
	CStockDeleteDatabaseDlg( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales, CWnd* pParent = NULL );   

	//{{AFX_DATA(CStockDeleteDatabaseDlg)
	enum { IDD = IDD_STOCK_DELETE_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeleteDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CStockDeleteDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStockDeleteRange();
	afx_msg void OnDefaultButton();
	afx_msg void OnStockDeleteAll();
	afx_msg void OnStockDeleteNone();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPrint();
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

public:
	int GetFilterSize() { return m_FilterArray.GetSize(); }
	bool GetDoneDeleteFlag() { return m_bDoneDelete; }

private:
	void OnFindByCode();
	void DisplayScreen ( int nIndex );
	void ToggleFlag();
	void SelectAll ( int nFlag );
	void AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth );

private:
	void SortListInternal( int nType );

private:
	HICON m_hIcon;
	CString m_strDatabaseName;
	CString m_strStockCodeSearch;
	CStockFilterArray m_FilterArray;
	bool m_bStockCodeSearchFromStart;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listStock;	
	int m_nDeletionCount;

private:
	CString m_strTitle;
	bool m_bUnusedMode;

private:
	CStockCodeSearch m_StockCodeSearch;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	bool m_bDoneDelete;
};

/**********************************************************************/
#endif
/**********************************************************************/
