/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once   
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "ManualSalesEditStockDlg.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CManualSalesDatabaseStockDlg : public CDialog
{
public:
	CManualSalesDatabaseStockDlg( int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL );   
	~CManualSalesDatabaseStockDlg();

	void SetPreviousSession( const char* sz ){ m_strPreviousSession = sz; }
	
	//{{AFX_DATA(CManualSalesDatabaseStockDlg)
	enum { IDD = IDD_MANUAL_SALES_DATABASE_STOCK };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CManualSalesDatabaseStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CManualSalesDatabaseStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnImportWrapper();
	afx_msg void OnAdd();
	afx_msg void OnPreview();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnProcessReturn ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialAddItem ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	const char* GetDisplayLine( int nIndex );
	void ImportWrapperInternal( CString& strImportPath, int nAction );

private:
	void AuditSales ( COleDateTime date, bool bUpdateStock );
	void AuditSalesInternal ( COleDateTime date, bool bUpdateStock );
	void WriteSessionFile( bool bDetail );

private:
	void DeleteImportFiles();
	void DeleteSessionFile();

public:
	bool m_bSaveChanges;

private:
	HICON m_hIcon;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listStock;	

private:
	int m_nSpIdx;
	int m_nSpNo;

private:
	CStringArray m_arraySales;
	CStringArray m_ImportFilesArray;
	CString m_strDisplayLine;
	CString m_strTitle;
	CString m_strPreviousSession;

private:
	CSuppRefFinder m_SuppRefFinder;

private:
	CManualSalesEditStockDlg* m_pManualStockSalesEditDlg;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	CStocktakeCSVRecord* m_pStocktake;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
