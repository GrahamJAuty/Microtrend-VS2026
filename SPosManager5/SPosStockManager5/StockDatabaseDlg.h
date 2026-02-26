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

class CStockDatabaseDlg : public CDialog
{
public:
	CStockDatabaseDlg( int nDbIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockDatabaseDlg)
	enum { IDD = IDD_STOCK_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void DisplayScreen ( int nIndex );
	void SetReadOnlyFlag( bool b ) { m_bReadOnly = b; }

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CStockDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStockAdd();
	afx_msg void OnStockDeleteCurrent();
	afx_msg void OnStockDeleteRange();
	afx_msg void OnStockEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportProfit();
	afx_msg void OnNoFilterStockRecords();
	afx_msg void OnFilterStockRecords();
	afx_msg void OnStockRangeProgram();
	afx_msg void OnStockExportCsv();
	afx_msg void OnStockImportCsv();
	afx_msg void OnColumnEdit();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnStockRangeUnits();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportsCustom1();
	afx_msg void OnReportsCustom2();
	afx_msg void OnReportsCustom3();
	afx_msg void OnReportsCustom4();
	afx_msg void OnReportsCustom5();
	//}}AFX_MSG
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	afx_msg void OnCancel();	
	DECLARE_MESSAGE_MAP()
	void OnFindByCode();
	void OnFindByDescription();
	
private:
	void BuildRecordList();
	void EditStock( int nIndex );
	void DoCustomListReport( int nRepNum );
	void SetCustomListMenuNames();

private:
	bool ContextMenuSort( CPoint& point );
	void SortListInternal( int nType );
	
private:
	HICON m_hIcon;
	CMenuChanger m_MenuChanger;
	CStockFilterArray m_FilterArray;
	CSSListCtrlVirtual m_listStock;
	bool m_bInitDialog;
	bool m_bModifiedStock;
	CString m_strDbName;
	int m_nApparentComboPos;	
	CWordArray m_wSuppSelectArray;

private:
	CStockCodeSearch m_StockCodeSearch;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	bool m_bReadOnly;
};

/**********************************************************************/
#endif
/**********************************************************************/
