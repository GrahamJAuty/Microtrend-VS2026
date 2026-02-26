#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
#include "PluMonitorStockman.h"
#include "StockCodeSearch.h"
#include "StockFilterArray.h"
#include "StockpointCSVArray.h"
/**********************************************************************/

class CStockListDlg : public CDialog
{
public:
	CStockListDlg ( CWnd* pParent = NULL );
	CStockListDlg( int nReportType, int nExceptionType, int nSpIdx, CWnd* pParent = NULL);   
	~CStockListDlg();

	//{{AFX_DATA(CStockListDlg)
	enum { IDD = IDD_STOCK_LIST };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//{{AFX_MSG(CStockListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditStockLevel();
	afx_msg void OnCreateImportSheet();
	afx_msg void OnImport();
	afx_msg void OnImportWrapper();
	afx_msg void OnImportSheetFile();
	afx_msg void OnImportSheetFolder();
	afx_msg void OnFilterStockRecords();
	afx_msg void OnNofilterStockRecords();
	afx_msg void OnEditStockLevelRandom();
	afx_msg void OnCopyLevels();
	afx_msg void OnPrintSession();
	afx_msg void OnCopyPlu();
	afx_msg void OnCopySupplier();
	afx_msg void OnRemoveZeros();
	afx_msg void OnRangeActive();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );	
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
	void OnFindByCode();
	
public:
	virtual void SetColumns() = 0;						//returns number of columns
	virtual void EditStockLevel() = 0;	
	virtual const char* GetTitle() = 0;
	virtual void SetFilterArray() = 0;
	virtual void PrepareMenu() = 0;
	virtual const char* GetSpName();
	virtual void EditStockLevelRandom(){}
	virtual void AppendAudit(){};
	virtual void RemoveAudit();
	virtual void HandleCancel();
	virtual void HandleImport(){};
	virtual void HandleImportSheetFile(){};
	virtual void HandleImportSheetFolder(){};
	virtual void HandleImportWrapper(){};
	virtual void HandlePrintSession(){};
	virtual void HandleCopyLevels(){}
	virtual void HandleCopySupplierStockCodes(){}
	virtual void HandleCopyPluNumbers(){}
	virtual void HandleRemoveStockZeros(){}
	virtual void HandleRangeActive(){}
	virtual void HandleCreateImportSheet(){}

public:
	bool GetStockWriteErrorFlag() { return m_bStockWriteError; }

protected:
	void BuildRecordList();
	virtual void DisplayScreen ( int nIndex );

private:
	void SortListInternal( int nType );

protected:
	CMenuChanger m_MenuChanger;
	HICON m_hIcon;
	CStockFilterArray* m_pFilterArray;
	CSSListCtrlVirtual m_listStock;
	bool m_bInitDialog;
	bool m_bImported;
	bool m_bStockWriteError;
	CStringArray m_ImportFilesArray;
	CString m_strSpName;
	int m_nExceptionType;
	int m_nReportType;
	int m_nCategory;
	int m_nSpIdx;

protected:
	CColumnManager m_ColumnManager;
	CStockCodeSearch m_StockCodeSearch;

private:
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

/**********************************************************************/
#endif		//__STOCKLISTDLG_H__
/**********************************************************************/
