#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "StockImportFilterArray.h"
/**********************************************************************/

class CStockImportFilterListDlg : public CDialog
{
public:
	CStockImportFilterListDlg( int nSpIdx, int nNodeType, int nAction, CStockImportFilterArray& arrayImport, CWnd* pParent = NULL );   
	
	//{{AFX_DATA(CStockImportFilterListDlg)
	enum { IDD = IDD_STOCK_IMPORT_FILTER_LIST };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockImportFilterListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockImportFilterListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnStockSelectAll();
	afx_msg void OnStockSelectNone();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPrint();
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	void ToggleFlag();
	void SelectAll ( int nFlag );
	void AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth );

private:
	HICON m_hIcon;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listStock;	

private:
	CString m_strTitle;
	
private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	int m_nSpIdx;
	int m_nNodeType;
	int m_nAction;
	CStockImportFilterArray& m_arrayImport;
};

/**********************************************************************/
#endif
/**********************************************************************/
